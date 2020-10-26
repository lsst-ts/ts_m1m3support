/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Telescope & Site Software
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

#include <BumpTestController.h>
#include <ForceController.h>
#include <Model.h>
#include <SettingReader.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

BumpTestController::BumpTestController() {
    spdlog::debug("BumpTestController: BumpTestController()");
    _zIndex = -1;
}

void BumpTestController::setBumpTestActuator(int actuatorId, bool testPrimary, bool testSecondary) {
    _zIndex = SettingReader::get().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(actuatorId);
    _testPrimary = testPrimary;
    _testSecondary = testSecondary;
    _stage = IDLE;
}

States::Type BumpTestController::runLoop() {
    ForceController *forceController = Model::get().getForceController();
    switch (_stage) {
        case IDLE:
            forceController->applyActuatorOffset(_zIndex, 50);
            forceController->processAppliedForces();
            _stage = PLUS;
            break;

        case FINISHED:
            _zIndex = -1;
            return States::ParkedEngineeringState;
    }
    return States::NoStateTransition;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
