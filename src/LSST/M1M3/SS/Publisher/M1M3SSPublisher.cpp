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
#include <SAL_MTM1M3.h>
#include <ccpp_sal_MTM1M3.h>  // Provides access to enumerations

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSPublisher::M1M3SSPublisher(std::shared_ptr<SAL_MTM1M3> m1m3SAL) {
    spdlog::debug("M1M3SSPublisher: M1M3SSPublisher()");
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

void M1M3SSPublisher::putAccelerometerData() {
    _m1m3SAL->putSample_accelerometerData(&_accelerometerData);
}
void M1M3SSPublisher::putForceActuatorData() {
    _m1m3SAL->putSample_forceActuatorData(&_forceActuatorData);
}
void M1M3SSPublisher::putGyroData() { _m1m3SAL->putSample_gyroData(&_gyroData); }
void M1M3SSPublisher::putHardpointActuatorData() {
    _m1m3SAL->putSample_hardpointActuatorData(&_hardpointActuatorData);
}
void M1M3SSPublisher::putHardpointMonitorData() {
    _m1m3SAL->putSample_hardpointMonitorData(&_hardpointMonitorData);
}
void M1M3SSPublisher::putIMSData() { _m1m3SAL->putSample_imsData(&_imsData); }
void M1M3SSPublisher::putInclinometerData() {
    _m1m3SAL->putSample_inclinometerData(&_inclinometerData);
}
void M1M3SSPublisher::putOuterLoopData() { _m1m3SAL->putSample_outerLoopData(&_outerLoopData); }
void M1M3SSPublisher::putPIDData() { _m1m3SAL->putSample_pidData(&_pidData); }
void M1M3SSPublisher::putPowerSupplyData() {
    _m1m3SAL->putSample_powerSupplyData(&_powerSupplyData);
}

void M1M3SSPublisher::logAccelerometerWarning() {
    _eventAccelerometerWarning.anyWarning = _eventAccelerometerWarning.responseTimeout;
    _m1m3SAL->logEvent_accelerometerWarning(&_eventAccelerometerWarning, 0);
    _previousEventAccelerometerWarning = _eventAccelerometerWarning;
}

void M1M3SSPublisher::tryLogAccelerometerWarning() {
    if (_eventAccelerometerWarning.responseTimeout !=
        _previousEventAccelerometerWarning.responseTimeout) {
        this->logAccelerometerWarning();
    }
}

void M1M3SSPublisher::logAirSupplyStatus() {
    _m1m3SAL->logEvent_airSupplyStatus(&_eventAirSupplyStatus, 0);
    _previousEventAirSupplyStatus = _eventAirSupplyStatus;
}

void M1M3SSPublisher::tryLogAirSupplyStatus() {
    if (_eventAirSupplyStatus.airCommandedOn != _previousEventAirSupplyStatus.airCommandedOn ||
        _eventAirSupplyStatus.airCommandOutputOn !=
                _previousEventAirSupplyStatus.airCommandOutputOn ||
        _eventAirSupplyStatus.airValveOpened != _previousEventAirSupplyStatus.airValveOpened ||
        _eventAirSupplyStatus.airValveClosed != _previousEventAirSupplyStatus.airValveClosed) {
        this->logAirSupplyStatus();
    }
}

void M1M3SSPublisher::logAirSupplyWarning() {
    _eventAirSupplyWarning.anyWarning = _eventAirSupplyWarning.commandOutputMismatch ||
                                             _eventAirSupplyWarning.commandSensorMismatch;
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
    this->previousEventAppliedAberrationForces = _eventAppliedAberrationForces;
}

void M1M3SSPublisher::tryLogAppliedAberrationForces() {
    bool changeDetected =
            _eventAppliedAberrationForces.fz != this->previousEventAppliedAberrationForces.fz ||
            _eventAppliedAberrationForces.mx != this->previousEventAppliedAberrationForces.mx ||
            _eventAppliedAberrationForces.my != this->previousEventAppliedAberrationForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventAppliedAberrationForces.zForces[i] !=
                                                   this->previousEventAppliedAberrationForces.zForces[i];
    }
    if (changeDetected) {
        this->logAppliedAberrationForces();
    }
}

void M1M3SSPublisher::logAppliedAccelerationForces() {
    _m1m3SAL->logEvent_appliedAccelerationForces(&_eventAppliedAccelerationForces, 0);
    this->previousEventAppliedAccelerationForces = _eventAppliedAccelerationForces;
}

void M1M3SSPublisher::tryLogAppliedAccelerationForces() {
    bool changeDetected =
            _eventAppliedAccelerationForces.fx != this->previousEventAppliedAccelerationForces.fx ||
            _eventAppliedAccelerationForces.fy != this->previousEventAppliedAccelerationForces.fy ||
            _eventAppliedAccelerationForces.fz != this->previousEventAppliedAccelerationForces.fz ||
            _eventAppliedAccelerationForces.mx != this->previousEventAppliedAccelerationForces.mx ||
            _eventAppliedAccelerationForces.my != this->previousEventAppliedAccelerationForces.my ||
            _eventAppliedAccelerationForces.mz != this->previousEventAppliedAccelerationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedAccelerationForces.xForces[i] !=
                                            this->previousEventAppliedAccelerationForces.xForces[i]) ||
                         (i < 100 && _eventAppliedAccelerationForces.yForces[i] !=
                                             this->previousEventAppliedAccelerationForces.yForces[i]) ||
                         (_eventAppliedAccelerationForces.zForces[i] !=
                          this->previousEventAppliedAccelerationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedAccelerationForces();
    }
}

void M1M3SSPublisher::logAppliedActiveOpticForces() {
    _m1m3SAL->logEvent_appliedActiveOpticForces(&_eventAppliedActiveOpticForces, 0);
    this->previousEventAppliedActiveOpticForces = _eventAppliedActiveOpticForces;
}

void M1M3SSPublisher::tryLogAppliedActiveOpticForces() {
    bool changeDetected =
            _eventAppliedActiveOpticForces.fz != this->previousEventAppliedActiveOpticForces.fz ||
            _eventAppliedActiveOpticForces.mx != this->previousEventAppliedActiveOpticForces.mx ||
            _eventAppliedActiveOpticForces.my != this->previousEventAppliedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventAppliedActiveOpticForces.zForces[i] !=
                                                   this->previousEventAppliedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        this->logAppliedActiveOpticForces();
    }
}

void M1M3SSPublisher::logAppliedAzimuthForces() {
    _m1m3SAL->logEvent_appliedAzimuthForces(&_eventAppliedAzimuthForces, 0);
    this->previousEventAppliedAzimuthForces = _eventAppliedAzimuthForces;
}

void M1M3SSPublisher::tryLogAppliedAzimuthForces() {
    bool changeDetected = _eventAppliedAzimuthForces.fx != this->previousEventAppliedAzimuthForces.fx ||
                          _eventAppliedAzimuthForces.fy != this->previousEventAppliedAzimuthForces.fy ||
                          _eventAppliedAzimuthForces.fz != this->previousEventAppliedAzimuthForces.fz ||
                          _eventAppliedAzimuthForces.mx != this->previousEventAppliedAzimuthForces.mx ||
                          _eventAppliedAzimuthForces.my != this->previousEventAppliedAzimuthForces.my ||
                          _eventAppliedAzimuthForces.mz != this->previousEventAppliedAzimuthForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedAzimuthForces.xForces[i] !=
                                            this->previousEventAppliedAzimuthForces.xForces[i]) ||
                         (i < 100 && _eventAppliedAzimuthForces.yForces[i] !=
                                             this->previousEventAppliedAzimuthForces.yForces[i]) ||
                         (_eventAppliedAzimuthForces.zForces[i] !=
                          this->previousEventAppliedAzimuthForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedAzimuthForces();
    }
}

void M1M3SSPublisher::logAppliedBalanceForces() {
    _m1m3SAL->logEvent_appliedBalanceForces(&_eventAppliedBalanceForces, 0);
    this->previousEventAppliedBalanceForces = _eventAppliedBalanceForces;
}

void M1M3SSPublisher::tryLogAppliedBalanceForces() {
    bool changeDetected = _eventAppliedBalanceForces.fx != this->previousEventAppliedBalanceForces.fx ||
                          _eventAppliedBalanceForces.fy != this->previousEventAppliedBalanceForces.fy ||
                          _eventAppliedBalanceForces.fz != this->previousEventAppliedBalanceForces.fz ||
                          _eventAppliedBalanceForces.mx != this->previousEventAppliedBalanceForces.mx ||
                          _eventAppliedBalanceForces.my != this->previousEventAppliedBalanceForces.my ||
                          _eventAppliedBalanceForces.mz != this->previousEventAppliedBalanceForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedBalanceForces.xForces[i] !=
                                            this->previousEventAppliedBalanceForces.xForces[i]) ||
                         (i < 100 && _eventAppliedBalanceForces.yForces[i] !=
                                             this->previousEventAppliedBalanceForces.yForces[i]) ||
                         (_eventAppliedBalanceForces.zForces[i] !=
                          this->previousEventAppliedBalanceForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedBalanceForces();
    }
}

void M1M3SSPublisher::logAppliedCylinderForces() {
    _m1m3SAL->logEvent_appliedCylinderForces(&_eventAppliedCylinderForces, 0);
    this->previousEventAppliedCylinderForces = _eventAppliedCylinderForces;
}

void M1M3SSPublisher::tryLogAppliedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && _eventAppliedCylinderForces.secondaryCylinderForces[i] !=
                                    this->previousEventAppliedCylinderForces.secondaryCylinderForces[i]) ||
                (_eventAppliedCylinderForces.primaryCylinderForces[i] !=
                 this->previousEventAppliedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        this->logAppliedCylinderForces();
    }
}

void M1M3SSPublisher::logAppliedElevationForces() {
    _m1m3SAL->logEvent_appliedElevationForces(&this->eventAppliedElevationForces, 0);
    this->previousEventAppliedElevationForces = this->eventAppliedElevationForces;
}

void M1M3SSPublisher::tryLogAppliedElevationForces() {
    bool changeDetected =
            this->eventAppliedElevationForces.fx != this->previousEventAppliedElevationForces.fx ||
            this->eventAppliedElevationForces.fy != this->previousEventAppliedElevationForces.fy ||
            this->eventAppliedElevationForces.fz != this->previousEventAppliedElevationForces.fz ||
            this->eventAppliedElevationForces.mx != this->previousEventAppliedElevationForces.mx ||
            this->eventAppliedElevationForces.my != this->previousEventAppliedElevationForces.my ||
            this->eventAppliedElevationForces.mz != this->previousEventAppliedElevationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedElevationForces.xForces[i] !=
                                            this->previousEventAppliedElevationForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedElevationForces.yForces[i] !=
                                             this->previousEventAppliedElevationForces.yForces[i]) ||
                         (this->eventAppliedElevationForces.zForces[i] !=
                          this->previousEventAppliedElevationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedElevationForces();
    }
}

void M1M3SSPublisher::logAppliedForces() {
    _m1m3SAL->logEvent_appliedForces(&_eventAppliedForces, 0);
    this->previousEventAppliedForces = _eventAppliedForces;
}

void M1M3SSPublisher::tryLogAppliedForces() {
    bool changeDetected = _eventAppliedForces.fx != this->previousEventAppliedForces.fx ||
                          _eventAppliedForces.fy != this->previousEventAppliedForces.fy ||
                          _eventAppliedForces.fz != this->previousEventAppliedForces.fz ||
                          _eventAppliedForces.mx != this->previousEventAppliedForces.mx ||
                          _eventAppliedForces.my != this->previousEventAppliedForces.my ||
                          _eventAppliedForces.mz != this->previousEventAppliedForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedForces.xForces[i] !=
                                            this->previousEventAppliedForces.xForces[i]) ||
                         (i < 100 && _eventAppliedForces.yForces[i] !=
                                             this->previousEventAppliedForces.yForces[i]) ||
                         (_eventAppliedForces.zForces[i] != this->previousEventAppliedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedForces();
    }
}

void M1M3SSPublisher::logAppliedOffsetForces() {
    _m1m3SAL->logEvent_appliedOffsetForces(&_eventAppliedOffsetForces, 0);
    this->previousEventAppliedOffsetForces = _eventAppliedOffsetForces;
}

