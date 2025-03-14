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

#include <stdexcept>

#include <SAL_MTM1M3.h>

#include <cRIO/DataTypes.h>

#include "FABumpTestData.h"
#include "SettingReader.h"

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
    return _capacity - (_tail - _head);
}

BumpTestStatus FABumpTestData::test_actuator(int actuator_id, char kind, float expected_force, float error,
                                             float warning) {
    if (size() != _capacity) {
        return BumpTestStatus::NO_DATA;
    }

    auto fa_app_settings = SettingReader::instance().getForceActuatorApplicationSettings();
    auto z_index = fa_app_settings->ActuatorIdToZIndex(actuator_id);
    auto y_index = fa_app_settings->ZIndexToYIndex[z_index];
    auto x_index = fa_app_settings->ZIndexToXIndex[z_index];
    auto s_index = fa_app_settings->ZIndexToSecondaryCylinderIndex[z_index];

    // make sure the actuator state remain the same throughout testing period
    auto p_states = _primary_states[z_index][0];
    for (size_t i = 1; i < _capacity; i++) {
        if (_primary_states[z_index][i] != p_states) {
            return BumpTestStatus::WRONG_STATE_HISTORY;
        }
    }
    if (s_index != -1) {
        auto s_states = _secondary_states[s_index][0];
        for (size_t i = 1; i < _capacity; i++) {
            if (_secondary_states[z_index][i] != s_states) {
                return BumpTestStatus::WRONG_STATE_HISTORY;
            }
        }
    }

    auto in_range = [error, warning](float force, float expected_force) -> BumpTestStatus {
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
    };

    for (size_t i = 0; i < _capacity; i++) {
        BumpTestStatus p_state = BumpTestStatus::PASSED;
        BumpTestStatus s_state = BumpTestStatus::PASSED;
        switch (kind) {
            case 'P':
                p_state = in_range(_primary_forces[z_index][i], expected_force);
                if (s_index != -1) {
                    s_state = in_range(_secondary_forces[z_index][i], 0);
                }
                break;
            case 'S':
                p_state = in_range(_primary_forces[z_index][i], 0);
                s_state = in_range(_secondary_forces[s_index][i], expected_force);
                break;
            case 'x':
                s_state = in_range(_x_forces[x_index][i], expected_force);
                p_state = in_range(_z_forces[z_index][i], 0);
                break;
            case 'Y':
                s_state = in_range(_y_forces[y_index][i], expected_force);
                p_state = in_range(_z_forces[z_index][i], 0);
                break;
            case 'Z':
                p_state = in_range(_z_forces[z_index][i], expected_force);

                if (x_index != -1) {
                    s_state = in_range(_x_forces[x_index][i], 0);
                } else if (y_index != -1) {
                    s_state = in_range(_y_forces[y_index][i], 0);
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
