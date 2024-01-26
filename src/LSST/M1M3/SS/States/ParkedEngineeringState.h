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

    States::Type update(UpdateCommand *command) override;
    States::Type raiseM1M3(RaiseM1M3Command *command) override;
    States::Type exitEngineering(ExitEngineeringCommand *command) override;
    States::Type disable(DisableCommand *command) override;
    States::Type forceActuatorBumpTest(ForceActuatorBumpTestCommand *command) override;
    States::Type killForceActuatorBumpTest(KillForceActuatorBumpTestCommand *command) override;
    States::Type testHardpoint(TestHardpointCommand *command) override;
    States::Type killHardpointTest(KillHardpointTestCommand *command) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDENGINEERINGSTATE_H_ */
