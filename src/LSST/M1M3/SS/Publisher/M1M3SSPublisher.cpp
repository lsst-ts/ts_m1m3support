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

#include <M1M3SSPublisher.h>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSPublisher::M1M3SSPublisher() : _m1m3SAL(NULL) { spdlog::debug("M1M3SSPublisher: M1M3SSPublisher()"); }

M1M3SSPublisher& M1M3SSPublisher::get() {
    static M1M3SSPublisher publisher;
    return publisher;
}

void M1M3SSPublisher::setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL) {
    _m1m3SAL = m1m3SAL;

    spdlog::debug("M1M3SSPublisher: Initializing SAL Telemetry");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_accelerometerData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_forceActuatorData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_gyroData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_hardpointActuatorData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_hardpointMonitorData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_imsData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_inclinometerData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_outerLoopData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_pidData");
    _m1m3SAL->salTelemetryPub((char*)"MTM1M3_powerSupplyData");

    spdlog::debug("M1M3SSPublisher: Initializing SAL Events");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_accelerometerWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_accelerometerWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_airSupplyStatus");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_airSupplyWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedAberrationForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedAccelerationForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedActiveOpticForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedAzimuthForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedBalanceForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedCylinderForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedElevationForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedOffsetForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedSettingsMatchStart");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedStaticForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedThermalForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedVelocityForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_cellLightStatus");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_cellLightWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_commandRejectionWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_detailedState");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_displacementSensorWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_errorCode");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorForceWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorInfo");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorState");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceSetpointWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_gyroWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointActuatorInfo");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointActuatorState");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointActuatorWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointMonitorInfo");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointMonitorState");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointMonitorWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_ilcWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_inclinometerSensorWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_interlockStatus");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_interlockWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_modbusResponse");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_pidInfo");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_powerStatus");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_powerSupplyStatus");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_powerWarning");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedAberrationForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedAccelerationForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedActiveOpticForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedAzimuthForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedBalanceForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedCylinderForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedElevationForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedOffsetForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedStaticForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedThermalForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedVelocityForces");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_settingVersions");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_settingsApplied");
    _m1m3SAL->salEventPub((char*)"MTM1M3_logevent_summaryState");
}

double M1M3SSPublisher::getTimestamp() { return _m1m3SAL->getCurrentTime(); }

void M1M3SSPublisher::putAccelerometerData() { _m1m3SAL->putSample_accelerometerData(&_accelerometerData); }
void M1M3SSPublisher::putForceActuatorData() { _m1m3SAL->putSample_forceActuatorData(&_forceActuatorData); }
void M1M3SSPublisher::putGyroData() { _m1m3SAL->putSample_gyroData(&_gyroData); }
void M1M3SSPublisher::putHardpointActuatorData() {
    _m1m3SAL->putSample_hardpointActuatorData(&_hardpointActuatorData);
}
void M1M3SSPublisher::putHardpointMonitorData() {
    _m1m3SAL->putSample_hardpointMonitorData(&_hardpointMonitorData);
}
void M1M3SSPublisher::putIMSData() { _m1m3SAL->putSample_imsData(&_imsData); }
void M1M3SSPublisher::putInclinometerData() { _m1m3SAL->putSample_inclinometerData(&_inclinometerData); }
void M1M3SSPublisher::putOuterLoopData() { _m1m3SAL->putSample_outerLoopData(&_outerLoopData); }
void M1M3SSPublisher::putPIDData() { _m1m3SAL->putSample_pidData(&_pidData); }
void M1M3SSPublisher::putPowerSupplyData() { _m1m3SAL->putSample_powerSupplyData(&_powerSupplyData); }

void M1M3SSPublisher::logAccelerometerWarning() {
    _eventAccelerometerWarning.anyWarning = _eventAccelerometerWarning.responseTimeout;
    _m1m3SAL->logEvent_accelerometerWarning(&_eventAccelerometerWarning, 0);
    _previousEventAccelerometerWarning = _eventAccelerometerWarning;
}

void M1M3SSPublisher::tryLogAccelerometerWarning() {
    if (_eventAccelerometerWarning.responseTimeout != _previousEventAccelerometerWarning.responseTimeout) {
        this->logAccelerometerWarning();
    }
}

void M1M3SSPublisher::logAirSupplyStatus() {
    _m1m3SAL->logEvent_airSupplyStatus(&_eventAirSupplyStatus, 0);
    _previousEventAirSupplyStatus = _eventAirSupplyStatus;
}

void M1M3SSPublisher::tryLogAirSupplyStatus() {
    if (_eventAirSupplyStatus.airCommandedOn != _previousEventAirSupplyStatus.airCommandedOn ||
        _eventAirSupplyStatus.airCommandOutputOn != _previousEventAirSupplyStatus.airCommandOutputOn ||
        _eventAirSupplyStatus.airValveOpened != _previousEventAirSupplyStatus.airValveOpened ||
        _eventAirSupplyStatus.airValveClosed != _previousEventAirSupplyStatus.airValveClosed) {
        this->logAirSupplyStatus();
    }
}

void M1M3SSPublisher::logAirSupplyWarning() {
    _eventAirSupplyWarning.anyWarning =
            _eventAirSupplyWarning.commandOutputMismatch || _eventAirSupplyWarning.commandSensorMismatch;
    _m1m3SAL->logEvent_airSupplyWarning(&_eventAirSupplyWarning, 0);
    _previousEventAirSupplyWarning = _eventAirSupplyWarning;
}

void M1M3SSPublisher::tryLogAirSupplyWarning() {
    if (_eventAirSupplyWarning.commandOutputMismatch !=
                _previousEventAirSupplyWarning.commandOutputMismatch ||
        _eventAirSupplyWarning.commandSensorMismatch !=
                _previousEventAirSupplyWarning.commandSensorMismatch) {
        this->logAirSupplyWarning();
    }
}

void M1M3SSPublisher::logAppliedAberrationForces() {
    _m1m3SAL->logEvent_appliedAberrationForces(&_eventAppliedAberrationForces, 0);
    _previousEventAppliedAberrationForces = _eventAppliedAberrationForces;
}

void M1M3SSPublisher::tryLogAppliedAberrationForces() {
    bool changeDetected = _eventAppliedAberrationForces.fz != _previousEventAppliedAberrationForces.fz ||
                          _eventAppliedAberrationForces.mx != _previousEventAppliedAberrationForces.mx ||
                          _eventAppliedAberrationForces.my != _previousEventAppliedAberrationForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventAppliedAberrationForces.zForces[i] !=
                                                   _previousEventAppliedAberrationForces.zForces[i];
    }
    if (changeDetected) {
        this->logAppliedAberrationForces();
    }
}

void M1M3SSPublisher::logAppliedAccelerationForces() {
    _m1m3SAL->logEvent_appliedAccelerationForces(&_eventAppliedAccelerationForces, 0);
    _previousEventAppliedAccelerationForces = _eventAppliedAccelerationForces;
}

void M1M3SSPublisher::tryLogAppliedAccelerationForces() {
    bool changeDetected = _eventAppliedAccelerationForces.fx != _previousEventAppliedAccelerationForces.fx ||
                          _eventAppliedAccelerationForces.fy != _previousEventAppliedAccelerationForces.fy ||
                          _eventAppliedAccelerationForces.fz != _previousEventAppliedAccelerationForces.fz ||
                          _eventAppliedAccelerationForces.mx != _previousEventAppliedAccelerationForces.mx ||
                          _eventAppliedAccelerationForces.my != _previousEventAppliedAccelerationForces.my ||
                          _eventAppliedAccelerationForces.mz != _previousEventAppliedAccelerationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedAccelerationForces.xForces[i] !=
                                            _previousEventAppliedAccelerationForces.xForces[i]) ||
                         (i < 100 && _eventAppliedAccelerationForces.yForces[i] !=
                                             _previousEventAppliedAccelerationForces.yForces[i]) ||
                         (_eventAppliedAccelerationForces.zForces[i] !=
                          _previousEventAppliedAccelerationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedAccelerationForces();
    }
}

void M1M3SSPublisher::logAppliedActiveOpticForces() {
    _m1m3SAL->logEvent_appliedActiveOpticForces(&_eventAppliedActiveOpticForces, 0);
    _previousEventAppliedActiveOpticForces = _eventAppliedActiveOpticForces;
}

void M1M3SSPublisher::tryLogAppliedActiveOpticForces() {
    bool changeDetected = _eventAppliedActiveOpticForces.fz != _previousEventAppliedActiveOpticForces.fz ||
                          _eventAppliedActiveOpticForces.mx != _previousEventAppliedActiveOpticForces.mx ||
                          _eventAppliedActiveOpticForces.my != _previousEventAppliedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventAppliedActiveOpticForces.zForces[i] !=
                                                   _previousEventAppliedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        this->logAppliedActiveOpticForces();
    }
}

void M1M3SSPublisher::logAppliedAzimuthForces() {
    _m1m3SAL->logEvent_appliedAzimuthForces(&_eventAppliedAzimuthForces, 0);
    _previousEventAppliedAzimuthForces = _eventAppliedAzimuthForces;
}

void M1M3SSPublisher::tryLogAppliedAzimuthForces() {
    bool changeDetected = _eventAppliedAzimuthForces.fx != _previousEventAppliedAzimuthForces.fx ||
                          _eventAppliedAzimuthForces.fy != _previousEventAppliedAzimuthForces.fy ||
                          _eventAppliedAzimuthForces.fz != _previousEventAppliedAzimuthForces.fz ||
                          _eventAppliedAzimuthForces.mx != _previousEventAppliedAzimuthForces.mx ||
                          _eventAppliedAzimuthForces.my != _previousEventAppliedAzimuthForces.my ||
                          _eventAppliedAzimuthForces.mz != _previousEventAppliedAzimuthForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventAppliedAzimuthForces.xForces[i] != _previousEventAppliedAzimuthForces.xForces[i]) ||
                (i < 100 &&
                 _eventAppliedAzimuthForces.yForces[i] != _previousEventAppliedAzimuthForces.yForces[i]) ||
                (_eventAppliedAzimuthForces.zForces[i] != _previousEventAppliedAzimuthForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedAzimuthForces();
    }
}

void M1M3SSPublisher::logAppliedBalanceForces() {
    _m1m3SAL->logEvent_appliedBalanceForces(&_eventAppliedBalanceForces, 0);
    _previousEventAppliedBalanceForces = _eventAppliedBalanceForces;
}

void M1M3SSPublisher::tryLogAppliedBalanceForces() {
    bool changeDetected = _eventAppliedBalanceForces.fx != _previousEventAppliedBalanceForces.fx ||
                          _eventAppliedBalanceForces.fy != _previousEventAppliedBalanceForces.fy ||
                          _eventAppliedBalanceForces.fz != _previousEventAppliedBalanceForces.fz ||
                          _eventAppliedBalanceForces.mx != _previousEventAppliedBalanceForces.mx ||
                          _eventAppliedBalanceForces.my != _previousEventAppliedBalanceForces.my ||
                          _eventAppliedBalanceForces.mz != _previousEventAppliedBalanceForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventAppliedBalanceForces.xForces[i] != _previousEventAppliedBalanceForces.xForces[i]) ||
                (i < 100 &&
                 _eventAppliedBalanceForces.yForces[i] != _previousEventAppliedBalanceForces.yForces[i]) ||
                (_eventAppliedBalanceForces.zForces[i] != _previousEventAppliedBalanceForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedBalanceForces();
    }
}

void M1M3SSPublisher::logAppliedCylinderForces() {
    _m1m3SAL->logEvent_appliedCylinderForces(&_eventAppliedCylinderForces, 0);
    _previousEventAppliedCylinderForces = _eventAppliedCylinderForces;
}

void M1M3SSPublisher::tryLogAppliedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && _eventAppliedCylinderForces.secondaryCylinderForces[i] !=
                                    _previousEventAppliedCylinderForces.secondaryCylinderForces[i]) ||
                (_eventAppliedCylinderForces.primaryCylinderForces[i] !=
                 _previousEventAppliedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        this->logAppliedCylinderForces();
    }
}

void M1M3SSPublisher::logAppliedElevationForces() {
    _m1m3SAL->logEvent_appliedElevationForces(&this->eventAppliedElevationForces, 0);
    _previousEventAppliedElevationForces = this->eventAppliedElevationForces;
}

void M1M3SSPublisher::tryLogAppliedElevationForces() {
    bool changeDetected = this->eventAppliedElevationForces.fx != _previousEventAppliedElevationForces.fx ||
                          this->eventAppliedElevationForces.fy != _previousEventAppliedElevationForces.fy ||
                          this->eventAppliedElevationForces.fz != _previousEventAppliedElevationForces.fz ||
                          this->eventAppliedElevationForces.mx != _previousEventAppliedElevationForces.mx ||
                          this->eventAppliedElevationForces.my != _previousEventAppliedElevationForces.my ||
                          this->eventAppliedElevationForces.mz != _previousEventAppliedElevationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedElevationForces.xForces[i] !=
                                            _previousEventAppliedElevationForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedElevationForces.yForces[i] !=
                                             _previousEventAppliedElevationForces.yForces[i]) ||
                         (this->eventAppliedElevationForces.zForces[i] !=
                          _previousEventAppliedElevationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedElevationForces();
    }
}

void M1M3SSPublisher::logAppliedForces() {
    _m1m3SAL->logEvent_appliedForces(&_eventAppliedForces, 0);
    _previousEventAppliedForces = _eventAppliedForces;
}

void M1M3SSPublisher::tryLogAppliedForces() {
    bool changeDetected = _eventAppliedForces.fx != _previousEventAppliedForces.fx ||
                          _eventAppliedForces.fy != _previousEventAppliedForces.fy ||
                          _eventAppliedForces.fz != _previousEventAppliedForces.fz ||
                          _eventAppliedForces.mx != _previousEventAppliedForces.mx ||
                          _eventAppliedForces.my != _previousEventAppliedForces.my ||
                          _eventAppliedForces.mz != _previousEventAppliedForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 && _eventAppliedForces.xForces[i] != _previousEventAppliedForces.xForces[i]) ||
                (i < 100 && _eventAppliedForces.yForces[i] != _previousEventAppliedForces.yForces[i]) ||
                (_eventAppliedForces.zForces[i] != _previousEventAppliedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedForces();
    }
}

void M1M3SSPublisher::logAppliedOffsetForces() {
    _m1m3SAL->logEvent_appliedOffsetForces(&_eventAppliedOffsetForces, 0);
    _previousEventAppliedOffsetForces = _eventAppliedOffsetForces;
}

void M1M3SSPublisher::tryLogAppliedOffsetForces() {
    bool changeDetected = _eventAppliedOffsetForces.fx != _previousEventAppliedOffsetForces.fx ||
                          _eventAppliedOffsetForces.fy != _previousEventAppliedOffsetForces.fy ||
                          _eventAppliedOffsetForces.fz != _previousEventAppliedOffsetForces.fz ||
                          _eventAppliedOffsetForces.mx != _previousEventAppliedOffsetForces.mx ||
                          _eventAppliedOffsetForces.my != _previousEventAppliedOffsetForces.my ||
                          _eventAppliedOffsetForces.mz != _previousEventAppliedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventAppliedOffsetForces.xForces[i] != _previousEventAppliedOffsetForces.xForces[i]) ||
                (i < 100 &&
                 _eventAppliedOffsetForces.yForces[i] != _previousEventAppliedOffsetForces.yForces[i]) ||
                (_eventAppliedOffsetForces.zForces[i] != _previousEventAppliedOffsetForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedOffsetForces();
    }
}

void M1M3SSPublisher::logAppliedSettingsMatchStart() {
    _m1m3SAL->logEvent_appliedSettingsMatchStart(&_eventAppliedSettingsMatchStart, 0);
    _previousEventAppliedSettingsMatchStart = _eventAppliedSettingsMatchStart;
}

void M1M3SSPublisher::tryLogAppliedSettingsMatchStart() {
    if (_eventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue !=
        _previousEventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue) {
        this->logAppliedSettingsMatchStart();
    }
}

