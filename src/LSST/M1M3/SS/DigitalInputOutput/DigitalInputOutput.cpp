/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <DigitalInputOutput.h>
#include <InterlockApplicationSettings.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <Range.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

DigitalInputOutput::DigitalInputOutput(InterlockApplicationSettings* interlockApplicationSettings,
                                       M1M3SSPublisher* publisher) {
    spdlog::debug("DigitalInputOutput: DigitalInputOutput()");
    this->interlockApplicationSettings = interlockApplicationSettings;
    this->publisher = publisher;
    this->safetyController = 0;

    this->airSupplyStatus = this->publisher->getEventAirSupplyStatus();
    this->airSupplyWarning = this->publisher->getEventAirSupplyWarning();
    this->cellLightStatus = this->publisher->getEventCellLightStatus();
    this->cellLightWarning = this->publisher->getEventCellLightWarning();
    this->interlockStatus = this->publisher->getEventInterlockStatus();
    this->interlockWarning = this->publisher->getEventInterlockWarning();

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
    spdlog::debug("DigitalInputOutput: setSafetyController()");
    this->safetyController = safetyController;
}

void DigitalInputOutput::processData() {
    // TODO: Handle no data available
    spdlog::trace("DigitalInputOutput: processData()");
    bool tryPublish = false;
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    double timestamp = Timestamp::fromFPGA(
            Range::Max(fpgaData->DigitalOutputTimestamp, fpgaData->DigitalInputTimestamp));
    if (fpgaData->DigitalOutputTimestamp != this->lastDOTimestamp) {
        tryPublish = true;
        this->lastDOTimestamp = fpgaData->DigitalOutputTimestamp;

        this->airSupplyStatus->timestamp = timestamp;
        this->airSupplyStatus->airCommandOutputOn = (fpgaData->DigitalOutputStates & 0x10) != 0;

        this->airSupplyWarning->timestamp = timestamp;
        this->airSupplyWarning->commandOutputMismatch =
                this->airSupplyStatus->airCommandOutputOn != this->airSupplyStatus->airCommandedOn;

        this->cellLightStatus->timestamp = timestamp;
        // Polarity is swapped
        this->cellLightStatus->cellLightsOutputOn = (fpgaData->DigitalOutputStates & 0x20) == 0;

        this->cellLightWarning->timestamp = timestamp;
        this->cellLightWarning->cellLightsOutputMismatch =
                this->cellLightStatus->cellLightsOutputOn != this->cellLightStatus->cellLightsCommandedOn;

        this->interlockStatus->timestamp = timestamp;
        this->interlockStatus->heartbeatOutputState = (fpgaData->DigitalOutputStates & 0x01) != 0;

        this->interlockWarning->timestamp = timestamp;
        this->interlockWarning->heartbeatStateOutputMismatch =
                this->interlockStatus->heartbeatOutputState != this->interlockStatus->heartbeatCommandedState;

        if (this->safetyController) {
            this->safetyController->airControllerNotifyCommandOutputMismatch(
                    this->airSupplyWarning->commandOutputMismatch);
            this->safetyController->cellLightNotifyOutputMismatch(
                    this->cellLightWarning->cellLightsOutputMismatch);
            this->safetyController->interlockNotifyHeartbeatStateOutputMismatch(
                    this->interlockWarning->heartbeatStateOutputMismatch);
        }
    }
    if (fpgaData->DigitalInputTimestamp != this->lastDITimestamp) {
        tryPublish = true;
        this->lastDITimestamp = fpgaData->DigitalInputTimestamp;

        this->airSupplyStatus->timestamp = timestamp;
        // Polarity is swapped
        this->airSupplyStatus->airValveOpened = (fpgaData->DigitalInputStates & 0x0100) == 0;
        // Polarity is swapped
        this->airSupplyStatus->airValveClosed = (fpgaData->DigitalInputStates & 0x0200) == 0;

        this->airSupplyWarning->timestamp = timestamp;
        this->airSupplyWarning->commandSensorMismatch =
                (this->airSupplyStatus->airCommandedOn &&
                 (!this->airSupplyStatus->airValveOpened || this->airSupplyStatus->airValveClosed)) ||
                (!this->airSupplyStatus->airCommandedOn &&
                 (this->airSupplyStatus->airValveOpened || !this->airSupplyStatus->airValveClosed));

        this->cellLightStatus->timestamp = timestamp;
        this->cellLightStatus->cellLightsOn = (fpgaData->DigitalInputStates & 0x0400) != 0;

        this->cellLightWarning->timestamp = timestamp;
        this->cellLightWarning->cellLightsSensorMismatch =
                this->cellLightStatus->cellLightsCommandedOn != this->cellLightStatus->cellLightsOn;

        this->interlockWarning->timestamp = timestamp;
        this->interlockWarning->auxPowerNetworksOff = (fpgaData->DigitalInputStates & 0x0001) == 0;
        this->interlockWarning->thermalEquipmentOff = (fpgaData->DigitalInputStates & 0x0002) == 0;
        this->interlockWarning->airSupplyOff = (fpgaData->DigitalInputStates & 0x0008) == 0;
        this->interlockWarning->cabinetDoorOpen = (fpgaData->DigitalInputStates & 0x0010) == 0;
        this->interlockWarning->tmaMotionStop = (fpgaData->DigitalInputStates & 0x0040) == 0;
        this->interlockWarning->gisHeartbeatLost = (fpgaData->DigitalInputStates & 0x0080) == 0;

        if (this->safetyController) {
            this->safetyController->airControllerNotifyCommandSensorMismatch(
                    this->airSupplyWarning->commandSensorMismatch);
            this->safetyController->cellLightNotifySensorMismatch(
                    this->cellLightWarning->cellLightsSensorMismatch);
            this->safetyController->interlockNotifyAuxPowerNetworksOff(
                    this->interlockWarning->auxPowerNetworksOff);
            this->safetyController->interlockNotifyThermalEquipmentOff(
                    this->interlockWarning->thermalEquipmentOff);
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
    spdlog::trace("DigitalInputOutput: tryToggleHeartbeat()");
    double timestamp = this->publisher->getTimestamp();
    if (timestamp >=
        (this->lastToggleTimestamp + this->interlockApplicationSettings->HeartbeatPeriodInSeconds)) {
        spdlog::debug("DigitalInputOutput: toggleHeartbeat()");
        this->lastToggleTimestamp = timestamp;
        this->interlockStatus->heartbeatCommandedState = !this->interlockStatus->heartbeatCommandedState;
        uint16_t buffer[2] = {FPGAAddresses::HeartbeatToSafetyController,
                              (uint16_t)this->interlockStatus->heartbeatCommandedState};
        IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    }
}

void DigitalInputOutput::turnAirOn() {
    spdlog::info("DigitalInputOutput: turnAirOn()");
    this->airSupplyStatus->airCommandedOn = true;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl,
                          (uint16_t)this->airSupplyStatus->airCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOff() {
    spdlog::info("DigitalInputOutput: turnAirOff()");
    this->airSupplyStatus->airCommandedOn = false;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl,
                          (uint16_t)this->airSupplyStatus->airCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOn() {
    spdlog::info("DigitalInputOutput: turnCellLightsOn()");
    this->cellLightStatus->cellLightsCommandedOn = true;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl,
                          (uint16_t)(!this->cellLightStatus->cellLightsCommandedOn)};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOff() {
    spdlog::info("DigitalInputOutput: turnCellLightsOff()");
    this->cellLightStatus->cellLightsCommandedOn = false;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl,
                          (uint16_t)(!this->cellLightStatus->cellLightsCommandedOn)};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
