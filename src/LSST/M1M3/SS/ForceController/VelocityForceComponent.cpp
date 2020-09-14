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

#include <VelocityForceComponent.h>
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

VelocityForceComponent::VelocityForceComponent(
        SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Velocity") {
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedVelocityForces = M1M3SSPublisher::get().getEventAppliedVelocityForces();
    _rejectedVelocityForces = M1M3SSPublisher::get().getEventRejectedVelocityForces();
    this->maxRateOfChange = _forceActuatorSettings->VelocityComponentSettings.MaxRateOfChange;
    this->nearZeroValue = _forceActuatorSettings->VelocityComponentSettings.NearZeroValue;
}

void VelocityForceComponent::applyVelocityForces(float* x, float* y, float* z) {
    spdlog::trace("VelocityForceComponent: applyVelocityForces()");
    if (!this->enabled) {
        spdlog::error(
                "VelocityForceComponent: applyVelocityForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("VelocityForceComponent: applyVelocityForces() called when the component is disabling");
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

void VelocityForceComponent::applyVelocityForcesByAngularVelocity(float angularVelocityX,
                                                                  float angularVelocityY,
                                                                  float angularVelocityZ) {
    spdlog::trace("VelocityForceComponent: applyVelocityForcesByMirrorForces({:.1f}, {:.1f}, {:.1f})",
                  angularVelocityX, angularVelocityY, angularVelocityZ);
    DistributedForces forces = ForceConverter::calculateForceFromAngularVelocity(
            _forceActuatorSettings, angularVelocityX, angularVelocityY, angularVelocityZ);
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
    this->applyVelocityForces(xForces, yForces, zForces);
}

void VelocityForceComponent::postEnableDisableActions() {
    spdlog::debug("VelocityForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->velocityForcesApplied = this->enabled;
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void VelocityForceComponent::postUpdateActions() {
    spdlog::trace("VelocityForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedVelocityForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _rejectedVelocityForces->timestamp = _appliedVelocityForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->velocityForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->VelocityLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->VelocityLimitXTable[xIndex].HighFault;
            _rejectedVelocityForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _rejectedVelocityForces->xForces[xIndex],
                                             _appliedVelocityForces->xForces + xIndex);
            _forceSetpointWarning->velocityForceWarning[zIndex] =
                    _forceSetpointWarning->velocityForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->VelocityLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->VelocityLimitYTable[yIndex].HighFault;
            _rejectedVelocityForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _rejectedVelocityForces->yForces[yIndex],
                                             _appliedVelocityForces->yForces + yIndex);
            _forceSetpointWarning->velocityForceWarning[zIndex] =
                    _forceSetpointWarning->velocityForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->VelocityLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->VelocityLimitZTable[zIndex].HighFault;
        _rejectedVelocityForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedVelocityForces->zForces[zIndex],
                                              _appliedVelocityForces->zForces + zIndex);
        _forceSetpointWarning->velocityForceWarning[zIndex] =
                _forceSetpointWarning->velocityForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->velocityForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedVelocityForces->xForces,
            _appliedVelocityForces->yForces, _appliedVelocityForces->zForces);
    _appliedVelocityForces->fx = fm.Fx;
    _appliedVelocityForces->fy = fm.Fy;
    _appliedVelocityForces->fz = fm.Fz;
    _appliedVelocityForces->mx = fm.Mx;
    _appliedVelocityForces->my = fm.My;
    _appliedVelocityForces->mz = fm.Mz;
    _appliedVelocityForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _rejectedVelocityForces->xForces,
            _rejectedVelocityForces->yForces, _rejectedVelocityForces->zForces);
    _rejectedVelocityForces->fx = fm.Fx;
    _rejectedVelocityForces->fy = fm.Fy;
    _rejectedVelocityForces->fz = fm.Fz;
    _rejectedVelocityForces->mx = fm.Mx;
    _rejectedVelocityForces->my = fm.My;
    _rejectedVelocityForces->mz = fm.Mz;
    _rejectedVelocityForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyVelocityForceClipping(rejectionRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (rejectionRequired) {
        M1M3SSPublisher::get().logRejectedVelocityForces();
    }
    M1M3SSPublisher::get().logAppliedVelocityForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
