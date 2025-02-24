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

#include <AirSupplyStatus.h>
#include <ForceController.h>
#include <HardpointActuatorWarning.h>
#include <M1M3SSPublisher.h>
#include <MirrorRaiseController.h>
#include <PositionController.h>
#include <PowerController.h>
#include <RaisingLoweringInfo.h>
#include <SafetyController.h>

using namespace LSST::M1M3::SS;

MirrorRaiseController::MirrorRaiseController(PositionController *positionController,
                                             ForceController *forceController,
                                             SafetyController *safetyController,
                                             PowerController *powerController) {
    SPDLOG_DEBUG("MirrorRaiseController: MirrorRaiseController()");
    _positionController = positionController;
    _forceController = forceController;
    _safetyController = safetyController;
    _powerController = powerController;
    _cachedStartTime = 0;
    _remaininingTimedout = 0;
    RaisingLoweringInfo::instance().setTimeTimeout(0);
    _bypassMoveToReference = false;
    _lastForceFilled = false;
    _lastPositionCompleted = false;
    _raisingPaused = false;
}

void MirrorRaiseController::start(bool bypassMoveToReference) {
    SPDLOG_INFO("MirrorRaiseController: start({})", bypassMoveToReference);
    _bypassMoveToReference = bypassMoveToReference;
    _lastForceFilled = false;
    _lastPositionCompleted = false;
    _raisePauseReported = false;

    _raisingPaused = false;

    _safetyController->raiseOperationTimeout(false);
    _positionController->startRaise();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    RaisingLoweringInfo::instance().zeroSupportPercentage();
    _cachedStartTime = M1M3SSPublisher::instance().getTimestamp();
    _remaininingTimedout = _positionController->getRaiseTimeout();
    RaisingLoweringInfo::instance().setTimeTimeout(_cachedStartTime + _remaininingTimedout);

    RaisingLoweringInfo::instance().setWaitAirPressure(false);

    if (AirSupplyStatus::instance().airValveClosed == true) {
        SPDLOG_WARN(
                "Air valve is closed and the mirror was commanded to raise. "
                "Please check that the compressed "
                "air is provided to the M1M3 support system");
    }
}

void MirrorRaiseController::runLoop() {
    if (_raisingPaused == true) {
        return;
    }
    SPDLOG_TRACE("MirrorRaiseController: runLoop() {}",
                 RaisingLoweringInfo::instance().weightSupportedPercent);
    auto raiseInfo = &RaisingLoweringInfo::instance();
    if (!raiseInfo->supportPercentageFilled()) {
        auto hpWarning = &HardpointActuatorWarning::instance();
        // Wait for pressure to raise after valve opening
        if (raiseInfo->supportPercentageZeroed() && hpWarning->anyLowAirPressureFault) {
            raiseInfo->setWaitAirPressure(true);
            // hpRaiseLowerForcesInTolerance needs to be called to stop encoder
            // movement if on tension limit
            _positionController->hpRaiseLowerForcesInTolerance(true);
            return;
        }
        raiseInfo->setWaitAirPressure(false);
        // We are still in the process of transferring the support force from the
        // static supports to the force actuators
        if (_positionController->hpRaiseLowerForcesInTolerance(true) &&
            _forceController->faRaiseFollowingErrorInTolerance()) {
            // The forces on the hardpoints are within tolerance and
            // the force actuators are following their setpoints, we can continue to
            // transfer the support force from the static supports to the force
            // actuators
            RaisingLoweringInfo::instance().incSupportPercentage();
            if (_raisePauseReported == true) {
                _raisePauseReported = false;
                SPDLOG_INFO("Raising resumed");
            }
            if (RaisingLoweringInfo::instance().supportPercentageFilled()) {
                // All of the support force has been transfered from the static supports
                // to the force actuators, stop the hardpoints from chasing and start
                // moving to the reference position
                _positionController->disableChaseAll();
                if (!_bypassMoveToReference) {
                    _positionController->moveToReferencePosition();
                }
            }
        } else {
            if (_raisePauseReported == false) {
                _raisePauseReported = true;
                SPDLOG_WARN("Raising paused - waiting for hardpoints movements");
            }
        }
        raiseInfo->sendUpdates();
    }
}

bool MirrorRaiseController::checkComplete() {
    bool forceFilled = RaisingLoweringInfo::instance().supportPercentageFilled();
    bool positionCompleted = _positionController->motionComplete();
    if (_lastForceFilled != forceFilled) {
        SPDLOG_INFO(
                "MirrorRaiseController::checkComplete force controller support "
                "percentage {}",
                forceFilled ? "filled" : "not filled");
    }
    if (_lastPositionCompleted != positionCompleted) {
        SPDLOG_INFO("MirrorRaiseController::checkComplete position controller moves {}",
                    positionCompleted ? "completed" : "not completed");
    }

    _lastForceFilled = forceFilled;
    _lastPositionCompleted = positionCompleted;

    return forceFilled && positionCompleted;
}

void MirrorRaiseController::complete() {
    SPDLOG_INFO("MirrorRaiseController: complete()");
    // Transition to the end state (active or active engineering) if all of the
    // support force has been transfered from the static supports to the force
    // actuators and all hardpoints have completed their commanded motions
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->applyAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->applyStaticForces();
    _forceController->applyThermalForces();
    _forceController->zeroVelocityForces();
    if (ForceActuatorSettings::instance().hardpointBalanceForcesOnInActiveState) {
        _forceController->applyBalanceForces();
    }
    RaisingLoweringInfo::instance().fillSupportPercentage();
}

bool MirrorRaiseController::checkTimeout() {
    if (isnan(RaisingLoweringInfo::instance().timeTimeout)) {
        return false;
    }
    return M1M3SSPublisher::instance().getTimestamp() >= RaisingLoweringInfo::instance().timeTimeout;
}

void MirrorRaiseController::timeout() {
    SPDLOG_ERROR("MirrorRaiseController: timeout()");
    _safetyController->raiseOperationTimeout(true);
}

void MirrorRaiseController::pauseM1M3Raising() {
    _raisingPaused = true;
    _remaininingTimedout -= (M1M3SSPublisher::instance().getTimestamp() - _cachedStartTime);
    RaisingLoweringInfo::instance().setTimeTimeout(NAN);
}

void MirrorRaiseController::resumeM1M3Raising() {
    _raisingPaused = false;
    RaisingLoweringInfo::instance().setTimeTimeout(M1M3SSPublisher::instance().getTimestamp() +
                                                   _remaininingTimedout);
}
