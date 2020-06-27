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

#include <ThermalForceComponent.h>
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

ThermalForceComponent::ThermalForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    name = "Thermal";

    _publisher = publisher;
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorState = _publisher->getEventForceActuatorState();
    _forceSetpointWarning = _publisher->getEventForceSetpointWarning();
    _appliedThermalForces = _publisher->getEventAppliedThermalForces();
    _rejectedThermalForces = _publisher->getEventRejectedThermalForces();
    maxRateOfChange = _forceActuatorSettings->ThermalComponentSettings.MaxRateOfChange;
    nearZeroValue = _forceActuatorSettings->ThermalComponentSettings.NearZeroValue;
}

void ThermalForceComponent::applyThermalForces(float* x, float* y, float* z) {
    spdlog::trace("ThermalForceComponent: applyThermalForces()");
    if (!this->enabled) {
        spdlog::error("ThermalForceComponent: applyThermalForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("ThermalForceComponent: applyThermalForces() called when the component is disabling");
        return;
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

void ThermalForceComponent::applyThermalForcesByMirrorTemperature(float temperature) {
    spdlog::trace("ThermalForceComponent: applyThermalForcesByMirrorForces({:.1f})", temperature);
    DistributedForces forces =
            ForceConverter::calculateForceFromTemperature(_forceActuatorSettings, temperature);
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
    this->applyThermalForces(xForces, yForces, zForces);
}

void ThermalForceComponent::postEnableDisableActions() {
    spdlog::debug("ThermalForceComponent: postEnableDisableActions()");

    _forceActuatorState->timestamp = _publisher->getTimestamp();
    _forceActuatorState->thermalForcesApplied = this->enabled;
    _publisher->tryLogForceActuatorState();
}

void ThermalForceComponent::postUpdateActions() {
    spdlog::trace("ThermalForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedThermalForces->timestamp = _publisher->getTimestamp();
    _rejectedThermalForces->timestamp = _appliedThermalForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->thermalForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->ThermalLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->ThermalLimitXTable[xIndex].HighFault;
            _rejectedThermalForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _rejectedThermalForces->xForces[xIndex],
                                                  _appliedThermalForces->xForces + xIndex);
            _forceSetpointWarning->thermalForceWarning[zIndex] =
                    _forceSetpointWarning->thermalForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->ThermalLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->ThermalLimitYTable[yIndex].HighFault;
            _rejectedThermalForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _rejectedThermalForces->yForces[yIndex],
                                                  _appliedThermalForces->yForces + yIndex);
            _forceSetpointWarning->thermalForceWarning[zIndex] =
                    _forceSetpointWarning->thermalForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->ThermalLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->ThermalLimitZTable[zIndex].HighFault;
        _rejectedThermalForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedThermalForces->zForces[zIndex],
                                         _appliedThermalForces->zForces + zIndex);
        _forceSetpointWarning->thermalForceWarning[zIndex] =
                _forceSetpointWarning->thermalForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->thermalForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings,
            _appliedThermalForces->xForces, _appliedThermalForces->yForces,
            _appliedThermalForces->zForces);
    _appliedThermalForces->fx = fm.Fx;
    _appliedThermalForces->fy = fm.Fy;
    _appliedThermalForces->fz = fm.Fz;
    _appliedThermalForces->mx = fm.Mx;
    _appliedThermalForces->my = fm.My;
    _appliedThermalForces->mz = fm.Mz;
    _appliedThermalForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings,
            _rejectedThermalForces->xForces, _rejectedThermalForces->yForces,
            _rejectedThermalForces->zForces);
    _rejectedThermalForces->fx = fm.Fx;
    _rejectedThermalForces->fy = fm.Fy;
    _rejectedThermalForces->fz = fm.Fz;
    _rejectedThermalForces->mx = fm.Mx;
    _rejectedThermalForces->my = fm.My;
    _rejectedThermalForces->mz = fm.Mz;
    _rejectedThermalForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyThermalForceClipping(rejectionRequired);

    _publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        _publisher->logRejectedThermalForces();
    }
    _publisher->logAppliedThermalForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