void M1M3SSPublisher::logAppliedStaticForces() {
    _m1m3SAL->logEvent_appliedStaticForces(&this->eventAppliedStaticForces, 0);
    _previousEventAppliedStaticForces = this->eventAppliedStaticForces;
}

void M1M3SSPublisher::tryLogAppliedStaticForces() {
    bool changeDetected = this->eventAppliedStaticForces.fx != _previousEventAppliedStaticForces.fx ||
                          this->eventAppliedStaticForces.fy != _previousEventAppliedStaticForces.fy ||
                          this->eventAppliedStaticForces.fz != _previousEventAppliedStaticForces.fz ||
                          this->eventAppliedStaticForces.mx != _previousEventAppliedStaticForces.mx ||
                          this->eventAppliedStaticForces.my != _previousEventAppliedStaticForces.my ||
                          this->eventAppliedStaticForces.mz != _previousEventAppliedStaticForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 this->eventAppliedStaticForces.xForces[i] != _previousEventAppliedStaticForces.xForces[i]) ||
                (i < 100 &&
                 this->eventAppliedStaticForces.yForces[i] != _previousEventAppliedStaticForces.yForces[i]) ||
                (this->eventAppliedStaticForces.zForces[i] != _previousEventAppliedStaticForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedStaticForces();
    }
}

void M1M3SSPublisher::logAppliedThermalForces() {
    _m1m3SAL->logEvent_appliedThermalForces(&_eventAppliedThermalForces, 0);
    _previousEventAppliedThermalForces = _eventAppliedThermalForces;
}

void M1M3SSPublisher::tryLogAppliedThermalForces() {
    bool changeDetected = _eventAppliedThermalForces.fx != _previousEventAppliedThermalForces.fx ||
                          _eventAppliedThermalForces.fy != _previousEventAppliedThermalForces.fy ||
                          _eventAppliedThermalForces.fz != _previousEventAppliedThermalForces.fz ||
                          _eventAppliedThermalForces.mx != _previousEventAppliedThermalForces.mx ||
                          _eventAppliedThermalForces.my != _previousEventAppliedThermalForces.my ||
                          _eventAppliedThermalForces.mz != _previousEventAppliedThermalForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventAppliedThermalForces.xForces[i] != _previousEventAppliedThermalForces.xForces[i]) ||
                (i < 100 &&
                 _eventAppliedThermalForces.yForces[i] != _previousEventAppliedThermalForces.yForces[i]) ||
                (_eventAppliedThermalForces.zForces[i] != _previousEventAppliedThermalForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedThermalForces();
    }
}

void M1M3SSPublisher::logAppliedVelocityForces() {
    _m1m3SAL->logEvent_appliedVelocityForces(&_eventAppliedVelocityForces, 0);
    _previousEventAppliedVelocityForces = _eventAppliedVelocityForces;
}

void M1M3SSPublisher::tryLogAppliedVelocityForces() {
    bool changeDetected = _eventAppliedVelocityForces.fx != _previousEventAppliedVelocityForces.fx ||
                          _eventAppliedVelocityForces.fy != _previousEventAppliedVelocityForces.fy ||
                          _eventAppliedVelocityForces.fz != _previousEventAppliedVelocityForces.fz ||
                          _eventAppliedVelocityForces.mx != _previousEventAppliedVelocityForces.mx ||
                          _eventAppliedVelocityForces.my != _previousEventAppliedVelocityForces.my ||
                          _eventAppliedVelocityForces.mz != _previousEventAppliedVelocityForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventAppliedVelocityForces.xForces[i] != _previousEventAppliedVelocityForces.xForces[i]) ||
                (i < 100 &&
                 _eventAppliedVelocityForces.yForces[i] != _previousEventAppliedVelocityForces.yForces[i]) ||
                (_eventAppliedVelocityForces.zForces[i] != _previousEventAppliedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedVelocityForces();
    }
}

void M1M3SSPublisher::logCellLightStatus() {
    _m1m3SAL->logEvent_cellLightStatus(&_eventCellLightStatus, 0);
    _previousEventCellLightStatus = _eventCellLightStatus;
}

void M1M3SSPublisher::tryLogCellLightStatus() {
    if (_eventCellLightStatus.cellLightsCommandedOn != _previousEventCellLightStatus.cellLightsCommandedOn ||
        _eventCellLightStatus.cellLightsOutputOn != _previousEventCellLightStatus.cellLightsOutputOn ||
        _eventCellLightStatus.cellLightsOn != _previousEventCellLightStatus.cellLightsOn) {
        this->logCellLightStatus();
    }
}

void M1M3SSPublisher::logCellLightWarning() {
    _eventCellLightWarning.anyWarning = _eventCellLightWarning.cellLightsOutputMismatch ||
                                        _eventCellLightWarning.cellLightsSensorMismatch;
    _m1m3SAL->logEvent_cellLightWarning(&_eventCellLightWarning, 0);
    _previousEventCellLightWarning = _eventCellLightWarning;
}

void M1M3SSPublisher::tryLogCellLightWarning() {
    if (_eventCellLightWarning.cellLightsOutputMismatch !=
                _previousEventCellLightWarning.cellLightsOutputMismatch ||
        _eventCellLightWarning.cellLightsSensorMismatch !=
                _previousEventCellLightWarning.cellLightsSensorMismatch) {
        this->logCellLightWarning();
    }
}

void M1M3SSPublisher::logCommandRejectionWarning() {
    _m1m3SAL->logEvent_commandRejectionWarning(&_eventCommandRejectionWarning, 0);
    _previousEventCommandRejectionWarning = _eventCommandRejectionWarning;
}

void M1M3SSPublisher::tryLogCommandRejectionWarning() {
    if (_eventCommandRejectionWarning.command.compare(_previousEventCommandRejectionWarning.command) != 0 ||
        _eventCommandRejectionWarning.reason.compare(_previousEventCommandRejectionWarning.reason) != 0) {
        this->logCommandRejectionWarning();
    }
}

void M1M3SSPublisher::logCommandRejectionWarning(std::string command, std::string reason) {
    _eventCommandRejectionWarning.timestamp = this->getTimestamp();
    _eventCommandRejectionWarning.command = command;
    _eventCommandRejectionWarning.reason = reason;
    this->logCommandRejectionWarning();
}

void M1M3SSPublisher::tryLogCommandRejectionWarning(std::string command, std::string reason) {
    _eventCommandRejectionWarning.timestamp = this->getTimestamp();
    _eventCommandRejectionWarning.command = command;
    _eventCommandRejectionWarning.reason = reason;
    this->tryLogCommandRejectionWarning();
}

void M1M3SSPublisher::logDetailedState() {
    _m1m3SAL->logEvent_detailedState(&_eventDetailedState, 0);
    _previousEventDetailedState = _eventDetailedState;
}

void M1M3SSPublisher::tryLogDetailedState() {
    if (_eventDetailedState.detailedState != _previousEventDetailedState.detailedState) {
        this->logDetailedState();
    }
}

void M1M3SSPublisher::logDisplacementSensorWarning() {
    _eventDisplacementSensorWarning.anyWarning =
            _eventDisplacementSensorWarning.sensorReportsInvalidCommand ||
            _eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
            _eventDisplacementSensorWarning.sensorReportsDataLengthError ||
            _eventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
            _eventDisplacementSensorWarning.sensorReportsParameterError ||
            _eventDisplacementSensorWarning.sensorReportsCommunicationError ||
            _eventDisplacementSensorWarning.sensorReportsIDNumberError ||
            _eventDisplacementSensorWarning.sensorReportsExpansionLineError ||
            _eventDisplacementSensorWarning.sensorReportsWriteControlError ||
            _eventDisplacementSensorWarning.responseTimeout ||
            _eventDisplacementSensorWarning.invalidLength ||
            _eventDisplacementSensorWarning.invalidResponse ||
            _eventDisplacementSensorWarning.unknownCommand || _eventDisplacementSensorWarning.unknownProblem;
    _m1m3SAL->logEvent_displacementSensorWarning(&_eventDisplacementSensorWarning, 0);
    _previousEventDisplacementSensorWarning = _eventDisplacementSensorWarning;
}

void M1M3SSPublisher::tryLogDisplacementSensorWarning() {
    if (_eventDisplacementSensorWarning.sensorReportsInvalidCommand !=
                _previousEventDisplacementSensorWarning.sensorReportsInvalidCommand ||
        _eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError !=
                _previousEventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
        _eventDisplacementSensorWarning.sensorReportsDataLengthError !=
                _previousEventDisplacementSensorWarning.sensorReportsDataLengthError ||
        _eventDisplacementSensorWarning.sensorReportsNumberOfParametersError !=
                _previousEventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
        _eventDisplacementSensorWarning.sensorReportsParameterError !=
                _previousEventDisplacementSensorWarning.sensorReportsParameterError ||
        _eventDisplacementSensorWarning.sensorReportsCommunicationError !=
                _previousEventDisplacementSensorWarning.sensorReportsCommunicationError ||
        _eventDisplacementSensorWarning.sensorReportsIDNumberError !=
                _previousEventDisplacementSensorWarning.sensorReportsIDNumberError ||
        _eventDisplacementSensorWarning.sensorReportsExpansionLineError !=
                _previousEventDisplacementSensorWarning.sensorReportsExpansionLineError ||
        _eventDisplacementSensorWarning.sensorReportsWriteControlError !=
                _previousEventDisplacementSensorWarning.sensorReportsWriteControlError ||
        _eventDisplacementSensorWarning.responseTimeout !=
                _previousEventDisplacementSensorWarning.responseTimeout ||
        _eventDisplacementSensorWarning.invalidLength !=
                _previousEventDisplacementSensorWarning.invalidLength ||
        _eventDisplacementSensorWarning.invalidResponse !=
                _previousEventDisplacementSensorWarning.invalidResponse ||
        _eventDisplacementSensorWarning.unknownCommand !=
                _previousEventDisplacementSensorWarning.unknownCommand ||
        _eventDisplacementSensorWarning.unknownProblem !=
                _previousEventDisplacementSensorWarning.unknownProblem) {
        this->logDisplacementSensorWarning();
    }
}

void M1M3SSPublisher::logErrorCode() {
    _m1m3SAL->logEvent_errorCode(&_eventErrorCode, 0);
    _previousEventErrorCode = _eventErrorCode;
}

void M1M3SSPublisher::tryLogErrorCode() {
    if (_eventErrorCode.errorCode != _previousEventErrorCode.errorCode) {
        this->logErrorCode();
    }
}

void M1M3SSPublisher::logForceActuatorForceWarning() {
    _eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning = false;
    _eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning = false;
    _eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning = false;
    _eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        _eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning =
                _eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning ||
                _eventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i];
        _eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning =
                _eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning ||
                _eventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i];
        _eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning =
                _eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning ||
                _eventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i];
        _eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning =
                _eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning ||
                _eventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i];
    }
    _eventForceActuatorForceWarning.anyWarning =
            _eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning ||
            _eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning ||
            _eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning ||
            _eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning;
    _m1m3SAL->logEvent_forceActuatorForceWarning(&_eventForceActuatorForceWarning, 0);
    _previousEventForceActuatorForceWarning = _eventForceActuatorForceWarning;
}

void M1M3SSPublisher::tryLogForceActuatorForceWarning() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                _eventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i] !=
                        _previousEventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i] ||
                _eventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i] !=
                        _previousEventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i] ||
                _eventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i] !=
                        _previousEventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i] ||
                _eventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i] !=
                        _previousEventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i];
    }
    if (changeDetected) {
        this->logForceActuatorForceWarning();
    }
}

void M1M3SSPublisher::logForceActuatorInfo() {
    _m1m3SAL->logEvent_forceActuatorInfo(&_eventForceActuatorInfo, 0);
    _previousEventForceActuatorInfo = _eventForceActuatorInfo;
}

void M1M3SSPublisher::tryLogForceActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                _eventForceActuatorInfo.referenceId[i] != _previousEventForceActuatorInfo.referenceId[i] ||
                _eventForceActuatorInfo.xDataReferenceId[i] !=
                        _previousEventForceActuatorInfo.xDataReferenceId[i] ||
                _eventForceActuatorInfo.yDataReferenceId[i] !=
                        _previousEventForceActuatorInfo.yDataReferenceId[i] ||
                _eventForceActuatorInfo.zDataReferenceId[i] !=
                        _previousEventForceActuatorInfo.zDataReferenceId[i] ||
                _eventForceActuatorInfo.actuatorType[i] != _previousEventForceActuatorInfo.actuatorType[i] ||
                _eventForceActuatorInfo.actuatorOrientation[i] !=
                        _previousEventForceActuatorInfo.actuatorOrientation[i] ||
                _eventForceActuatorInfo.modbusSubnet[i] != _previousEventForceActuatorInfo.modbusSubnet[i] ||
                _eventForceActuatorInfo.modbusAddress[i] !=
                        _previousEventForceActuatorInfo.modbusAddress[i] ||
                _eventForceActuatorInfo.xPosition[i] != _previousEventForceActuatorInfo.xPosition[i] ||
                _eventForceActuatorInfo.yPosition[i] != _previousEventForceActuatorInfo.yPosition[i] ||
                _eventForceActuatorInfo.zPosition[i] != _previousEventForceActuatorInfo.zPosition[i] ||
                _eventForceActuatorInfo.ilcUniqueId[i] != _previousEventForceActuatorInfo.ilcUniqueId[i] ||
                _eventForceActuatorInfo.ilcApplicationType[i] !=
                        _previousEventForceActuatorInfo.ilcApplicationType[i] ||
                _eventForceActuatorInfo.networkNodeType[i] !=
                        _previousEventForceActuatorInfo.networkNodeType[i] ||
                _eventForceActuatorInfo.ilcSelectedOptions[i] !=
                        _previousEventForceActuatorInfo.ilcSelectedOptions[i] ||
                _eventForceActuatorInfo.networkNodeOptions[i] !=
                        _previousEventForceActuatorInfo.networkNodeOptions[i] ||
                _eventForceActuatorInfo.majorRevision[i] !=
                        _previousEventForceActuatorInfo.majorRevision[i] ||
                _eventForceActuatorInfo.minorRevision[i] !=
                        _previousEventForceActuatorInfo.minorRevision[i] ||
                _eventForceActuatorInfo.adcScanRate[i] != _previousEventForceActuatorInfo.adcScanRate[i] ||
                _eventForceActuatorInfo.mainPrimaryCylinderCoefficient[i] !=
                        _previousEventForceActuatorInfo.mainPrimaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.mainSecondaryCylinderCoefficient[i] !=
                        _previousEventForceActuatorInfo.mainSecondaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.mainPrimaryCylinderLoadCellOffset[i] !=
                        _previousEventForceActuatorInfo.mainPrimaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.mainSecondaryCylinderLoadCellOffset[i] !=
                        _previousEventForceActuatorInfo.mainSecondaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.mainPrimaryCylinderLoadCellSensitivity[i] !=
                        _previousEventForceActuatorInfo.mainPrimaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.mainSecondaryCylinderLoadCellSensitivity[i] !=
                        _previousEventForceActuatorInfo.mainSecondaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.backupPrimaryCylinderCoefficient[i] !=
                        _previousEventForceActuatorInfo.backupPrimaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.backupSecondaryCylinderCoefficient[i] !=
                        _previousEventForceActuatorInfo.backupSecondaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.backupPrimaryCylinderLoadCellOffset[i] !=
                        _previousEventForceActuatorInfo.backupPrimaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.backupSecondaryCylinderLoadCellOffset[i] !=
                        _previousEventForceActuatorInfo.backupSecondaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.backupPrimaryCylinderLoadCellSensitivity[i] !=
                        _previousEventForceActuatorInfo.backupPrimaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.backupSecondaryCylinderLoadCellSensitivity[i] !=
                        _previousEventForceActuatorInfo.backupSecondaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.mezzaninePrimaryCylinderGain[i] !=
                        _previousEventForceActuatorInfo.mezzaninePrimaryCylinderGain[i] ||
                _eventForceActuatorInfo.mezzanineSecondaryCylinderGain[i] !=
                        _previousEventForceActuatorInfo.mezzanineSecondaryCylinderGain[i] ||
                _eventForceActuatorInfo.mezzanineUniqueId[i] !=
                        _previousEventForceActuatorInfo.mezzanineUniqueId[i] ||
                _eventForceActuatorInfo.mezzanineFirmwareType[i] !=
                        _previousEventForceActuatorInfo.mezzanineFirmwareType[i] ||
                _eventForceActuatorInfo.mezzanineMajorRevision[i] !=
                        _previousEventForceActuatorInfo.mezzanineMajorRevision[i] ||
                _eventForceActuatorInfo.mezzanineMinorRevision[i] !=
                        _previousEventForceActuatorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        this->logForceActuatorInfo();
    }
}

