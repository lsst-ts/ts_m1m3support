/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Telescope & Site Software
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

#include <StateTypes.h>

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
 * 1. test if measured force is roughly equal to 0
 * 2. apply a small positive force
 * 3. wait for measured force to roughly equal to the applied force
 * 4. null applied offset
 * 5. wait for measured force to roughly reach 0
 * 6. apply a small negative force
 * 7. wait for measured force to roughly equal to the applied force
 * 8. null applied offset
 * 9. wait for measured force to roughly reach 0
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

    States::Type runLoop();

private:
    int _zIndex;
    int _secondaryIndex;
    bool _testPrimary;
    bool _testSecondary;

    float _tolerance;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* BUMPTESTCONTROLLER_H_ */
