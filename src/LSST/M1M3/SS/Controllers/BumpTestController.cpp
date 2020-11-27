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
    SPDLOG_DEBUG("BumpTestController: BumpTestController()");
    _xIndex = -1;
    _yIndex = -1;
    _zIndex = -1;

    M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus()->actuatorId = -1;
    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
}

void BumpTestController::setBumpTestActuator(int actuatorId, bool testPrimary, bool testSecondary) {
    _zIndex = SettingReader::get().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(actuatorId);
    _xIndex = SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[_zIndex];
    _yIndex = SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[_zIndex];
    _secondaryIndex = SettingReader::get()
                              .getForceActuatorApplicationSettings()
                              ->ZIndexToSecondaryCylinderIndex[_zIndex];

    _testPrimary = testPrimary;
    _testSecondary = _secondaryIndex < 0 ? false : testSecondary;

    _testForce = 222;
    _warning = 2.5;
    _tolerance = 5;
    _testSettleTime = 3.0;
    _testMeasurements = 10;

    _resetProgress();

    MTM1M3_logevent_forceActuatorBumpTestStatusC* forceActuatorBumpTestStatus =
            M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus();

    forceActuatorBumpTestStatus->actuatorId = actuatorId;

    if (_testPrimary) {
        forceActuatorBumpTestStatus->primaryTest[_zIndex] = MTM1M3_shared_BumpTest_NotTested;
    }

    if (_testSecondary) {
        forceActuatorBumpTestStatus->secondaryTest[_secondaryIndex] = MTM1M3_shared_BumpTest_NotTested;
    }

    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
}

void BumpTestController::runLoop() {
    // force actuator data are updated only in UpdateCommand; as only a single
    // command can be executed, there isn't a race condition
    MTM1M3_logevent_forceActuatorBumpTestStatusC* forceActuatorBumpTestStatus =
            M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus();

    if (forceActuatorBumpTestStatus->actuatorId < 0) return;

    if (_testPrimary) {
        bool pRet =
                _runCylinder('Z', _zIndex, _zAverages, &(forceActuatorBumpTestStatus->primaryTest[_zIndex]));
        if (pRet == true) {
            M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
            return;
        }
        forceActuatorBumpTestStatus->primaryTestTimestamps[_zIndex] = M1M3SSPublisher::get().getTimestamp();
        _testPrimary = false;
    }

    if (_testSecondary) {
        bool sRet = true;
        short int* secondaryStage = &(forceActuatorBumpTestStatus->secondaryTest[_secondaryIndex]);
        if (_xIndex >= 0)
            sRet = _runCylinder('X', _xIndex, _xAverages, secondaryStage);
        else if (_yIndex >= 0)
            sRet = _runCylinder('Y', _yIndex, _yAverages, secondaryStage);

        if (sRet == true) {
            M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
            return;
        }
        forceActuatorBumpTestStatus->secondaryTestTimestamps[_secondaryIndex] =
                M1M3SSPublisher::get().getTimestamp();
        _testSecondary = false;
    }

    forceActuatorBumpTestStatus->actuatorId = -1;
    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
}

void BumpTestController::stopAll() {
    M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus()->actuatorId = -1;

    _resetProgress();

    M1M3SSPublisher::get().logForceActuatorBumpTestStatus();
}

void BumpTestController::stopCylinder(char axis) {
    if ((axis == 'Z' && _testSecondary == false) || _testPrimary == false) {
        stopAll();
        return;
    }

    if (axis == 'Z') {
        _testPrimary = false;
    } else {
        _testSecondary = false;
    }

    _resetProgress();

    _sleepUntil = M1M3SSPublisher::get().getTimestamp() + _testSettleTime;
}

