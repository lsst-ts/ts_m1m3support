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

#include <BumpTestController.h>
#include <ForceActuatorData.h>
#include <ForceActuatorSettings.h>
#include <ForceController.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SettingReader.h>

using namespace MTM1M3;

namespace LSST {
namespace M1M3 {
namespace SS {

BumpTestController::BumpTestController() : _testForce(222) {
    SPDLOG_DEBUG("BumpTestController: BumpTestController()");
    _xIndex = -1;
    _yIndex = -1;
    _zIndex = -1;

    MTM1M3_logevent_forceActuatorBumpTestStatusC *forceActuatorBumpTestStatus =
            M1M3SSPublisher::instance().getEventForceActuatorBumpTestStatus();
    for (int i = 0; i < FA_COUNT; i++) {
        forceActuatorBumpTestStatus->primaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
    }
    for (int i = 0; i < FA_X_COUNT + FA_Y_COUNT; i++) {
        forceActuatorBumpTestStatus->secondaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
    }

    forceActuatorBumpTestStatus->actuatorId = -1;
    M1M3SSPublisher::instance().logForceActuatorBumpTestStatus();
}

int BumpTestController::setBumpTestActuator(int actuatorId, bool testPrimary, bool testSecondary) {
    _zIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(actuatorId);
    _xIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[_zIndex];
    _yIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[_zIndex];
    _secondaryIndex = SettingReader::instance()
                              .getForceActuatorApplicationSettings()
                              ->ZIndexToSecondaryCylinderIndex[_zIndex];

    _testedWarning = ForceActuatorSettings::instance().TestedTolerances.warning;
    _testedError = ForceActuatorSettings::instance().TestedTolerances.error;
    _nonTestedWarning = ForceActuatorSettings::instance().NonTestedTolerances.warning;
    _nonTestedError = ForceActuatorSettings::instance().NonTestedTolerances.error;

    _testSettleTime = ForceActuatorSettings::instance().bumpTestSettleTime;
    _testMeasurements = ForceActuatorSettings::instance().bumpTestMeasurements;

    _testPrimary = testPrimary;
    _testSecondary = _secondaryIndex < 0 ? false : testSecondary;

    _resetProgress();

    SettingReader::instance().getSafetyControllerSettings()->ForceController.enterBumpTesting();

    MTM1M3_logevent_forceActuatorBumpTestStatusC *forceActuatorBumpTestStatus =
            M1M3SSPublisher::instance().getEventForceActuatorBumpTestStatus();

    forceActuatorBumpTestStatus->actuatorId = actuatorId;

    if (_testPrimary) {
        forceActuatorBumpTestStatus->primaryTest[_zIndex] = MTM1M3_shared_BumpTest_NotTested;
    }

    if (_testSecondary) {
        forceActuatorBumpTestStatus->secondaryTest[_secondaryIndex] = MTM1M3_shared_BumpTest_NotTested;
    }

    M1M3SSPublisher::instance().logForceActuatorBumpTestStatus();
    return 0;
}

void BumpTestController::runLoop() {
    // force actuator data are updated only in UpdateCommand; as only a single
    // command can be executed, there isn't a race condition
    MTM1M3_logevent_forceActuatorBumpTestStatusC *forceActuatorBumpTestStatus =
            M1M3SSPublisher::instance().getEventForceActuatorBumpTestStatus();

    if (forceActuatorBumpTestStatus->actuatorId < 0) {
        if (_sleepUntil != 0) {
            double timestamp = M1M3SSPublisher::instance().getTimestamp();
            if (_sleepUntil <= timestamp) {
                SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();
                _sleepUntil = 0;
            }
        }
        return;
    }

    if (_testPrimary) {
        runCylinderReturn_t pRet =
                _runCylinder('Z', _zIndex, _zAverages, &(forceActuatorBumpTestStatus->primaryTest[_zIndex]));
        switch (pRet) {
            case STATE_CHANGED:
                M1M3SSPublisher::instance().logForceActuatorBumpTestStatus();
                return;
            case NO_CHANGE:
                return;
            case FAILED:
            case FINISHED:
                break;
        }

        forceActuatorBumpTestStatus->primaryTestTimestamps[_zIndex] =
                M1M3SSPublisher::instance().getTimestamp();
        _testPrimary = false;
    }

    if (_testSecondary) {
        runCylinderReturn_t sRet = FAILED;
        auto secondaryStage = &(forceActuatorBumpTestStatus->secondaryTest[_secondaryIndex]);
        if (_xIndex >= 0)
            sRet = _runCylinder('X', _xIndex, _xAverages, secondaryStage);
        else if (_yIndex >= 0)
            sRet = _runCylinder('Y', _yIndex, _yAverages, secondaryStage);

        switch (sRet) {
            case STATE_CHANGED:
                M1M3SSPublisher::instance().logForceActuatorBumpTestStatus();
                return;
            case NO_CHANGE:
                return;
            case FAILED:
            case FINISHED:
                break;
        }

        forceActuatorBumpTestStatus->secondaryTestTimestamps[_secondaryIndex] =
                M1M3SSPublisher::instance().getTimestamp();
        _testSecondary = false;
    }

    forceActuatorBumpTestStatus->actuatorId = -1;
    M1M3SSPublisher::instance().logForceActuatorBumpTestStatus();
}

void BumpTestController::stopAll(bool forced) {
    M1M3SSPublisher::instance().getEventForceActuatorBumpTestStatus()->actuatorId = -1;

    _resetProgress();

    if (forced) {
        SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();
    }

    M1M3SSPublisher::instance().logForceActuatorBumpTestStatus();
}

void BumpTestController::stopCylinder(char axis) {
    if ((axis == 'Z' && _testSecondary == false) || _testPrimary == false) {
        stopAll(false);
        return;
    }

    if (axis == 'Z') {
        _testPrimary = false;
    } else {
        _testSecondary = false;
    }

    _resetProgress();
}

BumpTestController::runCylinderReturn_t BumpTestController::_runCylinder(char axis, int index,
                                                                         double averages[], int *stage) {
    ForceController *forceController = Model::instance().getForceController();
    double timestamp = M1M3SSPublisher::instance().getTimestamp();
    MTM1M3_logevent_forceActuatorBumpTestStatusC *forceActuatorBumpTestStatus =
            M1M3SSPublisher::instance().getEventForceActuatorBumpTestStatus();
    forceActuatorBumpTestStatus->timestamp = timestamp;

    bool positive = false;

    int checkRet = 0x11;

    switch (*stage) {
        case MTM1M3_shared_BumpTest_NotTested:
        case MTM1M3_shared_BumpTest_TestingPositiveWait:
        case MTM1M3_shared_BumpTest_TestingNegativeWait:
            if (timestamp < _sleepUntil || _collectAverages() == false) return NO_CHANGE;

            checkRet = _checkAverages();

            if (checkRet & 0x01) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                SPDLOG_ERROR(
                        "Failed FA ID {} ({}{}) bump test - measured parked force "
                        "({:.3f}) is too "
                        "far from "
                        "0\xb1{}",
                        forceActuatorBumpTestStatus->actuatorId, axis, index, averages[index], _testedError);
                stopCylinder(axis);
                return FAILED;
            }
            if (checkRet & 0x10) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                SPDLOG_ERROR(
                        "Failed FA ID {} ({}{}) bump test - measured parked force on some "
                        "actuator(s) is too "
                        "far from 0\xb1{}",
                        forceActuatorBumpTestStatus->actuatorId, axis, index, _nonTestedError);
                stopCylinder(axis);
                return FAILED;
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
                    return FINISHED;
            }
            forceController->processAppliedForces();
            _sleepUntil = timestamp + _testSettleTime;
            break;

