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

#ifndef BUMPTESTCONTROLLER_H_
#define BUMPTESTCONTROLLER_H_

#include <chrono>

#include <cRIO/DataTypes.h>

#include "FABumpTestData.h"

namespace LSST {
namespace M1M3 {
namespace SS {

/***
 * Performs force actuator (FA) bump test. Bump tests of an actuator is
 * performed by applying a small positive and negative force offsets on
 * stationary (parked) mirror. The tests is evaluated by comparing value
 * reported by the force actuator load cell with the target (offset) value.
 */
class BumpTestController {
public:
    BumpTestController();

    ~BumpTestController();

    /**
     * Set current bump tests parameters.
     *
     * @param actuatoro_id actuator ID (101-443).
     * @param test_primary true if test primary (Z) actuator.
     * @param test_secondary true if test secondary (X or Y) actuator.
     *
     * @return 0 on success, 1 if another force actuator is being tested.
     */
    int setBumpTestActuator(int actuator_id, bool cylinders, bool test_primary, bool test_secondary);

    /**
     * Run single loop. Shall be called from update command after telemetry
     * data are queried and send.
     */
    void runLoop();

    /**
     * Stops all running bump tests.
     *
     * @param forced if true, make sure Far Neighbor checks bypass is disabled.
     */
    void stopAll(bool forced);

    /**
     * Stops bump test on given cylinder.
     *
     * @param axis cylinder axis (X,Y or Z).
     * @param index actuator index (0..155).
     */
    void stopCylinder(char axis, int index);

    BumpTestStatus primary_status[FA_COUNT];
    BumpTestStatus secondary_status[FA_COUNT];
    BumpTestStatus x_status[FA_COUNT];
    BumpTestStatus y_status[FA_COUNT];
    BumpTestStatus z_status[FA_COUNT];

private:
    float _test_force;

    std::chrono::milliseconds _test_settle_time;
    std::chrono::time_point<std::chrono::steady_clock> _test_start[FA_COUNT];
    std::chrono::time_point<std::chrono::steady_clock> _test_timeout[FA_COUNT];
    bool _cylinders[FA_COUNT];

    FABumpTestData *_bump_test_data;

    /**
     * Called after statistics was collected. Report back interesting (above warning/error level) values.
     *
     * @param actuator_id Actuator ID (101-443).
     * @param z_index Actuator Z/primary index (0-155).
     * @param s_index Actuator secondary index (-1 or 0-112).
     * @param test_type
     * @param stage
     * @param stat
     * @param now Current time.
     * @param status Test result - see BumpTestStatus enumeration.
     */
    void _finalize_test(int actuator_id, int z_index, int s_index, int test_type, int stage,
                        const BumpTestStatistics &stat, std::chrono::steady_clock::time_point now,
                        BumpTestStatus status);

    /**
     * Run test on an axis.
     *
     * @param axis_index Test axis index - X, Y Z, primary (=Z) or secondary index.
     * @param z_index FA Z index (0-156).
     * @param s_index FA secondary index (-1 or 1-112).
     * @param actuator_id Actuator ID (101-443).
     * @param test_type one of BumpTestType - Primary, Secondary, X, Y or Z.
     * @param status see FABumpTestData::BumpTestStatus for allowed values.
     * @param stage Bump tests stage - one of MTM1M3_shared_BumpTest_xxx value.
     * @param timestamp current time.
     */
    bool _run_axis(int axis_index, int z_index, int s_index, int actuator_id, int test_type,
                   BumpTestStatus status, int &stage, double &timestamp);
    void _reset_progress(bool zeroOffsets = true);

    /**
     * Collect data.
     */
    void _collect_results();

    int _final_primary_states[FA_COUNT];
    int _final_secondary_states[FA_S_COUNT];
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* BUMPTESTCONTROLLER_H_ */
