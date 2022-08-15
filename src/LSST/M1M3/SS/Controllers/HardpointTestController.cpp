/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Telescope & Site Software Systems.
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

#include <HardpointActuatorMotionStates.h>
#include <HardpointTestController.h>
#include <ForceController.h>
#include <Model.h>
#include <Publisher.h>
#include <SettingReader.h>

using namespace MTM1M3;

namespace LSST {
namespace M1M3 {
namespace SS {

HardpointTestController::HardpointTestController(PositionController *positionController) {
    SPDLOG_DEBUG("HardpointTestController: HardpointTestController()");

    _positionController = positionController;
    _hardpointActuatorData = M1M3SSPublisher::get().getHardpointActuatorData();
    _hardpointActuatorState = M1M3SSPublisher::get().getEventHardpointActuatorState();
    _hardpointActuatorWarning = M1M3SSPublisher::get().getEventHardpointActuatorWarning();

    for (int i = 0; i < HP_COUNT; i++) {
        testState[i] = MTM1M3_shared_HardpointTest_NotTested;
    }

    M1M3SSPublisher::get().logHardpointTestStatus(this);
}

int HardpointTestController::startHardpointTest(int hardpointIndex) {
    switch (testState[hardpointIndex]) {
        case MTM1M3_shared_HardpointTest_NotTested:
        case MTM1M3_shared_HardpointTest_Passed:
        case MTM1M3_shared_HardpointTest_Failed:
            testState[hardpointIndex] = MTM1M3_shared_HardpointTest_MovingNegative;
            M1M3SSPublisher::get().logHardpointTestStatus(this);
            return 0;
        default:
            return 1;
    }
}

void HardpointTestController::runLoop() {
    bool changed = false;
    for (int i = 0; i < HP_COUNT; i++) {
        changed |= _runHardpointLoop(i);
    }

    if (changed) {
        M1M3SSPublisher::get().logHardpointTestStatus(this);
    }
}

int HardpointTestController::killHardpointTest(int hardpointIndex) {
    switch (testState[hardpointIndex]) {
        case MTM1M3_shared_HardpointTest_NotTested:
        case MTM1M3_shared_HardpointTest_Passed:
        case MTM1M3_shared_HardpointTest_Failed:
            return 1;
        default:
            testState[hardpointIndex] = MTM1M3_shared_HardpointTest_NotTested;
            M1M3SSPublisher::get().logHardpointTestStatus(this);
            return 0;
    }
}

bool HardpointTestController::_runHardpointLoop(int hardpointIndex) {
    int oldState = testState[hardpointIndex];
    switch (oldState) {
        case MTM1M3_shared_HardpointTest_NotTested:
        case MTM1M3_shared_HardpointTest_Passed:
        case MTM1M3_shared_HardpointTest_Failed:
            return false;
        case MTM1M3_shared_HardpointTest_MovingNegative:
            if (_hardpointActuatorWarning->limitSwitch2Operated[hardpointIndex]) {
                testState[hardpointIndex] = MTM1M3_shared_HardpointTest_TestingPositive;
            } else if (_hardpointActuatorState->motionState[hardpointIndex] !=
                       HardpointActuatorMotionStates::Stepping) {
                int32_t steps[HP_COUNT];
                memset(steps, 0, sizeof(steps));
                steps[hardpointIndex] = -999999999;
                if (_positionController->move(steps) == false) {
                    testState[hardpointIndex] = MTM1M3_shared_HardpointTest_Failed;
                }
            }
            break;
        case MTM1M3_shared_HardpointTest_TestingPositive:
            break;
        case MTM1M3_shared_HardpointTest_TestingNegative:
            break;
        case MTM1M3_shared_HardpointTest_MovingReference:
            break;
        default:
            break;
    }

    return oldState != testState[hardpointIndex];
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