bool BumpTestController::_runCylinder(char axis, int index, double averages[], short int* stage) {
    ForceController* forceController = Model::get().getForceController();
    double timestamp = M1M3SSPublisher::get().getTimestamp();
    MTM1M3_logevent_forceActuatorBumpTestStatusC* forceActuatorBumpTestStatus =
            M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus();
    forceActuatorBumpTestStatus->timestamp = timestamp;

    bool positive = false;

    switch (*stage) {
        case MTM1M3_shared_BumpTest_NotTested:
        case MTM1M3_shared_BumpTest_TestingPositiveWait:
        case MTM1M3_shared_BumpTest_TestingNegativeWait:
            if (timestamp < _sleepUntil || _collectAverages() == false) break;

            if (_checkAverages() & 0x01) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                SPDLOG_ERROR(
                        "Failed FA ID {} ({}{}) bump test - measured parked force is too far from 0 ({}, {})",
                        forceActuatorBumpTestStatus->actuatorId, axis, index, averages[index], _tolerance);
                stopCylinder(axis);
                return false;
            }
            switch (*stage) {
                case MTM1M3_shared_BumpTest_NotTested:
                    forceController->applyActuatorOffset(axis, index, _testForce);
                    *stage = MTM1M3_shared_BumpTest_TestingPositive;
                    break;
                case MTM1M3_shared_BumpTest_TestingPositiveWait:
                    forceController->applyActuatorOffset(axis, index, -_testForce);
                    *stage = MTM1M3_shared_BumpTest_TestingNegative;
                    break;
                case MTM1M3_shared_BumpTest_TestingNegativeWait:
                    *stage = MTM1M3_shared_BumpTest_Passed;
                    SPDLOG_INFO("Passed FA ID {} ({}{}) bump test", forceActuatorBumpTestStatus->actuatorId,
                                axis, index);
                    _resetProgress(false);
                    return false;
            }
            forceController->processAppliedForces();
            _sleepUntil = timestamp + _testSettleTime;
            break;

        case MTM1M3_shared_BumpTest_TestingPositive:
            positive = true;
        case MTM1M3_shared_BumpTest_TestingNegative:
            if (timestamp < _sleepUntil || _collectAverages() == false) break;

            if (_checkAverages(axis, index, positive ? _testForce : -_testForce) & 0x01) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                SPDLOG_ERROR("Failed FA ID {} ({}{}) bump test - measured {} force is too far {} ({}, {})",
                             forceActuatorBumpTestStatus->actuatorId, axis, index,
                             positive ? "plus" : "negative", _testForce, averages[index], _tolerance);
                stopCylinder(axis);
                return false;
            }

            forceController->zeroOffsetForces();
            forceController->processAppliedForces();
            *stage = positive ? MTM1M3_shared_BumpTest_TestingPositiveWait
                              : MTM1M3_shared_BumpTest_TestingNegativeWait;
            _sleepUntil = timestamp + _testSettleTime;

            break;

        case MTM1M3_shared_BumpTest_Passed:
        case MTM1M3_shared_BumpTest_Failed:
            return false;
    }

    return true;
}

void BumpTestController::_resetProgress(bool zeroOffsets) {
    _testProgress = 0;
    _sleepUntil = 0;

    _resetAverages();

    if (zeroOffsets) {
        Model::get().getForceController()->zeroOffsetForces();
        Model::get().getForceController()->processAppliedForces();
    }
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

int BumpTestController::_checkAverages(char axis, int index, double value) {
    auto _inTolerance = [](char axis, int index, double value, double expected, double tolerance,
                           double warning) {
        double err = abs(value - expected);
        if (err >= tolerance) {
            SPDLOG_ERROR(
                    "Value for {} actuator {} outside of tolerance - measured {}, expected {}, tolerance "
                    "{}",
                    axis, index, value, expected, tolerance);
            return 0x01;
        }
        if (err >= warning) {
            SPDLOG_WARN(
                    "Value for {} actuator {} following error warning - measured {}, expected {}, tolerance "
                    "{}",
                    axis, index, value, expected, warning);
            return 0x02;
        }

        return 0;
    };

    int ret = 0;

    for (int i = 0; i < FA_X_COUNT; ++i) {
        if (axis == 'X' && index == i) {
            ret |= _inTolerance('X', i, _xAverages[i], value, _tolerance, _warning);

        } else {
            ret |= _inTolerance('X', i, _xAverages[i], 0, _tolerance, _warning);
        }
    }
    for (int i = 0; i < FA_Y_COUNT; ++i) {
        if (axis == 'Y' && index == i) {
            ret |= _inTolerance('Y', i, _yAverages[i], value, _tolerance, _warning);

        } else {
            ret |= _inTolerance('Y', i, _yAverages[i], 0, _tolerance, _warning);
        }
    }
    for (int i = 0; i < FA_Z_COUNT; ++i) {
        if (axis == 'Z' && index == i) {
            ret |= _inTolerance('Z', i, _zAverages[i], value, _tolerance, _warning);

        } else {
            ret |= _inTolerance('Z', i, _zAverages[i], 0, _tolerance, _warning);
        }
    }

    return ret;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
