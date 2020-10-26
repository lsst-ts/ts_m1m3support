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
#include <Publisher.h>
#include <SettingReader.h>

#include <sal_MTM1M3.h>
#include <spdlog/spdlog.h>

using namespace MTM1M3;

namespace LSST {
namespace M1M3 {
namespace SS {

BumpTestController::BumpTestController() {
    spdlog::debug("BumpTestController: BumpTestController()");
    _zIndex = -1;
}

void BumpTestController::setBumpTestActuator(int actuatorId, bool testPrimary, bool testSecondary) {
    _zIndex = SettingReader::get().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(actuatorId);
    _secondaryIndex = SettingReader::get()
                              .getForceActuatorApplicationSettings()
                              ->ZIndexToSecondaryCylinderIndex[_zIndex];
    _testPrimary = testPrimary;
    _testSecondary = testSecondary;

    _tolerance = 1;

    MTM1M3_logevent_forceActuatorBumpTestStatusC* forceActuatorBumpTestStatus =
            M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus();

    if (testPrimary) {
        forceActuatorBumpTestStatus->primaryTest[_zIndex] = MTM1M3_shared_BumpTest_NotTested;
    }

    if (testSecondary) {
        forceActuatorBumpTestStatus->secondaryTest[_secondaryIndex] = MTM1M3_shared_BumpTest_NotTested;
    }

    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
}

States::Type BumpTestController::runLoop() {
    ForceController* forceController = Model::get().getForceController();
    double timestamp = M1M3SSPublisher::get().getTimestamp();
    // force actuator data are updated only in UpdateCommand; as only a single
    // command can be executed, there isn't a race condition
    MTM1M3_logevent_forceActuatorBumpTestStatusC* forceActuatorBumpTestStatus =
            M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus();
    MTM1M3_forceActuatorDataC* forceActuatorData = M1M3SSPublisher::get().getForceActuatorData();

    short int* stage = &(forceActuatorBumpTestStatus->primaryTest[_zIndex]);

    switch (*stage) {
        case MTM1M3_shared_BumpTest_NotTested:
            if (abs(forceActuatorData->zForce[_zIndex]) >= _tolerance) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                spdlog::error("Cannot test {}, as measured parked force is too off 0 ({}, {})", _zIndex,
                              forceActuatorData->zForce[_zIndex], _tolerance);
                M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
                return States::BumpTestState;
            }
            forceController->applyActuatorOffset(_zIndex, 50);
            forceController->processAppliedForces();
            *stage = MTM1M3_shared_BumpTest_TestingPlus;
            break;
        case MTM1M3_shared_BumpTest_TestingPlus:
            break;

        case MTM1M3_shared_BumpTest_Passed:
            _zIndex = -1;
            return States::ParkedEngineeringState;
    }
    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
    return States::NoStateTransition;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
