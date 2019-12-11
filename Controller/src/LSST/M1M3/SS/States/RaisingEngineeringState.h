/*
 * RaisingEngineeringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef RAISINGENGINEERINGSTATE_H_
#define RAISINGENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RaisingEngineeringState : public EngineeringState {
   public:
    RaisingEngineeringState(M1M3SSPublisher* publisher);

    States::Type update(UpdateCommand* command, Model* model);
    States::Type abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model);
    States::Type stopHardpointMotion(StopHardpointMotionCommand* command, Model* model);
    States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model);
    States::Type enableHardpointChase(EnableHardpointChaseCommand* command, Model* model);
    States::Type disableHardpointChase(DisableHardpointChaseCommand* command, Model* model);
    States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model);
    States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model);
    States::Type clearOffsetForces(ClearOffsetForcesCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISINGENGINEERINGSTATE_H_ */
