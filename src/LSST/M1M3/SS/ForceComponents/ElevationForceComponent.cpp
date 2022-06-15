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

#include <ElevationForceComponent.h>
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

ElevationForceComponent::ElevationForceComponent(
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Elevation", forceActuatorSettings->ElevationComponentSettings) {
    _safetyController = Model::get().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedElevationForces = M1M3SSPublisher::get().getEventAppliedElevationForces();
    _preclippedElevationForces = M1M3SSPublisher::get().getEventPreclippedElevationForces();
}

void ElevationForceComponent::applyElevationForces(float* x, float* y, float* z) {
    SPDLOG_TRACE("ElevationForceComponent: applyElevationForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "ElevationForceComponent: applyElevationForces() called when the component is not "
                "applied");
        return;
    }

    float supportRatio = _forceActuatorState->supportPercentage / 100.0;

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            xTarget[i] = x[i] * supportRatio;
        }

        if (i < FA_Y_COUNT) {
            yTarget[i] = y[i] * supportRatio;
        }

        zTarget[i] = z[i] * supportRatio;
    }
}

void ElevationForceComponent::applyElevationForcesByElevationAngle(float elevationAngle) {
    SPDLOG_TRACE("ElevationForceComponent: applyElevationForcesByMirrorForces({:.1f})", elevationAngle);
    DistributedForces forces =
            ForceConverter::calculateForceFromElevationAngle(_forceActuatorSettings, elevationAngle);
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
    applyElevationForces(xForces, yForces, zForces);
}

void ElevationForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("ElevationForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->elevationForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void ElevationForceComponent::postUpdateActions() {
    SPDLOG_TRACE("ElevationForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedElevationForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedElevationForces->timestamp = _appliedElevationForces->timestamp;
    for (int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->elevationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->ElevationLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->ElevationLimitXTable[xIndex].HighFault;
            _preclippedElevationForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclippedElevationForces->xForces[xIndex],
                                                  _appliedElevationForces->xForces + xIndex);
            _forceSetpointWarning->elevationForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->elevationForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->ElevationLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->ElevationLimitYTable[yIndex].HighFault;
            _preclippedElevationForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclippedElevationForces->yForces[yIndex],
                                                  _appliedElevationForces->yForces + yIndex);
            _forceSetpointWarning->elevationForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->elevationForceWarning[zIndex];
        }

        float zLowFault = _forceActuatorSettings->ElevationLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ElevationLimitZTable[zIndex].HighFault;
        _preclippedElevationForces->zForces[zIndex] = zCurrent[zIndex];

        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedElevationForces->zForces[zIndex],
                                         _appliedElevationForces->zForces + zIndex);
        _forceSetpointWarning->elevationForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->elevationForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->elevationForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedElevationForces->xForces,
            _appliedElevationForces->yForces, _appliedElevationForces->zForces);
    _appliedElevationForces->fx = fm.Fx;
    _appliedElevationForces->fy = fm.Fy;
    _appliedElevationForces->fz = fm.Fz;
    _appliedElevationForces->mx = fm.Mx;
    _appliedElevationForces->my = fm.My;
    _appliedElevationForces->mz = fm.Mz;
    _appliedElevationForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _preclippedElevationForces->xForces,
            _preclippedElevationForces->yForces, _preclippedElevationForces->zForces);
    _preclippedElevationForces->fx = fm.Fx;
    _preclippedElevationForces->fy = fm.Fy;
    _preclippedElevationForces->fz = fm.Fz;
    _preclippedElevationForces->mx = fm.Mx;
    _preclippedElevationForces->my = fm.My;
    _preclippedElevationForces->mz = fm.Mz;
    _preclippedElevationForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyElevationForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedElevationForces();
    }
    M1M3SSPublisher::get().logAppliedElevationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
