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

#include <DistributedForces.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForceControllerState.h>
#include <ForcesAndMoments.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <OffsetForceComponent.h>
#include <Range.h>
#include <SafetyController.h>

using namespace LSST::M1M3::SS;

OffsetForceComponent::OffsetForceComponent()
        : ForceComponent("Offset", &ForceActuatorSettings::instance().OffsetComponentSettings),
          _preclipped_offset_forces(
                  [](MTM1M3_logevent_preclippedOffsetForcesC* data) {
                      M1M3SSPublisher::instance().logPreclippedOffsetForces(data);
                  },
                  ForceActuatorSettings::instance().preclippedIgnoreChanges,
                  std::chrono::milliseconds(
                          static_cast<int>(ForceActuatorSettings::instance().preclippedMaxDelay * 1000.0))) {
    _safetyController = Model::instance().getSafetyController();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedOffsetForces = M1M3SSPublisher::instance().getEventAppliedOffsetForces();
    zeroOffsetForces();
}

void OffsetForceComponent::applyOffsetForces(const std::vector<float>& x, const std::vector<float>& y,
                                             const std::vector<float>& z) {
    SPDLOG_DEBUG("OffsetForceComponent: applyOffsetForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "OffsetForceComponent: applyOffsetForces() called when the "
                "component is not applied");
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

void OffsetForceComponent::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce,
                                                           float xMoment, float yMoment, float zMoment) {
    SPDLOG_DEBUG(
            "OffsetForceComponent: applyOffsetForcesByMirrorForces({:.1f}, "
            "{:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    DistributedForces forces = ForceActuatorSettings::instance().calculateForceDistribution(
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    std::vector<float> xForces(FA_X_COUNT, 0);
    std::vector<float> yForces(FA_Y_COUNT, 0);
    std::vector<float> zForces(FA_Z_COUNT, 0);

    auto& faa_settings = ForceActuatorApplicationSettings::instance();

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
    applyOffsetForces(xForces, yForces, zForces);
}

void OffsetForceComponent::applyActuatorOffset(char axis, int index, float offset) {
    SPDLOG_DEBUG("OffsetForceComponent: applyActuatorOffset({}, {}, {:1.f}", axis, index, offset);
    switch (axis) {
        case 'X':
            xTarget[index] = offset;
            break;
        case 'Y':
            yTarget[index] = offset;
            break;
        case 'Z':
            zTarget[index] = offset;
            break;
    }
}

void OffsetForceComponent::zeroOffsetForces() {
    memset(xTarget, 0, sizeof(xTarget));
    memset(yTarget, 0, sizeof(yTarget));
    memset(zTarget, 0, sizeof(zTarget));
}

void OffsetForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("OffsetForceComponent: postEnableDisableActions()");

    ForceControllerState::instance().set_offsetForcesApplied(isEnabled());
}

void OffsetForceComponent::postUpdateActions() {
    SPDLOG_TRACE("OffsetForceController: postUpdateActions()");

    auto& faa_settings = ForceActuatorApplicationSettings::instance();

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedOffsetForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclipped_offset_forces.timestamp = _appliedOffsetForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

        _forceSetpointWarning->offsetForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().OffsetLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().OffsetLimitXTable[xIndex].HighFault;
            _preclipped_offset_forces.xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclipped_offset_forces.xForces[xIndex],
                                             _appliedOffsetForces->xForces[xIndex]);
            _forceSetpointWarning->offsetForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->offsetForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().OffsetLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().OffsetLimitYTable[yIndex].HighFault;
            _preclipped_offset_forces.yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclipped_offset_forces.yForces[yIndex],
                                             _appliedOffsetForces->yForces[yIndex]);
            _forceSetpointWarning->offsetForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->offsetForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().OffsetLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().OffsetLimitZTable[zIndex].HighFault;
        _preclipped_offset_forces.zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclipped_offset_forces.zForces[zIndex],
                                         _appliedOffsetForces->zForces[zIndex]);
        _forceSetpointWarning->offsetForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->offsetForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->offsetForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _appliedOffsetForces->xForces, _appliedOffsetForces->yForces, _appliedOffsetForces->zForces);
    _appliedOffsetForces->fx = fm.Fx;
    _appliedOffsetForces->fy = fm.Fy;
    _appliedOffsetForces->fz = fm.Fz;
    _appliedOffsetForces->mx = fm.Mx;
    _appliedOffsetForces->my = fm.My;
    _appliedOffsetForces->mz = fm.Mz;
    _appliedOffsetForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyOffsetForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        _preclipped_offset_forces.calculate_forces_and_moments();
        _preclipped_offset_forces.check_changes();
    }
    M1M3SSPublisher::instance().logAppliedOffsetForces();
}
