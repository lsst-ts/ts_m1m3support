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

#include <AberrationForceComponent.h>
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

AberrationForceComponent::AberrationForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Aberration";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedAberrationForces = this->publisher->getEventAppliedAberrationForces();
    this->rejectedAberrationForces = this->publisher->getEventRejectedAberrationForces();
    this->maxRateOfChange = this->forceActuatorSettings->AberrationComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->AberrationComponentSettings.NearZeroValue;
}

void AberrationForceComponent::applyAberrationForces(float* z) {
    spdlog::debug("AberrationForceComponent: applyAberrationForces()");
    if (!this->enabled) {
        spdlog::error(
                "AberrationForceComponent: applyAberrationForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn(
                "AberrationForceComponent: applyAberrationForces() called when the component is disabling");
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        this->zTarget[i] = z[i];
    }
}

void AberrationForceComponent::applyAberrationForcesByBendingModes(float* coefficients) {
    spdlog::debug("AberrationForceComponent: applyAberrationForcesByBendingModes()");
    DistributedForces forces =
            ForceConverter::calculateForceFromBendingModes(this->forceActuatorSettings, coefficients);
    this->applyAberrationForces(forces.ZForces);
}

void AberrationForceComponent::postEnableDisableActions() {
    spdlog::debug("AberrationForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->aberrationForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void AberrationForceComponent::postUpdateActions() {
    spdlog::trace("AberrationForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedAberrationForces->timestamp = this->publisher->getTimestamp();
    this->rejectedAberrationForces->timestamp = this->appliedAberrationForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        float zLowFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].HighFault;

        this->forceSetpointWarning->aberrationForceWarning[zIndex] = false;

        this->rejectedAberrationForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault,
                                              this->rejectedAberrationForces->zForces[zIndex],
                                              this->appliedAberrationForces->zForces + zIndex);
        this->forceSetpointWarning->aberrationForceWarning[zIndex] =
                this->forceSetpointWarning->aberrationForceWarning[zIndex] || notInRange;

        rejectionRequired = rejectionRequired || this->forceSetpointWarning->aberrationForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings,
                                                                    this->forceActuatorSettings,
                                                                    this->appliedAberrationForces->zForces);
    this->appliedAberrationForces->fz = fm.Fz;
    this->appliedAberrationForces->mx = fm.Mx;
    this->appliedAberrationForces->my = fm.My;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings,
                                                   this->forceActuatorSettings,
                                                   this->rejectedAberrationForces->zForces);
    this->rejectedAberrationForces->fz = fm.Fz;
    this->rejectedAberrationForces->mx = fm.Mx;
    this->rejectedAberrationForces->my = fm.My;

    this->forceSetpointWarning->aberrationNetForceWarning =
            !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance,
                            this->forceActuatorSettings->NetAberrationForceTolerance,
                            this->appliedAberrationForces->fz) ||
            !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance,
                            this->forceActuatorSettings->NetAberrationForceTolerance,
                            this->appliedAberrationForces->mx) ||
            !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance,
                            this->forceActuatorSettings->NetAberrationForceTolerance,
                            this->appliedAberrationForces->my);

    this->safetyController->forceControllerNotifyAberrationForceClipping(rejectionRequired);
    this->safetyController->forceControllerNotifyAberrationNetForceCheck(
            this->forceSetpointWarning->aberrationNetForceWarning);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedAberrationForces();
    }
    this->publisher->logAppliedAberrationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