void M1M3SSPublisher::logForceActuatorState() {
    _m1m3SAL->logEvent_forceActuatorState(&_eventForceActuatorState, 0);
    _previousEventForceActuatorState = _eventForceActuatorState;
}

void M1M3SSPublisher::tryLogForceActuatorState() {
    bool changeDetected =
            _eventForceActuatorState.slewFlag != _previousEventForceActuatorState.slewFlag ||
            _eventForceActuatorState.staticForcesApplied !=
                    _previousEventForceActuatorState.staticForcesApplied ||
            _eventForceActuatorState.elevationForcesApplied !=
                    _previousEventForceActuatorState.elevationForcesApplied ||
            _eventForceActuatorState.azimuthForcesApplied !=
                    _previousEventForceActuatorState.azimuthForcesApplied ||
            _eventForceActuatorState.thermalForcesApplied !=
                    _previousEventForceActuatorState.thermalForcesApplied ||
            _eventForceActuatorState.offsetForcesApplied !=
                    _previousEventForceActuatorState.offsetForcesApplied ||
            _eventForceActuatorState.accelerationForcesApplied !=
                    _previousEventForceActuatorState.accelerationForcesApplied ||
            _eventForceActuatorState.velocityForcesApplied !=
                    _previousEventForceActuatorState.velocityForcesApplied ||
            _eventForceActuatorState.activeOpticForcesApplied !=
                    _previousEventForceActuatorState.activeOpticForcesApplied ||
            _eventForceActuatorState.aberrationForcesApplied !=
                    _previousEventForceActuatorState.aberrationForcesApplied ||
            _eventForceActuatorState.balanceForcesApplied !=
                    _previousEventForceActuatorState.balanceForcesApplied ||
            _eventForceActuatorState.supportPercentage != _previousEventForceActuatorState.supportPercentage;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventForceActuatorState.ilcState[i] != _previousEventForceActuatorState.ilcState[i];
    }
    if (changeDetected) {
        this->logForceActuatorState();
    }
}

void M1M3SSPublisher::logForceActuatorWarning() {
    _eventForceActuatorWarning.anyMajorFault = false;
    _eventForceActuatorWarning.anyMinorFault = false;
    _eventForceActuatorWarning.anyFaultOverride = false;
    _eventForceActuatorWarning.anyMainCalibrationError = false;
    _eventForceActuatorWarning.anyBackupCalibrationError = false;
    _eventForceActuatorWarning.anyMezzanineError = false;
    _eventForceActuatorWarning.anyMezzanineBootloaderActive = false;
    _eventForceActuatorWarning.anyUniqueIdCRCError = false;
    _eventForceActuatorWarning.anyApplicationTypeMismatch = false;
    _eventForceActuatorWarning.anyApplicationMissing = false;
    _eventForceActuatorWarning.anyApplicationCRCMismatch = false;
    _eventForceActuatorWarning.anyOneWireMissing = false;
    _eventForceActuatorWarning.anyOneWire1Mismatch = false;
    _eventForceActuatorWarning.anyOneWire2Mismatch = false;
    _eventForceActuatorWarning.anyWatchdogReset = false;
    _eventForceActuatorWarning.anyBrownOut = false;
    _eventForceActuatorWarning.anyEventTrapReset = false;
    _eventForceActuatorWarning.anySSRPowerFault = false;
    _eventForceActuatorWarning.anyAuxPowerFault = false;
    _eventForceActuatorWarning.anyMezzaninePowerFault = false;
    _eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault = false;
    _eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault = false;
    _eventForceActuatorWarning.anyMezzanineUniqueIdCRCError = false;
    _eventForceActuatorWarning.anyMezzanineMainCalibrationError = false;
    _eventForceActuatorWarning.anyMezzanineBackupCalibrationError = false;
    _eventForceActuatorWarning.anyMezzanineEventTrapReset = false;
    _eventForceActuatorWarning.anyMezzanineApplicationMissing = false;
    _eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch = false;
    _eventForceActuatorWarning.anyILCFault = false;
    _eventForceActuatorWarning.anyBroadcastCounterWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        _eventForceActuatorWarning.anyMajorFault =
                _eventForceActuatorWarning.anyMajorFault || _eventForceActuatorWarning.majorFault[i];
        _eventForceActuatorWarning.anyMinorFault =
                _eventForceActuatorWarning.anyMinorFault || _eventForceActuatorWarning.minorFault[i];
        _eventForceActuatorWarning.anyFaultOverride =
                _eventForceActuatorWarning.anyFaultOverride || _eventForceActuatorWarning.faultOverride[i];
        _eventForceActuatorWarning.anyMainCalibrationError =
                _eventForceActuatorWarning.anyMainCalibrationError ||
                _eventForceActuatorWarning.mainCalibrationError[i];
        _eventForceActuatorWarning.anyBackupCalibrationError =
                _eventForceActuatorWarning.anyBackupCalibrationError ||
                _eventForceActuatorWarning.backupCalibrationError[i];
        _eventForceActuatorWarning.anyMezzanineError =
                _eventForceActuatorWarning.anyMezzanineError || _eventForceActuatorWarning.mezzanineError[i];
        _eventForceActuatorWarning.anyMezzanineBootloaderActive =
                _eventForceActuatorWarning.anyMezzanineBootloaderActive ||
                _eventForceActuatorWarning.mezzanineBootloaderActive[i];
        _eventForceActuatorWarning.anyUniqueIdCRCError = _eventForceActuatorWarning.anyUniqueIdCRCError ||
                                                         _eventForceActuatorWarning.uniqueIdCRCError[i];
        _eventForceActuatorWarning.anyApplicationTypeMismatch =
                _eventForceActuatorWarning.anyApplicationTypeMismatch ||
                _eventForceActuatorWarning.applicationTypeMismatch[i];
        _eventForceActuatorWarning.anyApplicationMissing = _eventForceActuatorWarning.anyApplicationMissing ||
                                                           _eventForceActuatorWarning.applicationMissing[i];
        _eventForceActuatorWarning.anyApplicationCRCMismatch =
                _eventForceActuatorWarning.anyApplicationCRCMismatch ||
                _eventForceActuatorWarning.applicationCRCMismatch[i];
        _eventForceActuatorWarning.anyOneWireMissing =
                _eventForceActuatorWarning.anyOneWireMissing || _eventForceActuatorWarning.oneWireMissing[i];
        _eventForceActuatorWarning.anyOneWire1Mismatch = _eventForceActuatorWarning.anyOneWire1Mismatch ||
                                                         _eventForceActuatorWarning.oneWire1Mismatch[i];
        _eventForceActuatorWarning.anyOneWire2Mismatch = _eventForceActuatorWarning.anyOneWire2Mismatch ||
                                                         _eventForceActuatorWarning.oneWire2Mismatch[i];
        _eventForceActuatorWarning.anyWatchdogReset =
                _eventForceActuatorWarning.anyWatchdogReset || _eventForceActuatorWarning.watchdogReset[i];
        _eventForceActuatorWarning.anyBrownOut =
                _eventForceActuatorWarning.anyBrownOut || _eventForceActuatorWarning.brownOut[i];
        _eventForceActuatorWarning.anyEventTrapReset =
                _eventForceActuatorWarning.anyEventTrapReset || _eventForceActuatorWarning.eventTrapReset[i];
        _eventForceActuatorWarning.anySSRPowerFault =
                _eventForceActuatorWarning.anySSRPowerFault || _eventForceActuatorWarning.ssrPowerFault[i];
        _eventForceActuatorWarning.anyAuxPowerFault =
                _eventForceActuatorWarning.anyAuxPowerFault || _eventForceActuatorWarning.auxPowerFault[i];
        _eventForceActuatorWarning.anyMezzaninePowerFault =
                _eventForceActuatorWarning.anyMezzaninePowerFault ||
                _eventForceActuatorWarning.mezzaninePowerFault[i];
        _eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault =
                _eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault ||
                _eventForceActuatorWarning.mezzanineCurrentAmp1Fault[i];
        _eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault =
                _eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault ||
                _eventForceActuatorWarning.mezzanineCurrentAmp2Fault[i];
        _eventForceActuatorWarning.anyMezzanineUniqueIdCRCError =
                _eventForceActuatorWarning.anyMezzanineUniqueIdCRCError ||
                _eventForceActuatorWarning.mezzanineUniqueIdCRCError[i];
        _eventForceActuatorWarning.anyMezzanineMainCalibrationError =
                _eventForceActuatorWarning.anyMezzanineMainCalibrationError ||
                _eventForceActuatorWarning.mezzanineMainCalibrationError[i];
        _eventForceActuatorWarning.anyMezzanineBackupCalibrationError =
                _eventForceActuatorWarning.anyMezzanineBackupCalibrationError ||
                _eventForceActuatorWarning.mezzanineBackupCalibrationError[i];
        _eventForceActuatorWarning.anyMezzanineEventTrapReset =
                _eventForceActuatorWarning.anyMezzanineEventTrapReset ||
                _eventForceActuatorWarning.mezzanineEventTrapReset[i];
        _eventForceActuatorWarning.anyMezzanineApplicationMissing =
                _eventForceActuatorWarning.anyMezzanineApplicationMissing ||
                _eventForceActuatorWarning.mezzanineApplicationMissing[i];
        _eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch =
                _eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch ||
                _eventForceActuatorWarning.mezzanineApplicationCRCMismatch[i];
        _eventForceActuatorWarning.anyILCFault =
                _eventForceActuatorWarning.anyILCFault || _eventForceActuatorWarning.ilcFault[i];
        _eventForceActuatorWarning.anyBroadcastCounterWarning =
                _eventForceActuatorWarning.anyBroadcastCounterWarning ||
                _eventForceActuatorWarning.broadcastCounterWarning[i];
    }
    _eventForceActuatorWarning.anyWarning =
            _eventForceActuatorWarning.anyMajorFault || _eventForceActuatorWarning.anyMinorFault ||
            _eventForceActuatorWarning.anyFaultOverride ||
            _eventForceActuatorWarning.anyMainCalibrationError ||
            _eventForceActuatorWarning.anyBackupCalibrationError ||
            _eventForceActuatorWarning.anyMezzanineError ||
            _eventForceActuatorWarning.anyMezzanineBootloaderActive ||
            _eventForceActuatorWarning.anyUniqueIdCRCError ||
            _eventForceActuatorWarning.anyApplicationTypeMismatch ||
            _eventForceActuatorWarning.anyApplicationMissing ||
            _eventForceActuatorWarning.anyApplicationCRCMismatch ||
            _eventForceActuatorWarning.anyOneWireMissing || _eventForceActuatorWarning.anyOneWire1Mismatch ||
            _eventForceActuatorWarning.anyOneWire2Mismatch || _eventForceActuatorWarning.anyWatchdogReset ||
            _eventForceActuatorWarning.anyBrownOut || _eventForceActuatorWarning.anyEventTrapReset ||
            _eventForceActuatorWarning.anySSRPowerFault || _eventForceActuatorWarning.anyAuxPowerFault ||
            _eventForceActuatorWarning.anyMezzaninePowerFault ||
            _eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault ||
            _eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault ||
            _eventForceActuatorWarning.anyMezzanineUniqueIdCRCError ||
            _eventForceActuatorWarning.anyMezzanineMainCalibrationError ||
            _eventForceActuatorWarning.anyMezzanineBackupCalibrationError ||
            _eventForceActuatorWarning.anyMezzanineEventTrapReset ||
            _eventForceActuatorWarning.anyMezzanineApplicationMissing ||
            _eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch ||
            _eventForceActuatorWarning.anyILCFault || _eventForceActuatorWarning.anyBroadcastCounterWarning;
    _m1m3SAL->logEvent_forceActuatorWarning(&_eventForceActuatorWarning, 0);
    _previousEventForceActuatorWarning = _eventForceActuatorWarning;
}

void M1M3SSPublisher::tryLogForceActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                _eventForceActuatorWarning.majorFault[i] !=
                        _previousEventForceActuatorWarning.majorFault[i] ||
                _eventForceActuatorWarning.minorFault[i] !=
                        _previousEventForceActuatorWarning.minorFault[i] ||
                _eventForceActuatorWarning.faultOverride[i] !=
                        _previousEventForceActuatorWarning.faultOverride[i] ||
                _eventForceActuatorWarning.mainCalibrationError[i] !=
                        _previousEventForceActuatorWarning.mainCalibrationError[i] ||
                _eventForceActuatorWarning.backupCalibrationError[i] !=
                        _previousEventForceActuatorWarning.backupCalibrationError[i] ||
                _eventForceActuatorWarning.mezzanineError[i] !=
                        _previousEventForceActuatorWarning.mezzanineError[i] ||
                _eventForceActuatorWarning.mezzanineBootloaderActive[i] !=
                        _previousEventForceActuatorWarning.mezzanineBootloaderActive[i] ||
                _eventForceActuatorWarning.uniqueIdCRCError[i] !=
                        _previousEventForceActuatorWarning.uniqueIdCRCError[i] ||
                _eventForceActuatorWarning.applicationTypeMismatch[i] !=
                        _previousEventForceActuatorWarning.applicationTypeMismatch[i] ||
                _eventForceActuatorWarning.applicationMissing[i] !=
                        _previousEventForceActuatorWarning.applicationMissing[i] ||
                _eventForceActuatorWarning.applicationCRCMismatch[i] !=
                        _previousEventForceActuatorWarning.applicationCRCMismatch[i] ||
                _eventForceActuatorWarning.oneWireMissing[i] !=
                        _previousEventForceActuatorWarning.oneWireMissing[i] ||
                _eventForceActuatorWarning.oneWire1Mismatch[i] !=
                        _previousEventForceActuatorWarning.oneWire1Mismatch[i] ||
                _eventForceActuatorWarning.oneWire2Mismatch[i] !=
                        _previousEventForceActuatorWarning.oneWire2Mismatch[i] ||
                _eventForceActuatorWarning.watchdogReset[i] !=
                        _previousEventForceActuatorWarning.watchdogReset[i] ||
                _eventForceActuatorWarning.brownOut[i] != _previousEventForceActuatorWarning.brownOut[i] ||
                _eventForceActuatorWarning.eventTrapReset[i] !=
                        _previousEventForceActuatorWarning.eventTrapReset[i] ||
                _eventForceActuatorWarning.ssrPowerFault[i] !=
                        _previousEventForceActuatorWarning.ssrPowerFault[i] ||
                _eventForceActuatorWarning.auxPowerFault[i] !=
                        _previousEventForceActuatorWarning.auxPowerFault[i] ||
                _eventForceActuatorWarning.mezzaninePowerFault[i] !=
                        _previousEventForceActuatorWarning.mezzaninePowerFault[i] ||
                _eventForceActuatorWarning.mezzanineCurrentAmp1Fault[i] !=
                        _previousEventForceActuatorWarning.mezzanineCurrentAmp1Fault[i] ||
                _eventForceActuatorWarning.mezzanineCurrentAmp2Fault[i] !=
                        _previousEventForceActuatorWarning.mezzanineCurrentAmp2Fault[i] ||
                _eventForceActuatorWarning.mezzanineUniqueIdCRCError[i] !=
                        _previousEventForceActuatorWarning.mezzanineUniqueIdCRCError[i] ||
                _eventForceActuatorWarning.mezzanineMainCalibrationError[i] !=
                        _previousEventForceActuatorWarning.mezzanineMainCalibrationError[i] ||
                _eventForceActuatorWarning.mezzanineBackupCalibrationError[i] !=
                        _previousEventForceActuatorWarning.mezzanineBackupCalibrationError[i] ||
                _eventForceActuatorWarning.mezzanineEventTrapReset[i] !=
                        _previousEventForceActuatorWarning.mezzanineEventTrapReset[i] ||
                _eventForceActuatorWarning.mezzanineApplicationMissing[i] !=
                        _previousEventForceActuatorWarning.mezzanineApplicationMissing[i] ||
                _eventForceActuatorWarning.mezzanineApplicationCRCMismatch[i] !=
                        _previousEventForceActuatorWarning.mezzanineApplicationCRCMismatch[i] ||
                _eventForceActuatorWarning.ilcFault[i] != _previousEventForceActuatorWarning.ilcFault[i] ||
                _eventForceActuatorWarning.broadcastCounterWarning[i] !=
                        _previousEventForceActuatorWarning.broadcastCounterWarning[i];
    }
    if (changeDetected) {
        this->logForceActuatorForceWarning();
    }
}

