#include <ForceController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorSettings.h>
#include <ForceActuatorLimits.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <PIDSettings.h>
#include <Range.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <ForceConverter.h>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                 ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings,
                                 M1M3SSPublisher* publisher, SafetyController* safetyController)
        : aberrationForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                   forceActuatorSettings),
          accelerationForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                     forceActuatorSettings),
          activeOpticForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                    forceActuatorSettings),
          azimuthForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                forceActuatorSettings),
          balanceForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                forceActuatorSettings, pidSettings),
          elevationForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                  forceActuatorSettings),
          offsetForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                               forceActuatorSettings),
          staticForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                               forceActuatorSettings),
          thermalForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                forceActuatorSettings),
          velocityForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                                 forceActuatorSettings),
          finalForceComponent(publisher, safetyController, forceActuatorApplicationSettings,
                              forceActuatorSettings) {
    spdlog::debug("ForceController: ForceController()");
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->publisher = publisher;
    this->safetyController = safetyController;
    this->pidSettings = pidSettings;

    this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
    this->appliedForces = this->publisher->getEventAppliedForces();
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->rejectedCylinderForces = this->publisher->getEventRejectedCylinderForces();
    this->rejectedForces = this->publisher->getEventRejectedForces();

    this->forceActuatorInfo = this->publisher->getEventForceActuatorInfo();
    this->inclinometerData = this->publisher->getInclinometerData();
    this->forceActuatorData = this->publisher->getForceActuatorData();

    this->pidData = this->publisher->getPIDData();
    this->pidInfo = this->publisher->getEventPIDInfo();
    this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
    this->accelerometerData = this->publisher->getAccelerometerData();
    this->gyroData = this->publisher->getGyroData();

    memset(this->forceActuatorData, 0, sizeof(MTM1M3_forceActuatorDataC));
    memset(&this->tmaAzimuthData, 0, sizeof(MTMount_AzimuthC));
    memset(&this->tmaElevationData, 0, sizeof(MTMount_ElevationC));

    memset(this->appliedForces, 0, sizeof(MTM1M3_logevent_appliedForcesC));
    memset(this->forceActuatorState, 0, sizeof(MTM1M3_logevent_forceActuatorStateC));
    memset(this->forceSetpointWarning, 0, sizeof(MTM1M3_logevent_forceSetpointWarningC));
    memset(this->rejectedCylinderForces, 0, sizeof(MTM1M3_logevent_rejectedCylinderForcesC));
    memset(this->rejectedForces, 0, sizeof(MTM1M3_logevent_rejectedForcesC));

    this->aberrationForceComponent.reset();
    this->accelerationForceComponent.reset();
    this->activeOpticForceComponent.reset();
    this->azimuthForceComponent.reset();
    this->balanceForceComponent.reset();
    this->elevationForceComponent.reset();
    this->offsetForceComponent.reset();
    this->staticForceComponent.reset();
    this->thermalForceComponent.reset();
    this->velocityForceComponent.reset();
    this->finalForceComponent.reset();

    double timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->timestamp = timestamp;
    this->forceSetpointWarning->timestamp = timestamp;

    this->publisher->logForceActuatorState();
    this->publisher->logForceSetpointWarning();

    this->mirrorWeight = 0.0;
    float* zForces =
            ForceConverter::calculateForceFromElevationAngle(this->forceActuatorSettings, 0.0).ZForces;
    for (int i = 0; i < FA_COUNT; i++) {
        this->mirrorWeight += zForces[i];
        this->zero[i] = 0;
        ForceActuatorNeighbors neighbors;
        for (unsigned int j = 0; j < this->forceActuatorSettings->Neighbors[i].NearZIDs.size(); ++j) {
            int32_t id = this->forceActuatorSettings->Neighbors[i].NearZIDs[j];
            for (unsigned int k = 0; k < this->forceActuatorApplicationSettings->Table.size(); ++k) {
                if (this->forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
                    neighbors.NearZIDs.push_back(k);
                    break;
                }
            }
        }
        for (unsigned int j = 0; j < this->forceActuatorSettings->Neighbors[i].FarIDs.size(); ++j) {
            int32_t id = this->forceActuatorSettings->Neighbors[i].FarIDs[j];
            for (unsigned int k = 0; k < this->forceActuatorApplicationSettings->Table.size(); ++k) {
                if (this->forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
                    neighbors.FarIDs.push_back(k);
                    break;
                }
            }
        }
        this->neighbors.push_back(neighbors);
    }
}

