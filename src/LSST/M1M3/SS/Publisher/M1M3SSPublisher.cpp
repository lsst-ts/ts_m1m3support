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
    this->m1m3SAL = m1m3SAL;

    spdlog::debug("M1M3SSPublisher: Initializing SAL Telemetry");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_accelerometerData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_forceActuatorData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_gyroData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_hardpointActuatorData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_hardpointMonitorData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_imsData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_inclinometerData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_outerLoopData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_pidData");
    this->m1m3SAL->salTelemetryPub((char*)"MTM1M3_powerSupplyData");

    spdlog::debug("M1M3SSPublisher: Initializing SAL Events");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_accelerometerWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_accelerometerWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_airSupplyStatus");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_airSupplyWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedAberrationForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedAccelerationForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedActiveOpticForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedAzimuthForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedBalanceForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedCylinderForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedElevationForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedOffsetForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedSettingsMatchStart");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedStaticForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedThermalForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_appliedVelocityForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_cellLightStatus");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_cellLightWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_commandRejectionWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_detailedState");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_displacementSensorWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_errorCode");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorForceWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorInfo");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorState");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceActuatorWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_forceSetpointWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_gyroWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointActuatorInfo");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointActuatorState");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointActuatorWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointMonitorInfo");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointMonitorState");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_hardpointMonitorWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_ilcWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_inclinometerSensorWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_interlockStatus");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_interlockWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_modbusResponse");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_pidInfo");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_powerStatus");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_powerSupplyStatus");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_powerWarning");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedAberrationForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedAccelerationForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedActiveOpticForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedAzimuthForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedBalanceForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedCylinderForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedElevationForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedOffsetForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedStaticForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedThermalForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_rejectedVelocityForces");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_settingVersions");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_settingsApplied");
    this->m1m3SAL->salEventPub((char*)"MTM1M3_logevent_summaryState");
}

double M1M3SSPublisher::getTimestamp() { return this->m1m3SAL->getCurrentTime(); }

void M1M3SSPublisher::putAccelerometerData() {
    this->m1m3SAL->putSample_accelerometerData(&this->accelerometerData);
}
void M1M3SSPublisher::putForceActuatorData() {
    this->m1m3SAL->putSample_forceActuatorData(&this->forceActuatorData);
}
void M1M3SSPublisher::putGyroData() { this->m1m3SAL->putSample_gyroData(&this->gyroData); }
void M1M3SSPublisher::putHardpointActuatorData() {
    this->m1m3SAL->putSample_hardpointActuatorData(&this->hardpointActuatorData);
}
void M1M3SSPublisher::putHardpointMonitorData() {
    this->m1m3SAL->putSample_hardpointMonitorData(&this->hardpointMonitorData);
}
void M1M3SSPublisher::putIMSData() { this->m1m3SAL->putSample_imsData(&this->imsData); }
void M1M3SSPublisher::putInclinometerData() {
    this->m1m3SAL->putSample_inclinometerData(&this->inclinometerData);
}
void M1M3SSPublisher::putOuterLoopData() { this->m1m3SAL->putSample_outerLoopData(&this->outerLoopData); }
void M1M3SSPublisher::putPIDData() { this->m1m3SAL->putSample_pidData(&this->pidData); }
void M1M3SSPublisher::putPowerSupplyData() {
    this->m1m3SAL->putSample_powerSupplyData(&this->powerSupplyData);
}

void M1M3SSPublisher::logAccelerometerWarning() {
    this->eventAccelerometerWarning.anyWarning = this->eventAccelerometerWarning.responseTimeout;
    this->m1m3SAL->logEvent_accelerometerWarning(&this->eventAccelerometerWarning, 0);
    this->previousEventAccelerometerWarning = this->eventAccelerometerWarning;
}

void M1M3SSPublisher::tryLogAccelerometerWarning() {
    if (this->eventAccelerometerWarning.responseTimeout !=
        this->previousEventAccelerometerWarning.responseTimeout) {
        this->logAccelerometerWarning();
    }
}

void M1M3SSPublisher::logAirSupplyStatus() {
    this->m1m3SAL->logEvent_airSupplyStatus(&this->eventAirSupplyStatus, 0);
    this->previousEventAirSupplyStatus = this->eventAirSupplyStatus;
}

void M1M3SSPublisher::tryLogAirSupplyStatus() {
    if (this->eventAirSupplyStatus.airCommandedOn != this->previousEventAirSupplyStatus.airCommandedOn ||
        this->eventAirSupplyStatus.airCommandOutputOn !=
                this->previousEventAirSupplyStatus.airCommandOutputOn ||
        this->eventAirSupplyStatus.airValveOpened != this->previousEventAirSupplyStatus.airValveOpened ||
        this->eventAirSupplyStatus.airValveClosed != this->previousEventAirSupplyStatus.airValveClosed) {
        this->logAirSupplyStatus();
    }
}

void M1M3SSPublisher::logAirSupplyWarning() {
    this->eventAirSupplyWarning.anyWarning = this->eventAirSupplyWarning.commandOutputMismatch ||
                                             this->eventAirSupplyWarning.commandSensorMismatch;
    this->m1m3SAL->logEvent_airSupplyWarning(&this->eventAirSupplyWarning, 0);
    this->previousEventAirSupplyWarning = this->eventAirSupplyWarning;
}

void M1M3SSPublisher::tryLogAirSupplyWarning() {
    if (this->eventAirSupplyWarning.commandOutputMismatch !=
                this->previousEventAirSupplyWarning.commandOutputMismatch ||
        this->eventAirSupplyWarning.commandSensorMismatch !=
                this->previousEventAirSupplyWarning.commandSensorMismatch) {
        this->logAirSupplyWarning();
    }
}

void M1M3SSPublisher::logAppliedAberrationForces() {
    this->m1m3SAL->logEvent_appliedAberrationForces(&this->eventAppliedAberrationForces, 0);
    this->previousEventAppliedAberrationForces = this->eventAppliedAberrationForces;
}

void M1M3SSPublisher::tryLogAppliedAberrationForces() {
    bool changeDetected =
            this->eventAppliedAberrationForces.fz != this->previousEventAppliedAberrationForces.fz ||
            this->eventAppliedAberrationForces.mx != this->previousEventAppliedAberrationForces.mx ||
            this->eventAppliedAberrationForces.my != this->previousEventAppliedAberrationForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || this->eventAppliedAberrationForces.zForces[i] !=
                                                   this->previousEventAppliedAberrationForces.zForces[i];
    }
    if (changeDetected) {
        this->logAppliedAberrationForces();
    }
}

void M1M3SSPublisher::logAppliedAccelerationForces() {
    this->m1m3SAL->logEvent_appliedAccelerationForces(&this->eventAppliedAccelerationForces, 0);
    this->previousEventAppliedAccelerationForces = this->eventAppliedAccelerationForces;
}

void M1M3SSPublisher::tryLogAppliedAccelerationForces() {
    bool changeDetected =
            this->eventAppliedAccelerationForces.fx != this->previousEventAppliedAccelerationForces.fx ||
            this->eventAppliedAccelerationForces.fy != this->previousEventAppliedAccelerationForces.fy ||
            this->eventAppliedAccelerationForces.fz != this->previousEventAppliedAccelerationForces.fz ||
            this->eventAppliedAccelerationForces.mx != this->previousEventAppliedAccelerationForces.mx ||
            this->eventAppliedAccelerationForces.my != this->previousEventAppliedAccelerationForces.my ||
            this->eventAppliedAccelerationForces.mz != this->previousEventAppliedAccelerationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedAccelerationForces.xForces[i] !=
                                            this->previousEventAppliedAccelerationForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedAccelerationForces.yForces[i] !=
                                             this->previousEventAppliedAccelerationForces.yForces[i]) ||
                         (this->eventAppliedAccelerationForces.zForces[i] !=
                          this->previousEventAppliedAccelerationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedAccelerationForces();
    }
}

void M1M3SSPublisher::logAppliedActiveOpticForces() {
    this->m1m3SAL->logEvent_appliedActiveOpticForces(&this->eventAppliedActiveOpticForces, 0);
    this->previousEventAppliedActiveOpticForces = this->eventAppliedActiveOpticForces;
}

void M1M3SSPublisher::tryLogAppliedActiveOpticForces() {
    bool changeDetected =
            this->eventAppliedActiveOpticForces.fz != this->previousEventAppliedActiveOpticForces.fz ||
            this->eventAppliedActiveOpticForces.mx != this->previousEventAppliedActiveOpticForces.mx ||
            this->eventAppliedActiveOpticForces.my != this->previousEventAppliedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || this->eventAppliedActiveOpticForces.zForces[i] !=
                                                   this->previousEventAppliedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        this->logAppliedActiveOpticForces();
    }
}

void M1M3SSPublisher::logAppliedAzimuthForces() {
    this->m1m3SAL->logEvent_appliedAzimuthForces(&this->eventAppliedAzimuthForces, 0);
    this->previousEventAppliedAzimuthForces = this->eventAppliedAzimuthForces;
}

void M1M3SSPublisher::tryLogAppliedAzimuthForces() {
    bool changeDetected = this->eventAppliedAzimuthForces.fx != this->previousEventAppliedAzimuthForces.fx ||
                          this->eventAppliedAzimuthForces.fy != this->previousEventAppliedAzimuthForces.fy ||
                          this->eventAppliedAzimuthForces.fz != this->previousEventAppliedAzimuthForces.fz ||
                          this->eventAppliedAzimuthForces.mx != this->previousEventAppliedAzimuthForces.mx ||
                          this->eventAppliedAzimuthForces.my != this->previousEventAppliedAzimuthForces.my ||
                          this->eventAppliedAzimuthForces.mz != this->previousEventAppliedAzimuthForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedAzimuthForces.xForces[i] !=
                                            this->previousEventAppliedAzimuthForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedAzimuthForces.yForces[i] !=
                                             this->previousEventAppliedAzimuthForces.yForces[i]) ||
                         (this->eventAppliedAzimuthForces.zForces[i] !=
                          this->previousEventAppliedAzimuthForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedAzimuthForces();
    }
}

void M1M3SSPublisher::logAppliedBalanceForces() {
    this->m1m3SAL->logEvent_appliedBalanceForces(&this->eventAppliedBalanceForces, 0);
    this->previousEventAppliedBalanceForces = this->eventAppliedBalanceForces;
}

void M1M3SSPublisher::tryLogAppliedBalanceForces() {
    bool changeDetected = this->eventAppliedBalanceForces.fx != this->previousEventAppliedBalanceForces.fx ||
                          this->eventAppliedBalanceForces.fy != this->previousEventAppliedBalanceForces.fy ||
                          this->eventAppliedBalanceForces.fz != this->previousEventAppliedBalanceForces.fz ||
                          this->eventAppliedBalanceForces.mx != this->previousEventAppliedBalanceForces.mx ||
                          this->eventAppliedBalanceForces.my != this->previousEventAppliedBalanceForces.my ||
                          this->eventAppliedBalanceForces.mz != this->previousEventAppliedBalanceForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedBalanceForces.xForces[i] !=
                                            this->previousEventAppliedBalanceForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedBalanceForces.yForces[i] !=
                                             this->previousEventAppliedBalanceForces.yForces[i]) ||
                         (this->eventAppliedBalanceForces.zForces[i] !=
                          this->previousEventAppliedBalanceForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedBalanceForces();
    }
}

void M1M3SSPublisher::logAppliedCylinderForces() {
    this->m1m3SAL->logEvent_appliedCylinderForces(&this->eventAppliedCylinderForces, 0);
    this->previousEventAppliedCylinderForces = this->eventAppliedCylinderForces;
}

void M1M3SSPublisher::tryLogAppliedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && this->eventAppliedCylinderForces.secondaryCylinderForces[i] !=
                                    this->previousEventAppliedCylinderForces.secondaryCylinderForces[i]) ||
                (this->eventAppliedCylinderForces.primaryCylinderForces[i] !=
                 this->previousEventAppliedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        this->logAppliedCylinderForces();
    }
}

void M1M3SSPublisher::logAppliedElevationForces() {
    this->m1m3SAL->logEvent_appliedElevationForces(&this->eventAppliedElevationForces, 0);
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
    this->m1m3SAL->logEvent_appliedForces(&this->eventAppliedForces, 0);
    this->previousEventAppliedForces = this->eventAppliedForces;
}

void M1M3SSPublisher::tryLogAppliedForces() {
    bool changeDetected = this->eventAppliedForces.fx != this->previousEventAppliedForces.fx ||
                          this->eventAppliedForces.fy != this->previousEventAppliedForces.fy ||
                          this->eventAppliedForces.fz != this->previousEventAppliedForces.fz ||
                          this->eventAppliedForces.mx != this->previousEventAppliedForces.mx ||
                          this->eventAppliedForces.my != this->previousEventAppliedForces.my ||
                          this->eventAppliedForces.mz != this->previousEventAppliedForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedForces.xForces[i] !=
                                            this->previousEventAppliedForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedForces.yForces[i] !=
                                             this->previousEventAppliedForces.yForces[i]) ||
                         (this->eventAppliedForces.zForces[i] != this->previousEventAppliedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedForces();
    }
}

void M1M3SSPublisher::logAppliedOffsetForces() {
    this->m1m3SAL->logEvent_appliedOffsetForces(&this->eventAppliedOffsetForces, 0);
    this->previousEventAppliedOffsetForces = this->eventAppliedOffsetForces;
}

void M1M3SSPublisher::tryLogAppliedOffsetForces() {
    bool changeDetected = this->eventAppliedOffsetForces.fx != this->previousEventAppliedOffsetForces.fx ||
                          this->eventAppliedOffsetForces.fy != this->previousEventAppliedOffsetForces.fy ||
                          this->eventAppliedOffsetForces.fz != this->previousEventAppliedOffsetForces.fz ||
                          this->eventAppliedOffsetForces.mx != this->previousEventAppliedOffsetForces.mx ||
                          this->eventAppliedOffsetForces.my != this->previousEventAppliedOffsetForces.my ||
                          this->eventAppliedOffsetForces.mz != this->previousEventAppliedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedOffsetForces.xForces[i] !=
                                            this->previousEventAppliedOffsetForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedOffsetForces.yForces[i] !=
                                             this->previousEventAppliedOffsetForces.yForces[i]) ||
                         (this->eventAppliedOffsetForces.zForces[i] !=
                          this->previousEventAppliedOffsetForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedOffsetForces();
    }
}

void M1M3SSPublisher::logAppliedSettingsMatchStart() {
    this->m1m3SAL->logEvent_appliedSettingsMatchStart(&this->eventAppliedSettingsMatchStart, 0);
    this->previousEventAppliedSettingsMatchStart = this->eventAppliedSettingsMatchStart;
}

void M1M3SSPublisher::tryLogAppliedSettingsMatchStart() {
    if (this->eventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue !=
        this->previousEventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue) {
        this->logAppliedSettingsMatchStart();
    }
}

void M1M3SSPublisher::logAppliedStaticForces() {
    this->m1m3SAL->logEvent_appliedStaticForces(&this->eventAppliedStaticForces, 0);
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
    this->m1m3SAL->logEvent_appliedThermalForces(&this->eventAppliedThermalForces, 0);
    this->previousEventAppliedThermalForces = this->eventAppliedThermalForces;
}

void M1M3SSPublisher::tryLogAppliedThermalForces() {
    bool changeDetected = this->eventAppliedThermalForces.fx != this->previousEventAppliedThermalForces.fx ||
                          this->eventAppliedThermalForces.fy != this->previousEventAppliedThermalForces.fy ||
                          this->eventAppliedThermalForces.fz != this->previousEventAppliedThermalForces.fz ||
                          this->eventAppliedThermalForces.mx != this->previousEventAppliedThermalForces.mx ||
                          this->eventAppliedThermalForces.my != this->previousEventAppliedThermalForces.my ||
                          this->eventAppliedThermalForces.mz != this->previousEventAppliedThermalForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedThermalForces.xForces[i] !=
                                            this->previousEventAppliedThermalForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedThermalForces.yForces[i] !=
                                             this->previousEventAppliedThermalForces.yForces[i]) ||
                         (this->eventAppliedThermalForces.zForces[i] !=
                          this->previousEventAppliedThermalForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedThermalForces();
    }
}

