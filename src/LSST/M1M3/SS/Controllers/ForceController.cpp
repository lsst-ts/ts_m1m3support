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

#include <ForceController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorSettings.h>
#include <ForceActuatorLimits.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SafetyController.h>
#include <SettingReader.h>
#include <PIDSettings.h>
#include <Range.h>
#include <TMA.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <ForceConverter.h>

#include <iostream>
using namespace std;

using namespace LSST::M1M3::SS;

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                 ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings)
        : _accelerationForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _activeOpticForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _azimuthForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _balanceForceComponent(forceActuatorApplicationSettings, forceActuatorSettings, pidSettings),
          _elevationForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _offsetForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _staticForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _thermalForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _velocityForceComponent(forceActuatorApplicationSettings, forceActuatorSettings),
          _finalForceComponent(forceActuatorApplicationSettings, forceActuatorSettings) {
    SPDLOG_DEBUG("ForceController: ForceController()");
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _safetyController = Model::get().getSafetyController();
    _pidSettings = pidSettings;

    _appliedCylinderForces = M1M3SSPublisher::instance().getAppliedCylinderForces();
    _appliedForces = M1M3SSPublisher::instance().getAppliedForces();
    _forceActuatorState = M1M3SSPublisher::instance().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _preclippedCylinderForces = M1M3SSPublisher::instance().getEventPreclippedCylinderForces();

    _forceActuatorInfo = M1M3SSPublisher::instance().getEventForceActuatorInfo();
    _inclinometerData = M1M3SSPublisher::instance().getInclinometerData();
    _forceActuatorData = M1M3SSPublisher::instance().getForceActuatorData();

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
    DistributedForces df = ForceConverter::calculateForceFromElevationAngle(_forceActuatorSettings, 0.0);
    for (int i = 0; i < FA_COUNT; i++) {
        _mirrorWeight += df.ZForces[i];
        _zero[i] = 0;
        ForceActuatorIndicesNeighbors neighbors;
        for (unsigned int j = 0; j < _forceActuatorSettings->Neighbors[i].NearZIDs.size(); ++j) {
            int index = _forceActuatorApplicationSettings->ActuatorIdToZIndex(
                    _forceActuatorSettings->Neighbors[i].NearZIDs[j]);
            if (index < 0) {
                SPDLOG_CRITICAL("Invalid near neighbor ID: {} FA index {} ID {}",
                                _forceActuatorSettings->Neighbors[i].NearZIDs[j], i,
                                _forceActuatorApplicationSettings->ZIndexToActuatorId(i));
                exit(EXIT_FAILURE);
            }
            neighbors.NearZIndices.push_back(index);
        }
        for (unsigned int j = 0; j < _forceActuatorSettings->Neighbors[i].FarIDs.size(); ++j) {
            int index = _forceActuatorApplicationSettings->ActuatorIdToZIndex(
                    _forceActuatorSettings->Neighbors[i].FarIDs[j]);
            if (index < 0) {
                SPDLOG_CRITICAL("Invalid far neighbor ID: {} FA index {} ID {}",
                                _forceActuatorSettings->Neighbors[i].FarIDs[j], i,
                                _forceActuatorApplicationSettings->ZIndexToActuatorId(i));
                exit(EXIT_FAILURE);
            }
            neighbors.FarIndices.push_back(index);
        }
        _neighbors.push_back(neighbors);
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

void ForceController::incSupportPercentage() {
    SPDLOG_TRACE("ForceController: incSupportPercentage()");
    _forceActuatorState->supportPercentage += _forceActuatorSettings->raiseIncrementPercentage;
    if (supportPercentageFilled()) {
        _forceActuatorState->supportPercentage = 100.0;
    }
    M1M3SSPublisher::instance().logForceActuatorState();
}

void ForceController::decSupportPercentage() {
    SPDLOG_TRACE("ForceController: decSupportPercentage()");
    _forceActuatorState->supportPercentage -= _forceActuatorSettings->lowerDecrementPercentage;
    if (supportPercentageZeroed()) {
        _forceActuatorState->supportPercentage = 0.0;
    }
    M1M3SSPublisher::instance().logForceActuatorState();
}

void ForceController::zeroSupportPercentage() {
    SPDLOG_INFO("ForceController: zeroSupportPercentage()");
    _forceActuatorState->supportPercentage = 0.0;
    M1M3SSPublisher::instance().logForceActuatorState();
}

void ForceController::fillSupportPercentage() {
    SPDLOG_INFO("ForceController: fillSupportPercentage()");
    _forceActuatorState->supportPercentage = 100.0;
    M1M3SSPublisher::instance().logForceActuatorState();
}

bool ForceController::supportPercentageFilled() { return _forceActuatorState->supportPercentage >= 100.0; }

bool ForceController::supportPercentageZeroed() { return _forceActuatorState->supportPercentage <= 0.0; }

bool ForceController::followingErrorInTolerance() {
    SPDLOG_TRACE("ForceController: followingErrorInTolerance()");
    float limit = _forceActuatorSettings->raiseLowerFollowingErrorLimit;
    bool inTolerance = true;

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            float fe = _forceActuatorData->xForce[i] - _appliedForces->xForces[i];
            inTolerance &= Range::InRangeTrigger(-limit, limit, fe, limitTriggerX[i], limit, fe);
        }

        if (i < FA_Y_COUNT) {
            float fe = _forceActuatorData->yForce[i] - _appliedForces->yForces[i];
            inTolerance &= Range::InRangeTrigger(-limit, limit, fe, limitTriggerY[i], limit, fe);
        }

        float fe = _forceActuatorData->zForce[i] - _appliedForces->zForces[i];
        inTolerance &= Range::InRangeTrigger(-limit, limit, fe, limitTriggerZ[i], limit, fe);
    }
    return inTolerance;
}

