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

/**
 * Performs bump test on single force actuator (FA). Bump tests of an actuator
 * is performed by applying a small positive and negative force offsets on
 * stationary (parked) mirror. The tests is evaluated by comparing value
 * reported by the force actuator load cell with the target (offset) value.
 *
 * Settings stored under ForceActuatorSettings/BumpTest specify timeouts,
 * allowable ranges and number of measurements performed. The configuration
 * values are enclosed in the following text with : - for example
 * :TestedTolerances/Warning:
 *
 * Bump test is performed in the following stages (per actuator axis, so if both
 * primary and secondary shall be tested, this is repeated twice per dual axis
 * actuator):
 *
 * 1. check that average of :TestMeasurements: measured forces roughly equal to
 * 0 (within :TestedTolerances/Error: for the FA being tested, and within
 * :NonTestedTolerances/Error: for FAs not tested)
 *
 * 2. apply a small positive force (222 N, _testForce member variable)
 *
 * 3. sleep for :SettleTime: seconds
 *
 * 4. check that the average of :TestMeasurements: force actuator values
 * following the settle time equals (within :TestedTolerances/Error: for tested
 * FA and :NonTestedTolerances/Error: for not tested FAs) equals to the applied
 * (target offset) force
 *
 * 5. null applied offset
 *
 * 6. sleep for :SettleTime: seconds
 *
 * 7. check that the average of :TestMeasurements: force actuator values
 * following the settle time equals (within :TestedTolerances/Error: for tested
 * FA and :NonTestedTolerances/Error: for not tested FAs) equals to the 0
 *
 * 8. apply a small negative force (-222 N, - _testForce member variable)
 *
 * 9. sleep for :SettleTime: seconds
 *
 * 10. check that the average of :TestMeasurements: force actuator values
 * following the settle time equals (within :TestedTolerances/Error: for tested
 * FA and :NonTestedTolerances/Error: for not tested FAs) equals to the 0
 *
 * 11. null applied offset
 *
 * 12. sleep for :SettleTime: seconds
 *
 * 13. check that the average of :TestMeasurements: force actuator values
 * following the settle time equals (within :TestedTolerances/Error: for tested
 * FA and :NonTestedTolerances/Error: for not tested FAs) equals to the 0
 *
 * If any of the steps fails, transition to failed stage, reset to zero all
 * applied offsets and exit the bump test for the given FA.
 *
 * :TestedTolerances/Warning: and :NonTestedTolerances/Warning: are used as
 * warning levels. If the error surpassed those, but doesn't trigger an error
 * (is not above error value), a warning message is send to the system log.
 */
class BumpTestController {
public:
    BumpTestController();

    ~BumpTestController();

    /**
     * Set current bump tests parameters.
     *
     * @param actuatoro_id actuator ID (101-443)
     * @param test_primary true if test primary (Z) actuator
     * @param test_secondary true if test secondary (X or Y) actuator
     *
     * @return 0 on success, 1 if another force actuator is being tested
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
     * @param forced if true, make sure Far Neighbor checks bypass is disabled
     */
    void stopAll(bool forced);

    /**
     * Stops bump test on given cylinder.
     *
     * @param axis cylinder axis (X,Y or Z)
     * @param index actuator index (0..155)
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
    // if NAN, don't sleep
    std::chrono::time_point<std::chrono::steady_clock> _test_start[FA_COUNT];
    std::chrono::time_point<std::chrono::steady_clock> _test_timeout[FA_COUNT];
    bool _cylinders[FA_COUNT];

    FABumpTestData *_bump_test_data;

    void _finalize_test(int actuator_id, int z_index, int s_index, int test_type, int stage,
                        const BumpTestStatistics &stat, std::chrono::steady_clock::time_point now,
                        BumpTestStatus status);

    /**
     * Run tests on cylinder.
     *
     * @param index
     * @param axis
     * @param stage
     * @param timestamp
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
