/*
 * EngineeringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef ENGINEERINGSTATE_H_
#define ENGINEERINGSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class EngineeringState : public EnabledState {
public:
    EngineeringState(M1M3SSPublisher* publisher);
    EngineeringState(M1M3SSPublisher* publisher, std::string name);

    States::Type turnAirOn(TurnAirOnCommand* command, Model* model);
    States::Type turnAirOff(TurnAirOffCommand* command, Model* model);
    virtual States::Type stopHardpointMotion(StopHardpointMotionCommand* command, Model* model);
    virtual States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model);
    virtual States::Type enableHardpointChase(EnableHardpointChaseCommand* command, Model* model);
    virtual States::Type disableHardpointChase(DisableHardpointChaseCommand* command, Model* model);
    virtual States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model);
    virtual States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model);
    virtual States::Type clearOffsetForces(ClearOffsetForcesCommand* command, Model* model);
    States::Type turnLightsOn(TurnLightsOnCommand* command, Model* model);
    States::Type turnLightsOff(TurnLightsOffCommand* command, Model* model);
    States::Type turnPowerOn(TurnPowerOnCommand* command, Model* model);
    States::Type turnPowerOff(TurnPowerOffCommand* command, Model* model);
    States::Type modbusTransmit(ModbusTransmitCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENGINEERINGSTATE_H_ */
