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

FinalForceComponent::FinalForceComponent(SafetyController* safetyController,
                                         ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                         ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Final";

    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedForces = M1M3SSPublisher::get().getEventAppliedForces();
    _rejectedForces = M1M3SSPublisher::get().getEventRejectedForces();
    this->maxRateOfChange = _forceActuatorSettings->FinalComponentSettings.MaxRateOfChange;
    this->nearZeroValue = _forceActuatorSettings->FinalComponentSettings.NearZeroValue;

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

    this->enable();
}

void FinalForceComponent::applyForces(float* x, float* y, float* z) {
    spdlog::trace("FinalForceComponent: applyForces()");
    if (!this->isEnabled()) {
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] = x[i];
        }

        if (i < 100) {
            this->yTarget[i] = y[i];
        }

        this->zTarget[i] = z[i];
    }
}

void FinalForceComponent::applyForcesByComponents() {
    spdlog::trace("FinalForceComponent: applyForcesByComponents()");
    if (!this->isEnabled()) {
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] = (_appliedAccelerationForces->xForces[i] + _appliedAzimuthForces->xForces[i] +
                                _appliedBalanceForces->xForces[i] + _appliedElevationForces->xForces[i] +
                                _appliedOffsetForces->xForces[i] + _appliedStaticForces->xForces[i] +
                                _appliedThermalForces->xForces[i] + _appliedVelocityForces->xForces[i]);
        }

        if (i < 100) {
            this->yTarget[i] = (_appliedAccelerationForces->yForces[i] + _appliedAzimuthForces->yForces[i] +
                                _appliedBalanceForces->yForces[i] + _appliedElevationForces->yForces[i] +
                                _appliedOffsetForces->yForces[i] + _appliedStaticForces->yForces[i] +
                                _appliedThermalForces->yForces[i] + _appliedVelocityForces->yForces[i]);
        }

        this->zTarget[i] = (_appliedAberrationForces->zForces[i] + _appliedAccelerationForces->zForces[i] +
                            _appliedActiveOpticForces->zForces[i] + _appliedAzimuthForces->zForces[i] +
                            _appliedBalanceForces->zForces[i] + _appliedElevationForces->zForces[i] +
                            _appliedOffsetForces->zForces[i] + _appliedStaticForces->zForces[i] +
                            _appliedThermalForces->zForces[i] + _appliedVelocityForces->zForces[i]);
    }
}

void FinalForceComponent::postEnableDisableActions() {
    spdlog::trace("FinalForceComponent: postEnableDisableActions()");
}

void FinalForceComponent::postUpdateActions() {
    spdlog::trace("FinalForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _rejectedForces->timestamp = _appliedForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->forceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->ForceLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->ForceLimitXTable[xIndex].HighFault;
            _rejectedForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, _rejectedForces->xForces[xIndex],
                                                  _appliedForces->xForces + xIndex);
            _forceSetpointWarning->forceWarning[zIndex] =
                    _forceSetpointWarning->forceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->ForceLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->ForceLimitYTable[yIndex].HighFault;
            _rejectedForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, _rejectedForces->yForces[yIndex],
                                                  _appliedForces->yForces + yIndex);
            _forceSetpointWarning->forceWarning[zIndex] =
                    _forceSetpointWarning->forceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->ForceLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ForceLimitZTable[zIndex].HighFault;
        _rejectedForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedForces->zForces[zIndex],
                                              _appliedForces->zForces + zIndex);
        _forceSetpointWarning->forceWarning[zIndex] =
                _forceSetpointWarning->forceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->forceWarning[zIndex];
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
                                                   _rejectedForces->xForces, _rejectedForces->yForces,
                                                   _rejectedForces->zForces);
    _rejectedForces->fx = fm.Fx;
    _rejectedForces->fy = fm.Fy;
    _rejectedForces->fz = fm.Fz;
    _rejectedForces->mx = fm.Mx;
    _rejectedForces->my = fm.My;
    _rejectedForces->mz = fm.Mz;
    _rejectedForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyForceClipping(rejectionRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (rejectionRequired) {
        M1M3SSPublisher::get().logRejectedForces();
    }
    M1M3SSPublisher::get().logAppliedForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
