/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Telescope & Site Software Systems.
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
#include <cmath>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

#include <SAL_MTM1M3C.h>

#include <ForceActuatorData.h>
#include <ForceActuatorInfo.h>
#include <ForceController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <RaisingLoweringInfo.h>
#include <SafetyController.h>
#include <SettingReader.h>
#include <PIDSettings.h>
#include <Range.h>
#include <TMA.h>

using namespace std;

using namespace LSST::M1M3::SS;

ForceActuatorIndicesNeighbors::ForceActuatorIndicesNeighbors() {
    nearCount = 0;
    memset(NearZIndices, 0, sizeof(NearZIndices));
    memset(FarIndices, 0, sizeof(FarIndices));
}

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings)
        : _accelerationForceComponent(forceActuatorApplicationSettings),
          _activeOpticForceComponent(forceActuatorApplicationSettings),
          _azimuthForceComponent(forceActuatorApplicationSettings),
          _balanceForceComponent(forceActuatorApplicationSettings),
          _elevationForceComponent(forceActuatorApplicationSettings),
          _offsetForceComponent(forceActuatorApplicationSettings),
          _staticForceComponent(forceActuatorApplicationSettings),
          _thermalForceComponent(forceActuatorApplicationSettings),
          _velocityForceComponent(forceActuatorApplicationSettings),
          _finalForceComponent(forceActuatorApplicationSettings) {
    SPDLOG_DEBUG("ForceController: ForceController()");
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _safetyController = Model::get().getSafetyController();
    _pidSettings = &PIDSettings::instance();

    _appliedCylinderForces = M1M3SSPublisher::instance().getAppliedCylinderForces();
    _appliedForces = M1M3SSPublisher::instance().getAppliedForces();
    _forceActuatorState = M1M3SSPublisher::instance().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _preclippedCylinderForces = M1M3SSPublisher::instance().getEventPreclippedCylinderForces();

    _inclinometerData = M1M3SSPublisher::instance().getInclinometerData();

    _pidData = M1M3SSPublisher::instance().getPIDData();
    _pidInfo = M1M3SSPublisher::instance().getEventPIDInfo();
    _hardpointActuatorData = M1M3SSPublisher::instance().getHardpointActuatorData();
    _accelerometerData = M1M3SSPublisher::instance().getAccelerometerData();
    _gyroData = M1M3SSPublisher::instance().getGyroData();

    reset();

    double timestamp = M1M3SSPublisher::instance().getTimestamp();
    _forceActuatorState->timestamp = timestamp;
    _forceSetpointWarning->timestamp = timestamp;

    M1M3SSPublisher::instance().logForceActuatorState();
    M1M3SSPublisher::instance().logForceSetpointWarning();

    _mirrorWeight = 0.0;
    DistributedForces df = ForceActuatorSettings::instance().calculateForceFromElevationAngle(0.0);
    for (int i = 0; i < FA_COUNT; i++) {
        _mirrorWeight += df.ZForces[i];
        _zero[i] = 0;
        ForceActuatorIndicesNeighbors* currentNeighbors = _neighbors + i;
        currentNeighbors->nearCount = 0;
        for (size_t j = 0; j < FA_MAX_NEAR_COUNT; ++j) {
            if (ForceActuatorSettings::instance().Neighbors[i].NearZIDs[j] == 0) {
                break;
            }
            int index = _forceActuatorApplicationSettings->ActuatorIdToZIndex(
                    ForceActuatorSettings::instance().Neighbors[i].NearZIDs[j]);
            if (index < 0) {
                SPDLOG_CRITICAL("Invalid near neighbor ID: {} FA index {} ID {}",
                                ForceActuatorSettings::instance().Neighbors[i].NearZIDs[j], i,
                                _forceActuatorApplicationSettings->ZIndexToActuatorId(i));
                exit(EXIT_FAILURE);
            }
            currentNeighbors->nearCount++;
            currentNeighbors->NearZIndices[j] = index;
        }
        if (currentNeighbors->nearCount < 3) {
            SPDLOG_CRITICAL("Invalid number of near neighbors for FA index {}: expected 3+, found {}", i,
                            currentNeighbors->nearCount);
            exit(EXIT_FAILURE);
        }
        for (size_t j = 0; j < FA_FAR_COUNT; ++j) {
            int index = _forceActuatorApplicationSettings->ActuatorIdToZIndex(
                    ForceActuatorSettings::instance().Neighbors[i].FarIDs[j]);
            if (index < 0) {
                SPDLOG_CRITICAL("Invalid far neighbor ID: {} FA index {} Neighbor {} ID {}",
                                ForceActuatorSettings::instance().Neighbors[i].FarIDs[j], i, j,
                                _forceActuatorApplicationSettings->ZIndexToActuatorId(
                                        ForceActuatorSettings::instance().Neighbors[i].FarIDs[j]));
                exit(EXIT_FAILURE);
            }
            currentNeighbors->FarIndices[j] = index;
        }
    }

    SPDLOG_INFO("ForceController mirror weight/all Z forces {}N", _mirrorWeight);

    for (int i = 0; i < FA_X_COUNT; i++) {
        limitTriggerX[i] = ForceLimitTrigger('X', _forceActuatorApplicationSettings->XIndexToActuatorId(i));
    }

    for (int i = 0; i < FA_Y_COUNT; i++) {
        limitTriggerY[i] = ForceLimitTrigger('Y', _forceActuatorApplicationSettings->YIndexToActuatorId(i));
    }

    for (int i = 0; i < FA_Z_COUNT; i++) {
        limitTriggerZ[i] = ForceLimitTrigger('Z', _forceActuatorApplicationSettings->ZIndexToActuatorId(i));
    }
}

