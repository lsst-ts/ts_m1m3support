/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Telescope & Site Software Systems.
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

#ifndef BOOSTERVALVE_H_
#define BOOSTERVALVE_H_

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Controls autonomous operation of booster valves. The rules for booster
 * valves control depends on M1M3 SS mode (Automatic/Engineering). Booster
 * valves can be controlled either by user boosterValveOn/boosterValveOff
 * commands, or automatically when certain conditions are met.
 *
 * In Automatic mode, only automatic control of the booster valves is allowed.
 * User cannot trigger booster valve on or off. If quick action is needed, user
 * shall trigger Panic, which will fault the mirror and remove air supply from
 * the booster valves.
 *
 * In Engineering mode, any of autonomous triggers (if enabled in
 * configuration) or user action can turn booster valves on. Booster valves
 * will be turn off only if all enabled control (autonomous, user) signal the
 * valve shall be off.
 */
class BoosterValveController {
public:
    BoosterValveController();

    void checkFollowingErrors(bool engineering);
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* BOOSTERVALVE_H_ */
