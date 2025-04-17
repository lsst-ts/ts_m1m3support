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

#ifndef LSST_FORCEACTUATORBUMPTESTSTATUS_H
#define LSST_FORCEACTUATORBUMPTESTSTATUS_H

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/***
 * Control force actuator bump tests, provides methods to manipulate the bump test.
 */
class ForceActuatorBumpTestStatus : public MTM1M3_logevent_forceActuatorBumpTestStatusC,
                                    public cRIO::Singleton<ForceActuatorBumpTestStatus> {
public:
    ForceActuatorBumpTestStatus(token);

    void reset(bool send_data = true);

    /**
     * Returns minimal distance to other currently tested force actuator.
     *
     * @param actuator_id
     * @param min_actuator_id
     *
     * @return minimal distance from any currently tested force actuator
     */
    float minimal_tested_distance(int actuator_id, int &min_actuator_id);

    void trigger_bump_test(int z_index, bool test_primary, bool test_secondary);

    /**
     * Return true if the passed status indicate the force actuator is being bump
     * tested.
     *
     * @param status the force actuator bump test status
     *
     * @return true if the force actuator is being bump tested
     */
    static bool is_tested(int status);

    bool primary_tested(int actuator_index) { return is_tested(primaryTest[actuator_index]); }

    bool secondary_tested(int actuator_index) { return is_tested(secondaryTest[actuator_index]); }

    /**
     * Is any force actuator bump test in progress?
     *
     * @return true if bump tests is in progress
     */
    bool test_in_progress();

    /**
     * Stops all tests - put NotTested as state and send updated message if any
     * force actuator state was changed.
     */
    void stop_all();

    /**
     * Sends updates through SAL.
     */
    void send(bool force = false);

private:
    bool _should_send;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // !LSST_FORCEACTUATORBUMPTESTSTATUS_H
