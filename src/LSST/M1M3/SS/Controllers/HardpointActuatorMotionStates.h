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

#ifndef HARDPOINTACTUATORMOTIONSTATES_H_
#define HARDPOINTACTUATORMOTIONSTATES_H_

/**
 * Actuator states. PositionController::updateSteps uses those to control HP
 * movement.
 *
 * @see PositionController::updateSteps
 */
struct HardpointActuatorMotionStates {
    enum Type {
        /// HP is not moving
        Standby = 0,
        /// HP is chasing target to reach active state
        Chasing = 1,
        /// HP is stepping to reach user request
        Stepping = 2,
        /// HP moved with moveToEncoder command. Uses for translating, reference
        /// position etc.
        QuickPositioning = 3,
        /**
         * HP fine positioning. If following error is large enough, commands HP
         * to move.
         */
        FinePositioning = 4,
    };
};

#endif /* HARDPOINTACTUATORMOTIONSTATES_H_ */
