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

#ifndef ENABLEDACTIVESTATE_H_
#define ENABLEDACTIVESTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Enabled active state. Abstract class implementing commands valid both in
 * ActiveState and ActiveEngineeringState.
 */
class EnabledActiveState : public virtual EnabledState {
public:
    States::Type lowerM1M3(LowerM1M3Command *command) override;
    States::Type applyActiveOpticForces(ApplyActiveOpticForcesCommand *command) override;
    States::Type clearActiveOpticForces(ClearActiveOpticForcesCommand *command) override;

protected:
    virtual States::Type getLoweringState() = 0;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* ENABLEDACTIVESTATE_H_ */
