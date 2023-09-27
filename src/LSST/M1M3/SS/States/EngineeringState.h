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

#ifndef ENGINEERINGSTATE_H_
#define ENGINEERINGSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Engineering state. Parent class for all engineering classes. Allows manual
 * control of air, force actuators and hard-points.
 */
class EngineeringState : public virtual EnabledState {
public:
    States::Type turnAirOn(TurnAirOnCommand* command) override;
    States::Type turnAirOff(TurnAirOffCommand* command) override;
    States::Type boosterValveOpen(BoosterValveOpenCommand* command) override;
    States::Type boosterValveClose(BoosterValveCloseCommand* command) override;
    States::Type stopHardpointMotion(StopHardpointMotionCommand* command) override;
    States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command) override;
    States::Type enableHardpointChase(EnableHardpointChaseCommand* command) override;
    States::Type disableHardpointChase(DisableHardpointChaseCommand* command) override;
    States::Type applyOffsetForces(ApplyOffsetForcesCommand* command) override;
    States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) override;
    States::Type clearOffsetForces(ClearOffsetForcesCommand* command) override;
    States::Type turnLightsOn(TurnLightsOnCommand* command) override;
    States::Type turnLightsOff(TurnLightsOffCommand* command) override;
    States::Type turnPowerOn(TurnPowerOnCommand* command) override;
    States::Type turnPowerOff(TurnPowerOffCommand* command) override;
    States::Type disableForceActuator(DisableForceActuatorCommand* command) override;
    States::Type enableForceActuator(EnableForceActuatorCommand* command) override;
    States::Type enableAllForceActuators(EnableAllForceActuatorsCommand* command) override;
    States::Type setSlewControllerSettings(SetSlewControllerSettingsCommand* command) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENGINEERINGSTATE_H_ */
