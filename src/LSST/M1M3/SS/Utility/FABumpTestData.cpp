/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <cmath>
#include <limits>
#include <stdexcept>

#include <SAL_MTM1M3.h>

#include <cRIO/DataTypes.h>

#include "FABumpTestData.h"
#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorSettings.h"
#include "M1M3SSPublisher.h"

using namespace LSST::M1M3::SS;

FABumpTestData::FABumpTestData(size_t capacity) {
    _head = 0;
    _tail = 0;
    _capacity = capacity;

    for (size_t c = 0; c < FA_COUNT; c++) {
        if (c < FA_X_COUNT) {
            _x_forces[c] = new float[_capacity];
        }
        if (c < FA_Y_COUNT) {
            _y_forces[c] = new float[_capacity];
        }
        _z_forces[c] = new float[_capacity];

        _primary_forces[c] = new float[_capacity];
        _primary_states[c] = new int[_capacity];
        if (c < FA_S_COUNT) {
            _secondary_forces[c] = new float[_capacity];
            _secondary_states[c] = new int[_capacity];
        }
    }
}

FABumpTestData::~FABumpTestData() {
    for (size_t c = 0; c < FA_COUNT; c++) {
        if (c < FA_X_COUNT) {
            delete[] _x_forces[c];
        }
        if (c < FA_Y_COUNT) {
            delete[] _y_forces[c];
        }
        delete[] _z_forces[c];

        delete[] _primary_forces[c];
        delete[] _primary_states[c];
        if (c < FA_S_COUNT) {
            delete[] _secondary_forces[c];
            delete[] _secondary_states[c];
        }
    }
}

void FABumpTestData::add_data(const float_v &x_forces, const float_v &y_forces, const float_v &z_forces,
                              const float_v &primary_forces, const float_v &secondary_forces,
                              const int_v &primary_states, const int_v &secondary_states) {
    std::lock_guard<std::mutex> lock(_g_mutex);

    for (size_t c = 0; c < FA_COUNT; c++) {
        if (c < FA_X_COUNT) {
            _x_forces[c][_head] = x_forces[c];
        }
        if (c < FA_Y_COUNT) {
            _y_forces[c][_head] = y_forces[c];
        }
        _z_forces[c][_head] = z_forces[c];

        _primary_forces[c][_head] = primary_forces[c];
        _primary_states[c][_head] = primary_states[c];
        if (c < FA_S_COUNT) {
            _secondary_forces[c][_head] = secondary_forces[c];
            _secondary_states[c][_head] = secondary_states[c];
        }
    }
    _head = (_head + 1) % _capacity;
    if (_head == _tail) {
        _tail = (_tail + 1) % _capacity;
    }
}

size_t FABumpTestData::size() const {
    if (_head >= _tail) {
        return _head - _tail;
    }
    return 1 + _capacity - (_tail - _head);
}

BumpTestStatus FABumpTestData::test_actuator(int z_index, BumpTestKind kind, float expected_force,
                                             float error, float warning) {
    if (size() != _capacity) {
        return BumpTestStatus::NO_DATA;
    }

    auto &faa_settings = ForceActuatorApplicationSettings::instance();
    auto y_index = faa_settings.ZIndexToYIndex[z_index];
    auto x_index = faa_settings.ZIndexToXIndex[z_index];
    auto s_index = faa_settings.ZIndexToSecondaryCylinderIndex[z_index];

    // make sure the actuator state remain the same throughout testing period
    auto p_states = _primary_states[z_index][0];
    for (size_t i = 0; i < _capacity; i++) {
        if (_primary_states[z_index][i] != p_states) {
            return BumpTestStatus::WRONG_STATE_HISTORY;
        }
    }
    if (s_index != -1) {
        auto s_states = _secondary_states[s_index][0];
        for (size_t i = 0; i < _capacity; i++) {
            if (_secondary_states[s_index][i] != s_states) {
                return BumpTestStatus::WRONG_STATE_HISTORY;
            }
        }
    }

    return _test_rms(x_index, y_index, z_index, s_index, kind, expected_force, error, warning);
    // return _test_min_max(x_index, y_index, z_index, s_index, kind, expected_force, error, warning);
}

void FABumpTestData::test_mirror(BumpTestKind kind, BumpTestStatus (&results)[FA_COUNT]) {
    auto &tested_tolerance = ForceActuatorSettings::instance().TestedTolerances;

    for (int i = 0; i < FA_COUNT; i++) {
        results[i] = test_actuator(i, kind, NAN, tested_tolerance.error, tested_tolerance.warning);
    }
}

