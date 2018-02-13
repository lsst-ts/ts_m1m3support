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
	ActiveEngineeringState(M1M3SSPublisher* publisher);

	States::Type update(UpdateCommand* command, Model* model);
	States::Type lowerM1M3(LowerM1M3Command* command, Model* model);
	States::Type exitEngineering(ExitEngineeringCommand* command, Model* model);
	States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model);
	States::Type clearOffsetForces(ClearOffsetForcesCommand* command, Model* model);
	States::Type applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, Model* model);
	States::Type applyAberrationByForces(ApplyAberrationByForcesCommand* command, Model* model);
	States::Type clearAberration(ClearAberrationCommand* command, Model* model);
	States::Type applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, Model* model);
	States::Type applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, Model* model);
	States::Type clearAOSCorrection(ClearAOSCorrectionCommand* command, Model* model);
	States::Type stopHardpointMotion(StopHardpointMotionCommand* command, Model* model);
	States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model);
	States::Type translateM1M3(TranslateM1M3Command* command, Model* model);
	States::Type positionM1M3(PositionM1M3Command* command, Model* model);
	States::Type turnLightsOn(TurnLightsOnCommand* command, Model* model);
	States::Type turnLightsOff(TurnLightsOffCommand* command, Model* model);
	States::Type turnPowerOn(TurnPowerOnCommand* command, Model* model);
	States::Type turnPowerOff(TurnPowerOffCommand* command, Model* model);
	States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model);
	States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model);
	States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model);
	States::Type runHardpointCorrectionProfile(RunHardpointCorrectionProfileCommand* command, Model* model);
	States::Type updatePID(UpdatePIDCommand* command, Model* model);
	States::Type resetPID(ResetPIDCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEENGINEERINGSTATE_H_ */
