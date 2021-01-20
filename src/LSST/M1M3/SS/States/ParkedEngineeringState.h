/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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

#ifndef PARKEDENGINEERINGSTATE_H_
#define PARKEDENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Parked Engineering State. Mirror can be raised, switched into Disabled State
 * or returned to Parked State with Exit Engineering command.
 */
class ParkedEngineeringState : public EngineeringState {
public:
    ParkedEngineeringState();

    virtual States::Type update(UpdateCommand* command) override;
    virtual States::Type raiseM1M3(RaiseM1M3Command* command) override;
    virtual States::Type exitEngineering(ExitEngineeringCommand* command) override;
    virtual States::Type disable(DisableCommand* command) override;
    virtual States::Type forceActuatorBumpTest(ForceActuatorBumpTestCommand* command) override;
    virtual States::Type killForceActuatorBumpTest(KillForceActuatorBumpTestCommand* command) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDENGINEERINGSTATE_H_ */
