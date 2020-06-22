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

#include <StaticForceComponent.h>
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

StaticForceComponent::StaticForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                                           ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                           ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Static";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedStaticForces = this->publisher->getEventAppliedStaticForces();
    this->rejectedStaticForces = this->publisher->getEventRejectedStaticForces();
    this->maxRateOfChange = this->forceActuatorSettings->StaticComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->StaticComponentSettings.NearZeroValue;
}

void StaticForceComponent::applyStaticForces(std::vector<float>* x, std::vector<float>* y,
                                             std::vector<float>* z) {
    spdlog::debug("StaticForceComponent: applyStaticForces()");
    if (!this->enabled) {
        spdlog::error("StaticForceComponent: applyStaticForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("StaticForceComponent: applyStaticForces() called when the component is disabling");
        return;
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] = (*x)[i];
        }

        if (i < 100) {
            this->yTarget[i] = (*y)[i];
        }

        this->zTarget[i] = (*z)[i];
    }
}

void StaticForceComponent::postEnableDisableActions() {
    spdlog::debug("StaticForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->staticForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void StaticForceComponent::postUpdateActions() {
    spdlog::trace("StaticForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedStaticForces->timestamp = this->publisher->getTimestamp();
    this->rejectedStaticForces->timestamp = this->appliedStaticForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->staticForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
            this->rejectedStaticForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedStaticForces->xForces[xIndex],
                                                  this->appliedStaticForces->xForces + xIndex);
            this->forceSetpointWarning->staticForceWarning[zIndex] =
                    this->forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
            this->rejectedStaticForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedStaticForces->yForces[yIndex],
                                                  this->appliedStaticForces->yForces + yIndex);
            this->forceSetpointWarning->staticForceWarning[zIndex] =
                    this->forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        }

        float zLowFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
        this->rejectedStaticForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedStaticForces->zForces[zIndex],
                                         this->appliedStaticForces->zForces + zIndex);
        this->forceSetpointWarning->staticForceWarning[zIndex] =
                this->forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->staticForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->appliedStaticForces->xForces, this->appliedStaticForces->yForces,
            this->appliedStaticForces->zForces);
    this->appliedStaticForces->fx = fm.Fx;
    this->appliedStaticForces->fy = fm.Fy;
    this->appliedStaticForces->fz = fm.Fz;
    this->appliedStaticForces->mx = fm.Mx;
    this->appliedStaticForces->my = fm.My;
    this->appliedStaticForces->mz = fm.Mz;
    this->appliedStaticForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->rejectedStaticForces->xForces, this->rejectedStaticForces->yForces,
            this->rejectedStaticForces->zForces);
    this->rejectedStaticForces->fx = fm.Fx;
    this->rejectedStaticForces->fy = fm.Fy;
    this->rejectedStaticForces->fz = fm.Fz;
    this->rejectedStaticForces->mx = fm.Mx;
    this->rejectedStaticForces->my = fm.My;
    this->rejectedStaticForces->mz = fm.Mz;
    this->rejectedStaticForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyStaticForceClipping(rejectionRequired);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedStaticForces();
    }
    this->publisher->logAppliedStaticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
