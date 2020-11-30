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

StaticForceComponent::StaticForceComponent(SafetyController* safetyController,
                                           ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                           ForceActuatorSettings* forceActuatorSettings)
        : ForceComponent("Static", forceActuatorSettings->StaticComponentSettings) {
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedStaticForces = M1M3SSPublisher::get().getEventAppliedStaticForces();
    _preclippedStaticForces = M1M3SSPublisher::get().getEventPreclippedStaticForces();
}

void StaticForceComponent::applyStaticForces(std::vector<float>* x, std::vector<float>* y,
                                             std::vector<float>* z) {
    SPDLOG_DEBUG("StaticForceComponent: applyStaticForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR("StaticForceComponent: applyStaticForces() called when the component is not applied");
        return;
    }

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            xTarget[i] = (*x)[i];
        }

        if (i < FA_Y_COUNT) {
            yTarget[i] = (*y)[i];
        }

        zTarget[i] = (*z)[i];
    }
}  // namespace SS

void StaticForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("StaticForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->staticForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void StaticForceComponent::postUpdateActions() {
    SPDLOG_TRACE("StaticForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedStaticForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedStaticForces->timestamp = _appliedStaticForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->staticForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
            _preclippedStaticForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclippedStaticForces->xForces[xIndex],
                                             _appliedStaticForces->xForces + xIndex);
            _forceSetpointWarning->staticForceWarning[zIndex] |= notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
            _preclippedStaticForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclippedStaticForces->yForces[yIndex],
                                             _appliedStaticForces->yForces + yIndex);
            _forceSetpointWarning->staticForceWarning[zIndex] |= notInRange;
        }

        float zLowFault = _forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
        _preclippedStaticForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedStaticForces->zForces[zIndex],
                                              _appliedStaticForces->zForces + zIndex);
        _forceSetpointWarning->staticForceWarning[zIndex] |= notInRange;
        clippingRequired |= _forceSetpointWarning->staticForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedStaticForces->xForces,
            _appliedStaticForces->yForces, _appliedStaticForces->zForces);
    _appliedStaticForces->fx = fm.Fx;
    _appliedStaticForces->fy = fm.Fy;
    _appliedStaticForces->fz = fm.Fz;
    _appliedStaticForces->mx = fm.Mx;
    _appliedStaticForces->my = fm.My;
    _appliedStaticForces->mz = fm.Mz;
    _appliedStaticForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _preclippedStaticForces->xForces,
            _preclippedStaticForces->yForces, _preclippedStaticForces->zForces);
    _preclippedStaticForces->fx = fm.Fx;
    _preclippedStaticForces->fy = fm.Fy;
    _preclippedStaticForces->fz = fm.Fz;
    _preclippedStaticForces->mx = fm.Mx;
    _preclippedStaticForces->my = fm.My;
    _preclippedStaticForces->mz = fm.Mz;
    _preclippedStaticForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyStaticForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedStaticForces();
    }
    M1M3SSPublisher::get().logAppliedStaticForces();
}

}  // namespace SS
}  // namespace M1M3
} /* namespace LSST */
