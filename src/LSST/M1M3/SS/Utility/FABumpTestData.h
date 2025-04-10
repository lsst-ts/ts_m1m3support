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

#include "ForceActuatorApplicationSettings.h"

namespace LSST {
namespace M1M3 {
namespace SS {

enum BumpTestStatus {
    PASSED,
    NO_DATA,
    INVALID_TEST_KIND,
    INVALID_ACTUATOR,
    WRONG_STATE_HISTORY,
    UNDERSHOOT_ERROR,
    UNDERSHOOT_WARNING,
    OVERSHOOT_WARNING,
    OVERSHOOT_ERROR
};

typedef std::vector<float> float_v;
typedef std::vector<int> int_v;

/**
 * Keep track of forces measured during force actuators bump tests. Provides
 * functions to check that force actuator tested fine.
 */
class FABumpTestData {
public:
    FABumpTestData(size_t capacity);
    virtual ~FABumpTestData();

    void add_data(const float_v &x_forces, const float_v &y_forces, const float_v &z_forces,
                  const float_v &primary_forces, const float_v &secondary_forces, const int_v &primary_states,
                  const int_v &secondary_states);

    void clear() {
        _head = 0;
        _tail = 0;
    }

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
     * @param actuator_id Actuator to test.
     * @param kind Test kind - P,S or axis (XYZ) forces
     * @param expected_force Force expected to be measured by the force actuator
     * @param error allowed error margin
     * @param warning allowed warning margin
     *
     * @return force actuator status
     */
    BumpTestStatus test_actuator(int actuator_id, char kind, float expected_force, float error,
                                 float warning);

    /**
     * Test the mirror. Call test_actuator on all actuators.
     *
     * @param kind axis - X,Y,Z,P or S
     * @param results tests results, indexed by z_index (not the axis index)
     */
    void test_mirror(char kind, BumpTestStatus (&results)[FA_COUNT]);

    /**
     * Retrieve test statics.
     *
     * @param axis_index
     * @param axis
     * @param min
     * @param max
     * @param average
     */
    void statistics(int axis_index, char axis, float &min, float &max, float &average);

private:
    float *_x_forces[FA_X_COUNT];
    float *_y_forces[FA_Y_COUNT];
    float *_z_forces[FA_Z_COUNT];

    float *_primary_forces[FA_COUNT];
    float *_secondary_forces[FA_S_COUNT];

    int *_primary_states[FA_COUNT];
    int *_secondary_states[FA_S_COUNT];

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
