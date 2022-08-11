/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

VelocityForceComponent::VelocityForceComponent(
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Velocity", forceActuatorSettings->VelocityComponentSettings) {
    _safetyController = Model::get().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedVelocityForces = M1M3SSPublisher::get().getAppliedVelocityForces();
    _preclippedVelocityForces = M1M3SSPublisher::get().getEventPreclippedVelocityForces();
}

void VelocityForceComponent::applyVelocityForces(float* x, float* y, float* z) {
    SPDLOG_TRACE("VelocityForceComponent: applyVelocityForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "VelocityForceComponent: applyVelocityForces() called when the component is not applied");
        return;
    }

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            xTarget[i] = x[i];
        }

        if (i < FA_Y_COUNT) {
            yTarget[i] = y[i];
        }

        zTarget[i] = z[i];
    }
}

void VelocityForceComponent::applyVelocityForcesByAngularVelocity(float angularVelocityX,
                                                                  float angularVelocityY,
                                                                  float angularVelocityZ) {
    SPDLOG_TRACE("VelocityForceComponent: applyVelocityForcesByMirrorForces({:.1f}, {:.1f}, {:.1f})",
                 angularVelocityX, angularVelocityY, angularVelocityZ);
    DistributedForces forces = ForceConverter::calculateForceFromAngularVelocity(
            _forceActuatorSettings, angularVelocityX, angularVelocityY, angularVelocityZ);
    float xForces[FA_X_COUNT];
    float yForces[FA_Y_COUNT];
    float zForces[FA_Z_COUNT];
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
    applyVelocityForces(xForces, yForces, zForces);
}

void VelocityForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("VelocityForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->velocityForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void VelocityForceComponent::postUpdateActions() {
    SPDLOG_TRACE("VelocityForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedVelocityForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedVelocityForces->timestamp = _appliedVelocityForces->timestamp;
    for (int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->velocityForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->VelocityLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->VelocityLimitXTable[xIndex].HighFault;
            _preclippedVelocityForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclippedVelocityForces->xForces[xIndex],
                                                  _appliedVelocityForces->xForces + xIndex);
            _forceSetpointWarning->velocityForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->velocityForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->VelocityLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->VelocityLimitYTable[yIndex].HighFault;
            _preclippedVelocityForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclippedVelocityForces->yForces[yIndex],
                                                  _appliedVelocityForces->yForces + yIndex);
            _forceSetpointWarning->velocityForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->velocityForceWarning[zIndex];
        }

        float zLowFault = _forceActuatorSettings->VelocityLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->VelocityLimitZTable[zIndex].HighFault;
        _preclippedVelocityForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedVelocityForces->zForces[zIndex],
                                         _appliedVelocityForces->zForces + zIndex);
        _forceSetpointWarning->velocityForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->velocityForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->velocityForceWarning[zIndex] || clippingRequired;
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
            _forceActuatorApplicationSettings, _forceActuatorSettings, _preclippedVelocityForces->xForces,
            _preclippedVelocityForces->yForces, _preclippedVelocityForces->zForces);
    _preclippedVelocityForces->fx = fm.Fx;
    _preclippedVelocityForces->fy = fm.Fy;
    _preclippedVelocityForces->fz = fm.Fz;
    _preclippedVelocityForces->mx = fm.Mx;
    _preclippedVelocityForces->my = fm.My;
    _preclippedVelocityForces->mz = fm.Mz;
    _preclippedVelocityForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyVelocityForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedVelocityForces();
    }
    M1M3SSPublisher::get().logAppliedVelocityForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
