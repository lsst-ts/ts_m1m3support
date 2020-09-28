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
    _rejectedStaticForces = M1M3SSPublisher::get().getEventRejectedStaticForces();
}

void StaticForceComponent::applyStaticForces(std::vector<float>* x, std::vector<float>* y,
                                             std::vector<float>* z) {
    spdlog::debug("StaticForceComponent: applyStaticForces()");

    if (!isEnabled()) {
        spdlog::error("StaticForceComponent: applyStaticForces() called when the component is not applied");
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
    spdlog::debug("StaticForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->staticForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void StaticForceComponent::postUpdateActions() {
    spdlog::trace("StaticForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedStaticForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _rejectedStaticForces->timestamp = _appliedStaticForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->staticForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
            _rejectedStaticForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _rejectedStaticForces->xForces[xIndex],
                                             _appliedStaticForces->xForces + xIndex);
            _forceSetpointWarning->staticForceWarning[zIndex] =
                    _forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
            _rejectedStaticForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _rejectedStaticForces->yForces[yIndex],
                                             _appliedStaticForces->yForces + yIndex);
            _forceSetpointWarning->staticForceWarning[zIndex] =
                    _forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
        _rejectedStaticForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedStaticForces->zForces[zIndex],
                                              _appliedStaticForces->zForces + zIndex);
        _forceSetpointWarning->staticForceWarning[zIndex] =
                _forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->staticForceWarning[zIndex];
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
            _forceActuatorApplicationSettings, _forceActuatorSettings, _rejectedStaticForces->xForces,
            _rejectedStaticForces->yForces, _rejectedStaticForces->zForces);
    _rejectedStaticForces->fx = fm.Fx;
    _rejectedStaticForces->fy = fm.Fy;
    _rejectedStaticForces->fz = fm.Fz;
    _rejectedStaticForces->mx = fm.Mx;
    _rejectedStaticForces->my = fm.My;
    _rejectedStaticForces->mz = fm.Mz;
    _rejectedStaticForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyStaticForceClipping(rejectionRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (rejectionRequired) {
        M1M3SSPublisher::get().logRejectedStaticForces();
    }
    M1M3SSPublisher::get().logAppliedStaticForces();
}

}  // namespace SS
}  // namespace M1M3
} /* namespace LSST */
