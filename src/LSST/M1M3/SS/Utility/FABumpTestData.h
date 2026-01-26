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

#include <map>
#include <math.h>
#include <mutex>

#include <SAL_MTM1M3.h>

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
    OVERSHOOT_ERROR,
    RMS_ERROR,
    RMS_WARNING
};

typedef std::vector<float> float_v;
typedef std::vector<int> int_v;

/**
 * Stores statistics of data acquired during bump test.
 */
struct BumpTestStatistics {
    BumpTestStatistics();

    // signal minimal value
    float min;
    // signal maximal value
    float max;
    // signal average/mean value
    float average;
    // Error RMS
    float error_rms;
    // expected force value - baseline for RMS calculation
    float rms_baseline;
};

/**
 * Collection of statistics entries. Organized as map of BumpTestStatistics,
 * where index is FA index (z_index).
 */
struct FABumpTestStatistics {
    FABumpTestStatistics();

    std::map<int, BumpTestStatistics> statistics;

    /**
     * Clear all cached statistics.
     */
    void clear();
};

/**
 * Keep track of forces measured during force actuators bump tests. Provides
 * functions to check that force actuator tested fine.
 */
class FABumpTestData {
public:
    FABumpTestData(size_t capacity);
    virtual ~FABumpTestData();

    /**
     * Add new data. Data are the same as send out in measuredForces message.
     * Updates caches used to calculate statistics of the bump tests. All input
     * data are arrays, indexed by appropriate axis.
     *
     * @param x_forces Measured X forces (calculated from cylinder forces).
     * @param y_forces Measured Y forces (calculated from cylinder forces).
     * @param z_forces Measured Z forces (calculated from cylinder forces).
     * @param primary_forces Measured primary cylinder forces (taken from load cells).
     * @param secondary_forces Measured secondary cylinder forces (taken from load cells).
     * @param primary_states Primary cylinder/axis bump test states.
     * @param secondary_states Secondary cylinder/axis bump test states.
     */
    void add_data(const float_v &x_forces, const float_v &y_forces, const float_v &z_forces,
                  const float_v &primary_forces, const float_v &secondary_forces, const int_v &primary_states,
                  const int_v &secondary_states);

    /**
     * Clear all cache entries.
     */
    void clear();

    /**
     * Returns true if there aren't data.
     *
     * @return true if no data were entered into array.
     */
    bool empty() const { return _head == 0 && _filled == false; }

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
     * @param type Test type - P,S or axis (XYZ) forces
     *
     * @return force actuator status
     */
    BumpTestStatus test_actuator(int actuator_id, int test_type);

    /**
     * Test the mirror. Call test_actuator on all actuators.
     *
     * @param test_type Test type - axis - X,Y,Z,P or S
     * @param results tests results, indexed by z_index (not the axis index)
     */
    void test_mirror(int test_type, BumpTestStatus (&results)[FA_COUNT]);

    float *get_data(int axis_index, int test_type);

    static bool is_primary(int test_type) {
        switch (test_type) {
            case MTM1M3::MTM1M3_shared_BumpTestType_Primary:
            case MTM1M3::MTM1M3_shared_BumpTestType_Z:
                return true;
            default:
                return false;
        }
    }

    static float get_expected_force(int axis_index, int test_type);

    /**
     * Retrieve test statics. Returns cached data if they are available.
     * Otherwise calculate the statistics, add it to the cache, and return it.
     *
     * @param axis_index
     * @param axis
     * @param rms_baseline
     *
     * @return BumpTestStatistics with various statistics computed from cached data or retrieved from cache.
     */
    BumpTestStatistics statistics(int fa_index, int axis_index, int test_type, float rms_baseline = NAN);

    /**
     * Returns cached statistics. Throws std::out_of_range if it isn't available.
     *
     * @param fa_index FA index (0-155)
     * @param test_type Test type (see MTM1M3_shared_BumpTestType_*).
     *
     * @return BumpTestStatistics with various statistics retrieved from cache.
     */
    BumpTestStatistics cached_statistics(int fa_index, int test_type);

    FABumpTestStatistics fa_statistics[FA_COUNT];

private:
    BumpTestStatus _test_rms(int x_index, int y_index, int z_index, int s_index, int test_type, float error,
                             float warning);
    BumpTestStatus _test_min_max(int x_index, int y_index, int z_index, int s_index, int test_type,
                                 float error, float warning);

    float *_x_forces[FA_X_COUNT];
    float *_y_forces[FA_Y_COUNT];
    float *_z_forces[FA_Z_COUNT];

    float *_primary_forces[FA_COUNT];
    float *_secondary_forces[FA_S_COUNT];

    int *_primary_states[FA_COUNT];
    int *_secondary_states[FA_S_COUNT];

    // circular buffer indice
    size_t _head;
    // when true, the buffer is full. New members are added to _head position,
    // all elements contains valid data
    bool _filled;
    size_t _capacity;

    // current test result
    BumpTestStatus _primary_results[FA_COUNT];
    BumpTestStatus _secondary_results[FA_S_COUNT];

    std::mutex _g_mutex;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  //! FABUMPTESTDATA_H_
