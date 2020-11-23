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

#include <MirrorLowerController.h>
#include <PositionController.h>
#include <ForceController.h>
#include <SafetyController.h>
#include <M1M3SSPublisher.h>
#include <PowerController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

MirrorLowerController::MirrorLowerController(PositionController* positionController,
                                             ForceController* forceController,
                                             SafetyController* safetyController,
                                             PowerController* powerController) {
    spdlog::debug("MirrorLowerController: MirrorLowerController()");
    _positionController = positionController;
    _forceController = forceController;
    _safetyController = safetyController;
    _powerController = powerController;
    _cachedTimestamp = 0;
}

void MirrorLowerController::start() {
    spdlog::info("MirrorLowerController: startLowerOperation()");
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

void MirrorLowerController::runLoop() {
    spdlog::trace("MirrorLowerController: runLoop()");
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

bool MirrorLowerController::checkComplete() { return _forceController->supportPercentageZeroed(); }

void MirrorLowerController::complete() {
    spdlog::info("MirrorLowerController: complete()");
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

bool MirrorLowerController::checkTimeout() {
    return M1M3SSPublisher::get().getTimestamp() >=
           (_cachedTimestamp + _positionController->getRaiseLowerTimeout());
}

void MirrorLowerController::timeout() {
    spdlog::error("MirrorLowerController: timeout()");
    // TODO: How should the system react if the operation times out?
    //       For now we will assume the worst and fault the systeme
    _safetyController->lowerOperationTimeout(true);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