void M1M3SSPublisher::logForceSetpointWarning() {
    _eventForceSetpointWarning.anySafetyLimitWarning = false;
    _eventForceSetpointWarning.anyNearNeighborWarning = false;
    _eventForceSetpointWarning.anyFarNeighborWarning = false;
    _eventForceSetpointWarning.anyElevationForceWarning = false;
    _eventForceSetpointWarning.anyAzimuthForceWarning = false;
    _eventForceSetpointWarning.anyThermalForceWarning = false;
    _eventForceSetpointWarning.anyBalanceForceWarning = false;
    _eventForceSetpointWarning.anyAccelerationForceWarning = false;
    _eventForceSetpointWarning.anyVelocityForceWarning = false;
    _eventForceSetpointWarning.anyActiveOpticForceWarning = false;
    _eventForceSetpointWarning.anyStaticForceWarning = false;
    _eventForceSetpointWarning.anyAberrationForceWarning = false;
    _eventForceSetpointWarning.anyOffsetForceWarning = false;
    _eventForceSetpointWarning.anyForceWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        _eventForceSetpointWarning.anySafetyLimitWarning = _eventForceSetpointWarning.anySafetyLimitWarning ||
                                                           _eventForceSetpointWarning.safetyLimitWarning[i];
        _eventForceSetpointWarning.anyNearNeighborWarning =
                _eventForceSetpointWarning.anyNearNeighborWarning ||
                _eventForceSetpointWarning.nearNeighborWarning[i];
        _eventForceSetpointWarning.anyFarNeighborWarning = _eventForceSetpointWarning.anyFarNeighborWarning ||
                                                           _eventForceSetpointWarning.farNeighborWarning[i];
        _eventForceSetpointWarning.anyElevationForceWarning =
                _eventForceSetpointWarning.anyElevationForceWarning ||
                _eventForceSetpointWarning.elevationForceWarning[i];
        _eventForceSetpointWarning.anyAzimuthForceWarning =
                _eventForceSetpointWarning.anyAzimuthForceWarning ||
                _eventForceSetpointWarning.azimuthForceWarning[i];
        _eventForceSetpointWarning.anyThermalForceWarning =
                _eventForceSetpointWarning.anyThermalForceWarning ||
                _eventForceSetpointWarning.thermalForceWarning[i];
        _eventForceSetpointWarning.anyBalanceForceWarning =
                _eventForceSetpointWarning.anyBalanceForceWarning ||
                _eventForceSetpointWarning.balanceForceWarning[i];
        _eventForceSetpointWarning.anyAccelerationForceWarning =
                _eventForceSetpointWarning.anyAccelerationForceWarning ||
                _eventForceSetpointWarning.accelerationForceWarning[i];
        _eventForceSetpointWarning.anyVelocityForceWarning =
                _eventForceSetpointWarning.anyVelocityForceWarning ||
                _eventForceSetpointWarning.velocityForceWarning[i];
        _eventForceSetpointWarning.anyActiveOpticForceWarning =
                _eventForceSetpointWarning.anyActiveOpticForceWarning ||
                _eventForceSetpointWarning.activeOpticForceWarning[i];
        _eventForceSetpointWarning.anyStaticForceWarning = _eventForceSetpointWarning.anyStaticForceWarning ||
                                                           _eventForceSetpointWarning.staticForceWarning[i];
        _eventForceSetpointWarning.anyAberrationForceWarning =
                _eventForceSetpointWarning.anyAberrationForceWarning ||
                _eventForceSetpointWarning.aberrationForceWarning[i];
        _eventForceSetpointWarning.anyOffsetForceWarning = _eventForceSetpointWarning.anyOffsetForceWarning ||
                                                           _eventForceSetpointWarning.offsetForceWarning[i];
        _eventForceSetpointWarning.anyForceWarning =
                _eventForceSetpointWarning.anyForceWarning || _eventForceSetpointWarning.forceWarning[i];
    }
    _eventForceSetpointWarning.anyWarning =
            _eventForceSetpointWarning.anySafetyLimitWarning || _eventForceSetpointWarning.xMomentWarning ||
            _eventForceSetpointWarning.yMomentWarning || _eventForceSetpointWarning.zMomentWarning ||
            _eventForceSetpointWarning.anyNearNeighborWarning ||
            _eventForceSetpointWarning.magnitudeWarning || _eventForceSetpointWarning.anyFarNeighborWarning ||
            _eventForceSetpointWarning.anyElevationForceWarning ||
            _eventForceSetpointWarning.anyAzimuthForceWarning ||
            _eventForceSetpointWarning.anyThermalForceWarning ||
            _eventForceSetpointWarning.anyBalanceForceWarning ||
            _eventForceSetpointWarning.anyVelocityForceWarning ||
            _eventForceSetpointWarning.anyAccelerationForceWarning ||
            _eventForceSetpointWarning.activeOpticNetForceWarning ||
            _eventForceSetpointWarning.anyActiveOpticForceWarning ||
            _eventForceSetpointWarning.anyStaticForceWarning ||
            _eventForceSetpointWarning.aberrationNetForceWarning ||
            _eventForceSetpointWarning.anyAberrationForceWarning ||
            _eventForceSetpointWarning.anyOffsetForceWarning || _eventForceSetpointWarning.anyForceWarning;
    _m1m3SAL->logEvent_forceSetpointWarning(&_eventForceSetpointWarning, 0);
    _previousEventForceSetpointWarning = _eventForceSetpointWarning;
}

void M1M3SSPublisher::tryLogForceSetpointWarning() {
    bool changeDetected =
            _eventForceSetpointWarning.xMomentWarning != _previousEventForceSetpointWarning.xMomentWarning ||
            _eventForceSetpointWarning.yMomentWarning != _previousEventForceSetpointWarning.yMomentWarning ||
            _eventForceSetpointWarning.zMomentWarning != _previousEventForceSetpointWarning.zMomentWarning ||
            _eventForceSetpointWarning.magnitudeWarning !=
                    _previousEventForceSetpointWarning.magnitudeWarning ||
            _eventForceSetpointWarning.activeOpticNetForceWarning !=
                    _previousEventForceSetpointWarning.activeOpticNetForceWarning ||
            _eventForceSetpointWarning.aberrationNetForceWarning !=
                    _previousEventForceSetpointWarning.aberrationNetForceWarning;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventForceSetpointWarning.safetyLimitWarning[i] !=
                                 _previousEventForceSetpointWarning.safetyLimitWarning[i] ||
                         _eventForceSetpointWarning.nearNeighborWarning[i] !=
                                 _previousEventForceSetpointWarning.nearNeighborWarning[i] ||
                         _eventForceSetpointWarning.farNeighborWarning[i] !=
                                 _previousEventForceSetpointWarning.farNeighborWarning[i] ||
                         _eventForceSetpointWarning.elevationForceWarning[i] !=
                                 _previousEventForceSetpointWarning.elevationForceWarning[i] ||
                         _eventForceSetpointWarning.azimuthForceWarning[i] !=
                                 _previousEventForceSetpointWarning.azimuthForceWarning[i] ||
                         _eventForceSetpointWarning.thermalForceWarning[i] !=
                                 _previousEventForceSetpointWarning.thermalForceWarning[i] ||
                         _eventForceSetpointWarning.balanceForceWarning[i] !=
                                 _previousEventForceSetpointWarning.balanceForceWarning[i] ||
                         _eventForceSetpointWarning.accelerationForceWarning[i] !=
                                 _previousEventForceSetpointWarning.accelerationForceWarning[i] ||
                         _eventForceSetpointWarning.velocityForceWarning[i] !=
                                 _previousEventForceSetpointWarning.velocityForceWarning[i] ||
                         _eventForceSetpointWarning.activeOpticForceWarning[i] !=
                                 _previousEventForceSetpointWarning.activeOpticForceWarning[i] ||
                         _eventForceSetpointWarning.staticForceWarning[i] !=
                                 _previousEventForceSetpointWarning.staticForceWarning[i] ||
                         _eventForceSetpointWarning.aberrationForceWarning[i] !=
                                 _previousEventForceSetpointWarning.aberrationForceWarning[i] ||
                         _eventForceSetpointWarning.offsetForceWarning[i] !=
                                 _previousEventForceSetpointWarning.offsetForceWarning[i] ||
                         _eventForceSetpointWarning.forceWarning[i] !=
                                 _previousEventForceSetpointWarning.forceWarning[i];
    }
    if (changeDetected) {
        this->logForceSetpointWarning();
    }
}

void M1M3SSPublisher::logGyroWarning() {
    _eventGyroWarning.anyWarning =
            _eventGyroWarning.gyroXStatusWarning || _eventGyroWarning.gyroYStatusWarning ||
            _eventGyroWarning.gyroZStatusWarning || _eventGyroWarning.sequenceNumberWarning ||
            _eventGyroWarning.crcMismatchWarning || _eventGyroWarning.invalidLengthWarning ||
            _eventGyroWarning.invalidHeaderWarning || _eventGyroWarning.incompleteFrameWarning ||
            _eventGyroWarning.gyroXSLDWarning || _eventGyroWarning.gyroXMODDACWarning ||
            _eventGyroWarning.gyroXPhaseWarning || _eventGyroWarning.gyroXFlashWarning ||
            _eventGyroWarning.gyroYSLDWarning || _eventGyroWarning.gyroYMODDACWarning ||
            _eventGyroWarning.gyroYPhaseWarning || _eventGyroWarning.gyroYFlashWarning ||
            _eventGyroWarning.gyroZSLDWarning || _eventGyroWarning.gyroZMODDACWarning ||
            _eventGyroWarning.gyroZPhaseWarning || _eventGyroWarning.gyroZFlashWarning ||
            _eventGyroWarning.gyroXSLDTemperatureStatusWarning ||
            _eventGyroWarning.gyroYSLDTemperatureStatusWarning ||
            _eventGyroWarning.gyroZSLDTemperatureStatusWarning ||
            _eventGyroWarning.gcbTemperatureStatusWarning || _eventGyroWarning.temperatureStatusWarning ||
            _eventGyroWarning.gcbDSPSPIFlashStatusWarning || _eventGyroWarning.gcbFPGASPIFlashStatusWarning ||
            _eventGyroWarning.dspSPIFlashStatusWarning || _eventGyroWarning.fpgaSPIFlashStatusWarning ||
            _eventGyroWarning.gcb1_2VStatusWarning || _eventGyroWarning.gcb3_3VStatusWarning ||
            _eventGyroWarning.gcb5VStatusWarning || _eventGyroWarning.v1_2StatusWarning ||
            _eventGyroWarning.v3_3StatusWarning || _eventGyroWarning.v5StatusWarning ||
            _eventGyroWarning.gcbFPGAStatusWarning || _eventGyroWarning.fpgaStatusWarning ||
            _eventGyroWarning.hiSpeedSPORTStatusWarning || _eventGyroWarning.auxSPORTStatusWarning ||
            _eventGyroWarning.sufficientSoftwareResourcesWarning ||
            _eventGyroWarning.gyroEOVoltsPositiveWarning || _eventGyroWarning.gyroEOVoltsNegativeWarning ||
            _eventGyroWarning.gyroXVoltsWarning || _eventGyroWarning.gyroYVoltsWarning ||
            _eventGyroWarning.gyroZVoltsWarning || _eventGyroWarning.gcbADCCommsWarning ||
            _eventGyroWarning.mSYNCExternalTimingWarning;
    _m1m3SAL->logEvent_gyroWarning(&_eventGyroWarning, 0);
    _previousEventGyroWarning = _eventGyroWarning;
}

void M1M3SSPublisher::tryLogGyroWarning() {
    if (_eventGyroWarning.gyroXStatusWarning != _previousEventGyroWarning.gyroXStatusWarning ||
        _eventGyroWarning.gyroYStatusWarning != _previousEventGyroWarning.gyroYStatusWarning ||
        _eventGyroWarning.gyroZStatusWarning != _previousEventGyroWarning.gyroZStatusWarning ||
        _eventGyroWarning.sequenceNumberWarning != _previousEventGyroWarning.sequenceNumberWarning ||
        _eventGyroWarning.crcMismatchWarning != _previousEventGyroWarning.crcMismatchWarning ||
        _eventGyroWarning.invalidLengthWarning != _previousEventGyroWarning.invalidLengthWarning ||
        _eventGyroWarning.invalidHeaderWarning != _previousEventGyroWarning.invalidHeaderWarning ||
        _eventGyroWarning.incompleteFrameWarning != _previousEventGyroWarning.incompleteFrameWarning ||
        _eventGyroWarning.gyroXSLDWarning != _previousEventGyroWarning.gyroXSLDWarning ||
        _eventGyroWarning.gyroXMODDACWarning != _previousEventGyroWarning.gyroXMODDACWarning ||
        _eventGyroWarning.gyroXPhaseWarning != _previousEventGyroWarning.gyroXPhaseWarning ||
        _eventGyroWarning.gyroXFlashWarning != _previousEventGyroWarning.gyroXFlashWarning ||
        _eventGyroWarning.gyroYSLDWarning != _previousEventGyroWarning.gyroYSLDWarning ||
        _eventGyroWarning.gyroYMODDACWarning != _previousEventGyroWarning.gyroYMODDACWarning ||
        _eventGyroWarning.gyroYPhaseWarning != _previousEventGyroWarning.gyroYPhaseWarning ||
        _eventGyroWarning.gyroYFlashWarning != _previousEventGyroWarning.gyroYFlashWarning ||
        _eventGyroWarning.gyroZSLDWarning != _previousEventGyroWarning.gyroZSLDWarning ||
        _eventGyroWarning.gyroZMODDACWarning != _previousEventGyroWarning.gyroZMODDACWarning ||
        _eventGyroWarning.gyroZPhaseWarning != _previousEventGyroWarning.gyroZPhaseWarning ||
        _eventGyroWarning.gyroZFlashWarning != _previousEventGyroWarning.gyroZFlashWarning ||
        _eventGyroWarning.gyroXSLDTemperatureStatusWarning !=
                _previousEventGyroWarning.gyroXSLDTemperatureStatusWarning ||
        _eventGyroWarning.gyroYSLDTemperatureStatusWarning !=
                _previousEventGyroWarning.gyroYSLDTemperatureStatusWarning ||
        _eventGyroWarning.gyroZSLDTemperatureStatusWarning !=
                _previousEventGyroWarning.gyroZSLDTemperatureStatusWarning ||
        _eventGyroWarning.gcbTemperatureStatusWarning !=
                _previousEventGyroWarning.gcbTemperatureStatusWarning ||
        _eventGyroWarning.temperatureStatusWarning != _previousEventGyroWarning.temperatureStatusWarning ||
        _eventGyroWarning.gcbDSPSPIFlashStatusWarning !=
                _previousEventGyroWarning.gcbDSPSPIFlashStatusWarning ||
        _eventGyroWarning.gcbFPGASPIFlashStatusWarning !=
                _previousEventGyroWarning.gcbFPGASPIFlashStatusWarning ||
        _eventGyroWarning.dspSPIFlashStatusWarning != _previousEventGyroWarning.dspSPIFlashStatusWarning ||
        _eventGyroWarning.fpgaSPIFlashStatusWarning != _previousEventGyroWarning.fpgaSPIFlashStatusWarning ||
        _eventGyroWarning.gcb1_2VStatusWarning != _previousEventGyroWarning.gcb1_2VStatusWarning ||
        _eventGyroWarning.gcb3_3VStatusWarning != _previousEventGyroWarning.gcb3_3VStatusWarning ||
        _eventGyroWarning.gcb5VStatusWarning != _previousEventGyroWarning.gcb5VStatusWarning ||
        _eventGyroWarning.v1_2StatusWarning != _previousEventGyroWarning.v1_2StatusWarning ||
        _eventGyroWarning.v3_3StatusWarning != _previousEventGyroWarning.v3_3StatusWarning ||
        _eventGyroWarning.v5StatusWarning != _previousEventGyroWarning.v5StatusWarning ||
        _eventGyroWarning.gcbFPGAStatusWarning != _previousEventGyroWarning.gcbFPGAStatusWarning ||
        _eventGyroWarning.fpgaStatusWarning != _previousEventGyroWarning.fpgaStatusWarning ||
        _eventGyroWarning.hiSpeedSPORTStatusWarning != _previousEventGyroWarning.hiSpeedSPORTStatusWarning ||
        _eventGyroWarning.auxSPORTStatusWarning != _previousEventGyroWarning.auxSPORTStatusWarning ||
        _eventGyroWarning.sufficientSoftwareResourcesWarning !=
                _previousEventGyroWarning.sufficientSoftwareResourcesWarning ||
        _eventGyroWarning.gyroEOVoltsPositiveWarning !=
                _previousEventGyroWarning.gyroEOVoltsPositiveWarning ||
        _eventGyroWarning.gyroEOVoltsNegativeWarning !=
                _previousEventGyroWarning.gyroEOVoltsNegativeWarning ||
        _eventGyroWarning.gyroXVoltsWarning != _previousEventGyroWarning.gyroXVoltsWarning ||
        _eventGyroWarning.gyroYVoltsWarning != _previousEventGyroWarning.gyroYVoltsWarning ||
        _eventGyroWarning.gyroZVoltsWarning != _previousEventGyroWarning.gyroZVoltsWarning ||
        _eventGyroWarning.gcbADCCommsWarning != _previousEventGyroWarning.gcbADCCommsWarning ||
        _eventGyroWarning.mSYNCExternalTimingWarning !=
                _previousEventGyroWarning.mSYNCExternalTimingWarning) {
        this->logGyroWarning();
    }
}

