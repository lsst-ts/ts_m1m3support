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

DigitalInputOutput::DigitalInputOutput(InterlockApplicationSettings* interlockApplicationSettings) {
    spdlog::debug("DigitalInputOutput: DigitalInputOutput()");
    _interlockApplicationSettings = interlockApplicationSettings;
    _safetyController = 0;

    M1M3SSPublisher publisher = M1M3SSPublisher::get();

    _airSupplyStatus = publisher.getEventAirSupplyStatus();
    _airSupplyWarning = publisher.getEventAirSupplyWarning();
    _cellLightStatus = publisher.getEventCellLightStatus();
    _cellLightWarning = publisher.getEventCellLightWarning();
    _interlockStatus = publisher.getEventInterlockStatus();
    _interlockWarning = publisher.getEventInterlockWarning();

    _lastDITimestamp = 0;
    _lastDOTimestamp = 0;
    _lastToggleTimestamp = 0;

    memset(_airSupplyStatus, 0, sizeof(MTM1M3_logevent_airSupplyStatusC));
    memset(_airSupplyWarning, 0, sizeof(MTM1M3_logevent_airSupplyWarningC));
    memset(_cellLightStatus, 0, sizeof(MTM1M3_logevent_cellLightStatusC));
    memset(_cellLightWarning, 0, sizeof(MTM1M3_logevent_cellLightWarningC));
    memset(_interlockStatus, 0, sizeof(MTM1M3_logevent_interlockStatusC));
    memset(_interlockWarning, 0, sizeof(MTM1M3_logevent_interlockWarningC));
}

void DigitalInputOutput::setSafetyController(SafetyController* safetyController) {
    spdlog::debug("DigitalInputOutput: setSafetyController()");
    _safetyController = safetyController;
}

