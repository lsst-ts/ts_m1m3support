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

    _testForce = 222;
    _tolerance = 1;
    _testSettleTime = 3.0;
    _testMeasurements = 10;

    _resetProgress();

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

    short int* stage = &(forceActuatorBumpTestStatus->primaryTest[_zIndex]);

    bool positive = false;

    switch (*stage) {
        case MTM1M3_shared_BumpTest_NotTested:
        case MTM1M3_shared_BumpTest_TestingPositiveWait:
        case MTM1M3_shared_BumpTest_TestingNegativeWait:
            if (timestamp < _sleepUntil || _collectAverages() == false) break;

            if (_checkAverages() == false) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                spdlog::error("Cannot test {}, as measured parked force is too off 0 ({}, {})", _zIndex,
                              _zAverages[_zIndex], _tolerance);
                M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
                _resetProgress();
                return States::BumpTestState;
            }
            switch (*stage) {
                case MTM1M3_shared_BumpTest_NotTested:
                    forceController->applyActuatorOffset(_zIndex, _testForce);
                    *stage = MTM1M3_shared_BumpTest_TestingPlus;
                    break;
                case MTM1M3_shared_BumpTest_TestingPositiveWait:
                    forceController->applyActuatorOffset(_zIndex, -_testForce);
                    *stage = MTM1M3_shared_BumpTest_TestingNegative;
                    break;
                case MTM1M3_shared_BumpTest_TestingNegativeWait:
                    _resetProgress();
                    *stage = MTM1M3_shared_BumpTest_Passed;
                    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
                    return States::ParkedEngineeringState;
                    break;
            }
            forceController->processAppliedForces();
            _sleepUntil = timestamp + _testSettleTime;
            break;

        case MTM1M3_shared_BumpTest_TestingPlus:
            positive = true;
        case MTM1M3_shared_BumpTest_TestingNegative:
            if (timestamp < _sleepUntil || _collectAverages() == false) break;

            if (_checkAverages('Z', _zIndex, positive ? _testForce : -_testForce) == false) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                spdlog::error("Cannot test {}, as measured {} force is too off {} ({}, {})",
                              positive ? "plus" : "negative", _zIndex, _testForce, _zAverages[_zIndex],
                              _tolerance);
                M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
                _resetProgress();
                return States::ParkedEngineeringState;
            }

            forceController->zeroOffsetForces();
            forceController->processAppliedForces();
            *stage = positive ? MTM1M3_shared_BumpTest_TestingPositiveWait
                              : MTM1M3_shared_BumpTest_TestingNegativeWait;
            _sleepUntil = timestamp + _testSettleTime;

            break;

        case MTM1M3_shared_BumpTest_Passed:
            _zIndex = -1;
            return States::ParkedEngineeringState;
    }
    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
    return States::NoStateTransition;
}  // namespace SS

void BumpTestController::_resetProgress() {
    _testProgress = 0;
    _sleepUntil = 0;

    _resetAverages();

    Model::get().getForceController()->zeroOffsetForces();
    Model::get().getForceController()->processAppliedForces();
}

void BumpTestController::_resetAverages() {
    memset(_xAverages, 0, sizeof(_xAverages));
    memset(_yAverages, 0, sizeof(_yAverages));
    memset(_zAverages, 0, sizeof(_zAverages));
}

bool BumpTestController::_collectAverages() {
    if (_testProgress == 0) _resetAverages();

    _testProgress++;

    for (int i = 0; i < FA_X_COUNT; ++i)
        _xAverages[i] += M1M3SSPublisher::get().getForceActuatorData()->xForce[i];
    for (int i = 0; i < FA_Y_COUNT; ++i)
        _yAverages[i] += M1M3SSPublisher::get().getForceActuatorData()->yForce[i];
    for (int i = 0; i < FA_Z_COUNT; ++i)
        _zAverages[i] += M1M3SSPublisher::get().getForceActuatorData()->zForce[i];

    if (_testProgress >= _testMeasurements) {
        for (int i = 0; i < FA_X_COUNT; ++i) _xAverages[i] /= _testProgress;
        for (int i = 0; i < FA_Y_COUNT; ++i) _yAverages[i] /= _testProgress;
        for (int i = 0; i < FA_Z_COUNT; ++i) _zAverages[i] /= _testProgress;
        _testProgress = 0;

        return true;
    }
    return false;
}

bool BumpTestController::_checkAverages(char axis, int index, double value) {
    auto _inTolerance = [](char axis, int index, double value, double expected, double tolerance) {
        if (abs(value - expected) >= tolerance) {
            spdlog::warn(
                    "Value for {} actuator {} outside of tolerance - measured {}, expected {}, tolerance {}",
                    axis, index, value, expected, tolerance);
            return false;
        }
        return true;
    };

    bool ret = true;

    for (int i = 0; i < FA_X_COUNT; ++i) {
        if (axis == 'X' && index == i) {
            ret &= _inTolerance('X', i, _xAverages[i], value, _tolerance);

        } else {
            ret &= _inTolerance('X', i, _xAverages[i], 0, _tolerance);
        }
    }
    for (int i = 0; i < FA_Y_COUNT; ++i) {
        if (axis == 'Y' && index == i) {
            ret &= _inTolerance('Y', i, _yAverages[i], value, _tolerance);

        } else {
            ret &= _inTolerance('Y', i, _yAverages[i], 0, _tolerance);
        }
    }
    for (int i = 0; i < FA_Z_COUNT; ++i) {
        if (axis == 'Z' && index == i) {
            ret &= _inTolerance('Z', i, _zAverages[i], value, _tolerance);

        } else {
            ret &= _inTolerance('Z', i, _zAverages[i], 0, _tolerance);
        }
    }

    return ret;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
