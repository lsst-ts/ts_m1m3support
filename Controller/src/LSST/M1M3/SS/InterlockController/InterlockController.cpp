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
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

InterlockController::InterlockController(IPublisher* publisher, ISafetyController* safetyController, IFPGA* fpga) {
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->fpga = fpga;
	this->interlockStatus = this->publisher->getEventInterlockStatus();
	this->interlockWarning = this->publisher->getEventInterlockWarning();
}

void InterlockController::setHeartbeat(bool state) {
	this->interlockStatus->HeartbeatCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::HeartbeatToSafetyController;
	this->txBuffer[1] = this->interlockStatus->HeartbeatCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkHeartbeatOutputState();
	this->publishInterlockStatus();
	if (this->checkForHeartbeatOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setCriticalFault(bool state) {
	this->interlockStatus->CriticalFaultCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::CriticalFaultToSafetyController;
	this->txBuffer[1] = this->interlockStatus->CriticalFaultCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkCriticalFaultOutputState();
	this->publishInterlockStatus();
	if (this->checkForCriticalFaultOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setMirrorLoweringRaising(bool state) {
	this->interlockStatus->MirrorLoweringRaisingCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::MirrorLoweringRaisingToSafetyController;
	this->txBuffer[1] = this->interlockStatus->MirrorLoweringRaisingCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkMirrorLoweringRaisingOutputState();
	this->publishInterlockStatus();
	if (this->checkForMirrorLoweringRaisingOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

void InterlockController::setMirrorParked(bool state) {
	this->interlockStatus->MirrorParkedCommandedState = state;
	this->txBuffer[0] = FPGAAddresses::MirrorParkedToSafetyController;
	this->txBuffer[1] = this->interlockStatus->MirrorParkedCommandedState;
	this->fpga->writeCommandFIFO(this->txBuffer, 2, 0);
	this->checkMirrorParkedOutputState();
	this->publishInterlockStatus();
	if (this->checkForMirrorParkedOutputStateMismatch()) {
		this->publishInterlockWarning();
	}
}

bool InterlockController::checkHeartbeatOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::HeartbeatToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool heartbeatOutputState = this->statusBuffer[0] != 0;
	bool statusChanged = heartbeatOutputState != this->interlockStatus->HeartbeatOutputState;
	this->interlockStatus->HeartbeatOutputState = heartbeatOutputState;
	return statusChanged;
}

bool InterlockController::checkCriticalFaultOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::CriticalFaultToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool criticalFaultOutputState = this->statusBuffer[0] != 0;
	bool statusChanged = criticalFaultOutputState != this->interlockStatus->CriticalFaultOutputState;
	this->interlockStatus->CriticalFaultOutputState = criticalFaultOutputState;
	return statusChanged;
}

bool InterlockController::checkMirrorLoweringRaisingOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::MirrorLoweringRaisingToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool mirrorLoweringRaisingOutputState = this->statusBuffer[0] != 0;
	bool statusChanged = mirrorLoweringRaisingOutputState != this->interlockStatus->MirrorLoweringRaisingOutputState;
	this->interlockStatus->MirrorLoweringRaisingOutputState = mirrorLoweringRaisingOutputState;
	return statusChanged;
}

bool InterlockController::checkMirrorParkedOutputState() {
	this->fpga->writeRequestFIFO(FPGAAddresses::MirrorParkedToSafetyController, 0);
	this->fpga->readU16ResponseFIFO(this->statusBuffer, 1, 10);
	bool mirrorParkedOutputState = this->statusBuffer[0] != 0;
	bool statusChanged = mirrorParkedOutputState != this->interlockStatus->MirrorParkedOutputState;
	this->interlockStatus->MirrorParkedOutputState = mirrorParkedOutputState;
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