void M1M3SSPublisher::logHardpointActuatorInfo() {
    _m1m3SAL->logEvent_hardpointActuatorInfo(&_eventHardpointActuatorInfo, 0);
    _previousEventHardpointActuatorInfo = _eventHardpointActuatorInfo;
}

void M1M3SSPublisher::tryLogHardpointActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorInfo.referenceId[i] !=
                                 _previousEventHardpointActuatorInfo.referenceId[i] ||
                         _eventHardpointActuatorInfo.referencePosition[i] !=
                                 _previousEventHardpointActuatorInfo.referencePosition[i] ||
                         _eventHardpointActuatorInfo.modbusSubnet[i] !=
                                 _previousEventHardpointActuatorInfo.modbusSubnet[i] ||
                         _eventHardpointActuatorInfo.modbusAddress[i] !=
                                 _previousEventHardpointActuatorInfo.modbusAddress[i] ||
                         _eventHardpointActuatorInfo.xPosition[i] !=
                                 _previousEventHardpointActuatorInfo.xPosition[i] ||
                         _eventHardpointActuatorInfo.yPosition[i] !=
                                 _previousEventHardpointActuatorInfo.yPosition[i] ||
                         _eventHardpointActuatorInfo.zPosition[i] !=
                                 _previousEventHardpointActuatorInfo.zPosition[i] ||
                         _eventHardpointActuatorInfo.ilcUniqueId[i] !=
                                 _previousEventHardpointActuatorInfo.ilcUniqueId[i] ||
                         _eventHardpointActuatorInfo.ilcApplicationType[i] !=
                                 _previousEventHardpointActuatorInfo.ilcApplicationType[i] ||
                         _eventHardpointActuatorInfo.networkNodeType[i] !=
                                 _previousEventHardpointActuatorInfo.networkNodeType[i] ||
                         _eventHardpointActuatorInfo.ilcSelectedOptions[i] !=
                                 _previousEventHardpointActuatorInfo.ilcSelectedOptions[i] ||
                         _eventHardpointActuatorInfo.networkNodeOptions[i] !=
                                 _previousEventHardpointActuatorInfo.networkNodeOptions[i] ||
                         _eventHardpointActuatorInfo.majorRevision[i] !=
                                 _previousEventHardpointActuatorInfo.majorRevision[i] ||
                         _eventHardpointActuatorInfo.minorRevision[i] !=
                                 _previousEventHardpointActuatorInfo.minorRevision[i] ||
                         _eventHardpointActuatorInfo.adcScanRate[i] !=
                                 _previousEventHardpointActuatorInfo.adcScanRate[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellCoefficient[i] !=
                                 _previousEventHardpointActuatorInfo.mainLoadCellCoefficient[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellOffset[i] !=
                                 _previousEventHardpointActuatorInfo.mainLoadCellOffset[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellSensitivity[i] !=
                                 _previousEventHardpointActuatorInfo.mainLoadCellSensitivity[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellCoefficient[i] !=
                                 _previousEventHardpointActuatorInfo.backupLoadCellCoefficient[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellOffset[i] !=
                                 _previousEventHardpointActuatorInfo.backupLoadCellOffset[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellSensitivity[i] !=
                                 _previousEventHardpointActuatorInfo.backupLoadCellSensitivity[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorInfo();
    }
}

void M1M3SSPublisher::logHardpointActuatorState() {
    _m1m3SAL->logEvent_hardpointActuatorState(&_eventHardpointActuatorState, 0);
    _previousEventHardpointActuatorState = _eventHardpointActuatorState;
}

void M1M3SSPublisher::tryLogHardpointActuatorState() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorState.ilcState[i] !=
                                 _previousEventHardpointActuatorState.ilcState[i] ||
                         _eventHardpointActuatorState.motionState[i] !=
                                 _previousEventHardpointActuatorState.motionState[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorState();
    }
}

void M1M3SSPublisher::logHardpointActuatorWarning() {
    _eventHardpointActuatorWarning.anyMajorFault = false;
    _eventHardpointActuatorWarning.anyMinorFault = false;
    _eventHardpointActuatorWarning.anyFaultOverride = false;
    _eventHardpointActuatorWarning.anyMainCalibrationError = false;
    _eventHardpointActuatorWarning.anyBackupCalibrationError = false;
    _eventHardpointActuatorWarning.anyLimitSwitch1Operated = false;
    _eventHardpointActuatorWarning.anyLimitSwitch2Operated = false;
    _eventHardpointActuatorWarning.anyUniqueIdCRCError = false;
    _eventHardpointActuatorWarning.anyApplicationTypeMismatch = false;
    _eventHardpointActuatorWarning.anyApplicationMissing = false;
    _eventHardpointActuatorWarning.anyApplicationCRCMismatch = false;
    _eventHardpointActuatorWarning.anyOneWireMissing = false;
    _eventHardpointActuatorWarning.anyOneWire1Mismatch = false;
    _eventHardpointActuatorWarning.anyOneWire2Mismatch = false;
    _eventHardpointActuatorWarning.anyWatchdogReset = false;
    _eventHardpointActuatorWarning.anyBrownOut = false;
    _eventHardpointActuatorWarning.anyEventTrapReset = false;
    _eventHardpointActuatorWarning.anyMotorDriverFault = false;
    _eventHardpointActuatorWarning.anySSRPowerFault = false;
    _eventHardpointActuatorWarning.anyAuxPowerFault = false;
    _eventHardpointActuatorWarning.anySMCPowerFault = false;
    _eventHardpointActuatorWarning.anyILCFault = false;
    _eventHardpointActuatorWarning.anyBroadcastCounterWarning = false;
    for (int i = 0; i < HP_COUNT; ++i) {
        _eventHardpointActuatorWarning.anyMajorFault =
                _eventHardpointActuatorWarning.anyMajorFault || _eventHardpointActuatorWarning.majorFault[i];
        _eventHardpointActuatorWarning.anyMinorFault =
                _eventHardpointActuatorWarning.anyMinorFault || _eventHardpointActuatorWarning.minorFault[i];
        _eventHardpointActuatorWarning.anyFaultOverride = _eventHardpointActuatorWarning.anyFaultOverride ||
                                                          _eventHardpointActuatorWarning.faultOverride[i];
        _eventHardpointActuatorWarning.anyMainCalibrationError =
                _eventHardpointActuatorWarning.anyMainCalibrationError ||
                _eventHardpointActuatorWarning.mainCalibrationError[i];
        _eventHardpointActuatorWarning.anyBackupCalibrationError =
                _eventHardpointActuatorWarning.anyBackupCalibrationError ||
                _eventHardpointActuatorWarning.backupCalibrationError[i];
        _eventHardpointActuatorWarning.anyLimitSwitch1Operated =
                _eventHardpointActuatorWarning.anyLimitSwitch1Operated ||
                _eventHardpointActuatorWarning.limitSwitch1Operated[i];
        _eventHardpointActuatorWarning.anyLimitSwitch2Operated =
                _eventHardpointActuatorWarning.anyLimitSwitch2Operated ||
                _eventHardpointActuatorWarning.limitSwitch2Operated[i];
        _eventHardpointActuatorWarning.anyUniqueIdCRCError =
                _eventHardpointActuatorWarning.anyUniqueIdCRCError ||
                _eventHardpointActuatorWarning.uniqueIdCRCError[i];
        _eventHardpointActuatorWarning.anyApplicationTypeMismatch =
                _eventHardpointActuatorWarning.anyApplicationTypeMismatch ||
                _eventHardpointActuatorWarning.applicationTypeMismatch[i];
        _eventHardpointActuatorWarning.anyApplicationMissing =
                _eventHardpointActuatorWarning.anyApplicationMissing ||
                _eventHardpointActuatorWarning.applicationMissing[i];
        _eventHardpointActuatorWarning.anyApplicationCRCMismatch =
                _eventHardpointActuatorWarning.anyApplicationCRCMismatch ||
                _eventHardpointActuatorWarning.applicationCRCMismatch[i];
        _eventHardpointActuatorWarning.anyOneWireMissing = _eventHardpointActuatorWarning.anyOneWireMissing ||
                                                           _eventHardpointActuatorWarning.oneWireMissing[i];
        _eventHardpointActuatorWarning.anyOneWire1Mismatch =
                _eventHardpointActuatorWarning.anyOneWire1Mismatch ||
                _eventHardpointActuatorWarning.oneWire1Mismatch[i];
        _eventHardpointActuatorWarning.anyOneWire2Mismatch =
                _eventHardpointActuatorWarning.anyOneWire2Mismatch ||
                _eventHardpointActuatorWarning.oneWire2Mismatch[i];
        _eventHardpointActuatorWarning.anyWatchdogReset = _eventHardpointActuatorWarning.anyWatchdogReset ||
                                                          _eventHardpointActuatorWarning.watchdogReset[i];
        _eventHardpointActuatorWarning.anyBrownOut =
                _eventHardpointActuatorWarning.anyBrownOut || _eventHardpointActuatorWarning.brownOut[i];
        _eventHardpointActuatorWarning.anyEventTrapReset = _eventHardpointActuatorWarning.anyEventTrapReset ||
                                                           _eventHardpointActuatorWarning.eventTrapReset[i];
        _eventHardpointActuatorWarning.anyMotorDriverFault =
                _eventHardpointActuatorWarning.anyMotorDriverFault ||
                _eventHardpointActuatorWarning.motorDriverFault[i];
        _eventHardpointActuatorWarning.anySSRPowerFault = _eventHardpointActuatorWarning.anySSRPowerFault ||
                                                          _eventHardpointActuatorWarning.ssrPowerFault[i];
        _eventHardpointActuatorWarning.anyAuxPowerFault = _eventHardpointActuatorWarning.anyAuxPowerFault ||
                                                          _eventHardpointActuatorWarning.auxPowerFault[i];
        _eventHardpointActuatorWarning.anySMCPowerFault = _eventHardpointActuatorWarning.anySMCPowerFault ||
                                                          _eventHardpointActuatorWarning.smcPowerFault[i];
        _eventHardpointActuatorWarning.anyILCFault =
                _eventHardpointActuatorWarning.anyILCFault || _eventHardpointActuatorWarning.ilcFault[i];
        _eventHardpointActuatorWarning.anyBroadcastCounterWarning =
                _eventHardpointActuatorWarning.anyBroadcastCounterWarning ||
                _eventHardpointActuatorWarning.broadcastCounterWarning[i];
    }
    _eventHardpointActuatorWarning.anyWarning =
            _eventHardpointActuatorWarning.anyMajorFault || _eventHardpointActuatorWarning.anyMinorFault ||
            _eventHardpointActuatorWarning.anyFaultOverride ||
            _eventHardpointActuatorWarning.anyMainCalibrationError ||
            _eventHardpointActuatorWarning.anyBackupCalibrationError ||
            _eventHardpointActuatorWarning.anyLimitSwitch1Operated ||
            _eventHardpointActuatorWarning.anyLimitSwitch2Operated ||
            _eventHardpointActuatorWarning.anyUniqueIdCRCError ||
            _eventHardpointActuatorWarning.anyApplicationTypeMismatch ||
            _eventHardpointActuatorWarning.anyApplicationMissing ||
            _eventHardpointActuatorWarning.anyApplicationCRCMismatch ||
            _eventHardpointActuatorWarning.anyOneWireMissing ||
            _eventHardpointActuatorWarning.anyOneWire1Mismatch ||
            _eventHardpointActuatorWarning.anyOneWire2Mismatch ||
            _eventHardpointActuatorWarning.anyWatchdogReset || _eventHardpointActuatorWarning.anyBrownOut ||
            _eventHardpointActuatorWarning.anyEventTrapReset ||
            _eventHardpointActuatorWarning.anyMotorDriverFault ||
            _eventHardpointActuatorWarning.anySSRPowerFault ||
            _eventHardpointActuatorWarning.anyAuxPowerFault ||
            _eventHardpointActuatorWarning.anySMCPowerFault || _eventHardpointActuatorWarning.anyILCFault ||
            _eventHardpointActuatorWarning.anyBroadcastCounterWarning;
    _m1m3SAL->logEvent_hardpointActuatorWarning(&_eventHardpointActuatorWarning, 0);
    _previousEventHardpointActuatorWarning = _eventHardpointActuatorWarning;
}

void M1M3SSPublisher::tryLogHardpointActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorWarning.majorFault[i] !=
                                 _previousEventHardpointActuatorWarning.majorFault[i] ||
                         _eventHardpointActuatorWarning.minorFault[i] !=
                                 _previousEventHardpointActuatorWarning.minorFault[i] ||
                         _eventHardpointActuatorWarning.faultOverride[i] !=
                                 _previousEventHardpointActuatorWarning.faultOverride[i] ||
                         _eventHardpointActuatorWarning.mainCalibrationError[i] !=
                                 _previousEventHardpointActuatorWarning.mainCalibrationError[i] ||
                         _eventHardpointActuatorWarning.backupCalibrationError[i] !=
                                 _previousEventHardpointActuatorWarning.backupCalibrationError[i] ||
                         _eventHardpointActuatorWarning.limitSwitch1Operated[i] !=
                                 _previousEventHardpointActuatorWarning.limitSwitch1Operated[i] ||
                         _eventHardpointActuatorWarning.limitSwitch2Operated[i] !=
                                 _previousEventHardpointActuatorWarning.limitSwitch2Operated[i] ||
                         _eventHardpointActuatorWarning.uniqueIdCRCError[i] !=
                                 _previousEventHardpointActuatorWarning.uniqueIdCRCError[i] ||
                         _eventHardpointActuatorWarning.applicationTypeMismatch[i] !=
                                 _previousEventHardpointActuatorWarning.applicationTypeMismatch[i] ||
                         _eventHardpointActuatorWarning.applicationMissing[i] !=
                                 _previousEventHardpointActuatorWarning.applicationMissing[i] ||
                         _eventHardpointActuatorWarning.applicationCRCMismatch[i] !=
                                 _previousEventHardpointActuatorWarning.applicationCRCMismatch[i] ||
                         _eventHardpointActuatorWarning.oneWireMissing[i] !=
                                 _previousEventHardpointActuatorWarning.oneWireMissing[i] ||
                         _eventHardpointActuatorWarning.oneWire1Mismatch[i] !=
                                 _previousEventHardpointActuatorWarning.oneWire1Mismatch[i] ||
                         _eventHardpointActuatorWarning.oneWire2Mismatch[i] !=
                                 _previousEventHardpointActuatorWarning.oneWire2Mismatch[i] ||
                         _eventHardpointActuatorWarning.watchdogReset[i] !=
                                 _previousEventHardpointActuatorWarning.watchdogReset[i] ||
                         _eventHardpointActuatorWarning.brownOut[i] !=
                                 _previousEventHardpointActuatorWarning.brownOut[i] ||
                         _eventHardpointActuatorWarning.eventTrapReset[i] !=
                                 _previousEventHardpointActuatorWarning.eventTrapReset[i] ||
                         _eventHardpointActuatorWarning.motorDriverFault[i] !=
                                 _previousEventHardpointActuatorWarning.motorDriverFault[i] ||
                         _eventHardpointActuatorWarning.ssrPowerFault[i] !=
                                 _previousEventHardpointActuatorWarning.ssrPowerFault[i] ||
                         _eventHardpointActuatorWarning.auxPowerFault[i] !=
                                 _previousEventHardpointActuatorWarning.auxPowerFault[i] ||
                         _eventHardpointActuatorWarning.smcPowerFault[i] !=
                                 _previousEventHardpointActuatorWarning.smcPowerFault[i] ||
                         _eventHardpointActuatorWarning.ilcFault[i] !=
                                 _previousEventHardpointActuatorWarning.ilcFault[i] ||
                         _eventHardpointActuatorWarning.broadcastCounterWarning[i] !=
                                 _previousEventHardpointActuatorWarning.broadcastCounterWarning[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorWarning();
    }
}

void M1M3SSPublisher::logHardpointMonitorInfo() {
    _m1m3SAL->logEvent_hardpointMonitorInfo(&_eventHardpointMonitorInfo, 0);
    _previousEventHardpointMonitorInfo = _eventHardpointMonitorInfo;
}

void M1M3SSPublisher::tryLogHardpointMonitorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointMonitorInfo.referenceId[i] !=
                                 _previousEventHardpointMonitorInfo.referenceId[i] ||
                         _eventHardpointMonitorInfo.modbusSubnet[i] !=
                                 _previousEventHardpointMonitorInfo.modbusSubnet[i] ||
                         _eventHardpointMonitorInfo.modbusAddress[i] !=
                                 _previousEventHardpointMonitorInfo.modbusAddress[i] ||
                         _eventHardpointMonitorInfo.ilcUniqueId[i] !=
                                 _previousEventHardpointMonitorInfo.ilcUniqueId[i] ||
                         _eventHardpointMonitorInfo.ilcApplicationType[i] !=
                                 _previousEventHardpointMonitorInfo.ilcApplicationType[i] ||
                         _eventHardpointMonitorInfo.networkNodeType[i] !=
                                 _previousEventHardpointMonitorInfo.networkNodeType[i] ||
                         _eventHardpointMonitorInfo.majorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.majorRevision[i] ||
                         _eventHardpointMonitorInfo.minorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.minorRevision[i] ||
                         _eventHardpointMonitorInfo.mezzanineUniqueId[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineUniqueId[i] ||
                         _eventHardpointMonitorInfo.mezzanineFirmwareType[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineFirmwareType[i] ||
                         _eventHardpointMonitorInfo.mezzanineMajorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineMajorRevision[i] ||
                         _eventHardpointMonitorInfo.mezzanineMinorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorInfo();
    }
}

void M1M3SSPublisher::logHardpointMonitorState() {
    _m1m3SAL->logEvent_hardpointMonitorState(&_eventHardpointMonitorState, 0);
    _previousEventHardpointMonitorState = _eventHardpointMonitorState;
}

void M1M3SSPublisher::tryLogHardpointMonitorState() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventHardpointMonitorState.ilcState[i] !=
                                                   _previousEventHardpointMonitorState.ilcState[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorState();
    }
}

void M1M3SSPublisher::logHardpointMonitorWarning() {
    _eventHardpointMonitorWarning.anyMajorFault = false;
    _eventHardpointMonitorWarning.anyMinorFault = false;
    _eventHardpointMonitorWarning.anyFaultOverride = false;
    _eventHardpointMonitorWarning.anyInstrumentError = false;
    _eventHardpointMonitorWarning.anyMezzanineError = false;
    _eventHardpointMonitorWarning.anyMezzanineBootloaderActive = false;
    _eventHardpointMonitorWarning.anyUniqueIdCRCError = false;
    _eventHardpointMonitorWarning.anyApplicationTypeMismatch = false;
    _eventHardpointMonitorWarning.anyApplicationMissing = false;
    _eventHardpointMonitorWarning.anyApplicationCRCMismatch = false;
    _eventHardpointMonitorWarning.anyOneWireMissing = false;
    _eventHardpointMonitorWarning.anyOneWire1Mismatch = false;
    _eventHardpointMonitorWarning.anyOneWire2Mismatch = false;
    _eventHardpointMonitorWarning.anyWatchdogReset = false;
    _eventHardpointMonitorWarning.anyBrownOut = false;
    _eventHardpointMonitorWarning.anyEventTrapReset = false;
    _eventHardpointMonitorWarning.anySSRPowerFault = false;
    _eventHardpointMonitorWarning.anyAuxPowerFault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError = false;
    _eventHardpointMonitorWarning.anyMezzanineEventTrapReset = false;
    _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault = false;
    _eventHardpointMonitorWarning.anyMezzanineApplicationMissing = false;
    _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch = false;
    for (int i = 0; i < HM_COUNT; ++i) {
        _eventHardpointMonitorWarning.anyMajorFault =
                _eventHardpointMonitorWarning.anyMajorFault || _eventHardpointMonitorWarning.majorFault[i];
        _eventHardpointMonitorWarning.anyMinorFault =
                _eventHardpointMonitorWarning.anyMinorFault || _eventHardpointMonitorWarning.minorFault[i];
        _eventHardpointMonitorWarning.anyFaultOverride = _eventHardpointMonitorWarning.anyFaultOverride ||
                                                         _eventHardpointMonitorWarning.faultOverride[i];
        _eventHardpointMonitorWarning.anyInstrumentError = _eventHardpointMonitorWarning.anyInstrumentError ||
                                                           _eventHardpointMonitorWarning.instrumentError[i];
        _eventHardpointMonitorWarning.anyMezzanineError = _eventHardpointMonitorWarning.anyMezzanineError ||
                                                          _eventHardpointMonitorWarning.mezzanineError[i];
        _eventHardpointMonitorWarning.anyMezzanineBootloaderActive =
                _eventHardpointMonitorWarning.anyMezzanineBootloaderActive ||
                _eventHardpointMonitorWarning.mezzanineBootloaderActive[i];
        _eventHardpointMonitorWarning.anyUniqueIdCRCError =
                _eventHardpointMonitorWarning.anyUniqueIdCRCError ||
                _eventHardpointMonitorWarning.uniqueIdCRCError[i];
        _eventHardpointMonitorWarning.anyApplicationTypeMismatch =
                _eventHardpointMonitorWarning.anyApplicationTypeMismatch ||
                _eventHardpointMonitorWarning.applicationTypeMismatch[i];
        _eventHardpointMonitorWarning.anyApplicationMissing =
                _eventHardpointMonitorWarning.anyApplicationMissing ||
                _eventHardpointMonitorWarning.applicationMissing[i];
        _eventHardpointMonitorWarning.anyApplicationCRCMismatch =
                _eventHardpointMonitorWarning.anyApplicationCRCMismatch ||
                _eventHardpointMonitorWarning.applicationCRCMismatch[i];
        _eventHardpointMonitorWarning.anyOneWireMissing = _eventHardpointMonitorWarning.anyOneWireMissing ||
                                                          _eventHardpointMonitorWarning.oneWireMissing[i];
        _eventHardpointMonitorWarning.anyOneWire1Mismatch =
                _eventHardpointMonitorWarning.anyOneWire1Mismatch ||
                _eventHardpointMonitorWarning.oneWire1Mismatch[i];
        _eventHardpointMonitorWarning.anyOneWire2Mismatch =
                _eventHardpointMonitorWarning.anyOneWire2Mismatch ||
                _eventHardpointMonitorWarning.oneWire2Mismatch[i];
        _eventHardpointMonitorWarning.anyWatchdogReset = _eventHardpointMonitorWarning.anyWatchdogReset ||
                                                         _eventHardpointMonitorWarning.watchdogReset[i];
        _eventHardpointMonitorWarning.anyBrownOut =
                _eventHardpointMonitorWarning.anyBrownOut || _eventHardpointMonitorWarning.brownOut[i];
        _eventHardpointMonitorWarning.anyEventTrapReset = _eventHardpointMonitorWarning.anyEventTrapReset ||
                                                          _eventHardpointMonitorWarning.eventTrapReset[i];
        _eventHardpointMonitorWarning.anySSRPowerFault = _eventHardpointMonitorWarning.anySSRPowerFault ||
                                                         _eventHardpointMonitorWarning.ssrPowerFault[i];
        _eventHardpointMonitorWarning.anyAuxPowerFault = _eventHardpointMonitorWarning.anyAuxPowerFault ||
                                                         _eventHardpointMonitorWarning.auxPowerFault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault ||
                _eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault ||
                _eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault ||
                _eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault ||
                _eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError =
                _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError ||
                _eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i];
        _eventHardpointMonitorWarning.anyMezzanineEventTrapReset =
                _eventHardpointMonitorWarning.anyMezzanineEventTrapReset ||
                _eventHardpointMonitorWarning.mezzanineEventTrapReset[i];
        _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault =
                _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault ||
                _eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i];
        _eventHardpointMonitorWarning.anyMezzanineApplicationMissing =
                _eventHardpointMonitorWarning.anyMezzanineApplicationMissing ||
                _eventHardpointMonitorWarning.mezzanineApplicationMissing[i];
        _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch =
                _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch ||
                _eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    _eventHardpointMonitorWarning.anyWarning =
            _eventHardpointMonitorWarning.anyMajorFault || _eventHardpointMonitorWarning.anyMinorFault ||
            _eventHardpointMonitorWarning.anyFaultOverride ||
            _eventHardpointMonitorWarning.anyInstrumentError ||
            _eventHardpointMonitorWarning.anyMezzanineError ||
            _eventHardpointMonitorWarning.anyMezzanineBootloaderActive ||
            _eventHardpointMonitorWarning.anyUniqueIdCRCError ||
            _eventHardpointMonitorWarning.anyApplicationTypeMismatch ||
            _eventHardpointMonitorWarning.anyApplicationMissing ||
            _eventHardpointMonitorWarning.anyApplicationCRCMismatch ||
            _eventHardpointMonitorWarning.anyOneWireMissing ||
            _eventHardpointMonitorWarning.anyOneWire1Mismatch ||
            _eventHardpointMonitorWarning.anyOneWire2Mismatch ||
            _eventHardpointMonitorWarning.anyWatchdogReset || _eventHardpointMonitorWarning.anyBrownOut ||
            _eventHardpointMonitorWarning.anyEventTrapReset ||
            _eventHardpointMonitorWarning.anySSRPowerFault ||
            _eventHardpointMonitorWarning.anyAuxPowerFault ||
            _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault ||
            _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault ||
            _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault ||
            _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault ||
            _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError ||
            _eventHardpointMonitorWarning.anyMezzanineEventTrapReset ||
            _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault ||
            _eventHardpointMonitorWarning.anyMezzanineApplicationMissing ||
            _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch;
    _m1m3SAL->logEvent_hardpointMonitorWarning(&_eventHardpointMonitorWarning, 0);
    _previousEventHardpointMonitorWarning = _eventHardpointMonitorWarning;
}

void M1M3SSPublisher::tryLogHardpointMonitorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointMonitorWarning.majorFault[i] !=
                                 _previousEventHardpointMonitorWarning.majorFault[i] ||
                         _eventHardpointMonitorWarning.minorFault[i] !=
                                 _previousEventHardpointMonitorWarning.minorFault[i] ||
                         _eventHardpointMonitorWarning.faultOverride[i] !=
                                 _previousEventHardpointMonitorWarning.faultOverride[i] ||
                         _eventHardpointMonitorWarning.instrumentError[i] !=
                                 _previousEventHardpointMonitorWarning.instrumentError[i] ||
                         _eventHardpointMonitorWarning.mezzanineError[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineError[i] ||
                         _eventHardpointMonitorWarning.mezzanineBootloaderActive[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineBootloaderActive[i] ||
                         _eventHardpointMonitorWarning.uniqueIdCRCError[i] !=
                                 _previousEventHardpointMonitorWarning.uniqueIdCRCError[i] ||
                         _eventHardpointMonitorWarning.applicationTypeMismatch[i] !=
                                 _previousEventHardpointMonitorWarning.applicationTypeMismatch[i] ||
                         _eventHardpointMonitorWarning.applicationMissing[i] !=
                                 _previousEventHardpointMonitorWarning.applicationMissing[i] ||
                         _eventHardpointMonitorWarning.applicationCRCMismatch[i] !=
                                 _previousEventHardpointMonitorWarning.applicationCRCMismatch[i] ||
                         _eventHardpointMonitorWarning.oneWireMissing[i] !=
                                 _previousEventHardpointMonitorWarning.oneWireMissing[i] ||
                         _eventHardpointMonitorWarning.oneWire1Mismatch[i] !=
                                 _previousEventHardpointMonitorWarning.oneWire1Mismatch[i] ||
                         _eventHardpointMonitorWarning.oneWire2Mismatch[i] !=
                                 _previousEventHardpointMonitorWarning.oneWire2Mismatch[i] ||
                         _eventHardpointMonitorWarning.watchdogReset[i] !=
                                 _previousEventHardpointMonitorWarning.watchdogReset[i] ||
                         _eventHardpointMonitorWarning.brownOut[i] !=
                                 _previousEventHardpointMonitorWarning.brownOut[i] ||
                         _eventHardpointMonitorWarning.eventTrapReset[i] !=
                                 _previousEventHardpointMonitorWarning.eventTrapReset[i] ||
                         _eventHardpointMonitorWarning.ssrPowerFault[i] !=
                                 _previousEventHardpointMonitorWarning.ssrPowerFault[i] ||
                         _eventHardpointMonitorWarning.auxPowerFault[i] !=
                                 _previousEventHardpointMonitorWarning.auxPowerFault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] ||
                         _eventHardpointMonitorWarning.mezzanineEventTrapReset[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineEventTrapReset[i] ||
                         _eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] ||
                         _eventHardpointMonitorWarning.mezzanineApplicationMissing[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineApplicationMissing[i] ||
                         _eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorWarning();
    }
}

void M1M3SSPublisher::logILCWarning() {
    _eventILCWarning.anyWarning = _eventILCWarning.responseTimeout || _eventILCWarning.invalidCRC ||
                                  _eventILCWarning.illegalFunction || _eventILCWarning.illegalDataValue ||
                                  _eventILCWarning.invalidLength || _eventILCWarning.unknownSubnet ||
                                  _eventILCWarning.unknownAddress || _eventILCWarning.unknownFunction ||
                                  _eventILCWarning.unknownProblem;
    _m1m3SAL->logEvent_ilcWarning(&_eventILCWarning, 0);
    _previousEventILCWarning = _eventILCWarning;
}

void M1M3SSPublisher::tryLogILCWarning() {
    if (_eventILCWarning.actuatorId != _previousEventILCWarning.actuatorId ||
        _eventILCWarning.responseTimeout != _previousEventILCWarning.responseTimeout ||
        _eventILCWarning.invalidCRC != _previousEventILCWarning.invalidCRC ||
        _eventILCWarning.illegalFunction != _previousEventILCWarning.illegalFunction ||
        _eventILCWarning.illegalDataValue != _previousEventILCWarning.illegalDataValue ||
        _eventILCWarning.invalidLength != _previousEventILCWarning.invalidLength ||
        _eventILCWarning.unknownSubnet != _previousEventILCWarning.unknownSubnet ||
        _eventILCWarning.unknownAddress != _previousEventILCWarning.unknownAddress ||
        _eventILCWarning.unknownFunction != _previousEventILCWarning.unknownFunction ||
        _eventILCWarning.unknownProblem != _previousEventILCWarning.unknownProblem) {
        this->logILCWarning();
    }
}

void M1M3SSPublisher::logInclinometerSensorWarning() {
    _eventInclinometerSensorWarning.anyWarning =
            _eventInclinometerSensorWarning.sensorReportsIllegalFunction ||
            _eventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
            _eventInclinometerSensorWarning.responseTimeout || _eventInclinometerSensorWarning.invalidCRC ||
            _eventInclinometerSensorWarning.invalidLength || _eventInclinometerSensorWarning.unknownAddress ||
            _eventInclinometerSensorWarning.unknownFunction || _eventInclinometerSensorWarning.unknownProblem;
    _m1m3SAL->logEvent_inclinometerSensorWarning(&_eventInclinometerSensorWarning, 0);
    _previousEventInclinometerSensorWarning = _eventInclinometerSensorWarning;
}

void M1M3SSPublisher::tryLogInclinometerSensorWarning() {
    if (_eventInclinometerSensorWarning.sensorReportsIllegalFunction !=
                _previousEventInclinometerSensorWarning.sensorReportsIllegalFunction ||
        _eventInclinometerSensorWarning.sensorReportsIllegalDataAddress !=
                _previousEventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
        _eventInclinometerSensorWarning.responseTimeout !=
                _previousEventInclinometerSensorWarning.responseTimeout ||
        _eventInclinometerSensorWarning.invalidCRC != _previousEventInclinometerSensorWarning.invalidCRC ||
        _eventInclinometerSensorWarning.invalidLength !=
                _previousEventInclinometerSensorWarning.invalidLength ||
        _eventInclinometerSensorWarning.unknownAddress !=
                _previousEventInclinometerSensorWarning.unknownAddress ||
        _eventInclinometerSensorWarning.unknownFunction !=
                _previousEventInclinometerSensorWarning.unknownFunction ||
        _eventInclinometerSensorWarning.unknownProblem !=
                _previousEventInclinometerSensorWarning.unknownProblem) {
        this->logInclinometerSensorWarning();
    }
}

void M1M3SSPublisher::logInterlockStatus() {
    _m1m3SAL->logEvent_interlockStatus(&_eventInterlockStatus, 0);
    _previousEventInterlockStatus = _eventInterlockStatus;
}

void M1M3SSPublisher::tryLogInterlockStatus() {
    if (_eventInterlockStatus.heartbeatCommandedState !=
                _previousEventInterlockStatus.heartbeatCommandedState ||
        _eventInterlockStatus.heartbeatOutputState != _previousEventInterlockStatus.heartbeatOutputState) {
        this->logInterlockStatus();
    }
}

void M1M3SSPublisher::logInterlockWarning() {
    _eventInterlockWarning.anyWarning =
            _eventInterlockWarning.heartbeatStateOutputMismatch ||
            _eventInterlockWarning.auxPowerNetworksOff || _eventInterlockWarning.thermalEquipmentOff ||
            _eventInterlockWarning.airSupplyOff || _eventInterlockWarning.cabinetDoorOpen ||
            _eventInterlockWarning.tmaMotionStop || _eventInterlockWarning.gisHeartbeatLost;
    _m1m3SAL->logEvent_interlockWarning(&_eventInterlockWarning, 0);
    _previousEventInterlockWarning = _eventInterlockWarning;
}

void M1M3SSPublisher::tryLogInterlockWarning() {
    if (_eventInterlockWarning.heartbeatStateOutputMismatch !=
                _previousEventInterlockWarning.heartbeatStateOutputMismatch ||
        _eventInterlockWarning.auxPowerNetworksOff != _previousEventInterlockWarning.auxPowerNetworksOff ||
        _eventInterlockWarning.thermalEquipmentOff != _previousEventInterlockWarning.thermalEquipmentOff ||
        _eventInterlockWarning.airSupplyOff != _previousEventInterlockWarning.airSupplyOff ||
        _eventInterlockWarning.cabinetDoorOpen != _previousEventInterlockWarning.cabinetDoorOpen ||
        _eventInterlockWarning.tmaMotionStop != _previousEventInterlockWarning.tmaMotionStop ||
        _eventInterlockWarning.gisHeartbeatLost != _previousEventInterlockWarning.gisHeartbeatLost) {
        this->logInterlockWarning();
    }
}

void M1M3SSPublisher::logModbusResponse() {
    _m1m3SAL->logEvent_modbusResponse(&_eventModbusResponse, 0);
    _previousEventModbusResponse = _eventModbusResponse;
}

void M1M3SSPublisher::tryLogModbusResponse() {
    if (_eventModbusResponse.responseValid != _previousEventModbusResponse.responseValid ||
        _eventModbusResponse.address != _previousEventModbusResponse.address ||
        _eventModbusResponse.functionCode != _previousEventModbusResponse.functionCode ||
        _eventModbusResponse.crc != _previousEventModbusResponse.crc ||
        _eventModbusResponse.dataLength != _previousEventModbusResponse.dataLength) {
        this->logModbusResponse();
    }
}

void M1M3SSPublisher::logPIDInfo() {
    _m1m3SAL->logEvent_pidInfo(&_eventPIDInfo, 0);
    _previousEventPIDInfo = _eventPIDInfo;
}

void M1M3SSPublisher::tryLogPIDInfo() {
    bool changeDetected = false;
    for (int i = 0; i < 6 && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventPIDInfo.timestep[i] != _previousEventPIDInfo.timestep[i] ||
                         _eventPIDInfo.p[i] != _previousEventPIDInfo.p[i] ||
                         _eventPIDInfo.i[i] != _previousEventPIDInfo.i[i] ||
                         _eventPIDInfo.d[i] != _previousEventPIDInfo.d[i] ||
                         _eventPIDInfo.n[i] != _previousEventPIDInfo.n[i] ||
                         _eventPIDInfo.calculatedA[i] != _previousEventPIDInfo.calculatedA[i] ||
                         _eventPIDInfo.calculatedB[i] != _previousEventPIDInfo.calculatedB[i] ||
                         _eventPIDInfo.calculatedC[i] != _previousEventPIDInfo.calculatedC[i] ||
                         _eventPIDInfo.calculatedD[i] != _previousEventPIDInfo.calculatedD[i] ||
                         _eventPIDInfo.calculatedE[i] != _previousEventPIDInfo.calculatedE[i];
    }
    if (changeDetected) {
        this->logPIDInfo();
    }
}

void M1M3SSPublisher::logPowerStatus() {
    _m1m3SAL->logEvent_powerStatus(&_eventPowerStatus, 0);
    _previousEventPowerStatus = _eventPowerStatus;
}

void M1M3SSPublisher::tryLogPowerStatus() {
    if (_eventPowerStatus.powerNetworkACommandedOn != _previousEventPowerStatus.powerNetworkACommandedOn ||
        _eventPowerStatus.powerNetworkAOutputOn != _previousEventPowerStatus.powerNetworkAOutputOn ||
        _eventPowerStatus.powerNetworkBCommandedOn != _previousEventPowerStatus.powerNetworkBCommandedOn ||
        _eventPowerStatus.powerNetworkBOutputOn != _previousEventPowerStatus.powerNetworkBOutputOn ||
        _eventPowerStatus.powerNetworkCCommandedOn != _previousEventPowerStatus.powerNetworkCCommandedOn ||
        _eventPowerStatus.powerNetworkCOutputOn != _previousEventPowerStatus.powerNetworkCOutputOn ||
        _eventPowerStatus.powerNetworkDCommandedOn != _previousEventPowerStatus.powerNetworkDCommandedOn ||
        _eventPowerStatus.powerNetworkDOutputOn != _previousEventPowerStatus.powerNetworkDOutputOn ||
        _eventPowerStatus.auxPowerNetworkACommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkACommandedOn ||
        _eventPowerStatus.auxPowerNetworkAOutputOn != _previousEventPowerStatus.auxPowerNetworkAOutputOn ||
        _eventPowerStatus.auxPowerNetworkBCommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkBCommandedOn ||
        _eventPowerStatus.auxPowerNetworkBOutputOn != _previousEventPowerStatus.auxPowerNetworkBOutputOn ||
        _eventPowerStatus.auxPowerNetworkCCommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkCCommandedOn ||
        _eventPowerStatus.auxPowerNetworkCOutputOn != _previousEventPowerStatus.auxPowerNetworkCOutputOn ||
        _eventPowerStatus.auxPowerNetworkDCommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkDCommandedOn ||
        _eventPowerStatus.auxPowerNetworkDOutputOn != _previousEventPowerStatus.auxPowerNetworkDOutputOn) {
        this->logPowerStatus();
    }
}

void M1M3SSPublisher::logPowerSupplyStatus() {
    _m1m3SAL->logEvent_powerSupplyStatus(&_eventPowerSupplyStatus, 0);
    _previousEventPowerSupplyStatus = _eventPowerSupplyStatus;
}

void M1M3SSPublisher::tryLogPowerSupplyStatus() {
    if (_eventPowerSupplyStatus.rcpMirrorCellUtility220VAC1Status !=
                _previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC1Status ||
        _eventPowerSupplyStatus.rcpCabinetUtility220VACStatus !=
                _previousEventPowerSupplyStatus.rcpCabinetUtility220VACStatus ||
        _eventPowerSupplyStatus.rcpExternalEquipment220VACStatus !=
                _previousEventPowerSupplyStatus.rcpExternalEquipment220VACStatus ||
        _eventPowerSupplyStatus.rcpMirrorCellUtility220VAC2Status !=
                _previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC2Status ||
        _eventPowerSupplyStatus.rcpMirrorCellUtility220VAC3Status !=
                _previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC3Status ||
        _eventPowerSupplyStatus.powerNetworkARedundancyControlStatus !=
                _previousEventPowerSupplyStatus.powerNetworkARedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkBRedundancyControlStatus !=
                _previousEventPowerSupplyStatus.powerNetworkBRedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkCRedundancyControlStatus !=
                _previousEventPowerSupplyStatus.powerNetworkCRedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkDRedundancyControlStatus !=
                _previousEventPowerSupplyStatus.powerNetworkDRedundancyControlStatus ||
        _eventPowerSupplyStatus.controlsPowerNetworkRedundancyControlStatus !=
                _previousEventPowerSupplyStatus.controlsPowerNetworkRedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkAStatus != _previousEventPowerSupplyStatus.powerNetworkAStatus ||
        _eventPowerSupplyStatus.powerNetworkARedundantStatus !=
                _previousEventPowerSupplyStatus.powerNetworkARedundantStatus ||
        _eventPowerSupplyStatus.powerNetworkBStatus != _previousEventPowerSupplyStatus.powerNetworkBStatus ||
        _eventPowerSupplyStatus.powerNetworkBRedundantStatus !=
                _previousEventPowerSupplyStatus.powerNetworkBRedundantStatus ||
        _eventPowerSupplyStatus.powerNetworkCStatus != _previousEventPowerSupplyStatus.powerNetworkCStatus ||
        _eventPowerSupplyStatus.powerNetworkCRedundantStatus !=
                _previousEventPowerSupplyStatus.powerNetworkCRedundantStatus ||
        _eventPowerSupplyStatus.powerNetworkDStatus != _previousEventPowerSupplyStatus.powerNetworkDStatus ||
        _eventPowerSupplyStatus.powerNetworkDRedundantStatus !=
                _previousEventPowerSupplyStatus.powerNetworkDRedundantStatus ||
        _eventPowerSupplyStatus.controlsPowerNetworkStatus !=
                _previousEventPowerSupplyStatus.controlsPowerNetworkStatus ||
        _eventPowerSupplyStatus.controlsPowerNetworkRedundantStatus !=
                _previousEventPowerSupplyStatus.controlsPowerNetworkRedundantStatus ||
        _eventPowerSupplyStatus.lightPowerNetworkStatus !=
                _previousEventPowerSupplyStatus.lightPowerNetworkStatus ||
        _eventPowerSupplyStatus.externalEquipmentPowerNetworkStatus !=
                _previousEventPowerSupplyStatus.externalEquipmentPowerNetworkStatus ||
        _eventPowerSupplyStatus.laserTrackerPowerNetworkStatus !=
                _previousEventPowerSupplyStatus.laserTrackerPowerNetworkStatus) {
        this->logPowerSupplyStatus();
    }
}

void M1M3SSPublisher::logPowerWarning() {
    _eventPowerWarning.anyWarning = _eventPowerWarning.powerNetworkAOutputMismatch ||
                                    _eventPowerWarning.powerNetworkBOutputMismatch ||
                                    _eventPowerWarning.powerNetworkCOutputMismatch ||
                                    _eventPowerWarning.powerNetworkDOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkAOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkBOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkCOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkDOutputMismatch;
    _m1m3SAL->logEvent_powerWarning(&_eventPowerWarning, 0);
    _previousEventPowerWarning = _eventPowerWarning;
}

void M1M3SSPublisher::tryLogPowerWarning() {
    if (_eventPowerWarning.powerNetworkAOutputMismatch !=
                _previousEventPowerWarning.powerNetworkAOutputMismatch ||
        _eventPowerWarning.powerNetworkBOutputMismatch !=
                _previousEventPowerWarning.powerNetworkBOutputMismatch ||
        _eventPowerWarning.powerNetworkCOutputMismatch !=
                _previousEventPowerWarning.powerNetworkCOutputMismatch ||
        _eventPowerWarning.powerNetworkDOutputMismatch !=
                _previousEventPowerWarning.powerNetworkDOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkAOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkAOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkBOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkBOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkCOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkCOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkDOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkDOutputMismatch) {
        this->logPowerWarning();
    }
}

void M1M3SSPublisher::logRejectedAberrationForces() {
    _m1m3SAL->logEvent_rejectedAberrationForces(&_eventRejectedAberrationForces, 0);
    _previousEventRejectedAberrationForces = _eventRejectedAberrationForces;
}

void M1M3SSPublisher::tryLogRejectedAberrationForces() {
    bool changeDetected = _eventRejectedAberrationForces.fz != _previousEventRejectedAberrationForces.fz ||
                          _eventRejectedAberrationForces.mx != _previousEventRejectedAberrationForces.mx ||
                          _eventRejectedAberrationForces.my != _previousEventRejectedAberrationForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventRejectedAberrationForces.zForces[i] !=
                                                   _previousEventRejectedAberrationForces.zForces[i];
    }
    if (changeDetected) {
        this->logRejectedAberrationForces();
    }
}