void M1M3SSPublisher::tryLogAppliedOffsetForces() {
    bool changeDetected = _eventAppliedOffsetForces.fx != this->previousEventAppliedOffsetForces.fx ||
                          _eventAppliedOffsetForces.fy != this->previousEventAppliedOffsetForces.fy ||
                          _eventAppliedOffsetForces.fz != this->previousEventAppliedOffsetForces.fz ||
                          _eventAppliedOffsetForces.mx != this->previousEventAppliedOffsetForces.mx ||
                          _eventAppliedOffsetForces.my != this->previousEventAppliedOffsetForces.my ||
                          _eventAppliedOffsetForces.mz != this->previousEventAppliedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedOffsetForces.xForces[i] != this->previousEventAppliedOffsetForces.xForces[i]) || (i < 100 && _eventAppliedOffsetForces.yForces[i] != this->previousEventAppliedOffsetForces.yForces[i]) || (_eventAppliedOffsetForces.zForces[i] != this->previousEventAppliedOffsetForces.zForces[i]); } if (changeDetected) {
        this->logAppliedOffsetForces();
    } }

void M1M3SSPublisher::logAppliedSettingsMatchStart() { _m1m3SAL->logEvent_appliedSettingsMatchStart(&_eventAppliedSettingsMatchStart, 0);
    this->previousEventAppliedSettingsMatchStart = _eventAppliedSettingsMatchStart;
}

void M1M3SSPublisher::tryLogAppliedSettingsMatchStart() {
    if (_eventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue !=
        this->previousEventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue) {
        this->logAppliedSettingsMatchStart();
    }
}

void M1M3SSPublisher::logAppliedStaticForces() {
    _m1m3SAL->logEvent_appliedStaticForces(&this->eventAppliedStaticForces, 0);
    this->previousEventAppliedStaticForces = this->eventAppliedStaticForces;
}

void M1M3SSPublisher::tryLogAppliedStaticForces() {
    bool changeDetected = this->eventAppliedStaticForces.fx != this->previousEventAppliedStaticForces.fx ||
                          this->eventAppliedStaticForces.fy != this->previousEventAppliedStaticForces.fy ||
                          this->eventAppliedStaticForces.fz != this->previousEventAppliedStaticForces.fz ||
                          this->eventAppliedStaticForces.mx != this->previousEventAppliedStaticForces.mx ||
                          this->eventAppliedStaticForces.my != this->previousEventAppliedStaticForces.my ||
                          this->eventAppliedStaticForces.mz != this->previousEventAppliedStaticForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedStaticForces.xForces[i] !=
                                            this->previousEventAppliedStaticForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedStaticForces.yForces[i] !=
                                             this->previousEventAppliedStaticForces.yForces[i]) ||
                         (this->eventAppliedStaticForces.zForces[i] !=
                          this->previousEventAppliedStaticForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedStaticForces();
    }
}

void M1M3SSPublisher::logAppliedThermalForces() {
    _m1m3SAL->logEvent_appliedThermalForces(&_eventAppliedThermalForces, 0);
    this->previousEventAppliedThermalForces = _eventAppliedThermalForces;
}

void M1M3SSPublisher::tryLogAppliedThermalForces() { bool changeDetected = _eventAppliedThermalForces.fx != this->previousEventAppliedThermalForces.fx ||
                          _eventAppliedThermalForces.fy != this->previousEventAppliedThermalForces.fy ||
                          _eventAppliedThermalForces.fz != this->previousEventAppliedThermalForces.fz ||
                          _eventAppliedThermalForces.mx != this->previousEventAppliedThermalForces.mx ||
                          _eventAppliedThermalForces.my != this->previousEventAppliedThermalForces.my ||
                          _eventAppliedThermalForces.mz != this->previousEventAppliedThermalForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedThermalForces.xForces[i] !=
                                            this->previousEventAppliedThermalForces.xForces[i]) ||
                         (i < 100 && _eventAppliedThermalForces.yForces[i] !=
                                             this->previousEventAppliedThermalForces.yForces[i]) ||
                         (_eventAppliedThermalForces.zForces[i] !=
                          this->previousEventAppliedThermalForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedThermalForces();
    }
}

void M1M3SSPublisher::logAppliedVelocityForces() {
    _m1m3SAL->logEvent_appliedVelocityForces(&_eventAppliedVelocityForces, 0);
    this->previousEventAppliedVelocityForces = _eventAppliedVelocityForces;
}

void M1M3SSPublisher::tryLogAppliedVelocityForces() {
    bool changeDetected =
            _eventAppliedVelocityForces.fx != this->previousEventAppliedVelocityForces.fx ||
            _eventAppliedVelocityForces.fy != this->previousEventAppliedVelocityForces.fy ||
            _eventAppliedVelocityForces.fz != this->previousEventAppliedVelocityForces.fz ||
            _eventAppliedVelocityForces.mx != this->previousEventAppliedVelocityForces.mx ||
            _eventAppliedVelocityForces.my != this->previousEventAppliedVelocityForces.my ||
            _eventAppliedVelocityForces.mz != this->previousEventAppliedVelocityForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventAppliedVelocityForces.xForces[i] !=
                                            this->previousEventAppliedVelocityForces.xForces[i]) ||
                         (i < 100 && _eventAppliedVelocityForces.yForces[i] !=
                                             this->previousEventAppliedVelocityForces.yForces[i]) ||
                         (_eventAppliedVelocityForces.zForces[i] !=
                          this->previousEventAppliedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedVelocityForces();
    }
}

void M1M3SSPublisher::logCellLightStatus() {
    _m1m3SAL->logEvent_cellLightStatus(&_eventCellLightStatus, 0);
    this->previousEventCellLightStatus = _eventCellLightStatus;
}

void M1M3SSPublisher::tryLogCellLightStatus() {
    if (_eventCellLightStatus.cellLightsCommandedOn !=
                this->previousEventCellLightStatus.cellLightsCommandedOn ||
        _eventCellLightStatus.cellLightsOutputOn !=
                this->previousEventCellLightStatus.cellLightsOutputOn ||
        _eventCellLightStatus.cellLightsOn != this->previousEventCellLightStatus.cellLightsOn) {
        this->logCellLightStatus();
    }
}

void M1M3SSPublisher::logCellLightWarning() {
    _eventCellLightWarning.anyWarning = _eventCellLightWarning.cellLightsOutputMismatch ||
                                             _eventCellLightWarning.cellLightsSensorMismatch;
    _m1m3SAL->logEvent_cellLightWarning(&_eventCellLightWarning, 0);
    this->previousEventCellLightWarning = _eventCellLightWarning;
}

void M1M3SSPublisher::tryLogCellLightWarning() {
    if (_eventCellLightWarning.cellLightsOutputMismatch !=
                this->previousEventCellLightWarning.cellLightsOutputMismatch ||
        _eventCellLightWarning.cellLightsSensorMismatch !=
                this->previousEventCellLightWarning.cellLightsSensorMismatch) {
        this->logCellLightWarning();
    }
}

void M1M3SSPublisher::logCommandRejectionWarning() {
    _m1m3SAL->logEvent_commandRejectionWarning(&_eventCommandRejectionWarning, 0);
    this->previousEventCommandRejectionWarning = _eventCommandRejectionWarning;
}

void M1M3SSPublisher::tryLogCommandRejectionWarning() {
    if (_eventCommandRejectionWarning.command.compare(
                this->previousEventCommandRejectionWarning.command) != 0 ||
        _eventCommandRejectionWarning.reason.compare(
                this->previousEventCommandRejectionWarning.reason) != 0) {
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
    this->previousEventDetailedState = _eventDetailedState;
}

void M1M3SSPublisher::tryLogDetailedState() {
    if (_eventDetailedState.detailedState != this->previousEventDetailedState.detailedState) {
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
            _eventDisplacementSensorWarning.unknownCommand ||
            _eventDisplacementSensorWarning.unknownProblem;
    _m1m3SAL->logEvent_displacementSensorWarning(&_eventDisplacementSensorWarning, 0);
    this->previousEventDisplacementSensorWarning = _eventDisplacementSensorWarning;
}

void M1M3SSPublisher::tryLogDisplacementSensorWarning() {
    if (_eventDisplacementSensorWarning.sensorReportsInvalidCommand !=
                this->previousEventDisplacementSensorWarning.sensorReportsInvalidCommand ||
        _eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError !=
                this->previousEventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
        _eventDisplacementSensorWarning.sensorReportsDataLengthError !=
                this->previousEventDisplacementSensorWarning.sensorReportsDataLengthError ||
        _eventDisplacementSensorWarning.sensorReportsNumberOfParametersError !=
                this->previousEventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
        _eventDisplacementSensorWarning.sensorReportsParameterError !=
                this->previousEventDisplacementSensorWarning.sensorReportsParameterError ||
        _eventDisplacementSensorWarning.sensorReportsCommunicationError !=
                this->previousEventDisplacementSensorWarning.sensorReportsCommunicationError ||
        _eventDisplacementSensorWarning.sensorReportsIDNumberError !=
                this->previousEventDisplacementSensorWarning.sensorReportsIDNumberError ||
        _eventDisplacementSensorWarning.sensorReportsExpansionLineError !=
                this->previousEventDisplacementSensorWarning.sensorReportsExpansionLineError ||
        _eventDisplacementSensorWarning.sensorReportsWriteControlError !=
                this->previousEventDisplacementSensorWarning.sensorReportsWriteControlError ||
        _eventDisplacementSensorWarning.responseTimeout !=
                this->previousEventDisplacementSensorWarning.responseTimeout ||
        _eventDisplacementSensorWarning.invalidLength !=
                this->previousEventDisplacementSensorWarning.invalidLength ||
        _eventDisplacementSensorWarning.invalidResponse !=
                this->previousEventDisplacementSensorWarning.invalidResponse ||
        _eventDisplacementSensorWarning.unknownCommand !=
                this->previousEventDisplacementSensorWarning.unknownCommand ||
        _eventDisplacementSensorWarning.unknownProblem !=
                this->previousEventDisplacementSensorWarning.unknownProblem) {
        this->logDisplacementSensorWarning();
    }
}

void M1M3SSPublisher::logErrorCode() {
    _m1m3SAL->logEvent_errorCode(&_eventErrorCode, 0);
    this->previousEventErrorCode = _eventErrorCode;
}

void M1M3SSPublisher::tryLogErrorCode() {
    if (_eventErrorCode.errorCode != this->previousEventErrorCode.errorCode) {
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
    this->previousEventForceActuatorForceWarning = _eventForceActuatorForceWarning;
}

void M1M3SSPublisher::tryLogForceActuatorForceWarning() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                _eventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i] !=
                        this->previousEventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i] ||
                _eventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i] !=
                        this->previousEventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i] ||
                _eventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i] !=
                        this->previousEventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i] ||
                _eventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i] !=
                        this->previousEventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i];
    }
    if (changeDetected) {
        this->logForceActuatorForceWarning();
    }
}

void M1M3SSPublisher::logForceActuatorInfo() {
    _m1m3SAL->logEvent_forceActuatorInfo(&_eventForceActuatorInfo, 0);
    this->previousEventForceActuatorInfo = _eventForceActuatorInfo;
}