void M1M3SSPublisher::logAppliedVelocityForces() {
    this->m1m3SAL->logEvent_appliedVelocityForces(&this->eventAppliedVelocityForces, 0);
    this->previousEventAppliedVelocityForces = this->eventAppliedVelocityForces;
}

void M1M3SSPublisher::tryLogAppliedVelocityForces() {
    bool changeDetected =
            this->eventAppliedVelocityForces.fx != this->previousEventAppliedVelocityForces.fx ||
            this->eventAppliedVelocityForces.fy != this->previousEventAppliedVelocityForces.fy ||
            this->eventAppliedVelocityForces.fz != this->previousEventAppliedVelocityForces.fz ||
            this->eventAppliedVelocityForces.mx != this->previousEventAppliedVelocityForces.mx ||
            this->eventAppliedVelocityForces.my != this->previousEventAppliedVelocityForces.my ||
            this->eventAppliedVelocityForces.mz != this->previousEventAppliedVelocityForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedVelocityForces.xForces[i] !=
                                            this->previousEventAppliedVelocityForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedVelocityForces.yForces[i] !=
                                             this->previousEventAppliedVelocityForces.yForces[i]) ||
                         (this->eventAppliedVelocityForces.zForces[i] !=
                          this->previousEventAppliedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedVelocityForces();
    }
}

void M1M3SSPublisher::logCellLightStatus() {
    this->m1m3SAL->logEvent_cellLightStatus(&this->eventCellLightStatus, 0);
    this->previousEventCellLightStatus = this->eventCellLightStatus;
}

void M1M3SSPublisher::tryLogCellLightStatus() {
    if (this->eventCellLightStatus.cellLightsCommandedOn !=
                this->previousEventCellLightStatus.cellLightsCommandedOn ||
        this->eventCellLightStatus.cellLightsOutputOn !=
                this->previousEventCellLightStatus.cellLightsOutputOn ||
        this->eventCellLightStatus.cellLightsOn != this->previousEventCellLightStatus.cellLightsOn) {
        this->logCellLightStatus();
    }
}

void M1M3SSPublisher::logCellLightWarning() {
    this->eventCellLightWarning.anyWarning = this->eventCellLightWarning.cellLightsOutputMismatch ||
                                             this->eventCellLightWarning.cellLightsSensorMismatch;
    this->m1m3SAL->logEvent_cellLightWarning(&this->eventCellLightWarning, 0);
    this->previousEventCellLightWarning = this->eventCellLightWarning;
}

void M1M3SSPublisher::tryLogCellLightWarning() {
    if (this->eventCellLightWarning.cellLightsOutputMismatch !=
                this->previousEventCellLightWarning.cellLightsOutputMismatch ||
        this->eventCellLightWarning.cellLightsSensorMismatch !=
                this->previousEventCellLightWarning.cellLightsSensorMismatch) {
        this->logCellLightWarning();
    }
}

void M1M3SSPublisher::logCommandRejectionWarning() {
    this->m1m3SAL->logEvent_commandRejectionWarning(&this->eventCommandRejectionWarning, 0);
    this->previousEventCommandRejectionWarning = this->eventCommandRejectionWarning;
}

void M1M3SSPublisher::tryLogCommandRejectionWarning() {
    if (this->eventCommandRejectionWarning.command.compare(
                this->previousEventCommandRejectionWarning.command) != 0 ||
        this->eventCommandRejectionWarning.reason.compare(
                this->previousEventCommandRejectionWarning.reason) != 0) {
        this->logCommandRejectionWarning();
    }
}

void M1M3SSPublisher::logCommandRejectionWarning(std::string command, std::string reason) {
    this->eventCommandRejectionWarning.timestamp = this->getTimestamp();
    this->eventCommandRejectionWarning.command = command;
    this->eventCommandRejectionWarning.reason = reason;
    this->logCommandRejectionWarning();
}

void M1M3SSPublisher::tryLogCommandRejectionWarning(std::string command, std::string reason) {
    this->eventCommandRejectionWarning.timestamp = this->getTimestamp();
    this->eventCommandRejectionWarning.command = command;
    this->eventCommandRejectionWarning.reason = reason;
    this->tryLogCommandRejectionWarning();
}

void M1M3SSPublisher::logDetailedState() {
    this->m1m3SAL->logEvent_detailedState(&this->eventDetailedState, 0);
    this->previousEventDetailedState = this->eventDetailedState;
}

void M1M3SSPublisher::tryLogDetailedState() {
    if (this->eventDetailedState.detailedState != this->previousEventDetailedState.detailedState) {
        this->logDetailedState();
    }
}

void M1M3SSPublisher::logDisplacementSensorWarning() {
    this->eventDisplacementSensorWarning.anyWarning =
            this->eventDisplacementSensorWarning.sensorReportsInvalidCommand ||
            this->eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
            this->eventDisplacementSensorWarning.sensorReportsDataLengthError ||
            this->eventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
            this->eventDisplacementSensorWarning.sensorReportsParameterError ||
            this->eventDisplacementSensorWarning.sensorReportsCommunicationError ||
            this->eventDisplacementSensorWarning.sensorReportsIDNumberError ||
            this->eventDisplacementSensorWarning.sensorReportsExpansionLineError ||
            this->eventDisplacementSensorWarning.sensorReportsWriteControlError ||
            this->eventDisplacementSensorWarning.responseTimeout ||
            this->eventDisplacementSensorWarning.invalidLength ||
            this->eventDisplacementSensorWarning.invalidResponse ||
            this->eventDisplacementSensorWarning.unknownCommand ||
            this->eventDisplacementSensorWarning.unknownProblem;
    this->m1m3SAL->logEvent_displacementSensorWarning(&this->eventDisplacementSensorWarning, 0);
    this->previousEventDisplacementSensorWarning = this->eventDisplacementSensorWarning;
}

void M1M3SSPublisher::tryLogDisplacementSensorWarning() {
    if (this->eventDisplacementSensorWarning.sensorReportsInvalidCommand !=
                this->previousEventDisplacementSensorWarning.sensorReportsInvalidCommand ||
        this->eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError !=
                this->previousEventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
        this->eventDisplacementSensorWarning.sensorReportsDataLengthError !=
                this->previousEventDisplacementSensorWarning.sensorReportsDataLengthError ||
        this->eventDisplacementSensorWarning.sensorReportsNumberOfParametersError !=
                this->previousEventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
        this->eventDisplacementSensorWarning.sensorReportsParameterError !=
                this->previousEventDisplacementSensorWarning.sensorReportsParameterError ||
        this->eventDisplacementSensorWarning.sensorReportsCommunicationError !=
                this->previousEventDisplacementSensorWarning.sensorReportsCommunicationError ||
        this->eventDisplacementSensorWarning.sensorReportsIDNumberError !=
                this->previousEventDisplacementSensorWarning.sensorReportsIDNumberError ||
        this->eventDisplacementSensorWarning.sensorReportsExpansionLineError !=
                this->previousEventDisplacementSensorWarning.sensorReportsExpansionLineError ||
        this->eventDisplacementSensorWarning.sensorReportsWriteControlError !=
                this->previousEventDisplacementSensorWarning.sensorReportsWriteControlError ||
        this->eventDisplacementSensorWarning.responseTimeout !=
                this->previousEventDisplacementSensorWarning.responseTimeout ||
        this->eventDisplacementSensorWarning.invalidLength !=
                this->previousEventDisplacementSensorWarning.invalidLength ||
        this->eventDisplacementSensorWarning.invalidResponse !=
                this->previousEventDisplacementSensorWarning.invalidResponse ||
        this->eventDisplacementSensorWarning.unknownCommand !=
                this->previousEventDisplacementSensorWarning.unknownCommand ||
        this->eventDisplacementSensorWarning.unknownProblem !=
                this->previousEventDisplacementSensorWarning.unknownProblem) {
        this->logDisplacementSensorWarning();
    }
}

void M1M3SSPublisher::logErrorCode() {
    this->m1m3SAL->logEvent_errorCode(&this->eventErrorCode, 0);
    this->previousEventErrorCode = this->eventErrorCode;
}

void M1M3SSPublisher::tryLogErrorCode() {
    if (this->eventErrorCode.errorCode != this->previousEventErrorCode.errorCode) {
        this->logErrorCode();
    }
}

void M1M3SSPublisher::logForceActuatorForceWarning() {
    this->eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning = false;
    this->eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning = false;
    this->eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning = false;
    this->eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        this->eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning =
                this->eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning ||
                this->eventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i];
        this->eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning =
                this->eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning ||
                this->eventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i];
        this->eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning =
                this->eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning ||
                this->eventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i];
        this->eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning =
                this->eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning ||
                this->eventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i];
    }
    this->eventForceActuatorForceWarning.anyWarning =
            this->eventForceActuatorForceWarning.anyPrimaryAxisMeasuredForceWarning ||
            this->eventForceActuatorForceWarning.anySecondaryAxisMeasuredForceWarning ||
            this->eventForceActuatorForceWarning.anyPrimaryAxisFollowingErrorWarning ||
            this->eventForceActuatorForceWarning.anySecondaryAxisFollowingErrorWarning;
    this->m1m3SAL->logEvent_forceActuatorForceWarning(&this->eventForceActuatorForceWarning, 0);
    this->previousEventForceActuatorForceWarning = this->eventForceActuatorForceWarning;
}

void M1M3SSPublisher::tryLogForceActuatorForceWarning() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                this->eventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i] !=
                        this->previousEventForceActuatorForceWarning.primaryAxisMeasuredForceWarning[i] ||
                this->eventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i] !=
                        this->previousEventForceActuatorForceWarning.secondaryAxisMeasuredForceWarning[i] ||
                this->eventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i] !=
                        this->previousEventForceActuatorForceWarning.primaryAxisFollowingErrorWarning[i] ||
                this->eventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i] !=
                        this->previousEventForceActuatorForceWarning.secondaryAxisFollowingErrorWarning[i];
    }
    if (changeDetected) {
        this->logForceActuatorForceWarning();
    }
}

void M1M3SSPublisher::logForceActuatorInfo() {
    this->m1m3SAL->logEvent_forceActuatorInfo(&this->eventForceActuatorInfo, 0);
    this->previousEventForceActuatorInfo = this->eventForceActuatorInfo;
}

void M1M3SSPublisher::tryLogForceActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                this->eventForceActuatorInfo.referenceId[i] !=
                        this->previousEventForceActuatorInfo.referenceId[i] ||
                this->eventForceActuatorInfo.xDataReferenceId[i] !=
                        this->previousEventForceActuatorInfo.xDataReferenceId[i] ||
                this->eventForceActuatorInfo.yDataReferenceId[i] !=
                        this->previousEventForceActuatorInfo.yDataReferenceId[i] ||
                this->eventForceActuatorInfo.zDataReferenceId[i] !=
                        this->previousEventForceActuatorInfo.zDataReferenceId[i] ||
                this->eventForceActuatorInfo.actuatorType[i] !=
                        this->previousEventForceActuatorInfo.actuatorType[i] ||
                this->eventForceActuatorInfo.actuatorOrientation[i] !=
                        this->previousEventForceActuatorInfo.actuatorOrientation[i] ||
                this->eventForceActuatorInfo.modbusSubnet[i] !=
                        this->previousEventForceActuatorInfo.modbusSubnet[i] ||
                this->eventForceActuatorInfo.modbusAddress[i] !=
                        this->previousEventForceActuatorInfo.modbusAddress[i] ||
                this->eventForceActuatorInfo.xPosition[i] !=
                        this->previousEventForceActuatorInfo.xPosition[i] ||
                this->eventForceActuatorInfo.yPosition[i] !=
                        this->previousEventForceActuatorInfo.yPosition[i] ||
                this->eventForceActuatorInfo.zPosition[i] !=
                        this->previousEventForceActuatorInfo.zPosition[i] ||
                this->eventForceActuatorInfo.ilcUniqueId[i] !=
                        this->previousEventForceActuatorInfo.ilcUniqueId[i] ||
                this->eventForceActuatorInfo.ilcApplicationType[i] !=
                        this->previousEventForceActuatorInfo.ilcApplicationType[i] ||
                this->eventForceActuatorInfo.networkNodeType[i] !=
                        this->previousEventForceActuatorInfo.networkNodeType[i] ||
                this->eventForceActuatorInfo.ilcSelectedOptions[i] !=
                        this->previousEventForceActuatorInfo.ilcSelectedOptions[i] ||
                this->eventForceActuatorInfo.networkNodeOptions[i] !=
                        this->previousEventForceActuatorInfo.networkNodeOptions[i] ||
                this->eventForceActuatorInfo.majorRevision[i] !=
                        this->previousEventForceActuatorInfo.majorRevision[i] ||
                this->eventForceActuatorInfo.minorRevision[i] !=
                        this->previousEventForceActuatorInfo.minorRevision[i] ||
                this->eventForceActuatorInfo.adcScanRate[i] !=
                        this->previousEventForceActuatorInfo.adcScanRate[i] ||
                this->eventForceActuatorInfo.mainPrimaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.mainPrimaryCylinderCoefficient[i] ||
                this->eventForceActuatorInfo.mainSecondaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.mainSecondaryCylinderCoefficient[i] ||
                this->eventForceActuatorInfo.mainPrimaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.mainPrimaryCylinderLoadCellOffset[i] ||
                this->eventForceActuatorInfo.mainSecondaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.mainSecondaryCylinderLoadCellOffset[i] ||
                this->eventForceActuatorInfo.mainPrimaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.mainPrimaryCylinderLoadCellSensitivity[i] ||
                this->eventForceActuatorInfo.mainSecondaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.mainSecondaryCylinderLoadCellSensitivity[i] ||
                this->eventForceActuatorInfo.backupPrimaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.backupPrimaryCylinderCoefficient[i] ||
                this->eventForceActuatorInfo.backupSecondaryCylinderCoefficient[i] !=
                        this->previousEventForceActuatorInfo.backupSecondaryCylinderCoefficient[i] ||
                this->eventForceActuatorInfo.backupPrimaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.backupPrimaryCylinderLoadCellOffset[i] ||
                this->eventForceActuatorInfo.backupSecondaryCylinderLoadCellOffset[i] !=
                        this->previousEventForceActuatorInfo.backupSecondaryCylinderLoadCellOffset[i] ||
                this->eventForceActuatorInfo.backupPrimaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.backupPrimaryCylinderLoadCellSensitivity[i] ||
                this->eventForceActuatorInfo.backupSecondaryCylinderLoadCellSensitivity[i] !=
                        this->previousEventForceActuatorInfo.backupSecondaryCylinderLoadCellSensitivity[i] ||
                this->eventForceActuatorInfo.mezzaninePrimaryCylinderGain[i] !=
                        this->previousEventForceActuatorInfo.mezzaninePrimaryCylinderGain[i] ||
                this->eventForceActuatorInfo.mezzanineSecondaryCylinderGain[i] !=
                        this->previousEventForceActuatorInfo.mezzanineSecondaryCylinderGain[i] ||
                this->eventForceActuatorInfo.mezzanineUniqueId[i] !=
                        this->previousEventForceActuatorInfo.mezzanineUniqueId[i] ||
                this->eventForceActuatorInfo.mezzanineFirmwareType[i] !=
                        this->previousEventForceActuatorInfo.mezzanineFirmwareType[i] ||
                this->eventForceActuatorInfo.mezzanineMajorRevision[i] !=
                        this->previousEventForceActuatorInfo.mezzanineMajorRevision[i] ||
                this->eventForceActuatorInfo.mezzanineMinorRevision[i] !=
                        this->previousEventForceActuatorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        this->logForceActuatorInfo();
    }
}

void M1M3SSPublisher::logForceActuatorState() {
    this->m1m3SAL->logEvent_forceActuatorState(&this->eventForceActuatorState, 0);
    this->previousEventForceActuatorState = this->eventForceActuatorState;
}

