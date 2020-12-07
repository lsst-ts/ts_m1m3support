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

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Performs bump test on single actuator. Bump tests actuator performance by
 * applying small positive and negative force offsets on stationary (parked)
 * mirror, and compare measured forces from force cell.
 *
 * Bump test is performed in the following stages (per actuator, so if both
 * primary and secondary shall be tested, repeat:
 *
 * 1. check that average of _testMeasurements measured forces roughly equal to 0 (in _tolerance)
 * 2. apply a small positive force
 * 3. sleep for _testSettleTime
 * 4. check that average of _testMeasurements measured forces roughly equal to the applied force (in
 * _tolerance)
 * 5. null applied offset
 * 6. sleep for _testSettleTime
 * 7. check that average of _testMeasurements measured forces roughly equal to 0 (in _tolerance)
 * 8. apply a small negative force
 * 9. sleep for _testSettleTime
 * 10. check that average of _testMeasurements measured forces roughly equal to the applied force (in
 * _tolerance)
 * 11. null applied offset
 * 12. sleep for _testSettleTime
 * 13. check that average of _testMeasurements measured forces roughly equal to 0 (in _tolerance)
 *
 * If any of the steps fails, transition to failed stage and exit.
 */
class BumpTestController {
public:
    BumpTestController();

    /**
     * Set current bump tests parameters.
     *
     * @param actuatorId actuator ID (101-443)
     * @param testPrimary true if test primary (Z) actuator
     * @param testSecondary true if test secondary (X or Y) actuator
     */
    void setBumpTestActuator(int actuatorId, bool testPrimary, bool testSecondary);

    /**
     * Run single loop. Shall be called from update command after telemetry
     * data are queried and send.
     */
    void runLoop();

    /**
     * Stops all running bump tests.
     */
    void stopAll();

    /**
     * Stops bump test on given cylinder.
     *
     * @param axis cylinder axis (X,Y or Z)
     */
    void stopCylinder(char axis);

private:
    int _xIndex;
    int _yIndex;
    int _zIndex;
    int _secondaryIndex;
    bool _testPrimary;
    bool _testSecondary;

    float _testForce;
    float _warning;
    float _tolerance;
    float _testSettleTime;
    int _testMeasurements;
    int _testProgress;
    // if NAN, don't sleep
    double _sleepUntil;

    typedef enum { FINISHED, FAILED, NO_CHANGE, STATE_CHANGED } runCylinderReturn_t;
    runCylinderReturn_t _runCylinder(char axis, int index, double averages[], short int* stage);

    void _resetProgress(bool zeroOffsets = true);
    void _resetAverages();

    double _xAverages[FA_X_COUNT];
    double _yAverages[FA_Y_COUNT];
    double _zAverages[FA_Z_COUNT];

    /**
     * Collect averages
     */
    bool _collectAverages();

    /**
     * @return 0x01 on error, 0x02 on warning.
     */
    int _checkAverages(char axis = ' ', int index = -1, double value = 0);
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* BUMPTESTCONTROLLER_H_ */
