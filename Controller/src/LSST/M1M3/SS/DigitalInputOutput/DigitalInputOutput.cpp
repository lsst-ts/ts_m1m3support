/*
 * DigitalInputOutput.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#include <DigitalInputOutput.h>
#include <InterlockApplicationSettings.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <Range.h>
#include <SAL_MTM1M3C.h>
#include <SALEnumerations.h>
#include <BitHelper.h>
#include <Log.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

DigitalInputOutput::DigitalInputOutput(InterlockApplicationSettings* interlockApplicationSettings, IFPGA* fpga, M1M3SSPublisher* publisher) {
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
    if (this->fpgaData->DigitalOutputTimestamp != this->lastDOTimestamp) {
        tryPublish = true;
        this->lastDOTimestamp = this->fpgaData->DigitalOutputTimestamp;
    }
    if (this->fpgaData->DigitalInputTimestamp != this->lastDITimestamp) {
        tryPublish = true;
        this->lastDITimestamp = this->fpgaData->DigitalInputTimestamp;

        // Polarity is swapped
        this->airSupplyStatus->airValveOpened = (this->fpgaData->DigitalInputStates & 0x0100) == 0;
        // Polarity is swapped
        this->airSupplyStatus->airValveClosed = (this->fpgaData->DigitalInputStates & 0x0200) == 0;
        BitHelper::set(&this->airSupplyWarning->airSupplyFlags, AirSupplyFlags::AirValveSensorMismatch, (this->airSupplyStatus->airCommandedOn && (!this->airSupplyStatus->airValveOpened || this->airSupplyStatus->airValveClosed)) || (!this->airSupplyStatus->airCommandedOn && (this->airSupplyStatus->airValveOpened || !this->airSupplyStatus->airValveClosed)));

        this->cellLightStatus->cellLightsOn = (this->fpgaData->DigitalInputStates & 0x0400) != 0;
        BitHelper::set(&this->cellLightWarning->cellLightFlags, CellLightFlags::CellLightSensorMismatch, this->cellLightStatus->cellLightsCommandedOn != this->cellLightStatus->cellLightsOn);

        BitHelper::set(&this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::AuxPowerNetworksOff, (this->fpgaData->DigitalInputStates & 0x0001) == 0);
        BitHelper::set(&this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::ThermalEquipmentOff, (this->fpgaData->DigitalInputStates & 0x0002) == 0);
        BitHelper::set(&this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::AirSupplyOff, (this->fpgaData->DigitalInputStates & 0x0008) == 0);
        BitHelper::set(&this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::CabinetDoorOpen, (this->fpgaData->DigitalInputStates & 0x0010) == 0);
        BitHelper::set(&this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::TMAMotionStop, (this->fpgaData->DigitalInputStates & 0x0040) == 0);
        BitHelper::set(&this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::GISHeartbeatLost, (this->fpgaData->DigitalInputStates & 0x0080) == 0);

        if (this->safetyController) {
            this->safetyController->airControllerNotifyCommandSensorMismatch(BitHelper::get(this->airSupplyWarning->airSupplyFlags, AirSupplyFlags::AirValveSensorMismatch));
            this->safetyController->cellLightNotifySensorMismatch(BitHelper::get(this->cellLightWarning->cellLightFlags, CellLightFlags::CellLightSensorMismatch));
            this->safetyController->interlockNotifyAuxPowerNetworksOff(BitHelper::get(this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::AuxPowerNetworksOff));
            this->safetyController->interlockNotifyThermalEquipmentOff(BitHelper::get(this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::ThermalEquipmentOff));
            this->safetyController->interlockNotifyAirSupplyOff(BitHelper::get(this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::AirSupplyOff));
            this->safetyController->interlockNotifyCabinetDoorOpen(BitHelper::get(this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::CabinetDoorOpen));
            this->safetyController->interlockNotifyTMAMotionStop(BitHelper::get(this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::TMAMotionStop));
            this->safetyController->interlockNotifyGISHeartbeatLost(BitHelper::get(this->interlockWarning->interlockSystemFlags, InterlockSystemFlags::GISHeartbeatLost));
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
        uint16_t buffer[2] = {FPGAAddresses::HeartbeatToSafetyController, (uint16_t)this->interlockStatus->heartbeatCommandedState};
        this->fpga->writeCommandFIFO(buffer, 2, 0);
    }
}

void DigitalInputOutput::turnAirOn() {
    Log.Info("DigitalInputOutput: turnAirOn()");
    this->airSupplyStatus->airCommandedOn = true;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, (uint16_t)this->airSupplyStatus->airCommandedOn};
    this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOff() {
    Log.Info("DigitalInputOutput: turnAirOff()");
    this->airSupplyStatus->airCommandedOn = false;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, (uint16_t)this->airSupplyStatus->airCommandedOn};
    this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOn() {
    Log.Info("DigitalInputOutput: turnCellLightsOn()");
    this->cellLightStatus->cellLightsCommandedOn = true;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl, (uint16_t)(!this->cellLightStatus->cellLightsCommandedOn)};
    this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOff() {
    Log.Info("DigitalInputOutput: turnCellLightsOff()");
    this->cellLightStatus->cellLightsCommandedOn = false;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl, (uint16_t)(!this->cellLightStatus->cellLightsCommandedOn)};
    this->fpga->writeCommandFIFO(buffer, 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
