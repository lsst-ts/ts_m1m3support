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

#ifndef SLEWCONTROLLER_H_
#define SLEWCONTROLLER_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Provides methods to enter and exit slew configuration.
 */
class SlewController {
public:
    SlewController();

    void enterSlew();
    void exitSlew();

    void reset();

private:
    bool _balanceForcesEnabled;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* SLEWCONTROLLER_H_ */
