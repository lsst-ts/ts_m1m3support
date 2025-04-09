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

BumpTestController::BumpTestController() : _test_force(222), _bump_test_data(NULL) {
    SPDLOG_DEBUG("BumpTestController: BumpTestController()");

    ForceActuatorBumpTestStatus::instance().reset();
}

BumpTestController::~BumpTestController() { delete _bump_test_data; }

int BumpTestController::setBumpTestActuator(int actuator_id, bool cylinders, bool test_primary,
                                            bool test_secondary) {
    _test_settle_time =
            milliseconds(static_cast<int>(ForceActuatorSettings::instance().bumpTestSettleTime * 1000));

    SettingReader::instance().getSafetyControllerSettings()->ForceController.enterBumpTesting();

    auto fa_app_settings = SettingReader::instance().getForceActuatorApplicationSettings();
    auto z_index = fa_app_settings->ActuatorIdToZIndex(actuator_id);

    _test_timeout[z_index] = steady_clock::now() + _test_settle_time;
    _cylinders[z_index] = cylinders;

    ForceActuatorBumpTestStatus::instance().trigger_bump_test(z_index, test_primary, test_secondary);

    return 0;
}

void BumpTestController::runLoop() {
    if (_bump_test_data == NULL) {
        _bump_test_data = new FABumpTestData(ForceActuatorSettings::instance().bumpTestMeasurements);
    }
    // force actuator data are updated only in UpdateCommand; as only a single
    // command can be executed, there isn't a race condition
    size_t tested_count = 0;
    size_t status_change = 0;

    auto &actuator_status = ForceActuatorBumpTestStatus::instance();
    auto fa_app_settings = SettingReader::instance().getForceActuatorApplicationSettings();

    _collect_results();

    BumpTestStatus primary_status[FA_COUNT], secondary_status[FA_COUNT], x_status[FA_COUNT],
            y_status[FA_COUNT], z_status[FA_COUNT];

    _bump_test_data->test_mirror('P', primary_status);
    _bump_test_data->test_mirror('S', secondary_status);

    _bump_test_data->test_mirror('X', x_status);
    _bump_test_data->test_mirror('Y', y_status);
    _bump_test_data->test_mirror('Z', z_status);

    for (int i = 0; i < FA_COUNT; i++) {
        int actuator_id = fa_app_settings->Table[i].ActuatorID;

        if (actuator_status.primary_tested(i) == true) {
            bool changed = false;
            if (_cylinders[i]) {
                changed = _run_axis(i, i, actuator_id, 'P', primary_status[i], actuator_status.primaryTest[i],
                                    actuator_status.primaryTestTimestamps[i]);
            } else {
                changed = _run_axis(i, i, actuator_id, 'Z', z_status[i], actuator_status.primaryTest[i],
                                    actuator_status.primaryTestTimestamps[i]);
            }
            tested_count++;
            if (changed) {
                status_change++;
            }
        }

        if (i < FA_S_COUNT) {
            actuator_id = fa_app_settings->SecondaryCylinderIndexToActuatorId(i);
            int z_index = fa_app_settings->ActuatorIdToZIndex(actuator_id);
            if (actuator_status.primary_tested(z_index) == false &&
                actuator_status.secondary_tested(i) == true) {
                bool changed = false;
                if (_cylinders[i]) {
                    changed = _run_axis(i, z_index, actuator_id, 'S', secondary_status[z_index],
                                        actuator_status.secondaryTest[i],
                                        actuator_status.secondaryTestTimestamps[i]);
                } else {
                    int x_index = fa_app_settings->ZIndexToXIndex[z_index];
                    if (x_index >= 0) {
                        changed = _run_axis(x_index, z_index, actuator_id, 'X', x_status[z_index],
                                            actuator_status.secondaryTest[i],
                                            actuator_status.secondaryTestTimestamps[i]);
                    } else {
                        int y_index = fa_app_settings->ZIndexToYIndex[z_index];
                        changed = _run_axis(y_index, z_index, actuator_id, 'Y', y_status[z_index],
                                            actuator_status.secondaryTest[i],
                                            actuator_status.secondaryTestTimestamps[i]);
                    }
                }

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

    delete _bump_test_data;
    _bump_test_data = NULL;

    if (forced) {
        SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();
    }
}

bool BumpTestController::_run_axis(int axis_index, int z_index, int actuator_id, char axis,
                                   const BumpTestStatus status, int &stage, double &timestamp) {
    ForceController *forceController = Model::instance().getForceController();
    double now_timestamp = M1M3SSPublisher::instance().getTimestamp();

    auto now = steady_clock::now();

    bool positive = false;

    int checkRet = 0x11;

    if (_test_timeout[z_index] <= now) {
        stage = MTM1M3_shared_BumpTest_Failed_Timeout;
        SPDLOG_WARN("BumpTest: Timeout of actuator {} - {} (axis index {}, Z index {})", actuator_id, axis,
                    axis_index, z_index);
        return true;
    }

    switch (stage) {
        case MTM1M3_shared_BumpTest_Triggered:
        case MTM1M3_shared_BumpTest_TestingPositiveWait:
        case MTM1M3_shared_BumpTest_TestingNegativeWait:
            if (status != BumpTestStatus::PASSED) {
                return false;
            }

            switch (stage) {
                case MTM1M3_shared_BumpTest_Triggered:
                    forceController->applyActuatorOffset(axis, axis_index, _test_force);
                    stage = MTM1M3_shared_BumpTest_TestingPositive;
                    break;
                case MTM1M3_shared_BumpTest_TestingPositiveWait:
                    forceController->applyActuatorOffset(axis, axis_index, -_test_force);
                    stage = MTM1M3_shared_BumpTest_TestingNegative;
                    break;
                case MTM1M3_shared_BumpTest_TestingNegativeWait:
                    stage = MTM1M3_shared_BumpTest_Passed;
                    SPDLOG_INFO("Passed FA ID {} ({}{}) bump test", actuator_id, axis, axis_index);
                    forceController->applyActuatorOffset(axis, axis_index, 0);
                    break;
            }

            forceController->processAppliedForces();
            _test_timeout[z_index] = now + _test_settle_time;
            return true;

        case MTM1M3_shared_BumpTest_TestingPositive:
            positive = true;
        case MTM1M3_shared_BumpTest_TestingNegative:
            if (status != BumpTestStatus::PASSED) {
                return false;
            }

            forceController->applyActuatorOffset(axis, axis_index, 0);

            forceController->processAppliedForces();
            stage = positive ? MTM1M3_shared_BumpTest_TestingPositiveWait
                             : MTM1M3_shared_BumpTest_TestingNegativeWait;
            _test_timeout[z_index] = now + _test_settle_time;
            return true;

        default:
            return false;
    }

    return false;
}

void BumpTestController::_reset_progress(bool zeroOffsets) {
    auto now = steady_clock::now();

    for (int i = 0; i < FA_COUNT; i++) {
        _test_timeout[i] = now - _test_settle_time;
        _cylinders[i] = false;
    }

    _bump_test_data->clear();

    if (zeroOffsets) {
        Model::instance().getForceController()->zeroOffsetForces();
        Model::instance().getForceController()->processAppliedForces();

        // make sure exitBumpTesting will be caled after FA gets back to ~proper
        // values
        //
        // Drop of the measuredForce on top of FAs can take some time, as it isn't
        // instantinous due to physics involved
    }
}

void BumpTestController::_collect_results() {
    auto &fa_data = ForceActuatorData::instance();
    auto &fa_status = ForceActuatorBumpTestStatus::instance();

    _bump_test_data->add_data(fa_data.xForce, fa_data.yForce, fa_data.zForce, fa_data.primaryCylinderForce,
                              fa_data.secondaryCylinderForce, fa_status.primaryTest, fa_status.secondaryTest);
}