void M1M3SSPublisher::logRejectedAccelerationForces() {
    _m1m3SAL->logEvent_rejectedAccelerationForces(&_eventRejectedAccelerationForces, 0);
    _previousEventRejectedAccelerationForces = _eventRejectedAccelerationForces;
}

void M1M3SSPublisher::tryLogRejectedAccelerationForces() {
    bool changeDetected =
            _eventRejectedAccelerationForces.fx != _previousEventRejectedAccelerationForces.fx ||
            _eventRejectedAccelerationForces.fy != _previousEventRejectedAccelerationForces.fy ||
            _eventRejectedAccelerationForces.fz != _previousEventRejectedAccelerationForces.fz ||
            _eventRejectedAccelerationForces.mx != _previousEventRejectedAccelerationForces.mx ||
            _eventRejectedAccelerationForces.my != _previousEventRejectedAccelerationForces.my ||
            _eventRejectedAccelerationForces.mz != _previousEventRejectedAccelerationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedAccelerationForces.xForces[i] !=
                                            _previousEventRejectedAccelerationForces.xForces[i]) ||
                         (i < 100 && _eventRejectedAccelerationForces.yForces[i] !=
                                             _previousEventRejectedAccelerationForces.yForces[i]) ||
                         (_eventRejectedAccelerationForces.zForces[i] !=
                          _previousEventRejectedAccelerationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedAccelerationForces();
    }
}