void ForceController::reset() {
    SPDLOG_INFO("ForceController: reset()");
    _accelerationForceComponent.reset();
    _activeOpticForceComponent.reset();
    _azimuthForceComponent.reset();
    _balanceForceComponent.reset();
    _elevationForceComponent.reset();
    _offsetForceComponent.reset();
    _staticForceComponent.reset();
    _thermalForceComponent.reset();
    _velocityForceComponent.reset();
    _finalForceComponent.reset();
}

bool ForceController::faRaiseFollowingErrorInTolerance() {
    SPDLOG_TRACE("ForceController: followingErrorInTolerance()");
    float limit = ForceActuatorSettings::instance().raiseLowerFollowingErrorLimit;
    auto raiseInfo = &RaisingLoweringInfo::instance();
    bool ret = true;

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            float fe = ForceActuatorData::instance().xForce[i] - _appliedForces->xForces[i];
            bool xInRange = Range::InRangeTrigger(-limit, limit, fe, limitTriggerX[i], limit, fe);
            raiseInfo->setFAXWait(i, !xInRange);
            ret = ret & xInRange;
        }

        if (i < FA_Y_COUNT) {
            float fe = ForceActuatorData::instance().yForce[i] - _appliedForces->yForces[i];
            bool yInRange = Range::InRangeTrigger(-limit, limit, fe, limitTriggerY[i], limit, fe);
            raiseInfo->setFAYWait(i, !yInRange);
            ret = ret & yInRange;
        }

        float fe = ForceActuatorData::instance().zForce[i] - _appliedForces->zForces[i];
        bool zInRange = Range::InRangeTrigger(-limit, limit, fe, limitTriggerZ[i], limit, fe);
        raiseInfo->setFAZWait(i, !zInRange);
        ret = ret & zInRange;
    }
    return ret;
}