float *FABumpTestData::get_data(int axis_index, BumpTestKind axis) {
    switch (axis) {
        case BumpTestKind::AXIS_X:
            return _x_forces[axis_index];
        case BumpTestKind::AXIS_Y:
            return _y_forces[axis_index];
        case BumpTestKind::AXIS_Z:
            return _z_forces[axis_index];
        case BumpTestKind::PRIMARY:
            return _primary_forces[axis_index];
        case BumpTestKind::SECONDARY:
            return _secondary_forces[axis_index];
        default:
            throw std::runtime_error("Invalid axis kind");
    };
}

void FABumpTestData::statistics(int axis_index, BumpTestKind axis, float rms_baseline, float &min, float &max,
                                float &average, float &rms) {
    float *data = get_data(axis_index, axis);

    if (size() == 0) {
        throw std::runtime_error("Cannot compute statistics of empty data.");
    }

    if (isnan(rms_baseline)) {
        rms_baseline = get_expected_force(axis_index, axis);
    }

    min = max = average = data[_head];

    rms = data[_head] - rms_baseline;
    rms = rms * rms;

    size_t count = 0;

    for (size_t i = _tail; i != _head; i = ((i + 1) % _capacity), count++) {
        float v = data[i];
        if (v < min) {
            min = v;
        }
        if (v > max) {
            max = v;
        }
        average += v;

        float v_b = (v - rms_baseline);
        rms += v_b * v_b;
    }

    average /= count;

    rms = sqrt(rms / count);
}

BumpTestStatus in_range(float force, float expected_force, float error, float warning) {
    if (force > (expected_force + error)) {
        return BumpTestStatus::OVERSHOOT_ERROR;
    }
    if (force > (expected_force + warning)) {
        return BumpTestStatus::OVERSHOOT_WARNING;
    }
    if (force < (expected_force - error)) {
        return BumpTestStatus::UNDERSHOOT_ERROR;
    }
    if (force < (expected_force - warning)) {
        return BumpTestStatus::UNDERSHOOT_WARNING;
    }
    return BumpTestStatus::PASSED;
}

float FABumpTestData::get_expected_force(int axis_index, BumpTestKind axis) {
    auto &pub = M1M3SSPublisher::instance();
    switch (axis) {
        case BumpTestKind::PRIMARY:
            return pub.getAppliedCylinderForces()->primaryCylinderForces[axis_index];
        case BumpTestKind::SECONDARY:
            return pub.getAppliedCylinderForces()->secondaryCylinderForces[axis_index];
        case AXIS_X:
            return pub.getEventAppliedOffsetForces()->xForces[axis_index];
        case AXIS_Y:
            return pub.getEventAppliedOffsetForces()->yForces[axis_index];
        case AXIS_Z:
            return pub.getEventAppliedOffsetForces()->zForces[axis_index];
    }
    return 0;
}

BumpTestStatus FABumpTestData::_test_rms(int x_index, int y_index, int z_index, int s_index,
                                         BumpTestKind kind, float expected_force, float error,
                                         float warning) {
    float min, max, average, rms;

    BumpTestStatus p_state = BumpTestStatus::PASSED;
    BumpTestStatus s_state = BumpTestStatus::PASSED;

    switch (kind) {
        case BumpTestKind::PRIMARY:
            if (isnan(expected_force)) {
                expected_force = get_expected_force(z_index, kind);
            }
            statistics(z_index, BumpTestKind::PRIMARY, expected_force, min, max, average, rms);
            p_state = in_range(rms, 0, error, warning);

            if (s_index != -1) {
                statistics(s_index, BumpTestKind::SECONDARY, 0, min, max, average, rms);
                s_state = in_range(rms, 0, error, warning);
            }
            break;
        case BumpTestKind::SECONDARY:
            if (s_index == -1) {
                return BumpTestStatus::INVALID_ACTUATOR;
            }
            if (isnan(expected_force)) {
                expected_force = get_expected_force(s_index, kind);
            }
            statistics(z_index, BumpTestKind::PRIMARY, 0, min, max, average, rms);
            p_state = in_range(rms, 0, error, warning);

            statistics(s_index, BumpTestKind::SECONDARY, expected_force, min, max, average, rms);
            s_state = in_range(rms, 0, error, warning);
            break;
        case BumpTestKind::AXIS_X:
            if (x_index == -1) {
                return BumpTestStatus::INVALID_ACTUATOR;
            }
            if (isnan(expected_force)) {
                expected_force = get_expected_force(x_index, kind);
            }
            statistics(x_index, BumpTestKind::AXIS_X, expected_force, min, max, average, rms);
            s_state = in_range(rms, 0, error, warning);

            statistics(z_index, BumpTestKind::AXIS_Z, 0, min, max, average, rms);
            s_state = in_range(rms, 0, error, warning);
            break;
        case BumpTestKind::AXIS_Y:
            if (y_index == -1) {
                return BumpTestStatus::INVALID_ACTUATOR;
            }
            if (isnan(expected_force)) {
                expected_force = get_expected_force(y_index, kind);
            }
            statistics(y_index, BumpTestKind::AXIS_Y, expected_force, min, max, average, rms);
            s_state = in_range(rms, 0, error, warning);

            statistics(z_index, BumpTestKind::AXIS_Z, 0, min, max, average, rms);
            p_state = in_range(rms, 0, error, warning);
            break;
        case BumpTestKind::AXIS_Z:
            if (isnan(expected_force)) {
                expected_force = get_expected_force(z_index, kind);
            }

            statistics(z_index, BumpTestKind::AXIS_Z, expected_force, min, max, average, rms);
            p_state = in_range(rms, 0, error, warning);

            if (x_index != -1) {
                statistics(x_index, BumpTestKind::AXIS_X, 0, min, max, average, rms);
                s_state = in_range(rms, 0, error, warning);
            } else if (y_index != -1) {
                statistics(y_index, BumpTestKind::AXIS_Y, 0, min, max, average, rms);
                s_state = in_range(rms, 0, error, warning);
            }
            break;
        default:
            return BumpTestStatus::INVALID_TEST_KIND;
    }

    if (p_state != BumpTestStatus::PASSED) {
        return p_state;
    }
    if (s_state != BumpTestStatus::PASSED) {
        return s_state;
    }

    return BumpTestStatus::PASSED;
}

