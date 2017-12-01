/*
 * ActiveEngineeringState.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef ACTIVEENGINEERINGSTATE_H_
#define ACTIVEENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveEngineeringState: public EngineeringState {
public:
	States::Type update(UpdateCommand* command, IModel* model);
	States::Type lowerM1M3(LowerM1M3Command* command, IModel* model);
	States::Type exitEngineering(ExitEngineeringCommand* command, IModel* model);

	States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model);
	States::Type clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model);
	States::Type applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, IModel* model);
	States::Type applyAberrationByForces(ApplyAberrationByForcesCommand* command, IModel* model);
	States::Type clearAberration(ClearAberrationCommand* command, IModel* model);
	States::Type applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, IModel* model);
	States::Type applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, IModel* model);
	States::Type clearAOSCorrection(ClearAOSCorrectionCommand* command, IModel* model);
	States::Type stopHardpointMotion(StopHardpointMotionCommand* command, IModel* model);
	States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, IModel* model);
	States::Type translateM1M3(TranslateM1M3Command* command, IModel* model);
	States::Type positionM1M3(PositionM1M3Command* command, IModel* model);
	States::Type turnLightsOn(TurnLightsOnCommand* command, IModel* model);
	States::Type turnLightsOff(TurnLightsOffCommand* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEENGINEERINGSTATE_H_ */