void ForceController::updateAppliedForces() {
    SPDLOG_TRACE("ForceController: updateAppliedForces()");

    if (_accelerationForceComponent.isActive()) {
        if (_accelerationForceComponent.isEnabled()) {
            double x, y, z;
            TMA::instance().getMirrorAngularAccelerations(x, y, z);
            _accelerationForceComponent.applyAccelerationForcesByAngularAccelerations(x, y, z);
        }
        _accelerationForceComponent.update();
    }
    if (_activeOpticForceComponent.isActive()) {
        _activeOpticForceComponent.update();
    }
    if (_azimuthForceComponent.isActive()) {
        _azimuthForceComponent.update();
    }
    if (_balanceForceComponent.isActive()) {
        if (_balanceForceComponent.isEnabled()) {
            _balanceForceComponent.applyBalanceForcesByMirrorForces(
                    _hardpointActuatorData->fx, _hardpointActuatorData->fy, _hardpointActuatorData->fz,
                    _hardpointActuatorData->mx, _hardpointActuatorData->my, _hardpointActuatorData->mz);
        }
        _balanceForceComponent.update();
    }
    if (_elevationForceComponent.isActive()) {
        if (_elevationForceComponent.isEnabled()) {
            double elevationAngle = TMA::instance().getElevation();

            // Convert elevation angle to zenith angle (used by matrix)
            _elevationForceComponent.applyElevationForcesByElevationAngle(90.0 - elevationAngle);
        }
        _elevationForceComponent.update();
    }
    if (_offsetForceComponent.isActive()) {
        _offsetForceComponent.update();
    }
    if (_staticForceComponent.isActive()) {
        _staticForceComponent.update();
    }
    if (_thermalForceComponent.isActive()) {
        _thermalForceComponent.update();
    }
    if (_velocityForceComponent.isActive()) {
        if (_velocityForceComponent.isEnabled()) {
            double x, y, z;
            TMA::instance().getMirrorAngularVelocities(x, y, z);
            _velocityForceComponent.applyVelocityForcesByAngularVelocity(x, y, z);
        }
        _velocityForceComponent.update();
    }
}

void ForceController::processAppliedForces() {
    SPDLOG_TRACE("ForceController: processAppliedForces()");
    _forceSetpointWarning->timestamp = _appliedForces->timestamp;
    _sumAllForces();
    _convertForcesToSetpoints();
    _checkMirrorMoments();
    _checkNearNeighbors();
    _checkMirrorWeight();
    _checkFarNeighbors();

    TMA::instance().checkTimestamps(_azimuthForceComponent.isEnabled(), _elevationForceComponent.isEnabled());

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
}

bool ForceController::applyAccelerationForces() {
    SPDLOG_INFO("ForceController: applyAccelerationForces()");
    if (!_accelerationForceComponent.isEnabled()) {
        _accelerationForceComponent.enable();
        return false;
    }
    return true;
}

void ForceController::zeroAccelerationForces() {
    SPDLOG_INFO("ForceController: zeroAccelerationForces()");
    if (_accelerationForceComponent.isEnabled()) {
        _accelerationForceComponent.disable();
    }
}

void ForceController::applyActiveOpticForces(float* z) {
    SPDLOG_INFO("ForceController: applyActiveOpticForces()");
    if (!_activeOpticForceComponent.isEnabled()) {
        _activeOpticForceComponent.enable();
    }
    _activeOpticForceComponent.applyActiveOpticForces(z);
}

void ForceController::zeroActiveOpticForces() {
    SPDLOG_INFO("ForceController: zeroActiveOpticForces()");
    if (_activeOpticForceComponent.isEnabled()) {
        _activeOpticForceComponent.disable();
    }
}

void ForceController::applyAzimuthForces() {
    SPDLOG_INFO("ForceController: applyAzimuthForces()");
    if (!_azimuthForceComponent.isEnabled()) {
        _azimuthForceComponent.enable();
    }
}

void ForceController::updateTMAAzimuthForces(MTMount_azimuthC* tmaAzimuthData) {
    SPDLOG_TRACE("ForceController: updateTMAAzimuthForces() {:.4f}", tmaAzimuthData->actualPosition);
    if (_azimuthForceComponent.isEnabled()) {
        _azimuthForceComponent.applyAzimuthForcesByAzimuthAngle(tmaAzimuthData->actualPosition);
    }
}