        case MTM1M3_shared_BumpTest_TestingPositive:
            positive = true;
        case MTM1M3_shared_BumpTest_TestingNegative:
            if (timestamp < _sleepUntil || _collectAverages() == false) return NO_CHANGE;

            checkRet = _checkAverages(axis, index, positive ? _testForce : -_testForce);

            if (checkRet & 0x01) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                SPDLOG_ERROR(
                        "Failed FA ID {} ({}{}) bump test - measured force {} "
                        "({:.3f}) is too far "
                        "{}\xb1{}",
                        forceActuatorBumpTestStatus->actuatorId, axis, index, positive ? "plus" : "negative",
                        averages[index], _testForce, _testedError);
                stopCylinder(axis);
                return FAILED;
            }
            if (checkRet & 0x10) {
                *stage = MTM1M3_shared_BumpTest_Failed;
                SPDLOG_ERROR(
                        "Failed FA ID {} ({}{}) bump test - measured force on some "
                        "actuator(s) is "
                        "too far "
                        "from far from \xb1{}",
                        forceActuatorBumpTestStatus->actuatorId, axis, index, _nonTestedError);
                stopCylinder(axis);
                return FAILED;
            }

            forceController->zeroOffsetForces();
            forceController->processAppliedForces();
            *stage = positive ? MTM1M3_shared_BumpTest_TestingPositiveWait
                              : MTM1M3_shared_BumpTest_TestingNegativeWait;
            _sleepUntil = timestamp + _testSettleTime;
            break;