void M1M3SSPublisher::tryLogForceActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                _eventForceActuatorInfo.referenceId[i] !=
                        this->previousEventForceActuatorInfo.referenceId[i] ||
                _eventForceActuatorInfo.xDataReferenceId[i] !=
                        this->previousEventForceActuatorInfo.xDataReferenceId[i] ||
                _eventForceActuatorInfo.yDataReferenceId[i] !=
                        this->previousEventForceActuatorInfo.yDataReferenceId[i] ||
                _eventForceActuatorInfo.zDataReferenceId[i] !=
                        this->previousEventForceActuatorInfo.zDataReferenceId[i] ||
                _eventForceActuatorInfo.actuatorType[i] !=
                        this->previousEventForceActuatorInfo.actuatorType[i] ||
                _eventForceActuatorInfo.actuatorOrientation[i] !=
                        this->previousEventForceActuatorInfo.actuatorOrientation[i] ||
                _eventForceActuatorInfo.modbusSubnet[i] !=
                        this->previousEventForceActuatorInfo.modbusSubnet[i] ||
                _eventForceActuatorInfo.modbusAddress[i] !=
                        this->previousEventForceActuatorInfo.modbusAddress[i] ||
                _eventForceActuatorInfo.xPosition[i] !=
                        this->previousEventForceActuatorInfo.xPosition[i] ||
                _eventForceActuatorInfo.yPosition[i] !=
                        this->previousEventForceActuatorInfo.yPosition[i] ||
                _eventForceActuatorInfo.zPosition[i] !=
                        this->previousEventForceActuatorInfo.zPosition[i] ||
                _eventForceActuatorInfo.ilcUniqueId[i] !=
                        this->previousEventForceActuatorInfo.ilcUniqueId[i] ||
                _eventForceActuatorInfo.ilcApplicationType[i] !=
                        this->previousEventForceActuatorInfo.ilcApplicationType[i] ||
                _eventForceActuatorInfo.networkNodeType[i] !=
                        this->previousEventForceActuatorInfo.networkNodeType[i] ||
                _eventForceActuatorInfo.ilcSelectedOptions[i] !=
                        this->previousEventForceActuatorInfo.ilcSelectedOptions[i] ||
                _eventForceActuatorInfo.networkNodeOptions[i] !=
                        this->previousEventForceActuatorInfo.networkNodeOptions[i] ||
                _eventForceActuatorInfo.majorRevision[i] !=
                        this->previousEventForceActuatorInfo.majorRevision[i] ||
                _eventForceActuatorInfo.minorRevision[i] !=
                        this->previousEventForceActuatorInfo.minorRevision[i] ||
                _eventForceActuatorInfo.adcScanRate[i] !=
                        this->previousEventForceActuatorInfo.adcScanRate[i] ||
                _eventForceActuatorInfo.mainPrimaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.mainPrimaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.mainSecondaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.mainSecondaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.mainPrimaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.mainPrimaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.mainSecondaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.mainSecondaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.mainPrimaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.mainPrimaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.mainSecondaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.mainSecondaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.backupPrimaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.backupPrimaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.backupSecondaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.backupSecondaryCylinderCoefficient[i] ||
                _eventForceActuatorInfo.backupPrimaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.backupPrimaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.backupSecondaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.backupSecondaryCylinderLoadCellOffset[i] ||
                _eventForceActuatorInfo.backupPrimaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.backupPrimaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.backupSecondaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.backupSecondaryCylinderLoadCellSensitivity[i] ||
                _eventForceActuatorInfo.mezzaninePrimaryCylinderGain[i] !=
                        this->previousEventForceActuatorInfo.mezzaninePrimaryCylinderGain[i] ||
                _eventForceActuatorInfo.mezzanineSecondaryCylinderGain[i] !=
                        this->previousEventForceActuatorInfo.mezzanineSecondaryCylinderGain[i] ||
                _eventForceActuatorInfo.mezzanineUniqueId[i] !=
                        this->previousEventForceActuatorInfo.mezzanineUniqueId[i] ||
                _eventForceActuatorInfo.mezzanineFirmwareType[i] !=
                        this->previousEventForceActuatorInfo.mezzanineFirmwareType[i] ||
                _eventForceActuatorInfo.mezzanineMajorRevision[i] !=
                        this->previousEventForceActuatorInfo.mezzanineMajorRevision[i] ||
                _eventForceActuatorInfo.mezzanineMinorRevision[i] !=
                        this->previousEventForceActuatorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        this->logForceActuatorInfo();
    }
}

void M1M3SSPublisher::logForceActuatorState() {
    _m1m3SAL->logEvent_forceActuatorState(&_eventForceActuatorState, 0);
    this->previousEventForceActuatorState = _eventForceActuatorState;
}

void M1M3SSPublisher::tryLogForceActuatorState() {
    bool changeDetected =
            _eventForceActuatorState.slewFlag != this->previousEventForceActuatorState.slewFlag ||
            _eventForceActuatorState.staticForcesApplied !=
                    this->previousEventForceActuatorState.staticForcesApplied ||
            _eventForceActuatorState.elevationForcesApplied !=
                    this->previousEventForceActuatorState.elevationForcesApplied ||
            _eventForceActuatorState.azimuthForcesApplied !=
                    this->previousEventForceActuatorState.azimuthForcesApplied ||
            _eventForceActuatorState.thermalForcesApplied !=
                    this->previousEventForceActuatorState.thermalForcesApplied ||
            _eventForceActuatorState.offsetForcesApplied !=
                    this->previousEventForceActuatorState.offsetForcesApplied ||
            _eventForceActuatorState.accelerationForcesApplied !=
                    this->previousEventForceActuatorState.accelerationForcesApplied ||
            _eventForceActuatorState.velocityForcesApplied !=
                    this->previousEventForceActuatorState.velocityForcesApplied ||
            _eventForceActuatorState.activeOpticForcesApplied !=
                    this->previousEventForceActuatorState.activeOpticForcesApplied ||
            _eventForceActuatorState.aberrationForcesApplied !=
                    this->previousEventForceActuatorState.aberrationForcesApplied ||
            _eventForceActuatorState.balanceForcesApplied !=
                    this->previousEventForceActuatorState.balanceForcesApplied ||
            _eventForceActuatorState.supportPercentage !=
                    this->previousEventForceActuatorState.supportPercentage;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventForceActuatorState.ilcState[i] !=
                                                   this->previousEventForceActuatorState.ilcState[i];
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
        _eventForceActuatorWarning.anyMajorFault = _eventForceActuatorWarning.anyMajorFault ||
                                                        _eventForceActuatorWarning.majorFault[i];
        _eventForceActuatorWarning.anyMinorFault = _eventForceActuatorWarning.anyMinorFault ||
                                                        _eventForceActuatorWarning.minorFault[i];
        _eventForceActuatorWarning.anyFaultOverride = _eventForceActuatorWarning.anyFaultOverride ||
                                                           _eventForceActuatorWarning.faultOverride[i];
        _eventForceActuatorWarning.anyMainCalibrationError =
                _eventForceActuatorWarning.anyMainCalibrationError ||
                _eventForceActuatorWarning.mainCalibrationError[i];
        _eventForceActuatorWarning.anyBackupCalibrationError =
                _eventForceActuatorWarning.anyBackupCalibrationError ||
                _eventForceActuatorWarning.backupCalibrationError[i];
        _eventForceActuatorWarning.anyMezzanineError =
                _eventForceActuatorWarning.anyMezzanineError ||
                _eventForceActuatorWarning.mezzanineError[i];
        _eventForceActuatorWarning.anyMezzanineBootloaderActive =
                _eventForceActuatorWarning.anyMezzanineBootloaderActive ||
                _eventForceActuatorWarning.mezzanineBootloaderActive[i];
        _eventForceActuatorWarning.anyUniqueIdCRCError =
                _eventForceActuatorWarning.anyUniqueIdCRCError ||
                _eventForceActuatorWarning.uniqueIdCRCError[i];
        _eventForceActuatorWarning.anyApplicationTypeMismatch =
                _eventForceActuatorWarning.anyApplicationTypeMismatch ||
                _eventForceActuatorWarning.applicationTypeMismatch[i];
        _eventForceActuatorWarning.anyApplicationMissing =
                _eventForceActuatorWarning.anyApplicationMissing ||
                _eventForceActuatorWarning.applicationMissing[i];
        _eventForceActuatorWarning.anyApplicationCRCMismatch =
                _eventForceActuatorWarning.anyApplicationCRCMismatch ||
                _eventForceActuatorWarning.applicationCRCMismatch[i];
        _eventForceActuatorWarning.anyOneWireMissing =
                _eventForceActuatorWarning.anyOneWireMissing ||
                _eventForceActuatorWarning.oneWireMissing[i];
        _eventForceActuatorWarning.anyOneWire1Mismatch =
                _eventForceActuatorWarning.anyOneWire1Mismatch ||
                _eventForceActuatorWarning.oneWire1Mismatch[i];
        _eventForceActuatorWarning.anyOneWire2Mismatch =
                _eventForceActuatorWarning.anyOneWire2Mismatch ||
                _eventForceActuatorWarning.oneWire2Mismatch[i];
        _eventForceActuatorWarning.anyWatchdogReset = _eventForceActuatorWarning.anyWatchdogReset ||
                                                           _eventForceActuatorWarning.watchdogReset[i];
        _eventForceActuatorWarning.anyBrownOut =
                _eventForceActuatorWarning.anyBrownOut || _eventForceActuatorWarning.brownOut[i];
        _eventForceActuatorWarning.anyEventTrapReset =
                _eventForceActuatorWarning.anyEventTrapReset ||
                _eventForceActuatorWarning.eventTrapReset[i];
        _eventForceActuatorWarning.anySSRPowerFault = _eventForceActuatorWarning.anySSRPowerFault ||
                                                           _eventForceActuatorWarning.ssrPowerFault[i];
        _eventForceActuatorWarning.anyAuxPowerFault = _eventForceActuatorWarning.anyAuxPowerFault ||
                                                           _eventForceActuatorWarning.auxPowerFault[i];
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
            _eventForceActuatorWarning.anyOneWireMissing ||
            _eventForceActuatorWarning.anyOneWire1Mismatch ||
            _eventForceActuatorWarning.anyOneWire2Mismatch ||
            _eventForceActuatorWarning.anyWatchdogReset || _eventForceActuatorWarning.anyBrownOut ||
            _eventForceActuatorWarning.anyEventTrapReset ||
            _eventForceActuatorWarning.anySSRPowerFault ||
            _eventForceActuatorWarning.anyAuxPowerFault ||
            _eventForceActuatorWarning.anyMezzaninePowerFault ||
            _eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault ||
            _eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault ||
            _eventForceActuatorWarning.anyMezzanineUniqueIdCRCError ||
            _eventForceActuatorWarning.anyMezzanineMainCalibrationError ||
            _eventForceActuatorWarning.anyMezzanineBackupCalibrationError ||
            _eventForceActuatorWarning.anyMezzanineEventTrapReset ||
            _eventForceActuatorWarning.anyMezzanineApplicationMissing ||
            _eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch ||
            _eventForceActuatorWarning.anyILCFault ||
            _eventForceActuatorWarning.anyBroadcastCounterWarning;
    _m1m3SAL->logEvent_forceActuatorWarning(&_eventForceActuatorWarning, 0);
    this->previousEventForceActuatorWarning = _eventForceActuatorWarning;
}

