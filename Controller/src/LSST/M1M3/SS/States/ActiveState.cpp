/*
 * ActiveState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <ActiveState.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <ForceController.h>
#include <ApplyActiveOpticForcesByBendingModesCommand.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ClearActiveOpticForcesCommand.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveState::ActiveState(M1M3SSPublisher* publisher) : EnabledState(publisher, "ActiveState") { }

States::Type ActiveState::update(UpdateCommand* command, Model* model) {
	Log.Trace("ActiveState: update()");
	this->startTimer();
	EnabledState::update(command, model);
	this->stopTimer();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command, Model* model) {
	Log.Info("ActiveState: enterEngineering()");
	States::Type newState = States::ActiveEngineeringState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command, Model* model) {
	Log.Info("ActiveState: lowerM1M3()");
	States::Type newState = States::LoweringState;
	model->getAutomaticOperationsController()->startLowerOperation();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model) {
	Log.Info("ActiveState: enableHardpointCorrections()");
	model->getForceController()->applyBalanceForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model) {
	Log.Info("ActiveState: disableHardpointCorrections()");
	model->getForceController()->zeroBalanceForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command, Model* model) {
	Log.Info("ActiveState: applyActiveOpticForcesByBendingModes()");
	model->getForceController()->applyActiveOpticForcesByBendingModes(command->getData()->coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model) {
	Log.Info("ActiveState: applyActiveOpticForces()");
	model->getForceController()->applyActiveOpticForces(command->getData()->zForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model) {
	Log.Info("ActiveState: clearActiveOpticForces()");
	model->getForceController()->zeroActiveOpticForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
