/*
 * AirController.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#include <AirController.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <SAL_m1m3C.h>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

AirController::AirController(M1M3SSPublisher* publisher, SafetyController* safetyController, FPGA* fpga) {
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->fpga = fpga;
	this->airSupplyStatus = this->publisher->getEventAirSupplyStatus();
	this->airSupplyWarning = this->publisher->getEventAirSupplyWarning();
	memset(this->airSupplyStatus, 0, sizeof(m1m3_logevent_AirSupplyStatusC));
	memset(this->airSupplyWarning, 0, sizeof(m1m3_logevent_AirSupplyWarningC));
	this->createTurnAirOnBuffer();
	this->createTurnAirOffBuffer();
	this->airCommandedOnChanged = true;
	this->forcePublishAirSupplyWarning = true;
}

void AirController::turnAirOn() {
	this->fpga->writeCommandFIFO(this->turnAirOnBuffer, 2, 0);
	this->airCommandedOnChanged = !this->airSupplyStatus->AirCommandedOn;
	this->airSupplyStatus->AirCommandedOn = true;
	this->checkStatus();
}

void AirController::turnAirOff() {
	this->fpga->writeCommandFIFO(this->turnAirOffBuffer, 2, 0);
	this->airCommandedOnChanged = this->airSupplyStatus->AirCommandedOn;
	this->airSupplyStatus->AirCommandedOn = false;
	this->checkStatus();
}

void AirController::checkStatus() {
	bool controlStatusChanged = this->checkControlStatus();
	bool valveOpenStatusChanged = this->checkValveOpenStatus();
	bool valveClosedStatusChanged = this->checkValveClosedStatus();
	if (this->airCommandedOnChanged || controlStatusChanged || valveOpenStatusChanged || valveClosedStatusChanged) {
		this->airSupplyStatus->Timestamp = this->publisher->getTimestamp();
		this->publishAirSupplyStatus();
		this->airCommandedOnChanged = false;
	}

	bool commandOutputMismatchChanged = this->checkForCommandOutputMismatch();
	bool commandSensorMismatchChanged = this->checkForCommandSensorMismatch();
	if (this->forcePublishAirSupplyWarning || commandOutputMismatchChanged || commandSensorMismatchChanged) {
		this->airSupplyWarning->Timestamp = this->publisher->getTimestamp();
		this->airSupplyWarning->AnyWarning = this->airSupplyWarning->CommandOutputMismatch || this->airSupplyWarning->CommandSensorMismatch;
		this->publishAirSupplyWarning();
		this->forcePublishAirSupplyWarning = false;
	}
}

void AirController::createTurnAirOnBuffer() {
	this->turnAirOnBuffer[0] = FPGAAddresses::AirSupplyValveControl;
	this->turnAirOnBuffer[1] = true;
}

void AirController::createTurnAirOffBuffer() {
	this->turnAirOffBuffer[0] = FPGAAddresses::AirSupplyValveControl;
	this->turnAirOffBuffer[1] = false;
}

bool AirController::checkControlStatus() {
	this->fpga->writeRequestFIFO(FPGAAddresses::AirSupplyValveControl, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool airCommandOutputOn = this->statusBuffer[0] != 0;
	bool statusChanged = airCommandOutputOn != this->airSupplyStatus->AirCommandOutputOn;
	this->airSupplyStatus->AirCommandOutputOn = airCommandOutputOn;
	return statusChanged;
}

bool AirController::checkValveOpenStatus() {
	this->fpga->writeRequestFIFO(FPGAAddresses::AirSupplyValveOpen, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool airValveOpened = this->statusBuffer[0] != 0;
	bool statusChanged = airValveOpened != this->airSupplyStatus->AirValveOpened;
	this->airSupplyStatus->AirValveOpened = this->statusBuffer[0] != 0;
	return statusChanged;
}

bool AirController::checkValveClosedStatus() {
	this->fpga->writeRequestFIFO(FPGAAddresses::AirSupplyValveClosed, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool airValveClosed = this->statusBuffer[0] != 0;
	bool statusChanged = airValveClosed != this->airSupplyStatus->AirValveClosed;
	this->airSupplyStatus->AirValveClosed = this->statusBuffer[0] != 0;
	return statusChanged;
}

bool AirController::checkForCommandOutputMismatch() {
	bool commandOutputMismatch = this->airSupplyStatus->AirCommandedOn != this->airSupplyStatus->AirCommandOutputOn;
	bool statusChanged = commandOutputMismatch != this->airSupplyWarning->CommandOutputMismatch;
	this->airSupplyWarning->CommandOutputMismatch = commandOutputMismatch;
	this->safetyController->airControllerNotifyCommandOutputMismatch(this->airSupplyWarning->CommandOutputMismatch);
	return statusChanged;
}

bool AirController::checkForCommandSensorMismatch() {
	bool commandSensorMismatch = this->airSupplyStatus->AirCommandedOn != this->airSupplyStatus->AirValveOpened || this->airSupplyStatus->AirCommandedOn == this->airSupplyStatus->AirValveClosed;
	bool statusChanged = commandSensorMismatch != this->airSupplyWarning->CommandSensorMismatch;
	this->airSupplyWarning->CommandSensorMismatch = commandSensorMismatch;
	this->safetyController->airControllerNotifyCommandSensorMismatch(this->airSupplyWarning->CommandSensorMismatch);
	return statusChanged;
}

void AirController::publishAirSupplyStatus() {
	this->publisher->logAirSupplyStatus();
}

void AirController::publishAirSupplyWarning() {
	this->publisher->logAirSupplyWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
