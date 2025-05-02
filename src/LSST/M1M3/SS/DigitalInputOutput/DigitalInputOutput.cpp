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

#include <algorithm>
#include <chrono>
#include <cstring>

#include <spdlog/spdlog.h>

#include <SAL_MTM1M3C.h>

#include <Context.h>
#include <DigitalInputOutput.h>
#include <FPGAAddresses.h>
#include <InterlockWarning.h>
#include <M1M3SSPublisher.h>
#include <Range.h>
#include <SafetyController.h>
#include <SettingReader.h>
#include <SupportFPGAData.h>
#include <Timestamp.h>

using namespace LSST::M1M3::SS;
using namespace LSST::M1M3::SS::FPGAAddresses;

const int FREQ = 9;
const int FREQ_BASE = 49;

DigitalInputOutput::DigitalInputOutput(token) {
    SPDLOG_DEBUG("DigitalInputOutput: DigitalInputOutput()");
    _safetyController = 0;

    _airSupplyStatus = &AirSupplyStatus::instance();
    _airSupplyWarning = M1M3SSPublisher::instance().getEventAirSupplyWarning();
    _cellLightStatus = M1M3SSPublisher::instance().getEventCellLightStatus();
    _cellLightWarning = M1M3SSPublisher::instance().getEventCellLightWarning();
    _interlockStatus = &InterlockStatus::instance();

    _lastDITimestamp = 0;
    _lastDOTimestamp = 0;

    _lightToggledTime = std::chrono::steady_clock::now();

    memset(_airSupplyWarning, 0, sizeof(MTM1M3_logevent_airSupplyWarningC));
    memset(_cellLightStatus, 0, sizeof(MTM1M3_logevent_cellLightStatusC));
    memset(_cellLightWarning, 0, sizeof(MTM1M3_logevent_cellLightWarningC));
}

void DigitalInputOutput::setSafetyController(SafetyController *safetyController) {
    SPDLOG_DEBUG("DigitalInputOutput: setSafetyController()");
    _safetyController = safetyController;
}

void DigitalInputOutput::processData() {
    // TODO: Handle no data available
    SPDLOG_TRACE("DigitalInputOutput: processData()");
    bool tryPublish = false;

    if (_safetyController == nullptr && Context::instance().in_standby() == false) {
        throw std::runtime_error("Safety controller must be set in outside of standby state.");
    }

    SupportFPGAData *fpgaData = IFPGA::get().getSupportFPGAData();
    double timestamp =
            Timestamp::fromFPGA(std::max(fpgaData->DigitalOutputTimestamp, fpgaData->DigitalInputTimestamp));
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if (fpgaData->DigitalOutputTimestamp != _lastDOTimestamp) {
        tryPublish = true;
        _lastDOTimestamp = fpgaData->DigitalOutputTimestamp;

        // Polarity is swapped
        bool airMismatch = _airSupplyStatus->setOutputs(
                timestamp, (fpgaData->DigitalOutputStates & DigitalOutputs::AirCommandOutputOn) != 0);

        _airSupplyWarning->timestamp = timestamp;
        _airSupplyWarning->commandOutputMismatch = airMismatch;

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
        _interlockStatus->log();

        InterlockWarning::instance().setHearbeatOutputMismatch(
                timestamp,
                _interlockStatus->heartbeatOutputState != _interlockStatus->heartbeatCommandedState);

        if (Context::instance().in_disabled_or_standby() == false) {
            assert(_safetyController != nullptr);
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

        // Polarity is swapped
        bool airMismatch = _airSupplyStatus->setInputs(
                timestamp, (fpgaData->DigitalInputStates & DigitalInputs::AirValveClosed) == 0,
                (fpgaData->DigitalInputStates & DigitalInputs::AirValveOpened) == 0);

        _airSupplyWarning->timestamp = timestamp;
        _airSupplyWarning->commandSensorMismatch = airMismatch;

        _cellLightStatus->timestamp = timestamp;
        _cellLightStatus->cellLightsOn = (fpgaData->DigitalInputStates & DigitalInputs::CellLightsOn) != 0;

        _cellLightWarning->timestamp = timestamp;
        _cellLightWarning->cellLightsSensorMismatch =
                (now - _lightToggledTime) > std::chrono::milliseconds(100) &&
                _cellLightStatus->cellLightsCommandedOn != _cellLightStatus->cellLightsOn;

        InterlockWarning::instance().setData(timestamp, fpgaData->DigitalInputStates);

        if (Context::instance().in_disabled_or_standby() == false) {
            assert(_safetyController != nullptr);
            // report heartbeat lost first. GIS cuts power if heartbeats aren't
            // comming.
            _safetyController->interlockNotifyGISHeartbeatLost(InterlockWarning::instance().gisHeartbeatLost);

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
        }
    }
    _airSupplyStatus->send();
    if (tryPublish) {
        M1M3SSPublisher::instance().tryLogAirSupplyWarning();
        M1M3SSPublisher::instance().tryLogCellLightStatus();
        M1M3SSPublisher::instance().tryLogCellLightWarning();
    }
}

void DigitalInputOutput::toggleHeartbeat(double globalTimestamp) {
    _interlockStatus->timestamp = globalTimestamp;
    _interlockStatus->heartbeatCommandedState = !_interlockStatus->heartbeatCommandedState;
    uint16_t buffer[2] = {FPGAAddresses::HeartbeatToSafetyController,
                          (uint16_t)_interlockStatus->heartbeatCommandedState};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);

    _interlockStatus->log();
}

void DigitalInputOutput::toggleSystemOperationalHB(int index, bool on) {
    uint16_t buffer[2] = {
            index == 0 ? FPGAAddresses::SystemOperationalHB1 : FPGAAddresses::SystemOperationalHB2, on};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::toggleMirrorRaisedHB(int index, bool on) {
    uint16_t buffer[2] = {index == 0 ? FPGAAddresses::MirrorRaisedHB1 : FPGAAddresses::MirrorRaisedHB2, on};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOn() {
    SPDLOG_INFO("Air valve commanded to open");
    _airSupplyStatus->setAirCommanded(true);
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, true};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
}

void DigitalInputOutput::turnAirOff() {
    SPDLOG_INFO("Air valve commanded to close");
    _airSupplyStatus->setAirCommanded(false);
    uint16_t buffer[2] = {FPGAAddresses::AirSupplyValveControl, false};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
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
