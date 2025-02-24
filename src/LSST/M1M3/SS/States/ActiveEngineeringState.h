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

#ifndef ACTIVEENGINEERINGSTATE_H_
#define ACTIVEENGINEERINGSTATE_H_

#include <EnabledActiveState.h>
#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Active Engineering state. Similar to Active state, mirror is raised and it's
 * weight is supported by force actuators. This state allows execution of
 * commands to position the mirror and change its shape.
 */
class ActiveEngineeringState : public EngineeringState, public EnabledActiveState {
public:
    ActiveEngineeringState();

    States::Type update(UpdateCommand *command) override;
    States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand *command) override;
    States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand *command) override;
    States::Type setSlewFlag(SetSlewFlagCommand *command) override;
    States::Type clearSlewFlag(ClearSlewFlagCommand *command) override;
    States::Type exitEngineering(ExitEngineeringCommand *command) override;
    States::Type lowerM1M3(LowerM1M3Command *command) override;
    States::Type translateM1M3(TranslateM1M3Command *command) override;
    States::Type positionM1M3(PositionM1M3Command *command) override;
    States::Type runMirrorForceProfile(RunMirrorForceProfileCommand *command) override;
    States::Type updatePID(UpdatePIDCommand *command) override;
    States::Type resetPID(ResetPIDCommand *command) override;
    States::Type enableDisableForceComponent(EnableDisableForceComponentCommand *command) override;

protected:
    States::Type getLoweringState() override { return States::LoweringEngineeringState; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEENGINEERINGSTATE_H_ */
