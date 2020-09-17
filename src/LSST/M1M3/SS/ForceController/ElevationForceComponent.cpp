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

#include <ElevationForceComponent.h>
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

ElevationForceComponent::ElevationForceComponent(
        SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Elevation", forceActuatorSettings->ElevationComponentSettings) {
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedElevationForces = M1M3SSPublisher::get().getEventAppliedElevationForces();
    _rejectedElevationForces = M1M3SSPublisher::get().getEventRejectedElevationForces();
}

void ElevationForceComponent::applyElevationForces(float* x, float* y, float* z) {
    spdlog::trace("ElevationForceComponent: applyElevationForces()");

    if (!isEnabled()) {
        spdlog::error(
                "ElevationForceComponent: applyElevationForces() called when the component is not "
                "applied");
        return;
    }

    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            xTarget[i] = x[i] * _forceActuatorState->supportPercentage;
        }

        if (i < 100) {
            yTarget[i] = y[i] * _forceActuatorState->supportPercentage;
        }

        zTarget[i] = z[i] * _forceActuatorState->supportPercentage;
    }
}

void ElevationForceComponent::applyElevationForcesByElevationAngle(float elevationAngle) {
    spdlog::trace("ElevationForceComponent: applyElevationForcesByMirrorForces({:.1f})", elevationAngle);
    DistributedForces forces =
            ForceConverter::calculateForceFromElevationAngle(_forceActuatorSettings, elevationAngle);
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
    applyElevationForces(xForces, yForces, zForces);
}

void ElevationForceComponent::postEnableDisableActions() {
    spdlog::debug("ElevationForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->elevationForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void ElevationForceComponent::postUpdateActions() {
    spdlog::trace("ElevationForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedElevationForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _rejectedElevationForces->timestamp = _appliedElevationForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->elevationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->ElevationLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->ElevationLimitXTable[xIndex].HighFault;
            _rejectedElevationForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _rejectedElevationForces->xForces[xIndex],
                                             _appliedElevationForces->xForces + xIndex);
            _forceSetpointWarning->elevationForceWarning[zIndex] =
                    _forceSetpointWarning->elevationForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->ElevationLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->ElevationLimitYTable[yIndex].HighFault;
            _rejectedElevationForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _rejectedElevationForces->yForces[yIndex],
                                             _appliedElevationForces->yForces + yIndex);
            _forceSetpointWarning->elevationForceWarning[zIndex] =
                    _forceSetpointWarning->elevationForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->ElevationLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ElevationLimitZTable[zIndex].HighFault;
        _rejectedElevationForces->zForces[zIndex] = zCurrent[zIndex];

        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedElevationForces->zForces[zIndex],
                                         _appliedElevationForces->zForces + zIndex);
        _forceSetpointWarning->elevationForceWarning[zIndex] =
                _forceSetpointWarning->elevationForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->elevationForceWarning[zIndex];
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
            _forceActuatorApplicationSettings, _forceActuatorSettings, _rejectedElevationForces->xForces,
            _rejectedElevationForces->yForces, _rejectedElevationForces->zForces);
    _rejectedElevationForces->fx = fm.Fx;
    _rejectedElevationForces->fy = fm.Fy;
    _rejectedElevationForces->fz = fm.Fz;
    _rejectedElevationForces->mx = fm.Mx;
    _rejectedElevationForces->my = fm.My;
    _rejectedElevationForces->mz = fm.Mz;
    _rejectedElevationForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyElevationForceClipping(rejectionRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (rejectionRequired) {
        M1M3SSPublisher::get().logRejectedElevationForces();
    }
    M1M3SSPublisher::get().logAppliedElevationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
