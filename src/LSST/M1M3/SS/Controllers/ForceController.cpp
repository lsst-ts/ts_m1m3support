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
                                 SafetyController* safetyController)
        : _aberrationForceComponent(safetyController, forceActuatorApplicationSettings,
                                    forceActuatorSettings),
          _accelerationForceComponent(safetyController, forceActuatorApplicationSettings,
                                      forceActuatorSettings),
          _activeOpticForceComponent(safetyController, forceActuatorApplicationSettings,
                                     forceActuatorSettings),
          _azimuthForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
          _balanceForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings,
                                 pidSettings),
          _elevationForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
          _offsetForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
          _staticForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
          _thermalForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
          _velocityForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
          _finalForceComponent(safetyController, forceActuatorApplicationSettings, forceActuatorSettings) {
    spdlog::debug("ForceController: ForceController()");
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _safetyController = safetyController;
    _pidSettings = pidSettings;

    _appliedCylinderForces = M1M3SSPublisher::get().getEventAppliedCylinderForces();
    _appliedForces = M1M3SSPublisher::get().getEventAppliedForces();
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _preclippedCylinderForces = M1M3SSPublisher::get().getEventPreclippedCylinderForces();

    _forceActuatorInfo = M1M3SSPublisher::get().getEventForceActuatorInfo();
    _inclinometerData = M1M3SSPublisher::get().getInclinometerData();
    _forceActuatorData = M1M3SSPublisher::get().getForceActuatorData();

    _pidData = M1M3SSPublisher::get().getPIDData();
    _pidInfo = M1M3SSPublisher::get().getEventPIDInfo();
    _hardpointActuatorData = M1M3SSPublisher::get().getHardpointActuatorData();
    _accelerometerData = M1M3SSPublisher::get().getAccelerometerData();
    _gyroData = M1M3SSPublisher::get().getGyroData();

    _elevation_Timestamp = 0;
    _elevation_Angle_Actual = NAN;

    reset();

    double timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->timestamp = timestamp;
    _forceSetpointWarning->timestamp = timestamp;

    M1M3SSPublisher::get().logForceActuatorState();
    M1M3SSPublisher::get().logForceSetpointWarning();

    _mirrorWeight = 0.0;
    DistributedForces df = ForceConverter::calculateForceFromElevationAngle(_forceActuatorSettings, 0.0);
    for (int i = 0; i < FA_COUNT; i++) {
        _mirrorWeight += df.ZForces[i];
        _zero[i] = 0;
        ForceActuatorNeighbors neighbors;
        for (unsigned int j = 0; j < _forceActuatorSettings->Neighbors[i].NearZIDs.size(); ++j) {
            int32_t id = _forceActuatorSettings->Neighbors[i].NearZIDs[j];
            for (unsigned int k = 0; k < FA_COUNT; ++k) {
                if (_forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
                    neighbors.NearZIDs.push_back(k);
                    break;
                }
            }
        }
        for (unsigned int j = 0; j < _forceActuatorSettings->Neighbors[i].FarIDs.size(); ++j) {
            int32_t id = _forceActuatorSettings->Neighbors[i].FarIDs[j];
            for (unsigned int k = 0; k < FA_COUNT; ++k) {
                if (_forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
                    neighbors.FarIDs.push_back(k);
                    break;
                }
            }
        }
        _neighbors.push_back(neighbors);
    }
}