void ForceController::reset() {
    spdlog::info("ForceController: reset()");
    this->aberrationForceComponent.reset();
    this->accelerationForceComponent.reset();
    this->activeOpticForceComponent.reset();
    this->azimuthForceComponent.reset();
    this->balanceForceComponent.reset();
    this->elevationForceComponent.reset();
    this->offsetForceComponent.reset();
    this->staticForceComponent.reset();
    this->thermalForceComponent.reset();
    this->velocityForceComponent.reset();
    this->finalForceComponent.reset();
}

void ForceController::updateTMAElevationData(MTMount_ElevationC* tmaElevationData) {
    spdlog::trace("ForceController: updateTMAElevationData()");
    memcpy(&this->tmaElevationData, tmaElevationData, sizeof(MTMount_ElevationC));
}

void ForceController::incSupportPercentage() {
    spdlog::trace("ForceController: incSupportPercentage()");
    this->forceActuatorState->supportPercentage += this->forceActuatorSettings->RaiseIncrementPercentage;
    if (this->supportPercentageFilled()) {
        this->forceActuatorState->supportPercentage = 1.0;
    }
}

void ForceController::decSupportPercentage() {
    spdlog::trace("ForceController: decSupportPercentage()");
    this->forceActuatorState->supportPercentage -= this->forceActuatorSettings->LowerDecrementPercentage;
    if (this->supportPercentageZeroed()) {
        this->forceActuatorState->supportPercentage = 0.0;
    }
}

void ForceController::zeroSupportPercentage() {
    spdlog::info("ForceController: zeroSupportPercentage()");
    this->forceActuatorState->supportPercentage = 0.0;
}

void ForceController::fillSupportPercentage() {
    spdlog::info("ForceController: fillSupportPercentage()");
    this->forceActuatorState->supportPercentage = 1.0;
}

bool ForceController::supportPercentageFilled() { return this->forceActuatorState->supportPercentage >= 1.0; }

bool ForceController::supportPercentageZeroed() { return this->forceActuatorState->supportPercentage <= 0.0; }

bool ForceController::followingErrorInTolerance() {
    spdlog::trace("ForceController: followingErrorInTolerance()");
    float limit = this->forceActuatorSettings->RaiseLowerFollowingErrorLimit;
    for (int i = 0; i < FA_COUNT; ++i) {
        if ((i < FA_X_COUNT &&
             !Range::InRange(-limit, limit,
                             this->forceActuatorData->xForce[i] - this->appliedForces->xForces[i])) ||
            (i < FA_Y_COUNT &&
             !Range::InRange(-limit, limit,
                             this->forceActuatorData->yForce[i] - this->appliedForces->yForces[i])) ||
            (!Range::InRange(-limit, limit,
                             this->forceActuatorData->zForce[i] - this->appliedForces->zForces[i]))) {
            return false;
        }
    }
    return true;
}