void M1M3SSPublisher::tryLogForceActuatorState() {
    bool changeDetected =
            this->eventForceActuatorState.slewFlag != this->previousEventForceActuatorState.slewFlag ||
            this->eventForceActuatorState.staticForcesApplied !=
                    this->previousEventForceActuatorState.staticForcesApplied ||
            this->eventForceActuatorState.elevationForcesApplied !=
                    this->previousEventForceActuatorState.elevationForcesApplied ||
            this->eventForceActuatorState.azimuthForcesApplied !=
                    this->previousEventForceActuatorState.azimuthForcesApplied ||
            this->eventForceActuatorState.thermalForcesApplied !=
                    this->previousEventForceActuatorState.thermalForcesApplied ||
            this->eventForceActuatorState.offsetForcesApplied !=
                    this->previousEventForceActuatorState.offsetForcesApplied ||
            this->eventForceActuatorState.accelerationForcesApplied !=
                    this->previousEventForceActuatorState.accelerationForcesApplied ||
            this->eventForceActuatorState.velocityForcesApplied !=
                    this->previousEventForceActuatorState.velocityForcesApplied ||
            this->eventForceActuatorState.activeOpticForcesApplied !=
                    this->previousEventForceActuatorState.activeOpticForcesApplied ||
            this->eventForceActuatorState.aberrationForcesApplied !=
                    this->previousEventForceActuatorState.aberrationForcesApplied ||
            this->eventForceActuatorState.balanceForcesApplied !=
                    this->previousEventForceActuatorState.balanceForcesApplied ||
            this->eventForceActuatorState.supportPercentage !=
                    this->previousEventForceActuatorState.supportPercentage;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || this->eventForceActuatorState.ilcState[i] !=
                                                   this->previousEventForceActuatorState.ilcState[i];
    }
    if (changeDetected) {
        this->logForceActuatorState();
    }
}

void M1M3SSPublisher::logForceActuatorWarning() {
    this->eventForceActuatorWarning.anyMajorFault = false;
    this->eventForceActuatorWarning.anyMinorFault = false;
    this->eventForceActuatorWarning.anyFaultOverride = false;
    this->eventForceActuatorWarning.anyMainCalibrationError = false;
    this->eventForceActuatorWarning.anyBackupCalibrationError = false;
    this->eventForceActuatorWarning.anyMezzanineError = false;
    this->eventForceActuatorWarning.anyMezzanineBootloaderActive = false;
    this->eventForceActuatorWarning.anyUniqueIdCRCError = false;
    this->eventForceActuatorWarning.anyApplicationTypeMismatch = false;
    this->eventForceActuatorWarning.anyApplicationMissing = false;
    this->eventForceActuatorWarning.anyApplicationCRCMismatch = false;
    this->eventForceActuatorWarning.anyOneWireMissing = false;
    this->eventForceActuatorWarning.anyOneWire1Mismatch = false;
    this->eventForceActuatorWarning.anyOneWire2Mismatch = false;
    this->eventForceActuatorWarning.anyWatchdogReset = false;
    this->eventForceActuatorWarning.anyBrownOut = false;
    this->eventForceActuatorWarning.anyEventTrapReset = false;
    this->eventForceActuatorWarning.anySSRPowerFault = false;
    this->eventForceActuatorWarning.anyAuxPowerFault = false;
    this->eventForceActuatorWarning.anyMezzaninePowerFault = false;
    this->eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault = false;
    this->eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault = false;
    this->eventForceActuatorWarning.anyMezzanineUniqueIdCRCError = false;
    this->eventForceActuatorWarning.anyMezzanineMainCalibrationError = false;
    this->eventForceActuatorWarning.anyMezzanineBackupCalibrationError = false;
    this->eventForceActuatorWarning.anyMezzanineEventTrapReset = false;
    this->eventForceActuatorWarning.anyMezzanineApplicationMissing = false;
    this->eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch = false;
    this->eventForceActuatorWarning.anyILCFault = false;
    this->eventForceActuatorWarning.anyBroadcastCounterWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        this->eventForceActuatorWarning.anyMajorFault = this->eventForceActuatorWarning.anyMajorFault ||
                                                        this->eventForceActuatorWarning.majorFault[i];
        this->eventForceActuatorWarning.anyMinorFault = this->eventForceActuatorWarning.anyMinorFault ||
                                                        this->eventForceActuatorWarning.minorFault[i];
        this->eventForceActuatorWarning.anyFaultOverride = this->eventForceActuatorWarning.anyFaultOverride ||
                                                           this->eventForceActuatorWarning.faultOverride[i];
        this->eventForceActuatorWarning.anyMainCalibrationError =
                this->eventForceActuatorWarning.anyMainCalibrationError ||
                this->eventForceActuatorWarning.mainCalibrationError[i];
        this->eventForceActuatorWarning.anyBackupCalibrationError =
                this->eventForceActuatorWarning.anyBackupCalibrationError ||
                this->eventForceActuatorWarning.backupCalibrationError[i];
        this->eventForceActuatorWarning.anyMezzanineError =
                this->eventForceActuatorWarning.anyMezzanineError ||
                this->eventForceActuatorWarning.mezzanineError[i];
        this->eventForceActuatorWarning.anyMezzanineBootloaderActive =
                this->eventForceActuatorWarning.anyMezzanineBootloaderActive ||
                this->eventForceActuatorWarning.mezzanineBootloaderActive[i];
        this->eventForceActuatorWarning.anyUniqueIdCRCError =
                this->eventForceActuatorWarning.anyUniqueIdCRCError ||
                this->eventForceActuatorWarning.uniqueIdCRCError[i];
        this->eventForceActuatorWarning.anyApplicationTypeMismatch =
                this->eventForceActuatorWarning.anyApplicationTypeMismatch ||
                this->eventForceActuatorWarning.applicationTypeMismatch[i];
        this->eventForceActuatorWarning.anyApplicationMissing =
                this->eventForceActuatorWarning.anyApplicationMissing ||
                this->eventForceActuatorWarning.applicationMissing[i];
        this->eventForceActuatorWarning.anyApplicationCRCMismatch =
                this->eventForceActuatorWarning.anyApplicationCRCMismatch ||
                this->eventForceActuatorWarning.applicationCRCMismatch[i];
        this->eventForceActuatorWarning.anyOneWireMissing =
                this->eventForceActuatorWarning.anyOneWireMissing ||
                this->eventForceActuatorWarning.oneWireMissing[i];
        this->eventForceActuatorWarning.anyOneWire1Mismatch =
                this->eventForceActuatorWarning.anyOneWire1Mismatch ||
                this->eventForceActuatorWarning.oneWire1Mismatch[i];
        this->eventForceActuatorWarning.anyOneWire2Mismatch =
                this->eventForceActuatorWarning.anyOneWire2Mismatch ||
                this->eventForceActuatorWarning.oneWire2Mismatch[i];
        this->eventForceActuatorWarning.anyWatchdogReset = this->eventForceActuatorWarning.anyWatchdogReset ||
                                                           this->eventForceActuatorWarning.watchdogReset[i];
        this->eventForceActuatorWarning.anyBrownOut =
                this->eventForceActuatorWarning.anyBrownOut || this->eventForceActuatorWarning.brownOut[i];
        this->eventForceActuatorWarning.anyEventTrapReset =
                this->eventForceActuatorWarning.anyEventTrapReset ||
                this->eventForceActuatorWarning.eventTrapReset[i];
        this->eventForceActuatorWarning.anySSRPowerFault = this->eventForceActuatorWarning.anySSRPowerFault ||
                                                           this->eventForceActuatorWarning.ssrPowerFault[i];
        this->eventForceActuatorWarning.anyAuxPowerFault = this->eventForceActuatorWarning.anyAuxPowerFault ||
                                                           this->eventForceActuatorWarning.auxPowerFault[i];
        this->eventForceActuatorWarning.anyMezzaninePowerFault =
                this->eventForceActuatorWarning.anyMezzaninePowerFault ||
                this->eventForceActuatorWarning.mezzaninePowerFault[i];
        this->eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault =
                this->eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault ||
                this->eventForceActuatorWarning.mezzanineCurrentAmp1Fault[i];
        this->eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault =
                this->eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault ||
                this->eventForceActuatorWarning.mezzanineCurrentAmp2Fault[i];
        this->eventForceActuatorWarning.anyMezzanineUniqueIdCRCError =
                this->eventForceActuatorWarning.anyMezzanineUniqueIdCRCError ||
                this->eventForceActuatorWarning.mezzanineUniqueIdCRCError[i];
        this->eventForceActuatorWarning.anyMezzanineMainCalibrationError =
                this->eventForceActuatorWarning.anyMezzanineMainCalibrationError ||
                this->eventForceActuatorWarning.mezzanineMainCalibrationError[i];
        this->eventForceActuatorWarning.anyMezzanineBackupCalibrationError =
                this->eventForceActuatorWarning.anyMezzanineBackupCalibrationError ||
                this->eventForceActuatorWarning.mezzanineBackupCalibrationError[i];
        this->eventForceActuatorWarning.anyMezzanineEventTrapReset =
                this->eventForceActuatorWarning.anyMezzanineEventTrapReset ||
                this->eventForceActuatorWarning.mezzanineEventTrapReset[i];
        this->eventForceActuatorWarning.anyMezzanineApplicationMissing =
                this->eventForceActuatorWarning.anyMezzanineApplicationMissing ||
                this->eventForceActuatorWarning.mezzanineApplicationMissing[i];
        this->eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch =
                this->eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch ||
                this->eventForceActuatorWarning.mezzanineApplicationCRCMismatch[i];
        this->eventForceActuatorWarning.anyILCFault =
                this->eventForceActuatorWarning.anyILCFault || this->eventForceActuatorWarning.ilcFault[i];
        this->eventForceActuatorWarning.anyBroadcastCounterWarning =
                this->eventForceActuatorWarning.anyBroadcastCounterWarning ||
                this->eventForceActuatorWarning.broadcastCounterWarning[i];
    }
    this->eventForceActuatorWarning.anyWarning =
            this->eventForceActuatorWarning.anyMajorFault || this->eventForceActuatorWarning.anyMinorFault ||
            this->eventForceActuatorWarning.anyFaultOverride ||
            this->eventForceActuatorWarning.anyMainCalibrationError ||
            this->eventForceActuatorWarning.anyBackupCalibrationError ||
            this->eventForceActuatorWarning.anyMezzanineError ||
            this->eventForceActuatorWarning.anyMezzanineBootloaderActive ||
            this->eventForceActuatorWarning.anyUniqueIdCRCError ||
            this->eventForceActuatorWarning.anyApplicationTypeMismatch ||
            this->eventForceActuatorWarning.anyApplicationMissing ||
            this->eventForceActuatorWarning.anyApplicationCRCMismatch ||
            this->eventForceActuatorWarning.anyOneWireMissing ||
            this->eventForceActuatorWarning.anyOneWire1Mismatch ||
            this->eventForceActuatorWarning.anyOneWire2Mismatch ||
            this->eventForceActuatorWarning.anyWatchdogReset || this->eventForceActuatorWarning.anyBrownOut ||
            this->eventForceActuatorWarning.anyEventTrapReset ||
            this->eventForceActuatorWarning.anySSRPowerFault ||
            this->eventForceActuatorWarning.anyAuxPowerFault ||
            this->eventForceActuatorWarning.anyMezzaninePowerFault ||
            this->eventForceActuatorWarning.anyMezzanineCurrentAmp1Fault ||
            this->eventForceActuatorWarning.anyMezzanineCurrentAmp2Fault ||
            this->eventForceActuatorWarning.anyMezzanineUniqueIdCRCError ||
            this->eventForceActuatorWarning.anyMezzanineMainCalibrationError ||
            this->eventForceActuatorWarning.anyMezzanineBackupCalibrationError ||
            this->eventForceActuatorWarning.anyMezzanineEventTrapReset ||
            this->eventForceActuatorWarning.anyMezzanineApplicationMissing ||
            this->eventForceActuatorWarning.anyMezzanineApplicationCRCMismatch ||
            this->eventForceActuatorWarning.anyILCFault ||
            this->eventForceActuatorWarning.anyBroadcastCounterWarning;
    this->m1m3SAL->logEvent_forceActuatorWarning(&this->eventForceActuatorWarning, 0);
    this->previousEventForceActuatorWarning = this->eventForceActuatorWarning;
}