void ForceController::reset() {
    spdlog::info("ForceController: reset()");
    _aberrationForceComponent.reset();
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

void ForceController::updateTMAElevationData(MTMount_ElevationC* tmaElevationData) {
    spdlog::trace("ForceController: updateTMAElevationData()");
    _elevation_Angle_Actual = tmaElevationData->Elevation_Angle_Actual;
    _elevation_Timestamp = tmaElevationData->timestamp;
}

void ForceController::incSupportPercentage() {
    spdlog::trace("ForceController: incSupportPercentage()");
    _forceActuatorState->supportPercentage += _forceActuatorSettings->RaiseIncrementPercentage;
    if (supportPercentageFilled()) {
        _forceActuatorState->supportPercentage = 1.0;
    }
}

void ForceController::decSupportPercentage() {
    spdlog::trace("ForceController: decSupportPercentage()");
    _forceActuatorState->supportPercentage -= _forceActuatorSettings->LowerDecrementPercentage;
    if (supportPercentageZeroed()) {
        _forceActuatorState->supportPercentage = 0.0;
    }
}

void ForceController::zeroSupportPercentage() {
    spdlog::info("ForceController: zeroSupportPercentage()");
    _forceActuatorState->supportPercentage = 0.0;
}

void ForceController::fillSupportPercentage() {
    spdlog::info("ForceController: fillSupportPercentage()");
    _forceActuatorState->supportPercentage = 1.0;
}

bool ForceController::supportPercentageFilled() { return _forceActuatorState->supportPercentage >= 1.0; }

bool ForceController::supportPercentageZeroed() { return _forceActuatorState->supportPercentage <= 0.0; }

bool ForceController::followingErrorInTolerance() {
    spdlog::trace("ForceController: followingErrorInTolerance()");
    float limit = _forceActuatorSettings->RaiseLowerFollowingErrorLimit;
    for (int i = 0; i < FA_COUNT; ++i) {
        if ((i < FA_X_COUNT &&
             !Range::InRange(-limit, limit, _forceActuatorData->xForce[i] - _appliedForces->xForces[i])) ||
            (i < FA_Y_COUNT &&
             !Range::InRange(-limit, limit, _forceActuatorData->yForce[i] - _appliedForces->yForces[i])) ||
            (!Range::InRange(-limit, limit, _forceActuatorData->zForce[i] - _appliedForces->zForces[i]))) {
            return false;
        }
    }
    return true;
}

void ForceController::updateAppliedForces() {
    spdlog::trace("ForceController: updateAppliedForces()");

    if (_aberrationForceComponent.isEnabled() || _aberrationForceComponent.isDisabling()) {
        _aberrationForceComponent.update();
    }
    if (_accelerationForceComponent.isEnabled() || _accelerationForceComponent.isDisabling()) {
        if (_accelerationForceComponent.isEnabled()) {
            _accelerationForceComponent.applyAccelerationForcesByAngularAccelerations(
                    _accelerometerData->angularAccelerationX, _accelerometerData->angularAccelerationY,
                    _accelerometerData->angularAccelerationZ);
        }
        _accelerationForceComponent.update();
    }
    if (_activeOpticForceComponent.isEnabled() || _activeOpticForceComponent.isDisabling()) {
        _activeOpticForceComponent.update();
    }
    if (_azimuthForceComponent.isEnabled() || _azimuthForceComponent.isDisabling()) {
        _azimuthForceComponent.update();
    }
    if (_balanceForceComponent.isEnabled() || _balanceForceComponent.isDisabling()) {
        if (_balanceForceComponent.isEnabled()) {
            _balanceForceComponent.applyBalanceForcesByMirrorForces(
                    _hardpointActuatorData->fx, _hardpointActuatorData->fy, _hardpointActuatorData->fz,
                    _hardpointActuatorData->mx, _hardpointActuatorData->my, _hardpointActuatorData->mz);
        }
        _balanceForceComponent.update();
    }
    if (_elevationForceComponent.isEnabled() || _elevationForceComponent.isDisabling()) {
        if (_elevationForceComponent.isEnabled()) {
            double elevationAngle = _forceActuatorSettings->UseInclinometer
                                            ? _inclinometerData->inclinometerAngle
                                            : _elevation_Angle_Actual;
            // Convert elevation angle to zenith angle (used by matrix)
            elevationAngle = 90.0 - elevationAngle;
            _elevationForceComponent.applyElevationForcesByElevationAngle(elevationAngle);
        }
        _elevationForceComponent.update();
    }
    if (_offsetForceComponent.isEnabled() || _offsetForceComponent.isDisabling()) {
        _offsetForceComponent.update();
    }
    if (_staticForceComponent.isEnabled() || _staticForceComponent.isDisabling()) {
        _staticForceComponent.update();
    }
    if (_thermalForceComponent.isEnabled() || _thermalForceComponent.isDisabling()) {
        _thermalForceComponent.update();
    }
    if (_velocityForceComponent.isEnabled() || _velocityForceComponent.isDisabling()) {
        if (_velocityForceComponent.isEnabled()) {
            _velocityForceComponent.applyVelocityForcesByAngularVelocity(
                    _gyroData->angularVelocityX, _gyroData->angularVelocityY, _gyroData->angularVelocityZ);
        }
        _velocityForceComponent.update();
    }
}

void ForceController::processAppliedForces() {
    spdlog::trace("ForceController: processAppliedForces()");
    _sumAllForces();
    _convertForcesToSetpoints();
    _checkMirrorMoments();
    _checkNearNeighbors();
    _checkMirrorWeight();
    _checkFarNeighbors();
    M1M3SSPublisher::get().tryLogForceSetpointWarning();
}

void ForceController::applyAberrationForcesByBendingModes(float* coefficients) {
    spdlog::info("ForceController: applyAberrationForcesByBendingModes()");
    if (!_aberrationForceComponent.isEnabled()) {
        _aberrationForceComponent.enable();
    }
    _aberrationForceComponent.applyAberrationForcesByBendingModes(coefficients);
}

void ForceController::applyAberrationForces(float* z) {
    spdlog::info("ForceController: applyAberrationForces()");
    if (!_aberrationForceComponent.isEnabled()) {
        _aberrationForceComponent.enable();
    }
    _aberrationForceComponent.applyAberrationForces(z);
}

void ForceController::zeroAberrationForces() {
    spdlog::info("ForceController: zeroAberrationForces()");
    if (_aberrationForceComponent.isEnabled()) {
        _aberrationForceComponent.disable();
    }
}

void ForceController::applyAccelerationForces() {
    spdlog::info("ForceController: applyAccelerationForces()");
    if (!_accelerationForceComponent.isEnabled()) {
        _accelerationForceComponent.enable();
    }
}

void ForceController::zeroAccelerationForces() {
    spdlog::info("ForceController: zeroAccelerationForces()");
    if (_accelerationForceComponent.isEnabled()) {
        _accelerationForceComponent.disable();
    }
}

void ForceController::applyActiveOpticForcesByBendingModes(float* coefficients) {
    spdlog::info("ForceController: applyActiveOpticForcesByBendingModes()");
    if (!_activeOpticForceComponent.isEnabled()) {
        _activeOpticForceComponent.enable();
    }
    _activeOpticForceComponent.applyActiveOpticForcesByBendingModes(coefficients);
}

void ForceController::applyActiveOpticForces(float* z) {
    spdlog::info("ForceController: applyActiveOpticForces()");
    if (!_activeOpticForceComponent.isEnabled()) {
        _activeOpticForceComponent.enable();
    }
    _activeOpticForceComponent.applyActiveOpticForces(z);
}

void ForceController::zeroActiveOpticForces() {
    spdlog::info("ForceController: zeroActiveOpticForces()");
    if (_activeOpticForceComponent.isEnabled()) {
        _activeOpticForceComponent.disable();
    }
}

void ForceController::applyAzimuthForces() {
    spdlog::info("ForceController: applyAzimuthForces()");
    if (!_azimuthForceComponent.isEnabled()) {
        _azimuthForceComponent.enable();
    }
}

void ForceController::updateAzimuthForces(float azimuthAngle) {
    spdlog::trace("ForceController: updateAzimuthForces()");
    _azimuthForceComponent.applyAzimuthForcesByAzimuthAngle(azimuthAngle);
}

void ForceController::zeroAzimuthForces() {
    spdlog::info("ForceController: zeroAzimuthForces()");
    if (_azimuthForceComponent.isEnabled()) {
        _azimuthForceComponent.disable();
    }
}

void ForceController::applyBalanceForces() {
    spdlog::info("ForceController: applyBalanceForces()");
    if (!_balanceForceComponent.isEnabled()) {
        _balanceForceComponent.enable();
    }
}

void ForceController::zeroBalanceForces() {
    spdlog::info("ForceController: zeroBalanceForces()");
    if (_balanceForceComponent.isEnabled()) {
        _balanceForceComponent.disable();
    }
}

void ForceController::updatePID(int id, PIDParameters parameters) {
    spdlog::info("ForceController: updatePID()");
    _balanceForceComponent.updatePID(id, parameters);
}

void ForceController::resetPID(int id) {
    spdlog::info("ForceController: resetPID()");
    _balanceForceComponent.resetPID(id);
}

void ForceController::resetPIDs() {
    spdlog::info("ForceController: resetPIDs()");
    _balanceForceComponent.resetPIDs();
}

void ForceController::applyElevationForces() {
    spdlog::info("ForceController: applyElevationForces()");
    if (!_elevationForceComponent.isEnabled()) {
        _elevationForceComponent.enable();
    }
}

void ForceController::zeroElevationForces() {
    spdlog::info("ForceController: zeroElevationForces()");
    if (_elevationForceComponent.isEnabled()) {
        _elevationForceComponent.disable();
    }
}

void ForceController::applyOffsetForces(float* x, float* y, float* z) {
    spdlog::info("ForceController: applyOffsetForces()");
    if (!_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.enable();
    }
    _offsetForceComponent.applyOffsetForces(x, y, z);
}

void ForceController::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                                      float yMoment, float zMoment) {
    spdlog::info(
            "ForceController: applyOffsetForcesByMirrorForces({:.1f}, {:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    if (!_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.enable();
    }
    _offsetForceComponent.applyOffsetForcesByMirrorForces(xForce, yForce, zForce, xMoment, yMoment, zMoment);
}