void ForceController::updateAppliedForces() {
    spdlog::trace("ForceController: updateAppliedForces()");

    if (this->aberrationForceComponent.isEnabled() || this->aberrationForceComponent.isDisabling()) {
        this->aberrationForceComponent.update();
    }
    if (this->accelerationForceComponent.isEnabled() || this->accelerationForceComponent.isDisabling()) {
        if (this->accelerationForceComponent.isEnabled()) {
            this->accelerationForceComponent.applyAccelerationForcesByAngularAccelerations(
                    this->accelerometerData->angularAccelerationX,
                    this->accelerometerData->angularAccelerationY,
                    this->accelerometerData->angularAccelerationZ);
        }
        this->accelerationForceComponent.update();
    }
    if (this->activeOpticForceComponent.isEnabled() || this->activeOpticForceComponent.isDisabling()) {
        this->activeOpticForceComponent.update();
    }
    if (this->azimuthForceComponent.isEnabled() || this->azimuthForceComponent.isDisabling()) {
        this->azimuthForceComponent.update();
    }
    if (this->balanceForceComponent.isEnabled() || this->balanceForceComponent.isDisabling()) {
        if (this->balanceForceComponent.isEnabled()) {
            this->balanceForceComponent.applyBalanceForcesByMirrorForces(
                    this->hardpointActuatorData->fx, this->hardpointActuatorData->fy,
                    this->hardpointActuatorData->fz, this->hardpointActuatorData->mx,
                    this->hardpointActuatorData->my, this->hardpointActuatorData->mz);
        }
        this->balanceForceComponent.update();
    }
    if (this->elevationForceComponent.isEnabled() || this->elevationForceComponent.isDisabling()) {
        if (this->elevationForceComponent.isEnabled()) {
            double elevationAngle = this->forceActuatorSettings->UseInclinometer
                                            ? this->inclinometerData->inclinometerAngle
                                            : this->tmaElevationData.Elevation_Angle_Actual;
            // Convert elevation angle to zenith angle (used by matrix)
            elevationAngle = 90.0 - elevationAngle;
            this->elevationForceComponent.applyElevationForcesByElevationAngle(elevationAngle);
        }
        this->elevationForceComponent.update();
    }
    if (this->offsetForceComponent.isEnabled() || this->offsetForceComponent.isDisabling()) {
        this->offsetForceComponent.update();
    }
    if (this->staticForceComponent.isEnabled() || this->staticForceComponent.isDisabling()) {
        this->staticForceComponent.update();
    }
    if (this->thermalForceComponent.isEnabled() || this->thermalForceComponent.isDisabling()) {
        this->thermalForceComponent.update();
    }
    if (this->velocityForceComponent.isEnabled() || this->velocityForceComponent.isDisabling()) {
        if (this->velocityForceComponent.isEnabled()) {
            this->velocityForceComponent.applyVelocityForcesByAngularVelocity(
                    this->gyroData->angularVelocityX, this->gyroData->angularVelocityY,
                    this->gyroData->angularVelocityZ);
        }
        this->velocityForceComponent.update();
    }
}

void ForceController::processAppliedForces() {
    spdlog::trace("ForceController: processAppliedForces()");
    this->sumAllForces();
    this->convertForcesToSetpoints();
    this->checkMirrorMoments();
    this->checkNearNeighbors();
    this->checkMirrorWeight();
    this->checkFarNeighbors();
    this->publisher->tryLogForceSetpointWarning();
}

void ForceController::applyAberrationForcesByBendingModes(float* coefficients) {
    spdlog::info("ForceController: applyAberrationForcesByBendingModes()");
    if (!this->aberrationForceComponent.isEnabled()) {
        this->aberrationForceComponent.enable();
    }
    this->aberrationForceComponent.applyAberrationForcesByBendingModes(coefficients);
}

void ForceController::applyAberrationForces(float* z) {
    spdlog::info("ForceController: applyAberrationForces()");
    if (!this->aberrationForceComponent.isEnabled()) {
        this->aberrationForceComponent.enable();
    }
    this->aberrationForceComponent.applyAberrationForces(z);
}

void ForceController::zeroAberrationForces() {
    spdlog::info("ForceController: zeroAberrationForces()");
    if (this->aberrationForceComponent.isEnabled()) {
        this->aberrationForceComponent.disable();
    }
}

void ForceController::applyAccelerationForces() {
    spdlog::info("ForceController: applyAccelerationForces()");
    if (!this->accelerationForceComponent.isEnabled()) {
        this->accelerationForceComponent.enable();
    }
}

void ForceController::zeroAccelerationForces() {
    spdlog::info("ForceController: zeroAccelerationForces()");
    if (this->accelerationForceComponent.isEnabled()) {
        this->accelerationForceComponent.disable();
    }
}

void ForceController::applyActiveOpticForcesByBendingModes(float* coefficients) {
    spdlog::info("ForceController: applyActiveOpticForcesByBendingModes()");
    if (!this->activeOpticForceComponent.isEnabled()) {
        this->activeOpticForceComponent.enable();
    }
    this->activeOpticForceComponent.applyActiveOpticForcesByBendingModes(coefficients);
}

