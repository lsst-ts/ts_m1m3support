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

#ifndef FABUMPTESTDATA_H_
#define FABUMPTESTDATA_H_

#include <mutex>

#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

enum BumpTestStatus {
    PASSED,
    NO_DATA,
    UNDERSHOOT_ERROR,
    UNDERSHOOT_WARNING,
    OVERSHOOT_WARNING,
    OVERSHOOT_ERROR
};

/**
 * Keep track of forces measured during force actuators bump tests. Provides
 * functions to check that force actuator tested fine.
 */
class FABumpTestData {
public:
    FABumpTestData(size_t capacity);
    virtual void ~FABumpTestData();

    void add_data(float x_forces[FA_X_COUNT], float y_forces[FA_Y_COUNT], float z_forces[FA_Z_COUNT],
                  float primary_forces[FA_COUNT], float secondary_forces[FA_S_COUNT],
                  int primary_states[FA_COUNT], int secondary_states[FA_S_COUNT]);

    /**
     * Returns true if there aren't data.
     *
     * @return true if no data were entered into array.
     */
    bool empty() const { return _head == _tail; }

    /**
     * Returns size of archived data points.
     *
     * @return size of archived data points - in range 0 to capacity.
     */
    size_t size() const;

    /**
     * Confirms given force actuator test fine in capacity period.
     *
     * @param index
     *
     * @return force actuator status
     */
    BumpTestStatus test(int index);

private:
    float* _x_force[FA_X_COUNT];
    float* _y_forces[FA_Y_COUNT];
    float* _z_forces[FA_Z_COUNT];

    float* _primary_forces[FA_COUNT];
    float* _secondary_forces[FA_S_COUNT];

    int* _primary_states[FA_COUNT];
    int* _secondary_states[FA_S_COUNT];

    // circular buffer indices
    size_t _head;
    size_t _tail;
    size_t _capacity;

    std::mutex _g_mutex;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  //! FABUMPTESTDATA_H_