void ForceController::zeroOffsetForces() {
    spdlog::info("ForceController: zeroOffsetForces()");
    if (_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.disable();
    }
    _offsetForceComponent.zeroOffsetForces();
}

void ForceController::applyActuatorOffset(char axis, int index, float offset) {
    spdlog::info("ForceController: applyActuatorOffset({}, {}, {:.1f})", axis, index, offset);
    if (!_offsetForceComponent.isEnabled()) {
        _offsetForceComponent.enable();
    }
    _offsetForceComponent.applyActuatorOffset(axis, index, offset);
}

void ForceController::applyStaticForces() {
    spdlog::info("ForceController: applyStaticForces()");
    if (!_staticForceComponent.isEnabled()) {
        _staticForceComponent.enable();
    }
    _staticForceComponent.applyStaticForces(&_forceActuatorSettings->StaticXTable,
                                            &_forceActuatorSettings->StaticYTable,
                                            &_forceActuatorSettings->StaticZTable);
}

void ForceController::zeroStaticForces() {
    spdlog::info("ForceController: zeroStaticForces()");
    if (_staticForceComponent.isEnabled()) {
        _staticForceComponent.disable();
    }
}

void ForceController::applyThermalForces() {
    spdlog::info("ForceController: applyThermalForces()");
    if (!_thermalForceComponent.isEnabled()) {
        _thermalForceComponent.enable();
    }
}

