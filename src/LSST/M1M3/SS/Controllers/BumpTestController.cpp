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
#include <ForceActuatorBumpTestStatus.h>
#include <ForceActuatorData.h>
#include <ForceActuatorSettings.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>

using namespace std::chrono;
using namespace MTM1M3;
using namespace LSST::M1M3::SS;

BumpTestController::BumpTestController() : _test_force(222), _bump_test_data(20) {
    SPDLOG_DEBUG("BumpTestController: BumpTestController()");

    ForceActuatorBumpTestStatus::instance().reset();
}

int BumpTestController::setBumpTestActuator(int actuator_id, bool test_primary, bool test_secondary) {
    auto &actuator_settings = ForceActuatorSettings::instance();

    _testedWarning = actuator_settings.TestedTolerances.warning;
    _testedError = actuator_settings.TestedTolerances.error;
    _nonTestedWarning = actuator_settings.NonTestedTolerances.warning;
    _nonTestedError = actuator_settings.NonTestedTolerances.error;

    _test_settle_time = milliseconds(static_cast<int>(actuator_settings.bumpTestSettleTime * 1000));
    _testMeasurements = actuator_settings.bumpTestMeasurements;

    _reset_progress();

    SettingReader::instance().getSafetyControllerSettings()->ForceController.enterBumpTesting();

    ForceActuatorBumpTestStatus::instance().trigger_bump_test(actuator_id, test_primary, test_secondary);

    return 0;
}

void BumpTestController::runLoop() {
    // force actuator data are updated only in UpdateCommand; as only a single
    // command can be executed, there isn't a race condition
    size_t tested_count = 0;
    size_t status_change = 0;

    auto &actuator_status = ForceActuatorBumpTestStatus::instance();

    _collect_results();

    _process_actuators_data();

    for (int i = 0; i < FA_COUNT; i++) {

	int actuator_id = SettingReader::instance().getForceActuatorApplicationSettings().Table[i].ActuatorID;

        if (actuator_status.primary_tested(i)) {
            bool changed = _run_cylinder(i, actuator_id, 'P', actuator_status.primaryTest[i],
                                         actuator_status.primaryTestTimestamps[i]);
	    tested_count++;
            if (changed) {
                status_change++;
            }
        }

        if (i < FA_S_COUNT) {
            if (actuator_status.secondary_tested(i)) {

                bool changed = _run_cylinder(i, actuator_id, 'S', actuator_status.secondaryTest[i],
                                             actuator_status.secondaryTestTimestamps[i]);
		tested_count++;
                if (changed) {
                    status_change++;
                }
            }
        }

    }

    if (status_change > 0) {
        ForceActuatorBumpTestStatus::instance().send(true);
    }

    if (tested_count == 0) {
        SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();
    }
}

void BumpTestController::stopAll(bool forced) {
    ForceActuatorBumpTestStatus::instance().stop_all();

    _reset_progress();

    if (forced) {
        SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();
    }
}

bool BumpTestController::_run_cylinder(int index, int actuator_id, char axis, int &stage, double &timestamp) {
    ForceController *forceController = Model::instance().getForceController();
    double now_timestamp = M1M3SSPublisher::instance().getTimestamp();

    auto now = steady_clock::now();

    bool positive = false;

    int checkRet = 0x11;

    if (_test_timeout[i] <= now) {
	   stage = MTM1M3_shared_BumpTest_Failed_Timeout;
	   return true;
    }

    switch (stage) {
        case MTM1M3_shared_BumpTest_Triggered:
        case MTM1M3_shared_BumpTest_TestingPositiveWait:
        case MTM1M3_shared_BumpTest_TestingNegativeWait:

	    // TODO code to make sure we can proceed

            switch (stage) {
                case MTM1M3_shared_BumpTest_Triggered:
                    forceController->applyActuatorOffset(axis, index, _testForce);
                    stage = MTM1M3_shared_BumpTest_TestingPositive;
                    break;
                case MTM1M3_shared_BumpTest_TestingPositiveWait:
                    forceController->applyActuatorOffset(axis, index, -_testForce);
                    stage = MTM1M3_shared_BumpTest_TestingNegative;
                    break;
                case MTM1M3_shared_BumpTest_TestingNegativeWait:
                    stage = MTM1M3_shared_BumpTest_Passed;
                    SPDLOG_INFO("Passed FA ID {} ({}{}) bump test", actuator_id, axis, index);
                    forceController->applyActuatorOffset(axis, index, 0);
		    break;
            }

            forceController->processAppliedForces();
            _test_timeout[index] = now + _test_settle_time;
            return true;

        case MTM1M3_shared_BumpTest_TestingPositive:
            positive = true;
        case MTM1M3_shared_BumpTest_TestingNegative:

            forceController->applyActuatorOffset(axis, index, 0);

            forceController->processAppliedForces();
            stage = positive ? MTM1M3_shared_BumpTest_TestingPositiveWait
                             : MTM1M3_shared_BumpTest_TestingNegativeWait;
	    _test_timeout[index] = now + _test_settle_time;
            return true;

        case MTM1M3_shared_BumpTest_Passed:
        case MTM1M3_shared_BumpTest_Failed:
            return false;
    }

    return false;
}

void BumpTestController::_reset_progress(bool zeroOffsets) {
    auto now = steady_clock::now();

    for (int i = 0; i < FA_COUNT; i++) {
        _sleep_until[i] = now;
    }

    _bump_test_data.clear();

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

void BumpTestController::_collect_results() {
    auto &fa_data = ForceActuatorData::instance();
    auto &fa_status = ForceActuatorBumpTestStatus::instance();

    _bump_test_data.add_data(fa_data.xForce, fa_data.yForce, fa_data.zForce, fa_data.primaryCylinderForces,
                             fa_data.secondaryCylinderForces, fa_status.primaryTest, fa_status.secondaryTest);
}

int axisIndexToActuatorId(char axis, int index) {
    const ForceActuatorApplicationSettings *forceSettings =
            SettingReader::instance().getForceActuatorApplicationSettings();

    if (axis == 'X') return forceSettings->Table[forceSettings->XIndexToZIndex[index]].ActuatorID;
    if (axis == 'Y') return forceSettings->Table[forceSettings->YIndexToZIndex[index]].ActuatorID;
    return forceSettings->Table[index].ActuatorID;
}

int BumpTestController::_process_actuators_data() {
	
}

#if 0
checkAverages(char axis, int index, double value) {
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
#endif
