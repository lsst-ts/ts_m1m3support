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
#include <SAL_MTM1M3C.h>
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

	memset(this->airSupplyStatus, 0, sizeof(MTM1M3_logevent_airSupplyStatusC));
	memset(this->airSupplyWarning, 0, sizeof(MTM1M3_logevent_airSupplyWarningC));
	memset(this->cellLightStatus, 0, sizeof(MTM1M3_logevent_cellLightStatusC));
	memset(this->cellLightWarning, 0, sizeof(MTM1M3_logevent_cellLightWarningC));
	memset(this->interlockStatus, 0, sizeof(MTM1M3_logevent_interlockStatusC));
	memset(this->interlockWarning, 0, sizeof(MTM1M3_logevent_interlockWarningC));
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

		this->airSupplyStatus->timestamp = timestamp;
		this->airSupplyStatus->airCommandOutputOn = (this->fpgaData->DigitalOutputStates & 0x10) != 0;

		this->airSupplyWarning->timestamp = timestamp;
		this->airSupplyWarning->commandOutputMismatch = this->airSupplyStatus->airCommandOutputOn != this->airSupplyStatus->airCommandedOn;

		this->cellLightStatus->timestamp = timestamp;
		// Polarity is swapped
		this->cellLightStatus->cellLightsOutputOn = (this->fpgaData->DigitalOutputStates & 0x20) == 0;

		this->cellLightWarning->timestamp = timestamp;
		this->cellLightWarning->cellLightsOutputMismatch = this->cellLightStatus->cellLightsOutputOn != this->cellLightStatus->cellLightsCommandedOn;

		this->interlockStatus->timestamp = timestamp;
		this->interlockStatus->heartbeatOutputState = (this->fpgaData->DigitalOutputStates & 0x01) != 0;

		this->interlockWarning->timestamp = timestamp;
		this->interlockWarning->heartbeatStateOutputMismatch = this->interlockStatus->heartbeatOutputState != this->interlockStatus->heartbeatCommandedState;

		if (this->safetyController) {
			this->safetyController->airControllerNotifyCommandOutputMismatch(this->airSupplyWarning->commandOutputMismatch);
			this->safetyController->cellLightNotifyOutputMismatch(this->cellLightWarning->cellLightsOutputMismatch);
			this->safetyController->interlockNotifyHeartbeatStateOutputMismatch(this->interlockWarning->heartbeatStateOutputMismatch);
		}
	}
	if (this->fpgaData->DigitalInputTimestamp != this->lastDITimestamp) {
		tryPublish = true;
		this->lastDITimestamp = this->fpgaData->DigitalInputTimestamp;

		this->airSupplyStatus->timestamp = timestamp;
		// Polarity is swapped
		this->airSupplyStatus->airValveOpened = (this->fpgaData->DigitalInputStates & 0x0100) == 0;
		// Polarity is swapped
		this->airSupplyStatus->airValveClosed = (this->fpgaData->DigitalInputStates & 0x0200) == 0;

		this->airSupplyWarning->timestamp = timestamp;
		this->airSupplyWarning->commandSensorMismatch =
				(this->airSupplyStatus->airCommandedOn && (!this->airSupplyStatus->airValveOpened || this->airSupplyStatus->airValveClosed)) ||
				(!this->airSupplyStatus->airCommandedOn && (this->airSupplyStatus->airValveOpened || !this->airSupplyStatus->airValveClosed));

		this->cellLightStatus->timestamp = timestamp;
		this->cellLightStatus->cellLightsOn = (this->fpgaData->DigitalInputStates & 0x0400) != 0;

		this->cellLightWarning->timestamp = timestamp;
		this->cellLightWarning->cellLightsSensorMismatch = this->cellLightStatus->cellLightsCommandedOn != this->cellLightStatus->cellLightsOn;

		this->interlockWarning->timestamp = timestamp;
		this->interlockWarning->auxPowerNetworksOff = (this->fpgaData->DigitalInputStates & 0x0001) == 0;
		this->interlockWarning->thermalEquipmentOff = (this->fpgaData->DigitalInputStates & 0x0002) == 0;
		this->interlockWarning->airSupplyOff = (this->fpgaData->DigitalInputStates & 0x0008) == 0;
		this->interlockWarning->cabinetDoorOpen = (this->fpgaData->DigitalInputStates & 0x0010) == 0;
		this->interlockWarning->tmaMotionStop = (this->fpgaData->DigitalInputStates & 0x0040) == 0;
		this->interlockWarning->gisHeartbeatLost = (this->fpgaData->DigitalInputStates & 0x0080) == 0;

		if (this->safetyController) {
			this->safetyController->airControllerNotifyCommandSensorMismatch(this->airSupplyWarning->commandSensorMismatch);
			this->safetyController->cellLightNotifySensorMismatch(this->cellLightWarning->cellLightsSensorMismatch);
			this->safetyController->interlockNotifyAuxPowerNetworksOff(this->interlockWarning->auxPowerNetworksOff);
			this->safetyController->interlockNotifyThermalEquipmentOff(this->interlockWarning->thermalEquipmentOff);
			this->safetyController->interlockNotifyAirSupplyOff(this->interlockWarning->airSupplyOff);
			this->safetyController->interlockNotifyCabinetDoorOpen(this->interlockWarning->cabinetDoorOpen);
			this->safetyController->interlockNotifyTMAMotionStop(this->interlockWarning->tmaMotionStop);
			this->safetyController->interlockNotifyGISHeartbeatLost(this->interlockWarning->gisHeartbeatLost);
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
		this->interlockStatus->heartbeatCommandedState = !this->interlockStatus->heartbeatCommandedState;
		uint16_t buffer[2] = { FPGAAddresses::HeartbeatToSafetyController, (uint16_t)this->interlockStatus->heartbeatCommandedState };
		this->fpga->writeCommandFIFO(buffer, 2, 0);
	}
}

void DigitalInputOutput::turnAirOn() {
	Log.Info("DigitalInputOutput: turnAirOn()");
	this->airSupplyStatus->airCommandedOn = true;
	uint16_t buffer[2] = { FPGAAddresses::AirSupplyValveControl, (uint16_t)this->airSupplyStatus->airCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOff() {
	Log.Info("DigitalInputOutput: turnAirOff()");
	this->airSupplyStatus->airCommandedOn = false;
	uint16_t buffer[2] = { FPGAAddresses::AirSupplyValveControl, (uint16_t)this->airSupplyStatus->airCommandedOn };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOn() {
	Log.Info("DigitalInputOutput: turnCellLightsOn()");
	this->cellLightStatus->cellLightsCommandedOn = true;
	// Polarity is swapped
	uint16_t buffer[2] = { FPGAAddresses::MirrorCellLightControl, (uint16_t)(!this->cellLightStatus->cellLightsCommandedOn) };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOff() {
	Log.Info("DigitalInputOutput: turnCellLightsOff()");
	this->cellLightStatus->cellLightsCommandedOn = false;
	// Polarity is swapped
	uint16_t buffer[2] = { FPGAAddresses::MirrorCellLightControl, (uint16_t)(!this->cellLightStatus->cellLightsCommandedOn) };
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