void M1M3SSPublisher::tryLogForceActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorWarning.majorFault[i] !=
                                 this->previousEventForceActuatorWarning.majorFault[i] ||
                         this->eventForceActuatorWarning.minorFault[i] !=
                                 this->previousEventForceActuatorWarning.minorFault[i] ||
                         this->eventForceActuatorWarning.faultOverride[i] !=
                                 this->previousEventForceActuatorWarning.faultOverride[i] ||
                         this->eventForceActuatorWarning.mainCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.mainCalibrationError[i] ||
                         this->eventForceActuatorWarning.backupCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.backupCalibrationError[i] ||
                         this->eventForceActuatorWarning.mezzanineError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineError[i] ||
                         this->eventForceActuatorWarning.mezzanineBootloaderActive[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineBootloaderActive[i] ||
                         this->eventForceActuatorWarning.uniqueIdCRCError[i] !=
                                 this->previousEventForceActuatorWarning.uniqueIdCRCError[i] ||
                         this->eventForceActuatorWarning.applicationTypeMismatch[i] !=
                                 this->previousEventForceActuatorWarning.applicationTypeMismatch[i] ||
                         this->eventForceActuatorWarning.applicationMissing[i] !=
                                 this->previousEventForceActuatorWarning.applicationMissing[i] ||
                         this->eventForceActuatorWarning.applicationCRCMismatch[i] !=
                                 this->previousEventForceActuatorWarning.applicationCRCMismatch[i] ||
                         this->eventForceActuatorWarning.oneWireMissing[i] !=
                                 this->previousEventForceActuatorWarning.oneWireMissing[i] ||
                         this->eventForceActuatorWarning.oneWire1Mismatch[i] !=
                                 this->previousEventForceActuatorWarning.oneWire1Mismatch[i] ||
                         this->eventForceActuatorWarning.oneWire2Mismatch[i] !=
                                 this->previousEventForceActuatorWarning.oneWire2Mismatch[i] ||
                         this->eventForceActuatorWarning.watchdogReset[i] !=
                                 this->previousEventForceActuatorWarning.watchdogReset[i] ||
                         this->eventForceActuatorWarning.brownOut[i] !=
                                 this->previousEventForceActuatorWarning.brownOut[i] ||
                         this->eventForceActuatorWarning.eventTrapReset[i] !=
                                 this->previousEventForceActuatorWarning.eventTrapReset[i] ||
                         this->eventForceActuatorWarning.ssrPowerFault[i] !=
                                 this->previousEventForceActuatorWarning.ssrPowerFault[i] ||
                         this->eventForceActuatorWarning.auxPowerFault[i] !=
                                 this->previousEventForceActuatorWarning.auxPowerFault[i] ||
                         this->eventForceActuatorWarning.mezzaninePowerFault[i] !=
                                 this->previousEventForceActuatorWarning.mezzaninePowerFault[i] ||
                         this->eventForceActuatorWarning.mezzanineCurrentAmp1Fault[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineCurrentAmp1Fault[i] ||
                         this->eventForceActuatorWarning.mezzanineCurrentAmp2Fault[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineCurrentAmp2Fault[i] ||
                         this->eventForceActuatorWarning.mezzanineUniqueIdCRCError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineUniqueIdCRCError[i] ||
                         this->eventForceActuatorWarning.mezzanineMainCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineMainCalibrationError[i] ||
                         this->eventForceActuatorWarning.mezzanineBackupCalibrationError[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineBackupCalibrationError[i] ||
                         this->eventForceActuatorWarning.mezzanineEventTrapReset[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineEventTrapReset[i] ||
                         this->eventForceActuatorWarning.mezzanineApplicationMissing[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineApplicationMissing[i] ||
                         this->eventForceActuatorWarning.mezzanineApplicationCRCMismatch[i] !=
                                 this->previousEventForceActuatorWarning.mezzanineApplicationCRCMismatch[i] ||
                         this->eventForceActuatorWarning.ilcFault[i] !=
                                 this->previousEventForceActuatorWarning.ilcFault[i] ||
                         this->eventForceActuatorWarning.broadcastCounterWarning[i] !=
                                 this->previousEventForceActuatorWarning.broadcastCounterWarning[i];
    }
    if (changeDetected) {
        this->logForceActuatorForceWarning();
    }
}

void M1M3SSPublisher::logForceSetpointWarning() {
    this->eventForceSetpointWarning.anySafetyLimitWarning = false;
    this->eventForceSetpointWarning.anyNearNeighborWarning = false;
    this->eventForceSetpointWarning.anyFarNeighborWarning = false;
    this->eventForceSetpointWarning.anyElevationForceWarning = false;
    this->eventForceSetpointWarning.anyAzimuthForceWarning = false;
    this->eventForceSetpointWarning.anyThermalForceWarning = false;
    this->eventForceSetpointWarning.anyBalanceForceWarning = false;
    this->eventForceSetpointWarning.anyAccelerationForceWarning = false;
    this->eventForceSetpointWarning.anyVelocityForceWarning = false;
    this->eventForceSetpointWarning.anyActiveOpticForceWarning = false;
    this->eventForceSetpointWarning.anyStaticForceWarning = false;
    this->eventForceSetpointWarning.anyAberrationForceWarning = false;
    this->eventForceSetpointWarning.anyOffsetForceWarning = false;
    this->eventForceSetpointWarning.anyForceWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        this->eventForceSetpointWarning.anySafetyLimitWarning =
                this->eventForceSetpointWarning.anySafetyLimitWarning ||
                this->eventForceSetpointWarning.safetyLimitWarning[i];
        this->eventForceSetpointWarning.anyNearNeighborWarning =
                this->eventForceSetpointWarning.anyNearNeighborWarning ||
                this->eventForceSetpointWarning.nearNeighborWarning[i];
        this->eventForceSetpointWarning.anyFarNeighborWarning =
                this->eventForceSetpointWarning.anyFarNeighborWarning ||
                this->eventForceSetpointWarning.farNeighborWarning[i];
        this->eventForceSetpointWarning.anyElevationForceWarning =
                this->eventForceSetpointWarning.anyElevationForceWarning ||
                this->eventForceSetpointWarning.elevationForceWarning[i];
        this->eventForceSetpointWarning.anyAzimuthForceWarning =
                this->eventForceSetpointWarning.anyAzimuthForceWarning ||
                this->eventForceSetpointWarning.azimuthForceWarning[i];
        this->eventForceSetpointWarning.anyThermalForceWarning =
                this->eventForceSetpointWarning.anyThermalForceWarning ||
                this->eventForceSetpointWarning.thermalForceWarning[i];
        this->eventForceSetpointWarning.anyBalanceForceWarning =
                this->eventForceSetpointWarning.anyBalanceForceWarning ||
                this->eventForceSetpointWarning.balanceForceWarning[i];
        this->eventForceSetpointWarning.anyAccelerationForceWarning =
                this->eventForceSetpointWarning.anyAccelerationForceWarning ||
                this->eventForceSetpointWarning.accelerationForceWarning[i];
        this->eventForceSetpointWarning.anyVelocityForceWarning =
                this->eventForceSetpointWarning.anyVelocityForceWarning ||
                this->eventForceSetpointWarning.velocityForceWarning[i];
        this->eventForceSetpointWarning.anyActiveOpticForceWarning =
                this->eventForceSetpointWarning.anyActiveOpticForceWarning ||
                this->eventForceSetpointWarning.activeOpticForceWarning[i];
        this->eventForceSetpointWarning.anyStaticForceWarning =
                this->eventForceSetpointWarning.anyStaticForceWarning ||
                this->eventForceSetpointWarning.staticForceWarning[i];
        this->eventForceSetpointWarning.anyAberrationForceWarning =
                this->eventForceSetpointWarning.anyAberrationForceWarning ||
                this->eventForceSetpointWarning.aberrationForceWarning[i];
        this->eventForceSetpointWarning.anyOffsetForceWarning =
                this->eventForceSetpointWarning.anyOffsetForceWarning ||
                this->eventForceSetpointWarning.offsetForceWarning[i];
        this->eventForceSetpointWarning.anyForceWarning = this->eventForceSetpointWarning.anyForceWarning ||
                                                          this->eventForceSetpointWarning.forceWarning[i];
    }
    this->eventForceSetpointWarning.anyWarning =
            this->eventForceSetpointWarning.anySafetyLimitWarning ||
            this->eventForceSetpointWarning.xMomentWarning ||
            this->eventForceSetpointWarning.yMomentWarning ||
            this->eventForceSetpointWarning.zMomentWarning ||
            this->eventForceSetpointWarning.anyNearNeighborWarning ||
            this->eventForceSetpointWarning.magnitudeWarning ||
            this->eventForceSetpointWarning.anyFarNeighborWarning ||
            this->eventForceSetpointWarning.anyElevationForceWarning ||
            this->eventForceSetpointWarning.anyAzimuthForceWarning ||
            this->eventForceSetpointWarning.anyThermalForceWarning ||
            this->eventForceSetpointWarning.anyBalanceForceWarning ||
            this->eventForceSetpointWarning.anyVelocityForceWarning ||
            this->eventForceSetpointWarning.anyAccelerationForceWarning ||
            this->eventForceSetpointWarning.activeOpticNetForceWarning ||
            this->eventForceSetpointWarning.anyActiveOpticForceWarning ||
            this->eventForceSetpointWarning.anyStaticForceWarning ||
            this->eventForceSetpointWarning.aberrationNetForceWarning ||
            this->eventForceSetpointWarning.anyAberrationForceWarning ||
            this->eventForceSetpointWarning.anyOffsetForceWarning ||
            this->eventForceSetpointWarning.anyForceWarning;
    this->m1m3SAL->logEvent_forceSetpointWarning(&this->eventForceSetpointWarning, 0);
    this->previousEventForceSetpointWarning = this->eventForceSetpointWarning;
}

void M1M3SSPublisher::tryLogForceSetpointWarning() {
    bool changeDetected = this->eventForceSetpointWarning.xMomentWarning !=
                                  this->previousEventForceSetpointWarning.xMomentWarning ||
                          this->eventForceSetpointWarning.yMomentWarning !=
                                  this->previousEventForceSetpointWarning.yMomentWarning ||
                          this->eventForceSetpointWarning.zMomentWarning !=
                                  this->previousEventForceSetpointWarning.zMomentWarning ||
                          this->eventForceSetpointWarning.magnitudeWarning !=
                                  this->previousEventForceSetpointWarning.magnitudeWarning ||
                          this->eventForceSetpointWarning.activeOpticNetForceWarning !=
                                  this->previousEventForceSetpointWarning.activeOpticNetForceWarning ||
                          this->eventForceSetpointWarning.aberrationNetForceWarning !=
                                  this->previousEventForceSetpointWarning.aberrationNetForceWarning;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceSetpointWarning.safetyLimitWarning[i] !=
                                 this->previousEventForceSetpointWarning.safetyLimitWarning[i] ||
                         this->eventForceSetpointWarning.nearNeighborWarning[i] !=
                                 this->previousEventForceSetpointWarning.nearNeighborWarning[i] ||
                         this->eventForceSetpointWarning.farNeighborWarning[i] !=
                                 this->previousEventForceSetpointWarning.farNeighborWarning[i] ||
                         this->eventForceSetpointWarning.elevationForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.elevationForceWarning[i] ||
                         this->eventForceSetpointWarning.azimuthForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.azimuthForceWarning[i] ||
                         this->eventForceSetpointWarning.thermalForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.thermalForceWarning[i] ||
                         this->eventForceSetpointWarning.balanceForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.balanceForceWarning[i] ||
                         this->eventForceSetpointWarning.accelerationForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.accelerationForceWarning[i] ||
                         this->eventForceSetpointWarning.velocityForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.velocityForceWarning[i] ||
                         this->eventForceSetpointWarning.activeOpticForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.activeOpticForceWarning[i] ||
                         this->eventForceSetpointWarning.staticForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.staticForceWarning[i] ||
                         this->eventForceSetpointWarning.aberrationForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.aberrationForceWarning[i] ||
                         this->eventForceSetpointWarning.offsetForceWarning[i] !=
                                 this->previousEventForceSetpointWarning.offsetForceWarning[i] ||
                         this->eventForceSetpointWarning.forceWarning[i] !=
                                 this->previousEventForceSetpointWarning.forceWarning[i];
    }
    if (changeDetected) {
        this->logForceSetpointWarning();
    }
}

void M1M3SSPublisher::logGyroWarning() {
    this->eventGyroWarning.anyWarning =
            this->eventGyroWarning.gyroXStatusWarning || this->eventGyroWarning.gyroYStatusWarning ||
            this->eventGyroWarning.gyroZStatusWarning || this->eventGyroWarning.sequenceNumberWarning ||
            this->eventGyroWarning.crcMismatchWarning || this->eventGyroWarning.invalidLengthWarning ||
            this->eventGyroWarning.invalidHeaderWarning || this->eventGyroWarning.incompleteFrameWarning ||
            this->eventGyroWarning.gyroXSLDWarning || this->eventGyroWarning.gyroXMODDACWarning ||
            this->eventGyroWarning.gyroXPhaseWarning || this->eventGyroWarning.gyroXFlashWarning ||
            this->eventGyroWarning.gyroYSLDWarning || this->eventGyroWarning.gyroYMODDACWarning ||
            this->eventGyroWarning.gyroYPhaseWarning || this->eventGyroWarning.gyroYFlashWarning ||
            this->eventGyroWarning.gyroZSLDWarning || this->eventGyroWarning.gyroZMODDACWarning ||
            this->eventGyroWarning.gyroZPhaseWarning || this->eventGyroWarning.gyroZFlashWarning ||
            this->eventGyroWarning.gyroXSLDTemperatureStatusWarning ||
            this->eventGyroWarning.gyroYSLDTemperatureStatusWarning ||
            this->eventGyroWarning.gyroZSLDTemperatureStatusWarning ||
            this->eventGyroWarning.gcbTemperatureStatusWarning ||
            this->eventGyroWarning.temperatureStatusWarning ||
            this->eventGyroWarning.gcbDSPSPIFlashStatusWarning ||
            this->eventGyroWarning.gcbFPGASPIFlashStatusWarning ||
            this->eventGyroWarning.dspSPIFlashStatusWarning ||
            this->eventGyroWarning.fpgaSPIFlashStatusWarning || this->eventGyroWarning.gcb1_2VStatusWarning ||
            this->eventGyroWarning.gcb3_3VStatusWarning || this->eventGyroWarning.gcb5VStatusWarning ||
            this->eventGyroWarning.v1_2StatusWarning || this->eventGyroWarning.v3_3StatusWarning ||
            this->eventGyroWarning.v5StatusWarning || this->eventGyroWarning.gcbFPGAStatusWarning ||
            this->eventGyroWarning.fpgaStatusWarning || this->eventGyroWarning.hiSpeedSPORTStatusWarning ||
            this->eventGyroWarning.auxSPORTStatusWarning ||
            this->eventGyroWarning.sufficientSoftwareResourcesWarning ||
            this->eventGyroWarning.gyroEOVoltsPositiveWarning ||
            this->eventGyroWarning.gyroEOVoltsNegativeWarning || this->eventGyroWarning.gyroXVoltsWarning ||
            this->eventGyroWarning.gyroYVoltsWarning || this->eventGyroWarning.gyroZVoltsWarning ||
            this->eventGyroWarning.gcbADCCommsWarning || this->eventGyroWarning.mSYNCExternalTimingWarning;
    this->m1m3SAL->logEvent_gyroWarning(&this->eventGyroWarning, 0);
    this->previousEventGyroWarning = this->eventGyroWarning;
}

void M1M3SSPublisher::tryLogGyroWarning() {
    if (this->eventGyroWarning.gyroXStatusWarning != this->previousEventGyroWarning.gyroXStatusWarning ||
        this->eventGyroWarning.gyroYStatusWarning != this->previousEventGyroWarning.gyroYStatusWarning ||
        this->eventGyroWarning.gyroZStatusWarning != this->previousEventGyroWarning.gyroZStatusWarning ||
        this->eventGyroWarning.sequenceNumberWarning !=
                this->previousEventGyroWarning.sequenceNumberWarning ||
        this->eventGyroWarning.crcMismatchWarning != this->previousEventGyroWarning.crcMismatchWarning ||
        this->eventGyroWarning.invalidLengthWarning != this->previousEventGyroWarning.invalidLengthWarning ||
        this->eventGyroWarning.invalidHeaderWarning != this->previousEventGyroWarning.invalidHeaderWarning ||
        this->eventGyroWarning.incompleteFrameWarning !=
                this->previousEventGyroWarning.incompleteFrameWarning ||
        this->eventGyroWarning.gyroXSLDWarning != this->previousEventGyroWarning.gyroXSLDWarning ||
        this->eventGyroWarning.gyroXMODDACWarning != this->previousEventGyroWarning.gyroXMODDACWarning ||
        this->eventGyroWarning.gyroXPhaseWarning != this->previousEventGyroWarning.gyroXPhaseWarning ||
        this->eventGyroWarning.gyroXFlashWarning != this->previousEventGyroWarning.gyroXFlashWarning ||
        this->eventGyroWarning.gyroYSLDWarning != this->previousEventGyroWarning.gyroYSLDWarning ||
        this->eventGyroWarning.gyroYMODDACWarning != this->previousEventGyroWarning.gyroYMODDACWarning ||
        this->eventGyroWarning.gyroYPhaseWarning != this->previousEventGyroWarning.gyroYPhaseWarning ||
        this->eventGyroWarning.gyroYFlashWarning != this->previousEventGyroWarning.gyroYFlashWarning ||
        this->eventGyroWarning.gyroZSLDWarning != this->previousEventGyroWarning.gyroZSLDWarning ||
        this->eventGyroWarning.gyroZMODDACWarning != this->previousEventGyroWarning.gyroZMODDACWarning ||
        this->eventGyroWarning.gyroZPhaseWarning != this->previousEventGyroWarning.gyroZPhaseWarning ||
        this->eventGyroWarning.gyroZFlashWarning != this->previousEventGyroWarning.gyroZFlashWarning ||
        this->eventGyroWarning.gyroXSLDTemperatureStatusWarning !=
                this->previousEventGyroWarning.gyroXSLDTemperatureStatusWarning ||
        this->eventGyroWarning.gyroYSLDTemperatureStatusWarning !=
                this->previousEventGyroWarning.gyroYSLDTemperatureStatusWarning ||
        this->eventGyroWarning.gyroZSLDTemperatureStatusWarning !=
                this->previousEventGyroWarning.gyroZSLDTemperatureStatusWarning ||
        this->eventGyroWarning.gcbTemperatureStatusWarning !=
                this->previousEventGyroWarning.gcbTemperatureStatusWarning ||
        this->eventGyroWarning.temperatureStatusWarning !=
                this->previousEventGyroWarning.temperatureStatusWarning ||
        this->eventGyroWarning.gcbDSPSPIFlashStatusWarning !=
                this->previousEventGyroWarning.gcbDSPSPIFlashStatusWarning ||
        this->eventGyroWarning.gcbFPGASPIFlashStatusWarning !=
                this->previousEventGyroWarning.gcbFPGASPIFlashStatusWarning ||
        this->eventGyroWarning.dspSPIFlashStatusWarning !=
                this->previousEventGyroWarning.dspSPIFlashStatusWarning ||
        this->eventGyroWarning.fpgaSPIFlashStatusWarning !=
                this->previousEventGyroWarning.fpgaSPIFlashStatusWarning ||
        this->eventGyroWarning.gcb1_2VStatusWarning != this->previousEventGyroWarning.gcb1_2VStatusWarning ||
        this->eventGyroWarning.gcb3_3VStatusWarning != this->previousEventGyroWarning.gcb3_3VStatusWarning ||
        this->eventGyroWarning.gcb5VStatusWarning != this->previousEventGyroWarning.gcb5VStatusWarning ||
        this->eventGyroWarning.v1_2StatusWarning != this->previousEventGyroWarning.v1_2StatusWarning ||
        this->eventGyroWarning.v3_3StatusWarning != this->previousEventGyroWarning.v3_3StatusWarning ||
        this->eventGyroWarning.v5StatusWarning != this->previousEventGyroWarning.v5StatusWarning ||
        this->eventGyroWarning.gcbFPGAStatusWarning != this->previousEventGyroWarning.gcbFPGAStatusWarning ||
        this->eventGyroWarning.fpgaStatusWarning != this->previousEventGyroWarning.fpgaStatusWarning ||
        this->eventGyroWarning.hiSpeedSPORTStatusWarning !=
                this->previousEventGyroWarning.hiSpeedSPORTStatusWarning ||
        this->eventGyroWarning.auxSPORTStatusWarning !=
                this->previousEventGyroWarning.auxSPORTStatusWarning ||
        this->eventGyroWarning.sufficientSoftwareResourcesWarning !=
                this->previousEventGyroWarning.sufficientSoftwareResourcesWarning ||
        this->eventGyroWarning.gyroEOVoltsPositiveWarning !=
                this->previousEventGyroWarning.gyroEOVoltsPositiveWarning ||
        this->eventGyroWarning.gyroEOVoltsNegativeWarning !=
                this->previousEventGyroWarning.gyroEOVoltsNegativeWarning ||
        this->eventGyroWarning.gyroXVoltsWarning != this->previousEventGyroWarning.gyroXVoltsWarning ||
        this->eventGyroWarning.gyroYVoltsWarning != this->previousEventGyroWarning.gyroYVoltsWarning ||
        this->eventGyroWarning.gyroZVoltsWarning != this->previousEventGyroWarning.gyroZVoltsWarning ||
        this->eventGyroWarning.gcbADCCommsWarning != this->previousEventGyroWarning.gcbADCCommsWarning ||
        this->eventGyroWarning.mSYNCExternalTimingWarning !=
                this->previousEventGyroWarning.mSYNCExternalTimingWarning) {
        this->logGyroWarning();
    }
}

void M1M3SSPublisher::logHardpointActuatorInfo() {
    this->m1m3SAL->logEvent_hardpointActuatorInfo(&this->eventHardpointActuatorInfo, 0);
    this->previousEventHardpointActuatorInfo = this->eventHardpointActuatorInfo;
}

void M1M3SSPublisher::tryLogHardpointActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventHardpointActuatorInfo.referenceId[i] !=
                                 this->previousEventHardpointActuatorInfo.referenceId[i] ||
                         this->eventHardpointActuatorInfo.referencePosition[i] !=
                                 this->previousEventHardpointActuatorInfo.referencePosition[i] ||
                         this->eventHardpointActuatorInfo.modbusSubnet[i] !=
                                 this->previousEventHardpointActuatorInfo.modbusSubnet[i] ||
                         this->eventHardpointActuatorInfo.modbusAddress[i] !=
                                 this->previousEventHardpointActuatorInfo.modbusAddress[i] ||
                         this->eventHardpointActuatorInfo.xPosition[i] !=
                                 this->previousEventHardpointActuatorInfo.xPosition[i] ||
                         this->eventHardpointActuatorInfo.yPosition[i] !=
                                 this->previousEventHardpointActuatorInfo.yPosition[i] ||
                         this->eventHardpointActuatorInfo.zPosition[i] !=
                                 this->previousEventHardpointActuatorInfo.zPosition[i] ||
                         this->eventHardpointActuatorInfo.ilcUniqueId[i] !=
                                 this->previousEventHardpointActuatorInfo.ilcUniqueId[i] ||
                         this->eventHardpointActuatorInfo.ilcApplicationType[i] !=
                                 this->previousEventHardpointActuatorInfo.ilcApplicationType[i] ||
                         this->eventHardpointActuatorInfo.networkNodeType[i] !=
                                 this->previousEventHardpointActuatorInfo.networkNodeType[i] ||
                         this->eventHardpointActuatorInfo.ilcSelectedOptions[i] !=
                                 this->previousEventHardpointActuatorInfo.ilcSelectedOptions[i] ||
                         this->eventHardpointActuatorInfo.networkNodeOptions[i] !=
                                 this->previousEventHardpointActuatorInfo.networkNodeOptions[i] ||
                         this->eventHardpointActuatorInfo.majorRevision[i] !=
                                 this->previousEventHardpointActuatorInfo.majorRevision[i] ||
                         this->eventHardpointActuatorInfo.minorRevision[i] !=
                                 this->previousEventHardpointActuatorInfo.minorRevision[i] ||
                         this->eventHardpointActuatorInfo.adcScanRate[i] !=
                                 this->previousEventHardpointActuatorInfo.adcScanRate[i] ||
                         this->eventHardpointActuatorInfo.mainLoadCellCoefficient[i] !=
                                 this->previousEventHardpointActuatorInfo.mainLoadCellCoefficient[i] ||
                         this->eventHardpointActuatorInfo.mainLoadCellOffset[i] !=
                                 this->previousEventHardpointActuatorInfo.mainLoadCellOffset[i] ||
                         this->eventHardpointActuatorInfo.mainLoadCellSensitivity[i] !=
                                 this->previousEventHardpointActuatorInfo.mainLoadCellSensitivity[i] ||
                         this->eventHardpointActuatorInfo.backupLoadCellCoefficient[i] !=
                                 this->previousEventHardpointActuatorInfo.backupLoadCellCoefficient[i] ||
                         this->eventHardpointActuatorInfo.backupLoadCellOffset[i] !=
                                 this->previousEventHardpointActuatorInfo.backupLoadCellOffset[i] ||
                         this->eventHardpointActuatorInfo.backupLoadCellSensitivity[i] !=
                                 this->previousEventHardpointActuatorInfo.backupLoadCellSensitivity[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorInfo();
    }
}

void M1M3SSPublisher::logHardpointActuatorState() {
    this->m1m3SAL->logEvent_hardpointActuatorState(&this->eventHardpointActuatorState, 0);
    this->previousEventHardpointActuatorState = this->eventHardpointActuatorState;
}

void M1M3SSPublisher::tryLogHardpointActuatorState() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventHardpointActuatorState.ilcState[i] !=
                                 this->previousEventHardpointActuatorState.ilcState[i] ||
                         this->eventHardpointActuatorState.motionState[i] !=
                                 this->previousEventHardpointActuatorState.motionState[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorState();
    }
}