void ForceController::zeroAzimuthForces() {
    SPDLOG_INFO("ForceController: zeroAzimuthForces()");
    if (_azimuthForceComponent.isEnabled()) {
        _azimuthForceComponent.disable();
    }
}

void ForceController::applyBalanceForces() {
    SPDLOG_INFO("ForceController: applyBalanceForces()");
    if (!_balanceForceComponent.isEnabled()) {
        _balanceForceComponent.enable();
    }
}

void ForceController::zeroBalanceForces() {
    SPDLOG_INFO("ForceController: zeroBalanceForces()");
    if (_balanceForceComponent.isEnabled()) {
        _balanceForceComponent.disable();
    }
}

void ForceController::updatePID(int id, PIDParameters parameters) {
    SPDLOG_INFO("ForceController: updatePID()");
    _balanceForceComponent.updatePID(id, parameters);
}

void ForceController::resetPID(int id) {
    SPDLOG_INFO("ForceController: resetPID()");
    _balanceForceComponent.resetPID(id);
}

void ForceController::resetPIDs() {
    SPDLOG_INFO("ForceController: resetPIDs()");
    _balanceForceComponent.resetPIDs();
}

void ForceController::applyElevationForces() {
    SPDLOG_INFO("ForceController: applyElevationForces()");
    if (!_elevationForceComponent.isEnabled()) {
        _elevationForceComponent.enable();
    }
}

void ForceController::zeroElevationForces() {
    SPDLOG_INFO("ForceController: zeroElevationForces()");
    if (_elevationForceComponent.isEnabled()) {
        _elevationForceComponent.disable();
    }
}

void ForceController::applyOffsetForces(float* x, float* y, float* z) {
    SPDLOG_INFO("ForceController: applyOffsetForces()");
    if (!_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.enable();
    }
    _offsetForceComponent.applyOffsetForces(x, y, z);
}

void ForceController::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                                      float yMoment, float zMoment) {
    SPDLOG_INFO(
            "ForceController: applyOffsetForcesByMirrorForces({:.1f}, {:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    if (!_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.enable();
    }
    _offsetForceComponent.applyOffsetForcesByMirrorForces(xForce, yForce, zForce, xMoment, yMoment, zMoment);
}

void ForceController::zeroOffsetForces() {
    SPDLOG_INFO("ForceController: zeroOffsetForces()");
    if (_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.disable();
    }
    _offsetForceComponent.zeroOffsetForces();
}

void ForceController::applyActuatorOffset(char axis, int index, float offset) {
    SPDLOG_INFO("ForceController: applyActuatorOffset({}, {}, {:.1f})", axis, index, offset);
    if (!_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.enable();
    }
    _offsetForceComponent.applyActuatorOffset(axis, index, offset);
}

void ForceController::applyStaticForces() {
    SPDLOG_INFO("ForceController: applyStaticForces()");
    if (!_staticForceComponent.isEnabled()) {
        _staticForceComponent.enable();
    }
    _staticForceComponent.applyStaticForces(&(ForceActuatorSettings::instance().StaticXTable),
                                            &(ForceActuatorSettings::instance().StaticYTable),
                                            &(ForceActuatorSettings::instance().StaticZTable));
}

void ForceController::zeroStaticForces() {
    SPDLOG_INFO("ForceController: zeroStaticForces()");
    if (_staticForceComponent.isEnabled()) {
        _staticForceComponent.disable();
    }
}

void ForceController::applyThermalForces() {
    SPDLOG_INFO("ForceController: applyThermalForces()");
    if (!_thermalForceComponent.isEnabled()) {
        _thermalForceComponent.enable();
    }
}

void ForceController::updateThermalForces(float temperature) {
    SPDLOG_TRACE("ForceController: updateThermalForces()");
    _thermalForceComponent.applyThermalForcesByMirrorTemperature(temperature);
}

