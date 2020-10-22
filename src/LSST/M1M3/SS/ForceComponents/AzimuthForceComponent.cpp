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

#include <AzimuthForceComponent.h>
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

AzimuthForceComponent::AzimuthForceComponent(
        SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Azimuth", forceActuatorSettings->AzimuthComponentSettings) {
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedAzimuthForces = M1M3SSPublisher::get().getEventAppliedAzimuthForces();
    _preclippedAzimuthForces = M1M3SSPublisher::get().getEventPreclippedAzimuthForces();
}

void AzimuthForceComponent::applyAzimuthForces(float* x, float* y, float* z) {
    spdlog::trace("AzimuthForceComponent: applyAzimuthForces()");

    if (!isEnabled()) {
        spdlog::error("AzimuthForceComponent: applyAzimuthForces() called when the component is not applied");
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
    spdlog::trace("AzimuthForceComponent: applyAzimuthForcesByMirrorForces({:.1f})", azimuthAngle);
    DistributedForces forces =
            ForceConverter::calculateForceFromAzimuthAngle(_forceActuatorSettings, azimuthAngle);
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
    spdlog::debug("AzimuthForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->azimuthForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void AzimuthForceComponent::postUpdateActions() {
    spdlog::trace("AzimuthForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedAzimuthForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedAzimuthForces->timestamp = _appliedAzimuthForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->azimuthForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->AzimuthLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->AzimuthLimitXTable[xIndex].HighFault;
            _preclippedAzimuthForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclippedAzimuthForces->xForces[xIndex],
                                             _appliedAzimuthForces->xForces + xIndex);
            _forceSetpointWarning->azimuthForceWarning[zIndex] |= notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->AzimuthLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->AzimuthLimitYTable[yIndex].HighFault;
            _preclippedAzimuthForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclippedAzimuthForces->yForces[yIndex],
                                             _appliedAzimuthForces->yForces + yIndex);
            _forceSetpointWarning->azimuthForceWarning[zIndex] |= notInRange;
        }

        float zLowFault = _forceActuatorSettings->AzimuthLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->AzimuthLimitZTable[zIndex].HighFault;
        _preclippedAzimuthForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedAzimuthForces->zForces[zIndex],
                                         _appliedAzimuthForces->zForces + zIndex);
        _forceSetpointWarning->azimuthForceWarning[zIndex] |= notInRange;
        clippingRequired |= _forceSetpointWarning->azimuthForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedAzimuthForces->xForces,
            _appliedAzimuthForces->yForces, _appliedAzimuthForces->zForces);
    _appliedAzimuthForces->fx = fm.Fx;
    _appliedAzimuthForces->fy = fm.Fy;
    _appliedAzimuthForces->fz = fm.Fz;
    _appliedAzimuthForces->mx = fm.Mx;
    _appliedAzimuthForces->my = fm.My;
    _appliedAzimuthForces->mz = fm.Mz;
    _appliedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _preclippedAzimuthForces->xForces,
            _preclippedAzimuthForces->yForces, _preclippedAzimuthForces->zForces);
    _preclippedAzimuthForces->fx = fm.Fx;
    _preclippedAzimuthForces->fy = fm.Fy;
    _preclippedAzimuthForces->fz = fm.Fz;
    _preclippedAzimuthForces->mx = fm.Mx;
    _preclippedAzimuthForces->my = fm.My;
    _preclippedAzimuthForces->mz = fm.Mz;
    _preclippedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyAzimuthForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedAzimuthForces();
    }
    M1M3SSPublisher::get().logAppliedAzimuthForces();
}

}  // namespace SS
}  // namespace M1M3
} /* namespace LSST */
