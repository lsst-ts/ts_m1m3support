/*
 * LoweringEngineeringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef LOWERINGENGINEERINGSTATE_H_
#define LOWERINGENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class LoweringEngineeringState : public EngineeringState {
   public:
    LoweringEngineeringState(M1M3SSPublisher* publisher);

    States::Type update(UpdateCommand* command, Model* model);
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

#endif /* LOWERINGENGINEERINGSTATE_H_ */