void M1M3SSPublisher::tryLogForceActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventForceActuatorWarning.majorFault[i] !=
                                 this->previousEventForceActuatorWarning.majorFault[i] ||
                         _eventForceActuatorWarning.minorFault[i] !=
                                 this->previousEventForceActuatorWarning.minorFault[i] ||
                         _eventForceActuatorWarning.faultOverride[i] !=
                                 this->previousEventForceActuatorWarning.faultOverride[i] ||
                         _eventForceActuatorWarning.mainCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.mainCalibrationError[i] ||
                         _eventForceActuatorWarning.backupCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.backupCalibrationError[i] ||
                         _eventForceActuatorWarning.mezzanineError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineError[i] ||
                         _eventForceActuatorWarning.mezzanineBootloaderActive[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineBootloaderActive[i] ||
                         _eventForceActuatorWarning.uniqueIdCRCError[i] !=
                                 this->previousEventForceActuatorWarning.uniqueIdCRCError[i] ||
                         _eventForceActuatorWarning.applicationTypeMismatch[i] !=
                                 this->previousEventForceActuatorWarning.applicationTypeMismatch[i] ||
                         _eventForceActuatorWarning.applicationMissing[i] !=
                                 this->previousEventForceActuatorWarning.applicationMissing[i] ||
                         _eventForceActuatorWarning.applicationCRCMismatch[i] !=
                                 this->previousEventForceActuatorWarning.applicationCRCMismatch[i] ||
                         _eventForceActuatorWarning.oneWireMissing[i] !=
                                 this->previousEventForceActuatorWarning.oneWireMissing[i] ||
                         _eventForceActuatorWarning.oneWire1Mismatch[i] !=
                                 this->previousEventForceActuatorWarning.oneWire1Mismatch[i] ||
                         _eventForceActuatorWarning.oneWire2Mismatch[i] !=
                                 this->previousEventForceActuatorWarning.oneWire2Mismatch[i] ||
                         _eventForceActuatorWarning.watchdogReset[i] !=
                                 this->previousEventForceActuatorWarning.watchdogReset[i] ||
                         _eventForceActuatorWarning.brownOut[i] !=
                                 this->previousEventForceActuatorWarning.brownOut[i] ||
                         _eventForceActuatorWarning.eventTrapReset[i] !=
                                 this->previousEventForceActuatorWarning.eventTrapReset[i] ||
                         _eventForceActuatorWarning.ssrPowerFault[i] !=
                                 this->previousEventForceActuatorWarning.ssrPowerFault[i] ||
                         _eventForceActuatorWarning.auxPowerFault[i] !=
                                 this->previousEventForceActuatorWarning.auxPowerFault[i] ||
                         _eventForceActuatorWarning.mezzaninePowerFault[i] !=
                                 this->previousEventForceActuatorWarning.mezzaninePowerFault[i] ||
                         _eventForceActuatorWarning.mezzanineCurrentAmp1Fault[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineCurrentAmp1Fault[i] ||
                         _eventForceActuatorWarning.mezzanineCurrentAmp2Fault[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineCurrentAmp2Fault[i] ||
                         _eventForceActuatorWarning.mezzanineUniqueIdCRCError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineUniqueIdCRCError[i] ||
                         _eventForceActuatorWarning.mezzanineMainCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineMainCalibrationError[i] ||
                         _eventForceActuatorWarning.mezzanineBackupCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineBackupCalibrationError[i] ||
                         _eventForceActuatorWarning.mezzanineEventTrapReset[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineEventTrapReset[i] ||
                         _eventForceActuatorWarning.mezzanineApplicationMissing[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineApplicationMissing[i] ||
                         _eventForceActuatorWarning.mezzanineApplicationCRCMismatch[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineApplicationCRCMismatch[i] ||
                         _eventForceActuatorWarning.ilcFault[i] !=
                                 this->previousEventForceActuatorWarning.ilcFault[i] ||
                         _eventForceActuatorWarning.broadcastCounterWarning[i] !=
                                 this->previousEventForceActuatorWarning.broadcastCounterWarning[i];
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
        _eventForceSetpointWarning.anySafetyLimitWarning =
                _eventForceSetpointWarning.anySafetyLimitWarning ||
                _eventForceSetpointWarning.safetyLimitWarning[i];
        _eventForceSetpointWarning.anyNearNeighborWarning =
                _eventForceSetpointWarning.anyNearNeighborWarning ||
                _eventForceSetpointWarning.nearNeighborWarning[i];
        _eventForceSetpointWarning.anyFarNeighborWarning =
                _eventForceSetpointWarning.anyFarNeighborWarning ||
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
        _eventForceSetpointWarning.anyStaticForceWarning =
                _eventForceSetpointWarning.anyStaticForceWarning ||
                _eventForceSetpointWarning.staticForceWarning[i];
        _eventForceSetpointWarning.anyAberrationForceWarning =
                _eventForceSetpointWarning.anyAberrationForceWarning ||
                _eventForceSetpointWarning.aberrationForceWarning[i];
        _eventForceSetpointWarning.anyOffsetForceWarning =
                _eventForceSetpointWarning.anyOffsetForceWarning ||
                _eventForceSetpointWarning.offsetForceWarning[i];
        _eventForceSetpointWarning.anyForceWarning = _eventForceSetpointWarning.anyForceWarning ||
                                                          _eventForceSetpointWarning.forceWarning[i];
    }
    _eventForceSetpointWarning.anyWarning =
            _eventForceSetpointWarning.anySafetyLimitWarning ||
            _eventForceSetpointWarning.xMomentWarning ||
            _eventForceSetpointWarning.yMomentWarning ||
            _eventForceSetpointWarning.zMomentWarning ||
            _eventForceSetpointWarning.anyNearNeighborWarning ||
            _eventForceSetpointWarning.magnitudeWarning ||
            _eventForceSetpointWarning.anyFarNeighborWarning ||
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
            _eventForceSetpointWarning.anyOffsetForceWarning ||
            _eventForceSetpointWarning.anyForceWarning;
    _m1m3SAL->logEvent_forceSetpointWarning(&_eventForceSetpointWarning, 0);
    this->previousEventForceSetpointWarning = _eventForceSetpointWarning;
}

void M1M3SSPublisher::tryLogForceSetpointWarning() {
    bool changeDetected = _eventForceSetpointWarning.xMomentWarning !=
                                  this->previousEventForceSetpointWarning.xMomentWarning ||
                          _eventForceSetpointWarning.yMomentWarning !=
                                  this->previousEventForceSetpointWarning.yMomentWarning ||
                          _eventForceSetpointWarning.zMomentWarning !=
                                  this->previousEventForceSetpointWarning.zMomentWarning ||
                          _eventForceSetpointWarning.magnitudeWarning !=
                                  this->previousEventForceSetpointWarning.magnitudeWarning ||
                          _eventForceSetpointWarning.activeOpticNetForceWarning !=
                                  this->previousEventForceSetpointWarning.activeOpticNetForceWarning ||
                          _eventForceSetpointWarning.aberrationNetForceWarning !=
                                  this->previousEventForceSetpointWarning.aberrationNetForceWarning;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventForceSetpointWarning.safetyLimitWarning[i] !=
                                 this->previousEventForceSetpointWarning.safetyLimitWarning[i] ||
                         _eventForceSetpointWarning.nearNeighborWarning[i] !=
                                 this->previousEventForceSetpointWarning.nearNeighborWarning[i] ||
                         _eventForceSetpointWarning.farNeighborWarning[i] !=
                                 this->previousEventForceSetpointWarning.farNeighborWarning[i] ||
                         _eventForceSetpointWarning.elevationForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.elevationForceWarning[i] ||
                         _eventForceSetpointWarning.azimuthForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.azimuthForceWarning[i] ||
                         _eventForceSetpointWarning.thermalForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.thermalForceWarning[i] ||
                         _eventForceSetpointWarning.balanceForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.balanceForceWarning[i] ||
                         _eventForceSetpointWarning.accelerationForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.accelerationForceWarning[i] ||
                         _eventForceSetpointWarning.velocityForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.velocityForceWarning[i] ||
                         _eventForceSetpointWarning.activeOpticForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.activeOpticForceWarning[i] ||
                         _eventForceSetpointWarning.staticForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.staticForceWarning[i] ||
                         _eventForceSetpointWarning.aberrationForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.aberrationForceWarning[i] ||
                         _eventForceSetpointWarning.offsetForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.offsetForceWarning[i] ||
                         _eventForceSetpointWarning.forceWarning[i] !=
                                 this->previousEventForceSetpointWarning.forceWarning[i];
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
            _eventGyroWarning.gcbTemperatureStatusWarning ||
            _eventGyroWarning.temperatureStatusWarning ||
            _eventGyroWarning.gcbDSPSPIFlashStatusWarning ||
            _eventGyroWarning.gcbFPGASPIFlashStatusWarning ||
            _eventGyroWarning.dspSPIFlashStatusWarning ||
            _eventGyroWarning.fpgaSPIFlashStatusWarning || _eventGyroWarning.gcb1_2VStatusWarning ||
            _eventGyroWarning.gcb3_3VStatusWarning || _eventGyroWarning.gcb5VStatusWarning ||
            _eventGyroWarning.v1_2StatusWarning || _eventGyroWarning.v3_3StatusWarning ||
            _eventGyroWarning.v5StatusWarning || _eventGyroWarning.gcbFPGAStatusWarning ||
            _eventGyroWarning.fpgaStatusWarning || _eventGyroWarning.hiSpeedSPORTStatusWarning ||
            _eventGyroWarning.auxSPORTStatusWarning ||
            _eventGyroWarning.sufficientSoftwareResourcesWarning ||
            _eventGyroWarning.gyroEOVoltsPositiveWarning ||
            _eventGyroWarning.gyroEOVoltsNegativeWarning || _eventGyroWarning.gyroXVoltsWarning ||
            _eventGyroWarning.gyroYVoltsWarning || _eventGyroWarning.gyroZVoltsWarning ||
            _eventGyroWarning.gcbADCCommsWarning || _eventGyroWarning.mSYNCExternalTimingWarning;
    _m1m3SAL->logEvent_gyroWarning(&_eventGyroWarning, 0);
    this->previousEventGyroWarning = _eventGyroWarning;
}

void M1M3SSPublisher::tryLogGyroWarning() {
    if (_eventGyroWarning.gyroXStatusWarning != this->previousEventGyroWarning.gyroXStatusWarning ||
        _eventGyroWarning.gyroYStatusWarning != this->previousEventGyroWarning.gyroYStatusWarning ||
        _eventGyroWarning.gyroZStatusWarning != this->previousEventGyroWarning.gyroZStatusWarning ||
        _eventGyroWarning.sequenceNumberWarning !=
                this->previousEventGyroWarning.sequenceNumberWarning ||
        _eventGyroWarning.crcMismatchWarning != this->previousEventGyroWarning.crcMismatchWarning ||
        _eventGyroWarning.invalidLengthWarning != this->previousEventGyroWarning.invalidLengthWarning ||
        _eventGyroWarning.invalidHeaderWarning != this->previousEventGyroWarning.invalidHeaderWarning ||
        _eventGyroWarning.incompleteFrameWarning !=
                this->previousEventGyroWarning.incompleteFrameWarning ||
        _eventGyroWarning.gyroXSLDWarning != this->previousEventGyroWarning.gyroXSLDWarning ||
        _eventGyroWarning.gyroXMODDACWarning != this->previousEventGyroWarning.gyroXMODDACWarning ||
        _eventGyroWarning.gyroXPhaseWarning != this->previousEventGyroWarning.gyroXPhaseWarning ||
        _eventGyroWarning.gyroXFlashWarning != this->previousEventGyroWarning.gyroXFlashWarning ||
        _eventGyroWarning.gyroYSLDWarning != this->previousEventGyroWarning.gyroYSLDWarning ||
        _eventGyroWarning.gyroYMODDACWarning != this->previousEventGyroWarning.gyroYMODDACWarning ||
        _eventGyroWarning.gyroYPhaseWarning != this->previousEventGyroWarning.gyroYPhaseWarning ||
        _eventGyroWarning.gyroYFlashWarning != this->previousEventGyroWarning.gyroYFlashWarning ||
        _eventGyroWarning.gyroZSLDWarning != this->previousEventGyroWarning.gyroZSLDWarning ||
        _eventGyroWarning.gyroZMODDACWarning != this->previousEventGyroWarning.gyroZMODDACWarning ||
        _eventGyroWarning.gyroZPhaseWarning != this->previousEventGyroWarning.gyroZPhaseWarning ||
        _eventGyroWarning.gyroZFlashWarning != this->previousEventGyroWarning.gyroZFlashWarning ||
        _eventGyroWarning.gyroXSLDTemperatureStatusWarning !=
                this->previousEventGyroWarning.gyroXSLDTemperatureStatusWarning ||
        _eventGyroWarning.gyroYSLDTemperatureStatusWarning !=
                this->previousEventGyroWarning.gyroYSLDTemperatureStatusWarning ||
        _eventGyroWarning.gyroZSLDTemperatureStatusWarning !=
                this->previousEventGyroWarning.gyroZSLDTemperatureStatusWarning ||
        _eventGyroWarning.gcbTemperatureStatusWarning !=
                this->previousEventGyroWarning.gcbTemperatureStatusWarning ||
        _eventGyroWarning.temperatureStatusWarning !=
                this->previousEventGyroWarning.temperatureStatusWarning ||
        _eventGyroWarning.gcbDSPSPIFlashStatusWarning !=
                this->previousEventGyroWarning.gcbDSPSPIFlashStatusWarning ||
        _eventGyroWarning.gcbFPGASPIFlashStatusWarning !=
                this->previousEventGyroWarning.gcbFPGASPIFlashStatusWarning ||
        _eventGyroWarning.dspSPIFlashStatusWarning !=
                this->previousEventGyroWarning.dspSPIFlashStatusWarning ||
        _eventGyroWarning.fpgaSPIFlashStatusWarning !=
                this->previousEventGyroWarning.fpgaSPIFlashStatusWarning ||
        _eventGyroWarning.gcb1_2VStatusWarning != this->previousEventGyroWarning.gcb1_2VStatusWarning ||
        _eventGyroWarning.gcb3_3VStatusWarning != this->previousEventGyroWarning.gcb3_3VStatusWarning ||
        _eventGyroWarning.gcb5VStatusWarning != this->previousEventGyroWarning.gcb5VStatusWarning ||
        _eventGyroWarning.v1_2StatusWarning != this->previousEventGyroWarning.v1_2StatusWarning ||
        _eventGyroWarning.v3_3StatusWarning != this->previousEventGyroWarning.v3_3StatusWarning ||
        _eventGyroWarning.v5StatusWarning != this->previousEventGyroWarning.v5StatusWarning ||
        _eventGyroWarning.gcbFPGAStatusWarning != this->previousEventGyroWarning.gcbFPGAStatusWarning ||
        _eventGyroWarning.fpgaStatusWarning != this->previousEventGyroWarning.fpgaStatusWarning ||
        _eventGyroWarning.hiSpeedSPORTStatusWarning !=
                this->previousEventGyroWarning.hiSpeedSPORTStatusWarning ||
        _eventGyroWarning.auxSPORTStatusWarning !=
                this->previousEventGyroWarning.auxSPORTStatusWarning ||
        _eventGyroWarning.sufficientSoftwareResourcesWarning !=
                this->previousEventGyroWarning.sufficientSoftwareResourcesWarning ||
        _eventGyroWarning.gyroEOVoltsPositiveWarning !=
                this->previousEventGyroWarning.gyroEOVoltsPositiveWarning ||
        _eventGyroWarning.gyroEOVoltsNegativeWarning !=
                this->previousEventGyroWarning.gyroEOVoltsNegativeWarning ||
        _eventGyroWarning.gyroXVoltsWarning != this->previousEventGyroWarning.gyroXVoltsWarning ||
        _eventGyroWarning.gyroYVoltsWarning != this->previousEventGyroWarning.gyroYVoltsWarning ||
        _eventGyroWarning.gyroZVoltsWarning != this->previousEventGyroWarning.gyroZVoltsWarning ||
        _eventGyroWarning.gcbADCCommsWarning != this->previousEventGyroWarning.gcbADCCommsWarning ||
        _eventGyroWarning.mSYNCExternalTimingWarning !=
                this->previousEventGyroWarning.mSYNCExternalTimingWarning) {
        this->logGyroWarning();
    }
}

void M1M3SSPublisher::logHardpointActuatorInfo() {
    _m1m3SAL->logEvent_hardpointActuatorInfo(&_eventHardpointActuatorInfo, 0);
    this->previousEventHardpointActuatorInfo = _eventHardpointActuatorInfo;
}

void M1M3SSPublisher::tryLogHardpointActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorInfo.referenceId[i] !=
                                 this->previousEventHardpointActuatorInfo.referenceId[i] ||
                         _eventHardpointActuatorInfo.referencePosition[i] !=
                                 this->previousEventHardpointActuatorInfo.referencePosition[i] ||
                         _eventHardpointActuatorInfo.modbusSubnet[i] !=
                                 this->previousEventHardpointActuatorInfo.modbusSubnet[i] ||
                         _eventHardpointActuatorInfo.modbusAddress[i] !=
                                 this->previousEventHardpointActuatorInfo.modbusAddress[i] ||
                         _eventHardpointActuatorInfo.xPosition[i] !=
                                 this->previousEventHardpointActuatorInfo.xPosition[i] ||
                         _eventHardpointActuatorInfo.yPosition[i] !=
                                 this->previousEventHardpointActuatorInfo.yPosition[i] ||
                         _eventHardpointActuatorInfo.zPosition[i] !=
                                 this->previousEventHardpointActuatorInfo.zPosition[i] ||
                         _eventHardpointActuatorInfo.ilcUniqueId[i] !=
                                 this->previousEventHardpointActuatorInfo.ilcUniqueId[i] ||
                         _eventHardpointActuatorInfo.ilcApplicationType[i] !=
                                 this->previousEventHardpointActuatorInfo.ilcApplicationType[i] ||
                         _eventHardpointActuatorInfo.networkNodeType[i] !=
                                 this->previousEventHardpointActuatorInfo.networkNodeType[i] ||
                         _eventHardpointActuatorInfo.ilcSelectedOptions[i] !=
                                 this->previousEventHardpointActuatorInfo.ilcSelectedOptions[i] ||
                         _eventHardpointActuatorInfo.networkNodeOptions[i] !=
                                 this->previousEventHardpointActuatorInfo.networkNodeOptions[i] ||
                         _eventHardpointActuatorInfo.majorRevision[i] !=
                                 this->previousEventHardpointActuatorInfo.majorRevision[i] ||
                         _eventHardpointActuatorInfo.minorRevision[i] !=
                                 this->previousEventHardpointActuatorInfo.minorRevision[i] ||
                         _eventHardpointActuatorInfo.adcScanRate[i] !=
                                 this->previousEventHardpointActuatorInfo.adcScanRate[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellCoefficient[i] !=
                                 this->previousEventHardpointActuatorInfo.mainLoadCellCoefficient[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellOffset[i] !=
                                 this->previousEventHardpointActuatorInfo.mainLoadCellOffset[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellSensitivity[i] !=
                                 this->previousEventHardpointActuatorInfo.mainLoadCellSensitivity[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellCoefficient[i] !=
                                 this->previousEventHardpointActuatorInfo.backupLoadCellCoefficient[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellOffset[i] !=
                                 this->previousEventHardpointActuatorInfo.backupLoadCellOffset[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellSensitivity[i] !=
                                 this->previousEventHardpointActuatorInfo.backupLoadCellSensitivity[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorInfo();
    }
}

void M1M3SSPublisher::logHardpointActuatorState() {
    _m1m3SAL->logEvent_hardpointActuatorState(&_eventHardpointActuatorState, 0);
    this->previousEventHardpointActuatorState = _eventHardpointActuatorState;
}

void M1M3SSPublisher::tryLogHardpointActuatorState() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorState.ilcState[i] !=
                                 this->previousEventHardpointActuatorState.ilcState[i] ||
                         _eventHardpointActuatorState.motionState[i] !=
                                 this->previousEventHardpointActuatorState.motionState[i];
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
                _eventHardpointActuatorWarning.anyMajorFault ||
                _eventHardpointActuatorWarning.majorFault[i];
        _eventHardpointActuatorWarning.anyMinorFault =
                _eventHardpointActuatorWarning.anyMinorFault ||
                _eventHardpointActuatorWarning.minorFault[i];
        _eventHardpointActuatorWarning.anyFaultOverride =
                _eventHardpointActuatorWarning.anyFaultOverride ||
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
        _eventHardpointActuatorWarning.anyOneWireMissing =
                _eventHardpointActuatorWarning.anyOneWireMissing ||
                _eventHardpointActuatorWarning.oneWireMissing[i];
        _eventHardpointActuatorWarning.anyOneWire1Mismatch =
                _eventHardpointActuatorWarning.anyOneWire1Mismatch ||
                _eventHardpointActuatorWarning.oneWire1Mismatch[i];
        _eventHardpointActuatorWarning.anyOneWire2Mismatch =
                _eventHardpointActuatorWarning.anyOneWire2Mismatch ||
                _eventHardpointActuatorWarning.oneWire2Mismatch[i];
        _eventHardpointActuatorWarning.anyWatchdogReset =
                _eventHardpointActuatorWarning.anyWatchdogReset ||
                _eventHardpointActuatorWarning.watchdogReset[i];
        _eventHardpointActuatorWarning.anyBrownOut = _eventHardpointActuatorWarning.anyBrownOut ||
                                                          _eventHardpointActuatorWarning.brownOut[i];
        _eventHardpointActuatorWarning.anyEventTrapReset =
                _eventHardpointActuatorWarning.anyEventTrapReset ||
                _eventHardpointActuatorWarning.eventTrapReset[i];
        _eventHardpointActuatorWarning.anyMotorDriverFault =
                _eventHardpointActuatorWarning.anyMotorDriverFault ||
                _eventHardpointActuatorWarning.motorDriverFault[i];
        _eventHardpointActuatorWarning.anySSRPowerFault =
                _eventHardpointActuatorWarning.anySSRPowerFault ||
                _eventHardpointActuatorWarning.ssrPowerFault[i];
        _eventHardpointActuatorWarning.anyAuxPowerFault =
                _eventHardpointActuatorWarning.anyAuxPowerFault ||
                _eventHardpointActuatorWarning.auxPowerFault[i];
        _eventHardpointActuatorWarning.anySMCPowerFault =
                _eventHardpointActuatorWarning.anySMCPowerFault ||
                _eventHardpointActuatorWarning.smcPowerFault[i];
        _eventHardpointActuatorWarning.anyILCFault = _eventHardpointActuatorWarning.anyILCFault ||
                                                          _eventHardpointActuatorWarning.ilcFault[i];
        _eventHardpointActuatorWarning.anyBroadcastCounterWarning =
                _eventHardpointActuatorWarning.anyBroadcastCounterWarning ||
                _eventHardpointActuatorWarning.broadcastCounterWarning[i];
    }
    _eventHardpointActuatorWarning.anyWarning =
            _eventHardpointActuatorWarning.anyMajorFault ||
            _eventHardpointActuatorWarning.anyMinorFault ||
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
            _eventHardpointActuatorWarning.anyWatchdogReset ||
            _eventHardpointActuatorWarning.anyBrownOut ||
            _eventHardpointActuatorWarning.anyEventTrapReset ||
            _eventHardpointActuatorWarning.anyMotorDriverFault ||
            _eventHardpointActuatorWarning.anySSRPowerFault ||
            _eventHardpointActuatorWarning.anyAuxPowerFault ||
            _eventHardpointActuatorWarning.anySMCPowerFault ||
            _eventHardpointActuatorWarning.anyILCFault ||
            _eventHardpointActuatorWarning.anyBroadcastCounterWarning;
    _m1m3SAL->logEvent_hardpointActuatorWarning(&_eventHardpointActuatorWarning, 0);
    this->previousEventHardpointActuatorWarning = _eventHardpointActuatorWarning;
}

void M1M3SSPublisher::tryLogHardpointActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorWarning.majorFault[i] !=
                                 this->previousEventHardpointActuatorWarning.majorFault[i] ||
                         _eventHardpointActuatorWarning.minorFault[i] !=
                                 this->previousEventHardpointActuatorWarning.minorFault[i] ||
                         _eventHardpointActuatorWarning.faultOverride[i] !=
                                 this->previousEventHardpointActuatorWarning.faultOverride[i] ||
                         _eventHardpointActuatorWarning.mainCalibrationError[i] !=
                                 this->previousEventHardpointActuatorWarning.mainCalibrationError[i] ||
                         _eventHardpointActuatorWarning.backupCalibrationError[i] !=
                                 this->previousEventHardpointActuatorWarning.backupCalibrationError[i] ||
                         _eventHardpointActuatorWarning.limitSwitch1Operated[i] !=
                                 this->previousEventHardpointActuatorWarning.limitSwitch1Operated[i] ||
                         _eventHardpointActuatorWarning.limitSwitch2Operated[i] !=
                                 this->previousEventHardpointActuatorWarning.limitSwitch2Operated[i] ||
                         _eventHardpointActuatorWarning.uniqueIdCRCError[i] !=
                                 this->previousEventHardpointActuatorWarning.uniqueIdCRCError[i] ||
                         _eventHardpointActuatorWarning.applicationTypeMismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.applicationTypeMismatch[i] ||
                         _eventHardpointActuatorWarning.applicationMissing[i] !=
                                 this->previousEventHardpointActuatorWarning.applicationMissing[i] ||
                         _eventHardpointActuatorWarning.applicationCRCMismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.applicationCRCMismatch[i] ||
                         _eventHardpointActuatorWarning.oneWireMissing[i] !=
                                 this->previousEventHardpointActuatorWarning.oneWireMissing[i] ||
                         _eventHardpointActuatorWarning.oneWire1Mismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.oneWire1Mismatch[i] ||
                         _eventHardpointActuatorWarning.oneWire2Mismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.oneWire2Mismatch[i] ||
                         _eventHardpointActuatorWarning.watchdogReset[i] !=
                                 this->previousEventHardpointActuatorWarning.watchdogReset[i] ||
                         _eventHardpointActuatorWarning.brownOut[i] !=
                                 this->previousEventHardpointActuatorWarning.brownOut[i] ||
                         _eventHardpointActuatorWarning.eventTrapReset[i] !=
                                 this->previousEventHardpointActuatorWarning.eventTrapReset[i] ||
                         _eventHardpointActuatorWarning.motorDriverFault[i] !=
                                 this->previousEventHardpointActuatorWarning.motorDriverFault[i] ||
                         _eventHardpointActuatorWarning.ssrPowerFault[i] !=
                                 this->previousEventHardpointActuatorWarning.ssrPowerFault[i] ||
                         _eventHardpointActuatorWarning.auxPowerFault[i] !=
                                 this->previousEventHardpointActuatorWarning.auxPowerFault[i] ||
                         _eventHardpointActuatorWarning.smcPowerFault[i] !=
                                 this->previousEventHardpointActuatorWarning.smcPowerFault[i] ||
                         _eventHardpointActuatorWarning.ilcFault[i] !=
                                 this->previousEventHardpointActuatorWarning.ilcFault[i] ||
                         _eventHardpointActuatorWarning.broadcastCounterWarning[i] !=
                                 this->previousEventHardpointActuatorWarning.broadcastCounterWarning[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorWarning();
    }
}

void M1M3SSPublisher::logHardpointMonitorInfo() {
    _m1m3SAL->logEvent_hardpointMonitorInfo(&_eventHardpointMonitorInfo, 0);
    this->previousEventHardpointMonitorInfo = _eventHardpointMonitorInfo;
}

void M1M3SSPublisher::tryLogHardpointMonitorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointMonitorInfo.referenceId[i] !=
                                 this->previousEventHardpointMonitorInfo.referenceId[i] ||
                         _eventHardpointMonitorInfo.modbusSubnet[i] !=
                                 this->previousEventHardpointMonitorInfo.modbusSubnet[i] ||
                         _eventHardpointMonitorInfo.modbusAddress[i] !=
                                 this->previousEventHardpointMonitorInfo.modbusAddress[i] ||
                         _eventHardpointMonitorInfo.ilcUniqueId[i] !=
                                 this->previousEventHardpointMonitorInfo.ilcUniqueId[i] ||
                         _eventHardpointMonitorInfo.ilcApplicationType[i] !=
                                 this->previousEventHardpointMonitorInfo.ilcApplicationType[i] ||
                         _eventHardpointMonitorInfo.networkNodeType[i] !=
                                 this->previousEventHardpointMonitorInfo.networkNodeType[i] ||
                         _eventHardpointMonitorInfo.majorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.majorRevision[i] ||
                         _eventHardpointMonitorInfo.minorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.minorRevision[i] ||
                         _eventHardpointMonitorInfo.mezzanineUniqueId[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineUniqueId[i] ||
                         _eventHardpointMonitorInfo.mezzanineFirmwareType[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineFirmwareType[i] ||
                         _eventHardpointMonitorInfo.mezzanineMajorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineMajorRevision[i] ||
                         _eventHardpointMonitorInfo.mezzanineMinorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorInfo();
    }
}

void M1M3SSPublisher::logHardpointMonitorState() {
    _m1m3SAL->logEvent_hardpointMonitorState(&_eventHardpointMonitorState, 0);
    this->previousEventHardpointMonitorState = _eventHardpointMonitorState;
}

void M1M3SSPublisher::tryLogHardpointMonitorState() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventHardpointMonitorState.ilcState[i] !=
                                                   this->previousEventHardpointMonitorState.ilcState[i];
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
        _eventHardpointMonitorWarning.anyMajorFault = _eventHardpointMonitorWarning.anyMajorFault ||
                                                           _eventHardpointMonitorWarning.majorFault[i];
        _eventHardpointMonitorWarning.anyMinorFault = _eventHardpointMonitorWarning.anyMinorFault ||
                                                           _eventHardpointMonitorWarning.minorFault[i];
        _eventHardpointMonitorWarning.anyFaultOverride =
                _eventHardpointMonitorWarning.anyFaultOverride ||
                _eventHardpointMonitorWarning.faultOverride[i];
        _eventHardpointMonitorWarning.anyInstrumentError =
                _eventHardpointMonitorWarning.anyInstrumentError ||
                _eventHardpointMonitorWarning.instrumentError[i];
        _eventHardpointMonitorWarning.anyMezzanineError =
                _eventHardpointMonitorWarning.anyMezzanineError ||
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
        _eventHardpointMonitorWarning.anyOneWireMissing =
                _eventHardpointMonitorWarning.anyOneWireMissing ||
                _eventHardpointMonitorWarning.oneWireMissing[i];
        _eventHardpointMonitorWarning.anyOneWire1Mismatch =
                _eventHardpointMonitorWarning.anyOneWire1Mismatch ||
                _eventHardpointMonitorWarning.oneWire1Mismatch[i];
        _eventHardpointMonitorWarning.anyOneWire2Mismatch =
                _eventHardpointMonitorWarning.anyOneWire2Mismatch ||
                _eventHardpointMonitorWarning.oneWire2Mismatch[i];
        _eventHardpointMonitorWarning.anyWatchdogReset =
                _eventHardpointMonitorWarning.anyWatchdogReset ||
                _eventHardpointMonitorWarning.watchdogReset[i];
        _eventHardpointMonitorWarning.anyBrownOut = _eventHardpointMonitorWarning.anyBrownOut ||
                                                         _eventHardpointMonitorWarning.brownOut[i];
        _eventHardpointMonitorWarning.anyEventTrapReset =
                _eventHardpointMonitorWarning.anyEventTrapReset ||
                _eventHardpointMonitorWarning.eventTrapReset[i];
        _eventHardpointMonitorWarning.anySSRPowerFault =
                _eventHardpointMonitorWarning.anySSRPowerFault ||
                _eventHardpointMonitorWarning.ssrPowerFault[i];
        _eventHardpointMonitorWarning.anyAuxPowerFault =
                _eventHardpointMonitorWarning.anyAuxPowerFault ||
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
            _eventHardpointMonitorWarning.anyMajorFault ||
            _eventHardpointMonitorWarning.anyMinorFault ||
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
            _eventHardpointMonitorWarning.anyWatchdogReset ||
            _eventHardpointMonitorWarning.anyBrownOut ||
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
    this->previousEventHardpointMonitorWarning = _eventHardpointMonitorWarning;
}

void M1M3SSPublisher::tryLogHardpointMonitorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                _eventHardpointMonitorWarning.majorFault[i] !=
                        this->previousEventHardpointMonitorWarning.majorFault[i] ||
                _eventHardpointMonitorWarning.minorFault[i] !=
                        this->previousEventHardpointMonitorWarning.minorFault[i] ||
                _eventHardpointMonitorWarning.faultOverride[i] !=
                        this->previousEventHardpointMonitorWarning.faultOverride[i] ||
                _eventHardpointMonitorWarning.instrumentError[i] !=
                        this->previousEventHardpointMonitorWarning.instrumentError[i] ||
                _eventHardpointMonitorWarning.mezzanineError[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineError[i] ||
                _eventHardpointMonitorWarning.mezzanineBootloaderActive[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineBootloaderActive[i] ||
                _eventHardpointMonitorWarning.uniqueIdCRCError[i] !=
                        this->previousEventHardpointMonitorWarning.uniqueIdCRCError[i] ||
                _eventHardpointMonitorWarning.applicationTypeMismatch[i] !=
                        this->previousEventHardpointMonitorWarning.applicationTypeMismatch[i] ||
                _eventHardpointMonitorWarning.applicationMissing[i] !=
                        this->previousEventHardpointMonitorWarning.applicationMissing[i] ||
                _eventHardpointMonitorWarning.applicationCRCMismatch[i] !=
                        this->previousEventHardpointMonitorWarning.applicationCRCMismatch[i] ||
                _eventHardpointMonitorWarning.oneWireMissing[i] !=
                        this->previousEventHardpointMonitorWarning.oneWireMissing[i] ||
                _eventHardpointMonitorWarning.oneWire1Mismatch[i] !=
                        this->previousEventHardpointMonitorWarning.oneWire1Mismatch[i] ||
                _eventHardpointMonitorWarning.oneWire2Mismatch[i] !=
                        this->previousEventHardpointMonitorWarning.oneWire2Mismatch[i] ||
                _eventHardpointMonitorWarning.watchdogReset[i] !=
                        this->previousEventHardpointMonitorWarning.watchdogReset[i] ||
                _eventHardpointMonitorWarning.brownOut[i] !=
                        this->previousEventHardpointMonitorWarning.brownOut[i] ||
                _eventHardpointMonitorWarning.eventTrapReset[i] !=
                        this->previousEventHardpointMonitorWarning.eventTrapReset[i] ||
                _eventHardpointMonitorWarning.ssrPowerFault[i] !=
                        this->previousEventHardpointMonitorWarning.ssrPowerFault[i] ||
                _eventHardpointMonitorWarning.auxPowerFault[i] !=
                        this->previousEventHardpointMonitorWarning.auxPowerFault[i] ||
                _eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] ||
                _eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] ||
                _eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] ||
                _eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] ||
                _eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] ||
                _eventHardpointMonitorWarning.mezzanineEventTrapReset[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineEventTrapReset[i] ||
                _eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] ||
                _eventHardpointMonitorWarning.mezzanineApplicationMissing[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineApplicationMissing[i] ||
                _eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorWarning();
    }
}

void M1M3SSPublisher::logILCWarning() {
    _eventILCWarning.anyWarning =
            _eventILCWarning.responseTimeout || _eventILCWarning.invalidCRC ||
            _eventILCWarning.illegalFunction || _eventILCWarning.illegalDataValue ||
            _eventILCWarning.invalidLength || _eventILCWarning.unknownSubnet ||
            _eventILCWarning.unknownAddress || _eventILCWarning.unknownFunction ||
            _eventILCWarning.unknownProblem;
    _m1m3SAL->logEvent_ilcWarning(&_eventILCWarning, 0);
    this->previousEventILCWarning = _eventILCWarning;
}

void M1M3SSPublisher::tryLogILCWarning() {
    if (_eventILCWarning.actuatorId != this->previousEventILCWarning.actuatorId ||
        _eventILCWarning.responseTimeout != this->previousEventILCWarning.responseTimeout ||
        _eventILCWarning.invalidCRC != this->previousEventILCWarning.invalidCRC ||
        _eventILCWarning.illegalFunction != this->previousEventILCWarning.illegalFunction ||
        _eventILCWarning.illegalDataValue != this->previousEventILCWarning.illegalDataValue ||
        _eventILCWarning.invalidLength != this->previousEventILCWarning.invalidLength ||
        _eventILCWarning.unknownSubnet != this->previousEventILCWarning.unknownSubnet ||
        _eventILCWarning.unknownAddress != this->previousEventILCWarning.unknownAddress ||
        _eventILCWarning.unknownFunction != this->previousEventILCWarning.unknownFunction ||
        _eventILCWarning.unknownProblem != this->previousEventILCWarning.unknownProblem) {
        this->logILCWarning();
    }
}

void M1M3SSPublisher::logInclinometerSensorWarning() {
    _eventInclinometerSensorWarning.anyWarning =
            _eventInclinometerSensorWarning.sensorReportsIllegalFunction ||
            _eventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
            _eventInclinometerSensorWarning.responseTimeout ||
            _eventInclinometerSensorWarning.invalidCRC ||
            _eventInclinometerSensorWarning.invalidLength ||
            _eventInclinometerSensorWarning.unknownAddress ||
            _eventInclinometerSensorWarning.unknownFunction ||
            _eventInclinometerSensorWarning.unknownProblem;
    _m1m3SAL->logEvent_inclinometerSensorWarning(&_eventInclinometerSensorWarning, 0);
    this->previousEventInclinometerSensorWarning = _eventInclinometerSensorWarning;
}

void M1M3SSPublisher::tryLogInclinometerSensorWarning() {
    if (_eventInclinometerSensorWarning.sensorReportsIllegalFunction !=
                this->previousEventInclinometerSensorWarning.sensorReportsIllegalFunction ||
        _eventInclinometerSensorWarning.sensorReportsIllegalDataAddress !=
                this->previousEventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
        _eventInclinometerSensorWarning.responseTimeout !=
                this->previousEventInclinometerSensorWarning.responseTimeout ||
        _eventInclinometerSensorWarning.invalidCRC !=
                this->previousEventInclinometerSensorWarning.invalidCRC ||
        _eventInclinometerSensorWarning.invalidLength !=
                this->previousEventInclinometerSensorWarning.invalidLength ||
        _eventInclinometerSensorWarning.unknownAddress !=
                this->previousEventInclinometerSensorWarning.unknownAddress ||
        _eventInclinometerSensorWarning.unknownFunction !=
                this->previousEventInclinometerSensorWarning.unknownFunction ||
        _eventInclinometerSensorWarning.unknownProblem !=
                this->previousEventInclinometerSensorWarning.unknownProblem) {
        this->logInclinometerSensorWarning();
    }
}

void M1M3SSPublisher::logInterlockStatus() {
    _m1m3SAL->logEvent_interlockStatus(&_eventInterlockStatus, 0);
    this->previousEventInterlockStatus = _eventInterlockStatus;
}

void M1M3SSPublisher::tryLogInterlockStatus() {
    if (_eventInterlockStatus.heartbeatCommandedState !=
                this->previousEventInterlockStatus.heartbeatCommandedState ||
        _eventInterlockStatus.heartbeatOutputState !=
                this->previousEventInterlockStatus.heartbeatOutputState) {
        this->logInterlockStatus();
    }
}

void M1M3SSPublisher::logInterlockWarning() {
    _eventInterlockWarning.anyWarning =
            _eventInterlockWarning.heartbeatStateOutputMismatch ||
            _eventInterlockWarning.auxPowerNetworksOff ||
            _eventInterlockWarning.thermalEquipmentOff || _eventInterlockWarning.airSupplyOff ||
            _eventInterlockWarning.cabinetDoorOpen || _eventInterlockWarning.tmaMotionStop ||
            _eventInterlockWarning.gisHeartbeatLost;
    _m1m3SAL->logEvent_interlockWarning(&_eventInterlockWarning, 0);
    this->previousEventInterlockWarning = _eventInterlockWarning;
}

void M1M3SSPublisher::tryLogInterlockWarning() {
    if (_eventInterlockWarning.heartbeatStateOutputMismatch !=
                this->previousEventInterlockWarning.heartbeatStateOutputMismatch ||
        _eventInterlockWarning.auxPowerNetworksOff !=
                this->previousEventInterlockWarning.auxPowerNetworksOff ||
        _eventInterlockWarning.thermalEquipmentOff !=
                this->previousEventInterlockWarning.thermalEquipmentOff ||
        _eventInterlockWarning.airSupplyOff != this->previousEventInterlockWarning.airSupplyOff ||
        _eventInterlockWarning.cabinetDoorOpen != this->previousEventInterlockWarning.cabinetDoorOpen ||
        _eventInterlockWarning.tmaMotionStop != this->previousEventInterlockWarning.tmaMotionStop ||
        _eventInterlockWarning.gisHeartbeatLost !=
                this->previousEventInterlockWarning.gisHeartbeatLost) {
        this->logInterlockWarning();
    }
}

void M1M3SSPublisher::logModbusResponse() {
    _m1m3SAL->logEvent_modbusResponse(&_eventModbusResponse, 0);
    this->previousEventModbusResponse = _eventModbusResponse;
}

void M1M3SSPublisher::tryLogModbusResponse() {
    if (_eventModbusResponse.responseValid != this->previousEventModbusResponse.responseValid ||
        _eventModbusResponse.address != this->previousEventModbusResponse.address ||
        _eventModbusResponse.functionCode != this->previousEventModbusResponse.functionCode ||
        _eventModbusResponse.crc != this->previousEventModbusResponse.crc ||
        _eventModbusResponse.dataLength != this->previousEventModbusResponse.dataLength) {
        this->logModbusResponse();
    }
}

void M1M3SSPublisher::logPIDInfo() {
    _m1m3SAL->logEvent_pidInfo(&_eventPIDInfo, 0);
    this->previousEventPIDInfo = _eventPIDInfo;
}

void M1M3SSPublisher::tryLogPIDInfo() {
    bool changeDetected = false;
    for (int i = 0; i < 6 && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventPIDInfo.timestep[i] != this->previousEventPIDInfo.timestep[i] ||
                         _eventPIDInfo.p[i] != this->previousEventPIDInfo.p[i] ||
                         _eventPIDInfo.i[i] != this->previousEventPIDInfo.i[i] ||
                         _eventPIDInfo.d[i] != this->previousEventPIDInfo.d[i] ||
                         _eventPIDInfo.n[i] != this->previousEventPIDInfo.n[i] ||
                         _eventPIDInfo.calculatedA[i] != this->previousEventPIDInfo.calculatedA[i] ||
                         _eventPIDInfo.calculatedB[i] != this->previousEventPIDInfo.calculatedB[i] ||
                         _eventPIDInfo.calculatedC[i] != this->previousEventPIDInfo.calculatedC[i] ||
                         _eventPIDInfo.calculatedD[i] != this->previousEventPIDInfo.calculatedD[i] ||
                         _eventPIDInfo.calculatedE[i] != this->previousEventPIDInfo.calculatedE[i];
    }
    if (changeDetected) {
        this->logPIDInfo();
    }
}

void M1M3SSPublisher::logPowerStatus() {
    _m1m3SAL->logEvent_powerStatus(&_eventPowerStatus, 0);
    this->previousEventPowerStatus = _eventPowerStatus;
}

void M1M3SSPublisher::tryLogPowerStatus() {
    if (_eventPowerStatus.powerNetworkACommandedOn !=
                this->previousEventPowerStatus.powerNetworkACommandedOn ||
        _eventPowerStatus.powerNetworkAOutputOn !=
                this->previousEventPowerStatus.powerNetworkAOutputOn ||
        _eventPowerStatus.powerNetworkBCommandedOn !=
                this->previousEventPowerStatus.powerNetworkBCommandedOn ||
        _eventPowerStatus.powerNetworkBOutputOn !=
                this->previousEventPowerStatus.powerNetworkBOutputOn ||
        _eventPowerStatus.powerNetworkCCommandedOn !=
                this->previousEventPowerStatus.powerNetworkCCommandedOn ||
        _eventPowerStatus.powerNetworkCOutputOn !=
                this->previousEventPowerStatus.powerNetworkCOutputOn ||
        _eventPowerStatus.powerNetworkDCommandedOn !=
                this->previousEventPowerStatus.powerNetworkDCommandedOn ||
        _eventPowerStatus.powerNetworkDOutputOn !=
                this->previousEventPowerStatus.powerNetworkDOutputOn ||
        _eventPowerStatus.auxPowerNetworkACommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkACommandedOn ||
        _eventPowerStatus.auxPowerNetworkAOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkAOutputOn ||
        _eventPowerStatus.auxPowerNetworkBCommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkBCommandedOn ||
        _eventPowerStatus.auxPowerNetworkBOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkBOutputOn ||
        _eventPowerStatus.auxPowerNetworkCCommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkCCommandedOn ||
        _eventPowerStatus.auxPowerNetworkCOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkCOutputOn ||
        _eventPowerStatus.auxPowerNetworkDCommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkDCommandedOn ||
        _eventPowerStatus.auxPowerNetworkDOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkDOutputOn) {
        this->logPowerStatus();
    }
}

void M1M3SSPublisher::logPowerSupplyStatus() {
    _m1m3SAL->logEvent_powerSupplyStatus(&_eventPowerSupplyStatus, 0);
    this->previousEventPowerSupplyStatus = _eventPowerSupplyStatus;
}

void M1M3SSPublisher::tryLogPowerSupplyStatus() {
    if (_eventPowerSupplyStatus.rcpMirrorCellUtility220VAC1Status !=
                this->previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC1Status ||
        _eventPowerSupplyStatus.rcpCabinetUtility220VACStatus !=
                this->previousEventPowerSupplyStatus.rcpCabinetUtility220VACStatus ||
        _eventPowerSupplyStatus.rcpExternalEquipment220VACStatus !=
                this->previousEventPowerSupplyStatus.rcpExternalEquipment220VACStatus ||
        _eventPowerSupplyStatus.rcpMirrorCellUtility220VAC2Status !=
                this->previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC2Status ||
        _eventPowerSupplyStatus.rcpMirrorCellUtility220VAC3Status !=
                this->previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC3Status ||
        _eventPowerSupplyStatus.powerNetworkARedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkARedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkBRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkBRedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkCRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkCRedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkDRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkDRedundancyControlStatus ||
        _eventPowerSupplyStatus.controlsPowerNetworkRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.controlsPowerNetworkRedundancyControlStatus ||
        _eventPowerSupplyStatus.powerNetworkAStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkAStatus ||
        _eventPowerSupplyStatus.powerNetworkARedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkARedundantStatus ||
        _eventPowerSupplyStatus.powerNetworkBStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkBStatus ||
        _eventPowerSupplyStatus.powerNetworkBRedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkBRedundantStatus ||
        _eventPowerSupplyStatus.powerNetworkCStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkCStatus ||
        _eventPowerSupplyStatus.powerNetworkCRedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkCRedundantStatus ||
        _eventPowerSupplyStatus.powerNetworkDStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkDStatus ||
        _eventPowerSupplyStatus.powerNetworkDRedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkDRedundantStatus ||
        _eventPowerSupplyStatus.controlsPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.controlsPowerNetworkStatus ||
        _eventPowerSupplyStatus.controlsPowerNetworkRedundantStatus !=
                this->previousEventPowerSupplyStatus.controlsPowerNetworkRedundantStatus ||
        _eventPowerSupplyStatus.lightPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.lightPowerNetworkStatus ||
        _eventPowerSupplyStatus.externalEquipmentPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.externalEquipmentPowerNetworkStatus ||
        _eventPowerSupplyStatus.laserTrackerPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.laserTrackerPowerNetworkStatus) {
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
    this->previousEventPowerWarning = _eventPowerWarning;
}

void M1M3SSPublisher::tryLogPowerWarning() {
    if (_eventPowerWarning.powerNetworkAOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkAOutputMismatch ||
        _eventPowerWarning.powerNetworkBOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkBOutputMismatch ||
        _eventPowerWarning.powerNetworkCOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkCOutputMismatch ||
        _eventPowerWarning.powerNetworkDOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkDOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkAOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkAOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkBOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkBOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkCOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkCOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkDOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkDOutputMismatch) {
        this->logPowerWarning();
    }
}

void M1M3SSPublisher::logRejectedAberrationForces() {
    _m1m3SAL->logEvent_rejectedAberrationForces(&_eventRejectedAberrationForces, 0);
    this->previousEventRejectedAberrationForces = _eventRejectedAberrationForces;
}

void M1M3SSPublisher::tryLogRejectedAberrationForces() {
    bool changeDetected =
            _eventRejectedAberrationForces.fz != this->previousEventRejectedAberrationForces.fz ||
            _eventRejectedAberrationForces.mx != this->previousEventRejectedAberrationForces.mx ||
            _eventRejectedAberrationForces.my != this->previousEventRejectedAberrationForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventRejectedAberrationForces.zForces[i] !=
                                                   this->previousEventRejectedAberrationForces.zForces[i];
    }
    if (changeDetected) {
        this->logRejectedAberrationForces();
    }
}

void M1M3SSPublisher::logRejectedAccelerationForces() {
    _m1m3SAL->logEvent_rejectedAccelerationForces(&_eventRejectedAccelerationForces, 0);
    this->previousEventRejectedAccelerationForces = _eventRejectedAccelerationForces;
}

void M1M3SSPublisher::tryLogRejectedAccelerationForces() {
    bool changeDetected =
            _eventRejectedAccelerationForces.fx != this->previousEventRejectedAccelerationForces.fx ||
            _eventRejectedAccelerationForces.fy != this->previousEventRejectedAccelerationForces.fy ||
            _eventRejectedAccelerationForces.fz != this->previousEventRejectedAccelerationForces.fz ||
            _eventRejectedAccelerationForces.mx != this->previousEventRejectedAccelerationForces.mx ||
            _eventRejectedAccelerationForces.my != this->previousEventRejectedAccelerationForces.my ||
            _eventRejectedAccelerationForces.mz != this->previousEventRejectedAccelerationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedAccelerationForces.xForces[i] !=
                                            this->previousEventRejectedAccelerationForces.xForces[i]) ||
                         (i < 100 && _eventRejectedAccelerationForces.yForces[i] !=
                                             this->previousEventRejectedAccelerationForces.yForces[i]) ||
                         (_eventRejectedAccelerationForces.zForces[i] !=
                          this->previousEventRejectedAccelerationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedAccelerationForces();
    }
}

