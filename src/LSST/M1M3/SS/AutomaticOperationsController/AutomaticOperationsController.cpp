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
                                                             M1M3SSPublisher* publisher,
                                                             PowerController* powerController) {
    spdlog::debug("AutomaticOperationsController: AutomaticOperationsController()");
    this->positionController = positionController;
    this->forceController = forceController;
    this->safetyController = safetyController;
    this->publisher = publisher;
    this->powerController = powerController;
    this->cachedTimestamp = 0;
    this->bypassMoveToReference = false;
}

void AutomaticOperationsController::startRaiseOperation(bool bypassMoveToReference) {
    spdlog::info("AutomaticOperationsController: startRaiseOperation({})", bypassMoveToReference);
    this->bypassMoveToReference = bypassMoveToReference;
    this->safetyController->raiseOperationTimeout(false);
    this->positionController->stopMotion();
    this->positionController->enableChaseAll();
    this->forceController->zeroAberrationForces();
    this->forceController->zeroAccelerationForces();
    this->forceController->zeroActiveOpticForces();
    this->forceController->zeroAzimuthForces();
    this->forceController->zeroBalanceForces();
    this->forceController->applyElevationForces();
    this->forceController->zeroOffsetForces();
    this->forceController->zeroStaticForces();
    this->forceController->zeroThermalForces();
    this->forceController->zeroVelocityForces();
    this->forceController->zeroSupportPercentage();
    this->cachedTimestamp = this->publisher->getTimestamp();
}

void AutomaticOperationsController::tryIncrementingSupportPercentage() {
    spdlog::trace("AutomaticOperationsController: tryIncrementingSupportPercentage()");
    if (!this->forceController->supportPercentageFilled()) {
        // We are still in the process of transfering the support force from the static supports
        // to the force actuators
        if (this->positionController->forcesInTolerance() &&
            this->forceController->followingErrorInTolerance()) {
            // The forces on the hardpoints are within tolerance and
            // the force actuators are following their setpoints, we can continue to transfer the
            // support force from the static supports to the force actuators
            this->forceController->incSupportPercentage();
            if (this->forceController->supportPercentageFilled()) {
                // All of the support force has been transfered from the static supports to the
                // force actuators, stop the hardpoints from chasing and start moving to the
                // reference position
                this->positionController->disableChaseAll();
                if (!this->bypassMoveToReference) {
                    this->positionController->moveToReferencePosition();
                }
            }
        }
    }
}

bool AutomaticOperationsController::checkRaiseOperationComplete() {
    return this->forceController->supportPercentageFilled() && this->positionController->motionComplete();
}

void AutomaticOperationsController::completeRaiseOperation() {
    spdlog::info("AutomaticOperationsController: completeRaiseOperation()");
    // Transition to the end state (active or active engineering) if all of the support force has been
    // transfered from the static supports to the force actuators and all hardpoints have completed their
    // commanded motions
    this->forceController->zeroAberrationForces();
    this->forceController->zeroAccelerationForces();
    this->forceController->zeroActiveOpticForces();
    this->forceController->applyAzimuthForces();
    this->forceController->zeroBalanceForces();
    this->forceController->applyElevationForces();
    this->forceController->zeroOffsetForces();
    this->forceController->applyStaticForces();
    this->forceController->applyThermalForces();
    this->forceController->zeroVelocityForces();
    this->forceController->fillSupportPercentage();
}

bool AutomaticOperationsController::checkRaiseOperationTimeout() {
    return this->publisher->getTimestamp() >=
           (this->cachedTimestamp + this->positionController->getRaiseLowerTimeout());
}

void AutomaticOperationsController::timeoutRaiseOperation() {
    spdlog::error("AutomaticOperationsController: timeoutRaiseOperation()");
    this->safetyController->raiseOperationTimeout(true);
}

void AutomaticOperationsController::abortRaiseM1M3() {
    spdlog::info("AutomaticOperationsController:: abortRaiseM1M3()");
    this->safetyController->lowerOperationTimeout(false);
    this->positionController->stopMotion();
    this->positionController->enableChaseAll();
    this->forceController->zeroAberrationForces();
    this->forceController->zeroAccelerationForces();
    this->forceController->zeroActiveOpticForces();
    this->forceController->zeroAzimuthForces();
    this->forceController->zeroBalanceForces();
    this->forceController->applyElevationForces();
    this->forceController->zeroOffsetForces();
    this->forceController->zeroStaticForces();
    this->forceController->zeroThermalForces();
    this->forceController->zeroVelocityForces();
    this->cachedTimestamp = this->publisher->getTimestamp();
}

void AutomaticOperationsController::startLowerOperation() {
    spdlog::info("AutomaticOperationsController: startLowerOperation()");
    this->safetyController->lowerOperationTimeout(false);
    this->positionController->stopMotion();
    this->positionController->enableChaseAll();
    this->forceController->zeroAberrationForces();
    this->forceController->zeroAccelerationForces();
    this->forceController->zeroActiveOpticForces();
    this->forceController->zeroAzimuthForces();
    this->forceController->zeroBalanceForces();
    this->forceController->applyElevationForces();
    this->forceController->zeroOffsetForces();
    this->forceController->zeroStaticForces();
    this->forceController->zeroThermalForces();
    this->forceController->zeroVelocityForces();
    this->forceController->fillSupportPercentage();
    this->cachedTimestamp = this->publisher->getTimestamp();
}

void AutomaticOperationsController::tryDecrementSupportPercentage() {
    spdlog::trace("AutomaticOperationsController: tryDecrementSupportPercentage()");
    if (!this->forceController->supportPercentageZeroed()) {
        // We are still in the process of transfering the support force from the static supports
        // to the force actuators
        // TODO: Does it matter if the following error is bad when we are trying to lower the mirror?
        if (this->positionController->forcesInTolerance()) {
            // The forces on the hardpoints are within tolerance, we can continue to transfer the
            // support force from the static supports to the force actuators
            this->forceController->decSupportPercentage();
        }
    }
}

bool AutomaticOperationsController::checkLowerOperationComplete() {
    return this->forceController->supportPercentageZeroed();
}

void AutomaticOperationsController::completeLowerOperation() {
    spdlog::info("AutomaticOperationsController: completeLowerOperation()");
    // All of the support force has been transfered from the static supports to the
    // force actuators, stop the hardpoints from chasing
    // Transition to the end state (parked or parked engineering) if all of the support
    // force has been transfered from the force actuators to the static supports
    this->positionController->disableChaseAll();
    this->forceController->zeroAberrationForces();
    this->forceController->zeroAccelerationForces();
    this->forceController->zeroActiveOpticForces();
    this->forceController->zeroAzimuthForces();
    this->forceController->zeroBalanceForces();
    this->forceController->zeroElevationForces();
    this->forceController->zeroOffsetForces();
    this->forceController->zeroStaticForces();
    this->forceController->zeroThermalForces();
    this->forceController->zeroVelocityForces();
    this->forceController->zeroSupportPercentage();
}

bool AutomaticOperationsController::checkLowerOperationTimeout() {
    return this->publisher->getTimestamp() >=
           (this->cachedTimestamp + this->positionController->getRaiseLowerTimeout());
}

void AutomaticOperationsController::timeoutLowerOperation() {
    spdlog::error("AutomaticOperationsController: timeoutLowerOperation()");
    // TODO: How should the system react if the operation times out?
    //       For now we will assume the worst and fault the system
    this->safetyController->lowerOperationTimeout(true);
}

void AutomaticOperationsController::uncontrolledLowerOperation() {
    this->powerController->setAllAuxPowerNetworks(false);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