void ForceController::applyActiveOpticForces(float* z) {
    spdlog::info("ForceController: applyActiveOpticForces()");
    if (!this->activeOpticForceComponent.isEnabled()) {
        this->activeOpticForceComponent.enable();
    }
    this->activeOpticForceComponent.applyActiveOpticForces(z);
}

void ForceController::zeroActiveOpticForces() {
    spdlog::info("ForceController: zeroActiveOpticForces()");
    if (this->activeOpticForceComponent.isEnabled()) {
        this->activeOpticForceComponent.disable();
    }
}

void ForceController::applyAzimuthForces() {
    spdlog::info("ForceController: applyAzimuthForces()");
    if (!this->azimuthForceComponent.isEnabled()) {
        this->azimuthForceComponent.enable();
    }
}

void ForceController::updateAzimuthForces(float azimuthAngle) {
    spdlog::trace("ForceController: updateAzimuthForces()");
    this->azimuthForceComponent.applyAzimuthForcesByAzimuthAngle(azimuthAngle);
}

void ForceController::zeroAzimuthForces() {
    spdlog::info("ForceController: zeroAzimuthForces()");
    if (this->azimuthForceComponent.isEnabled()) {
        this->azimuthForceComponent.disable();
    }
}

void ForceController::applyBalanceForces() {
    spdlog::info("ForceController: applyBalanceForces()");
    if (!this->balanceForceComponent.isEnabled()) {
        this->balanceForceComponent.enable();
    }
}

void ForceController::zeroBalanceForces() {
    spdlog::info("ForceController: zeroBalanceForces()");
    if (this->balanceForceComponent.isEnabled()) {
        this->balanceForceComponent.disable();
    }
}

void ForceController::updatePID(int id, PIDParameters parameters) {
    spdlog::info("ForceController: updatePID()");
    this->balanceForceComponent.updatePID(id, parameters);
}

void ForceController::resetPID(int id) {
    spdlog::info("ForceController: resetPID()");
    this->balanceForceComponent.resetPID(id);
}

void ForceController::resetPIDs() {
    spdlog::info("ForceController: resetPIDs()");
    this->balanceForceComponent.resetPIDs();
}

void ForceController::applyElevationForces() {
    spdlog::info("ForceController: applyElevationForces()");
    if (!this->elevationForceComponent.isEnabled()) {
        this->elevationForceComponent.enable();
    }
}

void ForceController::zeroElevationForces() {
    spdlog::info("ForceController: zeroElevationForces()");
    if (this->elevationForceComponent.isEnabled()) {
        this->elevationForceComponent.disable();
    }
}

void ForceController::applyOffsetForces(float* x, float* y, float* z) {
    spdlog::info("ForceController: applyOffsetForces()");
    if (!this->offsetForceComponent.isEnabled()) {
        this->offsetForceComponent.enable();
    }
    this->offsetForceComponent.applyOffsetForces(x, y, z);
}

void ForceController::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                                      float yMoment, float zMoment) {
    spdlog::info(
            "ForceController: applyOffsetForcesByMirrorForces({:.1f}, {:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    if (!this->offsetForceComponent.isEnabled()) {
        this->offsetForceComponent.enable();
    }
    this->offsetForceComponent.applyOffsetForcesByMirrorForces(xForce, yForce, zForce, xMoment, yMoment,
                                                               zMoment);
}

void ForceController::zeroOffsetForces() {
    spdlog::info("ForceController: zeroOffsetForces()");
    if (this->offsetForceComponent.isEnabled()) {
        this->offsetForceComponent.disable();
    }
}

void ForceController::applyStaticForces() {
    spdlog::info("ForceController: applyStaticForces()");
    if (!this->staticForceComponent.isEnabled()) {
        this->staticForceComponent.enable();
    }
    this->staticForceComponent.applyStaticForces(&this->forceActuatorSettings->StaticXTable,
                                                 &this->forceActuatorSettings->StaticYTable,
                                                 &this->forceActuatorSettings->StaticZTable);
}