void M1M3SSPublisher::logRejectedActiveOpticForces() {
    _m1m3SAL->logEvent_rejectedActiveOpticForces(&_eventRejectedActiveOpticForces, 0);
    this->previousEventRejectedActiveOpticForces = _eventRejectedActiveOpticForces;
}

void M1M3SSPublisher::tryLogRejectedActiveOpticForces() {
    bool changeDetected =
            _eventRejectedActiveOpticForces.fz != this->previousEventRejectedActiveOpticForces.fz ||
            _eventRejectedActiveOpticForces.mx != this->previousEventRejectedActiveOpticForces.mx ||
            _eventRejectedActiveOpticForces.my != this->previousEventRejectedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventRejectedActiveOpticForces.zForces[i] !=
                                                   this->previousEventRejectedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        this->logRejectedActiveOpticForces();
    }
}

void M1M3SSPublisher::logRejectedAzimuthForces() {
    _m1m3SAL->logEvent_rejectedAzimuthForces(&_eventRejectedAzimuthForces, 0);
    this->previousEventRejectedAzimuthForces = _eventRejectedAzimuthForces;
}

void M1M3SSPublisher::tryLogRejectedAzimuthForces() {
    bool changeDetected =
            _eventRejectedAzimuthForces.fx != this->previousEventRejectedAzimuthForces.fx ||
            _eventRejectedAzimuthForces.fy != this->previousEventRejectedAzimuthForces.fy ||
            _eventRejectedAzimuthForces.fz != this->previousEventRejectedAzimuthForces.fz ||
            _eventRejectedAzimuthForces.mx != this->previousEventRejectedAzimuthForces.mx ||
            _eventRejectedAzimuthForces.my != this->previousEventRejectedAzimuthForces.my ||
            _eventRejectedAzimuthForces.mz != this->previousEventRejectedAzimuthForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedAzimuthForces.xForces[i] !=
                                            this->previousEventRejectedAzimuthForces.xForces[i]) ||
                         (i < 100 && _eventRejectedAzimuthForces.yForces[i] !=
                                             this->previousEventRejectedAzimuthForces.yForces[i]) ||
                         (_eventRejectedAzimuthForces.zForces[i] !=
                          this->previousEventRejectedAzimuthForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedAzimuthForces();
    }
}

