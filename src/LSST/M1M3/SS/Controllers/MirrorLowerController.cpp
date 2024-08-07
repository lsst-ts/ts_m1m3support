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

#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <MirrorLowerController.h>
#include <PositionController.h>
#include <PowerController.h>
#include <RaisingLoweringInfo.h>
#include <SafetyController.h>

using namespace LSST::M1M3::SS;

MirrorLowerController::MirrorLowerController(PositionController *positionController,
                                             ForceController *forceController,
                                             SafetyController *safetyController,
                                             PowerController *powerController) {
    SPDLOG_DEBUG("MirrorLowerController: MirrorLowerController()");
    _positionController = positionController;
    _forceController = forceController;
    _safetyController = safetyController;
    _powerController = powerController;

    _cachedStartTime = 0;
    _remaininingTimedout = 0;
    RaisingLoweringInfo::instance().setTimeTimeout(0);

    _movedToLowerPosition = false;

    _loweringPaused = false;
}

void MirrorLowerController::start() {
    SPDLOG_INFO("MirrorLowerController: startLowerOperation()");
    _safetyController->lowerOperationTimeout(false);
    _positionController->startLower();

    _movedToLowerPosition = false;
    _loweringPaused = false;

    if (_positionController->moveToLowerPosition() == false) {
        throw std::runtime_error("Cannot move to lower position before starting lowering the mirror");
    }

    setStartTimestamp();

    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    RaisingLoweringInfo::instance().fillSupportPercentage();
}

void MirrorLowerController::runLoop() {
    if (_loweringPaused == true) {
        return;
    }
    SPDLOG_TRACE("MirrorLowerController: runLoop() {}",
                 RaisingLoweringInfo::instance().weightSupportedPercent);
    if (_movedToLowerPosition == false) {
        _movedToLowerPosition = _positionController->motionComplete();
        if (_movedToLowerPosition == true) {
            _positionController->enableChaseAll();
        }
    } else if (RaisingLoweringInfo::instance().supportPercentageZeroed() == false) {
        // We are still in the process of transfering the support force from the
        // static supports to the force actuators
        // TODO: Does it matter if the following error is bad when we are trying to
        // lower the mirror?
        if (_positionController->hpRaiseLowerForcesInTolerance(false)) {
            // The forces on the hardpoints are within tolerance, we can continue to
            // transfer the support force from the static supports to the force
            // actuators
            RaisingLoweringInfo::instance().decSupportPercentage();
        }
    }

    RaisingLoweringInfo::instance().sendUpdates();
}

bool MirrorLowerController::checkComplete() {
    return RaisingLoweringInfo::instance().supportPercentageZeroed();
}

void MirrorLowerController::complete() {
    SPDLOG_INFO("MirrorLowerController: complete()");
    // All of the support force has been transfered from the static supports to
    // the force actuators, stop the hardpoints from chasing Transition to the end
    // state (parked or parked engineering) if all of the support force has been
    // transfered from the force actuators to the static supports
    _positionController->disableChaseAll();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->zeroElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    RaisingLoweringInfo::instance().zeroSupportPercentage();
}

bool MirrorLowerController::checkTimeout() {
    if (isnan(RaisingLoweringInfo::instance().timeTimeout)) {
        return false;
    }
    return M1M3SSPublisher::instance().getTimestamp() >= RaisingLoweringInfo::instance().timeTimeout;
}

void MirrorLowerController::timeout() {
    SPDLOG_ERROR("MirrorLowerController: timeout()");
    // TODO: How should the system react if the operation times out?
    //       For now we will assume the worst and fault the systeme
    _safetyController->lowerOperationTimeout(true);
}

void MirrorLowerController::abortRaiseM1M3() {
    SPDLOG_INFO("MirrorLowerController:: abortRaiseM1M3()");
    _safetyController->lowerOperationTimeout(false);
    _positionController->stopMotion();
    _positionController->enableChaseAll();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();

    _movedToLowerPosition = true;

    setStartTimestamp();
}

void MirrorLowerController::pauseM1M3Lowering() {
    _loweringPaused = true;
    _remaininingTimedout -= (M1M3SSPublisher::instance().getTimestamp() - _cachedStartTime);
    RaisingLoweringInfo::instance().setTimeTimeout(NAN);
}

void MirrorLowerController::resumeM1M3Lowering() {
    _loweringPaused = false;
    RaisingLoweringInfo::instance().setTimeTimeout(M1M3SSPublisher::instance().getTimestamp() +
                                                   _remaininingTimedout);
}

void MirrorLowerController::setStartTimestamp() {
    _cachedStartTime = M1M3SSPublisher::instance().getTimestamp();
    _remaininingTimedout = _positionController->getLowerTimeout();
    RaisingLoweringInfo::instance().setTimeTimeout(_cachedStartTime + _remaininingTimedout);
}