void ForceController::zeroStaticForces() {
    spdlog::info("ForceController: zeroStaticForces()");
    if (this->staticForceComponent.isEnabled()) {
        this->staticForceComponent.disable();
    }
}

void ForceController::applyThermalForces() {
    spdlog::info("ForceController: applyThermalForces()");
    if (!this->thermalForceComponent.isEnabled()) {
        this->thermalForceComponent.enable();
    }
}

void ForceController::updateThermalForces(float temperature) {
    spdlog::trace("ForceController: updateThermalForces()");
    this->thermalForceComponent.applyThermalForcesByMirrorTemperature(temperature);
}

void ForceController::zeroThermalForces() {
    spdlog::info("ForceController: zeroThermalForces()");
    if (this->thermalForceComponent.isEnabled()) {
        this->thermalForceComponent.disable();
    }
}

void ForceController::applyVelocityForces() {
    spdlog::info("ForceController: applyVelocityForces()");
    if (!this->velocityForceComponent.isEnabled()) {
        this->velocityForceComponent.enable();
    }
}

void ForceController::zeroVelocityForces() {
    spdlog::info("ForceController: zeroVelocityForces()");
    if (this->velocityForceComponent.isEnabled()) {
        this->velocityForceComponent.disable();
    }
}

void ForceController::sumAllForces() {
    spdlog::trace("ForceController: sumAllForces()");
    this->finalForceComponent.applyForcesByComponents();
    this->finalForceComponent.update();
}

void ForceController::convertForcesToSetpoints() {
    spdlog::trace("ForceController: convertForcesToSetpoints()");
    bool rejectionRequired = false;
    for (int pIndex = 0; pIndex < FA_COUNT; pIndex++) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[pIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[pIndex];
        int sIndex = this->forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[pIndex];

        this->forceSetpointWarning->safetyLimitWarning[pIndex] = false;

        if (sIndex != -1) {
            float secondaryLowFault =
                    this->forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].LowFault;
            float secondaryHighFault =
                    this->forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].HighFault;
            switch (this->forceActuatorInfo->actuatorOrientation[pIndex]) {
                case ForceActuatorOrientations::PositiveY:
                    this->rejectedCylinderForces->secondaryCylinderForces[sIndex] =
                            toInt24(this->appliedForces->yForces[yIndex] * sqrt2);
                    break;
                case ForceActuatorOrientations::PositiveX:
                    this->rejectedCylinderForces->secondaryCylinderForces[sIndex] =
                            toInt24(this->appliedForces->xForces[xIndex] * sqrt2);
                    break;
                case ForceActuatorOrientations::NegativeX:
                    this->rejectedCylinderForces->secondaryCylinderForces[sIndex] =
                            toInt24(-this->appliedForces->xForces[xIndex] * sqrt2);
                    break;
                case ForceActuatorOrientations::NegativeY:
                    this->rejectedCylinderForces->secondaryCylinderForces[sIndex] =
                            toInt24(-this->appliedForces->yForces[yIndex] * sqrt2);
                    break;
            }
            this->forceSetpointWarning->safetyLimitWarning[pIndex] =
                    this->forceSetpointWarning->safetyLimitWarning[pIndex] ||
                    !Range::InRangeAndCoerce((int)secondaryLowFault, (int)secondaryHighFault,
                                             this->rejectedCylinderForces->secondaryCylinderForces[sIndex],
                                             this->appliedCylinderForces->secondaryCylinderForces + sIndex);
        }

        float primaryLowFault = this->forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].LowFault;
        float primaryHighFault = this->forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].HighFault;
        switch (this->forceActuatorInfo->actuatorOrientation[pIndex]) {
            case ForceActuatorOrientations::PositiveY:
                this->rejectedCylinderForces->primaryCylinderForces[pIndex] =
                        toInt24(this->appliedForces->zForces[pIndex] - this->appliedForces->yForces[yIndex]);
                break;
            case ForceActuatorOrientations::NA:
                this->rejectedCylinderForces->primaryCylinderForces[pIndex] =
                        toInt24(this->appliedForces->zForces[pIndex]);
                break;
            case ForceActuatorOrientations::PositiveX:
                this->rejectedCylinderForces->primaryCylinderForces[pIndex] =
                        toInt24(this->appliedForces->zForces[pIndex] - this->appliedForces->xForces[xIndex]);
                break;
            case ForceActuatorOrientations::NegativeX:
                this->rejectedCylinderForces->primaryCylinderForces[pIndex] =
                        toInt24(this->appliedForces->zForces[pIndex] - -this->appliedForces->xForces[xIndex]);
                break;
            case ForceActuatorOrientations::NegativeY:
                this->rejectedCylinderForces->primaryCylinderForces[pIndex] =
                        toInt24(this->appliedForces->zForces[pIndex] - -this->appliedForces->yForces[yIndex]);
                break;
        }
        this->forceSetpointWarning->safetyLimitWarning[pIndex] =
                this->forceSetpointWarning->safetyLimitWarning[pIndex] ||
                !Range::InRangeAndCoerce((int)primaryLowFault, (int)primaryHighFault,
                                         this->rejectedCylinderForces->primaryCylinderForces[pIndex],
                                         this->appliedCylinderForces->primaryCylinderForces + pIndex);

        rejectionRequired = rejectionRequired || this->forceSetpointWarning->safetyLimitWarning[pIndex];
    }
    this->appliedCylinderForces->timestamp = this->publisher->getTimestamp();
    this->rejectedCylinderForces->timestamp = this->appliedCylinderForces->timestamp;
    this->safetyController->forceControllerNotifySafetyLimit(rejectionRequired);
    if (rejectionRequired) {
        this->publisher->logRejectedCylinderForces();
    }
    this->publisher->logAppliedCylinderForces();
}

bool ForceController::checkMirrorMoments() {
    spdlog::trace("ForceController: checkMirrorMoments()");
    float xMoment = this->appliedForces->mx;
    float yMoment = this->appliedForces->my;
    float zMoment = this->appliedForces->mz;
    this->forceSetpointWarning->xMomentWarning =
            !Range::InRange(this->forceActuatorSettings->MirrorXMoment *
                                    this->forceActuatorSettings->SetpointXMomentHighLimitPercentage,
                            this->forceActuatorSettings->MirrorXMoment *
                                    this->forceActuatorSettings->SetpointXMomentLowLimitPercentage,
                            xMoment);
    this->forceSetpointWarning->yMomentWarning =
            !Range::InRange(this->forceActuatorSettings->MirrorYMoment *
                                    this->forceActuatorSettings->SetpointYMomentHighLimitPercentage,
                            this->forceActuatorSettings->MirrorYMoment *
                                    this->forceActuatorSettings->SetpointYMomentLowLimitPercentage,
                            yMoment);
    this->forceSetpointWarning->zMomentWarning =
            !Range::InRange(this->forceActuatorSettings->MirrorZMoment *
                                    this->forceActuatorSettings->SetpointZMomentHighLimitPercentage,
                            this->forceActuatorSettings->MirrorZMoment *
                                    this->forceActuatorSettings->SetpointZMomentLowLimitPercentage,
                            zMoment);
    this->safetyController->forceControllerNotifyXMomentLimit(this->forceSetpointWarning->xMomentWarning);
    this->safetyController->forceControllerNotifyYMomentLimit(this->forceSetpointWarning->yMomentWarning);
    this->safetyController->forceControllerNotifyZMomentLimit(this->forceSetpointWarning->zMomentWarning);
    return this->forceSetpointWarning->xMomentWarning || this->forceSetpointWarning->yMomentWarning ||
           this->forceSetpointWarning->zMomentWarning;
}