void M1M3SSPublisher::logRejectedBalanceForces() {
    _m1m3SAL->logEvent_rejectedBalanceForces(&_eventRejectedBalanceForces, 0);
    this->previousEventRejectedBalanceForces = _eventRejectedBalanceForces;
}

void M1M3SSPublisher::tryLogRejectedBalanceForces() {
    bool changeDetected =
            _eventRejectedBalanceForces.fx != this->previousEventRejectedBalanceForces.fx ||
            _eventRejectedBalanceForces.fy != this->previousEventRejectedBalanceForces.fy ||
            _eventRejectedBalanceForces.fz != this->previousEventRejectedBalanceForces.fz ||
            _eventRejectedBalanceForces.mx != this->previousEventRejectedBalanceForces.mx ||
            _eventRejectedBalanceForces.my != this->previousEventRejectedBalanceForces.my ||
            _eventRejectedBalanceForces.mz != this->previousEventRejectedBalanceForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedBalanceForces.xForces[i] !=
                                            this->previousEventRejectedBalanceForces.xForces[i]) ||
                         (i < 100 && _eventRejectedBalanceForces.yForces[i] !=
                                             this->previousEventRejectedBalanceForces.yForces[i]) ||
                         (_eventRejectedBalanceForces.zForces[i] !=
                          this->previousEventRejectedBalanceForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedBalanceForces();
    }
}

