/*
 * PowerController.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#include <PowerController.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <ExpansionFPGA.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <Log.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

PowerController::PowerController(FPGA* fpga, ExpansionFPGA* expansionFPGA, M1M3SSPublisher* publisher, SafetyController* safetyController) {
	Log.Debug("PowerController: PowerController()");
	this->fpga = fpga;
	this->fpgaData = this->fpga->getSupportFPGAData();
	this->expansionFPGA = expansionFPGA;
	this->publisher = publisher;
	this->safetyController = safetyController;

	this->powerSupplyData = this->publisher->getPowerSupplyData();
	this->powerStatus = this->publisher->getEventPowerStatus();
	this->powerSupplyStatus = this->publisher->getEventPowerSupplyStatus();
	this->powerWarning = this->publisher->getEventPowerWarning();

	this->lastPowerTimestamp = 0;

	memset(this->powerSupplyData, 0, sizeof(MTM1M3_powerSupplyDataC));
	memset(this->powerStatus, 0, sizeof(MTM1M3_logevent_powerStatusC));
	memset(this->powerSupplyStatus, 0, sizeof(MTM1M3_logevent_powerSupplyStatusC));
	memset(this->powerWarning, 0, sizeof(MTM1M3_logevent_powerWarningC));
}

void PowerController::processData() {
	// TODO: Handle no data available
	// TODO: Handle limits, push to safety controller
	Log.Trace("PowerController: processData()");
	if (this->fpgaData->PowerSupplyTimestamp != this->lastPowerTimestamp) {
		this->lastPowerTimestamp = this->fpgaData->PowerSupplyTimestamp;
		double timestamp = Timestamp::fromFPGA(this->fpgaData->PowerSupplyTimestamp);
		this->powerStatus->timestamp = timestamp;
		this->powerStatus->auxPowerNetworkAOutputOn = (this->fpgaData->PowerSupplyStates & 0x01) != 0;
		this->powerStatus->auxPowerNetworkBOutputOn = (this->fpgaData->PowerSupplyStates & 0x02) != 0;
		this->powerStatus->auxPowerNetworkCOutputOn = (this->fpgaData->PowerSupplyStates & 0x04) != 0;
		this->powerStatus->auxPowerNetworkDOutputOn = (this->fpgaData->PowerSupplyStates & 0x08) != 0;
		this->powerStatus->powerNetworkAOutputOn = (this->fpgaData->PowerSupplyStates & 0x10) != 0;
		this->powerStatus->powerNetworkBOutputOn = (this->fpgaData->PowerSupplyStates & 0x20) != 0;
		this->powerStatus->powerNetworkCOutputOn = (this->fpgaData->PowerSupplyStates & 0x40) != 0;
		this->powerStatus->powerNetworkDOutputOn = (this->fpgaData->PowerSupplyStates & 0x80) != 0;
		this->publisher->tryLogPowerStatus();
		this->powerWarning->timestamp = timestamp;
		this->powerWarning->auxPowerNetworkAOutputMismatch = this->powerStatus->auxPowerNetworkACommandedOn != this->powerStatus->auxPowerNetworkAOutputOn;
		this->powerWarning->auxPowerNetworkBOutputMismatch = this->powerStatus->auxPowerNetworkBCommandedOn != this->powerStatus->auxPowerNetworkBOutputOn;
		this->powerWarning->auxPowerNetworkCOutputMismatch = this->powerStatus->auxPowerNetworkCCommandedOn != this->powerStatus->auxPowerNetworkCOutputOn;
		this->powerWarning->auxPowerNetworkDOutputMismatch = this->powerStatus->auxPowerNetworkDCommandedOn != this->powerStatus->auxPowerNetworkDOutputOn;
		this->powerWarning->powerNetworkAOutputMismatch = this->powerStatus->powerNetworkACommandedOn != this->powerStatus->powerNetworkAOutputOn;
		this->powerWarning->powerNetworkBOutputMismatch = this->powerStatus->powerNetworkBCommandedOn != this->powerStatus->powerNetworkBOutputOn;
		this->powerWarning->powerNetworkCOutputMismatch = this->powerStatus->powerNetworkCCommandedOn != this->powerStatus->powerNetworkCOutputOn;
		this->powerWarning->powerNetworkDOutputMismatch = this->powerStatus->powerNetworkDCommandedOn != this->powerStatus->powerNetworkDOutputOn;
		this->publisher->tryLogPowerWarning();
	}
	double timestamp = this->publisher->getTimestamp();
	this->expansionFPGA->sample();
	float sample[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->expansionFPGA->readSlot1(sample);
	this->powerSupplyData->timestamp = timestamp;
	this->powerSupplyData->powerNetworkACurrent = sample[0];
	this->powerSupplyData->powerNetworkBCurrent = sample[1];
	this->powerSupplyData->powerNetworkCCurrent = sample[2];
	this->powerSupplyData->powerNetworkDCurrent = sample[3];
	this->powerSupplyData->lightPowerNetworkCurrent = sample[4];
	this->powerSupplyData->controlsPowerNetworkCurrent = sample[5];
	this->publisher->putPowerSupplyData();
}

void PowerController::setBothPowerNetworks(bool on) {
	Log.Info("PowerController: setBothPowerNetworks(%d)", on);
	this->powerStatus->powerNetworkACommandedOn = on;
	this->powerStatus->powerNetworkBCommandedOn = on;
	this->powerStatus->powerNetworkCCommandedOn = on;
	this->powerStatus->powerNetworkDCommandedOn = on;
	this->powerStatus->auxPowerNetworkACommandedOn = on;
	this->powerStatus->auxPowerNetworkBCommandedOn = on;
	this->powerStatus->auxPowerNetworkCCommandedOn = on;
	this->powerStatus->auxPowerNetworkDCommandedOn = on;
	uint16_t buffer[16] = {
			FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->powerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->powerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->powerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->powerNetworkDCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->auxPowerNetworkACommandedOn,
			FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->auxPowerNetworkBCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->auxPowerNetworkCCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->auxPowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 16, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setAllPowerNetworks(bool on) {
	Log.Info("PowerController: setAllPowerNetworks(%d)", on);
	this->powerStatus->powerNetworkACommandedOn = on;
	this->powerStatus->powerNetworkBCommandedOn = on;
	this->powerStatus->powerNetworkCCommandedOn = on;
	this->powerStatus->powerNetworkDCommandedOn = on;
	uint16_t buffer[8] = {
			FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->powerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->powerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->powerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->powerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 8, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkA(bool on) {
	Log.Info("PowerController: setPowerNetworkA(%d)", on);
	this->powerStatus->powerNetworkACommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->powerNetworkACommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkB(bool on) {
	Log.Info("PowerController: setPowerNetworkB(%d)", on);
	this->powerStatus->powerNetworkBCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->powerNetworkBCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkC(bool on) {
	Log.Info("PowerController: setPowerNetworkC(%d)", on);
	this->powerStatus->powerNetworkCCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->powerNetworkCCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkD(bool on) {
	Log.Info("PowerController: setPowerNetworkD(%d)", on);
	this->powerStatus->powerNetworkDCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->powerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setAllAuxPowerNetworks(bool on) {
	Log.Info("PowerController: setAllAuxPowerNetworks(%d)", on);
	this->powerStatus->auxPowerNetworkACommandedOn = on;
	this->powerStatus->auxPowerNetworkBCommandedOn = on;
	this->powerStatus->auxPowerNetworkCCommandedOn = on;
	this->powerStatus->auxPowerNetworkDCommandedOn = on;
	uint16_t buffer[8] = {
				FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->auxPowerNetworkACommandedOn,
				FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->auxPowerNetworkBCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->auxPowerNetworkCCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->auxPowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 8, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkA(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkA(%d)", on);
	this->powerStatus->auxPowerNetworkACommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->auxPowerNetworkACommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkB(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkB(%d)", on);
	this->powerStatus->auxPowerNetworkBCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->auxPowerNetworkBCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkC(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkC(%d)", on);
	this->powerStatus->auxPowerNetworkCCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->auxPowerNetworkCCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkD(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkD(%d)", on);
	this->powerStatus->auxPowerNetworkDCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->auxPowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
	this->publisher->tryLogPowerStatus();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
