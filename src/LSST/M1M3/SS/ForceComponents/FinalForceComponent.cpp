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

#include <FinalForceComponent.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SafetyController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <Range.h>
#include <ForcesAndMoments.h>
#include <ForceConverter.h>
#include <DistributedForces.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FinalForceComponent::FinalForceComponent(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                         ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Final", forceActuatorSettings->FinalComponentSettings) {
    _safetyController = Model::get().getSafetyController();
    _enabledForceActuators = M1M3SSPublisher::get().getEnabledForceActuators();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedForces = M1M3SSPublisher::get().getEventAppliedForces();
    _preclippedForces = M1M3SSPublisher::get().getEventPreclippedForces();

    _appliedAberrationForces = M1M3SSPublisher::get().getEventAppliedAberrationForces();
    _appliedAccelerationForces = M1M3SSPublisher::get().getEventAppliedAccelerationForces();
    _appliedActiveOpticForces = M1M3SSPublisher::get().getEventAppliedActiveOpticForces();
    _appliedAzimuthForces = M1M3SSPublisher::get().getEventAppliedAzimuthForces();
    _appliedBalanceForces = M1M3SSPublisher::get().getEventAppliedBalanceForces();
    _appliedElevationForces = M1M3SSPublisher::get().getEventAppliedElevationForces();
    _appliedOffsetForces = M1M3SSPublisher::get().getEventAppliedOffsetForces();
    _appliedStaticForces = M1M3SSPublisher::get().getEventAppliedStaticForces();
    _appliedThermalForces = M1M3SSPublisher::get().getEventAppliedThermalForces();
    _appliedVelocityForces = M1M3SSPublisher::get().getEventAppliedVelocityForces();

    enable();
}

void FinalForceComponent::applyForcesByComponents() {
    SPDLOG_TRACE("FinalForceComponent: applyForcesByComponents()");

    if (!isEnabled()) {
        enable();
    }

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < 12) {
            int zIndex = _forceActuatorApplicationSettings->XIndexToZIndex[i];

            if (_enabledForceActuators->forceActuatorEnabled[zIndex] == true) {
                xTarget[i] = (_appliedAccelerationForces->xForces[i] + _appliedAzimuthForces->xForces[i] +
                              _appliedBalanceForces->xForces[i] + _appliedElevationForces->xForces[i] +
                              _appliedOffsetForces->xForces[i] + _appliedStaticForces->xForces[i] +
                              _appliedThermalForces->xForces[i] + _appliedVelocityForces->xForces[i]);
            } else {
                xTarget[i] = 0;
            }
        }

        if (i < 100) {
            int zIndex = _forceActuatorApplicationSettings->YIndexToZIndex[i];

            if (_enabledForceActuators->forceActuatorEnabled[zIndex] == true) {
                yTarget[i] = (_appliedAccelerationForces->yForces[i] + _appliedAzimuthForces->yForces[i] +
                              _appliedBalanceForces->yForces[i] + _appliedElevationForces->yForces[i] +
                              _appliedOffsetForces->yForces[i] + _appliedStaticForces->yForces[i] +
                              _appliedThermalForces->yForces[i] + _appliedVelocityForces->yForces[i]);
            } else {
                yTarget[i] = 0;
            }
        }

        if (_enabledForceActuators->forceActuatorEnabled[i] == true) {
            zTarget[i] = (_appliedAberrationForces->zForces[i] + _appliedAccelerationForces->zForces[i] +
                          _appliedActiveOpticForces->zForces[i] + _appliedAzimuthForces->zForces[i] +
                          _appliedBalanceForces->zForces[i] + _appliedElevationForces->zForces[i] +
                          _appliedOffsetForces->zForces[i] + _appliedStaticForces->zForces[i] +
                          _appliedThermalForces->zForces[i] + _appliedVelocityForces->zForces[i]);
        } else {
            zTarget[i] = 0;
        }
    }
}

void FinalForceComponent::postEnableDisableActions() {
    SPDLOG_TRACE("FinalForceComponent: postEnableDisableActions()");
}

void FinalForceComponent::postUpdateActions() {
    SPDLOG_TRACE("FinalForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedForces->timestamp = _appliedForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->forceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->ForceLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->ForceLimitXTable[xIndex].HighFault;
            _preclippedForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclippedForces->xForces[xIndex],
                                                  _appliedForces->xForces + xIndex);
            _forceSetpointWarning->forceWarning[zIndex] |= notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->ForceLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->ForceLimitYTable[yIndex].HighFault;
            _preclippedForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclippedForces->yForces[yIndex],
                                                  _appliedForces->yForces + yIndex);
            _forceSetpointWarning->forceWarning[zIndex] |= notInRange;
        }

        float zLowFault = _forceActuatorSettings->ForceLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ForceLimitZTable[zIndex].HighFault;
        _preclippedForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedForces->zForces[zIndex],
                                              _appliedForces->zForces + zIndex);
        _forceSetpointWarning->forceWarning[zIndex] |= notInRange;
        clippingRequired |= _forceSetpointWarning->forceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedForces->xForces,
            _appliedForces->yForces, _appliedForces->zForces);
    _appliedForces->fx = fm.Fx;
    _appliedForces->fy = fm.Fy;
    _appliedForces->fz = fm.Fz;
    _appliedForces->mx = fm.Mx;
    _appliedForces->my = fm.My;
    _appliedForces->mz = fm.Mz;
    _appliedForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(_forceActuatorApplicationSettings, _forceActuatorSettings,
                                                   _preclippedForces->xForces, _preclippedForces->yForces,
                                                   _preclippedForces->zForces);
    _preclippedForces->fx = fm.Fx;
    _preclippedForces->fy = fm.Fy;
    _preclippedForces->fz = fm.Fz;
    _preclippedForces->mx = fm.Mx;
    _preclippedForces->my = fm.My;
    _preclippedForces->mz = fm.Mz;
    _preclippedForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedForces();
    }
    M1M3SSPublisher::get().logAppliedForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
