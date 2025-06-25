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

#include "DistributedForces.h"
#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorSettings.h"
#include "ForceControllerState.h"
#include "ForcesAndMoments.h"
#include "M1M3SSPublisher.h"
#include "Model.h"
#include "Range.h"
#include "SafetyController.h"
#include "VelocityForceComponent.h"

using namespace LSST::M1M3::SS;

VelocityForceComponent::VelocityForceComponent()
        : ForceComponent("Velocity", &ForceActuatorSettings::instance().VelocityComponentSettings),
          _preclipped_velocity_forces(
                  [](MTM1M3_logevent_preclippedVelocityForcesC *data) {
                      M1M3SSPublisher::instance().logPreclippedVelocityForces(data);
                  },
                  ForceActuatorSettings::instance().preclippedIgnoreChanges,
                  std::chrono::milliseconds(
                          static_cast<int>(ForceActuatorSettings::instance().preclippedMaxDelay * 1000.0))) {
    _safetyController = Model::instance().getSafetyController();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedVelocityForces = M1M3SSPublisher::instance().getAppliedVelocityForces();
}

void VelocityForceComponent::applyVelocityForces(const std::vector<float> &x, const std::vector<float> &y,
                                                 const std::vector<float> &z) {
    SPDLOG_TRACE("VelocityForceComponent: applyVelocityForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "VelocityForceComponent: applyVelocityForces() called when "
                "the component is not applied");
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
    SPDLOG_TRACE(
            "VelocityForceComponent: "
            "applyVelocityForcesByMirrorForces({:.1f}, {:.1f}, {:.1f})",
            angularVelocityX, angularVelocityY, angularVelocityZ);
    DistributedForces forces = ForceActuatorSettings::instance().calculateForceFromAngularVelocity(
            angularVelocityX, angularVelocityY, angularVelocityZ);
    std::vector<float> xForces(FA_X_COUNT, 0);
    std::vector<float> yForces(FA_Y_COUNT, 0);
    std::vector<float> zForces(FA_Z_COUNT, 0);

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
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
    applyVelocityForces(xForces, yForces, zForces);
}

void VelocityForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("VelocityForceComponent: postEnableDisableActions()");

    ForceControllerState::instance().set_velocityForcesApplied(isEnabled());
}

void VelocityForceComponent::postUpdateActions() {
    SPDLOG_TRACE("VelocityForceController: postUpdateActions()");

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedVelocityForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclipped_velocity_forces.timestamp = _appliedVelocityForces->timestamp;
    for (int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

        _forceSetpointWarning->velocityForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().VelocityLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().VelocityLimitXTable[xIndex].HighFault;
            _preclipped_velocity_forces.xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclipped_velocity_forces.xForces[xIndex],
                                                  _appliedVelocityForces->xForces[xIndex]);
            _forceSetpointWarning->velocityForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->velocityForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().VelocityLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().VelocityLimitYTable[yIndex].HighFault;
            _preclipped_velocity_forces.yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclipped_velocity_forces.yForces[yIndex],
                                                  _appliedVelocityForces->yForces[yIndex]);
            _forceSetpointWarning->velocityForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->velocityForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().VelocityLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().VelocityLimitZTable[zIndex].HighFault;
        _preclipped_velocity_forces.zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclipped_velocity_forces.zForces[zIndex],
                                         _appliedVelocityForces->zForces[zIndex]);
        _forceSetpointWarning->velocityForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->velocityForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->velocityForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _appliedVelocityForces->xForces, _appliedVelocityForces->yForces,
            _appliedVelocityForces->zForces);
    _appliedVelocityForces->fx = fm.Fx;
    _appliedVelocityForces->fy = fm.Fy;
    _appliedVelocityForces->fz = fm.Fz;
    _appliedVelocityForces->mx = fm.Mx;
    _appliedVelocityForces->my = fm.My;
    _appliedVelocityForces->mz = fm.Mz;
    _appliedVelocityForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyVelocityForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        _preclipped_velocity_forces.calculate_forces_and_moments();
        _preclipped_velocity_forces.check_changes();
    }
    M1M3SSPublisher::instance().logAppliedVelocityForces();
}
