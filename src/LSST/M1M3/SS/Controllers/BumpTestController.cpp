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

#include "BumpTestController.h"
#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorBumpTestStatus.h"
#include "ForceActuatorData.h"
#include "ForceActuatorSettings.h"
#include "ForceController.h"
#include "M1M3SSPublisher.h"
#include "Model.h"
#include "SettingReader.h"

using namespace std::chrono;
using namespace MTM1M3;
using namespace LSST::M1M3::SS;

BumpTestController::BumpTestController() : _test_force(222), _bump_test_data(NULL) {
    SPDLOG_DEBUG("BumpTestController: BumpTestController()");

    ForceActuatorBumpTestStatus::instance().reset();

    auto test_timeout = steady_clock::now() - seconds(10);

    for (int i = 0; i < FA_COUNT; i++) {
        _test_timeout[i] = test_timeout;
    }
}

BumpTestController::~BumpTestController() { delete _bump_test_data; }

int BumpTestController::setBumpTestActuator(int actuator_id, bool cylinders, bool test_primary,
                                            bool test_secondary) {
    _test_settle_time =
            milliseconds(static_cast<int>(ForceActuatorSettings::instance().bumpTestSettleTime * 1000));

    SettingReader::instance().getSafetyControllerSettings()->ForceController.enterBumpTesting();

    auto& faa_settings = ForceActuatorApplicationSettings::instance();
    auto z_index = faa_settings.ActuatorIdToZIndex(actuator_id);

    _test_start[z_index] = steady_clock::now() + _test_settle_time;
    _test_timeout[z_index] = _test_start[z_index];
    _cylinders[z_index] = cylinders;

    if (test_primary) {
        _final_primary_states[z_index] = MTM1M3_shared_BumpTest_Passed;
    }

    if (test_secondary) {
        auto s_index = faa_settings.ZIndexToSecondaryCylinderIndex[z_index];
        if (s_index < 0) {
            throw std::runtime_error(fmt::format("Actuator {} ({}) isn't dual axis", actuator_id, z_index));
        }
        _final_secondary_states[s_index] = MTM1M3_shared_BumpTest_Passed;
    }

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

    auto& actuator_status = ForceActuatorBumpTestStatus::instance();
    auto& faa_settings = ForceActuatorApplicationSettings::instance();

    _collect_results();

    _bump_test_data->test_mirror(MTM1M3::MTM1M3_shared_BumpTestType_Primary, primary_status);
    _bump_test_data->test_mirror(MTM1M3::MTM1M3_shared_BumpTestType_Secondary, secondary_status);

    _bump_test_data->test_mirror(MTM1M3::MTM1M3_shared_BumpTestType_X, x_status);
    _bump_test_data->test_mirror(MTM1M3::MTM1M3_shared_BumpTestType_Y, y_status);
    _bump_test_data->test_mirror(MTM1M3::MTM1M3_shared_BumpTestType_Z, z_status);

    for (int z_index = 0; z_index < FA_COUNT; z_index++) {
        int actuator_id = faa_settings.Table[z_index].ActuatorID;
        int s_index = faa_settings.ZIndexToSecondaryCylinderIndex[z_index];

        if (actuator_status.primary_tested(z_index) == true) {
            bool changed = false;
            if (_cylinders[z_index]) {
                changed = _run_axis(z_index, z_index, s_index, actuator_id,
                                    MTM1M3::MTM1M3_shared_BumpTestType_Primary, primary_status[z_index],
                                    actuator_status.primaryTest[z_index],
                                    actuator_status.primaryTestTimestamps[z_index]);
            } else {
                changed = _run_axis(z_index, z_index, s_index, actuator_id,
                                    MTM1M3::MTM1M3_shared_BumpTestType_Z, z_status[z_index],
                                    actuator_status.primaryTest[z_index],
                                    actuator_status.primaryTestTimestamps[z_index]);
            }
            tested_count++;
            if (changed) {
                status_change++;
            }
        }

        if (s_index >= 0) {
            if (actuator_status.primary_tested(z_index) == false &&
                actuator_status.secondary_tested(s_index) == true) {
                bool changed = false;
                if (_cylinders[z_index]) {
                    changed = _run_axis(s_index, z_index, s_index, actuator_id,
                                        MTM1M3::MTM1M3_shared_BumpTestType_Secondary,
                                        secondary_status[z_index], actuator_status.secondaryTest[s_index],
                                        actuator_status.secondaryTestTimestamps[s_index]);
                } else {
                    int x_index = faa_settings.ZIndexToXIndex[z_index];
                    if (x_index >= 0) {
                        changed = _run_axis(x_index, z_index, s_index, actuator_id,
                                            MTM1M3::MTM1M3_shared_BumpTestType_X, x_status[z_index],
                                            actuator_status.secondaryTest[s_index],
                                            actuator_status.secondaryTestTimestamps[s_index]);
                    } else {
                        int y_index = faa_settings.ZIndexToYIndex[z_index];
                        changed = _run_axis(y_index, z_index, s_index, actuator_id,
                                            MTM1M3::MTM1M3_shared_BumpTestType_Y, y_status[z_index],
                                            actuator_status.secondaryTest[s_index],
                                            actuator_status.secondaryTestTimestamps[s_index]);
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
        auto now = steady_clock::now();
        bool call_exit = true;

        for (int i = 0; i < FA_COUNT; i++) {
            if (_test_timeout[i] > now) {
                call_exit = false;
                break;
            }
        }

        if (call_exit == true) {
            SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();
        }
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

int status_to_sal(BumpTestStatus status, const BumpTestStatistics& stat, int stage) {
    switch (status) {
        case BumpTestStatus::PASSED:
        case BumpTestStatus::OVERSHOOT_WARNING:
        case BumpTestStatus::UNDERSHOOT_WARNING:
        case BumpTestStatus::RMS_WARNING:
            return MTM1M3::MTM1M3_shared_BumpTest_Passed;
        case BumpTestStatus::OVERSHOOT_ERROR:
            switch (stage) {
                case MTM1M3::MTM1M3_shared_BumpTest_Triggered:
                case MTM1M3::MTM1M3_shared_BumpTest_TestingPositive:
                case MTM1M3::MTM1M3_shared_BumpTest_TestingPositiveWait:
                    return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedPositive_Overshoot;
                default:
                    return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedNegative_Overshoot;
            }
        case BumpTestStatus::UNDERSHOOT_ERROR:
            switch (stage) {
                case MTM1M3::MTM1M3_shared_BumpTest_Triggered:
                case MTM1M3::MTM1M3_shared_BumpTest_TestingPositive:
                case MTM1M3::MTM1M3_shared_BumpTest_TestingPositiveWait:
                    return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedPositive_Undershoot;
                default:
                    return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedNegative_Undershoot;
            }
        case BumpTestStatus::RMS_ERROR:
            if ((stat.average - stat.rms_baseline) >= 0) {
                switch (stage) {
                    case MTM1M3::MTM1M3_shared_BumpTest_Triggered:
                    case MTM1M3::MTM1M3_shared_BumpTest_TestingPositive:
                    case MTM1M3::MTM1M3_shared_BumpTest_TestingPositiveWait:
                        return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedPositive_Overshoot;
                    default:
                        return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedNegative_Overshoot;
                }
            } else {
                switch (stage) {
                    case MTM1M3::MTM1M3_shared_BumpTest_Triggered:
                    case MTM1M3::MTM1M3_shared_BumpTest_TestingPositive:
                    case MTM1M3::MTM1M3_shared_BumpTest_TestingPositiveWait:
                        return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedPositive_Undershoot;
                    default:
                        return MTM1M3::MTM1M3_shared_BumpTest_Failed_TestedNegative_Undershoot;
                }
            }
        default:
            return MTM1M3::MTM1M3_shared_BumpTest_Failed_Timeout;
    }
}

void BumpTestController::_finalize_test(int actuator_id, int z_index, int s_index, int test_type, int stage,
                                        const BumpTestStatistics& stat, steady_clock::time_point now,
                                        BumpTestStatus status) {
    if (status == BumpTestStatus::PASSED) {
        M1M3SSPublisher::instance().logForceActuatorBumpTestStatistics(
                actuator_id, test_type, stage,
                std::chrono::duration<float>(now - _test_start[z_index]).count(), stat);
    } else {
        if (FABumpTestData::is_primary(test_type)) {
            _final_primary_states[z_index] = status_to_sal(status, stat, stage);
        } else {
            _final_secondary_states[s_index] = status_to_sal(status, stat, stage);
        }

        M1M3SSPublisher::instance().logForceActuatorBumpTestStatistics(actuator_id, test_type, stage, NAN,
                                                                       stat);
    }

    int failed_count = 0;

    auto& faa_settings = ForceActuatorApplicationSettings::instance();

    bool test_cylinder = false;

    switch (test_type) {
        case MTM1M3::MTM1M3_shared_BumpTestType_Primary:
        case MTM1M3::MTM1M3_shared_BumpTestType_Secondary:
            test_cylinder = true;
        default:
            break;
    }

    for (int acc = 0; acc < FA_COUNT; acc++) {
        // don't look for problems on FAs being tested
        if (_test_timeout[acc] > now) continue;

        if (acc != z_index || FABumpTestData::is_primary(test_type) == false) {
            auto p_state = test_cylinder ? primary_status[acc] : z_status[acc];
            switch (p_state) {
                case BumpTestStatus::OVERSHOOT_ERROR:
                case BumpTestStatus::UNDERSHOOT_ERROR:
                case BumpTestStatus::RMS_ERROR: {
                    auto failed_id = faa_settings.ZIndexToActuatorId(acc);
                    M1M3SSPublisher::instance().logForceActuatorBumpTestStatistics(
                            actuator_id, test_type, test_cylinder ? failed_id : 4000 + failed_id, NAN,
                            _bump_test_data->cached_statistics(
                                    acc, test_cylinder ? MTM1M3::MTM1M3_shared_BumpTestType_Primary
                                                       : MTM1M3::MTM1M3_shared_BumpTestType_Z));
                    failed_count++;
                }
                default:
                    break;
            }
        }

        auto s_index = faa_settings.ZIndexToSecondaryCylinderIndex[acc];
        if (s_index >= 0 && (acc != z_index || FABumpTestData::is_primary(test_type) == true)) {
            BumpTestStatus s_state;

            int s_test_type = MTM1M3::MTM1M3_shared_BumpTestType_Secondary;
            int s_offset = 1000;

            if (test_cylinder) {
                s_state = secondary_status[acc];
            } else {
                switch (test_type) {
                    case MTM1M3::MTM1M3_shared_BumpTestType_X:
                        s_state = x_status[acc];
                        s_test_type = MTM1M3::MTM1M3_shared_BumpTestType_X;
                        s_offset = 2000;
                        break;
                    case MTM1M3::MTM1M3_shared_BumpTestType_Y:
                        s_state = y_status[acc];
                        s_test_type = MTM1M3::MTM1M3_shared_BumpTestType_Y;
                        s_offset = 3000;
                        break;
                    case MTM1M3::MTM1M3_shared_BumpTestType_Primary:
                        s_state = secondary_status[acc];
                        break;
                    case MTM1M3::MTM1M3_shared_BumpTestType_Z: {
                        auto x_index = faa_settings.ZIndexToXIndex[acc];
                        if (x_index >= 0) {
                            s_state = x_status[acc];
                            s_test_type = MTM1M3::MTM1M3_shared_BumpTestType_X;
                            s_offset = 2000;
                        } else {
                            auto y_index = faa_settings.ZIndexToYIndex[acc];
                            if (y_index < 0) {
                                SPDLOG_ERROR(
                                        "Expected either X or Y forces when looking for issues in "
                                        "non-cylinder test - z_index {} s_index {} x_index {} y_index {}.",
                                        acc, s_index, x_index, y_index);
                                continue;
                            }
                            s_state = y_status[acc];
                            s_test_type = MTM1M3::MTM1M3_shared_BumpTestType_Y;
                            s_offset = 3000;
                        };
                    }
                }
            }

            switch (s_state) {
                case BumpTestStatus::OVERSHOOT_ERROR:
                case BumpTestStatus::UNDERSHOOT_ERROR:
                case BumpTestStatus::RMS_ERROR: {
                    auto failed_id = faa_settings.SecondaryCylinderIndexToActuatorId(s_index);
                    M1M3SSPublisher::instance().logForceActuatorBumpTestStatistics(
                            actuator_id, test_type, s_offset + failed_id, NAN,
                            _bump_test_data->cached_statistics(acc, s_test_type));
                    failed_count++;
                }
                default:
                    break;
            }
        }
    }

    if (failed_count > 0) {
        if (FABumpTestData::is_primary(test_type)) {
            if (_final_primary_states[z_index] == MTM1M3::MTM1M3_shared_BumpTest_Passed) {
                _final_primary_states[z_index] = MTM1M3::MTM1M3_shared_BumpTest_Failed_NonTestedProblem;
            }
        } else {
            if (_final_secondary_states[s_index] == MTM1M3::MTM1M3_shared_BumpTest_Passed) {
                _final_secondary_states[s_index] = MTM1M3::MTM1M3_shared_BumpTest_Failed_NonTestedProblem;
            }
        }
    }
}

bool BumpTestController::_run_axis(int axis_index, int z_index, int s_index, int actuator_id, int test_type,
                                   BumpTestStatus status, int& stage, double& timestamp) {
    // runs after statistics is updated in runLoop. So all FA data are current.
    ForceController* force_controller = Model::instance().getForceController();

    auto now = steady_clock::now();

    bool positive = false;

    bool timed_out = false;

    BumpTestStatistics stat;

    int i = -1;

    try {
        stat = _bump_test_data->cached_statistics(z_index, test_type);
        if (_test_timeout[z_index] <= now) {
            SPDLOG_WARN(
                    "BumpTest: Timeout of actuator {} - {} (axis index {}, Z index {}, s index {}) - min "
                    "{:.2f} "
                    "max {:.2f} "
                    "average {:.2f} rms {:.2f}, stage {}. Status {}",
                    actuator_id, int(test_type), axis_index, z_index, s_index, stat.min, stat.max,
                    stat.average, stat.error_rms, stage, int(status));

            _finalize_test(actuator_id, z_index, s_index, test_type, stage, stat, now, status);

            timed_out = true;
        }
    } catch (std::out_of_range& ex) {
        if (_test_timeout[z_index] <= now) {
            SPDLOG_ERROR(
                    "Timeout when testing actuator {} - {} (axis index {}, z index {}, s index {}, i {}) - "
                    "cannot "
                    "retrieve cache.",
                    actuator_id, int(test_type), axis_index, z_index, s_index, i);

            M1M3SSPublisher::instance().logForceActuatorBumpTestStatistics(actuator_id, test_type, stage, NAN,
                                                                           stat);

            timed_out = true;
        }
    }

    char axis;
    switch (test_type) {
        case MTM1M3::MTM1M3_shared_BumpTestType_Primary:
            axis = 'P';
            break;
        case MTM1M3::MTM1M3_shared_BumpTestType_Secondary:
            axis = 'S';
            break;
        case MTM1M3::MTM1M3_shared_BumpTestType_X:
            axis = 'X';
            break;
        case MTM1M3::MTM1M3_shared_BumpTestType_Y:
            axis = 'Y';
            break;
        case MTM1M3::MTM1M3_shared_BumpTestType_Z:
            axis = 'Z';
            break;
        default:
            throw std::runtime_error(fmt::format("Unknow test type: {}", test_type));
    }

    switch (stage) {
        case MTM1M3_shared_BumpTest_Triggered:
        case MTM1M3_shared_BumpTest_TestingPositiveWait:
        case MTM1M3_shared_BumpTest_TestingNegativeWait:
            if (status == BumpTestStatus::PASSED) {
                _finalize_test(actuator_id, z_index, s_index, test_type, stage, stat, now, status);
            } else if (timed_out == false) {
                return false;
            }

            switch (stage) {
                case MTM1M3_shared_BumpTest_Triggered:
                    force_controller->applyActuatorOffset(axis, axis_index, _test_force);
                    stage = MTM1M3_shared_BumpTest_TestingPositive;
                    break;
                case MTM1M3_shared_BumpTest_TestingPositiveWait:
                    force_controller->applyActuatorOffset(axis, axis_index, -_test_force);
                    stage = MTM1M3_shared_BumpTest_TestingNegative;
                    break;
                case MTM1M3_shared_BumpTest_TestingNegativeWait:
                    force_controller->applyActuatorOffset(axis, axis_index, 0);

                    if (FABumpTestData::is_primary(test_type)) {
                        stage = _final_primary_states[z_index];
                    } else {
                        stage = _final_secondary_states[s_index];
                    }

                    if (stage == MTM1M3_shared_BumpTest_Passed) {
                        SPDLOG_INFO("Passed FA ID {} ({}{} {}) bump test", actuator_id, char(axis),
                                    axis_index, s_index);
                    } else {
                        SPDLOG_WARN("Failed FA ID {} ({}{} {}) bump test: {}", actuator_id, char(axis),
                                    axis_index, s_index, stage);
                    }
                    break;
            }

            force_controller->processAppliedForces();

            _test_start[z_index] = now;
            _test_timeout[z_index] = now + _test_settle_time;

            return true;

        case MTM1M3_shared_BumpTest_TestingPositive:
            positive = true;
        case MTM1M3_shared_BumpTest_TestingNegative:
            if (status == BumpTestStatus::PASSED) {
                _finalize_test(actuator_id, z_index, s_index, test_type, stage, stat, now, status);
            } else if (timed_out == false) {
                return false;
            }

            force_controller->applyActuatorOffset(axis, axis_index, 0);

            force_controller->processAppliedForces();

            stage = positive ? MTM1M3_shared_BumpTest_TestingPositiveWait
                             : MTM1M3_shared_BumpTest_TestingNegativeWait;
            _test_start[z_index] = now;
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

        // make sure exitBumpTesting will be called after FA gets back to ~proper
        // values
        //
        // Drop of the measuredForce on top of FAs can take some time, as it isn't
        // instantinous due to physics involved
    }
}

void BumpTestController::_collect_results() {
    auto& fa_data = ForceActuatorData::instance();
    auto& fa_status = ForceActuatorBumpTestStatus::instance();

    _bump_test_data->add_data(fa_data.xForce, fa_data.yForce, fa_data.zForce, fa_data.primaryCylinderForce,
                              fa_data.secondaryCylinderForce, fa_status.primaryTest, fa_status.secondaryTest);
}
