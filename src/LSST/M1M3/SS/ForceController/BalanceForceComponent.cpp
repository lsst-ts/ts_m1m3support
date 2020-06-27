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

#include <BalanceForceComponent.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <PIDSettings.h>
#include <Range.h>
#include <ForcesAndMoments.h>
#include <ForceConverter.h>
#include <DistributedForces.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

BalanceForceComponent::BalanceForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings)
        : _fx(0, pidSettings->Fx, publisher),
          _fy(1, pidSettings->Fy, publisher),
          _fz(2, pidSettings->Fz, publisher),
          _mx(3, pidSettings->Mx, publisher),
          _my(4, pidSettings->My, publisher),
          _mz(5, pidSettings->Mz, publisher) {
    name = "Balance";

    _publisher = publisher;
    _safetyController = safetyController;
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _pidSettings = pidSettings;
    _forceActuatorState = _publisher->getEventForceActuatorState();
    _forceSetpointWarning = _publisher->getEventForceSetpointWarning();
    _appliedBalanceForces = _publisher->getEventAppliedBalanceForces();
    _rejectedBalanceForces = _publisher->getEventRejectedBalanceForces();
    maxRateOfChange = _forceActuatorSettings->BalanceComponentSettings.MaxRateOfChange;
    nearZeroValue = _forceActuatorSettings->BalanceComponentSettings.NearZeroValue;
}

void BalanceForceComponent::applyBalanceForces(float* x, float* y, float* z) {
    spdlog::trace("BalanceForceComponent: applyBalanceForces()");
    if (!enabled) {
        spdlog::error("BalanceForceComponent: applyBalanceForces() called when the component is not applied");
        return;
    }
    if (disabling) {
        spdlog::warn("BalanceForceComponent: applyBalanceForces() called when the component is disabling");
        return;
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            xTarget[i] = x[i];
        }

        if (i < 100) {
            yTarget[i] = y[i];
        }

        zTarget[i] = z[i];
    }
}

void BalanceForceComponent::applyBalanceForcesByMirrorForces(float xForce, float yForce, float zForce,
                                                             float xMoment, float yMoment, float zMoment) {
    spdlog::trace(
            "BalanceForceComponent: applyBalanceForcesByMirrorForces({:.1f}, {:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    float fx = _fx.process(0, xForce);
    float fy = _fy.process(0, yForce);
    float fz = _fz.process(0, zForce);
    float mx = _mx.process(0, xMoment);
    float my = _my.process(0, yMoment);
    float mz = _mz.process(0, zMoment);
    // Note: Publishing from any PID will publish ALL PID data
    _fx.publishTelemetry();
    DistributedForces forces =
            ForceConverter::calculateForceDistribution(_forceActuatorSettings, fx, fy, fz, mx, my, mz);
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
    applyBalanceForces(xForces, yForces, zForces);
}

void BalanceForceComponent::updatePID(int id, PIDParameters parameters) {
    spdlog::debug("BalanceForceComponent: updatePID()");
    PID* pid = _idToPID(id);
    if (pid != 0) {
        pid->updateParameters(parameters);
    }
}

void BalanceForceComponent::resetPID(int id) {
    spdlog::debug("BalanceForceComponent: resetPID()");
    PID* pid = _idToPID(id);
    if (pid != 0) {
        pid->restoreInitialParameters();
    }
}

void BalanceForceComponent::resetPIDs() {
    spdlog::debug("BalanceForceComponent: resetPIDs()");
    _fx.restoreInitialParameters();
    _fy.restoreInitialParameters();
    _fz.restoreInitialParameters();
    _mx.restoreInitialParameters();
    _my.restoreInitialParameters();
    _mz.restoreInitialParameters();
}

void BalanceForceComponent::postEnableDisableActions() {
    spdlog::debug("BalanceForceComponent: postEnableDisableActions()");

    if (enabled) {
        resetPIDs();
    }

    _forceActuatorState->timestamp = _publisher->getTimestamp();
    _forceActuatorState->balanceForcesApplied = enabled;
    _publisher->tryLogForceActuatorState();
}

void BalanceForceComponent::postUpdateActions() {
    spdlog::trace("BalanceForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    _appliedBalanceForces->timestamp = _publisher->getTimestamp();
    _rejectedBalanceForces->timestamp = _appliedBalanceForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->balanceForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->BalanceLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->BalanceLimitXTable[xIndex].HighFault;
            _rejectedBalanceForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _rejectedBalanceForces->xForces[xIndex],
                                                  _appliedBalanceForces->xForces + xIndex);
            _forceSetpointWarning->balanceForceWarning[zIndex] =
                    _forceSetpointWarning->balanceForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->BalanceLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->BalanceLimitYTable[yIndex].HighFault;
            _rejectedBalanceForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _rejectedBalanceForces->yForces[yIndex],
                                                  _appliedBalanceForces->yForces + yIndex);
            _forceSetpointWarning->balanceForceWarning[zIndex] =
                    _forceSetpointWarning->balanceForceWarning[zIndex] || notInRange;
        }

        float zLowFault = _forceActuatorSettings->BalanceLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->BalanceLimitZTable[zIndex].HighFault;
        _rejectedBalanceForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _rejectedBalanceForces->zForces[zIndex],
                                         _appliedBalanceForces->zForces + zIndex);
        _forceSetpointWarning->balanceForceWarning[zIndex] =
                _forceSetpointWarning->balanceForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || _forceSetpointWarning->balanceForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings,
            _appliedBalanceForces->xForces, _appliedBalanceForces->yForces,
            _appliedBalanceForces->zForces);
    _appliedBalanceForces->fx = fm.Fx;
    _appliedBalanceForces->fy = fm.Fy;
    _appliedBalanceForces->fz = fm.Fz;
    _appliedBalanceForces->mx = fm.Mx;
    _appliedBalanceForces->my = fm.My;
    _appliedBalanceForces->mz = fm.Mz;
    _appliedBalanceForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings,
            _rejectedBalanceForces->xForces, _rejectedBalanceForces->yForces,
            _rejectedBalanceForces->zForces);
    _rejectedBalanceForces->fx = fm.Fx;
    _rejectedBalanceForces->fy = fm.Fy;
    _rejectedBalanceForces->fz = fm.Fz;
    _rejectedBalanceForces->mx = fm.Mx;
    _rejectedBalanceForces->my = fm.My;
    _rejectedBalanceForces->mz = fm.Mz;
    _rejectedBalanceForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyBalanceForceClipping(rejectionRequired);

    _publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        _publisher->logRejectedBalanceForces();
    }
    _publisher->logAppliedBalanceForces();
}

PID* BalanceForceComponent::_idToPID(int id) {
    switch (id) {
        case 1:
            return &_fx;
        case 2:
            return &_fy;
        case 3:
            return &_fz;
        case 4:
            return &_mx;
        case 5:
            return &_my;
        case 6:
            return &_mz;
        default:
            return 0;
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
