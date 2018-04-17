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
	States::Type applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command, Model* model);
	States::Type applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model);
	States::Type clearAberrationForces(ClearAberrationForcesCommand* command, Model* model);
	States::Type applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command, Model* model);
	States::Type applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model);
	States::Type clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model);
	States::Type translateM1M3(TranslateM1M3Command* command, Model* model);
	States::Type positionM1M3(PositionM1M3Command* command, Model* model);
	States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model);
	States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model);
	States::Type runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model);
	States::Type updatePID(UpdatePIDCommand* command, Model* model);
	States::Type resetPID(ResetPIDCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEENGINEERINGSTATE_H_ */
