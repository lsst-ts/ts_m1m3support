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

#include <OffsetForceComponent.h>
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

OffsetForceComponent::OffsetForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                                           ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                           ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Offset";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedOffsetForces = this->publisher->getEventAppliedOffsetForces();
    this->rejectedOffsetForces = this->publisher->getEventRejectedOffsetForces();
    this->maxRateOfChange = this->forceActuatorSettings->OffsetComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->OffsetComponentSettings.NearZeroValue;
}

void OffsetForceComponent::applyOffsetForces(float* x, float* y, float* z) {
    spdlog::debug("OffsetForceComponent: applyOffsetForces()");
    if (!this->enabled) {
        spdlog::error("OffsetForceComponent: applyOffsetForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("ThermalOffsetComponent: applyOffsetForces() called when the component is disabling");
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] = x[i];
        }

        if (i < 100) {
            this->yTarget[i] = y[i];
        }

        this->zTarget[i] = z[i];
    }
}

void OffsetForceComponent::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce,
                                                           float xMoment, float yMoment, float zMoment) {
    spdlog::debug(
            "OffsetForceComponent: applyOffsetForcesByMirrorForces({:.1f}, {:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    DistributedForces forces = ForceConverter::calculateForceDistribution(
            this->forceActuatorSettings, xForce, yForce, zForce, xMoment, yMoment, zMoment);
    float xForces[12];
    float yForces[100];
    float zForces[156];
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        if (xIndex != -1) {
            xForces[xIndex] = forces.XForces[zIndex];
        }
        if (yIndex != -1) {
            yForces[yIndex] = forces.YForces[zIndex];
        }
        zForces[zIndex] = forces.ZForces[zIndex];
    }
    this->applyOffsetForces(xForces, yForces, zForces);
}

void OffsetForceComponent::postEnableDisableActions() {
    spdlog::debug("OffsetForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->offsetForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void OffsetForceComponent::postUpdateActions() {
    spdlog::trace("OffsetForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedOffsetForces->timestamp = this->publisher->getTimestamp();
    this->rejectedOffsetForces->timestamp = this->appliedOffsetForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->offsetForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].HighFault;
            this->rejectedOffsetForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedOffsetForces->xForces[xIndex],
                                                  this->appliedOffsetForces->xForces + xIndex);
            this->forceSetpointWarning->offsetForceWarning[zIndex] =
                    this->forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].HighFault;
            this->rejectedOffsetForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedOffsetForces->yForces[yIndex],
                                                  this->appliedOffsetForces->yForces + yIndex);
            this->forceSetpointWarning->offsetForceWarning[zIndex] =
                    this->forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
        }

        float zLowFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].HighFault;
        this->rejectedOffsetForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedOffsetForces->zForces[zIndex],
                                         this->appliedOffsetForces->zForces + zIndex);
        this->forceSetpointWarning->offsetForceWarning[zIndex] =
                this->forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->offsetForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->appliedOffsetForces->xForces, this->appliedOffsetForces->yForces,
            this->appliedOffsetForces->zForces);
    this->appliedOffsetForces->fx = fm.Fx;
    this->appliedOffsetForces->fy = fm.Fy;
    this->appliedOffsetForces->fz = fm.Fz;
    this->appliedOffsetForces->mx = fm.Mx;
    this->appliedOffsetForces->my = fm.My;
    this->appliedOffsetForces->mz = fm.Mz;
    this->appliedOffsetForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->rejectedOffsetForces->xForces, this->rejectedOffsetForces->yForces,
            this->rejectedOffsetForces->zForces);
    this->rejectedOffsetForces->fx = fm.Fx;
    this->rejectedOffsetForces->fy = fm.Fy;
    this->rejectedOffsetForces->fz = fm.Fz;
    this->rejectedOffsetForces->mx = fm.Mx;
    this->rejectedOffsetForces->my = fm.My;
    this->rejectedOffsetForces->mz = fm.Mz;
    this->rejectedOffsetForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyOffsetForceClipping(rejectionRequired);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedOffsetForces();
    }
    this->publisher->logAppliedOffsetForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
