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

#include <AccelerationForceComponent.h>
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

AccelerationForceComponent::AccelerationForceComponent(
        SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Acceleration";

    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedAccelerationForces = M1M3SSPublisher::get().getEventAppliedAccelerationForces();
    _rejectedAccelerationForces = M1M3SSPublisher::get().getEventRejectedAccelerationForces();
    this->maxRateOfChange = _forceActuatorSettings->AccelerationComponentSettings.MaxRateOfChange;
    this->nearZeroValue = _forceActuatorSettings->AccelerationComponentSettings.NearZeroValue;
}

void AccelerationForceComponent::applyAccelerationForces(float* x, float* y, float* z) {
    spdlog::trace("AccelerationForceComponent: applyAccelerationForces()");
    if (!this->enabled) {
        spdlog::error(
                "AccelerationForceComponent: applyAccelerationForces() called when the component is not "
                "applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn(
                "AccelerationForceComponent: applyAccelerationForces() called when the component is "
                "disabling");
        return;
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

void AccelerationForceComponent::applyAccelerationForcesByAngularAccelerations(float angularAccelerationX,
                                                                               float angularAccelerationY,
                                                                               float angularAccelerationZ) {
    spdlog::trace(
            "AccelerationForceComponent: applyAccelerationForcesByAngularAccelerations(P:.1f}, {.1f}, {.1f})",
            angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    DistributedForces forces = ForceConverter::calculateForceFromAngularAcceleration(
            _forceActuatorSettings, angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    float xForces[12];
    float yForces[100];
    float zForces[156];
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        if (xIndex != -1) {
            xForces[xIndex] = forces.XForces[zIndex];
        }
        if (yIndex != -1) {
            yForces[yIndex] = forces.YForces[zIndex];
        }
        zForces[zIndex] = forces.ZForces[zIndex];
    }
    this->applyAccelerationForces(xForces, yForces, zForces);
}

void AccelerationForceComponent::postEnableDisableActions() {
    spdlog::debug("AccelerationForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->accelerationForcesApplied = this->enabled;
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void AccelerationForceComponent::postUpdateActions() {
    spdlog::trace("AccelerationForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedAccelerationForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _rejectedAccelerationForces->timestamp = _appliedAccelerationForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->accelerationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->AccelerationLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->AccelerationLimitXTable[xIndex].HighFault;
            _rejectedAccelerationForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _rejectedAccelerationForces->xForces[xIndex],
                                                  _appliedAccelerationForces->xForces + xIndex);
            _forceSetpointWarning->accelerationForceWarning[zIndex] =
                    _forceSetpointWarning->accelerationForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->AccelerationLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->AccelerationLimitYTable[yIndex].HighFault;
            _rejectedAccelerationForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _rejectedAccelerationForces->yForces[yIndex],
                                                  _appliedAccelerationForces->yForces + yIndex);
            _forceSetpointWarning->accelerationForceWarning[zIndex] =
                    _forceSetpointWarning->accelerationForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->AccelerationLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->AccelerationLimitZTable[zIndex].HighFault;
        _rejectedAccelerationForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedAccelerationForces->zForces[zIndex],
                                         _appliedAccelerationForces->zForces + zIndex);
        _forceSetpointWarning->accelerationForceWarning[zIndex] =
                _forceSetpointWarning->accelerationForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->accelerationForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedAccelerationForces->xForces,
            _appliedAccelerationForces->yForces, _appliedAccelerationForces->zForces);
    _appliedAccelerationForces->fx = fm.Fx;
    _appliedAccelerationForces->fy = fm.Fy;
    _appliedAccelerationForces->fz = fm.Fz;
    _appliedAccelerationForces->mx = fm.Mx;
    _appliedAccelerationForces->my = fm.My;
    _appliedAccelerationForces->mz = fm.Mz;
    _appliedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _rejectedAccelerationForces->xForces,
            _rejectedAccelerationForces->yForces, _rejectedAccelerationForces->zForces);
    _rejectedAccelerationForces->fx = fm.Fx;
    _rejectedAccelerationForces->fy = fm.Fy;
    _rejectedAccelerationForces->fz = fm.Fz;
    _rejectedAccelerationForces->mx = fm.Mx;
    _rejectedAccelerationForces->my = fm.My;
    _rejectedAccelerationForces->mz = fm.Mz;
    _rejectedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyAccelerationForceClipping(rejectionRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (rejectionRequired) {
        M1M3SSPublisher::get().logRejectedAccelerationForces();
    }
    M1M3SSPublisher::get().logAppliedAccelerationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
