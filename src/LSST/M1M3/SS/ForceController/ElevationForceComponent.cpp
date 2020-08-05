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
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Elevation";

    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    this->forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    this->appliedElevationForces = M1M3SSPublisher::get().getEventAppliedElevationForces();
    this->rejectedElevationForces = M1M3SSPublisher::get().getEventRejectedElevationForces();
    this->maxRateOfChange = _forceActuatorSettings->ElevationComponentSettings.MaxRateOfChange;
    this->nearZeroValue = _forceActuatorSettings->ElevationComponentSettings.NearZeroValue;
}

void ElevationForceComponent::applyElevationForces(float* x, float* y, float* z) {
    spdlog::trace("ElevationForceComponent: applyElevationForces()");
    if (!this->enabled) {
        spdlog::error(
                "ElevationForceComponent: applyElevationForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn(
                "ElevationForceComponent: applyElevationForces() called when the component is disabling");
        return;
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] = x[i] * this->forceActuatorState->supportPercentage;
        }

        if (i < 100) {
            this->yTarget[i] = y[i] * this->forceActuatorState->supportPercentage;
        }

        this->zTarget[i] = z[i] * this->forceActuatorState->supportPercentage;
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
    this->applyElevationForces(xForces, yForces, zForces);
}

void ElevationForceComponent::_postEnableDisableActions() {
    spdlog::debug("ElevationForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    this->forceActuatorState->elevationForcesApplied = this->enabled;
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void ElevationForceComponent::_postUpdateActions() {
    spdlog::trace("ElevationForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedElevationForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    this->rejectedElevationForces->timestamp = this->appliedElevationForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->elevationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->ElevationLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->ElevationLimitXTable[xIndex].HighFault;
            this->rejectedElevationForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedElevationForces->xForces[xIndex],
                                                  this->appliedElevationForces->xForces + xIndex);
            this->forceSetpointWarning->elevationForceWarning[zIndex] =
                    this->forceSetpointWarning->elevationForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->ElevationLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->ElevationLimitYTable[yIndex].HighFault;
            this->rejectedElevationForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedElevationForces->yForces[yIndex],
                                                  this->appliedElevationForces->yForces + yIndex);
            this->forceSetpointWarning->elevationForceWarning[zIndex] =
                    this->forceSetpointWarning->elevationForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->ElevationLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ElevationLimitZTable[zIndex].HighFault;
        this->rejectedElevationForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault,
                                              this->rejectedElevationForces->zForces[zIndex],
                                              this->appliedElevationForces->zForces + zIndex);
        this->forceSetpointWarning->elevationForceWarning[zIndex] =
                this->forceSetpointWarning->elevationForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->elevationForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, this->appliedElevationForces->xForces,
            this->appliedElevationForces->yForces, this->appliedElevationForces->zForces);
    this->appliedElevationForces->fx = fm.Fx;
    this->appliedElevationForces->fy = fm.Fy;
    this->appliedElevationForces->fz = fm.Fz;
    this->appliedElevationForces->mx = fm.Mx;
    this->appliedElevationForces->my = fm.My;
    this->appliedElevationForces->mz = fm.Mz;
    this->appliedElevationForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, this->rejectedElevationForces->xForces,
            this->rejectedElevationForces->yForces, this->rejectedElevationForces->zForces);
    this->rejectedElevationForces->fx = fm.Fx;
    this->rejectedElevationForces->fy = fm.Fy;
    this->rejectedElevationForces->fz = fm.Fz;
    this->rejectedElevationForces->mx = fm.Mx;
    this->rejectedElevationForces->my = fm.My;
    this->rejectedElevationForces->mz = fm.Mz;
    this->rejectedElevationForces->forceMagnitude = fm.ForceMagnitude;

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