void M1M3SSPublisher::logHardpointActuatorWarning() {
    this->eventHardpointActuatorWarning.anyMajorFault = false;
    this->eventHardpointActuatorWarning.anyMinorFault = false;
    this->eventHardpointActuatorWarning.anyFaultOverride = false;
    this->eventHardpointActuatorWarning.anyMainCalibrationError = false;
    this->eventHardpointActuatorWarning.anyBackupCalibrationError = false;
    this->eventHardpointActuatorWarning.anyLimitSwitch1Operated = false;
    this->eventHardpointActuatorWarning.anyLimitSwitch2Operated = false;
    this->eventHardpointActuatorWarning.anyUniqueIdCRCError = false;
    this->eventHardpointActuatorWarning.anyApplicationTypeMismatch = false;
    this->eventHardpointActuatorWarning.anyApplicationMissing = false;
    this->eventHardpointActuatorWarning.anyApplicationCRCMismatch = false;
    this->eventHardpointActuatorWarning.anyOneWireMissing = false;
    this->eventHardpointActuatorWarning.anyOneWire1Mismatch = false;
    this->eventHardpointActuatorWarning.anyOneWire2Mismatch = false;
    this->eventHardpointActuatorWarning.anyWatchdogReset = false;
    this->eventHardpointActuatorWarning.anyBrownOut = false;
    this->eventHardpointActuatorWarning.anyEventTrapReset = false;
    this->eventHardpointActuatorWarning.anyMotorDriverFault = false;
    this->eventHardpointActuatorWarning.anySSRPowerFault = false;
    this->eventHardpointActuatorWarning.anyAuxPowerFault = false;
    this->eventHardpointActuatorWarning.anySMCPowerFault = false;
    this->eventHardpointActuatorWarning.anyILCFault = false;
    this->eventHardpointActuatorWarning.anyBroadcastCounterWarning = false;
    for (int i = 0; i < HP_COUNT; ++i) {
        this->eventHardpointActuatorWarning.anyMajorFault =
                this->eventHardpointActuatorWarning.anyMajorFault ||
                this->eventHardpointActuatorWarning.majorFault[i];
        this->eventHardpointActuatorWarning.anyMinorFault =
                this->eventHardpointActuatorWarning.anyMinorFault ||
                this->eventHardpointActuatorWarning.minorFault[i];
        this->eventHardpointActuatorWarning.anyFaultOverride =
                this->eventHardpointActuatorWarning.anyFaultOverride ||
                this->eventHardpointActuatorWarning.faultOverride[i];
        this->eventHardpointActuatorWarning.anyMainCalibrationError =
                this->eventHardpointActuatorWarning.anyMainCalibrationError ||
                this->eventHardpointActuatorWarning.mainCalibrationError[i];
        this->eventHardpointActuatorWarning.anyBackupCalibrationError =
                this->eventHardpointActuatorWarning.anyBackupCalibrationError ||
                this->eventHardpointActuatorWarning.backupCalibrationError[i];
        this->eventHardpointActuatorWarning.anyLimitSwitch1Operated =
                this->eventHardpointActuatorWarning.anyLimitSwitch1Operated ||
                this->eventHardpointActuatorWarning.limitSwitch1Operated[i];
        this->eventHardpointActuatorWarning.anyLimitSwitch2Operated =
                this->eventHardpointActuatorWarning.anyLimitSwitch2Operated ||
                this->eventHardpointActuatorWarning.limitSwitch2Operated[i];
        this->eventHardpointActuatorWarning.anyUniqueIdCRCError =
                this->eventHardpointActuatorWarning.anyUniqueIdCRCError ||
                this->eventHardpointActuatorWarning.uniqueIdCRCError[i];
        this->eventHardpointActuatorWarning.anyApplicationTypeMismatch =
                this->eventHardpointActuatorWarning.anyApplicationTypeMismatch ||
                this->eventHardpointActuatorWarning.applicationTypeMismatch[i];
        this->eventHardpointActuatorWarning.anyApplicationMissing =
                this->eventHardpointActuatorWarning.anyApplicationMissing ||
                this->eventHardpointActuatorWarning.applicationMissing[i];
        this->eventHardpointActuatorWarning.anyApplicationCRCMismatch =
                this->eventHardpointActuatorWarning.anyApplicationCRCMismatch ||
                this->eventHardpointActuatorWarning.applicationCRCMismatch[i];
        this->eventHardpointActuatorWarning.anyOneWireMissing =
                this->eventHardpointActuatorWarning.anyOneWireMissing ||
                this->eventHardpointActuatorWarning.oneWireMissing[i];
        this->eventHardpointActuatorWarning.anyOneWire1Mismatch =
                this->eventHardpointActuatorWarning.anyOneWire1Mismatch ||
                this->eventHardpointActuatorWarning.oneWire1Mismatch[i];
        this->eventHardpointActuatorWarning.anyOneWire2Mismatch =
                this->eventHardpointActuatorWarning.anyOneWire2Mismatch ||
                this->eventHardpointActuatorWarning.oneWire2Mismatch[i];
        this->eventHardpointActuatorWarning.anyWatchdogReset =
                this->eventHardpointActuatorWarning.anyWatchdogReset ||
                this->eventHardpointActuatorWarning.watchdogReset[i];
        this->eventHardpointActuatorWarning.anyBrownOut = this->eventHardpointActuatorWarning.anyBrownOut ||
                                                          this->eventHardpointActuatorWarning.brownOut[i];
        this->eventHardpointActuatorWarning.anyEventTrapReset =
                this->eventHardpointActuatorWarning.anyEventTrapReset ||
                this->eventHardpointActuatorWarning.eventTrapReset[i];
        this->eventHardpointActuatorWarning.anyMotorDriverFault =
                this->eventHardpointActuatorWarning.anyMotorDriverFault ||
                this->eventHardpointActuatorWarning.motorDriverFault[i];
        this->eventHardpointActuatorWarning.anySSRPowerFault =
                this->eventHardpointActuatorWarning.anySSRPowerFault ||
                this->eventHardpointActuatorWarning.ssrPowerFault[i];
        this->eventHardpointActuatorWarning.anyAuxPowerFault =
                this->eventHardpointActuatorWarning.anyAuxPowerFault ||
                this->eventHardpointActuatorWarning.auxPowerFault[i];
        this->eventHardpointActuatorWarning.anySMCPowerFault =
                this->eventHardpointActuatorWarning.anySMCPowerFault ||
                this->eventHardpointActuatorWarning.smcPowerFault[i];
        this->eventHardpointActuatorWarning.anyILCFault = this->eventHardpointActuatorWarning.anyILCFault ||
                                                          this->eventHardpointActuatorWarning.ilcFault[i];
        this->eventHardpointActuatorWarning.anyBroadcastCounterWarning =
                this->eventHardpointActuatorWarning.anyBroadcastCounterWarning ||
                this->eventHardpointActuatorWarning.broadcastCounterWarning[i];
    }
    this->eventHardpointActuatorWarning.anyWarning =
            this->eventHardpointActuatorWarning.anyMajorFault ||
            this->eventHardpointActuatorWarning.anyMinorFault ||
            this->eventHardpointActuatorWarning.anyFaultOverride ||
            this->eventHardpointActuatorWarning.anyMainCalibrationError ||
            this->eventHardpointActuatorWarning.anyBackupCalibrationError ||
            this->eventHardpointActuatorWarning.anyLimitSwitch1Operated ||
            this->eventHardpointActuatorWarning.anyLimitSwitch2Operated ||
            this->eventHardpointActuatorWarning.anyUniqueIdCRCError ||
            this->eventHardpointActuatorWarning.anyApplicationTypeMismatch ||
            this->eventHardpointActuatorWarning.anyApplicationMissing ||
            this->eventHardpointActuatorWarning.anyApplicationCRCMismatch ||
            this->eventHardpointActuatorWarning.anyOneWireMissing ||
            this->eventHardpointActuatorWarning.anyOneWire1Mismatch ||
            this->eventHardpointActuatorWarning.anyOneWire2Mismatch ||
            this->eventHardpointActuatorWarning.anyWatchdogReset ||
            this->eventHardpointActuatorWarning.anyBrownOut ||
            this->eventHardpointActuatorWarning.anyEventTrapReset ||
            this->eventHardpointActuatorWarning.anyMotorDriverFault ||
            this->eventHardpointActuatorWarning.anySSRPowerFault ||
            this->eventHardpointActuatorWarning.anyAuxPowerFault ||
            this->eventHardpointActuatorWarning.anySMCPowerFault ||
            this->eventHardpointActuatorWarning.anyILCFault ||
            this->eventHardpointActuatorWarning.anyBroadcastCounterWarning;
    this->m1m3SAL->logEvent_hardpointActuatorWarning(&this->eventHardpointActuatorWarning, 0);
    this->previousEventHardpointActuatorWarning = this->eventHardpointActuatorWarning;
}

void M1M3SSPublisher::tryLogHardpointActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventHardpointActuatorWarning.majorFault[i] !=
                                 this->previousEventHardpointActuatorWarning.majorFault[i] ||
                         this->eventHardpointActuatorWarning.minorFault[i] !=
                                 this->previousEventHardpointActuatorWarning.minorFault[i] ||
                         this->eventHardpointActuatorWarning.faultOverride[i] !=
                                 this->previousEventHardpointActuatorWarning.faultOverride[i] ||
                         this->eventHardpointActuatorWarning.mainCalibrationError[i] !=
                                 this->previousEventHardpointActuatorWarning.mainCalibrationError[i] ||
                         this->eventHardpointActuatorWarning.backupCalibrationError[i] !=
                                 this->previousEventHardpointActuatorWarning.backupCalibrationError[i] ||
                         this->eventHardpointActuatorWarning.limitSwitch1Operated[i] !=
                                 this->previousEventHardpointActuatorWarning.limitSwitch1Operated[i] ||
                         this->eventHardpointActuatorWarning.limitSwitch2Operated[i] !=
                                 this->previousEventHardpointActuatorWarning.limitSwitch2Operated[i] ||
                         this->eventHardpointActuatorWarning.uniqueIdCRCError[i] !=
                                 this->previousEventHardpointActuatorWarning.uniqueIdCRCError[i] ||
                         this->eventHardpointActuatorWarning.applicationTypeMismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.applicationTypeMismatch[i] ||
                         this->eventHardpointActuatorWarning.applicationMissing[i] !=
                                 this->previousEventHardpointActuatorWarning.applicationMissing[i] ||
                         this->eventHardpointActuatorWarning.applicationCRCMismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.applicationCRCMismatch[i] ||
                         this->eventHardpointActuatorWarning.oneWireMissing[i] !=
                                 this->previousEventHardpointActuatorWarning.oneWireMissing[i] ||
                         this->eventHardpointActuatorWarning.oneWire1Mismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.oneWire1Mismatch[i] ||
                         this->eventHardpointActuatorWarning.oneWire2Mismatch[i] !=
                                 this->previousEventHardpointActuatorWarning.oneWire2Mismatch[i] ||
                         this->eventHardpointActuatorWarning.watchdogReset[i] !=
                                 this->previousEventHardpointActuatorWarning.watchdogReset[i] ||
                         this->eventHardpointActuatorWarning.brownOut[i] !=
                                 this->previousEventHardpointActuatorWarning.brownOut[i] ||
                         this->eventHardpointActuatorWarning.eventTrapReset[i] !=
                                 this->previousEventHardpointActuatorWarning.eventTrapReset[i] ||
                         this->eventHardpointActuatorWarning.motorDriverFault[i] !=
                                 this->previousEventHardpointActuatorWarning.motorDriverFault[i] ||
                         this->eventHardpointActuatorWarning.ssrPowerFault[i] !=
                                 this->previousEventHardpointActuatorWarning.ssrPowerFault[i] ||
                         this->eventHardpointActuatorWarning.auxPowerFault[i] !=
                                 this->previousEventHardpointActuatorWarning.auxPowerFault[i] ||
                         this->eventHardpointActuatorWarning.smcPowerFault[i] !=
                                 this->previousEventHardpointActuatorWarning.smcPowerFault[i] ||
                         this->eventHardpointActuatorWarning.ilcFault[i] !=
                                 this->previousEventHardpointActuatorWarning.ilcFault[i] ||
                         this->eventHardpointActuatorWarning.broadcastCounterWarning[i] !=
                                 this->previousEventHardpointActuatorWarning.broadcastCounterWarning[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorWarning();
    }
}

