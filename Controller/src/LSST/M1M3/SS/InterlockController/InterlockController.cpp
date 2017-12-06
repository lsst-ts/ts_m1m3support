/*
 * InterlockController.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include <InterlockController.h>
#include <IPublisher.h>
#include <ISafetyController.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <InterlockSettings.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

InterlockController::InterlockController(IPublisher* publisher, ISafetyController* safetyController, IFPGA* fpga, InterlockSettings* interlockSettings) {
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->fpga = fpga;
	this->interlockSettings = interlockSettings;
	this->cellLightStatus = this->publisher->getEventCellLightStatus();
	this->cellLightWarning = this->publisher->getEventCellLightWarning();
	this->interlockStatus = this->publisher->getEventInterlockStatus();
	this->interlockWarning = this->publisher->getEventInterlockWarning();
	this->lastToggleTimestamp = 0;
}

void InterlockController::tryToggleHeartbeat() {
	double currentTimestamp = this->publisher->getTimestamp();
	if (currentTimestamp >= (this->lastToggleTimestamp + this->interlockSettings->HeartbeatPeriodInSeconds)) {
		this->setHeartbeat(!this->interlockStatus->HeartbeatCommandedState);
		this->lastToggleTimestamp = currentTimestamp;
	}
}

void InterlockController::setHeartbeat(bool state) {
	// This signal is inverted (HIGH = OFF, LOW = ON)
	this->interlockStatus->HeartbeatCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::HeartbeatToSafetyController;
	this->txBuffer[1] = !this->interlockStatus->HeartbeatCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkHeartbeatOutputState();
	this->publishInterlockStatus();
	if (this->checkForHeartbeatOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setCriticalFault(bool state) {
	// This signal is inverted (HIGH = OFF, LOW = ON)
	this->interlockStatus->CriticalFaultCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::CriticalFaultToSafetyController;
	this->txBuffer[1] = !this->interlockStatus->CriticalFaultCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkCriticalFaultOutputState();
	this->publishInterlockStatus();
	if (this->checkForCriticalFaultOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setMirrorLoweringRaising(bool state) {
	// This signal is inverted (HIGH = OFF, LOW = ON)
	this->interlockStatus->MirrorLoweringRaisingCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::MirrorLoweringRaisingToSafetyController;
	this->txBuffer[1] = !this->interlockStatus->MirrorLoweringRaisingCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkMirrorLoweringRaisingOutputState();
	this->publishInterlockStatus();
	if (this->checkForMirrorLoweringRaisingOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setMirrorParked(bool state) {
	// This signal is inverted (HIGH = OFF, LOW = ON)
	this->interlockStatus->MirrorParkedCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::MirrorParkedToSafetyController;
	this->txBuffer[1] = !this->interlockStatus->MirrorParkedCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkMirrorParkedOutputState();
	this->publishInterlockStatus();
	if (this->checkForMirrorParkedOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setCellLightsOn(bool state) {
	this->cellLightStatus->CellLightsCommandedOn = state;
	this->txBuffer[0] = FPGAAddresses::MirrorCellLightControl;
	this->txBuffer[1] = this->cellLightStatus->CellLightsCommandedOn;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkCellLightOutputState();
	this->checkCellLightState();
	this->publishCellLightStatus();
	if (this->checkForCellLightOutputMismatch() || this->checkForCellLightSensorMismatch()) {
		this->publishCellLightWarning();
	}
}

bool InterlockController::checkHeartbeatOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::HeartbeatToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	// This signal is inverted (HIGH = OFF, LOW = ON)
	bool heartbeatOutputState = this->statusBuffer[0] == 0;
	bool statusChanged = heartbeatOutputState != this->interlockStatus->HeartbeatOutputState;
	this->interlockStatus->HeartbeatOutputState = heartbeatOutputState;
	return statusChanged;
}

bool InterlockController::checkCriticalFaultOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::CriticalFaultToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	// This signal is inverted (HIGH = OFF, LOW = ON)
	bool criticalFaultOutputState = this->statusBuffer[0] == 0;
	bool statusChanged = criticalFaultOutputState != this->interlockStatus->CriticalFaultOutputState;
	this->interlockStatus->CriticalFaultOutputState = criticalFaultOutputState;
	return statusChanged;
}

bool InterlockController::checkMirrorLoweringRaisingOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::MirrorLoweringRaisingToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	// This signal is inverted (HIGH = OFF, LOW = ON)
	bool mirrorLoweringRaisingOutputState = this->statusBuffer[0] == 0;
	bool statusChanged = mirrorLoweringRaisingOutputState != this->interlockStatus->MirrorLoweringRaisingOutputState;
	this->interlockStatus->MirrorLoweringRaisingOutputState = mirrorLoweringRaisingOutputState;
	return statusChanged;
}

bool InterlockController::checkMirrorParkedOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::MirrorParkedToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	// This signal is inverted (HIGH = OFF, LOW = ON)
	bool mirrorParkedOutputState = this->statusBuffer[0] == 0;
	bool statusChanged = mirrorParkedOutputState != this->interlockStatus->MirrorParkedOutputState;
	this->interlockStatus->MirrorParkedOutputState = mirrorParkedOutputState;
	return statusChanged;
}

bool InterlockController::checkCellLightOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::MirrorCellLightControl, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool mirrorCellLightControlState = this->statusBuffer[0] != 0;
	bool statusChanged = mirrorCellLightControlState != this->cellLightStatus->CellLightsCommandedOn;
	this->cellLightStatus->CellLightsOutputOn = mirrorCellLightControlState;
	return statusChanged;
}

bool InterlockController::checkCellLightState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::MirrorCellLightsOn, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool mirrorCellLightState = this->statusBuffer[0] != 0;
	bool statusChanged = mirrorCellLightState != this->cellLightStatus->CellLightsOn;
	this->cellLightStatus->CellLightsOn = mirrorCellLightState;
	return statusChanged;
}

bool InterlockController::checkForHeartbeatOutputStateMismatch() {
	bool heartbeatStateOutputMismatch = this->interlockStatus->HeartbeatCommandedState != this->interlockStatus->HeartbeatOutputState;
	bool statusChanged = heartbeatStateOutputMismatch != this->interlockWarning->HeartbeatStateOutputMismatch;
	this->interlockWarning->HeartbeatStateOutputMismatch = heartbeatStateOutputMismatch;
	this->safetyController->interlockNotifyHeartbeatStateOutputMismatch(this->interlockWarning->HeartbeatStateOutputMismatch);
	return statusChanged;
}

bool InterlockController::checkForCriticalFaultOutputStateMismatch() {
	bool criticalFaultStateOutputMismatch = this->interlockStatus->CriticalFaultCommandedState != this->interlockStatus->CriticalFaultOutputState;
	bool statusChanged = criticalFaultStateOutputMismatch != this->interlockWarning->CriticalFaultStateOutputMismatch;
	this->interlockWarning->CriticalFaultStateOutputMismatch = criticalFaultStateOutputMismatch;
	this->safetyController->interlockNotifyCriticalFaultStateOutputMismatch(this->interlockWarning->CriticalFaultStateOutputMismatch);
	return statusChanged;
}

bool InterlockController::checkForMirrorLoweringRaisingOutputStateMismatch() {
	bool mirrorLoweringRaisingStateOutputMismatch = this->interlockStatus->MirrorLoweringRaisingCommandedState != this->interlockStatus->MirrorLoweringRaisingOutputState;
	bool statusChanged = mirrorLoweringRaisingStateOutputMismatch != this->interlockWarning->MirrorLoweringRaisingStateOutputMismatch;
	this->interlockWarning->MirrorLoweringRaisingStateOutputMismatch = mirrorLoweringRaisingStateOutputMismatch;
	this->safetyController->interlockNotifyMirrorLoweringRaisingStateOutputMismatch(this->interlockWarning->MirrorLoweringRaisingStateOutputMismatch);
	return statusChanged;
}

bool InterlockController::checkForMirrorParkedOutputStateMismatch() {
	bool mirrorParkedStateOutputMismatch = this->interlockStatus->MirrorParkedCommandedState != this->interlockStatus->MirrorParkedOutputState;
	bool statusChanged = mirrorParkedStateOutputMismatch != this->interlockWarning->MirrorParkedStateOutputMismatch;
	this->interlockWarning->MirrorParkedStateOutputMismatch = mirrorParkedStateOutputMismatch;
	this->safetyController->interlockNotifyMirrorParkedStateOutputMismatch(this->interlockWarning->MirrorParkedStateOutputMismatch);
	return statusChanged;
}

bool InterlockController::checkForCellLightOutputMismatch() {
	bool cellLightOutputMismatch = this->cellLightStatus->CellLightsCommandedOn != this->cellLightStatus->CellLightsOutputOn;
	bool statusChanged = cellLightOutputMismatch != this->cellLightWarning->CellLightsOutputMismatch;
	this->cellLightWarning->CellLightsOutputMismatch = cellLightOutputMismatch;
	this->safetyController->cellLightNotifyOutputMismatch(this->cellLightWarning->CellLightsOutputMismatch);
	return statusChanged;
}

bool InterlockController::checkForCellLightSensorMismatch() {
	bool cellLightSensorMismatch = this->cellLightStatus->CellLightsCommandedOn != this->cellLightStatus->CellLightsOn;
	bool statusChanged = cellLightSensorMismatch != this->cellLightWarning->CellLightsSensorMismatch;
	this->cellLightWarning->CellLightsSensorMismatch = cellLightSensorMismatch;
	this->safetyController->cellLightNotifySensorMismatch(this->cellLightWarning->CellLightsSensorMismatch);
	return statusChanged;
}

void InterlockController::publishCellLightStatus() {
	this->cellLightStatus->Timestamp = this->publisher->getTimestamp();
	this->publisher->logCellLightStatus();
}

void InterlockController::publishCellLightWarning() {
	this->cellLightWarning->Timestamp = this->publisher->getTimestamp();
	this->cellLightWarning->AnyWarning =
			this->cellLightWarning->CellLightsOutputMismatch |
			this->cellLightWarning->CellLightsSensorMismatch;
	this->publisher->logCellLightWarning();
}

void InterlockController::publishInterlockStatus() {
	this->interlockStatus->Timestamp = this->publisher->getTimestamp();
	this->publisher->logInterlockStatus();
}

void InterlockController::publishInterlockWarning() {
	this->interlockWarning->Timestamp = this->publisher->getTimestamp();
	this->interlockWarning->AnyWarning =
			this->interlockWarning->HeartbeatStateOutputMismatch |
			this->interlockWarning->CriticalFaultStateOutputMismatch |
			this->interlockWarning->MirrorLoweringRaisingStateOutputMismatch |
			this->interlockWarning->MirrorParkedStateOutputMismatch;
	this->publisher->logInterlockWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
