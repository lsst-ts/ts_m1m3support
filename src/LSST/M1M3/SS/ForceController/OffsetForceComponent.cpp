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

    _publisher = publisher;
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = _publisher->getEventForceActuatorState();
    _forceSetpointWarning = _publisher->getEventForceSetpointWarning();
    _appliedOffsetForces = _publisher->getEventAppliedOffsetForces();
    _rejectedOffsetForces = _publisher->getEventRejectedOffsetForces();
    this->maxRateOfChange = _forceActuatorSettings->OffsetComponentSettings.MaxRateOfChange;
    this->nearZeroValue = _forceActuatorSettings->OffsetComponentSettings.NearZeroValue;
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
            _forceActuatorSettings, xForce, yForce, zForce, xMoment, yMoment, zMoment);
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
    this->applyOffsetForces(xForces, yForces, zForces);
}

void OffsetForceComponent::postEnableDisableActions() {
    spdlog::debug("OffsetForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = _publisher->getTimestamp();
    _forceActuatorState->offsetForcesApplied = this->enabled;
    _publisher->tryLogForceActuatorState();
}

void OffsetForceComponent::postUpdateActions() {
    spdlog::trace("OffsetForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedOffsetForces->timestamp = _publisher->getTimestamp();
    _rejectedOffsetForces->timestamp = _appliedOffsetForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->offsetForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->OffsetLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->OffsetLimitXTable[xIndex].HighFault;
            _rejectedOffsetForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _rejectedOffsetForces->xForces[xIndex],
                                             _appliedOffsetForces->xForces + xIndex);
            _forceSetpointWarning->offsetForceWarning[zIndex] =
                    _forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->OffsetLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->OffsetLimitYTable[yIndex].HighFault;
            _rejectedOffsetForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _rejectedOffsetForces->yForces[yIndex],
                                             _appliedOffsetForces->yForces + yIndex);
            _forceSetpointWarning->offsetForceWarning[zIndex] =
                    _forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->OffsetLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->OffsetLimitZTable[zIndex].HighFault;
        _rejectedOffsetForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedOffsetForces->zForces[zIndex],
                                              _appliedOffsetForces->zForces + zIndex);
        _forceSetpointWarning->offsetForceWarning[zIndex] =
                _forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->offsetForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedOffsetForces->xForces,
            _appliedOffsetForces->yForces, _appliedOffsetForces->zForces);
    _appliedOffsetForces->fx = fm.Fx;
    _appliedOffsetForces->fy = fm.Fy;
    _appliedOffsetForces->fz = fm.Fz;
    _appliedOffsetForces->mx = fm.Mx;
    _appliedOffsetForces->my = fm.My;
    _appliedOffsetForces->mz = fm.Mz;
    _appliedOffsetForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _rejectedOffsetForces->xForces,
            _rejectedOffsetForces->yForces, _rejectedOffsetForces->zForces);
    _rejectedOffsetForces->fx = fm.Fx;
    _rejectedOffsetForces->fy = fm.Fy;
    _rejectedOffsetForces->fz = fm.Fz;
    _rejectedOffsetForces->mx = fm.Mx;
    _rejectedOffsetForces->my = fm.My;
    _rejectedOffsetForces->mz = fm.Mz;
    _rejectedOffsetForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyOffsetForceClipping(rejectionRequired);

    _publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        _publisher->logRejectedOffsetForces();
    }
    _publisher->logAppliedOffsetForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