void M1M3SSPublisher::logRejectedActiveOpticForces() {
    _m1m3SAL->logEvent_rejectedActiveOpticForces(&_eventRejectedActiveOpticForces, 0);
    _previousEventRejectedActiveOpticForces = _eventRejectedActiveOpticForces;
}

void M1M3SSPublisher::tryLogRejectedActiveOpticForces() {
    bool changeDetected = _eventRejectedActiveOpticForces.fz != _previousEventRejectedActiveOpticForces.fz ||
                          _eventRejectedActiveOpticForces.mx != _previousEventRejectedActiveOpticForces.mx ||
                          _eventRejectedActiveOpticForces.my != _previousEventRejectedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventRejectedActiveOpticForces.zForces[i] !=
                                                   _previousEventRejectedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        this->logRejectedActiveOpticForces();
    }
}

void M1M3SSPublisher::logRejectedAzimuthForces() {
    _m1m3SAL->logEvent_rejectedAzimuthForces(&_eventRejectedAzimuthForces, 0);
    _previousEventRejectedAzimuthForces = _eventRejectedAzimuthForces;
}

void M1M3SSPublisher::tryLogRejectedAzimuthForces() {
    bool changeDetected = _eventRejectedAzimuthForces.fx != _previousEventRejectedAzimuthForces.fx ||
                          _eventRejectedAzimuthForces.fy != _previousEventRejectedAzimuthForces.fy ||
                          _eventRejectedAzimuthForces.fz != _previousEventRejectedAzimuthForces.fz ||
                          _eventRejectedAzimuthForces.mx != _previousEventRejectedAzimuthForces.mx ||
                          _eventRejectedAzimuthForces.my != _previousEventRejectedAzimuthForces.my ||
                          _eventRejectedAzimuthForces.mz != _previousEventRejectedAzimuthForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventRejectedAzimuthForces.xForces[i] != _previousEventRejectedAzimuthForces.xForces[i]) ||
                (i < 100 &&
                 _eventRejectedAzimuthForces.yForces[i] != _previousEventRejectedAzimuthForces.yForces[i]) ||
                (_eventRejectedAzimuthForces.zForces[i] != _previousEventRejectedAzimuthForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedAzimuthForces();
    }
}