void M1M3SSPublisher::logHardpointMonitorInfo() {
    this->m1m3SAL->logEvent_hardpointMonitorInfo(&this->eventHardpointMonitorInfo, 0);
    this->previousEventHardpointMonitorInfo = this->eventHardpointMonitorInfo;
}

void M1M3SSPublisher::tryLogHardpointMonitorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventHardpointMonitorInfo.referenceId[i] !=
                                 this->previousEventHardpointMonitorInfo.referenceId[i] ||
                         this->eventHardpointMonitorInfo.modbusSubnet[i] !=
                                 this->previousEventHardpointMonitorInfo.modbusSubnet[i] ||
                         this->eventHardpointMonitorInfo.modbusAddress[i] !=
                                 this->previousEventHardpointMonitorInfo.modbusAddress[i] ||
                         this->eventHardpointMonitorInfo.ilcUniqueId[i] !=
                                 this->previousEventHardpointMonitorInfo.ilcUniqueId[i] ||
                         this->eventHardpointMonitorInfo.ilcApplicationType[i] !=
                                 this->previousEventHardpointMonitorInfo.ilcApplicationType[i] ||
                         this->eventHardpointMonitorInfo.networkNodeType[i] !=
                                 this->previousEventHardpointMonitorInfo.networkNodeType[i] ||
                         this->eventHardpointMonitorInfo.majorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.majorRevision[i] ||
                         this->eventHardpointMonitorInfo.minorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.minorRevision[i] ||
                         this->eventHardpointMonitorInfo.mezzanineUniqueId[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineUniqueId[i] ||
                         this->eventHardpointMonitorInfo.mezzanineFirmwareType[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineFirmwareType[i] ||
                         this->eventHardpointMonitorInfo.mezzanineMajorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineMajorRevision[i] ||
                         this->eventHardpointMonitorInfo.mezzanineMinorRevision[i] !=
                                 this->previousEventHardpointMonitorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorInfo();
    }
}

void M1M3SSPublisher::logHardpointMonitorState() {
    this->m1m3SAL->logEvent_hardpointMonitorState(&this->eventHardpointMonitorState, 0);
    this->previousEventHardpointMonitorState = this->eventHardpointMonitorState;
}

void M1M3SSPublisher::tryLogHardpointMonitorState() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || this->eventHardpointMonitorState.ilcState[i] !=
                                                   this->previousEventHardpointMonitorState.ilcState[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorState();
    }
}

void M1M3SSPublisher::logHardpointMonitorWarning() {
    this->eventHardpointMonitorWarning.anyMajorFault = false;
    this->eventHardpointMonitorWarning.anyMinorFault = false;
    this->eventHardpointMonitorWarning.anyFaultOverride = false;
    this->eventHardpointMonitorWarning.anyInstrumentError = false;
    this->eventHardpointMonitorWarning.anyMezzanineError = false;
    this->eventHardpointMonitorWarning.anyMezzanineBootloaderActive = false;
    this->eventHardpointMonitorWarning.anyUniqueIdCRCError = false;
    this->eventHardpointMonitorWarning.anyApplicationTypeMismatch = false;
    this->eventHardpointMonitorWarning.anyApplicationMissing = false;
    this->eventHardpointMonitorWarning.anyApplicationCRCMismatch = false;
    this->eventHardpointMonitorWarning.anyOneWireMissing = false;
    this->eventHardpointMonitorWarning.anyOneWire1Mismatch = false;
    this->eventHardpointMonitorWarning.anyOneWire2Mismatch = false;
    this->eventHardpointMonitorWarning.anyWatchdogReset = false;
    this->eventHardpointMonitorWarning.anyBrownOut = false;
    this->eventHardpointMonitorWarning.anyEventTrapReset = false;
    this->eventHardpointMonitorWarning.anySSRPowerFault = false;
    this->eventHardpointMonitorWarning.anyAuxPowerFault = false;
    this->eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault = false;
    this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault = false;
    this->eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault = false;
    this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault = false;
    this->eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError = false;
    this->eventHardpointMonitorWarning.anyMezzanineEventTrapReset = false;
    this->eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault = false;
    this->eventHardpointMonitorWarning.anyMezzanineApplicationMissing = false;
    this->eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch = false;
    for (int i = 0; i < HM_COUNT; ++i) {
        this->eventHardpointMonitorWarning.anyMajorFault = this->eventHardpointMonitorWarning.anyMajorFault ||
                                                           this->eventHardpointMonitorWarning.majorFault[i];
        this->eventHardpointMonitorWarning.anyMinorFault = this->eventHardpointMonitorWarning.anyMinorFault ||
                                                           this->eventHardpointMonitorWarning.minorFault[i];
        this->eventHardpointMonitorWarning.anyFaultOverride =
                this->eventHardpointMonitorWarning.anyFaultOverride ||
                this->eventHardpointMonitorWarning.faultOverride[i];
        this->eventHardpointMonitorWarning.anyInstrumentError =
                this->eventHardpointMonitorWarning.anyInstrumentError ||
                this->eventHardpointMonitorWarning.instrumentError[i];
        this->eventHardpointMonitorWarning.anyMezzanineError =
                this->eventHardpointMonitorWarning.anyMezzanineError ||
                this->eventHardpointMonitorWarning.mezzanineError[i];
        this->eventHardpointMonitorWarning.anyMezzanineBootloaderActive =
                this->eventHardpointMonitorWarning.anyMezzanineBootloaderActive ||
                this->eventHardpointMonitorWarning.mezzanineBootloaderActive[i];
        this->eventHardpointMonitorWarning.anyUniqueIdCRCError =
                this->eventHardpointMonitorWarning.anyUniqueIdCRCError ||
                this->eventHardpointMonitorWarning.uniqueIdCRCError[i];
        this->eventHardpointMonitorWarning.anyApplicationTypeMismatch =
                this->eventHardpointMonitorWarning.anyApplicationTypeMismatch ||
                this->eventHardpointMonitorWarning.applicationTypeMismatch[i];
        this->eventHardpointMonitorWarning.anyApplicationMissing =
                this->eventHardpointMonitorWarning.anyApplicationMissing ||
                this->eventHardpointMonitorWarning.applicationMissing[i];
        this->eventHardpointMonitorWarning.anyApplicationCRCMismatch =
                this->eventHardpointMonitorWarning.anyApplicationCRCMismatch ||
                this->eventHardpointMonitorWarning.applicationCRCMismatch[i];
        this->eventHardpointMonitorWarning.anyOneWireMissing =
                this->eventHardpointMonitorWarning.anyOneWireMissing ||
                this->eventHardpointMonitorWarning.oneWireMissing[i];
        this->eventHardpointMonitorWarning.anyOneWire1Mismatch =
                this->eventHardpointMonitorWarning.anyOneWire1Mismatch ||
                this->eventHardpointMonitorWarning.oneWire1Mismatch[i];
        this->eventHardpointMonitorWarning.anyOneWire2Mismatch =
                this->eventHardpointMonitorWarning.anyOneWire2Mismatch ||
                this->eventHardpointMonitorWarning.oneWire2Mismatch[i];
        this->eventHardpointMonitorWarning.anyWatchdogReset =
                this->eventHardpointMonitorWarning.anyWatchdogReset ||
                this->eventHardpointMonitorWarning.watchdogReset[i];
        this->eventHardpointMonitorWarning.anyBrownOut = this->eventHardpointMonitorWarning.anyBrownOut ||
                                                         this->eventHardpointMonitorWarning.brownOut[i];
        this->eventHardpointMonitorWarning.anyEventTrapReset =
                this->eventHardpointMonitorWarning.anyEventTrapReset ||
                this->eventHardpointMonitorWarning.eventTrapReset[i];
        this->eventHardpointMonitorWarning.anySSRPowerFault =
                this->eventHardpointMonitorWarning.anySSRPowerFault ||
                this->eventHardpointMonitorWarning.ssrPowerFault[i];
        this->eventHardpointMonitorWarning.anyAuxPowerFault =
                this->eventHardpointMonitorWarning.anyAuxPowerFault ||
                this->eventHardpointMonitorWarning.auxPowerFault[i];
        this->eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault =
                this->eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault ||
                this->eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i];
        this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault =
                this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault ||
                this->eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i];
        this->eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault =
                this->eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault ||
                this->eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i];
        this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault =
                this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault ||
                this->eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i];
        this->eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError =
                this->eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError ||
                this->eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i];
        this->eventHardpointMonitorWarning.anyMezzanineEventTrapReset =
                this->eventHardpointMonitorWarning.anyMezzanineEventTrapReset ||
                this->eventHardpointMonitorWarning.mezzanineEventTrapReset[i];
        this->eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault =
                this->eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault ||
                this->eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i];
        this->eventHardpointMonitorWarning.anyMezzanineApplicationMissing =
                this->eventHardpointMonitorWarning.anyMezzanineApplicationMissing ||
                this->eventHardpointMonitorWarning.mezzanineApplicationMissing[i];
        this->eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch =
                this->eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch ||
                this->eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    this->eventHardpointMonitorWarning.anyWarning =
            this->eventHardpointMonitorWarning.anyMajorFault ||
            this->eventHardpointMonitorWarning.anyMinorFault ||
            this->eventHardpointMonitorWarning.anyFaultOverride ||
            this->eventHardpointMonitorWarning.anyInstrumentError ||
            this->eventHardpointMonitorWarning.anyMezzanineError ||
            this->eventHardpointMonitorWarning.anyMezzanineBootloaderActive ||
            this->eventHardpointMonitorWarning.anyUniqueIdCRCError ||
            this->eventHardpointMonitorWarning.anyApplicationTypeMismatch ||
            this->eventHardpointMonitorWarning.anyApplicationMissing ||
            this->eventHardpointMonitorWarning.anyApplicationCRCMismatch ||
            this->eventHardpointMonitorWarning.anyOneWireMissing ||
            this->eventHardpointMonitorWarning.anyOneWire1Mismatch ||
            this->eventHardpointMonitorWarning.anyOneWire2Mismatch ||
            this->eventHardpointMonitorWarning.anyWatchdogReset ||
            this->eventHardpointMonitorWarning.anyBrownOut ||
            this->eventHardpointMonitorWarning.anyEventTrapReset ||
            this->eventHardpointMonitorWarning.anySSRPowerFault ||
            this->eventHardpointMonitorWarning.anyAuxPowerFault ||
            this->eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault ||
            this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault ||
            this->eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault ||
            this->eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault ||
            this->eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError ||
            this->eventHardpointMonitorWarning.anyMezzanineEventTrapReset ||
            this->eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault ||
            this->eventHardpointMonitorWarning.anyMezzanineApplicationMissing ||
            this->eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch;
    this->m1m3SAL->logEvent_hardpointMonitorWarning(&this->eventHardpointMonitorWarning, 0);
    this->previousEventHardpointMonitorWarning = this->eventHardpointMonitorWarning;
}

void M1M3SSPublisher::tryLogHardpointMonitorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                this->eventHardpointMonitorWarning.majorFault[i] !=
                        this->previousEventHardpointMonitorWarning.majorFault[i] ||
                this->eventHardpointMonitorWarning.minorFault[i] !=
                        this->previousEventHardpointMonitorWarning.minorFault[i] ||
                this->eventHardpointMonitorWarning.faultOverride[i] !=
                        this->previousEventHardpointMonitorWarning.faultOverride[i] ||
                this->eventHardpointMonitorWarning.instrumentError[i] !=
                        this->previousEventHardpointMonitorWarning.instrumentError[i] ||
                this->eventHardpointMonitorWarning.mezzanineError[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineError[i] ||
                this->eventHardpointMonitorWarning.mezzanineBootloaderActive[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineBootloaderActive[i] ||
                this->eventHardpointMonitorWarning.uniqueIdCRCError[i] !=
                        this->previousEventHardpointMonitorWarning.uniqueIdCRCError[i] ||
                this->eventHardpointMonitorWarning.applicationTypeMismatch[i] !=
                        this->previousEventHardpointMonitorWarning.applicationTypeMismatch[i] ||
                this->eventHardpointMonitorWarning.applicationMissing[i] !=
                        this->previousEventHardpointMonitorWarning.applicationMissing[i] ||
                this->eventHardpointMonitorWarning.applicationCRCMismatch[i] !=
                        this->previousEventHardpointMonitorWarning.applicationCRCMismatch[i] ||
                this->eventHardpointMonitorWarning.oneWireMissing[i] !=
                        this->previousEventHardpointMonitorWarning.oneWireMissing[i] ||
                this->eventHardpointMonitorWarning.oneWire1Mismatch[i] !=
                        this->previousEventHardpointMonitorWarning.oneWire1Mismatch[i] ||
                this->eventHardpointMonitorWarning.oneWire2Mismatch[i] !=
                        this->previousEventHardpointMonitorWarning.oneWire2Mismatch[i] ||
                this->eventHardpointMonitorWarning.watchdogReset[i] !=
                        this->previousEventHardpointMonitorWarning.watchdogReset[i] ||
                this->eventHardpointMonitorWarning.brownOut[i] !=
                        this->previousEventHardpointMonitorWarning.brownOut[i] ||
                this->eventHardpointMonitorWarning.eventTrapReset[i] !=
                        this->previousEventHardpointMonitorWarning.eventTrapReset[i] ||
                this->eventHardpointMonitorWarning.ssrPowerFault[i] !=
                        this->previousEventHardpointMonitorWarning.ssrPowerFault[i] ||
                this->eventHardpointMonitorWarning.auxPowerFault[i] !=
                        this->previousEventHardpointMonitorWarning.auxPowerFault[i] ||
                this->eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] ||
                this->eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] ||
                this->eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] ||
                this->eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] ||
                this->eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] ||
                this->eventHardpointMonitorWarning.mezzanineEventTrapReset[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineEventTrapReset[i] ||
                this->eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] ||
                this->eventHardpointMonitorWarning.mezzanineApplicationMissing[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineApplicationMissing[i] ||
                this->eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i] !=
                        this->previousEventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorWarning();
    }
}

void M1M3SSPublisher::logILCWarning() {
    this->eventILCWarning.anyWarning =
            this->eventILCWarning.responseTimeout || this->eventILCWarning.invalidCRC ||
            this->eventILCWarning.illegalFunction || this->eventILCWarning.illegalDataValue ||
            this->eventILCWarning.invalidLength || this->eventILCWarning.unknownSubnet ||
            this->eventILCWarning.unknownAddress || this->eventILCWarning.unknownFunction ||
            this->eventILCWarning.unknownProblem;
    this->m1m3SAL->logEvent_ilcWarning(&this->eventILCWarning, 0);
    this->previousEventILCWarning = this->eventILCWarning;
}

void M1M3SSPublisher::tryLogILCWarning() {
    if (this->eventILCWarning.actuatorId != this->previousEventILCWarning.actuatorId ||
        this->eventILCWarning.responseTimeout != this->previousEventILCWarning.responseTimeout ||
        this->eventILCWarning.invalidCRC != this->previousEventILCWarning.invalidCRC ||
        this->eventILCWarning.illegalFunction != this->previousEventILCWarning.illegalFunction ||
        this->eventILCWarning.illegalDataValue != this->previousEventILCWarning.illegalDataValue ||
        this->eventILCWarning.invalidLength != this->previousEventILCWarning.invalidLength ||
        this->eventILCWarning.unknownSubnet != this->previousEventILCWarning.unknownSubnet ||
        this->eventILCWarning.unknownAddress != this->previousEventILCWarning.unknownAddress ||
        this->eventILCWarning.unknownFunction != this->previousEventILCWarning.unknownFunction ||
        this->eventILCWarning.unknownProblem != this->previousEventILCWarning.unknownProblem) {
        this->logILCWarning();
    }
}

