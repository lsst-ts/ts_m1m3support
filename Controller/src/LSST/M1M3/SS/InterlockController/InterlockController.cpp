/*
 * InterlockController.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include <InterlockController.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <InterlockApplicationSettings.h>
#include <SAL_m1m3C.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

InterlockController::InterlockController(M1M3SSPublisher* publisher, FPGA* fpga, InterlockApplicationSettings* interlockApplicationSettings) {
	Log.Debug("InterlockController: InterlockController()");
	this->publisher = publisher;
	this->safetyController = 0;
	this->fpga = fpga;
	this->interlockApplicationSettings = interlockApplicationSettings;
	this->cellLightStatus = this->publisher->getEventCellLightStatus();
	this->cellLightWarning = this->publisher->getEventCellLightWarning();
	this->interlockStatus = this->publisher->getEventInterlockStatus();
	this->interlockWarning = this->publisher->getEventInterlockWarning();
	this->lastToggleTimestamp = 0;
}

void InterlockController::setSafetyController(SafetyController* safetyController) {
	Log.Debug("InterlockController: setSafetyController()");
	this->safetyController = safetyController;
}

void InterlockController::tryToggleHeartbeat() {
	double currentTimestamp = this->publisher->getTimestamp();
	if (currentTimestamp >= (this->lastToggleTimestamp + this->interlockApplicationSettings->HeartbeatPeriodInSeconds)) {
		this->setHeartbeat(!this->interlockStatus->HeartbeatCommandedState);
		this->lastToggleTimestamp = currentTimestamp;
	}
}

void InterlockController::setHeartbeat(bool state) {
	Log.Trace("InterlockController: setHeartbeat(%d)", state);
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

void InterlockController::checkInterlockStatus() {
	Log.Trace("InterlockController: checkInterlockStatus()");
	// These signals are inverted (HIGH = Everything is OK, LOW = Bad this has happened)
	uint16_t buffer[6] = {
			FPGAAddresses::ILCPowerInterlockStatus,
			FPGAAddresses::FanCoilerHeaterInterlockStatus,
			FPGAAddresses::AirSupplyInterlockStatus,
			FPGAAddresses::CabinetDoorInterlockStatus,
			FPGAAddresses::TMAMotionStopInterlockStatus,
			FPGAAddresses::GISHeartbeatInterlockStatus
	};
	this->fpga->writeRequestFIFO(buffer, 6, 0);
	this->fpga->readU16ResponseFIFO(buffer, 6, 20);

	this->interlockWarning->AuxPowerNetworksOff = buffer[0] == 0;
	this->interlockWarning->ThermalEquipmentOff = buffer[1] == 0;
	this->interlockWarning->AirSupplyOff = buffer[2] == 0;
	this->interlockWarning->CabinetDoorOpen = buffer[3] == 0;
	this->interlockWarning->TMAMotionStop = buffer[4] == 0;
	this->interlockWarning->GISHeartbeatLost = buffer[5] == 0;

	if (this->safetyController) {
		this->safetyController->interlockNotifyAuxPowerNetworksOff(this->interlockWarning->AuxPowerNetworksOff);
		this->safetyController->interlockNotifyThermalEquipmentOff(this->interlockWarning->ThermalEquipmentOff);
		this->safetyController->interlockNotifyAirSupplyOff(this->interlockWarning->AirSupplyOff);
		this->safetyController->interlockNotifyCabinetDoorOpen(this->interlockWarning->CabinetDoorOpen);
		this->safetyController->interlockNotifyTMAMotionStop(this->interlockWarning->TMAMotionStop);
		this->safetyController->interlockNotifyGISHeartbeatLost(this->interlockWarning->GISHeartbeatLost);
	}
	this->interlockWarning->Timestamp = this->publisher->getTimestamp();
	this->publisher->tryLogInterlockWarning();
}

void InterlockController::setCellLightsOn(bool state) {
	Log.Info("InterlockController: setCellLightsOn(%d)", state);
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
	if (this->safetyController) {
		this->safetyController->interlockNotifyHeartbeatStateOutputMismatch(this->interlockWarning->HeartbeatStateOutputMismatch);
	}
	return statusChanged;
}

bool InterlockController::checkForCellLightOutputMismatch() {
	bool cellLightOutputMismatch = this->cellLightStatus->CellLightsCommandedOn != this->cellLightStatus->CellLightsOutputOn;
	bool statusChanged = cellLightOutputMismatch != this->cellLightWarning->CellLightsOutputMismatch;
	this->cellLightWarning->CellLightsOutputMismatch = cellLightOutputMismatch;
	if (this->safetyController) {
		this->safetyController->cellLightNotifyOutputMismatch(this->cellLightWarning->CellLightsOutputMismatch);
	}
	return statusChanged;
}

bool InterlockController::checkForCellLightSensorMismatch() {
	bool cellLightSensorMismatch = this->cellLightStatus->CellLightsCommandedOn != this->cellLightStatus->CellLightsOn;
	bool statusChanged = cellLightSensorMismatch != this->cellLightWarning->CellLightsSensorMismatch;
	this->cellLightWarning->CellLightsSensorMismatch = cellLightSensorMismatch;
	if (this->safetyController) {
		this->safetyController->cellLightNotifySensorMismatch(this->cellLightWarning->CellLightsSensorMismatch);
	}
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
	this->publisher->logInterlockWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