BumpTestStatus FABumpTestData::_test_min_max(int x_index, int y_index, int z_index, int s_index,
                                             BumpTestKind kind, float expected_force, float error,
                                             float warning) {
    for (size_t i = 0; i < _capacity; i++) {
        BumpTestStatus p_state = BumpTestStatus::PASSED;
        BumpTestStatus s_state = BumpTestStatus::PASSED;
        switch (kind) {
            case BumpTestKind::PRIMARY:
                if (isnan(expected_force)) {
                    expected_force = M1M3SSPublisher::instance()
                                             .getAppliedCylinderForces()
                                             ->primaryCylinderForces[z_index];
                }
                p_state = in_range(_primary_forces[z_index][i], expected_force, error, warning);
                if (s_index != -1) {
                    s_state = in_range(_secondary_forces[z_index][i], 0, error, warning);
                }
                break;
            case BumpTestKind::SECONDARY:
                if (s_index == -1) {
                    return BumpTestStatus::INVALID_ACTUATOR;
                }
                if (isnan(expected_force)) {
                    expected_force = M1M3SSPublisher::instance()
                                             .getAppliedCylinderForces()
                                             ->secondaryCylinderForces[s_index];
                }
                p_state = in_range(_primary_forces[z_index][i], 0, error, warning);
                s_state = in_range(_secondary_forces[s_index][i], expected_force, error, warning);
                break;
            case BumpTestKind::AXIS_X:
                if (x_index == -1) {
                    return BumpTestStatus::INVALID_ACTUATOR;
                }
                if (isnan(expected_force)) {
                    expected_force =
                            M1M3SSPublisher::instance().getEventAppliedOffsetForces()->xForces[x_index];
                }
                s_state = in_range(_x_forces[x_index][i], expected_force, error, warning);
                p_state = in_range(_z_forces[z_index][i], 0, error, warning);
                break;
            case BumpTestKind::AXIS_Y:
                if (y_index == -1) {
                    return BumpTestStatus::INVALID_ACTUATOR;
                }
                if (isnan(expected_force)) {
                    expected_force =
                            M1M3SSPublisher::instance().getEventAppliedOffsetForces()->yForces[y_index];
                }
                s_state = in_range(_y_forces[y_index][i], expected_force, error, warning);
                p_state = in_range(_z_forces[z_index][i], 0, error, warning);
                break;
            case BumpTestKind::AXIS_Z:
                if (isnan(expected_force)) {
                    expected_force =
                            M1M3SSPublisher::instance().getEventAppliedOffsetForces()->zForces[z_index];
                }

                p_state = in_range(_z_forces[z_index][i], expected_force, error, warning);

                if (x_index != -1) {
                    s_state = in_range(_x_forces[x_index][i], 0, error, warning);
                } else if (y_index != -1) {
                    s_state = in_range(_y_forces[y_index][i], 0, error, warning);
                }
                break;
            default:
                return BumpTestStatus::INVALID_TEST_KIND;
        }
        if (p_state != BumpTestStatus::PASSED) {
            return p_state;
        }
        if (s_state != BumpTestStatus::PASSED) {
            return s_state;
        }
    }

    return BumpTestStatus::PASSED;
}
