/*
 * M1M3SSPublisher.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSPublisher.h>
#include <SAL_MTM1M3.h>
#include <ccpp_sal_MTM1M3.h> // Provides access to enumerations

#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSPublisher::M1M3SSPublisher(SAL_MTM1M3* m1m3SAL) {
    Log.Debug("M1M3SSPublisher: M1M3SSPublisher()");
    this->m1m3SAL = m1m3SAL;

    Log.Debug("M1M3SSPublisher: Initializing SAL Telemetry");
    this->initTelemetry("MTM1M3_accelerometerData");
    this->initTelemetry("MTM1M3_forceActuatorData");
    this->initTelemetry("MTM1M3_gyroData");
    this->initTelemetry("MTM1M3_hardpointActuatorData");
    this->initTelemetry("MTM1M3_hardpointMonitorData");
    this->initTelemetry("MTM1M3_imsData");
    this->initTelemetry("MTM1M3_inclinometerData");
    this->initTelemetry("MTM1M3_outerLoopData");
    this->initTelemetry("MTM1M3_pidData");
    this->initTelemetry("MTM1M3_powerData");

    Log.Debug("M1M3SSPublisher: Initializing SAL Events");
    this->initEvent("MTM1M3_logevent_accelerometerWarning");
    this->initEvent("MTM1M3_logevent_accelerometerWarning");
    this->initEvent("MTM1M3_logevent_airSupplyStatus");
    this->initEvent("MTM1M3_logevent_airSupplyWarning");
    this->initEvent("MTM1M3_logevent_appliedAberrationForces");
    this->initEvent("MTM1M3_logevent_appliedAccelerationForces");
    this->initEvent("MTM1M3_logevent_appliedActiveOpticForces");
    this->initEvent("MTM1M3_logevent_appliedAzimuthForces");
    this->initEvent("MTM1M3_logevent_appliedBalanceForces");
    this->initEvent("MTM1M3_logevent_appliedCylinderForces");
    this->initEvent("MTM1M3_logevent_appliedElevationForces");
    this->initEvent("MTM1M3_logevent_appliedForces");
    this->initEvent("MTM1M3_logevent_appliedHardpointSteps");
    this->initEvent("MTM1M3_logevent_appliedOffsetForces");
    this->initEvent("MTM1M3_logevent_appliedSettingsMatchStart");
    this->initEvent("MTM1M3_logevent_appliedStaticForces");
    this->initEvent("MTM1M3_logevent_appliedThermalForces");
    this->initEvent("MTM1M3_logevent_appliedVelocityForces");
    this->initEvent("MTM1M3_logevent_cellLightStatus");
    this->initEvent("MTM1M3_logevent_cellLightWarning");
    this->initEvent("MTM1M3_logevent_detailedState");
    this->initEvent("MTM1M3_logevent_displacementSensorWarning");
    this->initEvent("MTM1M3_logevent_errorCode");
    this->initEvent("MTM1M3_logevent_forceActuatorBackupCalibrationInfo");
    this->initEvent("MTM1M3_logevent_forceActuatorIdInfo");
    this->initEvent("MTM1M3_logevent_forceActuatorILCInfo");
    this->initEvent("MTM1M3_logevent_forceActuatorMainCalibrationInfo");
    this->initEvent("MTM1M3_logevent_forceActuatorMezzanineCalibrationInfo");
    this->initEvent("MTM1M3_logevent_forceActuatorPositionInfo");
    this->initEvent("MTM1M3_logevent_forceActuatorState");
    this->initEvent("MTM1M3_logevent_forceActuatorWarning");
    this->initEvent("MTM1M3_logevent_gyroWarning");
    this->initEvent("MTM1M3_logevent_hardpointActuatorInfo");
    this->initEvent("MTM1M3_logevent_hardpointActuatorState");
    this->initEvent("MTM1M3_logevent_hardpointActuatorWarning");
    this->initEvent("MTM1M3_logevent_hardpointMonitorInfo");
    this->initEvent("MTM1M3_logevent_hardpointMonitorState");
    this->initEvent("MTM1M3_logevent_hardpointMonitorWarning");
    this->initEvent("MTM1M3_logevent_inclinometerSensorWarning");
    this->initEvent("MTM1M3_logevent_interlockStatus");
    this->initEvent("MTM1M3_logevent_interlockWarning");
    this->initEvent("MTM1M3_logevent_modbusResponse");
    this->initEvent("MTM1M3_logevent_pidInfo");
    this->initEvent("MTM1M3_logevent_powerStatus");
    this->initEvent("MTM1M3_logevent_powerWarning");
    this->initEvent("MTM1M3_logevent_rejectedAberrationForces");
    this->initEvent("MTM1M3_logevent_rejectedAccelerationForces");
    this->initEvent("MTM1M3_logevent_rejectedActiveOpticForces");
    this->initEvent("MTM1M3_logevent_rejectedAzimuthForces");
    this->initEvent("MTM1M3_logevent_rejectedBalanceForces");
    this->initEvent("MTM1M3_logevent_rejectedCylinderForces");
    this->initEvent("MTM1M3_logevent_rejectedElevationForces");
    this->initEvent("MTM1M3_logevent_rejectedForces");
    this->initEvent("MTM1M3_logevent_rejectedOffsetForces");
    this->initEvent("MTM1M3_logevent_rejectedStaticForces");
    this->initEvent("MTM1M3_logevent_rejectedThermalForces");
    this->initEvent("MTM1M3_logevent_rejectedVelocityForces");
    this->initEvent("MTM1M3_logevent_settingVersions");
    this->initEvent("MTM1M3_logevent_summaryState");
}

void M1M3SSPublisher::initEvent(std::string topic) {
    Log.Debug("M1M3SSPublisher::initEvent(%s)", topic.c_str());
    this->m1m3SAL->salEventPub((char*)topic.c_str());
}

void M1M3SSPublisher::initTelemetry(std::string topic) {
    Log.Debug("M1M3SSPublisher::initTelemetry(%s)", topic.c_str());
    this->m1m3SAL->salTelemetryPub((char*)topic.c_str());
}

double M1M3SSPublisher::getTimestamp() { return this->m1m3SAL->getCurrentTime(); }

void M1M3SSPublisher::putAccelerometerData() { this->m1m3SAL->putSample_accelerometerData(&this->accelerometerData); }
void M1M3SSPublisher::putForceActuatorData() { this->m1m3SAL->putSample_forceActuatorData(&this->forceActuatorData); }
void M1M3SSPublisher::putGyroData() { this->m1m3SAL->putSample_gyroData(&this->gyroData); }
void M1M3SSPublisher::putHardpointActuatorData() { this->m1m3SAL->putSample_hardpointActuatorData(&this->hardpointActuatorData); }
void M1M3SSPublisher::putHardpointMonitorData() { this->m1m3SAL->putSample_hardpointMonitorData(&this->hardpointMonitorData); }
void M1M3SSPublisher::putIMSData() { this->m1m3SAL->putSample_imsData(&this->imsData); }
void M1M3SSPublisher::putInclinometerData() { this->m1m3SAL->putSample_inclinometerData(&this->inclinometerData); }
void M1M3SSPublisher::putOuterLoopData() { this->m1m3SAL->putSample_outerLoopData(&this->outerLoopData); }
void M1M3SSPublisher::putPIDData() { this->m1m3SAL->putSample_pidData(&this->pidData); }
void M1M3SSPublisher::putPowerData() { this->m1m3SAL->putSample_powerData(&this->powerData); }

void M1M3SSPublisher::logAccelerometerWarning() {
    this->eventAccelerometerWarning.anyWarning = this->eventAccelerometerWarning.accelerometerFlags != 0;
    this->m1m3SAL->logEvent_accelerometerWarning(&this->eventAccelerometerWarning, 0);
    this->previousEventAccelerometerWarning = this->eventAccelerometerWarning;
}

void M1M3SSPublisher::tryLogAccelerometerWarning() {
    if (this->eventAccelerometerWarning.accelerometerFlags != this->previousEventAccelerometerWarning.accelerometerFlags) {
        this->logAccelerometerWarning();
    }
}

void M1M3SSPublisher::logAirSupplyStatus() {
    this->m1m3SAL->logEvent_airSupplyStatus(&this->eventAirSupplyStatus, 0);
    this->previousEventAirSupplyStatus = this->eventAirSupplyStatus;
}

void M1M3SSPublisher::tryLogAirSupplyStatus() {
    if (this->eventAirSupplyStatus.airCommandedOn != this->previousEventAirSupplyStatus.airCommandedOn ||
        this->eventAirSupplyStatus.airValveOpened != this->previousEventAirSupplyStatus.airValveOpened ||
        this->eventAirSupplyStatus.airValveClosed != this->previousEventAirSupplyStatus.airValveClosed) {
        this->logAirSupplyStatus();
    }
}

void M1M3SSPublisher::logAirSupplyWarning() {
    this->eventAirSupplyWarning.anyWarning = this->eventAirSupplyWarning.airSupplyFlags != 0;
    this->m1m3SAL->logEvent_airSupplyWarning(&this->eventAirSupplyWarning, 0);
    this->previousEventAirSupplyWarning = this->eventAirSupplyWarning;
}

void M1M3SSPublisher::tryLogAirSupplyWarning() {
    if (this->eventAirSupplyWarning.airSupplyFlags != this->previousEventAirSupplyWarning.airSupplyFlags) {
        this->logAirSupplyWarning();
    }
}

void M1M3SSPublisher::logAppliedAberrationForces() {
    this->m1m3SAL->logEvent_appliedAberrationForces(&this->eventAppliedAberrationForces, 0);
    this->previousEventAppliedAberrationForces = this->eventAppliedAberrationForces;
}

void M1M3SSPublisher::tryLogAppliedAberrationForces() {
    bool changeDetected =
            this->eventAppliedAberrationForces.fZ != this->previousEventAppliedAberrationForces.fZ ||
            this->eventAppliedAberrationForces.mX != this->previousEventAppliedAberrationForces.mX ||
            this->eventAppliedAberrationForces.mY != this->previousEventAppliedAberrationForces.mY;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventAppliedAberrationForces.zForces[i] != this->previousEventAppliedAberrationForces.zForces[i];
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
            this->eventAppliedAccelerationForces.fX != this->previousEventAppliedAccelerationForces.fX ||
            this->eventAppliedAccelerationForces.fY != this->previousEventAppliedAccelerationForces.fY ||
            this->eventAppliedAccelerationForces.fZ != this->previousEventAppliedAccelerationForces.fZ ||
            this->eventAppliedAccelerationForces.mX != this->previousEventAppliedAccelerationForces.mX ||
            this->eventAppliedAccelerationForces.mY != this->previousEventAppliedAccelerationForces.mY ||
            this->eventAppliedAccelerationForces.mZ != this->previousEventAppliedAccelerationForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedAccelerationForces.xForces[i] != this->previousEventAppliedAccelerationForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedAccelerationForces.yForces[i] != this->previousEventAppliedAccelerationForces.yForces[i]) ||
                         (this->eventAppliedAccelerationForces.zForces[i] != this->previousEventAppliedAccelerationForces.zForces[i]);
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
            this->eventAppliedActiveOpticForces.fZ != this->previousEventAppliedActiveOpticForces.fZ ||
            this->eventAppliedActiveOpticForces.mX != this->previousEventAppliedActiveOpticForces.mX ||
            this->eventAppliedActiveOpticForces.mY != this->previousEventAppliedActiveOpticForces.mY;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventAppliedActiveOpticForces.zForces[i] != this->previousEventAppliedActiveOpticForces.zForces[i];
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
    bool changeDetected =
            this->eventAppliedAzimuthForces.fX != this->previousEventAppliedAzimuthForces.fX ||
            this->eventAppliedAzimuthForces.fY != this->previousEventAppliedAzimuthForces.fY ||
            this->eventAppliedAzimuthForces.fZ != this->previousEventAppliedAzimuthForces.fZ ||
            this->eventAppliedAzimuthForces.mX != this->previousEventAppliedAzimuthForces.mX ||
            this->eventAppliedAzimuthForces.mY != this->previousEventAppliedAzimuthForces.mY ||
            this->eventAppliedAzimuthForces.mZ != this->previousEventAppliedAzimuthForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedAzimuthForces.xForces[i] != this->previousEventAppliedAzimuthForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedAzimuthForces.yForces[i] != this->previousEventAppliedAzimuthForces.yForces[i]) ||
                         (this->eventAppliedAzimuthForces.zForces[i] != this->previousEventAppliedAzimuthForces.zForces[i]);
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
    bool changeDetected =
            this->eventAppliedBalanceForces.fX != this->previousEventAppliedBalanceForces.fX ||
            this->eventAppliedBalanceForces.fY != this->previousEventAppliedBalanceForces.fY ||
            this->eventAppliedBalanceForces.fZ != this->previousEventAppliedBalanceForces.fZ ||
            this->eventAppliedBalanceForces.mX != this->previousEventAppliedBalanceForces.mX ||
            this->eventAppliedBalanceForces.mY != this->previousEventAppliedBalanceForces.mY ||
            this->eventAppliedBalanceForces.mZ != this->previousEventAppliedBalanceForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedBalanceForces.xForces[i] != this->previousEventAppliedBalanceForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedBalanceForces.yForces[i] != this->previousEventAppliedBalanceForces.yForces[i]) ||
                         (this->eventAppliedBalanceForces.zForces[i] != this->previousEventAppliedBalanceForces.zForces[i]);
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
        changeDetected = changeDetected ||
                         (i < 112 && this->eventAppliedCylinderForces.secondaryCylinderForces[i] != this->previousEventAppliedCylinderForces.secondaryCylinderForces[i]) ||
                         (this->eventAppliedCylinderForces.primaryCylinderForces[i] != this->previousEventAppliedCylinderForces.primaryCylinderForces[i]);
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
            this->eventAppliedElevationForces.fX != this->previousEventAppliedElevationForces.fX ||
            this->eventAppliedElevationForces.fY != this->previousEventAppliedElevationForces.fY ||
            this->eventAppliedElevationForces.fZ != this->previousEventAppliedElevationForces.fZ ||
            this->eventAppliedElevationForces.mX != this->previousEventAppliedElevationForces.mX ||
            this->eventAppliedElevationForces.mY != this->previousEventAppliedElevationForces.mY ||
            this->eventAppliedElevationForces.mZ != this->previousEventAppliedElevationForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedElevationForces.xForces[i] != this->previousEventAppliedElevationForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedElevationForces.yForces[i] != this->previousEventAppliedElevationForces.yForces[i]) ||
                         (this->eventAppliedElevationForces.zForces[i] != this->previousEventAppliedElevationForces.zForces[i]);
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
    bool changeDetected =
            this->eventAppliedForces.fX != this->previousEventAppliedForces.fX ||
            this->eventAppliedForces.fY != this->previousEventAppliedForces.fY ||
            this->eventAppliedForces.fZ != this->previousEventAppliedForces.fZ ||
            this->eventAppliedForces.mX != this->previousEventAppliedForces.mX ||
            this->eventAppliedForces.mY != this->previousEventAppliedForces.mY ||
            this->eventAppliedForces.mZ != this->previousEventAppliedForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedForces.xForces[i] != this->previousEventAppliedForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedForces.yForces[i] != this->previousEventAppliedForces.yForces[i]) ||
                         (this->eventAppliedForces.zForces[i] != this->previousEventAppliedForces.zForces[i]);
    }
    if (changeDetected) {
        this->logAppliedForces();
    }
}

void M1M3SSPublisher::logAppliedHardpointSteps() {
    this->m1m3SAL->logEvent_appliedHardpointSteps(&this->eventAppliedHardpointSteps, 0);
    this->previousEventAppliedHardpointSteps = this->eventAppliedHardpointSteps;
}

void M1M3SSPublisher::tryLogAppliedHardpointSteps() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventAppliedHardpointSteps.targetEncoderValues[i] != this->previousEventAppliedHardpointSteps.targetEncoderValues[i] ||
                         this->eventAppliedHardpointSteps.queuedSteps[i] != this->previousEventAppliedHardpointSteps.queuedSteps[i] ||
                         this->eventAppliedHardpointSteps.commandedSteps[i] != this->previousEventAppliedHardpointSteps.commandedSteps[i];
    }
    if (changeDetected) {
        this->logAppliedHardpointSteps();
    }
}

void M1M3SSPublisher::logAppliedOffsetForces() {
    this->m1m3SAL->logEvent_appliedOffsetForces(&this->eventAppliedOffsetForces, 0);
    this->previousEventAppliedOffsetForces = this->eventAppliedOffsetForces;
}

void M1M3SSPublisher::tryLogAppliedOffsetForces() {
    bool changeDetected =
            this->eventAppliedOffsetForces.fX != this->previousEventAppliedOffsetForces.fX ||
            this->eventAppliedOffsetForces.fY != this->previousEventAppliedOffsetForces.fY ||
            this->eventAppliedOffsetForces.fZ != this->previousEventAppliedOffsetForces.fZ ||
            this->eventAppliedOffsetForces.mX != this->previousEventAppliedOffsetForces.mX ||
            this->eventAppliedOffsetForces.mY != this->previousEventAppliedOffsetForces.mY ||
            this->eventAppliedOffsetForces.mZ != this->previousEventAppliedOffsetForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedOffsetForces.xForces[i] != this->previousEventAppliedOffsetForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedOffsetForces.yForces[i] != this->previousEventAppliedOffsetForces.yForces[i]) ||
                         (this->eventAppliedOffsetForces.zForces[i] != this->previousEventAppliedOffsetForces.zForces[i]);
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
    if (this->eventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue != this->previousEventAppliedSettingsMatchStart.appliedSettingsMatchStartIsTrue) {
        this->logAppliedSettingsMatchStart();
    }
}

void M1M3SSPublisher::logAppliedStaticForces() {
    this->m1m3SAL->logEvent_appliedStaticForces(&this->eventAppliedStaticForces, 0);
    this->previousEventAppliedStaticForces = this->eventAppliedStaticForces;
}

void M1M3SSPublisher::tryLogAppliedStaticForces() {
    bool changeDetected =
            this->eventAppliedStaticForces.fX != this->previousEventAppliedStaticForces.fX ||
            this->eventAppliedStaticForces.fY != this->previousEventAppliedStaticForces.fY ||
            this->eventAppliedStaticForces.fZ != this->previousEventAppliedStaticForces.fZ ||
            this->eventAppliedStaticForces.mX != this->previousEventAppliedStaticForces.mX ||
            this->eventAppliedStaticForces.mY != this->previousEventAppliedStaticForces.mY ||
            this->eventAppliedStaticForces.mZ != this->previousEventAppliedStaticForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedStaticForces.xForces[i] != this->previousEventAppliedStaticForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedStaticForces.yForces[i] != this->previousEventAppliedStaticForces.yForces[i]) ||
                         (this->eventAppliedStaticForces.zForces[i] != this->previousEventAppliedStaticForces.zForces[i]);
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
    bool changeDetected =
            this->eventAppliedThermalForces.fX != this->previousEventAppliedThermalForces.fX ||
            this->eventAppliedThermalForces.fY != this->previousEventAppliedThermalForces.fY ||
            this->eventAppliedThermalForces.fZ != this->previousEventAppliedThermalForces.fZ ||
            this->eventAppliedThermalForces.mX != this->previousEventAppliedThermalForces.mX ||
            this->eventAppliedThermalForces.mY != this->previousEventAppliedThermalForces.mY ||
            this->eventAppliedThermalForces.mZ != this->previousEventAppliedThermalForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedThermalForces.xForces[i] != this->previousEventAppliedThermalForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedThermalForces.yForces[i] != this->previousEventAppliedThermalForces.yForces[i]) ||
                         (this->eventAppliedThermalForces.zForces[i] != this->previousEventAppliedThermalForces.zForces[i]);
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
            this->eventAppliedVelocityForces.fX != this->previousEventAppliedVelocityForces.fX ||
            this->eventAppliedVelocityForces.fY != this->previousEventAppliedVelocityForces.fY ||
            this->eventAppliedVelocityForces.fZ != this->previousEventAppliedVelocityForces.fZ ||
            this->eventAppliedVelocityForces.mX != this->previousEventAppliedVelocityForces.mX ||
            this->eventAppliedVelocityForces.mY != this->previousEventAppliedVelocityForces.mY ||
            this->eventAppliedVelocityForces.mZ != this->previousEventAppliedVelocityForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventAppliedVelocityForces.xForces[i] != this->previousEventAppliedVelocityForces.xForces[i]) ||
                         (i < 100 && this->eventAppliedVelocityForces.yForces[i] != this->previousEventAppliedVelocityForces.yForces[i]) ||
                         (this->eventAppliedVelocityForces.zForces[i] != this->previousEventAppliedVelocityForces.zForces[i]);
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
    if (this->eventCellLightStatus.cellLightsCommandedOn != this->previousEventCellLightStatus.cellLightsCommandedOn ||
        this->eventCellLightStatus.cellLightsOn != this->previousEventCellLightStatus.cellLightsOn) {
        this->logCellLightStatus();
    }
}

void M1M3SSPublisher::logCellLightWarning() {
    this->eventCellLightWarning.anyWarning = this->eventCellLightWarning.cellLightFlags != 0;
    this->m1m3SAL->logEvent_cellLightWarning(&this->eventCellLightWarning, 0);
    this->previousEventCellLightWarning = this->eventCellLightWarning;
}

void M1M3SSPublisher::tryLogCellLightWarning() {
    if (this->eventCellLightWarning.cellLightFlags != this->previousEventCellLightWarning.cellLightFlags) {
        this->logCellLightWarning();
    }
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
    this->eventDisplacementSensorWarning.anyWarning = this->eventDisplacementSensorWarning.displacementSensorFlags != 0;
    this->m1m3SAL->logEvent_displacementSensorWarning(&this->eventDisplacementSensorWarning, 0);
    this->previousEventDisplacementSensorWarning = this->eventDisplacementSensorWarning;
}

void M1M3SSPublisher::tryLogDisplacementSensorWarning() {
    if (this->eventDisplacementSensorWarning.displacementSensorFlags != this->previousEventDisplacementSensorWarning.displacementSensorFlags) {
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

void M1M3SSPublisher::logForceActuatorBackupCalibrationInfo() {
    this->m1m3SAL->logEvent_forceActuatorBackupCalibrationInfo(&this->eventForceActuatorBackupCalibrationInfo, 0);
    this->previousEventForceActuatorBackupCalibrationInfo = this->eventForceActuatorBackupCalibrationInfo;
}

void M1M3SSPublisher::tryLogForceActuatorBackupCalibrationInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorBackupCalibrationInfo.primaryCoefficient[i] != this->previousEventForceActuatorBackupCalibrationInfo.primaryCoefficient[i] ||
                         this->eventForceActuatorBackupCalibrationInfo.primaryOffset[i] != this->previousEventForceActuatorBackupCalibrationInfo.primaryOffset[i] ||
                         this->eventForceActuatorBackupCalibrationInfo.primarySensitivity[i] != this->previousEventForceActuatorBackupCalibrationInfo.primarySensitivity[i];
        if (i < 112) {
            changeDetected = changeDetected ||
                             this->eventForceActuatorBackupCalibrationInfo.secondaryCoefficient[i] != this->previousEventForceActuatorBackupCalibrationInfo.secondaryCoefficient[i] ||
                             this->eventForceActuatorBackupCalibrationInfo.secondaryOffset[i] != this->previousEventForceActuatorBackupCalibrationInfo.secondaryOffset[i] ||
                             this->eventForceActuatorBackupCalibrationInfo.secondarySensitivity[i] != this->previousEventForceActuatorBackupCalibrationInfo.secondarySensitivity[i];
        }
    }
    if (changeDetected) {
        this->logForceActuatorBackupCalibrationInfo();
    }
}

void M1M3SSPublisher::logForceActuatorIdInfo() {
    this->m1m3SAL->logEvent_forceActuatorIdInfo(&this->eventForceActuatorIdInfo, 0);
    this->previousEventForceActuatorIdInfo = this->eventForceActuatorIdInfo;
}

void M1M3SSPublisher::tryLogForceActuatorIdInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorIdInfo.zDataReferenceId[i] != this->previousEventForceActuatorIdInfo.zDataReferenceId[i] ||
                         this->eventForceActuatorIdInfo.ilcUniqueId[i] != this->previousEventForceActuatorIdInfo.ilcUniqueId[i] ||
                         this->eventForceActuatorIdInfo.mezzanineUniqueId[i] != this->previousEventForceActuatorIdInfo.mezzanineUniqueId[i];
        if (i < 12) {
            changeDetected = changeDetected ||
                             this->eventForceActuatorIdInfo.xDataReferenceId[i] != this->previousEventForceActuatorIdInfo.xDataReferenceId[i];
        }
        if (i < 100) {
            changeDetected = changeDetected ||
                             this->eventForceActuatorIdInfo.yDataReferenceId[i] != this->previousEventForceActuatorIdInfo.yDataReferenceId[i];
        }
        if (i < 112) {
            changeDetected = changeDetected ||
                             this->eventForceActuatorIdInfo.sDataReferenceId[i] != this->previousEventForceActuatorIdInfo.sDataReferenceId[i];
        }
    }
    if (changeDetected) {
        this->logForceActuatorIdInfo();
    }
}

void M1M3SSPublisher::logForceActuatorILCInfo() {
    this->m1m3SAL->logEvent_forceActuatorILCInfo(&this->eventForceActuatorILCInfo, 0);
    this->previousEventForceActuatorILCInfo = this->eventForceActuatorILCInfo;
}

void M1M3SSPublisher::tryLogForceActuatorILCInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorILCInfo.modbusSubnet[i] != this->previousEventForceActuatorILCInfo.modbusSubnet[i] ||
                         this->eventForceActuatorILCInfo.modbusAddress[i] != this->previousEventForceActuatorILCInfo.modbusAddress[i] ||
                         this->eventForceActuatorILCInfo.ilcStatus[i] != this->previousEventForceActuatorILCInfo.ilcStatus[i] ||
                         this->eventForceActuatorILCInfo.mezzanineStatus[i] != this->previousEventForceActuatorILCInfo.mezzanineStatus[i];
    }
    if (changeDetected) {
        this->logForceActuatorILCInfo();
    }
}

void M1M3SSPublisher::logForceActuatorMainCalibrationInfo() {
    this->m1m3SAL->logEvent_forceActuatorMainCalibrationInfo(&this->eventForceActuatorMainCalibrationInfo, 0);
    this->previousEventForceActuatorMainCalibrationInfo = this->eventForceActuatorMainCalibrationInfo;
}

void M1M3SSPublisher::tryLogForceActuatorMainCalibrationInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorMainCalibrationInfo.primaryCoefficient[i] != this->previousEventForceActuatorMainCalibrationInfo.primaryCoefficient[i] ||
                         this->eventForceActuatorMainCalibrationInfo.primaryOffset[i] != this->previousEventForceActuatorMainCalibrationInfo.primaryOffset[i] ||
                         this->eventForceActuatorMainCalibrationInfo.primarySensitivity[i] != this->previousEventForceActuatorMainCalibrationInfo.primarySensitivity[i];
        if (i < 112) {
            changeDetected = changeDetected ||
                             this->eventForceActuatorMainCalibrationInfo.secondaryCoefficient[i] != this->previousEventForceActuatorMainCalibrationInfo.secondaryCoefficient[i] ||
                             this->eventForceActuatorMainCalibrationInfo.secondaryOffset[i] != this->previousEventForceActuatorMainCalibrationInfo.secondaryOffset[i] ||
                             this->eventForceActuatorMainCalibrationInfo.secondarySensitivity[i] != this->previousEventForceActuatorMainCalibrationInfo.secondarySensitivity[i];
        }
    }
    if (changeDetected) {
        this->logForceActuatorMainCalibrationInfo();
    }
}

void M1M3SSPublisher::logForceActuatorMezzanineCalibrationInfo() {
    this->m1m3SAL->logEvent_forceActuatorMezzanineCalibrationInfo(&this->eventForceActuatorMezzanineCalibrationInfo, 0);
    this->previousEventForceActuatorMezzanineCalibrationInfo = this->eventForceActuatorMezzanineCalibrationInfo;
}

void M1M3SSPublisher::tryLogForceActuatorMezzanineCalibrationInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorMezzanineCalibrationInfo.primaryCylinderGain[i] != this->previousEventForceActuatorMezzanineCalibrationInfo.primaryCylinderGain[i];
        if (i < 112) {
            changeDetected = changeDetected ||
                             this->eventForceActuatorMezzanineCalibrationInfo.secondaryCylinderGain[i] != this->previousEventForceActuatorMezzanineCalibrationInfo.secondaryCylinderGain[i];
        }
    }
    if (changeDetected) {
        this->logForceActuatorMezzanineCalibrationInfo();
    }
}

void M1M3SSPublisher::logForceActuatorPositionInfo() {
    this->m1m3SAL->logEvent_forceActuatorPositionInfo(&this->eventForceActuatorPositionInfo, 0);
    this->previousEventForceActuatorPositionInfo = this->eventForceActuatorPositionInfo;
}

void M1M3SSPublisher::tryLogForceActuatorPositionInfo() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorPositionInfo.actuatorType[i] != this->previousEventForceActuatorPositionInfo.actuatorType[i] ||
                         this->eventForceActuatorPositionInfo.actuatorOrientation[i] != this->previousEventForceActuatorPositionInfo.actuatorOrientation[i] ||
                         this->eventForceActuatorPositionInfo.xPosition[i] != this->previousEventForceActuatorPositionInfo.xPosition[i] ||
                         this->eventForceActuatorPositionInfo.yPosition[i] != this->previousEventForceActuatorPositionInfo.yPosition[i] ||
                         this->eventForceActuatorPositionInfo.zPosition[i] != this->previousEventForceActuatorPositionInfo.zPosition[i];
    }
    if (changeDetected) {
        this->logForceActuatorPositionInfo();
    }
}

void M1M3SSPublisher::logForceActuatorState() {
    this->m1m3SAL->logEvent_forceActuatorState(&this->eventForceActuatorState, 0);
    this->previousEventForceActuatorState = this->eventForceActuatorState;
}

void M1M3SSPublisher::tryLogForceActuatorState() {
    bool changeDetected =
            this->eventForceActuatorState.slewFlag != this->previousEventForceActuatorState.slewFlag ||
            this->eventForceActuatorState.staticForcesApplied != this->previousEventForceActuatorState.staticForcesApplied ||
            this->eventForceActuatorState.elevationForcesApplied != this->previousEventForceActuatorState.elevationForcesApplied ||
            this->eventForceActuatorState.azimuthForcesApplied != this->previousEventForceActuatorState.azimuthForcesApplied ||
            this->eventForceActuatorState.thermalForcesApplied != this->previousEventForceActuatorState.thermalForcesApplied ||
            this->eventForceActuatorState.offsetForcesApplied != this->previousEventForceActuatorState.offsetForcesApplied ||
            this->eventForceActuatorState.accelerationForcesApplied != this->previousEventForceActuatorState.accelerationForcesApplied ||
            this->eventForceActuatorState.velocityForcesApplied != this->previousEventForceActuatorState.velocityForcesApplied ||
            this->eventForceActuatorState.activeOpticForcesApplied != this->previousEventForceActuatorState.activeOpticForcesApplied ||
            this->eventForceActuatorState.aberrationForcesApplied != this->previousEventForceActuatorState.aberrationForcesApplied ||
            this->eventForceActuatorState.balanceForcesApplied != this->previousEventForceActuatorState.balanceForcesApplied ||
            this->eventForceActuatorState.supportPercentage != this->previousEventForceActuatorState.supportPercentage;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorState.ilcState[i] != this->previousEventForceActuatorState.ilcState[i];
    }
    if (changeDetected) {
        this->logForceActuatorState();
    }
}

void M1M3SSPublisher::logForceActuatorWarning() {
    this->eventForceActuatorWarning.anyForceActuatorFlags = 0;
    for (int i = 0; i < FA_COUNT; ++i) {
        this->eventForceActuatorWarning.anyForceActuatorFlags =
                this->eventForceActuatorWarning.anyForceActuatorFlags | this->eventForceActuatorWarning.forceActuatorFlags[i];
    }
    this->eventForceActuatorWarning.anyWarning =
            this->eventForceActuatorWarning.globalWarningFlags != 0 ||
            this->eventForceActuatorWarning.anyForceActuatorFlags != 0;
    this->m1m3SAL->logEvent_forceActuatorWarning(&this->eventForceActuatorWarning, 0);
    this->previousEventForceActuatorWarning = this->eventForceActuatorWarning;
}

void M1M3SSPublisher::tryLogForceActuatorWarning() {
    bool changeDetected =
            this->eventForceActuatorWarning.globalWarningFlags != this->previousEventForceActuatorWarning.globalWarningFlags;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventForceActuatorWarning.forceActuatorFlags[i] != this->previousEventForceActuatorWarning.forceActuatorFlags[i];
    }
    if (changeDetected) {
        this->logForceActuatorWarning();
    }
}

void M1M3SSPublisher::logGyroWarning() {
    this->eventGyroWarning.anyWarning = this->eventGyroWarning.gyroSensorFlags != 0;
    this->m1m3SAL->logEvent_gyroWarning(&this->eventGyroWarning, 0);
    this->previousEventGyroWarning = this->eventGyroWarning;
}

void M1M3SSPublisher::tryLogGyroWarning() {
    if (this->eventGyroWarning.gyroSensorFlags != this->previousEventGyroWarning.gyroSensorFlags) {
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
                         this->eventHardpointActuatorInfo.referenceId[i] != this->previousEventHardpointActuatorInfo.referenceId[i] ||
                         this->eventHardpointActuatorInfo.referencePosition[i] != this->previousEventHardpointActuatorInfo.referencePosition[i] ||
                         this->eventHardpointActuatorInfo.modbusSubnet[i] != this->previousEventHardpointActuatorInfo.modbusSubnet[i] ||
                         this->eventHardpointActuatorInfo.modbusAddress[i] != this->previousEventHardpointActuatorInfo.modbusAddress[i] ||
                         this->eventHardpointActuatorInfo.xPosition[i] != this->previousEventHardpointActuatorInfo.xPosition[i] ||
                         this->eventHardpointActuatorInfo.yPosition[i] != this->previousEventHardpointActuatorInfo.yPosition[i] ||
                         this->eventHardpointActuatorInfo.zPosition[i] != this->previousEventHardpointActuatorInfo.zPosition[i] ||
                         this->eventHardpointActuatorInfo.ilcUniqueId[i] != this->previousEventHardpointActuatorInfo.ilcUniqueId[i] ||
                         this->eventHardpointActuatorInfo.ilcApplicationType[i] != this->previousEventHardpointActuatorInfo.ilcApplicationType[i] ||
                         this->eventHardpointActuatorInfo.networkNodeType[i] != this->previousEventHardpointActuatorInfo.networkNodeType[i] ||
                         this->eventHardpointActuatorInfo.ilcSelectedOptions[i] != this->previousEventHardpointActuatorInfo.ilcSelectedOptions[i] ||
                         this->eventHardpointActuatorInfo.networkNodeOptions[i] != this->previousEventHardpointActuatorInfo.networkNodeOptions[i] ||
                         this->eventHardpointActuatorInfo.majorRevision[i] != this->previousEventHardpointActuatorInfo.majorRevision[i] ||
                         this->eventHardpointActuatorInfo.minorRevision[i] != this->previousEventHardpointActuatorInfo.minorRevision[i] ||
                         this->eventHardpointActuatorInfo.adcScanRate[i] != this->previousEventHardpointActuatorInfo.adcScanRate[i] ||
                         this->eventHardpointActuatorInfo.mainLoadCellCoefficient[i] != this->previousEventHardpointActuatorInfo.mainLoadCellCoefficient[i] ||
                         this->eventHardpointActuatorInfo.mainLoadCellOffset[i] != this->previousEventHardpointActuatorInfo.mainLoadCellOffset[i] ||
                         this->eventHardpointActuatorInfo.mainLoadCellSensitivity[i] != this->previousEventHardpointActuatorInfo.mainLoadCellSensitivity[i] ||
                         this->eventHardpointActuatorInfo.backupLoadCellCoefficient[i] != this->previousEventHardpointActuatorInfo.backupLoadCellCoefficient[i] ||
                         this->eventHardpointActuatorInfo.backupLoadCellOffset[i] != this->previousEventHardpointActuatorInfo.backupLoadCellOffset[i] ||
                         this->eventHardpointActuatorInfo.backupLoadCellSensitivity[i] != this->previousEventHardpointActuatorInfo.backupLoadCellSensitivity[i];
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
                         this->eventHardpointActuatorState.ilcState[i] != this->previousEventHardpointActuatorState.ilcState[i] ||
                         this->eventHardpointActuatorState.motionState[i] != this->previousEventHardpointActuatorState.motionState[i];
    }
    if (changeDetected) {
        this->logHardpointActuatorState();
    }
}

void M1M3SSPublisher::logHardpointActuatorWarning() {
    this->eventHardpointActuatorWarning.anyHardpointActuatorFlags = 0;
    for (int i = 0; i < HP_COUNT; ++i) {
        this->eventHardpointActuatorWarning.anyHardpointActuatorFlags =
                this->eventHardpointActuatorWarning.anyHardpointActuatorFlags | this->eventHardpointActuatorWarning.hardpointActuatorFlags[i];
    }
    this->eventHardpointActuatorWarning.anyWarning = this->eventHardpointActuatorWarning.anyHardpointActuatorFlags != 0;
    this->m1m3SAL->logEvent_hardpointActuatorWarning(&this->eventHardpointActuatorWarning, 0);
    this->previousEventHardpointActuatorWarning = this->eventHardpointActuatorWarning;
}

void M1M3SSPublisher::tryLogHardpointActuatorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventHardpointActuatorWarning.hardpointActuatorFlags[i] != this->previousEventHardpointActuatorWarning.hardpointActuatorFlags[i];
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
                         this->eventHardpointMonitorInfo.referenceId[i] != this->previousEventHardpointMonitorInfo.referenceId[i] ||
                         this->eventHardpointMonitorInfo.modbusSubnet[i] != this->previousEventHardpointMonitorInfo.modbusSubnet[i] ||
                         this->eventHardpointMonitorInfo.modbusAddress[i] != this->previousEventHardpointMonitorInfo.modbusAddress[i] ||
                         this->eventHardpointMonitorInfo.ilcUniqueId[i] != this->previousEventHardpointMonitorInfo.ilcUniqueId[i] ||
                         this->eventHardpointMonitorInfo.ilcApplicationType[i] != this->previousEventHardpointMonitorInfo.ilcApplicationType[i] ||
                         this->eventHardpointMonitorInfo.networkNodeType[i] != this->previousEventHardpointMonitorInfo.networkNodeType[i] ||
                         this->eventHardpointMonitorInfo.majorRevision[i] != this->previousEventHardpointMonitorInfo.majorRevision[i] ||
                         this->eventHardpointMonitorInfo.minorRevision[i] != this->previousEventHardpointMonitorInfo.minorRevision[i] ||
                         this->eventHardpointMonitorInfo.mezzanineUniqueId[i] != this->previousEventHardpointMonitorInfo.mezzanineUniqueId[i] ||
                         this->eventHardpointMonitorInfo.mezzanineFirmwareType[i] != this->previousEventHardpointMonitorInfo.mezzanineFirmwareType[i] ||
                         this->eventHardpointMonitorInfo.mezzanineMajorRevision[i] != this->previousEventHardpointMonitorInfo.mezzanineMajorRevision[i] ||
                         this->eventHardpointMonitorInfo.mezzanineMinorRevision[i] != this->previousEventHardpointMonitorInfo.mezzanineMinorRevision[i];
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
        changeDetected = changeDetected ||
                         this->eventHardpointMonitorState.ilcState[i] != this->previousEventHardpointMonitorState.ilcState[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorState();
    }
}

void M1M3SSPublisher::logHardpointMonitorWarning() {
    this->eventHardpointMonitorWarning.anyHardpointMonitorFlags = 0;
    for (int i = 0; i < HM_COUNT; ++i) {
        this->eventHardpointMonitorWarning.anyHardpointMonitorFlags =
                this->eventHardpointMonitorWarning.anyHardpointMonitorFlags | this->eventHardpointMonitorWarning.hardpointMonitorFlags[i];
    }
    this->eventHardpointMonitorWarning.anyWarning =
            this->eventHardpointMonitorWarning.anyHardpointMonitorFlags != 0;
    this->m1m3SAL->logEvent_hardpointMonitorWarning(&this->eventHardpointMonitorWarning, 0);
    this->previousEventHardpointMonitorWarning = this->eventHardpointMonitorWarning;
}

void M1M3SSPublisher::tryLogHardpointMonitorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HM_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventHardpointMonitorWarning.hardpointMonitorFlags[i] != this->previousEventHardpointMonitorWarning.hardpointMonitorFlags[i];
    }
    if (changeDetected) {
        this->logHardpointMonitorWarning();
    }
}

void M1M3SSPublisher::logInclinometerSensorWarning() {
    this->eventInclinometerSensorWarning.anyWarning =
            this->eventInclinometerSensorWarning.inclinometerSensorFlags != 0;
    this->m1m3SAL->logEvent_inclinometerSensorWarning(&this->eventInclinometerSensorWarning, 0);
    this->previousEventInclinometerSensorWarning = this->eventInclinometerSensorWarning;
}

void M1M3SSPublisher::tryLogInclinometerSensorWarning() {
    if (this->eventInclinometerSensorWarning.inclinometerSensorFlags != this->previousEventInclinometerSensorWarning.inclinometerSensorFlags) {
        this->logInclinometerSensorWarning();
    }
}

void M1M3SSPublisher::logInterlockStatus() {
    this->m1m3SAL->logEvent_interlockStatus(&this->eventInterlockStatus, 0);
    this->previousEventInterlockStatus = this->eventInterlockStatus;
}

void M1M3SSPublisher::tryLogInterlockStatus() {
    if (this->eventInterlockStatus.heartbeatCommandedState != this->previousEventInterlockStatus.heartbeatCommandedState) {
        this->logInterlockStatus();
    }
}

void M1M3SSPublisher::logInterlockWarning() {
    this->eventInterlockWarning.anyWarning =
            this->eventInterlockWarning.interlockSystemFlags != 0;
    this->m1m3SAL->logEvent_interlockWarning(&this->eventInterlockWarning, 0);
    this->previousEventInterlockWarning = this->eventInterlockWarning;
}

void M1M3SSPublisher::tryLogInterlockWarning() {
    if (this->eventInterlockWarning.interlockSystemFlags != this->previousEventInterlockWarning.interlockSystemFlags) {
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

void M1M3SSPublisher::logModbusWarning() {
    this->eventModbusWarning.anySubnetFlags =
            this->eventModbusWarning.subnetFlags[0] |
            this->eventModbusWarning.subnetFlags[1] |
            this->eventModbusWarning.subnetFlags[2] |
            this->eventModbusWarning.subnetFlags[3] |
            this->eventModbusWarning.subnetFlags[4];
    this->eventModbusWarning.anyWarning =
            this->eventModbusWarning.modbusSystemFlags != 0 ||
            this->eventModbusWarning.anySubnetFlags;
    this->m1m3SAL->logEvent_modbusWarning(&this->eventModbusWarning, 0);
    this->previousEventModbusWarning = this->eventModbusWarning;
}

void M1M3SSPublisher::tryLogModbusWarning() {
    if (this->eventModbusWarning.modbusSystemFlags != this->previousEventModbusWarning.modbusSystemFlags ||
        this->eventModbusWarning.subnetFlags[0] != this->previousEventModbusWarning.subnetFlags[0] ||
        this->eventModbusWarning.subnetFlags[1] != this->previousEventModbusWarning.subnetFlags[1] ||
        this->eventModbusWarning.subnetFlags[2] != this->previousEventModbusWarning.subnetFlags[2] ||
        this->eventModbusWarning.subnetFlags[3] != this->previousEventModbusWarning.subnetFlags[3] ||
        this->eventModbusWarning.subnetFlags[4] != this->previousEventModbusWarning.subnetFlags[4]) {
        this->logModbusWarning();
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
    if (this->eventPowerStatus.powerNetworkACommandedOn != this->previousEventPowerStatus.powerNetworkACommandedOn ||
        this->eventPowerStatus.powerNetworkBCommandedOn != this->previousEventPowerStatus.powerNetworkBCommandedOn ||
        this->eventPowerStatus.powerNetworkCCommandedOn != this->previousEventPowerStatus.powerNetworkCCommandedOn ||
        this->eventPowerStatus.powerNetworkDCommandedOn != this->previousEventPowerStatus.powerNetworkDCommandedOn ||
        this->eventPowerStatus.auxPowerNetworkACommandedOn != this->previousEventPowerStatus.auxPowerNetworkACommandedOn ||
        this->eventPowerStatus.auxPowerNetworkBCommandedOn != this->previousEventPowerStatus.auxPowerNetworkBCommandedOn ||
        this->eventPowerStatus.auxPowerNetworkCCommandedOn != this->previousEventPowerStatus.auxPowerNetworkCCommandedOn ||
        this->eventPowerStatus.auxPowerNetworkDCommandedOn != this->previousEventPowerStatus.auxPowerNetworkDCommandedOn) {
        this->logPowerStatus();
    }
}

void M1M3SSPublisher::logPowerWarning() {
    this->eventPowerWarning.anyWarning =
            this->eventPowerWarning.powerSystemFlags != 0;
    this->m1m3SAL->logEvent_powerWarning(&this->eventPowerWarning, 0);
    this->previousEventPowerWarning = this->eventPowerWarning;
}

void M1M3SSPublisher::tryLogPowerWarning() {
    if (this->eventPowerWarning.powerSystemFlags != this->previousEventPowerWarning.powerSystemFlags) {
        this->logPowerWarning();
    }
}

void M1M3SSPublisher::logRejectedAberrationForces() {
    this->m1m3SAL->logEvent_rejectedAberrationForces(&this->eventRejectedAberrationForces, 0);
    this->previousEventRejectedAberrationForces = this->eventRejectedAberrationForces;
}

void M1M3SSPublisher::tryLogRejectedAberrationForces() {
    bool changeDetected =
            this->eventRejectedAberrationForces.fZ != this->previousEventRejectedAberrationForces.fZ ||
            this->eventRejectedAberrationForces.mX != this->previousEventRejectedAberrationForces.mX ||
            this->eventRejectedAberrationForces.mY != this->previousEventRejectedAberrationForces.mY;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventRejectedAberrationForces.zForces[i] != this->previousEventRejectedAberrationForces.zForces[i];
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
            this->eventRejectedAccelerationForces.fX != this->previousEventRejectedAccelerationForces.fX ||
            this->eventRejectedAccelerationForces.fY != this->previousEventRejectedAccelerationForces.fY ||
            this->eventRejectedAccelerationForces.fZ != this->previousEventRejectedAccelerationForces.fZ ||
            this->eventRejectedAccelerationForces.mX != this->previousEventRejectedAccelerationForces.mX ||
            this->eventRejectedAccelerationForces.mY != this->previousEventRejectedAccelerationForces.mY ||
            this->eventRejectedAccelerationForces.mZ != this->previousEventRejectedAccelerationForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedAccelerationForces.xForces[i] != this->previousEventRejectedAccelerationForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedAccelerationForces.yForces[i] != this->previousEventRejectedAccelerationForces.yForces[i]) ||
                         (this->eventRejectedAccelerationForces.zForces[i] != this->previousEventRejectedAccelerationForces.zForces[i]);
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
            this->eventRejectedActiveOpticForces.fZ != this->previousEventRejectedActiveOpticForces.fZ ||
            this->eventRejectedActiveOpticForces.mX != this->previousEventRejectedActiveOpticForces.mX ||
            this->eventRejectedActiveOpticForces.mY != this->previousEventRejectedActiveOpticForces.mY;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         this->eventRejectedActiveOpticForces.zForces[i] != this->previousEventRejectedActiveOpticForces.zForces[i];
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
            this->eventRejectedAzimuthForces.fX != this->previousEventRejectedAzimuthForces.fX ||
            this->eventRejectedAzimuthForces.fY != this->previousEventRejectedAzimuthForces.fY ||
            this->eventRejectedAzimuthForces.fZ != this->previousEventRejectedAzimuthForces.fZ ||
            this->eventRejectedAzimuthForces.mX != this->previousEventRejectedAzimuthForces.mX ||
            this->eventRejectedAzimuthForces.mY != this->previousEventRejectedAzimuthForces.mY ||
            this->eventRejectedAzimuthForces.mZ != this->previousEventRejectedAzimuthForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedAzimuthForces.xForces[i] != this->previousEventRejectedAzimuthForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedAzimuthForces.yForces[i] != this->previousEventRejectedAzimuthForces.yForces[i]) ||
                         (this->eventRejectedAzimuthForces.zForces[i] != this->previousEventRejectedAzimuthForces.zForces[i]);
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
            this->eventRejectedBalanceForces.fX != this->previousEventRejectedBalanceForces.fX ||
            this->eventRejectedBalanceForces.fY != this->previousEventRejectedBalanceForces.fY ||
            this->eventRejectedBalanceForces.fZ != this->previousEventRejectedBalanceForces.fZ ||
            this->eventRejectedBalanceForces.mX != this->previousEventRejectedBalanceForces.mX ||
            this->eventRejectedBalanceForces.mY != this->previousEventRejectedBalanceForces.mY ||
            this->eventRejectedBalanceForces.mZ != this->previousEventRejectedBalanceForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedBalanceForces.xForces[i] != this->previousEventRejectedBalanceForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedBalanceForces.yForces[i] != this->previousEventRejectedBalanceForces.yForces[i]) ||
                         (this->eventRejectedBalanceForces.zForces[i] != this->previousEventRejectedBalanceForces.zForces[i]);
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
        changeDetected = changeDetected ||
                         (i < 112 && this->eventRejectedCylinderForces.secondaryCylinderForces[i] != this->previousEventRejectedCylinderForces.secondaryCylinderForces[i]) ||
                         (this->eventRejectedCylinderForces.primaryCylinderForces[i] != this->previousEventRejectedCylinderForces.primaryCylinderForces[i]);
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
            this->eventRejectedElevationForces.fX != this->previousEventRejectedElevationForces.fX ||
            this->eventRejectedElevationForces.fY != this->previousEventRejectedElevationForces.fY ||
            this->eventRejectedElevationForces.fZ != this->previousEventRejectedElevationForces.fZ ||
            this->eventRejectedElevationForces.mX != this->previousEventRejectedElevationForces.mX ||
            this->eventRejectedElevationForces.mY != this->previousEventRejectedElevationForces.mY ||
            this->eventRejectedElevationForces.mZ != this->previousEventRejectedElevationForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedElevationForces.xForces[i] != this->previousEventRejectedElevationForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedElevationForces.yForces[i] != this->previousEventRejectedElevationForces.yForces[i]) ||
                         (this->eventRejectedElevationForces.zForces[i] != this->previousEventRejectedElevationForces.zForces[i]);
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
    bool changeDetected =
            this->eventRejectedForces.fX != this->previousEventRejectedForces.fX ||
            this->eventRejectedForces.fY != this->previousEventRejectedForces.fY ||
            this->eventRejectedForces.fZ != this->previousEventRejectedForces.fZ ||
            this->eventRejectedForces.mX != this->previousEventRejectedForces.mX ||
            this->eventRejectedForces.mY != this->previousEventRejectedForces.mY ||
            this->eventRejectedForces.mZ != this->previousEventRejectedForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedForces.xForces[i] != this->previousEventRejectedForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedForces.yForces[i] != this->previousEventRejectedForces.yForces[i]) ||
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
    bool changeDetected =
            this->eventRejectedOffsetForces.fX != this->previousEventRejectedOffsetForces.fX ||
            this->eventRejectedOffsetForces.fY != this->previousEventRejectedOffsetForces.fY ||
            this->eventRejectedOffsetForces.fZ != this->previousEventRejectedOffsetForces.fZ ||
            this->eventRejectedOffsetForces.mX != this->previousEventRejectedOffsetForces.mX ||
            this->eventRejectedOffsetForces.mY != this->previousEventRejectedOffsetForces.mY ||
            this->eventRejectedOffsetForces.mZ != this->previousEventRejectedOffsetForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedOffsetForces.xForces[i] != this->previousEventRejectedOffsetForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedOffsetForces.yForces[i] != this->previousEventRejectedOffsetForces.yForces[i]) ||
                         (this->eventRejectedOffsetForces.zForces[i] != this->previousEventRejectedOffsetForces.zForces[i]);
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
    bool changeDetected =
            this->eventRejectedStaticForces.fX != this->previousEventRejectedStaticForces.fX ||
            this->eventRejectedStaticForces.fY != this->previousEventRejectedStaticForces.fY ||
            this->eventRejectedStaticForces.fZ != this->previousEventRejectedStaticForces.fZ ||
            this->eventRejectedStaticForces.mX != this->previousEventRejectedStaticForces.mX ||
            this->eventRejectedStaticForces.mY != this->previousEventRejectedStaticForces.mY ||
            this->eventRejectedStaticForces.mZ != this->previousEventRejectedStaticForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedStaticForces.xForces[i] != this->previousEventRejectedStaticForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedStaticForces.yForces[i] != this->previousEventRejectedStaticForces.yForces[i]) ||
                         (this->eventRejectedStaticForces.zForces[i] != this->previousEventRejectedStaticForces.zForces[i]);
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
            this->eventRejectedThermalForces.fX != this->previousEventRejectedThermalForces.fX ||
            this->eventRejectedThermalForces.fY != this->previousEventRejectedThermalForces.fY ||
            this->eventRejectedThermalForces.fZ != this->previousEventRejectedThermalForces.fZ ||
            this->eventRejectedThermalForces.mX != this->previousEventRejectedThermalForces.mX ||
            this->eventRejectedThermalForces.mY != this->previousEventRejectedThermalForces.mY ||
            this->eventRejectedThermalForces.mZ != this->previousEventRejectedThermalForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedThermalForces.xForces[i] != this->previousEventRejectedThermalForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedThermalForces.yForces[i] != this->previousEventRejectedThermalForces.yForces[i]) ||
                         (this->eventRejectedThermalForces.zForces[i] != this->previousEventRejectedThermalForces.zForces[i]);
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
            this->eventRejectedVelocityForces.fX != this->previousEventRejectedVelocityForces.fX ||
            this->eventRejectedVelocityForces.fY != this->previousEventRejectedVelocityForces.fY ||
            this->eventRejectedVelocityForces.fZ != this->previousEventRejectedVelocityForces.fZ ||
            this->eventRejectedVelocityForces.mX != this->previousEventRejectedVelocityForces.mX ||
            this->eventRejectedVelocityForces.mY != this->previousEventRejectedVelocityForces.mY ||
            this->eventRejectedVelocityForces.mZ != this->previousEventRejectedVelocityForces.mZ;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         (i < 12 && this->eventRejectedVelocityForces.xForces[i] != this->previousEventRejectedVelocityForces.xForces[i]) ||
                         (i < 100 && this->eventRejectedVelocityForces.yForces[i] != this->previousEventRejectedVelocityForces.yForces[i]) ||
                         (this->eventRejectedVelocityForces.zForces[i] != this->previousEventRejectedVelocityForces.zForces[i]);
    }
    if (changeDetected) {
        this->logRejectedVelocityForces();
    }
}

void M1M3SSPublisher::logSettingVersions() {
    this->m1m3SAL->logEvent_settingVersions(&this->eventSettingVersions, 0);
    this->previousEventSettingVersions = this->eventSettingVersions;
}

void M1M3SSPublisher::tryLogSettingVersions() {
    if (this->eventSettingVersions.recommendedSettingsLabels.compare(this->previousEventSettingVersions.recommendedSettingsLabels) != 0 ||
        this->eventSettingVersions.recommendedSettingsVersion.compare(this->previousEventSettingVersions.recommendedSettingsVersion) != 0) {
        this->logSettingVersions();
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

void M1M3SSPublisher::ackCommandStart(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_start(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnable(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_enable(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisable(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_disable(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandStandby(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_standby(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandShutdown(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_shutdown(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_turnAirOn(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_turnAirOff(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyOffsetForces(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_applyOffsetForces(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearOffsetForces(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_clearOffsetForces(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandRaiseM1M3(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_raiseM1M3(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandLowerM1M3(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_lowerM1M3(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyActiveOpticForcesByBendingModes(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_applyActiveOpticForcesByBendingModes(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyActiveOpticForces(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_applyActiveOpticForces(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearActiveOpticForces(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_clearActiveOpticForces(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAberrationForcesByBendingModes(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_applyAberrationForcesByBendingModes(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAberrationForces(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_applyAberrationForces(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearAberrationForces(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_clearAberrationForces(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnterEngineering(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_enterEngineering(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandExitEngineering(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_exitEngineering(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTestAir(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_testAir(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTestHardpoint(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_testHardpoint(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTestForceActuator(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_testForceActuator(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandMoveHardpointActuators(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_moveHardpointActuators(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnableHardpointChase(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_enableHardpointChase(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisableHardpointChase(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_disableHardpointChase(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandAbortRaiseM1M3(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_abortRaiseM1M3(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTranslateM1M3(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_translateM1M3(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandStopHardpointMotion(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_stopHardpointMotion(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandPositionM1M3(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_positionM1M3(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnLightsOn(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_turnLightsOn(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnLightsOff(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_turnLightsOff(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnPowerOn(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_turnPowerOn(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnPowerOff(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_turnPowerOff(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnableHardpointCorrections(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_enableHardpointCorrections(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisableHardpointCorrections(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_disableHardpointCorrections(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyOffsetForcesByMirrorForce(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_applyOffsetForcesByMirrorForce(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandUpdatePID(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_updatePID(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandResetPID(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_resetPID(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandProgramILC(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_programILC(commandID, ackCode, errorCode, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandModbusTransmit(int32_t commandID, int32_t ackCode, int32_t errorCode, std::string description) {
    this->m1m3SAL->ackCommand_modbusTransmit(commandID, ackCode, errorCode, (char*)description.c_str());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
