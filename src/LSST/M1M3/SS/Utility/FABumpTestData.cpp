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

#include "FABumpTestData.h"

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

void FABumpTestData::~FABumpTestData() {
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

void FABumpTestData::add_data(float x_forces[FA_X_COUNT], float y_forces[FA_Y_COUNT],
                              float z_forces[FA_Z_COUNT], float primary_forces[FA_COUNT],
                              float secondary_forces[FA_S_COUNT], int primary_states[FA_COUNT],
                              int secondary_states[FA_S_COUNT]) {
    std::lock_guard<std::mutex> lock(_c_mutex);

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
