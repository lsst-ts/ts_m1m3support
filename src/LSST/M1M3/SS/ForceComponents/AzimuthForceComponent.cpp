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

namespace LSST {
namespace M1M3 {
namespace SS {

AzimuthForceComponent::AzimuthForceComponent(
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings)
        : ForceComponent("Azimuth", &ForceActuatorSettings::instance().AzimuthComponentSettings) {
    _safetyController = Model::instance().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedAzimuthForces = M1M3SSPublisher::instance().getAppliedAzimuthForces();
    _preclippedAzimuthForces = M1M3SSPublisher::instance().getEventPreclippedAzimuthForces();
}

void AzimuthForceComponent::applyAzimuthForces(float* x, float* y, float* z) {
    SPDLOG_TRACE("AzimuthForceComponent: applyAzimuthForces()");

    if (!isEnabled()) {
        using namespace std::chrono_literals;
        TG_LOG_ERROR(5s,
                     "AzimuthForceComponent: applyAzimuthForces() called when the component is not applied");
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
    applyAzimuthForces(xForces, yForces, zForces);
}

void AzimuthForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("AzimuthForceComponent: postEnableDisableActions()");

    ForceControllerState::instance().set_azimuthForcesApplied(isEnabled());
}

void AzimuthForceComponent::postUpdateActions() {
    SPDLOG_TRACE("AzimuthForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedAzimuthForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclippedAzimuthForces->timestamp = _appliedAzimuthForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->azimuthForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().AzimuthLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().AzimuthLimitXTable[xIndex].HighFault;
            _preclippedAzimuthForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclippedAzimuthForces->xForces[xIndex],
                                             _appliedAzimuthForces->xForces + xIndex);
            _forceSetpointWarning->azimuthForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->azimuthForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().AzimuthLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().AzimuthLimitYTable[yIndex].HighFault;
            _preclippedAzimuthForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclippedAzimuthForces->yForces[yIndex],
                                             _appliedAzimuthForces->yForces + yIndex);
            _forceSetpointWarning->azimuthForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->azimuthForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().AzimuthLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().AzimuthLimitZTable[zIndex].HighFault;
        _preclippedAzimuthForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedAzimuthForces->zForces[zIndex],
                                         _appliedAzimuthForces->zForces + zIndex);
        _forceSetpointWarning->azimuthForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->azimuthForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->azimuthForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _appliedAzimuthForces->xForces, _appliedAzimuthForces->yForces,
            _appliedAzimuthForces->zForces);
    _appliedAzimuthForces->fx = fm.Fx;
    _appliedAzimuthForces->fy = fm.Fy;
    _appliedAzimuthForces->fz = fm.Fz;
    _appliedAzimuthForces->mx = fm.Mx;
    _appliedAzimuthForces->my = fm.My;
    _appliedAzimuthForces->mz = fm.Mz;
    _appliedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _preclippedAzimuthForces->xForces,
            _preclippedAzimuthForces->yForces, _preclippedAzimuthForces->zForces);
    _preclippedAzimuthForces->fx = fm.Fx;
    _preclippedAzimuthForces->fy = fm.Fy;
    _preclippedAzimuthForces->fz = fm.Fz;
    _preclippedAzimuthForces->mx = fm.Mx;
    _preclippedAzimuthForces->my = fm.My;
    _preclippedAzimuthForces->mz = fm.Mz;
    _preclippedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyAzimuthForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::instance().logPreclippedAzimuthForces();
    }
    M1M3SSPublisher::instance().logAppliedAzimuthForces();
}

}  // namespace SS
}  // namespace M1M3
} /* namespace LSST */
