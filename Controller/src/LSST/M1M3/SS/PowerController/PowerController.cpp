/*
 * PowerController.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#include <PowerController.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <IExpansionFPGA.h>
#include <IPublisher.h>
#include <ISafetyController.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PowerController::PowerController(IPublisher* publisher, IFPGA* fpga, IExpansionFPGA* expansionFPGA, ISafetyController* safetyController) {
	this->publisher = publisher;
	this->fpga = fpga;
	this->expansionFPGA = expansionFPGA;
	this->safetyController = safetyController;
	this->previousSlot2Sample = 0;
	this->powerSupplyData = this->publisher->getPowerSupplyData();
	this->powerStatus = this->publisher->getEventPowerStatus();
	this->powerSupplyStatus = this->publisher->getEventPowerSupplyStatus();
	this->powerWarning = this->publisher->getEventPowerWarning();
}

void PowerController::checkPowerStatus() {
	int previousStatus =
			(this->powerStatus->PowerNetworkAOutputOn ? 1 : 0) |
			(this->powerStatus->PowerNetworkBOutputOn ? 2 : 0) |
			(this->powerStatus->PowerNetworkCOutputOn ? 4 : 0) |
			(this->powerStatus->PowerNetworkDOutputOn ? 8 : 0) |
			(this->powerStatus->AuxPowerNetworkAOutputOn ? 16 : 0) |
			(this->powerStatus->AuxPowerNetworkBOutputOn ? 32 : 0) |
			(this->powerStatus->AuxPowerNetworkCOutputOn ? 64 : 0) |
			(this->powerStatus->AuxPowerNetworkDOutputOn ? 128 : 0);
	this->checkPowerNetworkAOutput();
	this->checkPowerNetworkBOutput();
	this->checkPowerNetworkCOutput();
	this->checkPowerNetworkDOutput();
	this->checkAuxPowerNetworkAOutput();
	this->checkAuxPowerNetworkBOutput();
	this->checkAuxPowerNetworkCOutput();
	this->checkAuxPowerNetworkDOutput();
	int currentStatus =
			(this->powerStatus->PowerNetworkAOutputOn ? 1 : 0) |
			(this->powerStatus->PowerNetworkBOutputOn ? 2 : 0) |
			(this->powerStatus->PowerNetworkCOutputOn ? 4 : 0) |
			(this->powerStatus->PowerNetworkDOutputOn ? 8 : 0) |
			(this->powerStatus->AuxPowerNetworkAOutputOn ? 16 : 0) |
			(this->powerStatus->AuxPowerNetworkBOutputOn ? 32 : 0) |
			(this->powerStatus->AuxPowerNetworkCOutputOn ? 64 : 0) |
			(this->powerStatus->AuxPowerNetworkDOutputOn ? 128 : 0);
	bool publishStatus = currentStatus != previousStatus;
	bool publishWarning = this->isChangeInPowerNetworkAOutputMismatch() ||
			this->isChangeInPowerNetworkBOutputMismatch() ||
			this->isChangeInPowerNetworkCOutputMismatch() ||
			this->isChangeInPowerNetworkDOutputMismatch() ||
			this->isChangeInAuxPowerNetworkAOutputMismatch() ||
			this->isChangeInAuxPowerNetworkBOutputMismatch() ||
			this->isChangeInAuxPowerNetworkCOutputMismatch() ||
			this->isChangeInAuxPowerNetworkDOutputMismatch();
	if (publishStatus) {
		this->publishStatus();
	}
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::samplePowerSupplyDataAndStatus() {
	this->expansionSampleTime = this->publisher->getTimestamp();
	this->expansionFPGA->sample();
}

void PowerController::publishPowerSupplyData() {
	float sample[6];
	this->expansionFPGA->readSlot1(sample);
	this->powerSupplyData->Timestamp = this->expansionSampleTime;
	this->powerSupplyData->PowerNetworkACurrent = sample[0];
	this->powerSupplyData->PowerNetworkBCurrent = sample[1];
	this->powerSupplyData->PowerNetworkCCurrent = sample[2];
	this->powerSupplyData->PowerNetworkDCurrent = sample[3];
	this->powerSupplyData->LightPowerNetworkCurrent = sample[4];
	this->powerSupplyData->ControlsPowerNetworkCurrent = sample[5];
	this->publisher->putPowerSupplyData();
}

void PowerController::publishPowerSupplyStatusIfRequired() {
	uint32_t data;
	this->expansionFPGA->readSlot2(&data);
	if (data != this->previousSlot2Sample) {
		this->previousSlot2Sample = data;
		this->powerSupplyStatus->Timestamp = this->expansionSampleTime;
		this->powerSupplyStatus->RCPMirrorCellUtility220VAC1Status =           (data >>  0) & 1;
		this->powerSupplyStatus->RCPCabinetUtility220VACStatus =               (data >>  1) & 1;
		this->powerSupplyStatus->RCPExternalEquipment220VACStatus =            (data >>  2) & 1;
		this->powerSupplyStatus->RCPMirrorCellUtility220VAC2Status =           (data >>  3) & 1;
		this->powerSupplyStatus->RCPMirrorCellUtility220VAC3Status =           (data >>  4) & 1;
		this->powerSupplyStatus->PowerNetworkARedundancyControlStatus =        (data >>  5) & 1;
		this->powerSupplyStatus->PowerNetworkBRedundancyControlStatus =        (data >>  6) & 1;
		this->powerSupplyStatus->PowerNetworkCRedundancyControlStatus =        (data >>  7) & 1;
		this->powerSupplyStatus->PowerNetworkDRedundancyControlStatus =        (data >>  8) & 1;
		this->powerSupplyStatus->ControlsPowerNetworkRedundancyControlStatus = (data >>  9) & 1;
		this->powerSupplyStatus->PowerNetworkAStatus =                         (data >> 11) & 1;
		this->powerSupplyStatus->PowerNetworkARedundantStatus =                (data >> 12) & 1;
		this->powerSupplyStatus->PowerNetworkBStatus =                         (data >> 13) & 1;
		this->powerSupplyStatus->PowerNetworkBRedundantStatus =                (data >> 14) & 1;
		this->powerSupplyStatus->PowerNetworkCStatus =                         (data >> 15) & 1;
		this->powerSupplyStatus->PowerNetworkCRedundantStatus =                (data >> 16) & 1;
		this->powerSupplyStatus->PowerNetworkDStatus =                         (data >> 17) & 1;
		this->powerSupplyStatus->PowerNetworkDRedundantStatus =                (data >> 18) & 1;
		this->powerSupplyStatus->ControlsPowerNetworkStatus =                  (data >> 19) & 1;
		this->powerSupplyStatus->ControlsPowerNetworkRedundantStatus =         (data >> 20) & 1;
		this->powerSupplyStatus->LightPowerNetworkStatus =                     (data >> 21) & 1;
		this->powerSupplyStatus->ExternalEquipmentPowerNetworkStatus =         (data >> 22) & 1;
		this->powerSupplyStatus->LaserTrackerPowerNetworkStatus =              (data >> 23) & 1;
		this->publisher->logPowerSupplyStatus();
	}
}

void PowerController::setBothPowerNetworks(bool on) {
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
	this->sendCommand(buffer, 16);
	this->checkPowerNetworkAOutput();
	this->checkPowerNetworkBOutput();
	this->checkPowerNetworkCOutput();
	this->checkPowerNetworkDOutput();
	this->checkAuxPowerNetworkAOutput();
	this->checkAuxPowerNetworkBOutput();
	this->checkAuxPowerNetworkCOutput();
	this->checkAuxPowerNetworkDOutput();

	bool publishWarning = this->isChangeInPowerNetworkAOutputMismatch() ||
			this->isChangeInPowerNetworkBOutputMismatch() ||
			this->isChangeInPowerNetworkCOutputMismatch() ||
			this->isChangeInPowerNetworkDOutputMismatch() ||
			this->isChangeInAuxPowerNetworkAOutputMismatch() ||
			this->isChangeInAuxPowerNetworkBOutputMismatch() ||
			this->isChangeInAuxPowerNetworkCOutputMismatch() ||
			this->isChangeInAuxPowerNetworkDOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setAllPowerNetworks(bool on) {
	this->powerStatus->PowerNetworkACommandedOn = on;
	this->powerStatus->PowerNetworkBCommandedOn = on;
	this->powerStatus->PowerNetworkCCommandedOn = on;
	this->powerStatus->PowerNetworkDCommandedOn = on;
	uint16_t buffer[8] = {
			FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->PowerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->PowerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->PowerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->PowerNetworkDCommandedOn };
	this->sendCommand(buffer, 8);
	this->checkPowerNetworkAOutput();
	this->checkPowerNetworkBOutput();
	this->checkPowerNetworkCOutput();
	this->checkPowerNetworkDOutput();

	bool publishWarning = this->isChangeInPowerNetworkAOutputMismatch() ||
			this->isChangeInPowerNetworkBOutputMismatch() ||
			this->isChangeInPowerNetworkCOutputMismatch() ||
			this->isChangeInPowerNetworkDOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setPowerNetworkA(bool on) {
	this->powerStatus->PowerNetworkACommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkAOn, (uint16_t)this->powerStatus->PowerNetworkACommandedOn };
	this->sendCommand(buffer, 2);
	this->checkPowerNetworkAOutput();

	bool publishWarning = this->isChangeInPowerNetworkAOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setPowerNetworkB(bool on) {
	this->powerStatus->PowerNetworkBCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkBOn, (uint16_t)this->powerStatus->PowerNetworkBCommandedOn };
	this->sendCommand(buffer, 2);
	this->checkPowerNetworkBOutput();

	bool publishWarning = this->isChangeInPowerNetworkBOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setPowerNetworkC(bool on) {
	this->powerStatus->PowerNetworkCCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkCOn, (uint16_t)this->powerStatus->PowerNetworkCCommandedOn };
	this->sendCommand(buffer, 2);
	this->checkPowerNetworkCOutput();

	bool publishWarning = this->isChangeInPowerNetworkCOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setPowerNetworkD(bool on) {
	this->powerStatus->PowerNetworkDCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkDOn, (uint16_t)this->powerStatus->PowerNetworkDCommandedOn };
	this->sendCommand(buffer, 2);
	this->checkPowerNetworkDOutput();

	bool publishWarning = this->isChangeInPowerNetworkDOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setAllAuxPowerNetworks(bool on) {
	this->powerStatus->AuxPowerNetworkACommandedOn = on;
	this->powerStatus->AuxPowerNetworkBCommandedOn = on;
	this->powerStatus->AuxPowerNetworkCCommandedOn = on;
	this->powerStatus->AuxPowerNetworkDCommandedOn = on;
	uint16_t buffer[8] = {
				FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->AuxPowerNetworkACommandedOn,
				FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->AuxPowerNetworkBCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->AuxPowerNetworkCCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->AuxPowerNetworkDCommandedOn };
	this->sendCommand(buffer, 8);
	this->checkAuxPowerNetworkAOutput();
	this->checkAuxPowerNetworkBOutput();
	this->checkAuxPowerNetworkCOutput();
	this->checkAuxPowerNetworkDOutput();

	bool publishWarning = this->isChangeInAuxPowerNetworkAOutputMismatch() ||
			this->isChangeInAuxPowerNetworkBOutputMismatch() ||
			this->isChangeInAuxPowerNetworkCOutputMismatch() ||
			this->isChangeInAuxPowerNetworkDOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setAuxPowerNetworkA(bool on) {
	this->powerStatus->AuxPowerNetworkACommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)this->powerStatus->AuxPowerNetworkACommandedOn };
	this->sendCommand(buffer, 2);
	this->checkAuxPowerNetworkAOutput();

	bool publishWarning = this->isChangeInAuxPowerNetworkAOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setAuxPowerNetworkB(bool on) {
	this->powerStatus->AuxPowerNetworkBCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)this->powerStatus->AuxPowerNetworkBCommandedOn };
	this->sendCommand(buffer, 2);
	this->checkAuxPowerNetworkBOutput();

	bool publishWarning = this->isChangeInAuxPowerNetworkBOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setAuxPowerNetworkC(bool on) {
	this->powerStatus->AuxPowerNetworkCCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)this->powerStatus->AuxPowerNetworkCCommandedOn };
	this->sendCommand(buffer, 2);
	this->checkAuxPowerNetworkCOutput();

	bool publishWarning = this->isChangeInAuxPowerNetworkCOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::setAuxPowerNetworkD(bool on) {
	this->powerStatus->AuxPowerNetworkDCommandedOn = on;
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)this->powerStatus->AuxPowerNetworkDCommandedOn };
	this->sendCommand(buffer, 2);
	this->checkAuxPowerNetworkDOutput();

	bool publishWarning = this->isChangeInAuxPowerNetworkDOutputMismatch();
	this->publishStatus();
	if (publishWarning) {
		this->publishWarning();
	}
}

void PowerController::checkPowerNetworkAOutput() {
	this->powerStatus->PowerNetworkAOutputOn = this->checkStatus(FPGAAddresses::DCPowerNetworkAOn);
}

void PowerController::checkPowerNetworkBOutput() {
	this->powerStatus->PowerNetworkBOutputOn = this->checkStatus(FPGAAddresses::DCPowerNetworkBOn);
}

void PowerController::checkPowerNetworkCOutput() {
	this->powerStatus->PowerNetworkCOutputOn = this->checkStatus(FPGAAddresses::DCPowerNetworkCOn);
}

void PowerController::checkPowerNetworkDOutput() {
	this->powerStatus->PowerNetworkDOutputOn = this->checkStatus(FPGAAddresses::DCPowerNetworkDOn);
}

void PowerController::checkAuxPowerNetworkAOutput() {
	this->powerStatus->AuxPowerNetworkAOutputOn = this->checkStatus(FPGAAddresses::DCAuxPowerNetworkAOn);
}

void PowerController::checkAuxPowerNetworkBOutput() {
	this->powerStatus->AuxPowerNetworkBOutputOn = this->checkStatus(FPGAAddresses::DCAuxPowerNetworkBOn);
}

void PowerController::checkAuxPowerNetworkCOutput() {
	this->powerStatus->AuxPowerNetworkCOutputOn = this->checkStatus(FPGAAddresses::DCAuxPowerNetworkCOn);
}

void PowerController::checkAuxPowerNetworkDOutput() {
	this->powerStatus->AuxPowerNetworkDOutputOn = this->checkStatus(FPGAAddresses::DCAuxPowerNetworkDOn);
}

bool PowerController::isChangeInPowerNetworkAOutputMismatch() {
	bool previousState = this->powerWarning->PowerNetworkAOutputMismatch;
	this->powerWarning->PowerNetworkAOutputMismatch = this->powerStatus->PowerNetworkACommandedOn != this->powerStatus->PowerNetworkAOutputOn;
	this->safetyController->powerControllerNotifyPowerNetworkAOutputMismatch(this->powerWarning->PowerNetworkAOutputMismatch);
	return this->powerWarning->PowerNetworkAOutputMismatch != previousState;
}

bool PowerController::isChangeInPowerNetworkBOutputMismatch() {
	bool previousState = this->powerWarning->PowerNetworkBOutputMismatch;
	this->powerWarning->PowerNetworkBOutputMismatch = this->powerStatus->PowerNetworkBCommandedOn != this->powerStatus->PowerNetworkBOutputOn;
	this->safetyController->powerControllerNotifyPowerNetworkBOutputMismatch(this->powerWarning->PowerNetworkBOutputMismatch);
	return this->powerWarning->PowerNetworkBOutputMismatch != previousState;
}

bool PowerController::isChangeInPowerNetworkCOutputMismatch() {
	bool previousState = this->powerWarning->PowerNetworkCOutputMismatch;
	this->powerWarning->PowerNetworkCOutputMismatch = this->powerStatus->PowerNetworkCCommandedOn != this->powerStatus->PowerNetworkCOutputOn;
	this->safetyController->powerControllerNotifyPowerNetworkCOutputMismatch(this->powerWarning->PowerNetworkCOutputMismatch);
	return this->powerWarning->PowerNetworkCOutputMismatch != previousState;
}

bool PowerController::isChangeInPowerNetworkDOutputMismatch() {
	bool previousState = this->powerWarning->PowerNetworkDOutputMismatch;
	this->powerWarning->PowerNetworkDOutputMismatch = this->powerStatus->PowerNetworkDCommandedOn != this->powerStatus->PowerNetworkDOutputOn;
	this->safetyController->powerControllerNotifyPowerNetworkDOutputMismatch(this->powerWarning->PowerNetworkDOutputMismatch);
	return this->powerWarning->PowerNetworkDOutputMismatch != previousState;
}

bool PowerController::isChangeInAuxPowerNetworkAOutputMismatch() {
	bool previousState = this->powerWarning->AuxPowerNetworkAOutputMismatch;
	this->powerWarning->AuxPowerNetworkAOutputMismatch = this->powerStatus->AuxPowerNetworkACommandedOn != this->powerStatus->AuxPowerNetworkAOutputOn;
	this->safetyController->powerControllerNotifyAuxPowerNetworkAOutputMismatch(this->powerWarning->AuxPowerNetworkAOutputMismatch);
	return this->powerWarning->AuxPowerNetworkAOutputMismatch != previousState;
}

bool PowerController::isChangeInAuxPowerNetworkBOutputMismatch() {
	bool previousState = this->powerWarning->AuxPowerNetworkBOutputMismatch;
	this->powerWarning->AuxPowerNetworkBOutputMismatch = this->powerStatus->AuxPowerNetworkBCommandedOn != this->powerStatus->AuxPowerNetworkBOutputOn;
	this->safetyController->powerControllerNotifyAuxPowerNetworkBOutputMismatch(this->powerWarning->AuxPowerNetworkBOutputMismatch);
	return this->powerWarning->AuxPowerNetworkBOutputMismatch != previousState;
}

bool PowerController::isChangeInAuxPowerNetworkCOutputMismatch() {
	bool previousState = this->powerWarning->AuxPowerNetworkCOutputMismatch;
	this->powerWarning->AuxPowerNetworkCOutputMismatch = this->powerStatus->AuxPowerNetworkCCommandedOn != this->powerStatus->AuxPowerNetworkCOutputOn;
	this->safetyController->powerControllerNotifyAuxPowerNetworkCOutputMismatch(this->powerWarning->AuxPowerNetworkCOutputMismatch);
	return this->powerWarning->AuxPowerNetworkCOutputMismatch != previousState;
}

bool PowerController::isChangeInAuxPowerNetworkDOutputMismatch() {
	bool previousState = this->powerWarning->AuxPowerNetworkDOutputMismatch;
	this->powerWarning->AuxPowerNetworkDOutputMismatch = this->powerStatus->AuxPowerNetworkDCommandedOn != this->powerStatus->AuxPowerNetworkDOutputOn;
	this->safetyController->powerControllerNotifyAuxPowerNetworkDOutputMismatch(this->powerWarning->AuxPowerNetworkDOutputMismatch);
	return this->powerWarning->AuxPowerNetworkDOutputMismatch != previousState;
}

void PowerController::sendCommand(uint16_t* buffer, int32_t length) {
	this->fpga->writeCommandFIFO(buffer, length, 0);
}

bool PowerController::checkStatus(uint16_t address) {
	this->fpga->writeRequestFIFO(address, 0);
	this->fpga->readU16ResponseFIFO(this->rxBuffer, 1, 10);
	return this->rxBuffer[0] != 0;
}

void PowerController::publishStatus() {
	this->powerStatus->Timestamp = this->publisher->getTimestamp();
	this->publisher->logPowerStatus();
}

void PowerController::publishWarning() {
	this->powerWarning->Timestamp = this->publisher->getTimestamp();
	this->powerWarning->AnyWarning =
			this->powerWarning->PowerNetworkAOutputMismatch ||
			this->powerWarning->PowerNetworkBOutputMismatch ||
			this->powerWarning->PowerNetworkCOutputMismatch ||
			this->powerWarning->PowerNetworkDOutputMismatch ||
			this->powerWarning->AuxPowerNetworkAOutputMismatch ||
			this->powerWarning->AuxPowerNetworkBOutputMismatch ||
			this->powerWarning->AuxPowerNetworkCOutputMismatch ||
			this->powerWarning->AuxPowerNetworkDOutputMismatch;
	this->publisher->logPowerWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
