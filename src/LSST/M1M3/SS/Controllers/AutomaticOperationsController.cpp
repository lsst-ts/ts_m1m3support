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

#include <AutomaticOperationsController.h>
#include <PositionController.h>
#include <ForceController.h>
#include <SafetyController.h>
#include <M1M3SSPublisher.h>
#include <PowerController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AutomaticOperationsController::AutomaticOperationsController(PositionController* positionController,
                                                             ForceController* forceController,
                                                             SafetyController* safetyController,
                                                             PowerController* powerController) {
    SPDLOG_DEBUG("AutomaticOperationsController: AutomaticOperationsController()");
    _positionController = positionController;
    _forceController = forceController;
    _safetyController = safetyController;
    _powerController = powerController;
    _cachedTimestamp = 0;
    _bypassMoveToReference = false;
}

void AutomaticOperationsController::startRaiseOperation(bool bypassMoveToReference) {
    SPDLOG_INFO("AutomaticOperationsController: startRaiseOperation({})", bypassMoveToReference);
    _bypassMoveToReference = bypassMoveToReference;
    _safetyController->raiseOperationTimeout(false);
    _positionController->stopMotion();
    _positionController->enableChaseAll();
    _forceController->zeroAberrationForces();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    _forceController->zeroSupportPercentage();
    _cachedTimestamp = M1M3SSPublisher::get().getTimestamp();
}

void AutomaticOperationsController::tryIncrementingSupportPercentage() {
    SPDLOG_TRACE("AutomaticOperationsController: tryIncrementingSupportPercentage()");
    if (!_forceController->supportPercentageFilled()) {
        // We are still in the process of transfering the support force from the static supports
        // to the force actuators
        if (_positionController->forcesInTolerance() && _forceController->followingErrorInTolerance()) {
            // The forces on the hardpoints are within tolerance and
            // the force actuators are following their setpoints, we can continue to transfer the
            // support force from the static supports to the force actuators
            _forceController->incSupportPercentage();
            if (_forceController->supportPercentageFilled()) {
                // All of the support force has been transfered from the static supports to the
                // force actuators, stop the hardpoints from chasing and start moving to the
                // reference position
                _positionController->disableChaseAll();
                if (!_bypassMoveToReference) {
                    _positionController->moveToReferencePosition();
                }
            }
        }
    }
}

bool AutomaticOperationsController::checkRaiseOperationComplete() {
    return _forceController->supportPercentageFilled() && _positionController->motionComplete();
}

void AutomaticOperationsController::completeRaiseOperation() {
    SPDLOG_INFO("AutomaticOperationsController: completeRaiseOperation()");
    // Transition to the end state (active or active engineering) if all of the support force has been
    // transfered from the static supports to the force actuators and all hardpoints have completed their
    // commanded motions
    _forceController->zeroAberrationForces();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->applyAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->applyStaticForces();
    _forceController->applyThermalForces();
    _forceController->zeroVelocityForces();
    _forceController->fillSupportPercentage();
}

bool AutomaticOperationsController::checkRaiseOperationTimeout() {
    return M1M3SSPublisher::get().getTimestamp() >=
           (_cachedTimestamp + _positionController->getRaiseLowerTimeout());
}

void AutomaticOperationsController::timeoutRaiseOperation() {
    SPDLOG_ERROR("AutomaticOperationsController: timeoutRaiseOperation()");
    _safetyController->raiseOperationTimeout(true);
}

void AutomaticOperationsController::abortRaiseM1M3() {
    SPDLOG_INFO("AutomaticOperationsController:: abortRaiseM1M3()");
    _safetyController->lowerOperationTimeout(false);
    _positionController->stopMotion();
    _positionController->enableChaseAll();
    _forceController->zeroAberrationForces();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    _cachedTimestamp = M1M3SSPublisher::get().getTimestamp();
}

void AutomaticOperationsController::startLowerOperation() {
    SPDLOG_INFO("AutomaticOperationsController: startLowerOperation()");
    _safetyController->lowerOperationTimeout(false);
    _positionController->stopMotion();
    _positionController->enableChaseAll();
    _forceController->zeroAberrationForces();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    _forceController->fillSupportPercentage();
    _cachedTimestamp = M1M3SSPublisher::get().getTimestamp();
}

void AutomaticOperationsController::tryDecrementSupportPercentage() {
    SPDLOG_TRACE("AutomaticOperationsController: tryDecrementSupportPercentage()");
    if (!_forceController->supportPercentageZeroed()) {
        // We are still in the process of transfering the support force from the static supports
        // to the force actuators
        // TODO: Does it matter if the following error is bad when we are trying to lower the mirror?
        if (_positionController->forcesInTolerance()) {
            // The forces on the hardpoints are within tolerance, we can continue to transfer the
            // support force from the static supports to the force actuators
            _forceController->decSupportPercentage();
        }
    }
}

bool AutomaticOperationsController::checkLowerOperationComplete() {
    return _forceController->supportPercentageZeroed();
}

void AutomaticOperationsController::completeLowerOperation() {
    SPDLOG_INFO("AutomaticOperationsController: completeLowerOperation()");
    // All of the support force has been transfered from the static supports to the
    // force actuators, stop the hardpoints from chasing
    // Transition to the end state (parked or parked engineering) if all of the support
    // force has been transfered from the force actuators to the static supports
    _positionController->disableChaseAll();
    _forceController->zeroAberrationForces();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->zeroElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    _forceController->zeroSupportPercentage();
}

bool AutomaticOperationsController::checkLowerOperationTimeout() {
    return M1M3SSPublisher::get().getTimestamp() >=
           (_cachedTimestamp + _positionController->getRaiseLowerTimeout());
}

void AutomaticOperationsController::timeoutLowerOperation() {
    SPDLOG_ERROR("AutomaticOperationsController: timeoutLowerOperation()");
    // TODO: How should the system react if the operation times out?
    //       For now we will assume the worst and fault the system
    _safetyController->lowerOperationTimeout(true);
}

void AutomaticOperationsController::uncontrolledLowerOperation() {
    _powerController->setAllAuxPowerNetworks(false);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
