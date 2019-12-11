/*
 * LoweringEngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <LoweringEngineeringState.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <ILC.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LoweringEngineeringState::LoweringEngineeringState(M1M3SSPublisher* publisher) : EngineeringState(publisher, "LoweringEngineeringState") {}

States::Type LoweringEngineeringState::update(UpdateCommand* command, Model* model) {
    Log.Trace("LoweringEngineeringState: update()");
    this->startTimer();
    States::Type newState = States::NoStateTransition;
    model->getAutomaticOperationsController()->tryDecrementSupportPercentage();
    EnabledState::update(command, model);
    if (model->getAutomaticOperationsController()->checkLowerOperationComplete()) {
        model->getAutomaticOperationsController()->completeLowerOperation();
        newState = States::ParkedEngineeringState;
    } else if (model->getAutomaticOperationsController()->checkLowerOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutLowerOperation();
    }
    model->getILC()->publishForceActuatorState();
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(newState);
}

States::Type LoweringEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type LoweringEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type LoweringEngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type LoweringEngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type LoweringEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type LoweringEngineeringState::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type LoweringEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
