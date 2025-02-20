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

#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Bump test single force actautor. Keeps record of the bump test progress.
 */
class FABumpTest {
public:
private:
};

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

    /**
     * Set current bump tests parameters.
     *
     * @param actuatorId actuator ID (101-443)
     * @param testPrimary true if test primary (Z) actuator
     * @param testSecondary true if test secondary (X or Y) actuator
     *
     * @return 0 on success, 1 if another force actuator is being tested
     */
    int setBumpTestActuator(int actuatorId, bool testPrimary, bool testSecondary);

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

private:
    int _xIndex;
    int _yIndex;
    int _zIndex;
    int _secondaryIndex;

    float _testForce;

    float _testedWarning;
    float _testedError;
    float _nonTestedWarning;
    float _nonTestedError;

    float _testSettleTime;
    int _testMeasurements;
    int _testProgress;
    // if NAN, don't sleep
    double _sleepUntil;

    typedef enum { FINISHED, FAILED, NO_CHANGE, STATE_CHANGED } runCylinderReturn_t;

    /**
     * Run tests on cylinder.
     *
     * @param axis
     * @param index
     * @param averages
     * @param stage
     */
    runCylinderReturn_t _runCylinder(int actuatorId, char axis, int index, double averages[], int *stage);
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