void ForceController::updateThermalForces(float temperature) {
    spdlog::trace("ForceController: updateThermalForces()");
    _thermalForceComponent.applyThermalForcesByMirrorTemperature(temperature);
}

void ForceController::zeroThermalForces() {
    spdlog::info("ForceController: zeroThermalForces()");
    if (_thermalForceComponent.isEnabled()) {
        _thermalForceComponent.disable();
    }
}

void ForceController::applyVelocityForces() {
    spdlog::info("ForceController: applyVelocityForces()");
    if (!_velocityForceComponent.isEnabled()) {
        _velocityForceComponent.enable();
    }
}

void ForceController::zeroVelocityForces() {
    spdlog::info("ForceController: zeroVelocityForces()");
    if (_velocityForceComponent.isEnabled()) {
        _velocityForceComponent.disable();
    }
}

void ForceController::_sumAllForces() {
    spdlog::trace("ForceController: sumAllForces()");
    _finalForceComponent.applyForcesByComponents();
    _finalForceComponent.update();
}

void ForceController::_convertForcesToSetpoints() {
    spdlog::trace("ForceController: convertForcesToSetpoints()");
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
            _forceSetpointWarning->safetyLimitWarning[pIndex] =
                    _forceSetpointWarning->safetyLimitWarning[pIndex] ||
                    !Range::InRangeAndCoerce((int)secondaryLowFault, (int)secondaryHighFault,
                                             _preclippedCylinderForces->secondaryCylinderForces[sIndex],
                                             _appliedCylinderForces->secondaryCylinderForces + sIndex);
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
        _forceSetpointWarning->safetyLimitWarning[pIndex] =
                _forceSetpointWarning->safetyLimitWarning[pIndex] ||
                !Range::InRangeAndCoerce((int)primaryLowFault, (int)primaryHighFault,
                                         _preclippedCylinderForces->primaryCylinderForces[pIndex],
                                         _appliedCylinderForces->primaryCylinderForces + pIndex);

        clippingRequired |= _forceSetpointWarning->safetyLimitWarning[pIndex];
    }
    _appliedCylinderForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedCylinderForces->timestamp = _appliedCylinderForces->timestamp;
    _safetyController->forceControllerNotifySafetyLimit(clippingRequired);
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedCylinderForces();
    }
    M1M3SSPublisher::get().logAppliedCylinderForces();
}