void ForceController::zeroThermalForces() {
    SPDLOG_INFO("ForceController: zeroThermalForces()");
    if (_thermalForceComponent.isEnabled()) {
        _thermalForceComponent.disable();
    }
}

bool ForceController::applyVelocityForces() {
    SPDLOG_INFO("ForceController: applyVelocityForces()");
    if (!_velocityForceComponent.isEnabled()) {
        _velocityForceComponent.enable();
        return false;
    }
    return true;
}

void ForceController::zeroVelocityForces() {
    SPDLOG_INFO("ForceController: zeroVelocityForces()");
    if (_velocityForceComponent.isEnabled()) {
        _velocityForceComponent.disable();
    }
}

void ForceController::enableDisableForceComponent(int forceComponentEnum, bool enabled) {
    ForceComponent* forceComponent = NULL;
    switch (forceComponentEnum) {
        case MTM1M3::enableDisableForceComponent_AccelerationForce:
            forceComponent = &_accelerationForceComponent;
            break;
        case MTM1M3::enableDisableForceComponent_ActiveOpticForce:
            forceComponent = &_activeOpticForceComponent;
            break;
        case MTM1M3::enableDisableForceComponent_AzimuthForce:
            forceComponent = &_azimuthForceComponent;
            break;
        case MTM1M3::enableDisableForceComponent_OffsetForce:
            forceComponent = &_offsetForceComponent;
            break;
        case MTM1M3::enableDisableForceComponent_StaticForce:
            forceComponent = &_staticForceComponent;
            break;
        case MTM1M3::enableDisableForceComponent_ThermalForce:
            forceComponent = &_thermalForceComponent;
            break;
        case MTM1M3::enableDisableForceComponent_VelocityForce:
            forceComponent = &_velocityForceComponent;
            break;
        default:
            throw std::runtime_error(fmt::format("Unknown forceComponent: {}", forceComponentEnum));
    }

    if (enabled) {
        if (forceComponent->isEnabled() == false && forceComponent->isInitialising() == false) {
            SPDLOG_INFO("Enabling {}ForceComponent, as requested", forceComponent->getName());
            forceComponent->enable();
        }
    } else {
        if (forceComponent->isEnabled() == true && forceComponent->isDisabling() == false) {
            SPDLOG_INFO("Disabling {}ForceComponent, as requested", forceComponent->getName());
            forceComponent->disable();
        }
    }

    forceComponent->postEnableDisableActions();
}

void ForceController::_sumAllForces() {
    SPDLOG_TRACE("ForceController: sumAllForces()");
    _finalForceComponent.applyForcesByComponents();
    _finalForceComponent.update();
}

void ForceController::_convertForcesToSetpoints() {
    SPDLOG_TRACE("ForceController: convertForcesToSetpoints()");
    bool clippingRequired = false;
    auto& forceActuatorInfo = ForceActuatorInfo::instance();
    for (int pIndex = 0; pIndex < FA_COUNT; pIndex++) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[pIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[pIndex];
        int sIndex = _forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[pIndex];

        _forceSetpointWarning->safetyLimitWarning[pIndex] = false;

        if (sIndex != -1) {
            float secondaryLowFault =
                    ForceActuatorSettings::instance().CylinderLimitSecondaryTable[sIndex].LowFault;
            float secondaryHighFault =
                    ForceActuatorSettings::instance().CylinderLimitSecondaryTable[sIndex].HighFault;
            switch (forceActuatorInfo.actuatorOrientation[pIndex]) {
                case ForceActuatorOrientations::PositiveY:
                    _preclippedCylinderForces->secondaryCylinderForces[sIndex] =
                            _toInt24(_appliedForces->yForces[yIndex] * _sqrt2);
                    break;
                case ForceActuatorOrientations::PositiveX:
                    _preclippedCylinderForces->secondaryCylinderForces[sIndex] =
                            _toInt24(_appliedForces->xForces[xIndex] * _sqrt2);
                    break;
                case ForceActuatorOrientations::NegativeX:
                    _preclippedCylinderForces->secondaryCylinderForces[sIndex] =
                            _toInt24(-_appliedForces->xForces[xIndex] * _sqrt2);
                    break;
                case ForceActuatorOrientations::NegativeY:
                    _preclippedCylinderForces->secondaryCylinderForces[sIndex] =
                            _toInt24(-_appliedForces->yForces[yIndex] * _sqrt2);
                    break;
            }
            bool notInRangeS =
                    !Range::InRangeAndCoerce((int)secondaryLowFault, (int)secondaryHighFault,
                                             _preclippedCylinderForces->secondaryCylinderForces[sIndex],
                                             _appliedCylinderForces->secondaryCylinderForces + sIndex);
            _forceSetpointWarning->safetyLimitWarning[pIndex] =
                    notInRangeS || _forceSetpointWarning->safetyLimitWarning[pIndex];
        }

        float primaryLowFault = ForceActuatorSettings::instance().CylinderLimitPrimaryTable[pIndex].LowFault;
        float primaryHighFault =
                ForceActuatorSettings::instance().CylinderLimitPrimaryTable[pIndex].HighFault;
        switch (forceActuatorInfo.actuatorOrientation[pIndex]) {
            case ForceActuatorOrientations::PositiveY:
                _preclippedCylinderForces->primaryCylinderForces[pIndex] =
                        _toInt24(_appliedForces->zForces[pIndex] - _appliedForces->yForces[yIndex]);
                break;
            case ForceActuatorOrientations::NA:
                _preclippedCylinderForces->primaryCylinderForces[pIndex] =
                        _toInt24(_appliedForces->zForces[pIndex]);
                break;
            case ForceActuatorOrientations::PositiveX:
                _preclippedCylinderForces->primaryCylinderForces[pIndex] =
                        _toInt24(_appliedForces->zForces[pIndex] - _appliedForces->xForces[xIndex]);
                break;
            case ForceActuatorOrientations::NegativeX:
                _preclippedCylinderForces->primaryCylinderForces[pIndex] =

                        _toInt24(_appliedForces->zForces[pIndex] - -_appliedForces->xForces[xIndex]);
                break;
            case ForceActuatorOrientations::NegativeY:
                _preclippedCylinderForces->primaryCylinderForces[pIndex] =
                        _toInt24(_appliedForces->zForces[pIndex] - -_appliedForces->yForces[yIndex]);
                break;
        }
        bool notInRange = !Range::InRangeAndCoerce((int)primaryLowFault, (int)primaryHighFault,
                                                   _preclippedCylinderForces->primaryCylinderForces[pIndex],
                                                   _appliedCylinderForces->primaryCylinderForces + pIndex);
        _forceSetpointWarning->safetyLimitWarning[pIndex] =
                notInRange || _forceSetpointWarning->safetyLimitWarning[pIndex];

        clippingRequired = _forceSetpointWarning->safetyLimitWarning[pIndex] || clippingRequired;
    }
    _appliedCylinderForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclippedCylinderForces->timestamp = _appliedCylinderForces->timestamp;
    _safetyController->forceControllerNotifySafetyLimit(clippingRequired);
    if (clippingRequired) {
        M1M3SSPublisher::instance().logPreclippedCylinderForces();
    }
    M1M3SSPublisher::instance().logAppliedCylinderForces();
}

