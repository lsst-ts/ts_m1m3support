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

#include <ActiveOpticForceComponent.h>
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

ActiveOpticForceComponent::ActiveOpticForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "ActiveOptic";

    _publisher = publisher;
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = _publisher->getEventForceActuatorState();
    _forceSetpointWarning = _publisher->getEventForceSetpointWarning();
    _appliedActiveOpticForces = _publisher->getEventAppliedActiveOpticForces();
    _rejectedActiveOpticForces = _publisher->getEventRejectedActiveOpticForces();
    maxRateOfChange = _forceActuatorSettings->ActiveOpticComponentSettings.MaxRateOfChange;
    nearZeroValue = _forceActuatorSettings->ActiveOpticComponentSettings.NearZeroValue;
}

void ActiveOpticForceComponent::applyActiveOpticForces(float* z) {
    spdlog::debug("ActiveOpticForceComponent: applyActiveOpticForces()");
    if (!this->enabled) {
        spdlog::error(
                "ActiveOpticForceComponent: applyActiveOpticForces() called when the component is not "
                "applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn(
                "ActiveOpticForceComponent: applyActiveOpticForces() called when the component is disabling");
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        this->zTarget[i] = z[i];
    }
}

void ActiveOpticForceComponent::applyActiveOpticForcesByBendingModes(float* coefficients) {
    spdlog::debug("ActiveOpticForceComponent: applyActiveOpticForcesByBendingModes()");
    DistributedForces forces =
            ForceConverter::calculateForceFromBendingModes(_forceActuatorSettings, coefficients);
    this->applyActiveOpticForces(forces.ZForces);
}

void ActiveOpticForceComponent::postEnableDisableActions() {
    spdlog::debug("ActiveOpticForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = _publisher->getTimestamp();
    _forceActuatorState->activeOpticForcesApplied = this->enabled;
    _publisher->tryLogForceActuatorState();
}

void ActiveOpticForceComponent::postUpdateActions() {
    spdlog::trace("ActiveOpticForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedActiveOpticForces->timestamp = _publisher->getTimestamp();
    _rejectedActiveOpticForces->timestamp = _appliedActiveOpticForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        float zLowFault = _forceActuatorSettings->ActiveOpticLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ActiveOpticLimitZTable[zIndex].HighFault;

        _forceSetpointWarning->activeOpticForceWarning[zIndex] = false;

        _rejectedActiveOpticForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedActiveOpticForces->zForces[zIndex],
                                         _appliedActiveOpticForces->zForces + zIndex);
        _forceSetpointWarning->activeOpticForceWarning[zIndex] =
                _forceSetpointWarning->activeOpticForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->activeOpticForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedActiveOpticForces->zForces);
    _appliedActiveOpticForces->fz = fm.Fz;
    _appliedActiveOpticForces->mx = fm.Mx;
    _appliedActiveOpticForces->my = fm.My;

    fm = ForceConverter::calculateForcesAndMoments(_forceActuatorApplicationSettings, _forceActuatorSettings,
                                                   _rejectedActiveOpticForces->zForces);
    _rejectedActiveOpticForces->fz = fm.Fz;
    _rejectedActiveOpticForces->mx = fm.Mx;
    _rejectedActiveOpticForces->my = fm.My;

    _forceSetpointWarning->activeOpticNetForceWarning =
            !Range::InRange(-_forceActuatorSettings->NetActiveOpticForceTolerance,
                            _forceActuatorSettings->NetActiveOpticForceTolerance,
                            _appliedActiveOpticForces->fz) ||
            !Range::InRange(-_forceActuatorSettings->NetActiveOpticForceTolerance,
                            _forceActuatorSettings->NetActiveOpticForceTolerance,
                            _appliedActiveOpticForces->mx) ||
            !Range::InRange(-_forceActuatorSettings->NetActiveOpticForceTolerance,
                            _forceActuatorSettings->NetActiveOpticForceTolerance,
                            _appliedActiveOpticForces->my);

    _safetyController->forceControllerNotifyActiveOpticForceClipping(rejectionRequired);
    _safetyController->forceControllerNotifyActiveOpticNetForceCheck(
            _forceSetpointWarning->activeOpticNetForceWarning);

    _publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        _publisher->logRejectedActiveOpticForces();
    }
    _publisher->logAppliedActiveOpticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
