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

#include <spdlog/spdlog.h>

#include "AccelerationForceComponent.h"
#include "DistributedForces.h"
#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorSettings.h"
#include "ForceControllerState.h"
#include "ForcesAndMoments.h"
#include "M1M3SSPublisher.h"
#include "Model.h"
#include "Range.h"

using namespace LSST::M1M3::SS;

AccelerationForceComponent::AccelerationForceComponent()
        : ForceComponent("Acceleration", &ForceActuatorSettings::instance().AccelerationComponentSettings),
          _preclipped_acceleration_forces(
                  [](MTM1M3_logevent_preclippedAccelerationForcesC *data) {
                      M1M3SSPublisher::instance().logPreclippedAccelerationForces(data);
                  },
                  ForceActuatorSettings::instance().preclippedIgnoreChanges,
                  std::chrono::milliseconds(
                          static_cast<int>(ForceActuatorSettings::instance().preclippedMaxDelay * 1000.0))) {
    _safetyController = Model::instance().getSafetyController();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedAccelerationForces = M1M3SSPublisher::instance().getAppliedAccelerationForces();
}

void AccelerationForceComponent::applyAccelerationForces(const std::vector<float> &x,
                                                         const std::vector<float> &y,
                                                         const std::vector<float> &z) {
    SPDLOG_TRACE("AccelerationForceComponent: applyAccelerationForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "AccelerationForceComponent: applyAccelerationForces() called "
                "when the component is not "
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
            "AccelerationForceComponent: "
            "applyAccelerationForcesByAngularAccelerations(P:.1f}, {.1f}, {.1f})",
            angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    DistributedForces forces = ForceActuatorSettings::instance().calculateForceFromAngularAcceleration(
            angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    std::vector<float> xForces(FA_X_COUNT, 0);
    std::vector<float> yForces(FA_Y_COUNT, 0);
    std::vector<float> zForces(FA_Z_COUNT, 0);

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    for (int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

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

    ForceControllerState::instance().set_accelerationForcesApplied(isEnabled());
}

void AccelerationForceComponent::postUpdateActions() {
    SPDLOG_TRACE("AccelerationForceController: postUpdateActions()");

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedAccelerationForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclipped_acceleration_forces.timestamp = _appliedAccelerationForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

        _forceSetpointWarning->accelerationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().AccelerationLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().AccelerationLimitXTable[xIndex].HighFault;
            _preclipped_acceleration_forces.xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclipped_acceleration_forces.xForces[xIndex],
                                                  _appliedAccelerationForces->xForces[xIndex]);
            _forceSetpointWarning->accelerationForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->accelerationForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().AccelerationLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().AccelerationLimitYTable[yIndex].HighFault;
            _preclipped_acceleration_forces.yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclipped_acceleration_forces.yForces[yIndex],
                                                  _appliedAccelerationForces->yForces[yIndex]);
            _forceSetpointWarning->accelerationForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->accelerationForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().AccelerationLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().AccelerationLimitZTable[zIndex].HighFault;
        _preclipped_acceleration_forces.zForces[zIndex] = zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault,
                                              _preclipped_acceleration_forces.zForces[zIndex],
                                              _appliedAccelerationForces->zForces[zIndex]);
        _forceSetpointWarning->accelerationForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->accelerationForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->accelerationForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _appliedAccelerationForces->xForces, _appliedAccelerationForces->yForces,
            _appliedAccelerationForces->zForces);
    _appliedAccelerationForces->fx = fm.Fx;
    _appliedAccelerationForces->fy = fm.Fy;
    _appliedAccelerationForces->fz = fm.Fz;
    _appliedAccelerationForces->mx = fm.Mx;
    _appliedAccelerationForces->my = fm.My;
    _appliedAccelerationForces->mz = fm.Mz;
    _appliedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyAccelerationForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        _preclipped_acceleration_forces.calculate_forces_and_moments();
        _preclipped_acceleration_forces.check_changes();
    }
    M1M3SSPublisher::instance().logAppliedAccelerationForces();
}
