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

    _publisher = publisher;
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = _publisher->getEventForceActuatorState();
    _forceSetpointWarning = _publisher->getEventForceSetpointWarning();
    _appliedStaticForces = _publisher->getEventAppliedStaticForces();
    _rejectedStaticForces = _publisher->getEventRejectedStaticForces();
    this->maxRateOfChange = _forceActuatorSettings->StaticComponentSettings.MaxRateOfChange;
    this->nearZeroValue = _forceActuatorSettings->StaticComponentSettings.NearZeroValue;
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

    _forceActuatorState->timestamp = _publisher->getTimestamp();
    _forceActuatorState->staticForcesApplied = this->enabled;
    _publisher->tryLogForceActuatorState();
}

void StaticForceComponent::postUpdateActions() {
    spdlog::trace("StaticForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedStaticForces->timestamp = _publisher->getTimestamp();
    _rejectedStaticForces->timestamp = _appliedStaticForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->staticForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
            _rejectedStaticForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _rejectedStaticForces->xForces[xIndex],
                                             _appliedStaticForces->xForces + xIndex);
            _forceSetpointWarning->staticForceWarning[zIndex] =
                    _forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
            _rejectedStaticForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _rejectedStaticForces->yForces[yIndex],
                                             _appliedStaticForces->yForces + yIndex);
            _forceSetpointWarning->staticForceWarning[zIndex] =
                    _forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
        _rejectedStaticForces->zForces[zIndex] = this->zCurrent[zIndex];
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

    _publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        _publisher->logRejectedStaticForces();
    }
    _publisher->logAppliedStaticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