void M1M3SSPublisher::logRejectedCylinderForces() {
    _m1m3SAL->logEvent_rejectedCylinderForces(&_eventRejectedCylinderForces, 0);
    this->previousEventRejectedCylinderForces = _eventRejectedCylinderForces;
}

void M1M3SSPublisher::tryLogRejectedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && _eventRejectedCylinderForces.secondaryCylinderForces[i] !=
                                    this->previousEventRejectedCylinderForces.secondaryCylinderForces[i]) ||
                (_eventRejectedCylinderForces.primaryCylinderForces[i] !=
                 this->previousEventRejectedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        this->logRejectedCylinderForces();
    }
}

void M1M3SSPublisher::logRejectedElevationForces() {
    _m1m3SAL->logEvent_rejectedElevationForces(&_eventRejectedElevationForces, 0);
    this->previousEventRejectedElevationForces = _eventRejectedElevationForces;
}

void M1M3SSPublisher::tryLogRejectedElevationForces() {
    bool changeDetected =
            _eventRejectedElevationForces.fx != this->previousEventRejectedElevationForces.fx ||
            _eventRejectedElevationForces.fy != this->previousEventRejectedElevationForces.fy ||
            _eventRejectedElevationForces.fz != this->previousEventRejectedElevationForces.fz ||
            _eventRejectedElevationForces.mx != this->previousEventRejectedElevationForces.mx ||
            _eventRejectedElevationForces.my != this->previousEventRejectedElevationForces.my ||
            _eventRejectedElevationForces.mz != this->previousEventRejectedElevationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedElevationForces.xForces[i] !=
                                            this->previousEventRejectedElevationForces.xForces[i]) ||
                         (i < 100 && _eventRejectedElevationForces.yForces[i] !=
                                             this->previousEventRejectedElevationForces.yForces[i]) ||
                         (_eventRejectedElevationForces.zForces[i] !=
                          this->previousEventRejectedElevationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedElevationForces();
    }
}