void M1M3SSPublisher::logInclinometerSensorWarning() {
    this->eventInclinometerSensorWarning.anyWarning =
            this->eventInclinometerSensorWarning.sensorReportsIllegalFunction ||
            this->eventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
            this->eventInclinometerSensorWarning.responseTimeout ||
            this->eventInclinometerSensorWarning.invalidCRC ||
            this->eventInclinometerSensorWarning.invalidLength ||
            this->eventInclinometerSensorWarning.unknownAddress ||
            this->eventInclinometerSensorWarning.unknownFunction ||
            this->eventInclinometerSensorWarning.unknownProblem;
    this->m1m3SAL->logEvent_inclinometerSensorWarning(&this->eventInclinometerSensorWarning, 0);
    this->previousEventInclinometerSensorWarning = this->eventInclinometerSensorWarning;
}

void M1M3SSPublisher::tryLogInclinometerSensorWarning() {
    if (this->eventInclinometerSensorWarning.sensorReportsIllegalFunction !=
                this->previousEventInclinometerSensorWarning.sensorReportsIllegalFunction ||
        this->eventInclinometerSensorWarning.sensorReportsIllegalDataAddress !=
                this->previousEventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
        this->eventInclinometerSensorWarning.responseTimeout !=
                this->previousEventInclinometerSensorWarning.responseTimeout ||
        this->eventInclinometerSensorWarning.invalidCRC !=
                this->previousEventInclinometerSensorWarning.invalidCRC ||
        this->eventInclinometerSensorWarning.invalidLength !=
                this->previousEventInclinometerSensorWarning.invalidLength ||
        this->eventInclinometerSensorWarning.unknownAddress !=
                this->previousEventInclinometerSensorWarning.unknownAddress ||
        this->eventInclinometerSensorWarning.unknownFunction !=
                this->previousEventInclinometerSensorWarning.unknownFunction ||
        this->eventInclinometerSensorWarning.unknownProblem !=
                this->previousEventInclinometerSensorWarning.unknownProblem) {
        this->logInclinometerSensorWarning();
    }
}

void M1M3SSPublisher::logInterlockStatus() {
    this->m1m3SAL->logEvent_interlockStatus(&this->eventInterlockStatus, 0);
    this->previousEventInterlockStatus = this->eventInterlockStatus;
}

void M1M3SSPublisher::tryLogInterlockStatus() {
    if (this->eventInterlockStatus.heartbeatCommandedState !=
                this->previousEventInterlockStatus.heartbeatCommandedState ||
        this->eventInterlockStatus.heartbeatOutputState !=
                this->previousEventInterlockStatus.heartbeatOutputState) {
        this->logInterlockStatus();
    }
}

void M1M3SSPublisher::logInterlockWarning() {
    this->eventInterlockWarning.anyWarning =
            this->eventInterlockWarning.heartbeatStateOutputMismatch ||
            this->eventInterlockWarning.auxPowerNetworksOff ||
            this->eventInterlockWarning.thermalEquipmentOff || this->eventInterlockWarning.airSupplyOff ||
            this->eventInterlockWarning.cabinetDoorOpen || this->eventInterlockWarning.tmaMotionStop ||
            this->eventInterlockWarning.gisHeartbeatLost;
    this->m1m3SAL->logEvent_interlockWarning(&this->eventInterlockWarning, 0);
    this->previousEventInterlockWarning = this->eventInterlockWarning;
}

void M1M3SSPublisher::tryLogInterlockWarning() {
    if (this->eventInterlockWarning.heartbeatStateOutputMismatch !=
                this->previousEventInterlockWarning.heartbeatStateOutputMismatch ||
        this->eventInterlockWarning.auxPowerNetworksOff !=
                this->previousEventInterlockWarning.auxPowerNetworksOff ||
        this->eventInterlockWarning.thermalEquipmentOff !=
                this->previousEventInterlockWarning.thermalEquipmentOff ||
        this->eventInterlockWarning.airSupplyOff != this->previousEventInterlockWarning.airSupplyOff ||
        this->eventInterlockWarning.cabinetDoorOpen != this->previousEventInterlockWarning.cabinetDoorOpen ||
        this->eventInterlockWarning.tmaMotionStop != this->previousEventInterlockWarning.tmaMotionStop ||
        this->eventInterlockWarning.gisHeartbeatLost !=
                this->previousEventInterlockWarning.gisHeartbeatLost) {
        this->logInterlockWarning();
    }
}

void M1M3SSPublisher::logModbusResponse() {
    this->m1m3SAL->logEvent_modbusResponse(&this->eventModbusResponse, 0);
    this->previousEventModbusResponse = this->eventModbusResponse;
}

void M1M3SSPublisher::tryLogModbusResponse() {
    if (this->eventModbusResponse.responseValid != this->previousEventModbusResponse.responseValid ||
        this->eventModbusResponse.address != this->previousEventModbusResponse.address ||
        this->eventModbusResponse.functionCode != this->previousEventModbusResponse.functionCode ||
        this->eventModbusResponse.crc != this->previousEventModbusResponse.crc ||
        this->eventModbusResponse.dataLength != this->previousEventModbusResponse.dataLength) {
        this->logModbusResponse();
    }
}

void M1M3SSPublisher::logPIDInfo() {
    this->m1m3SAL->logEvent_pidInfo(&this->eventPIDInfo, 0);
    this->previousEventPIDInfo = this->eventPIDInfo;
}

void M1M3SSPublisher::tryLogPIDInfo() {
    bool changeDetected = false;
    for (int i = 0; i < 6 && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventPIDInfo.timestep[i] != this->previousEventPIDInfo.timestep[i] ||
                         this->eventPIDInfo.p[i] != this->previousEventPIDInfo.p[i] ||
                         this->eventPIDInfo.i[i] != this->previousEventPIDInfo.i[i] ||
                         this->eventPIDInfo.d[i] != this->previousEventPIDInfo.d[i] ||
                         this->eventPIDInfo.n[i] != this->previousEventPIDInfo.n[i] ||
                         this->eventPIDInfo.calculatedA[i] != this->previousEventPIDInfo.calculatedA[i] ||
                         this->eventPIDInfo.calculatedB[i] != this->previousEventPIDInfo.calculatedB[i] ||
                         this->eventPIDInfo.calculatedC[i] != this->previousEventPIDInfo.calculatedC[i] ||
                         this->eventPIDInfo.calculatedD[i] != this->previousEventPIDInfo.calculatedD[i] ||
                         this->eventPIDInfo.calculatedE[i] != this->previousEventPIDInfo.calculatedE[i];
    }
    if (changeDetected) {
        this->logPIDInfo();
    }
}

void M1M3SSPublisher::logPowerStatus() {
    this->m1m3SAL->logEvent_powerStatus(&this->eventPowerStatus, 0);
    this->previousEventPowerStatus = this->eventPowerStatus;
}

void M1M3SSPublisher::tryLogPowerStatus() {
    if (this->eventPowerStatus.powerNetworkACommandedOn !=
                this->previousEventPowerStatus.powerNetworkACommandedOn ||
        this->eventPowerStatus.powerNetworkAOutputOn !=
                this->previousEventPowerStatus.powerNetworkAOutputOn ||
        this->eventPowerStatus.powerNetworkBCommandedOn !=
                this->previousEventPowerStatus.powerNetworkBCommandedOn ||
        this->eventPowerStatus.powerNetworkBOutputOn !=
                this->previousEventPowerStatus.powerNetworkBOutputOn ||
        this->eventPowerStatus.powerNetworkCCommandedOn !=
                this->previousEventPowerStatus.powerNetworkCCommandedOn ||
        this->eventPowerStatus.powerNetworkCOutputOn !=
                this->previousEventPowerStatus.powerNetworkCOutputOn ||
        this->eventPowerStatus.powerNetworkDCommandedOn !=
                this->previousEventPowerStatus.powerNetworkDCommandedOn ||
        this->eventPowerStatus.powerNetworkDOutputOn !=
                this->previousEventPowerStatus.powerNetworkDOutputOn ||
        this->eventPowerStatus.auxPowerNetworkACommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkACommandedOn ||
        this->eventPowerStatus.auxPowerNetworkAOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkAOutputOn ||
        this->eventPowerStatus.auxPowerNetworkBCommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkBCommandedOn ||
        this->eventPowerStatus.auxPowerNetworkBOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkBOutputOn ||
        this->eventPowerStatus.auxPowerNetworkCCommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkCCommandedOn ||
        this->eventPowerStatus.auxPowerNetworkCOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkCOutputOn ||
        this->eventPowerStatus.auxPowerNetworkDCommandedOn !=
                this->previousEventPowerStatus.auxPowerNetworkDCommandedOn ||
        this->eventPowerStatus.auxPowerNetworkDOutputOn !=
                this->previousEventPowerStatus.auxPowerNetworkDOutputOn) {
        this->logPowerStatus();
    }
}

void M1M3SSPublisher::logPowerSupplyStatus() {
    this->m1m3SAL->logEvent_powerSupplyStatus(&this->eventPowerSupplyStatus, 0);
    this->previousEventPowerSupplyStatus = this->eventPowerSupplyStatus;
}

void M1M3SSPublisher::tryLogPowerSupplyStatus() {
    if (this->eventPowerSupplyStatus.rcpMirrorCellUtility220VAC1Status !=
                this->previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC1Status ||
        this->eventPowerSupplyStatus.rcpCabinetUtility220VACStatus !=
                this->previousEventPowerSupplyStatus.rcpCabinetUtility220VACStatus ||
        this->eventPowerSupplyStatus.rcpExternalEquipment220VACStatus !=
                this->previousEventPowerSupplyStatus.rcpExternalEquipment220VACStatus ||
        this->eventPowerSupplyStatus.rcpMirrorCellUtility220VAC2Status !=
                this->previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC2Status ||
        this->eventPowerSupplyStatus.rcpMirrorCellUtility220VAC3Status !=
                this->previousEventPowerSupplyStatus.rcpMirrorCellUtility220VAC3Status ||
        this->eventPowerSupplyStatus.powerNetworkARedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkARedundancyControlStatus ||
        this->eventPowerSupplyStatus.powerNetworkBRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkBRedundancyControlStatus ||
        this->eventPowerSupplyStatus.powerNetworkCRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkCRedundancyControlStatus ||
        this->eventPowerSupplyStatus.powerNetworkDRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkDRedundancyControlStatus ||
        this->eventPowerSupplyStatus.controlsPowerNetworkRedundancyControlStatus !=
                this->previousEventPowerSupplyStatus.controlsPowerNetworkRedundancyControlStatus ||
        this->eventPowerSupplyStatus.powerNetworkAStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkAStatus ||
        this->eventPowerSupplyStatus.powerNetworkARedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkARedundantStatus ||
        this->eventPowerSupplyStatus.powerNetworkBStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkBStatus ||
        this->eventPowerSupplyStatus.powerNetworkBRedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkBRedundantStatus ||
        this->eventPowerSupplyStatus.powerNetworkCStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkCStatus ||
        this->eventPowerSupplyStatus.powerNetworkCRedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkCRedundantStatus ||
        this->eventPowerSupplyStatus.powerNetworkDStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkDStatus ||
        this->eventPowerSupplyStatus.powerNetworkDRedundantStatus !=
                this->previousEventPowerSupplyStatus.powerNetworkDRedundantStatus ||
        this->eventPowerSupplyStatus.controlsPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.controlsPowerNetworkStatus ||
        this->eventPowerSupplyStatus.controlsPowerNetworkRedundantStatus !=
                this->previousEventPowerSupplyStatus.controlsPowerNetworkRedundantStatus ||
        this->eventPowerSupplyStatus.lightPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.lightPowerNetworkStatus ||
        this->eventPowerSupplyStatus.externalEquipmentPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.externalEquipmentPowerNetworkStatus ||
        this->eventPowerSupplyStatus.laserTrackerPowerNetworkStatus !=
                this->previousEventPowerSupplyStatus.laserTrackerPowerNetworkStatus) {
        this->logPowerSupplyStatus();
    }
}

void M1M3SSPublisher::logPowerWarning() {
    this->eventPowerWarning.anyWarning = this->eventPowerWarning.powerNetworkAOutputMismatch ||
                                         this->eventPowerWarning.powerNetworkBOutputMismatch ||
                                         this->eventPowerWarning.powerNetworkCOutputMismatch ||
                                         this->eventPowerWarning.powerNetworkDOutputMismatch ||
                                         this->eventPowerWarning.auxPowerNetworkAOutputMismatch ||
                                         this->eventPowerWarning.auxPowerNetworkBOutputMismatch ||
                                         this->eventPowerWarning.auxPowerNetworkCOutputMismatch ||
                                         this->eventPowerWarning.auxPowerNetworkDOutputMismatch;
    this->m1m3SAL->logEvent_powerWarning(&this->eventPowerWarning, 0);
    this->previousEventPowerWarning = this->eventPowerWarning;
}

void M1M3SSPublisher::tryLogPowerWarning() {
    if (this->eventPowerWarning.powerNetworkAOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkAOutputMismatch ||
        this->eventPowerWarning.powerNetworkBOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkBOutputMismatch ||
        this->eventPowerWarning.powerNetworkCOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkCOutputMismatch ||
        this->eventPowerWarning.powerNetworkDOutputMismatch !=
                this->previousEventPowerWarning.powerNetworkDOutputMismatch ||
        this->eventPowerWarning.auxPowerNetworkAOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkAOutputMismatch ||
        this->eventPowerWarning.auxPowerNetworkBOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkBOutputMismatch ||
        this->eventPowerWarning.auxPowerNetworkCOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkCOutputMismatch ||
        this->eventPowerWarning.auxPowerNetworkDOutputMismatch !=
                this->previousEventPowerWarning.auxPowerNetworkDOutputMismatch) {
        this->logPowerWarning();
    }
}

void M1M3SSPublisher::logRejectedAberrationForces() {
    this->m1m3SAL->logEvent_rejectedAberrationForces(&this->eventRejectedAberrationForces, 0);
    this->previousEventRejectedAberrationForces = this->eventRejectedAberrationForces;
}

void M1M3SSPublisher::tryLogRejectedAberrationForces() {
    bool changeDetected =
            this->eventRejectedAberrationForces.fz != this->previousEventRejectedAberrationForces.fz ||
            this->eventRejectedAberrationForces.mx != this->previousEventRejectedAberrationForces.mx ||
            this->eventRejectedAberrationForces.my != this->previousEventRejectedAberrationForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || this->eventRejectedAberrationForces.zForces[i] !=
                                                   this->previousEventRejectedAberrationForces.zForces[i];
    }
    if (changeDetected) {
        this->logRejectedAberrationForces();
    }
}

void M1M3SSPublisher::logRejectedAccelerationForces() {
    this->m1m3SAL->logEvent_rejectedAccelerationForces(&this->eventRejectedAccelerationForces, 0);
    this->previousEventRejectedAccelerationForces = this->eventRejectedAccelerationForces;
}

void M1M3SSPublisher::tryLogRejectedAccelerationForces() {
    bool changeDetected =
            this->eventRejectedAccelerationForces.fx != this->previousEventRejectedAccelerationForces.fx ||
            this->eventRejectedAccelerationForces.fy != this->previousEventRejectedAccelerationForces.fy ||
            this->eventRejectedAccelerationForces.fz != this->previousEventRejectedAccelerationForces.fz ||
            this->eventRejectedAccelerationForces.mx != this->previousEventRejectedAccelerationForces.mx ||
            this->eventRejectedAccelerationForces.my != this->previousEventRejectedAccelerationForces.my ||
            this->eventRejectedAccelerationForces.mz != this->previousEventRejectedAccelerationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedAccelerationForces.xForces[i] !=
                                            this->previousEventRejectedAccelerationForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedAccelerationForces.yForces[i] !=
                                             this->previousEventRejectedAccelerationForces.yForces[i]) ||
                         (this->eventRejectedAccelerationForces.zForces[i] !=
                          this->previousEventRejectedAccelerationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedAccelerationForces();
    }
}

