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

#include <ActiveOpticForceComponent.h>
#include <DistributedForces.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForceControllerState.h>
#include <ForcesAndMoments.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <Range.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveOpticForceComponent::ActiveOpticForceComponent(
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings)
        : ForceComponent("ActiveOptic", ForceActuatorSettings::instance().ActiveOpticComponentSettings) {
    _safetyController = Model::get().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedActiveOpticForces = M1M3SSPublisher::instance().getEventAppliedActiveOpticForces();
    _preclippedActiveOpticForces = M1M3SSPublisher::instance().getEventPreclippedActiveOpticForces();
}

void ActiveOpticForceComponent::applyActiveOpticForces(float* z) {
    SPDLOG_DEBUG("ActiveOpticForceComponent: applyActiveOpticForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "ActiveOpticForceComponent: applyActiveOpticForces() called when the component is not "
                "applied");
        return;
    }

    for (int i = 0; i < FA_Z_COUNT; ++i) {
        zTarget[i] = z[i];
    }
}

void ActiveOpticForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("ActiveOpticForceComponent: postEnableDisableActions()");

    ForceControllerState::instance().set_activeOpticForcesApplied(isEnabled());
}

void ActiveOpticForceComponent::postUpdateActions() {
    SPDLOG_TRACE("ActiveOpticForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedActiveOpticForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclippedActiveOpticForces->timestamp = _appliedActiveOpticForces->timestamp;
    for (int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
        float zLowFault = ForceActuatorSettings::instance().ActiveOpticLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().ActiveOpticLimitZTable[zIndex].HighFault;

        _forceSetpointWarning->activeOpticForceWarning[zIndex] = false;

        _preclippedActiveOpticForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedActiveOpticForces->zForces[zIndex],
                                         _appliedActiveOpticForces->zForces + zIndex);
        _forceSetpointWarning->activeOpticForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->activeOpticForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->activeOpticForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _appliedActiveOpticForces->zForces);
    _appliedActiveOpticForces->fz = fm.Fz;
    _appliedActiveOpticForces->mx = fm.Mx;
    _appliedActiveOpticForces->my = fm.My;

    fm = ForceActuatorSettings::instance().calculateForcesAndMoments(_forceActuatorApplicationSettings,
                                                                     _preclippedActiveOpticForces->zForces);
    _preclippedActiveOpticForces->fz = fm.Fz;
    _preclippedActiveOpticForces->mx = fm.Mx;
    _preclippedActiveOpticForces->my = fm.My;

    _forceSetpointWarning->activeOpticNetForceWarning =
            !Range::InRange(-ForceActuatorSettings::instance().netActiveOpticForceTolerance,
                            ForceActuatorSettings::instance().netActiveOpticForceTolerance,
                            _appliedActiveOpticForces->fz) ||
            !Range::InRange(-ForceActuatorSettings::instance().netActiveOpticForceTolerance,
                            ForceActuatorSettings::instance().netActiveOpticForceTolerance,
                            _appliedActiveOpticForces->mx) ||
            !Range::InRange(-ForceActuatorSettings::instance().netActiveOpticForceTolerance,
                            ForceActuatorSettings::instance().netActiveOpticForceTolerance,
                            _appliedActiveOpticForces->my);

    _safetyController->forceControllerNotifyActiveOpticForceClipping(clippingRequired);
    _safetyController->forceControllerNotifyActiveOpticNetForceCheck(
            _forceSetpointWarning->activeOpticNetForceWarning);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::instance().logPreclippedActiveOpticForces();
    }
    M1M3SSPublisher::instance().logAppliedActiveOpticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
