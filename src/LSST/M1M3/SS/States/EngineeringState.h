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
class EngineeringState : public EnabledState {
public:
    EngineeringState(M1M3SSPublisher* publisher, std::string name);

    virtual States::Type turnAirOn(TurnAirOnCommand* command, Model* model) override;
    virtual States::Type turnAirOff(TurnAirOffCommand* command, Model* model) override;
    virtual States::Type stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) override;
    virtual States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command,
                                                Model* model) override;
    virtual States::Type enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) override;
    virtual States::Type disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) override;
    virtual States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) override;
    virtual States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command,
                                                        Model* model) override;
    virtual States::Type clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) override;
    virtual States::Type turnLightsOn(TurnLightsOnCommand* command, Model* model) override;
    virtual States::Type turnLightsOff(TurnLightsOffCommand* command, Model* model) override;
    virtual States::Type turnPowerOn(TurnPowerOnCommand* command, Model* model) override;
    virtual States::Type turnPowerOff(TurnPowerOffCommand* command, Model* model) override;
    virtual States::Type modbusTransmit(ModbusTransmitCommand* command, Model* model) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENGINEERINGSTATE_H_ */