void M1M3SSPublisher::logRejectedBalanceForces() {
    _m1m3SAL->logEvent_rejectedBalanceForces(&_eventRejectedBalanceForces, 0);
    _previousEventRejectedBalanceForces = _eventRejectedBalanceForces;
}

void M1M3SSPublisher::tryLogRejectedBalanceForces() {
    bool changeDetected = _eventRejectedBalanceForces.fx != _previousEventRejectedBalanceForces.fx ||
                          _eventRejectedBalanceForces.fy != _previousEventRejectedBalanceForces.fy ||
                          _eventRejectedBalanceForces.fz != _previousEventRejectedBalanceForces.fz ||
                          _eventRejectedBalanceForces.mx != _previousEventRejectedBalanceForces.mx ||
                          _eventRejectedBalanceForces.my != _previousEventRejectedBalanceForces.my ||
                          _eventRejectedBalanceForces.mz != _previousEventRejectedBalanceForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventRejectedBalanceForces.xForces[i] != _previousEventRejectedBalanceForces.xForces[i]) ||
                (i < 100 &&
                 _eventRejectedBalanceForces.yForces[i] != _previousEventRejectedBalanceForces.yForces[i]) ||
                (_eventRejectedBalanceForces.zForces[i] != _previousEventRejectedBalanceForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedBalanceForces();
    }
}

void M1M3SSPublisher::logRejectedCylinderForces() {
    _m1m3SAL->logEvent_rejectedCylinderForces(&_eventRejectedCylinderForces, 0);
    _previousEventRejectedCylinderForces = _eventRejectedCylinderForces;
}

void M1M3SSPublisher::tryLogRejectedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && _eventRejectedCylinderForces.secondaryCylinderForces[i] !=
                                    _previousEventRejectedCylinderForces.secondaryCylinderForces[i]) ||
                (_eventRejectedCylinderForces.primaryCylinderForces[i] !=
                 _previousEventRejectedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        this->logRejectedCylinderForces();
    }
}

void M1M3SSPublisher::logRejectedElevationForces() {
    _m1m3SAL->logEvent_rejectedElevationForces(&_eventRejectedElevationForces, 0);
    _previousEventRejectedElevationForces = _eventRejectedElevationForces;
}

void M1M3SSPublisher::tryLogRejectedElevationForces() {
    bool changeDetected = _eventRejectedElevationForces.fx != _previousEventRejectedElevationForces.fx ||
                          _eventRejectedElevationForces.fy != _previousEventRejectedElevationForces.fy ||
                          _eventRejectedElevationForces.fz != _previousEventRejectedElevationForces.fz ||
                          _eventRejectedElevationForces.mx != _previousEventRejectedElevationForces.mx ||
                          _eventRejectedElevationForces.my != _previousEventRejectedElevationForces.my ||
                          _eventRejectedElevationForces.mz != _previousEventRejectedElevationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedElevationForces.xForces[i] !=
                                            _previousEventRejectedElevationForces.xForces[i]) ||
                         (i < 100 && _eventRejectedElevationForces.yForces[i] !=
                                             _previousEventRejectedElevationForces.yForces[i]) ||
                         (_eventRejectedElevationForces.zForces[i] !=
                          _previousEventRejectedElevationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedElevationForces();
    }
}

void M1M3SSPublisher::logRejectedForces() {
    _m1m3SAL->logEvent_rejectedForces(&_eventRejectedForces, 0);
    _previousEventRejectedForces = _eventRejectedForces;
}

void M1M3SSPublisher::tryLogRejectedForces() {
    bool changeDetected = _eventRejectedForces.fx != _previousEventRejectedForces.fx ||
                          _eventRejectedForces.fy != _previousEventRejectedForces.fy ||
                          _eventRejectedForces.fz != _previousEventRejectedForces.fz ||
                          _eventRejectedForces.mx != _previousEventRejectedForces.mx ||
                          _eventRejectedForces.my != _previousEventRejectedForces.my ||
                          _eventRejectedForces.mz != _previousEventRejectedForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 && _eventRejectedForces.xForces[i] != _previousEventRejectedForces.xForces[i]) ||
                (i < 100 && _eventRejectedForces.yForces[i] != _previousEventRejectedForces.yForces[i]) ||
                (_eventRejectedForces.zForces[i] != _previousEventRejectedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedForces();
    }
}

void M1M3SSPublisher::logRejectedOffsetForces() {
    _m1m3SAL->logEvent_rejectedOffsetForces(&_eventRejectedOffsetForces, 0);
    _previousEventRejectedOffsetForces = _eventRejectedOffsetForces;
}

void M1M3SSPublisher::tryLogRejectedOffsetForces() {
    bool changeDetected = _eventRejectedOffsetForces.fx != _previousEventRejectedOffsetForces.fx ||
                          _eventRejectedOffsetForces.fy != _previousEventRejectedOffsetForces.fy ||
                          _eventRejectedOffsetForces.fz != _previousEventRejectedOffsetForces.fz ||
                          _eventRejectedOffsetForces.mx != _previousEventRejectedOffsetForces.mx ||
                          _eventRejectedOffsetForces.my != _previousEventRejectedOffsetForces.my ||
                          _eventRejectedOffsetForces.mz != _previousEventRejectedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventRejectedOffsetForces.xForces[i] != _previousEventRejectedOffsetForces.xForces[i]) ||
                (i < 100 &&
                 _eventRejectedOffsetForces.yForces[i] != _previousEventRejectedOffsetForces.yForces[i]) ||
                (_eventRejectedOffsetForces.zForces[i] != _previousEventRejectedOffsetForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedOffsetForces();
    }
}

void M1M3SSPublisher::logRejectedStaticForces() {
    _m1m3SAL->logEvent_rejectedStaticForces(&_eventRejectedStaticForces, 0);
    _previousEventRejectedStaticForces = _eventRejectedStaticForces;
}

void M1M3SSPublisher::tryLogRejectedStaticForces() {
    bool changeDetected = _eventRejectedStaticForces.fx != _previousEventRejectedStaticForces.fx ||
                          _eventRejectedStaticForces.fy != _previousEventRejectedStaticForces.fy ||
                          _eventRejectedStaticForces.fz != _previousEventRejectedStaticForces.fz ||
                          _eventRejectedStaticForces.mx != _previousEventRejectedStaticForces.mx ||
                          _eventRejectedStaticForces.my != _previousEventRejectedStaticForces.my ||
                          _eventRejectedStaticForces.mz != _previousEventRejectedStaticForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventRejectedStaticForces.xForces[i] != _previousEventRejectedStaticForces.xForces[i]) ||
                (i < 100 &&
                 _eventRejectedStaticForces.yForces[i] != _previousEventRejectedStaticForces.yForces[i]) ||
                (_eventRejectedStaticForces.zForces[i] != _previousEventRejectedStaticForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedStaticForces();
    }
}

void M1M3SSPublisher::logRejectedThermalForces() {
    _m1m3SAL->logEvent_rejectedThermalForces(&_eventRejectedThermalForces, 0);
    _previousEventRejectedThermalForces = _eventRejectedThermalForces;
}

void M1M3SSPublisher::tryLogRejectedThermalForces() {
    bool changeDetected = _eventRejectedThermalForces.fx != _previousEventRejectedThermalForces.fx ||
                          _eventRejectedThermalForces.fy != _previousEventRejectedThermalForces.fy ||
                          _eventRejectedThermalForces.fz != _previousEventRejectedThermalForces.fz ||
                          _eventRejectedThermalForces.mx != _previousEventRejectedThermalForces.mx ||
                          _eventRejectedThermalForces.my != _previousEventRejectedThermalForces.my ||
                          _eventRejectedThermalForces.mz != _previousEventRejectedThermalForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventRejectedThermalForces.xForces[i] != _previousEventRejectedThermalForces.xForces[i]) ||
                (i < 100 &&
                 _eventRejectedThermalForces.yForces[i] != _previousEventRejectedThermalForces.yForces[i]) ||
                (_eventRejectedThermalForces.zForces[i] != _previousEventRejectedThermalForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedThermalForces();
    }
}

void M1M3SSPublisher::logRejectedVelocityForces() {
    _m1m3SAL->logEvent_rejectedVelocityForces(&_eventRejectedVelocityForces, 0);
    _previousEventRejectedVelocityForces = _eventRejectedVelocityForces;
}

void M1M3SSPublisher::tryLogRejectedVelocityForces() {
    bool changeDetected = _eventRejectedVelocityForces.fx != _previousEventRejectedVelocityForces.fx ||
                          _eventRejectedVelocityForces.fy != _previousEventRejectedVelocityForces.fy ||
                          _eventRejectedVelocityForces.fz != _previousEventRejectedVelocityForces.fz ||
                          _eventRejectedVelocityForces.mx != _previousEventRejectedVelocityForces.mx ||
                          _eventRejectedVelocityForces.my != _previousEventRejectedVelocityForces.my ||
                          _eventRejectedVelocityForces.mz != _previousEventRejectedVelocityForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 && _eventRejectedVelocityForces.xForces[i] !=
                                   _previousEventRejectedVelocityForces.xForces[i]) ||
                (i < 100 && _eventRejectedVelocityForces.yForces[i] !=
                                    _previousEventRejectedVelocityForces.yForces[i]) ||
                (_eventRejectedVelocityForces.zForces[i] != _previousEventRejectedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedVelocityForces();
    }
}

void M1M3SSPublisher::logSettingVersions() { _m1m3SAL->logEvent_settingVersions(&_eventSettingVersions, 0); }

void M1M3SSPublisher::logSettingsApplied() {
    _m1m3SAL->logEvent_settingsApplied(&_eventSettingsApplied, 0);
    _previousEventSettingsApplied = _eventSettingsApplied;
}

void M1M3SSPublisher::tryLogSettingsApplied() {
    if (_eventSettingsApplied.settingsVersion.compare(_previousEventSettingsApplied.settingsVersion) != 0) {
        this->logSettingsApplied();
    }
}

void M1M3SSPublisher::logSummaryState() {
    _m1m3SAL->logEvent_summaryState(&_eventSummaryState, 0);
    _previousEventSummaryState = _eventSummaryState;
}

void M1M3SSPublisher::tryLogSummaryState() {
    if (_eventSummaryState.summaryState != _previousEventSummaryState.summaryState) {
        this->logSummaryState();
    }
}

void M1M3SSPublisher::ackCommandstart(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_start(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenable(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_enable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommanddisable(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_disable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandstandby(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_standby(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandexitControl(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_exitControl(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnAirOn(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_turnAirOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnAirOff(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_turnAirOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyOffsetForces(int32_t commandID, int32_t ackCode,
                                                  std::string description) {
    _m1m3SAL->ackCommand_applyOffsetForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandclearOffsetForces(int32_t commandID, int32_t ackCode,
                                                  std::string description) {
    _m1m3SAL->ackCommand_clearOffsetForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandraiseM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_raiseM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandlowerM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_lowerM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyActiveOpticForcesByBendingModes(int32_t commandID, int32_t ackCode,
                                                                     std::string description) {
    _m1m3SAL->ackCommand_applyActiveOpticForcesByBendingModes(commandID, ackCode, 0,
                                                              (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyActiveOpticForces(int32_t commandID, int32_t ackCode,
                                                       std::string description) {
    _m1m3SAL->ackCommand_applyActiveOpticForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandclearActiveOpticForces(int32_t commandID, int32_t ackCode,
                                                       std::string description) {
    _m1m3SAL->ackCommand_clearActiveOpticForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyAberrationForcesByBendingModes(int32_t commandID, int32_t ackCode,
                                                                    std::string description) {
    _m1m3SAL->ackCommand_applyAberrationForcesByBendingModes(commandID, ackCode, 0,
                                                             (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyAberrationForces(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    _m1m3SAL->ackCommand_applyAberrationForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandclearAberrationForces(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    _m1m3SAL->ackCommand_clearAberrationForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenterEngineering(int32_t commandID, int32_t ackCode,
                                                 std::string description) {
    _m1m3SAL->ackCommand_enterEngineering(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandexitEngineering(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_exitEngineering(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtestAir(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_testAir(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtestHardpoint(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_testHardpoint(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtestForceActuator(int32_t commandID, int32_t ackCode,
                                                  std::string description) {
    _m1m3SAL->ackCommand_testForceActuator(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandmoveHardpointActuators(int32_t commandID, int32_t ackCode,
                                                       std::string description) {
    _m1m3SAL->ackCommand_moveHardpointActuators(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenableHardpointChase(int32_t commandID, int32_t ackCode,
                                                     std::string description) {
    _m1m3SAL->ackCommand_enableHardpointChase(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommanddisableHardpointChase(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    _m1m3SAL->ackCommand_disableHardpointChase(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandabortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_abortRaiseM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtranslateM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_translateM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandstopHardpointMotion(int32_t commandID, int32_t ackCode,
                                                    std::string description) {
    _m1m3SAL->ackCommand_stopHardpointMotion(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandpositionM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_positionM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnLightsOn(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_turnLightsOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnLightsOff(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_turnLightsOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnPowerOn(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_turnPowerOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnPowerOff(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_turnPowerOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenableHardpointCorrections(int32_t commandID, int32_t ackCode,
                                                           std::string description) {
    _m1m3SAL->ackCommand_enableHardpointCorrections(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommanddisableHardpointCorrections(int32_t commandID, int32_t ackCode,
                                                            std::string description) {
    _m1m3SAL->ackCommand_disableHardpointCorrections(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandrunMirrorForceProfile(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    _m1m3SAL->ackCommand_runMirrorForceProfile(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandabortProfile(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_abortProfile(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyOffsetForcesByMirrorForce(int32_t commandID, int32_t ackCode,
                                                               std::string description) {
    _m1m3SAL->ackCommand_applyOffsetForcesByMirrorForce(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandupdatePID(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_updatePID(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandresetPID(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_resetPID(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandprogramILC(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_programILC(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandmodbusTransmit(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_modbusTransmit(commandID, ackCode, 0, (char*)description.c_str());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
