/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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
#include <InterlockWarning.h>
#include <SafetyController.h>
#include <SettingReader.h>
#include <Timestamp.h>
#include <Range.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstring>
#include <chrono>

using namespace LSST::M1M3::SS;
using namespace LSST::M1M3::SS::FPGAAddresses;

const float HEARTBEAT_PERIOD = 1.0;  //* Heartbeat period in seconds

DigitalInputOutput::DigitalInputOutput() {
    SPDLOG_DEBUG("DigitalInputOutput: DigitalInputOutput()");
    _safetyController = 0;

    _airSupplyStatus = M1M3SSPublisher::get().getEventAirSupplyStatus();
    _airSupplyWarning = M1M3SSPublisher::get().getEventAirSupplyWarning();
    _cellLightStatus = M1M3SSPublisher::get().getEventCellLightStatus();
    _cellLightWarning = M1M3SSPublisher::get().getEventCellLightWarning();
    _interlockStatus = M1M3SSPublisher::get().getEventInterlockStatus();

    _lastDITimestamp = 0;
    _lastDOTimestamp = 0;
    _lastToggleTimestamp = 0;

    _lightToggledTime = _airToggledTime = std::chrono::steady_clock::now();

    memset(_airSupplyStatus, 0, sizeof(MTM1M3_logevent_airSupplyStatusC));
    memset(_airSupplyWarning, 0, sizeof(MTM1M3_logevent_airSupplyWarningC));
    memset(_cellLightStatus, 0, sizeof(MTM1M3_logevent_cellLightStatusC));
    memset(_cellLightWarning, 0, sizeof(MTM1M3_logevent_cellLightWarningC));
    memset(_interlockStatus, 0, sizeof(MTM1M3_logevent_interlockStatusC));
}

void DigitalInputOutput::setSafetyController(SafetyController* safetyController) {
    SPDLOG_DEBUG("DigitalInputOutput: setSafetyController()");
    _safetyController = safetyController;
}

void DigitalInputOutput::processData() {
    // TODO: Handle no data available
    SPDLOG_TRACE("DigitalInputOutput: processData()");
    bool tryPublish = false;
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    double timestamp =
            Timestamp::fromFPGA(std::max(fpgaData->DigitalOutputTimestamp, fpgaData->DigitalInputTimestamp));
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if (fpgaData->DigitalOutputTimestamp != _lastDOTimestamp) {
        tryPublish = true;
        _lastDOTimestamp = fpgaData->DigitalOutputTimestamp;

        _airSupplyStatus->timestamp = timestamp;
        _airSupplyStatus->airCommandOutputOn =
                (fpgaData->DigitalOutputStates & DigitalOutputs::AirCommandOutputOn) != 0;

        _airSupplyWarning->timestamp = timestamp;
        _airSupplyWarning->commandOutputMismatch =
                _airSupplyStatus->airCommandOutputOn != _airSupplyStatus->airCommandedOn;

        _cellLightStatus->timestamp = timestamp;
        // Polarity is swapped
        _cellLightStatus->cellLightsOutputOn =
                (fpgaData->DigitalOutputStates & DigitalOutputs::CellLightsOutputOn) == 0;

        _cellLightWarning->timestamp = timestamp;
        _cellLightWarning->cellLightsOutputMismatch =
                _cellLightStatus->cellLightsOutputOn != _cellLightStatus->cellLightsCommandedOn;

        _interlockStatus->timestamp = timestamp;
        _interlockStatus->heartbeatOutputState =
                (fpgaData->DigitalOutputStates & DigitalOutputs::HeartbeatOutputState) != 0;

        InterlockWarning::instance().setHearbeatOutputMismatch(
                timestamp,
                _interlockStatus->heartbeatOutputState != _interlockStatus->heartbeatCommandedState);

        if (_safetyController) {
            _safetyController->airControllerNotifyCommandOutputMismatch(
                    _airSupplyWarning->commandOutputMismatch, _airSupplyStatus->airCommandedOn,
                    _airSupplyStatus->airCommandOutputOn);
            _safetyController->cellLightNotifyOutputMismatch(_cellLightWarning->cellLightsOutputMismatch,
                                                             _cellLightStatus->cellLightsCommandedOn,
                                                             _cellLightStatus->cellLightsOutputOn);
            _safetyController->interlockNotifyHeartbeatStateOutputMismatch(
                    InterlockWarning::instance().heartbeatStateOutputMismatch);
        }
    }
    if (fpgaData->DigitalInputTimestamp != _lastDITimestamp) {
        tryPublish = true;
        _lastDITimestamp = fpgaData->DigitalInputTimestamp;

        _airSupplyStatus->timestamp = timestamp;
        // Polarity is swapped
        _airSupplyStatus->airValveOpened =
                (fpgaData->DigitalInputStates & DigitalInputs::AirValveOpened) == 0;
        // Polarity is swapped
        _airSupplyStatus->airValveClosed =
                (fpgaData->DigitalInputStates & DigitalInputs::AirValveClosed) == 0;

        _airSupplyWarning->timestamp = timestamp;
        _airSupplyWarning->commandSensorMismatch =
                (now - _airToggledTime) >
                        std::chrono::seconds(SettingReader::instance()
                                                     .getSafetyControllerSettings()
                                                     ->AirController.ValveTransitionTimeout) &&
                ((_airSupplyStatus->airCommandedOn &&
                  (!_airSupplyStatus->airValveOpened || _airSupplyStatus->airValveClosed)) ||
                 (!_airSupplyStatus->airCommandedOn &&
                  (_airSupplyStatus->airValveOpened || !_airSupplyStatus->airValveClosed)));

        _cellLightStatus->timestamp = timestamp;
        _cellLightStatus->cellLightsOn = (fpgaData->DigitalInputStates & DigitalInputs::CellLightsOn) != 0;

        _cellLightWarning->timestamp = timestamp;
        _cellLightWarning->cellLightsSensorMismatch =
                (now - _lightToggledTime) > std::chrono::milliseconds(100) &&
                _cellLightStatus->cellLightsCommandedOn != _cellLightStatus->cellLightsOn;

        InterlockWarning::instance().setData(timestamp, fpgaData->DigitalInputStates);

        if (_safetyController) {
            _safetyController->airControllerNotifyCommandSensorMismatch(
                    _airSupplyWarning->commandSensorMismatch);
            _safetyController->cellLightNotifySensorMismatch(_cellLightWarning->cellLightsSensorMismatch,
                                                             _cellLightStatus->cellLightsCommandedOn,
                                                             _cellLightStatus->cellLightsOn);
            _safetyController->interlockNotifyAuxPowerNetworksOff(
                    InterlockWarning::instance().auxPowerNetworksOff);
            _safetyController->interlockNotifyThermalEquipmentOff(
                    InterlockWarning::instance().thermalEquipmentOff);
            _safetyController->interlockNotifyAirSupplyOff(InterlockWarning::instance().airSupplyOff);
            _safetyController->interlockNotifyCabinetDoorOpen(InterlockWarning::instance().cabinetDoorOpen);
            _safetyController->interlockNotifyTMAMotionStop(InterlockWarning::instance().tmaMotionStop);
            _safetyController->interlockNotifyGISHeartbeatLost(InterlockWarning::instance().gisHeartbeatLost);
        }
    }
    if (tryPublish) {
        M1M3SSPublisher::get().tryLogAirSupplyStatus();
        M1M3SSPublisher::get().tryLogAirSupplyWarning();
        M1M3SSPublisher::get().tryLogCellLightStatus();
        M1M3SSPublisher::get().tryLogCellLightWarning();
        M1M3SSPublisher::get().tryLogInterlockStatus();
    }
}

