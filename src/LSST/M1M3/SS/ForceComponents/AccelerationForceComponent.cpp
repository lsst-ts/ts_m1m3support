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

#include <AccelerationForceComponent.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
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
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Acceleration", forceActuatorSettings->AccelerationComponentSettings) {
    _safetyController = Model::get().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedAccelerationForces = M1M3SSPublisher::get().getAppliedAccelerationForces();
    _preclippedAccelerationForces = M1M3SSPublisher::get().getEventPreclippedAccelerationForces();
}

void AccelerationForceComponent::applyAccelerationForces(float* x, float* y, float* z) {
    SPDLOG_TRACE("AccelerationForceComponent: applyAccelerationForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "AccelerationForceComponent: applyAccelerationForces() called when the component is not "
                "applied");
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

void AccelerationForceComponent::applyAccelerationForcesByAngularAccelerations(float angularAccelerationX,
                                                                               float angularAccelerationY,
                                                                               float angularAccelerationZ) {
    SPDLOG_TRACE(
            "AccelerationForceComponent: applyAccelerationForcesByAngularAccelerations(P:.1f}, {.1f}, {.1f})",
            angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    DistributedForces forces = ForceConverter::calculateForceFromAngularAcceleration(
            _forceActuatorSettings, angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    float xForces[FA_X_COUNT];
    float yForces[FA_Y_COUNT];
    float zForces[FA_Z_COUNT];
    for (int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
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
    applyAccelerationForces(xForces, yForces, zForces);
}

void AccelerationForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("AccelerationForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->accelerationForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void AccelerationForceComponent::postUpdateActions() {
    SPDLOG_TRACE("AccelerationForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedAccelerationForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedAccelerationForces->timestamp = _appliedAccelerationForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->accelerationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->AccelerationLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->AccelerationLimitXTable[xIndex].HighFault;
            _preclippedAccelerationForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclippedAccelerationForces->xForces[xIndex],
                                                  _appliedAccelerationForces->xForces + xIndex);
            _forceSetpointWarning->accelerationForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->accelerationForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->AccelerationLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->AccelerationLimitYTable[yIndex].HighFault;
            _preclippedAccelerationForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclippedAccelerationForces->yForces[yIndex],
                                                  _appliedAccelerationForces->yForces + yIndex);
            _forceSetpointWarning->accelerationForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->accelerationForceWarning[zIndex];
        }

        float zLowFault = _forceActuatorSettings->AccelerationLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->AccelerationLimitZTable[zIndex].HighFault;
        _preclippedAccelerationForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault,
                                              _preclippedAccelerationForces->zForces[zIndex],
                                              _appliedAccelerationForces->zForces + zIndex);
        _forceSetpointWarning->accelerationForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->accelerationForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->accelerationForceWarning[zIndex] || clippingRequired;
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
            _forceActuatorApplicationSettings, _forceActuatorSettings, _preclippedAccelerationForces->xForces,
            _preclippedAccelerationForces->yForces, _preclippedAccelerationForces->zForces);
    _preclippedAccelerationForces->fx = fm.Fx;
    _preclippedAccelerationForces->fy = fm.Fy;
    _preclippedAccelerationForces->fz = fm.Fz;
    _preclippedAccelerationForces->mx = fm.Mx;
    _preclippedAccelerationForces->my = fm.My;
    _preclippedAccelerationForces->mz = fm.Mz;
    _preclippedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyAccelerationForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedAccelerationForces();
    }
    M1M3SSPublisher::get().logAppliedAccelerationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