void M1M3SSPublisher::logRejectedActiveOpticForces() {
    this->m1m3SAL->logEvent_rejectedActiveOpticForces(&this->eventRejectedActiveOpticForces, 0);
    this->previousEventRejectedActiveOpticForces = this->eventRejectedActiveOpticForces;
}

void M1M3SSPublisher::tryLogRejectedActiveOpticForces() {
    bool changeDetected =
            this->eventRejectedActiveOpticForces.fz != this->previousEventRejectedActiveOpticForces.fz ||
            this->eventRejectedActiveOpticForces.mx != this->previousEventRejectedActiveOpticForces.mx ||
            this->eventRejectedActiveOpticForces.my != this->previousEventRejectedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || this->eventRejectedActiveOpticForces.zForces[i] !=
                                                   this->previousEventRejectedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        this->logRejectedActiveOpticForces();
    }
}

void M1M3SSPublisher::logRejectedAzimuthForces() {
    this->m1m3SAL->logEvent_rejectedAzimuthForces(&this->eventRejectedAzimuthForces, 0);
    this->previousEventRejectedAzimuthForces = this->eventRejectedAzimuthForces;
}

void M1M3SSPublisher::tryLogRejectedAzimuthForces() {
    bool changeDetected =
            this->eventRejectedAzimuthForces.fx != this->previousEventRejectedAzimuthForces.fx ||
            this->eventRejectedAzimuthForces.fy != this->previousEventRejectedAzimuthForces.fy ||
            this->eventRejectedAzimuthForces.fz != this->previousEventRejectedAzimuthForces.fz ||
            this->eventRejectedAzimuthForces.mx != this->previousEventRejectedAzimuthForces.mx ||
            this->eventRejectedAzimuthForces.my != this->previousEventRejectedAzimuthForces.my ||
            this->eventRejectedAzimuthForces.mz != this->previousEventRejectedAzimuthForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedAzimuthForces.xForces[i] !=
                                            this->previousEventRejectedAzimuthForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedAzimuthForces.yForces[i] !=
                                             this->previousEventRejectedAzimuthForces.yForces[i]) ||
                         (this->eventRejectedAzimuthForces.zForces[i] !=
                          this->previousEventRejectedAzimuthForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedAzimuthForces();
    }
}

void M1M3SSPublisher::logRejectedBalanceForces() {
    this->m1m3SAL->logEvent_rejectedBalanceForces(&this->eventRejectedBalanceForces, 0);
    this->previousEventRejectedBalanceForces = this->eventRejectedBalanceForces;
}

void M1M3SSPublisher::tryLogRejectedBalanceForces() {
    bool changeDetected =
            this->eventRejectedBalanceForces.fx != this->previousEventRejectedBalanceForces.fx ||
            this->eventRejectedBalanceForces.fy != this->previousEventRejectedBalanceForces.fy ||
            this->eventRejectedBalanceForces.fz != this->previousEventRejectedBalanceForces.fz ||
            this->eventRejectedBalanceForces.mx != this->previousEventRejectedBalanceForces.mx ||
            this->eventRejectedBalanceForces.my != this->previousEventRejectedBalanceForces.my ||
            this->eventRejectedBalanceForces.mz != this->previousEventRejectedBalanceForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedBalanceForces.xForces[i] !=
                                            this->previousEventRejectedBalanceForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedBalanceForces.yForces[i] !=
                                             this->previousEventRejectedBalanceForces.yForces[i]) ||
                         (this->eventRejectedBalanceForces.zForces[i] !=
                          this->previousEventRejectedBalanceForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedBalanceForces();
    }
}

void M1M3SSPublisher::logRejectedCylinderForces() {
    this->m1m3SAL->logEvent_rejectedCylinderForces(&this->eventRejectedCylinderForces, 0);
    this->previousEventRejectedCylinderForces = this->eventRejectedCylinderForces;
}

void M1M3SSPublisher::tryLogRejectedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && this->eventRejectedCylinderForces.secondaryCylinderForces[i] !=
                                    this->previousEventRejectedCylinderForces.secondaryCylinderForces[i]) ||
                (this->eventRejectedCylinderForces.primaryCylinderForces[i] !=
                 this->previousEventRejectedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        this->logRejectedCylinderForces();
    }
}

void M1M3SSPublisher::logRejectedElevationForces() {
    this->m1m3SAL->logEvent_rejectedElevationForces(&this->eventRejectedElevationForces, 0);
    this->previousEventRejectedElevationForces = this->eventRejectedElevationForces;
}

void M1M3SSPublisher::tryLogRejectedElevationForces() {
    bool changeDetected =
            this->eventRejectedElevationForces.fx != this->previousEventRejectedElevationForces.fx ||
            this->eventRejectedElevationForces.fy != this->previousEventRejectedElevationForces.fy ||
            this->eventRejectedElevationForces.fz != this->previousEventRejectedElevationForces.fz ||
            this->eventRejectedElevationForces.mx != this->previousEventRejectedElevationForces.mx ||
            this->eventRejectedElevationForces.my != this->previousEventRejectedElevationForces.my ||
            this->eventRejectedElevationForces.mz != this->previousEventRejectedElevationForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedElevationForces.xForces[i] !=
                                            this->previousEventRejectedElevationForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedElevationForces.yForces[i] !=
                                             this->previousEventRejectedElevationForces.yForces[i]) ||
                         (this->eventRejectedElevationForces.zForces[i] !=
                          this->previousEventRejectedElevationForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedElevationForces();
    }
}

void M1M3SSPublisher::logRejectedForces() {
    this->m1m3SAL->logEvent_rejectedForces(&this->eventRejectedForces, 0);
    this->previousEventRejectedForces = this->eventRejectedForces;
}

void M1M3SSPublisher::tryLogRejectedForces() {
    bool changeDetected = this->eventRejectedForces.fx != this->previousEventRejectedForces.fx ||
                          this->eventRejectedForces.fy != this->previousEventRejectedForces.fy ||
                          this->eventRejectedForces.fz != this->previousEventRejectedForces.fz ||
                          this->eventRejectedForces.mx != this->previousEventRejectedForces.mx ||
                          this->eventRejectedForces.my != this->previousEventRejectedForces.my ||
                          this->eventRejectedForces.mz != this->previousEventRejectedForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 this->eventRejectedForces.xForces[i] != this->previousEventRejectedForces.xForces[i]) ||
                (i < 100 &&
                 this->eventRejectedForces.yForces[i] != this->previousEventRejectedForces.yForces[i]) ||
                (this->eventRejectedForces.zForces[i] != this->previousEventRejectedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedForces();
    }
}

void M1M3SSPublisher::logRejectedOffsetForces() {
    this->m1m3SAL->logEvent_rejectedOffsetForces(&this->eventRejectedOffsetForces, 0);
    this->previousEventRejectedOffsetForces = this->eventRejectedOffsetForces;
}

void M1M3SSPublisher::tryLogRejectedOffsetForces() {
    bool changeDetected = this->eventRejectedOffsetForces.fx != this->previousEventRejectedOffsetForces.fx ||
                          this->eventRejectedOffsetForces.fy != this->previousEventRejectedOffsetForces.fy ||
                          this->eventRejectedOffsetForces.fz != this->previousEventRejectedOffsetForces.fz ||
                          this->eventRejectedOffsetForces.mx != this->previousEventRejectedOffsetForces.mx ||
                          this->eventRejectedOffsetForces.my != this->previousEventRejectedOffsetForces.my ||
                          this->eventRejectedOffsetForces.mz != this->previousEventRejectedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedOffsetForces.xForces[i] !=
                                            this->previousEventRejectedOffsetForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedOffsetForces.yForces[i] !=
                                             this->previousEventRejectedOffsetForces.yForces[i]) ||
                         (this->eventRejectedOffsetForces.zForces[i] !=
                          this->previousEventRejectedOffsetForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedOffsetForces();
    }
}

void M1M3SSPublisher::logRejectedStaticForces() {
    this->m1m3SAL->logEvent_rejectedStaticForces(&this->eventRejectedStaticForces, 0);
    this->previousEventRejectedStaticForces = this->eventRejectedStaticForces;
}

void M1M3SSPublisher::tryLogRejectedStaticForces() {
    bool changeDetected = this->eventRejectedStaticForces.fx != this->previousEventRejectedStaticForces.fx ||
                          this->eventRejectedStaticForces.fy != this->previousEventRejectedStaticForces.fy ||
                          this->eventRejectedStaticForces.fz != this->previousEventRejectedStaticForces.fz ||
                          this->eventRejectedStaticForces.mx != this->previousEventRejectedStaticForces.mx ||
                          this->eventRejectedStaticForces.my != this->previousEventRejectedStaticForces.my ||
                          this->eventRejectedStaticForces.mz != this->previousEventRejectedStaticForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedStaticForces.xForces[i] !=
                                            this->previousEventRejectedStaticForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedStaticForces.yForces[i] !=
                                             this->previousEventRejectedStaticForces.yForces[i]) ||
                         (this->eventRejectedStaticForces.zForces[i] !=
                          this->previousEventRejectedStaticForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedStaticForces();
    }
}

void M1M3SSPublisher::logRejectedThermalForces() {
    this->m1m3SAL->logEvent_rejectedThermalForces(&this->eventRejectedThermalForces, 0);
    this->previousEventRejectedThermalForces = this->eventRejectedThermalForces;
}

void M1M3SSPublisher::tryLogRejectedThermalForces() {
    bool changeDetected =
            this->eventRejectedThermalForces.fx != this->previousEventRejectedThermalForces.fx ||
            this->eventRejectedThermalForces.fy != this->previousEventRejectedThermalForces.fy ||
            this->eventRejectedThermalForces.fz != this->previousEventRejectedThermalForces.fz ||
            this->eventRejectedThermalForces.mx != this->previousEventRejectedThermalForces.mx ||
            this->eventRejectedThermalForces.my != this->previousEventRejectedThermalForces.my ||
            this->eventRejectedThermalForces.mz != this->previousEventRejectedThermalForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedThermalForces.xForces[i] !=
                                            this->previousEventRejectedThermalForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedThermalForces.yForces[i] !=
                                             this->previousEventRejectedThermalForces.yForces[i]) ||
                         (this->eventRejectedThermalForces.zForces[i] !=
                          this->previousEventRejectedThermalForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedThermalForces();
    }
}

void M1M3SSPublisher::logRejectedVelocityForces() {
    this->m1m3SAL->logEvent_rejectedVelocityForces(&this->eventRejectedVelocityForces, 0);
    this->previousEventRejectedVelocityForces = this->eventRejectedVelocityForces;
}

void M1M3SSPublisher::tryLogRejectedVelocityForces() {
    bool changeDetected =
            this->eventRejectedVelocityForces.fx != this->previousEventRejectedVelocityForces.fx ||
            this->eventRejectedVelocityForces.fy != this->previousEventRejectedVelocityForces.fy ||
            this->eventRejectedVelocityForces.fz != this->previousEventRejectedVelocityForces.fz ||
            this->eventRejectedVelocityForces.mx != this->previousEventRejectedVelocityForces.mx ||
            this->eventRejectedVelocityForces.my != this->previousEventRejectedVelocityForces.my ||
            this->eventRejectedVelocityForces.mz != this->previousEventRejectedVelocityForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedVelocityForces.xForces[i] !=
                                            this->previousEventRejectedVelocityForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedVelocityForces.yForces[i] !=
                                             this->previousEventRejectedVelocityForces.yForces[i]) ||
                         (this->eventRejectedVelocityForces.zForces[i] !=
                          this->previousEventRejectedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedVelocityForces();
    }
}

void M1M3SSPublisher::logSettingVersions() {
    this->m1m3SAL->logEvent_settingVersions(&this->eventSettingVersions, 0);
}

void M1M3SSPublisher::logSettingsApplied() {
    this->m1m3SAL->logEvent_settingsApplied(&this->eventSettingsApplied, 0);
    this->previousEventSettingsApplied = this->eventSettingsApplied;
}

void M1M3SSPublisher::tryLogSettingsApplied() {
    if (this->eventSettingsApplied.settingsVersion.compare(
                this->previousEventSettingsApplied.settingsVersion) != 0) {
        this->logSettingsApplied();
    }
}

void M1M3SSPublisher::logSummaryState() {
    this->m1m3SAL->logEvent_summaryState(&this->eventSummaryState, 0);
    this->previousEventSummaryState = this->eventSummaryState;
}

void M1M3SSPublisher::tryLogSummaryState() {
    if (this->eventSummaryState.summaryState != this->previousEventSummaryState.summaryState) {
        this->logSummaryState();
    }
}

void M1M3SSPublisher::ackCommandstart(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_start(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenable(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_enable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommanddisable(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_disable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandstandby(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_standby(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandshutdown(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_shutdown(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnAirOn(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_turnAirOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnAirOff(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_turnAirOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyOffsetForces(int32_t commandID, int32_t ackCode,
                                                  std::string description) {
    this->m1m3SAL->ackCommand_applyOffsetForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandclearOffsetForces(int32_t commandID, int32_t ackCode,
                                                  std::string description) {
    this->m1m3SAL->ackCommand_clearOffsetForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandraiseM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_raiseM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandlowerM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_lowerM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyActiveOpticForcesByBendingModes(int32_t commandID, int32_t ackCode,
                                                                     std::string description) {
    this->m1m3SAL->ackCommand_applyActiveOpticForcesByBendingModes(commandID, ackCode, 0,
                                                                   (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyActiveOpticForces(int32_t commandID, int32_t ackCode,
                                                       std::string description) {
    this->m1m3SAL->ackCommand_applyActiveOpticForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandclearActiveOpticForces(int32_t commandID, int32_t ackCode,
                                                       std::string description) {
    this->m1m3SAL->ackCommand_clearActiveOpticForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyAberrationForcesByBendingModes(int32_t commandID, int32_t ackCode,
                                                                    std::string description) {
    this->m1m3SAL->ackCommand_applyAberrationForcesByBendingModes(commandID, ackCode, 0,
                                                                  (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyAberrationForces(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    this->m1m3SAL->ackCommand_applyAberrationForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandclearAberrationForces(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    this->m1m3SAL->ackCommand_clearAberrationForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenterEngineering(int32_t commandID, int32_t ackCode,
                                                 std::string description) {
    this->m1m3SAL->ackCommand_enterEngineering(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandexitEngineering(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_exitEngineering(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtestAir(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_testAir(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtestHardpoint(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_testHardpoint(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtestForceActuator(int32_t commandID, int32_t ackCode,
                                                  std::string description) {
    this->m1m3SAL->ackCommand_testForceActuator(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandmoveHardpointActuators(int32_t commandID, int32_t ackCode,
                                                       std::string description) {
    this->m1m3SAL->ackCommand_moveHardpointActuators(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenableHardpointChase(int32_t commandID, int32_t ackCode,
                                                     std::string description) {
    this->m1m3SAL->ackCommand_enableHardpointChase(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommanddisableHardpointChase(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    this->m1m3SAL->ackCommand_disableHardpointChase(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandabortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_abortRaiseM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandtranslateM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_translateM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandstopHardpointMotion(int32_t commandID, int32_t ackCode,
                                                    std::string description) {
    this->m1m3SAL->ackCommand_stopHardpointMotion(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandpositionM1M3(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_positionM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnLightsOn(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_turnLightsOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnLightsOff(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_turnLightsOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnPowerOn(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_turnPowerOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandturnPowerOff(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_turnPowerOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandenableHardpointCorrections(int32_t commandID, int32_t ackCode,
                                                           std::string description) {
    this->m1m3SAL->ackCommand_enableHardpointCorrections(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommanddisableHardpointCorrections(int32_t commandID, int32_t ackCode,
                                                            std::string description) {
    this->m1m3SAL->ackCommand_disableHardpointCorrections(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandrunMirrorForceProfile(int32_t commandID, int32_t ackCode,
                                                      std::string description) {
    this->m1m3SAL->ackCommand_runMirrorForceProfile(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandabortProfile(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_abortProfile(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandapplyOffsetForcesByMirrorForce(int32_t commandID, int32_t ackCode,
                                                               std::string description) {
    this->m1m3SAL->ackCommand_applyOffsetForcesByMirrorForce(commandID, ackCode, 0,
                                                             (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandupdatePID(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_updatePID(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandresetPID(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_resetPID(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandprogramILC(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_programILC(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandmodbusTransmit(int32_t commandID, int32_t ackCode, std::string description) {
    this->m1m3SAL->ackCommand_modbusTransmit(commandID, ackCode, 0, (char*)description.c_str());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
