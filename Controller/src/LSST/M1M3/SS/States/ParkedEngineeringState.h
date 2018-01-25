/*
 * ParkedEngineeringState.h
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
 */

#ifndef PARKEDENGINEERINGSTATE_H_
#define PARKEDENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ParkedEngineeringState: public EngineeringState {
public:
	ParkedEngineeringState(M1M3SSPublisher* publisher);

	States::Type update(UpdateCommand* command, Model* model);
	States::Type raiseM1M3(RaiseM1M3Command* command, Model* model);
	States::Type exitEngineering(ExitEngineeringCommand* command, Model* model);
	States::Type disable(DisableCommand* command, Model* model);
	States::Type turnAirOn(TurnAirOnCommand* command, Model* model);
	States::Type turnAirOff(TurnAirOffCommand* command, Model* model);
	States::Type stopHardpointMotion(StopHardpointMotionCommand* command, Model* model);
	States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model);
	States::Type enableHardpointChase(EnableHardpointChaseCommand* command, Model* model);
	States::Type disableHardpointChase(DisableHardpointChaseCommand* command, Model* model);
	States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model);
	States::Type clearOffsetForces(ClearOffsetForcesCommand* command, Model* model);
	States::Type turnLightsOn(TurnLightsOnCommand* command, Model* model);
	States::Type turnLightsOff(TurnLightsOffCommand* command, Model* model);
	States::Type turnPowerOn(TurnPowerOnCommand* command, Model* model);
	States::Type turnPowerOff(TurnPowerOffCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDENGINEERINGSTATE_H_ */
