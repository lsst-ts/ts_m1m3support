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

#ifndef HARDPOINTTESTCONTROLLER_H_
#define HARDPOINTTESTCONTROLLER_H_

#include <SAL_MTM1M3.h>

#include <DataTypes.h>
#include <PositionController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Performs hardpoint test. Hardpoint testing state is stored in inherited
 * MTM1M3_logevent_hardpointTestStatusC.
 */
class HardpointTestController : public MTM1M3_logevent_hardpointTestStatusC {
public:
    HardpointTestController(PositionController* positionController,
                            HardpointActuatorSettings* _hardpointActuatorSettings);

    /**
     * Starts hardpoint test.
     *
     * @param hardpointIndex hardpoint index (0-5)
     *
     * @return 0 on success, 1 on error
     */
    int startHardpointTest(int hardpointIndex);

    /**
     * Run single loop. Shall be called from update command after telemetry
     * data are queried and send.
     */
    void runLoop();

    /**
     * Stops hardpoint test in progress;
     *
     * @param hardpoint_index hardpoint index (0-5), negative to stop all hardpoints
     *
     * @return 0 on success, 1 on error
     */
    int killHardpointTest(int hardpointIndex);

    /**
     * Returns true if given hardpoint is being tested.
     *
     * @param hardpointIndex hardpoint index (0-5)
     *
     * @return true if the hardpoint is being tested
     */
    bool isTested(int hardpointIndex);

private:
    bool _runHardpointLoop(int hardpointIndex);

    PositionController* _positionController;
    HardpointActuatorSettings* _hardpointActuatorSettings;
    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    MTM1M3_logevent_hardpointActuatorStateC* _hardpointActuatorState;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* HARDPOINTTESTCONTROLLER_H_ */
