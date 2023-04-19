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

#include <HardpointActuatorApplicationSettings.h>

using namespace LSST::M1M3::SS;

//! [HarpointActuatorTableRow initialization]
HardpointActuatorTableRow HardpointActuatorApplicationSettings::Table[HP_COUNT] = {
        {0, 1, 776.78278, 0.00000, -89.00000, 5, 1, 0, 3},
        {1, 2, 1442.56799, 0.00000, -89.00000, 5, 2, 0, 3},
        {2, 3, 2108.37793, 0.00000, -89.00000, 5, 3, 0, 3},
        {3, 4, 2774.18799, 0.00000, -89.00000, 5, 4, 0, 3},
        {4, 5, 3439.99805, 0.00000, -89.00000, 5, 5, 0, 3},
        {5, 6, 3968.01294, 0.00000, -89.00000, 5, 6, 0, 3}};
//! [HardpointActuatorTableRow initialization]