bool ForceController::_checkMirrorMoments() {
    spdlog::trace("ForceController: checkMirrorMoments()");
    float xMoment = _appliedForces->mx;
    float yMoment = _appliedForces->my;
    float zMoment = _appliedForces->mz;
    _forceSetpointWarning->xMomentWarning = !Range::InRange(
            _forceActuatorSettings->MirrorXMoment *
                    _forceActuatorSettings->SetpointXMomentHighLimitPercentage,
            _forceActuatorSettings->MirrorXMoment * _forceActuatorSettings->SetpointXMomentLowLimitPercentage,
            xMoment);
    _forceSetpointWarning->yMomentWarning = !Range::InRange(
            _forceActuatorSettings->MirrorYMoment *
                    _forceActuatorSettings->SetpointYMomentHighLimitPercentage,
            _forceActuatorSettings->MirrorYMoment * _forceActuatorSettings->SetpointYMomentLowLimitPercentage,
            yMoment);
    _forceSetpointWarning->zMomentWarning = !Range::InRange(
            _forceActuatorSettings->MirrorZMoment *
                    _forceActuatorSettings->SetpointZMomentHighLimitPercentage,
            _forceActuatorSettings->MirrorZMoment * _forceActuatorSettings->SetpointZMomentLowLimitPercentage,
            zMoment);
    _safetyController->forceControllerNotifyXMomentLimit(_forceSetpointWarning->xMomentWarning);
    _safetyController->forceControllerNotifyYMomentLimit(_forceSetpointWarning->yMomentWarning);
    _safetyController->forceControllerNotifyZMomentLimit(_forceSetpointWarning->zMomentWarning);
    return _forceSetpointWarning->xMomentWarning || _forceSetpointWarning->yMomentWarning ||
           _forceSetpointWarning->zMomentWarning;
}

bool ForceController::_checkNearNeighbors() {
    spdlog::trace("ForceController: checkNearNeighbors()");
    float nominalZ = _mirrorWeight / (float)FA_COUNT;
    bool warningChanged = false;
    _forceSetpointWarning->anyNearNeighborWarning = false;
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
        float nearZ = 0;
        int nearNeighbors = _neighbors[zIndex].NearZIDs.size();
        for (int j = 0; j < nearNeighbors; ++j) {
            int neighborZIndex = _neighbors[zIndex].NearZIDs[j];

            nearZ += _appliedForces->zForces[neighborZIndex];
        }
        nearZ /= nearNeighbors;
        float deltaZ = 0;

        deltaZ = std::abs(_appliedForces->zForces[zIndex] - nearZ);

        bool previousWarning = _forceSetpointWarning->nearNeighborWarning[zIndex];
        _forceSetpointWarning->nearNeighborWarning[zIndex] =
                deltaZ > (nominalZ * _forceActuatorSettings->SetpointNearNeighborLimitPercentage);
        _forceSetpointWarning->anyNearNeighborWarning = _forceSetpointWarning->anyNearNeighborWarning ||
                                                        _forceSetpointWarning->nearNeighborWarning[zIndex];
        warningChanged |= (_forceSetpointWarning->nearNeighborWarning[zIndex] != previousWarning);
    }
    _safetyController->forceControllerNotifyNearNeighborCheck(_forceSetpointWarning->anyNearNeighborWarning);
    return warningChanged;
}

bool ForceController::_checkMirrorWeight() {
    spdlog::trace("ForceController: checkMirrorWeight()");
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
            globalForce > (_mirrorWeight * _forceActuatorSettings->SetpointMirrorWeightLimitPercentage);
    _safetyController->forceControllerNotifyMagnitudeLimit(_forceSetpointWarning->magnitudeWarning);
    return _forceSetpointWarning->magnitudeWarning != previousWarning;
}

bool ForceController::_checkFarNeighbors() {
    spdlog::trace("ForceController: checkFarNeighbors()");
    float globalX = _appliedForces->fx;
    float globalY = _appliedForces->fy;
    float globalZ = _appliedForces->fz;
    float globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
    float globalAverageForce = globalForce / FA_COUNT;
    float tolerance = globalAverageForce * _forceActuatorSettings->SetpointNearNeighborLimitPercentage;
    if (tolerance < 1) {
        tolerance = 1;
    }
    bool warningChanged = false;
    _forceSetpointWarning->anyFarNeighborWarning = false;
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
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
        int farNeighbors = _neighbors[zIndex].FarIDs.size();
        for (int j = 0; j < farNeighbors; ++j) {
            int neighborZIndex = _neighbors[zIndex].FarIDs[j];
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
        _forceSetpointWarning->farNeighborWarning[zIndex] =
                !Range::InRange(-tolerance, tolerance, magnitudeAverage - globalAverageForce);
        _forceSetpointWarning->anyFarNeighborWarning |= _forceSetpointWarning->farNeighborWarning[zIndex];
        warningChanged |= (_forceSetpointWarning->farNeighborWarning[zIndex] != previousWarning);
    }
    _safetyController->forceControllerNotifyFarNeighborCheck(_forceSetpointWarning->anyFarNeighborWarning);
    return warningChanged;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */