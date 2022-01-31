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
#include <Model.h>
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
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings)
        : ForceComponent("Balance", forceActuatorSettings->BalanceComponentSettings),
          _fx(0, pidSettings->getParameters(0)),
          _fy(1, pidSettings->getParameters(1)),
          _fz(2, pidSettings->getParameters(2)),
          _mx(3, pidSettings->getParameters(3)),
          _my(4, pidSettings->getParameters(4)),
          _mz(5, pidSettings->getParameters(5)) {
    _safetyController = Model::get().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _pidSettings = pidSettings;
    _forceActuatorState = M1M3SSPublisher::get().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::get().getEventForceSetpointWarning();
    _appliedBalanceForces = M1M3SSPublisher::get().getEventAppliedBalanceForces();
    _preclippedBalanceForces = M1M3SSPublisher::get().getEventPreclippedBalanceForces();
}

void BalanceForceComponent::applyBalanceForces(float* x, float* y, float* z) {
    SPDLOG_TRACE("BalanceForceComponent: applyBalanceForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR("BalanceForceComponent: applyBalanceForces() called when the component is not applied");
        return;
    }

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            xTarget[i] = x[i];
        }

        if (i < FA_Y_COUNT) {
            yTarget[i] = y[i];
        }

        zTarget[i] = z[i];
    }
}

void BalanceForceComponent::applyBalanceForcesByMirrorForces(float xForce, float yForce, float zForce,
                                                             float xMoment, float yMoment, float zMoment) {
    SPDLOG_TRACE(
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
    float xForces[FA_X_COUNT];
    float yForces[FA_Y_COUNT];
    float zForces[FA_Z_COUNT];
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
    SPDLOG_DEBUG("BalanceForceComponent: updatePID()");
    PID* pid = _idToPID(id);
    if (pid != NULL) {
        pid->updateParameters(parameters);
    }
}

void BalanceForceComponent::resetPID(int id) {
    SPDLOG_DEBUG("BalanceForceComponent: resetPID()");
    PID* pid = _idToPID(id);
    if (pid != NULL) {
        pid->restoreInitialParameters();
    }
}

void BalanceForceComponent::resetPIDs() {
    SPDLOG_DEBUG("BalanceForceComponent: resetPIDs()");
    _fx.restoreInitialParameters();
    _fy.restoreInitialParameters();
    _fz.restoreInitialParameters();
    _mx.restoreInitialParameters();
    _my.restoreInitialParameters();
    _mz.restoreInitialParameters();
}

void BalanceForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("BalanceForceComponent: postEnableDisableActions()");

    if (isEnabled()) {
        resetPIDs();
    }

    _forceActuatorState->timestamp = M1M3SSPublisher::get().getTimestamp();
    _forceActuatorState->balanceForcesApplied = isEnabled();
    M1M3SSPublisher::get().tryLogForceActuatorState();
}

void BalanceForceComponent::postUpdateActions() {
    SPDLOG_TRACE("BalanceForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedBalanceForces->timestamp = M1M3SSPublisher::get().getTimestamp();
    _preclippedBalanceForces->timestamp = _appliedBalanceForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->balanceForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = _forceActuatorSettings->BalanceLimitXTable[xIndex].LowFault;
            float xHighFault = _forceActuatorSettings->BalanceLimitXTable[xIndex].HighFault;
            _preclippedBalanceForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclippedBalanceForces->xForces[xIndex],
                                             _appliedBalanceForces->xForces + xIndex);
            _forceSetpointWarning->balanceForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->balanceForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = _forceActuatorSettings->BalanceLimitYTable[yIndex].LowFault;
            float yHighFault = _forceActuatorSettings->BalanceLimitYTable[yIndex].HighFault;
            _preclippedBalanceForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclippedBalanceForces->yForces[yIndex],
                                             _appliedBalanceForces->yForces + yIndex);
            _forceSetpointWarning->balanceForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->balanceForceWarning[zIndex];
        }

        float zLowFault = _forceActuatorSettings->BalanceLimitZTable[zIndex].LowFault;
        float zHighFault = _forceActuatorSettings->BalanceLimitZTable[zIndex].HighFault;
        _preclippedBalanceForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedBalanceForces->zForces[zIndex],
                                         _appliedBalanceForces->zForces + zIndex);
        _forceSetpointWarning->balanceForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->balanceForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->balanceForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _appliedBalanceForces->xForces,
            _appliedBalanceForces->yForces, _appliedBalanceForces->zForces);
    _appliedBalanceForces->fx = fm.Fx;
    _appliedBalanceForces->fy = fm.Fy;
    _appliedBalanceForces->fz = fm.Fz;
    _appliedBalanceForces->mx = fm.Mx;
    _appliedBalanceForces->my = fm.My;
    _appliedBalanceForces->mz = fm.Mz;
    _appliedBalanceForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _preclippedBalanceForces->xForces,
            _preclippedBalanceForces->yForces, _preclippedBalanceForces->zForces);
    _preclippedBalanceForces->fx = fm.Fx;
    _preclippedBalanceForces->fy = fm.Fy;
    _preclippedBalanceForces->fz = fm.Fz;
    _preclippedBalanceForces->mx = fm.Mx;
    _preclippedBalanceForces->my = fm.My;
    _preclippedBalanceForces->mz = fm.Mz;
    _preclippedBalanceForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyBalanceForceClipping(clippingRequired);

    M1M3SSPublisher::get().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::get().logPreclippedBalanceForces();
    }
    M1M3SSPublisher::get().logAppliedBalanceForces();
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