void DigitalInputOutput::tryToggleHeartbeat() {
    SPDLOG_TRACE("DigitalInputOutput: tryToggleHeartbeat()");
    double timestamp = M1M3SSPublisher::get().getTimestamp();
    if (timestamp >= (_lastToggleTimestamp + HEARTBEAT_PERIOD)) {
        SPDLOG_DEBUG("DigitalInputOutput: toggleHeartbeat()");
        _lastToggleTimestamp = timestamp;
        _interlockStatus->heartbeatCommandedState = !_interlockStatus->heartbeatCommandedState;
        uint16_t buffer[2] = {FPGAAddresses::HeartbeatToSafetyController,
                              (uint16_t)_interlockStatus->heartbeatCommandedState};
        IFPGA::get().writeCommandFIFO(buffer, 2, 0);

        // sends software heartbeat
        M1M3SSPublisher::get().logHeartbeat();
        M1M3SSPublisher::get().logInterlockStatus();
    }
}

void DigitalInputOutput::turnAirOn() {
    SPDLOG_INFO("DigitalInputOutput: turnAirOn()");
    _airSupplyStatus->airCommandedOn = true;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, (uint16_t)_airSupplyStatus->airCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _airToggledTime = std::chrono::steady_clock::now();
}

void DigitalInputOutput::turnAirOff() {
    SPDLOG_INFO("DigitalInputOutput: turnAirOff()");
    _airSupplyStatus->airCommandedOn = false;
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, (uint16_t)_airSupplyStatus->airCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _airToggledTime = std::chrono::steady_clock::now();
}

void DigitalInputOutput::turnCellLightsOn() {
    SPDLOG_INFO("DigitalInputOutput: turnCellLightsOn()");
    _cellLightStatus->cellLightsCommandedOn = true;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl,
                          (uint16_t)(!_cellLightStatus->cellLightsCommandedOn)};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _lightToggledTime = std::chrono::steady_clock::now();
}

void DigitalInputOutput::turnCellLightsOff() {
    SPDLOG_INFO("DigitalInputOutput: turnCellLightsOff()");
    _cellLightStatus->cellLightsCommandedOn = false;
    // Polarity is swapped
    uint16_t buffer[2] = {FPGAAddresses::MirrorCellLightControl,
                          (uint16_t)(!_cellLightStatus->cellLightsCommandedOn)};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _lightToggledTime = std::chrono::steady_clock::now();
}