bool ForceController::_checkMirrorMoments() {
    SPDLOG_TRACE("ForceController: checkMirrorMoments()");
    float xMoment = _appliedForces->mx;
    float yMoment = _appliedForces->my;
    float zMoment = _appliedForces->mz;

    float xMomentMin = ForceActuatorSettings::instance().mirrorXMoment *
                       ForceActuatorSettings::instance().setpointXMomentHighLimitFactor;
    float xMomentMax = ForceActuatorSettings::instance().mirrorXMoment *
                       ForceActuatorSettings::instance().setpointXMomentLowLimitFactor;

    float yMomentMin = ForceActuatorSettings::instance().mirrorYMoment *
                       ForceActuatorSettings::instance().setpointYMomentHighLimitFactor;
    float yMomentMax = ForceActuatorSettings::instance().mirrorYMoment *
                       ForceActuatorSettings::instance().setpointYMomentLowLimitFactor;

    float zMomentMin = ForceActuatorSettings::instance().mirrorZMoment *
                       ForceActuatorSettings::instance().setpointZMomentHighLimitFactor;
    float zMomentMax = ForceActuatorSettings::instance().mirrorZMoment *
                       ForceActuatorSettings::instance().setpointZMomentLowLimitFactor;

    _forceSetpointWarning->xMomentWarning = !Range::InRange(xMomentMin, xMomentMax, xMoment);
    _forceSetpointWarning->yMomentWarning = !Range::InRange(yMomentMin, yMomentMax, yMoment);
    _forceSetpointWarning->zMomentWarning = !Range::InRange(zMomentMin, zMomentMax, zMoment);
    _safetyController->forceControllerNotifyXMomentLimit(
            _forceSetpointWarning->xMomentWarning,
            fmt::format(
                    "Force controller X Moment Limit - applied {:.02f} N, expected {:.02f} N to {:.02f} N",
                    xMoment, xMomentMin, xMomentMax));
    _safetyController->forceControllerNotifyYMomentLimit(
            _forceSetpointWarning->yMomentWarning,
            fmt::format(
                    "Force controller Y Moment Limit - applied {:.02f} N, expected {:.02f} N to {:.02f} N",
                    yMoment, yMomentMin, yMomentMax));
    _safetyController->forceControllerNotifyZMomentLimit(
            _forceSetpointWarning->zMomentWarning,
            fmt::format(
                    "Force controller Z Moment Limit - applied {:.02f} N, expected {:.02f} N to {:.02f} N",
                    zMoment, zMomentMin, zMomentMax));
    return _forceSetpointWarning->xMomentWarning || _forceSetpointWarning->yMomentWarning ||
           _forceSetpointWarning->zMomentWarning;
}

bool ForceController::_checkNearNeighbors() {
    SPDLOG_TRACE("ForceController: checkNearNeighbors()");
    float nominalZ = _mirrorWeight / (float)FA_COUNT;
    float nominalZWarning = nominalZ * ForceActuatorSettings::instance().setpointNearNeighborLimitFactor;
    bool warningChanged = false;
    _forceSetpointWarning->anyNearNeighborWarning = false;
    string failed;
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
        // ignore check for disabled FA
        if (Model::get().getILC()->isDisabled(
                    SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(
                            zIndex))) {
            continue;
        }

        float nearZ = 0;
        for (int j = 0; j < _neighbors[zIndex].nearCount; ++j) {
            int neighborZIndex = _neighbors[zIndex].NearZIndices[j];

            nearZ += _appliedForces->zForces[neighborZIndex];
        }

        nearZ /= _neighbors[zIndex].nearCount;
        float deltaZ = abs(_appliedForces->zForces[zIndex] - nearZ);

        if (deltaZ > nominalZWarning) {
            _forceSetpointWarning->nearNeighborWarning[zIndex] = true;
            failed += to_string(_forceActuatorApplicationSettings->ZIndexToActuatorId(zIndex)) + ":" +
                      to_string(deltaZ) + " ";
        } else {
            _forceSetpointWarning->nearNeighborWarning[zIndex] = false;
        }

        bool previousWarning = _forceSetpointWarning->nearNeighborWarning[zIndex];
        _forceSetpointWarning->anyNearNeighborWarning = _forceSetpointWarning->nearNeighborWarning[zIndex] ||
                                                        _forceSetpointWarning->anyNearNeighborWarning;
        warningChanged =
                (_forceSetpointWarning->nearNeighborWarning[zIndex] != previousWarning) || warningChanged;
    }
    _safetyController->forceControllerNotifyNearNeighborCheck(_forceSetpointWarning->anyNearNeighborWarning,
                                                              failed, nominalZ, nominalZWarning);
    return warningChanged;
}

