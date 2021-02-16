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

#ifndef __LSST_UNITS_H__
#define __LSST_UNITS_H__

#include <cmath>

namespace LSST {
namespace M1M3 {
namespace SS {

const static double D2RAD = M_PI / 180.0;
const static double RAD2D = 180.0 / M_PI;
const static double M2MM = 1000.0;
const static double MM2M = 1.0 / 1000.0;

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
#endif  //!__LSST_UNITS_H__