void DigitalInputOutput::processData() {
    // TODO: Handle no data available
    spdlog::trace("DigitalInputOutput: processData()");
    bool tryPublish = false;
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    double timestamp = Timestamp::fromFPGA(
            Range::Max(fpgaData->DigitalOutputTimestamp, fpgaData->DigitalInputTimestamp));
    if (fpgaData->DigitalOutputTimestamp != _lastDOTimestamp) {
        tryPublish = true;
        _lastDOTimestamp = fpgaData->DigitalOutputTimestamp;

        _airSupplyStatus->timestamp = timestamp;
        _airSupplyStatus->airCommandOutputOn = (fpgaData->DigitalOutputStates & 0x10) != 0;

        _airSupplyWarning->timestamp = timestamp;
        _airSupplyWarning->commandOutputMismatch =
                _airSupplyStatus->airCommandOutputOn != _airSupplyStatus->airCommandedOn;

        _cellLightStatus->timestamp = timestamp;
        // Polarity is swapped
        _cellLightStatus->cellLightsOutputOn = (fpgaData->DigitalOutputStates & 0x20) == 0;

        _cellLightWarning->timestamp = timestamp;
        _cellLightWarning->cellLightsOutputMismatch =
                _cellLightStatus->cellLightsOutputOn != _cellLightStatus->cellLightsCommandedOn;

        _interlockStatus->timestamp = timestamp;
        _interlockStatus->heartbeatOutputState = (fpgaData->DigitalOutputStates & 0x01) != 0;

        _interlockWarning->timestamp = timestamp;
        _interlockWarning->heartbeatStateOutputMismatch =
                _interlockStatus->heartbeatOutputState != _interlockStatus->heartbeatCommandedState;

        if (_safetyController) {
            _safetyController->airControllerNotifyCommandOutputMismatch(
                    _airSupplyWarning->commandOutputMismatch);
            _safetyController->cellLightNotifyOutputMismatch(_cellLightWarning->cellLightsOutputMismatch);
            _safetyController->interlockNotifyHeartbeatStateOutputMismatch(
                    _interlockWarning->heartbeatStateOutputMismatch);
        }
    }
    if (fpgaData->DigitalInputTimestamp != _lastDITimestamp) {
        tryPublish = true;
        _lastDITimestamp = fpgaData->DigitalInputTimestamp;

        _airSupplyStatus->timestamp = timestamp;
        // Polarity is swapped
        _airSupplyStatus->airValveOpened = (fpgaData->DigitalInputStates & 0x0100) == 0;
        // Polarity is swapped
        _airSupplyStatus->airValveClosed = (fpgaData->DigitalInputStates & 0x0200) == 0;

        _airSupplyWarning->timestamp = timestamp;
        _airSupplyWarning->commandSensorMismatch =
                (_airSupplyStatus->airCommandedOn &&
                 (!_airSupplyStatus->airValveOpened || _airSupplyStatus->airValveClosed)) ||
                (!_airSupplyStatus->airCommandedOn &&
                 (_airSupplyStatus->airValveOpened || !_airSupplyStatus->airValveClosed));

        _cellLightStatus->timestamp = timestamp;
        _cellLightStatus->cellLightsOn = (fpgaData->DigitalInputStates & 0x0400) != 0;

        _cellLightWarning->timestamp = timestamp;
        _cellLightWarning->cellLightsSensorMismatch =
                _cellLightStatus->cellLightsCommandedOn != _cellLightStatus->cellLightsOn;

        _interlockWarning->timestamp = timestamp;
        _interlockWarning->auxPowerNetworksOff = (fpgaData->DigitalInputStates & 0x0001) == 0;
        _interlockWarning->thermalEquipmentOff = (fpgaData->DigitalInputStates & 0x0002) == 0;
        _interlockWarning->airSupplyOff = (fpgaData->DigitalInputStates & 0x0008) == 0;
        _interlockWarning->cabinetDoorOpen = (fpgaData->DigitalInputStates & 0x0010) == 0;
        _interlockWarning->tmaMotionStop = (fpgaData->DigitalInputStates & 0x0040) == 0;
        _interlockWarning->gisHeartbeatLost = (fpgaData->DigitalInputStates & 0x0080) == 0;

        if (_safetyController) {
            _safetyController->airControllerNotifyCommandSensorMismatch(
                    _airSupplyWarning->commandSensorMismatch);
            _safetyController->cellLightNotifySensorMismatch(_cellLightWarning->cellLightsSensorMismatch);
            _safetyController->interlockNotifyAuxPowerNetworksOff(_interlockWarning->auxPowerNetworksOff);
            _safetyController->interlockNotifyThermalEquipmentOff(_interlockWarning->thermalEquipmentOff);
            _safetyController->interlockNotifyAirSupplyOff(_interlockWarning->airSupplyOff);
            _safetyController->interlockNotifyCabinetDoorOpen(_interlockWarning->cabinetDoorOpen);
            _safetyController->interlockNotifyTMAMotionStop(_interlockWarning->tmaMotionStop);
            _safetyController->interlockNotifyGISHeartbeatLost(_interlockWarning->gisHeartbeatLost);
        }
    }
    if (tryPublish) {
        M1M3SSPublisher publisher = M1M3SSPublisher::get();
        publisher.tryLogAirSupplyStatus();
        publisher.tryLogAirSupplyWarning();
        publisher.tryLogCellLightStatus();
        publisher.tryLogCellLightWarning();
        publisher.tryLogInterlockStatus();
        publisher.tryLogInterlockWarning();
    }
}

void DigitalInputOutput::tryToggleHeartbeat() {
    spdlog::trace("DigitalInputOutput: tryToggleHeartbeat()");
    double timestamp = M1M3SSPublisher::get().getTimestamp();
    if (timestamp >= (_lastToggleTimestamp + _interlockApplicationSettings->heartbeatPeriodInSeconds)) {
        spdlog::debug("DigitalInputOutput: toggleHeartbeat()");
        _lastToggleTimestamp = timestamp;
        _interlockStatus->heartbeatCommandedState = !_interlockStatus->heartbeatCommandedState;
        uint16_t buffer[2] = {FPGAAddresses::HeartbeatToSafetyController,
                              (uint16_t)_interlockStatus->heartbeatCommandedState};
        IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    }
}

void DigitalInputOutput::turnAirOn() {
    spdlog::info("DigitalInputOutput: turnAirOn()");
    _airSupplyStatus->airCommandedOn = true;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, (uint16_t)_airSupplyStatus->airCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOff() {
    spdlog::info("DigitalInputOutput: turnAirOff()");
    _airSupplyStatus->airCommandedOn = false;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, (uint16_t)_airSupplyStatus->airCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOn() {
    spdlog::info("DigitalInputOutput: turnCellLightsOn()");
    _cellLightStatus->cellLightsCommandedOn = true;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl,
                          (uint16_t)(!_cellLightStatus->cellLightsCommandedOn)};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnCellLightsOff() {
    spdlog::info("DigitalInputOutput: turnCellLightsOff()");
    _cellLightStatus->cellLightsCommandedOn = false;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl,
                          (uint16_t)(!_cellLightStatus->cellLightsCommandedOn)};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
