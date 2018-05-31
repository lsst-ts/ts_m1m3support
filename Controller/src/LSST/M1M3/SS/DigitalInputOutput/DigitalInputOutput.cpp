/*
 * DigitalInputOutput.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#include <DigitalInputOutput.h>
#include <InterlockApplicationSettings.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <Range.h>
#include <SAL_m1m3C.h>
#include <Log.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

DigitalInputOutput::DigitalInputOutput(InterlockApplicationSettings* interlockApplicationSettings, FPGA* fpga, M1M3SSPublisher* publisher) {
	Log.Debug("DigitalInputOutput: DigitalInputOutput()");
	this->interlockApplicationSettings = interlockApplicationSettings;
	this->fpga = fpga;
	this->publisher = publisher;
	this->safetyController = 0;

	this->airSupplyStatus = this->publisher->getEventAirSupplyStatus();
	this->airSupplyWarning = this->publisher->getEventAirSupplyWarning();
	this->cellLightStatus = this->publisher->getEventCellLightStatus();
	this->cellLightWarning = this->publisher->getEventCellLightWarning();
	this->interlockStatus = this->publisher->getEventInterlockStatus();
	this->interlockWarning = this->publisher->getEventInterlockWarning();

	this->fpgaData = this->fpga->getSupportFPGAData();

	this->lastDITimestamp = 0;
	this->lastDOTimestamp = 0;
	this->lastToggleTimestamp = 0;

	memset(this->airSupplyStatus, 0, sizeof(m1m3_logevent_AirSupplyStatusC));
	memset(this->airSupplyWarning, 0, sizeof(m1m3_logevent_AirSupplyWarningC));
	memset(this->cellLightStatus, 0, sizeof(m1m3_logevent_CellLightStatusC));
	memset(this->cellLightWarning, 0, sizeof(m1m3_logevent_CellLightWarningC));
	memset(this->interlockStatus, 0, sizeof(m1m3_logevent_InterlockStatusC));
	memset(this->interlockWarning, 0, sizeof(m1m3_logevent_InterlockWarningC));
}

void DigitalInputOutput::setSafetyController(SafetyController* safetyController) {
	Log.Debug("DigitalInputOutput: setSafetyController()");
	this->safetyController = safetyController;
}

void DigitalInputOutput::processData() {
	// TODO: Handle no data available
	Log.Trace("DigitalInputOutput: processData()");
	bool tryPublish = false;
	double timestamp = Timestamp::fromFPGA(Range::Max(this->fpgaData->DigitalOutputTimestamp, this->fpgaData->DigitalInputTimestamp));
	if (this->fpgaData->DigitalOutputTimestamp != this->lastDOTimestamp) {
		tryPublish = true;
		this->lastDOTimestamp = this->fpgaData->DigitalOutputTimestamp;

		this->airSupplyStatus->Timestamp = timestamp;
		this->airSupplyStatus->AirCommandOutputOn = (this->fpgaData->DigitalOutputStates & 0x10) != 0;

		this->airSupplyWarning->Timestamp = timestamp;
		this->airSupplyWarning->CommandOutputMismatch = this->airSupplyStatus->AirCommandOutputOn != this->airSupplyStatus->AirCommandedOn;

		this->cellLightStatus->Timestamp = timestamp;
		this->cellLightStatus->CellLightsOutputOn = (this->fpgaData->DigitalOutputStates & 0x20) != 0;

		this->cellLightWarning->Timestamp = timestamp;
		this->cellLightWarning->CellLightsOutputMismatch = this->cellLightStatus->CellLightsOutputOn != this->cellLightStatus->CellLightsCommandedOn;

		this->interlockStatus->Timestamp = timestamp;
		this->interlockStatus->HeartbeatOutputState = (this->fpgaData->DigitalOutputStates & 0x01) != 0;

		this->interlockWarning->Timestamp = timestamp;
		this->interlockWarning->HeartbeatStateOutputMismatch = this->interlockStatus->HeartbeatOutputState == this->interlockStatus->HeartbeatCommandedState;

		if (this->safetyController) {
			this->safetyController->airControllerNotifyCommandOutputMismatch(this->airSupplyWarning->CommandOutputMismatch);
			this->safetyController->cellLightNotifyOutputMismatch(this->cellLightWarning->CellLightsOutputMismatch);
			this->safetyController->interlockNotifyHeartbeatStateOutputMismatch(this->interlockWarning->HeartbeatStateOutputMismatch);
		}
	}
	if (this->fpgaData->DigitalInputTimestamp != this->lastDITimestamp) {
		tryPublish = true;
		this->lastDITimestamp = this->fpgaData->DigitalInputTimestamp;

		this->airSupplyStatus->Timestamp = timestamp;
		this->airSupplyStatus->AirValveOpened = (this->fpgaData->DigitalInputStates & 0x0100) != 0;
		this->airSupplyStatus->AirValveClosed = (this->fpgaData->DigitalInputStates & 0x0200) != 0;

		this->airSupplyWarning->Timestamp = timestamp;
		this->airSupplyWarning->CommandSensorMismatch =
				(this->airSupplyStatus->AirCommandedOn && (!this->airSupplyStatus->AirValveOpened || this->airSupplyStatus->AirValveClosed)) ||
				(!this->airSupplyStatus->AirCommandedOn && (this->airSupplyStatus->AirValveOpened || !this->airSupplyStatus->AirValveClosed));

		this->cellLightStatus->Timestamp = timestamp;
		this->cellLightStatus->CellLightsOn = (this->fpgaData->DigitalInputStates & 0x0400) != 0;

		this->cellLightWarning->Timestamp = timestamp;
		this->cellLightWarning->CellLightsSensorMismatch = this->cellLightStatus->CellLightsCommandedOn != this->cellLightStatus->CellLightsOn;

		this->interlockWarning->Timestamp = timestamp;
		this->interlockWarning->AuxPowerNetworksOff = (this->fpgaData->DigitalInputStates & 0x0001) == 0;
		this->interlockWarning->ThermalEquipmentOff = (this->fpgaData->DigitalInputStates & 0x0002) == 0;
		this->interlockWarning->AirSupplyOff = (this->fpgaData->DigitalInputStates & 0x0008) == 0;
		this->interlockWarning->CabinetDoorOpen = (this->fpgaData->DigitalInputStates & 0x0010) == 0;
		this->interlockWarning->TMAMotionStop = (this->fpgaData->DigitalInputStates & 0x0040) == 0;
		this->interlockWarning->GISHeartbeatLost = (this->fpgaData->DigitalInputStates & 0x0080) == 0;

		if (this->safetyController) {
			this->safetyController->airControllerNotifyCommandSensorMismatch(this->airSupplyWarning->CommandSensorMismatch);
			this->safetyController->cellLightNotifySensorMismatch(this->cellLightWarning->CellLightsSensorMismatch);
			this->safetyController->interlockNotifyAirSupplyOff(this->interlockWarning->AuxPowerNetworksOff);
			this->safetyController->interlockNotifyThermalEquipmentOff(this->interlockWarning->ThermalEquipmentOff);
			this->safetyController->interlockNotifyAirSupplyOff(this->interlockWarning->AirSupplyOff);
			this->safetyController->interlockNotifyCabinetDoorOpen(this->interlockWarning->CabinetDoorOpen);
			this->safetyController->interlockNotifyTMAMotionStop(this->interlockWarning->TMAMotionStop);
			this->safetyController->interlockNotifyGISHeartbeatLost(this->interlockWarning->GISHeartbeatLost);
		}
	}
	if (tryPublish) {
		this->publisher->tryLogAirSupplyStatus();
		this->publisher->tryLogAirSupplyWarning();
		this->publisher->tryLogCellLightStatus();
		this->publisher->tryLogCellLightWarning();
		this->publisher->tryLogInterlockStatus();
		this->publisher->tryLogInterlockWarning();
	}
}

void DigitalInputOutput::tryToggleHeartbeat() {
	Log.Trace("DigitalInputOutput: tryToggleHeartbeat()");
	double timestamp = this->publisher->getTimestamp();
	if (timestamp >= (this->lastToggleTimestamp + this->interlockApplicationSettings->HeartbeatPeriodInSeconds)) {
		Log.Debug("DigitalInputOutput: toggleHeartbeat()");
		this->lastToggleTimestamp = timestamp;
		this->interlockStatus->HeartbeatCommandedState = !this->interlockStatus->HeartbeatCommandedState;
		uint16_t buffer[2] = { FPGAAddresses::HeartbeatToSafetyController, (uint16_t)this->interlockStatus->HeartbeatCommandedState };
		this->fpga->writeCommandFIFO(buffer, 2, 0);
	}
}

void DigitalInputOutput::turnAirOn() {
	Log.Info("DigitalInputOutput: turnAirOn()");
	this->airSupplyStatus->AirCommandedOn = true;
	uint16_t buffer[2] = { FPGAAddresses::AirSupplyValveControl, (uint16_t)this->airSupplyStatus->AirCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOff() {
	Log.Info("DigitalInputOutput: turnAirOff()");
	this->airSupplyStatus->AirCommandedOn = false;
	uint16_t buffer[2] = { FPGAAddresses::AirSupplyValveControl, (uint16_t)this->airSupplyStatus->AirCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOn() {
	Log.Info("DigitalInputOutput: turnCellLightsOn()");
	this->cellLightStatus->CellLightsCommandedOn = true;
	uint16_t buffer[2] = { FPGAAddresses::MirrorCellLightControl, (uint16_t)this->cellLightStatus->CellLightsCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOff() {
	Log.Info("DigitalInputOutput: turnCellLightsOff()");
	this->cellLightStatus->CellLightsCommandedOn = false;
	uint16_t buffer[2] = { FPGAAddresses::MirrorCellLightControl, (uint16_t)this->cellLightStatus->CellLightsCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
