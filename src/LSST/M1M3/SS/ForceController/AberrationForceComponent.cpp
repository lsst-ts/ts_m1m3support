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
        SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Aberration") {
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedAberrationForces = M1M3SSPublisher::get().getEventAppliedAberrationForces();
    _rejectedAberrationForces = M1M3SSPublisher::get().getEventRejectedAberrationForces();
    maxRateOfChange = _forceActuatorSettings->AberrationComponentSettings.MaxRateOfChange;
    nearZeroValue = _forceActuatorSettings->AberrationComponentSettings.NearZeroValue;
}

void AberrationForceComponent::applyAberrationForces(float* z) {
    spdlog::debug("AberrationForceComponent: applyAberrationForces()");
    if (!enabled) {
        spdlog::error(
                "AberrationForceComponent: applyAberrationForces() called when the component is not applied");
        return;
    }
    if (disabling) {
        spdlog::warn(
                "AberrationForceComponent: applyAberrationForces() called when the component is disabling");
        enable();
    }
    for (int i = 0; i < 156; ++i) {
        zTarget[i] = z[i];
    }
}

void AberrationForceComponent::applyAberrationForcesByBendingModes(float* coefficients) {
    spdlog::debug("AberrationForceComponent: applyAberrationForcesByBendingModes()");
    DistributedForces forces =
            ForceConverter::calculateForceFromBendingModes(_forceActuatorSettings, coefficients);
    applyAberrationForces(forces.ZForces);
}

void AberrationForceComponent::postEnableDisableActions() {
    spdlog::debug("AberrationForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->aberrationForcesApplied = enabled;
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void AberrationForceComponent::postUpdateActions() {
    spdlog::trace("AberrationForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedAberrationForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _rejectedAberrationForces->timestamp = _appliedAberrationForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        float zLowFault = _forceActuatorSettings->AberrationLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->AberrationLimitZTable[zIndex].HighFault;

        _forceSetpointWarning->aberrationForceWarning[zIndex] = false;

        _rejectedAberrationForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedAberrationForces->zForces[zIndex],
                                         _appliedAberrationForces->zForces + zIndex);
        _forceSetpointWarning->aberrationForceWarning[zIndex] =
                _forceSetpointWarning->aberrationForceWarning[zIndex] || notInRange;

        rejectionRequired = rejectionRequired || _forceSetpointWarning->aberrationForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedAberrationForces->zForces);
    _appliedAberrationForces->fz = fm.Fz;
    _appliedAberrationForces->mx = fm.Mx;
    _appliedAberrationForces->my = fm.My;

    fm = ForceConverter::calculateForcesAndMoments(_forceActuatorApplicationSettings, _forceActuatorSettings,
                                                   _rejectedAberrationForces->zForces);
    _rejectedAberrationForces->fz = fm.Fz;
    _rejectedAberrationForces->mx = fm.Mx;
    _rejectedAberrationForces->my = fm.My;

    _forceSetpointWarning->aberrationNetForceWarning =
            !Range::InRange(-_forceActuatorSettings->NetAberrationForceTolerance,
                            _forceActuatorSettings->NetAberrationForceTolerance,
                            _appliedAberrationForces->fz) ||
            !Range::InRange(-_forceActuatorSettings->NetAberrationForceTolerance,
                            _forceActuatorSettings->NetAberrationForceTolerance,
                            _appliedAberrationForces->mx) ||
            !Range::InRange(-_forceActuatorSettings->NetAberrationForceTolerance,
                            _forceActuatorSettings->NetAberrationForceTolerance,
                            _appliedAberrationForces->my);

    _safetyController->forceControllerNotifyAberrationForceClipping(rejectionRequired);
    _safetyController->forceControllerNotifyAberrationNetForceCheck(
            _forceSetpointWarning->aberrationNetForceWarning);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (rejectionRequired) {
        M1M3SSPublisher::get().logRejectedAberrationForces();
    }
    M1M3SSPublisher::get().logAppliedAberrationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
