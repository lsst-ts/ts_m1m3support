/*
 * RaisingEngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <RaisingEngineeringState.h>
#include <Model.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <ILC.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingEngineeringState::RaisingEngineeringState(M1M3SSPublisher* publisher) : EngineeringState(publisher, "RaisingEngineeringState") { }

States::Type RaisingEngineeringState::update(UpdateCommand* command, Model* model) {
	Log.Trace("RaisingEngineeringState: update()");
	this->startTimer();
	States::Type newState = States::NoStateTransition;
	model->getAutomaticOperationsController()->tryIncrementingSupportPercentage();
	EnabledState::update(command, model);
	if (model->getAutomaticOperationsController()->checkRaiseOperationComplete()) {
		model->getAutomaticOperationsController()->completeRaiseOperation();
		newState = States::ActiveEngineeringState;
	}
	else if (model->getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
		model->getAutomaticOperationsController()->timeoutRaiseOperation();
	}
	this->stopTimer();
	model->getILC()->publishForceActuatorState();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(newState);
}

States::Type RaisingEngineeringState::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
	Log.Info("RaisingEngineeringState: abortRaiseM1M3()");
	States::Type newState = States::LoweringEngineeringState;
	model->getAutomaticOperationsController()->abortRaiseM1M3();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type RaisingEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) { return this->rejectCommandInvalidState("StopHardpointMotion"); }
States::Type RaisingEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) { return this->rejectCommandInvalidState("MoveHardpointActuators"); }
States::Type RaisingEngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) { return this->rejectCommandInvalidState("EnableHardpointChase"); }
States::Type RaisingEngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) { return this->rejectCommandInvalidState("DisableHardpointChase"); }
States::Type RaisingEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) { return this->rejectCommandInvalidState("ApplyOffsetForces"); }
States::Type RaisingEngineeringState::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model) { return this->rejectCommandInvalidState("ApplyOffsetForcesByMirrorForce"); }
States::Type RaisingEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) { return this->rejectCommandInvalidState("ClearOffsetForces"); }


} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
