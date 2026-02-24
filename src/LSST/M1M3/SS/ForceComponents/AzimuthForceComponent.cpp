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

#include <AzimuthForceComponent.h>
#include <DistributedForces.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForceControllerState.h>
#include <ForcesAndMoments.h>
#include <LimitLog.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <Range.h>

using namespace LSST::M1M3::SS;

AzimuthForceComponent::AzimuthForceComponent()
        : ForceComponent("Azimuth", &ForceActuatorSettings::instance().AzimuthComponentSettings),
          _preclipped_azimuth_forces(
                  [](MTM1M3_logevent_preclippedAzimuthForcesC* data) {
                      M1M3SSPublisher::instance().logPreclippedAzimuthForces(data);
                  },
                  ForceActuatorSettings::instance().preclippedIgnoreChanges,
                  std::chrono::milliseconds(
                          static_cast<int>(ForceActuatorSettings::instance().preclippedMaxDelay * 1000.0))) {
    _safetyController = Model::instance().getSafetyController();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedAzimuthForces = M1M3SSPublisher::instance().getAppliedAzimuthForces();
}

void AzimuthForceComponent::applyAzimuthForces(const std::vector<float>& x, const std::vector<float>& y,
                                               const std::vector<float>& z) {
    SPDLOG_TRACE("AzimuthForceComponent: applyAzimuthForces()");

    if (!isEnabled()) {
        using namespace std::chrono_literals;
        TG_LOG_ERROR(5s,
                     "AzimuthForceComponent: applyAzimuthForces() called when "
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
}  // namespace SS

void AzimuthForceComponent::applyAzimuthForcesByAzimuthAngle(float azimuthAngle) {
    SPDLOG_TRACE("AzimuthForceComponent: applyAzimuthForcesByMirrorForces({:.4f})", azimuthAngle);
    DistributedForces forces = ForceActuatorSettings::instance().calculateForceFromAzimuthAngle(azimuthAngle);
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
    applyAzimuthForces(xForces, yForces, zForces);
}

void AzimuthForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("AzimuthForceComponent: postEnableDisableActions()");

    ForceControllerState::instance().set_azimuthForcesApplied(isEnabled());
}

void AzimuthForceComponent::postUpdateActions() {
    SPDLOG_TRACE("AzimuthForceController: postUpdateActions()");

    auto& faa_settings = ForceActuatorApplicationSettings::instance();

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedAzimuthForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclipped_azimuth_forces.timestamp = _appliedAzimuthForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

        _forceSetpointWarning->azimuthForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().AzimuthLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().AzimuthLimitXTable[xIndex].HighFault;
            _preclipped_azimuth_forces.xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclipped_azimuth_forces.xForces[xIndex],
                                                  _appliedAzimuthForces->xForces[xIndex]);
            _forceSetpointWarning->azimuthForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->azimuthForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().AzimuthLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().AzimuthLimitYTable[yIndex].HighFault;
            _preclipped_azimuth_forces.yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclipped_azimuth_forces.yForces[yIndex],
                                                  _appliedAzimuthForces->yForces[yIndex]);
            _forceSetpointWarning->azimuthForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->azimuthForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().AzimuthLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().AzimuthLimitZTable[zIndex].HighFault;
        _preclipped_azimuth_forces.zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclipped_azimuth_forces.zForces[zIndex],
                                         _appliedAzimuthForces->zForces[zIndex]);
        _forceSetpointWarning->azimuthForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->azimuthForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->azimuthForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _appliedAzimuthForces->xForces, _appliedAzimuthForces->yForces, _appliedAzimuthForces->zForces);
    _appliedAzimuthForces->fx = fm.Fx;
    _appliedAzimuthForces->fy = fm.Fy;
    _appliedAzimuthForces->fz = fm.Fz;
    _appliedAzimuthForces->mx = fm.Mx;
    _appliedAzimuthForces->my = fm.My;
    _appliedAzimuthForces->mz = fm.Mz;
    _appliedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyAzimuthForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        _preclipped_azimuth_forces.calculate_forces_and_moments();
        _preclipped_azimuth_forces.check_changes();
    }
    M1M3SSPublisher::instance().logAppliedAzimuthForces();
}