        case MTM1M3_shared_BumpTest_Passed:
            return FINISHED;
        case MTM1M3_shared_BumpTest_Failed:
            return FAILED;
    }

    return STATE_CHANGED;
}

void BumpTestController::_resetProgress(bool zeroOffsets) {
    _testProgress = 0;
    _sleepUntil = 0;

    _resetAverages();

    if (zeroOffsets) {
        Model::instance().getForceController()->zeroOffsetForces();
        Model::instance().getForceController()->processAppliedForces();

        // make sure exitBumpTesting will be caled after FA gets back to ~proper
        // values
        //
        // Drop of the measuredForce on top of FAs can take some time, as it isn't
        // instantinous due to physics involved
        _sleepUntil = M1M3SSPublisher::instance().getTimestamp() + _testSettleTime;
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

    for (int i = 0; i < FA_X_COUNT; ++i) _xAverages[i] += ForceActuatorData::instance().xForce[i];
    for (int i = 0; i < FA_Y_COUNT; ++i) _yAverages[i] += ForceActuatorData::instance().yForce[i];
    for (int i = 0; i < FA_Z_COUNT; ++i) _zAverages[i] += ForceActuatorData::instance().zForce[i];

    if (_testProgress >= _testMeasurements) {
        for (int i = 0; i < FA_X_COUNT; ++i) _xAverages[i] /= _testProgress;
        for (int i = 0; i < FA_Y_COUNT; ++i) _yAverages[i] /= _testProgress;
        for (int i = 0; i < FA_Z_COUNT; ++i) _zAverages[i] /= _testProgress;
        _testProgress = 0;

        return true;
    }
    return false;
}

int axisIndexToActuatorId(char axis, int index) {
    const ForceActuatorApplicationSettings *forceSettings =
            SettingReader::instance().getForceActuatorApplicationSettings();

    if (axis == 'X') return forceSettings->Table[forceSettings->XIndexToZIndex[index]].ActuatorID;
    if (axis == 'Y') return forceSettings->Table[forceSettings->YIndexToZIndex[index]].ActuatorID;
    return forceSettings->Table[index].ActuatorID;
}

int BumpTestController::_checkAverages(char axis, int index, double value) {
    auto _inTolerance = [](char axis, int index, double value, double expected, float error, float warning) {
        double err = abs(value - expected);
        if (err >= error) {
            SPDLOG_ERROR(
                    "FA ID {} ({}{}) following error violation - measured "
                    "{:.3f}, expected {}\xb1{}",
                    axisIndexToActuatorId(axis, index), axis, index, value, expected, error);
            return 0x01;
        }
        if (err >= warning) {
            SPDLOG_WARN(
                    "FA ID {} ({}{}) following error warning - measured {:.3f}, "
                    "expected {}\xb1{}",
                    axisIndexToActuatorId(axis, index), axis, index, value, expected, warning);
            return 0x02;
        }

        return 0;
    };

    int ret = 0;

    for (int i = 0; i < FA_X_COUNT; ++i) {
        if (axis == 'X' && index == i) {
            ret |= _inTolerance('X', i, _xAverages[i], value, _testedError, _testedWarning);
        } else {
            ret |= _inTolerance('X', i, _xAverages[i], 0, _nonTestedError, _nonTestedWarning) << 4;
        }
    }
    for (int i = 0; i < FA_Y_COUNT; ++i) {
        if (axis == 'Y' && index == i) {
            ret |= _inTolerance('Y', i, _yAverages[i], value, _testedError, _testedWarning);
        } else {
            ret |= _inTolerance('Y', i, _yAverages[i], 0, _nonTestedError, _nonTestedWarning) << 4;
        }
    }
    for (int i = 0; i < FA_Z_COUNT; ++i) {
        if (axis == 'Z' && index == i) {
            ret |= _inTolerance('Z', i, _zAverages[i], value, _testedError, _testedWarning);
        } else {
            ret |= _inTolerance('Z', i, _zAverages[i], 0, _nonTestedError, _nonTestedWarning) << 4;
        }
    }

    return ret;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