void ForceController::updateAppliedForces() {
    SPDLOG_TRACE("ForceController: updateAppliedForces()");

    if (_accelerationForceComponent.isActive()) {
        if (_accelerationForceComponent.isEnabled()) {
            _accelerationForceComponent.applyAccelerationForcesByAngularAccelerations(
                    _accelerometerData->angularAccelerationX, _accelerometerData->angularAccelerationY,
                    _accelerometerData->angularAccelerationZ);
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
            _velocityForceComponent.applyVelocityForcesByAngularVelocity(
                    _gyroData->angularVelocityX, _gyroData->angularVelocityY, _gyroData->angularVelocityZ);
        }
        _velocityForceComponent.update();
    }
}

void ForceController::processAppliedForces() {
    SPDLOG_TRACE("ForceController: processAppliedForces()");
    _sumAllForces();
    _convertForcesToSetpoints();
    _checkMirrorMoments();
    _checkNearNeighbors();
    _checkMirrorWeight();
    _checkFarNeighbors();

    TMA::instance().checkTimestamps(_azimuthForceComponent.isEnabled(), _elevationForceComponent.isEnabled());

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
}

void ForceController::applyAccelerationForces() {
    SPDLOG_INFO("ForceController: applyAccelerationForces()");
    if (!_accelerationForceComponent.isEnabled()) {
        _accelerationForceComponent.enable();
    }
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
    _staticForceComponent.applyStaticForces(&_forceActuatorSettings->StaticXTable,
                                            &_forceActuatorSettings->StaticYTable,
                                            &_forceActuatorSettings->StaticZTable);
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

void ForceController::applyVelocityForces() {
    SPDLOG_INFO("ForceController: applyVelocityForces()");
    if (!_velocityForceComponent.isEnabled()) {
        _velocityForceComponent.enable();
    }
}

void ForceController::zeroVelocityForces() {
    SPDLOG_INFO("ForceController: zeroVelocityForces()");
    if (_velocityForceComponent.isEnabled()) {
        _velocityForceComponent.disable();
    }
}

void ForceController::_sumAllForces() {
    SPDLOG_TRACE("ForceController: sumAllForces()");
    _finalForceComponent.applyForcesByComponents();
    _finalForceComponent.update();
}

void ForceController::_convertForcesToSetpoints() {
    SPDLOG_TRACE("ForceController: convertForcesToSetpoints()");
    bool clippingRequired = false;
    for (int pIndex = 0; pIndex < FA_COUNT; pIndex++) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[pIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[pIndex];
        int sIndex = _forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[pIndex];

        _forceSetpointWarning->safetyLimitWarning[pIndex] = false;

        if (sIndex != -1) {
            float secondaryLowFault = _forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].LowFault;
            float secondaryHighFault = _forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].HighFault;
            switch (_forceActuatorInfo->actuatorOrientation[pIndex]) {
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

        float primaryLowFault = _forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].LowFault;
        float primaryHighFault = _forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].HighFault;
        switch (_forceActuatorInfo->actuatorOrientation[pIndex]) {
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

    float xMomentMin =
            _forceActuatorSettings->mirrorXMoment * _forceActuatorSettings->setpointXMomentHighLimitFactor;
    float xMomentMax =
            _forceActuatorSettings->mirrorXMoment * _forceActuatorSettings->setpointXMomentLowLimitFactor;

    float yMomentMin =
            _forceActuatorSettings->mirrorYMoment * _forceActuatorSettings->setpointYMomentHighLimitFactor;
    float yMomentMax =
            _forceActuatorSettings->mirrorYMoment * _forceActuatorSettings->setpointYMomentLowLimitFactor;

    float zMomentMin =
            _forceActuatorSettings->mirrorZMoment * _forceActuatorSettings->setpointZMomentHighLimitFactor;
    float zMomentMax =
            _forceActuatorSettings->mirrorZMoment * _forceActuatorSettings->setpointZMomentLowLimitFactor;

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
    float nominalZWarning = nominalZ * _forceActuatorSettings->setpointNearNeighborLimitFactor;
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
        int nearNeighbors = _neighbors[zIndex].NearZIndices.size();
        for (int j = 0; j < nearNeighbors; ++j) {
            int neighborZIndex = _neighbors[zIndex].NearZIndices[j];

            nearZ += _appliedForces->zForces[neighborZIndex];
        }

        nearZ /= nearNeighbors;
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
            globalForce > (_mirrorWeight * _forceActuatorSettings->setpointMirrorWeightLimitFactor);
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
    float tolerance = globalAverageForce * _forceActuatorSettings->setpointNearNeighborLimitFactor;
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
        int farNeighbors = _neighbors[zIndex].FarIndices.size();
        for (int j = 0; j < farNeighbors; ++j) {
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
        float magnitudeAverage = magnitude / (farNeighbors + 1.0);
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