bool ForceController::checkNearNeighbors() {
    spdlog::trace("ForceController: checkNearNeighbors()");
    float nominalZ = this->mirrorWeight / (float)FA_COUNT;
    bool warningChanged = false;
    this->forceSetpointWarning->anyNearNeighborWarning = false;
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
        float nearZ = 0;
        int nearNeighbors = this->neighbors[zIndex].NearZIDs.size();
        for (int j = 0; j < nearNeighbors; ++j) {
            int neighborZIndex = this->neighbors[zIndex].NearZIDs[j];

            nearZ += this->appliedForces->zForces[neighborZIndex];
        }
        nearZ /= nearNeighbors;
        float deltaZ = 0;

        deltaZ = std::abs(this->appliedForces->zForces[zIndex] - nearZ);

        bool previousWarning = this->forceSetpointWarning->nearNeighborWarning[zIndex];
        this->forceSetpointWarning->nearNeighborWarning[zIndex] =
                deltaZ > (nominalZ * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage);
        this->forceSetpointWarning->anyNearNeighborWarning =
                this->forceSetpointWarning->anyNearNeighborWarning ||
                this->forceSetpointWarning->nearNeighborWarning[zIndex];
        warningChanged = warningChanged ||
                         (this->forceSetpointWarning->nearNeighborWarning[zIndex] != previousWarning);
    }
    this->safetyController->forceControllerNotifyNearNeighborCheck(
            this->forceSetpointWarning->anyNearNeighborWarning);
    return warningChanged;
}

bool ForceController::checkMirrorWeight() {
    spdlog::trace("ForceController: checkMirrorWeight()");
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            x += abs(this->appliedForces->xForces[i]);
        }
        if (i < FA_Y_COUNT) {
            y += abs(this->appliedForces->yForces[i]);
        }
        z += abs(this->appliedForces->zForces[i]);
    }
    float globalForce = x + y + z;
    bool previousWarning = this->forceSetpointWarning->magnitudeWarning;
    this->forceSetpointWarning->magnitudeWarning =
            globalForce >
            (this->mirrorWeight * this->forceActuatorSettings->SetpointMirrorWeightLimitPercentage);
    this->safetyController->forceControllerNotifyMagnitudeLimit(this->forceSetpointWarning->magnitudeWarning);
    return this->forceSetpointWarning->magnitudeWarning != previousWarning;
}

bool ForceController::checkFarNeighbors() {
    spdlog::trace("ForceController: checkFarNeighbors()");
    float globalX = this->appliedForces->fx;
    float globalY = this->appliedForces->fy;
    float globalZ = this->appliedForces->fz;
    float globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
    float globalAverageForce = globalForce / FA_COUNT;
    float tolerance = globalAverageForce * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage;
    if (tolerance < 1) {
        tolerance = 1;
    }
    bool warningChanged = false;
    this->forceSetpointWarning->anyFarNeighborWarning = false;
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        double x = 0;
        double y = 0;
        double z = 0;
        if (xIndex != -1) {
            x = this->appliedForces->xForces[xIndex];
        }

        if (yIndex != -1) {
            y = this->appliedForces->yForces[yIndex];
        }

        z = this->appliedForces->zForces[zIndex];
        int farNeighbors = this->neighbors[zIndex].FarIDs.size();
        for (int j = 0; j < farNeighbors; ++j) {
            int neighborZIndex = this->neighbors[zIndex].FarIDs[j];
            int neighborXIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[neighborZIndex];
            int neighborYIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[neighborZIndex];

            if (neighborXIndex != -1) {
                x += this->appliedForces->xForces[neighborXIndex];
            }

            if (neighborYIndex != -1) {
                y += this->appliedForces->yForces[neighborYIndex];
            }

            z += this->appliedForces->zForces[neighborZIndex];
        }
        float magnitude = sqrt(x * x + y * y + z * z);
        float magnitudeAverage = magnitude / (farNeighbors + 1.0);
        bool previousWarning = this->forceSetpointWarning->farNeighborWarning[zIndex];
        this->forceSetpointWarning->farNeighborWarning[zIndex] =
                !Range::InRange(-tolerance, tolerance, magnitudeAverage - globalAverageForce);
        this->forceSetpointWarning->anyFarNeighborWarning =
                this->forceSetpointWarning->anyFarNeighborWarning ||
                this->forceSetpointWarning->farNeighborWarning[zIndex];
        warningChanged =
                warningChanged || (this->forceSetpointWarning->farNeighborWarning[zIndex] != previousWarning);
    }
    this->safetyController->forceControllerNotifyFarNeighborCheck(
            this->forceSetpointWarning->anyFarNeighborWarning);
    return warningChanged;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
