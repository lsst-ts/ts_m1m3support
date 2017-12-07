/*
 * PowerController.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#include <PowerController.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <IPublisher.h>
#include <ISafetyController.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PowerController::PowerController(IPublisher* publisher, IFPGA* fpga, ISafetyController* safetyController) {
	this->publisher = publisher;
	this->fpga = fpga;
	this->safetyController = safetyController;
	this->powerStatus = this->publisher->getEventPowerStatus();
	this->powerWarning = this->publisher->getEventPowerWarning();
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
			FPGAAddresses::DCPowerNetworkAOn, this->powerStatus->PowerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, this->powerStatus->PowerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, this->powerStatus->PowerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, this->powerStatus->PowerNetworkDCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkAOn, this->powerStatus->AuxPowerNetworkACommandedOn,
			FPGAAddresses::DCAuxPowerNetworkBOn, this->powerStatus->AuxPowerNetworkBCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkCOn, this->powerStatus->AuxPowerNetworkCCommandedOn,
			FPGAAddresses::DCAuxPowerNetworkDOn, this->powerStatus->AuxPowerNetworkDCommandedOn };
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
			FPGAAddresses::DCPowerNetworkAOn, this->powerStatus->PowerNetworkACommandedOn,
			FPGAAddresses::DCPowerNetworkBOn, this->powerStatus->PowerNetworkBCommandedOn,
			FPGAAddresses::DCPowerNetworkCOn, this->powerStatus->PowerNetworkCCommandedOn,
			FPGAAddresses::DCPowerNetworkDOn, this->powerStatus->PowerNetworkDCommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkAOn, this->powerStatus->PowerNetworkACommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkBOn, this->powerStatus->PowerNetworkBCommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkCOn, this->powerStatus->PowerNetworkCCommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCPowerNetworkDOn, this->powerStatus->PowerNetworkDCommandedOn };
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
				FPGAAddresses::DCAuxPowerNetworkAOn, this->powerStatus->AuxPowerNetworkACommandedOn,
				FPGAAddresses::DCAuxPowerNetworkBOn, this->powerStatus->AuxPowerNetworkBCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkCOn, this->powerStatus->AuxPowerNetworkCCommandedOn,
				FPGAAddresses::DCAuxPowerNetworkDOn, this->powerStatus->AuxPowerNetworkDCommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkAOn, this->powerStatus->AuxPowerNetworkACommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkBOn, this->powerStatus->AuxPowerNetworkBCommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkCOn, this->powerStatus->AuxPowerNetworkCCommandedOn };
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
	uint16_t buffer[2] = { FPGAAddresses::DCAuxPowerNetworkDOn, this->powerStatus->AuxPowerNetworkDCommandedOn };
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