bool ForceController::_checkMirrorWeight() {
    SPDLOG_TRACE("ForceController: checkMirrorWeight()");
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            x += abs(_appliedForces->xForces[i]);
        }
        if (i < FA_Y_COUNT) {
            y += abs(_appliedForces->yForces[i]);
        }
        z += abs(_appliedForces->zForces[i]);
    }
    float globalForce = x + y + z;
    bool previousWarning = _forceSetpointWarning->magnitudeWarning;
    _forceSetpointWarning->magnitudeWarning =
            globalForce > (_mirrorWeight * ForceActuatorSettings::instance().setpointMirrorWeightLimitFactor);
    _safetyController->forceControllerNotifyMagnitudeLimit(_forceSetpointWarning->magnitudeWarning,
                                                           globalForce);
    return _forceSetpointWarning->magnitudeWarning != previousWarning;
}

bool ForceController::_checkFarNeighbors() {
    SPDLOG_TRACE("ForceController: checkFarNeighbors()");
    float globalX = _appliedForces->fx;
    float globalY = _appliedForces->fy;
    float globalZ = _appliedForces->fz;
    float globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
    float globalAverageForce = globalForce / FA_COUNT;
    float tolerance = globalAverageForce * ForceActuatorSettings::instance().setpointNearNeighborLimitFactor;
    if (tolerance < 1) {
        tolerance = 1;
    }
    bool warningChanged = false;
    string failed;
    _forceSetpointWarning->anyFarNeighborWarning = false;
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
        // ignore check for disabled FA
        if (Model::get().getILC()->isDisabled(
                    SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(
                            zIndex))) {
            continue;
        }

        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        double x = 0;
        double y = 0;
        double z = 0;
        if (xIndex != -1) {
            x = _appliedForces->xForces[xIndex];
        }

        if (yIndex != -1) {
            y = _appliedForces->yForces[yIndex];
        }

        z = _appliedForces->zForces[zIndex];
        for (int j = 0; j < FA_FAR_COUNT; ++j) {
            int neighborZIndex = _neighbors[zIndex].FarIndices[j];
            int neighborXIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[neighborZIndex];
            int neighborYIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[neighborZIndex];

            if (neighborXIndex != -1) {
                x += _appliedForces->xForces[neighborXIndex];
            }

            if (neighborYIndex != -1) {
                y += _appliedForces->yForces[neighborYIndex];
            }

            z += _appliedForces->zForces[neighborZIndex];
        }
        float magnitude = sqrt(x * x + y * y + z * z);
        float magnitudeAverage = magnitude / (FA_FAR_COUNT + 1.0);
        bool previousWarning = _forceSetpointWarning->farNeighborWarning[zIndex];
        if (!Range::InRange(-tolerance, tolerance, magnitudeAverage - globalAverageForce)) {
            failed += fmt::format(" {}: magA {:.2f} globalA {:.2f} |{:.2f}| < {:.2f}",
                                  _forceActuatorApplicationSettings->ZIndexToActuatorId(zIndex),
                                  magnitudeAverage, globalAverageForce, magnitudeAverage - globalAverageForce,
                                  tolerance);
            _forceSetpointWarning->farNeighborWarning[zIndex] = true;
        } else {
            _forceSetpointWarning->farNeighborWarning[zIndex] = false;
        }
        _forceSetpointWarning->anyFarNeighborWarning = _forceSetpointWarning->farNeighborWarning[zIndex] ||
                                                       _forceSetpointWarning->anyFarNeighborWarning;
        warningChanged =
                (_forceSetpointWarning->farNeighborWarning[zIndex] != previousWarning) || warningChanged;
    }
    _safetyController->forceControllerNotifyFarNeighborCheck(_forceSetpointWarning->anyFarNeighborWarning,
                                                             failed);
    return warningChanged;
}