void M1M3SSPublisher::logRejectedForces() {
    _m1m3SAL->logEvent_rejectedForces(&_eventRejectedForces, 0);
    this->previousEventRejectedForces = _eventRejectedForces;
}

void M1M3SSPublisher::tryLogRejectedForces() {
    bool changeDetected = _eventRejectedForces.fx != this->previousEventRejectedForces.fx ||
                          _eventRejectedForces.fy != this->previousEventRejectedForces.fy ||
                          _eventRejectedForces.fz != this->previousEventRejectedForces.fz ||
                          _eventRejectedForces.mx != this->previousEventRejectedForces.mx ||
                          _eventRejectedForces.my != this->previousEventRejectedForces.my ||
                          _eventRejectedForces.mz != this->previousEventRejectedForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventRejectedForces.xForces[i] != this->previousEventRejectedForces.xForces[i]) ||
                (i < 100 &&
                 _eventRejectedForces.yForces[i] != this->previousEventRejectedForces.yForces[i]) ||
                (_eventRejectedForces.zForces[i] != this->previousEventRejectedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedForces();
    }
}

void M1M3SSPublisher::logRejectedOffsetForces() {
    _m1m3SAL->logEvent_rejectedOffsetForces(&_eventRejectedOffsetForces, 0);
    this->previousEventRejectedOffsetForces = _eventRejectedOffsetForces;
}

void M1M3SSPublisher::tryLogRejectedOffsetForces() {
    bool changeDetected = _eventRejectedOffsetForces.fx != this->previousEventRejectedOffsetForces.fx ||
                          _eventRejectedOffsetForces.fy != this->previousEventRejectedOffsetForces.fy ||
                          _eventRejectedOffsetForces.fz != this->previousEventRejectedOffsetForces.fz ||
                          _eventRejectedOffsetForces.mx != this->previousEventRejectedOffsetForces.mx ||
                          _eventRejectedOffsetForces.my != this->previousEventRejectedOffsetForces.my ||
                          _eventRejectedOffsetForces.mz != this->previousEventRejectedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedOffsetForces.xForces[i] !=
                                            this->previousEventRejectedOffsetForces.xForces[i]) ||
                         (i < 100 && _eventRejectedOffsetForces.yForces[i] !=
                                             this->previousEventRejectedOffsetForces.yForces[i]) ||
                         (_eventRejectedOffsetForces.zForces[i] !=
                          this->previousEventRejectedOffsetForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedOffsetForces();
    }
}

void M1M3SSPublisher::logRejectedStaticForces() {
    _m1m3SAL->logEvent_rejectedStaticForces(&_eventRejectedStaticForces, 0);
    this->previousEventRejectedStaticForces = _eventRejectedStaticForces;
}

void M1M3SSPublisher::tryLogRejectedStaticForces() {
    bool changeDetected = _eventRejectedStaticForces.fx != this->previousEventRejectedStaticForces.fx ||
                          _eventRejectedStaticForces.fy != this->previousEventRejectedStaticForces.fy ||
                          _eventRejectedStaticForces.fz != this->previousEventRejectedStaticForces.fz ||
                          _eventRejectedStaticForces.mx != this->previousEventRejectedStaticForces.mx ||
                          _eventRejectedStaticForces.my != this->previousEventRejectedStaticForces.my ||
                          _eventRejectedStaticForces.mz != this->previousEventRejectedStaticForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedStaticForces.xForces[i] !=
                                            this->previousEventRejectedStaticForces.xForces[i]) ||
                         (i < 100 && _eventRejectedStaticForces.yForces[i] !=
                                             this->previousEventRejectedStaticForces.yForces[i]) ||
                         (_eventRejectedStaticForces.zForces[i] !=
                          this->previousEventRejectedStaticForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedStaticForces();
    }
}

void M1M3SSPublisher::logRejectedThermalForces() {
    _m1m3SAL->logEvent_rejectedThermalForces(&_eventRejectedThermalForces, 0);
    this->previousEventRejectedThermalForces = _eventRejectedThermalForces;
}

void M1M3SSPublisher::tryLogRejectedThermalForces() {
    bool changeDetected =
            _eventRejectedThermalForces.fx != this->previousEventRejectedThermalForces.fx ||
            _eventRejectedThermalForces.fy != this->previousEventRejectedThermalForces.fy ||
            _eventRejectedThermalForces.fz != this->previousEventRejectedThermalForces.fz ||
            _eventRejectedThermalForces.mx != this->previousEventRejectedThermalForces.mx ||
            _eventRejectedThermalForces.my != this->previousEventRejectedThermalForces.my ||
            _eventRejectedThermalForces.mz != this->previousEventRejectedThermalForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedThermalForces.xForces[i] !=
                                            this->previousEventRejectedThermalForces.xForces[i]) ||
                         (i < 100 && _eventRejectedThermalForces.yForces[i] !=
                                             this->previousEventRejectedThermalForces.yForces[i]) ||
                         (_eventRejectedThermalForces.zForces[i] !=
                          this->previousEventRejectedThermalForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedThermalForces();
    }
}

void M1M3SSPublisher::logRejectedVelocityForces() {
    _m1m3SAL->logEvent_rejectedVelocityForces(&_eventRejectedVelocityForces, 0);
    this->previousEventRejectedVelocityForces = _eventRejectedVelocityForces;
}

void M1M3SSPublisher::tryLogRejectedVelocityForces() {
    bool changeDetected =
            _eventRejectedVelocityForces.fx != this->previousEventRejectedVelocityForces.fx ||
            _eventRejectedVelocityForces.fy != this->previousEventRejectedVelocityForces.fy ||
            _eventRejectedVelocityForces.fz != this->previousEventRejectedVelocityForces.fz ||
            _eventRejectedVelocityForces.mx != this->previousEventRejectedVelocityForces.mx ||
            _eventRejectedVelocityForces.my != this->previousEventRejectedVelocityForces.my ||
            _eventRejectedVelocityForces.mz != this->previousEventRejectedVelocityForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && _eventRejectedVelocityForces.xForces[i] !=
                                            this->previousEventRejectedVelocityForces.xForces[i]) ||
                         (i < 100 && _eventRejectedVelocityForces.yForces[i] !=
                                             this->previousEventRejectedVelocityForces.yForces[i]) ||
                         (_eventRejectedVelocityForces.zForces[i] !=
                          this->previousEventRejectedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedVelocityForces();
    }
}

void M1M3SSPublisher::logSettingVersions() {
    _m1m3SAL->logEvent_settingVersions(&_eventSettingVersions, 0);
}

void M1M3SSPublisher::logSettingsApplied() {
    _m1m3SAL->logEvent_settingsApplied(&_eventSettingsApplied, 0);
    this->previousEventSettingsApplied = _eventSettingsApplied;
}

void M1M3SSPublisher::tryLogSettingsApplied() {
    if (_eventSettingsApplied.settingsVersion.compare(
                this->previousEventSettingsApplied.settingsVersion) != 0) {
        this->logSettingsApplied();
    }
}

void M1M3SSPublisher::logSummaryState() {
    _m1m3SAL->logEvent_summaryState(&_eventSummaryState, 0);
    this->previousEventSummaryState = _eventSummaryState;
}

void M1M3SSPublisher::tryLogSummaryState() {
    if (_eventSummaryState.summaryState != this->previousEventSummaryState.summaryState) {
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

void M1M3SSPublisher::ackCommandshutdown(int32_t commandID, int32_t ackCode, std::string description) {
    _m1m3SAL->ackCommand_shutdown(commandID, ackCode, 0, (char*)description.c_str());
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
    _m1m3SAL->ackCommand_applyOffsetForcesByMirrorForce(commandID, ackCode, 0,
                                                             (char*)description.c_str());
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
