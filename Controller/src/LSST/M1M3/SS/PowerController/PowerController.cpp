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
#include <SAL_m1m3C.h>
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

	memset(this->powerSupplyData, 0, sizeof(m1m3_PowerSupplyDataC));
	memset(this->powerStatus, 0, sizeof(m1m3_logevent_PowerStatusC));
	memset(this->powerSupplyStatus, 0, sizeof(m1m3_logevent_PowerSupplyStatusC));
	memset(this->powerWarning, 0, sizeof(m1m3_logevent_PowerWarningC));
}

void PowerController::processData() {
	// TODO: Handle no data available
	// TODO: Handle limits, push to safety controller
	Log.Trace("PowerController: processData()");
	if (this->fpgaData->PowerSupplyTimestamp != this->lastPowerTimestamp) {
		this->lastPowerTimestamp = this->fpgaData->PowerSupplyTimestamp;
		double timestamp = Timestamp::fromFPGA(this->fpgaData->PowerSupplyTimestamp);
		this->powerStatus->Timestamp = timestamp;
		this->powerStatus->AuxPowerNetworkAOutputOn = (this->fpgaData->PowerSupplyStates & 0x01) != 0;
		this->powerStatus->AuxPowerNetworkBOutputOn = (this->fpgaData->PowerSupplyStates & 0x02) != 0;
		this->powerStatus->AuxPowerNetworkCOutputOn = (this->fpgaData->PowerSupplyStates & 0x04) != 0;
		this->powerStatus->AuxPowerNetworkDOutputOn = (this->fpgaData->PowerSupplyStates & 0x08) != 0;
		this->powerStatus->PowerNetworkAOutputOn = (this->fpgaData->PowerSupplyStates & 0x10) != 0;
		this->powerStatus->PowerNetworkBOutputOn = (this->fpgaData->PowerSupplyStates & 0x20) != 0;
		this->powerStatus->PowerNetworkCOutputOn = (this->fpgaData->PowerSupplyStates & 0x40) != 0;
		this->powerStatus->PowerNetworkDOutputOn = (this->fpgaData->PowerSupplyStates & 0x80) != 0;
		this->publisher->tryLogPowerStatus();
		this->powerWarning->Timestamp = timestamp;
		this->powerWarning->AuxPowerNetworkAOutputMismatch = this->powerStatus->AuxPowerNetworkACommandedOn != this->powerStatus->AuxPowerNetworkAOutputOn;
		this->powerWarning->AuxPowerNetworkBOutputMismatch = this->powerStatus->AuxPowerNetworkBCommandedOn != this->powerStatus->AuxPowerNetworkBOutputOn;
		this->powerWarning->AuxPowerNetworkCOutputMismatch = this->powerStatus->AuxPowerNetworkCCommandedOn != this->powerStatus->AuxPowerNetworkCOutputOn;
		this->powerWarning->AuxPowerNetworkDOutputMismatch = this->powerStatus->AuxPowerNetworkDCommandedOn != this->powerStatus->AuxPowerNetworkDOutputOn;
		this->powerWarning->PowerNetworkAOutputMismatch = this->powerStatus->PowerNetworkACommandedOn != this->powerStatus->PowerNetworkAOutputOn;
		this->powerWarning->PowerNetworkBOutputMismatch = this->powerStatus->PowerNetworkBCommandedOn != this->powerStatus->PowerNetworkBOutputOn;
		this->powerWarning->PowerNetworkCOutputMismatch = this->powerStatus->PowerNetworkCCommandedOn != this->powerStatus->PowerNetworkCOutputOn;
		this->powerWarning->PowerNetworkDOutputMismatch = this->powerStatus->PowerNetworkDCommandedOn != this->powerStatus->PowerNetworkDOutputOn;
		this->publisher->tryLogPowerWarning();
	}
	double timestamp = this->publisher->getTimestamp();
	this->expansionFPGA->sample();
	float sample[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->expansionFPGA->readSlot1(sample);
	this->powerSupplyData->Timestamp = timestamp;
	this->powerSupplyData->PowerNetworkACurrent = sample[0];
	this->powerSupplyData->PowerNetworkBCurrent = sample[1];
	this->powerSupplyData->PowerNetworkCCurrent = sample[2];
	this->powerSupplyData->PowerNetworkDCurrent = sample[3];
	this->powerSupplyData->LightPowerNetworkCurrent = sample[4];
	this->powerSupplyData->ControlsPowerNetworkCurrent = sample[5];
	this->publisher->putPowerSupplyData();
}

void PowerController::setBothPowerNetworks(bool on) {
	Log.Info("PowerController: setBothPowerNetworks(%d)", on);
	this->powerStatus->PowerNetworkACommandedOn = on;
	this->powerStatus->PowerNetworkBCommandedOn = on;
	this->powerStatus->PowerNetworkCCommandedOn = on;
	this->powerStatus->PowerNetworkDCommandedOn = on;
	this->powerStatus->AuxPowerNetworkACommandedOn = on;
	this->powerStatus->AuxPowerNetworkBCommandedOn = on;
	this->powerStatus->AuxPowerNetworkCCommandedOn = on;
	this->powerStatus->AuxPowerNetworkDCommandedOn = on;
	uint16_t buffer[16] = {
			FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->PowerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->PowerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->PowerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->PowerNetworkDCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->AuxPowerNetworkACommandedOn,
			FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->AuxPowerNetworkBCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->AuxPowerNetworkCCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->AuxPowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 16, 0);
}

void PowerController::setAllPowerNetworks(bool on) {
	Log.Info("PowerController: setAllPowerNetworks(%d)", on);
	this->powerStatus->PowerNetworkACommandedOn = on;
	this->powerStatus->PowerNetworkBCommandedOn = on;
	this->powerStatus->PowerNetworkCCommandedOn = on;
	this->powerStatus->PowerNetworkDCommandedOn = on;
	uint16_t buffer[8] = {
			FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->PowerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->PowerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->PowerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->PowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 8, 0);
}

void PowerController::setPowerNetworkA(bool on) {
	Log.Info("PowerController: setPowerNetworkA(%d)", on);
	this->powerStatus->PowerNetworkACommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->PowerNetworkACommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setPowerNetworkB(bool on) {
	Log.Info("PowerController: setPowerNetworkB(%d)", on);
	this->powerStatus->PowerNetworkBCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->PowerNetworkBCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setPowerNetworkC(bool on) {
	Log.Info("PowerController: setPowerNetworkC(%d)", on);
	this->powerStatus->PowerNetworkCCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->PowerNetworkCCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setPowerNetworkD(bool on) {
	Log.Info("PowerController: setPowerNetworkD(%d)", on);
	this->powerStatus->PowerNetworkDCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->PowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setAllAuxPowerNetworks(bool on) {
	Log.Info("PowerController: setAllAuxPowerNetworks(%d)", on);
	this->powerStatus->AuxPowerNetworkACommandedOn = on;
	this->powerStatus->AuxPowerNetworkBCommandedOn = on;
	this->powerStatus->AuxPowerNetworkCCommandedOn = on;
	this->powerStatus->AuxPowerNetworkDCommandedOn = on;
	uint16_t buffer[8] = {
				FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->AuxPowerNetworkACommandedOn,
				FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->AuxPowerNetworkBCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->AuxPowerNetworkCCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->AuxPowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 8, 0);
}

void PowerController::setAuxPowerNetworkA(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkA(%d)", on);
	this->powerStatus->AuxPowerNetworkACommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->AuxPowerNetworkACommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setAuxPowerNetworkB(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkB(%d)", on);
	this->powerStatus->AuxPowerNetworkBCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->AuxPowerNetworkBCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setAuxPowerNetworkC(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkC(%d)", on);
	this->powerStatus->AuxPowerNetworkCCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->AuxPowerNetworkCCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void PowerController::setAuxPowerNetworkD(bool on) {
	Log.Info("PowerController: setAuxPowerNetworkD(%d)", on);
	this->powerStatus->AuxPowerNetworkDCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->AuxPowerNetworkDCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
