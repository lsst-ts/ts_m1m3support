/*
 * LoweringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <LoweringState.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <ILC.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LoweringState::LoweringState(M1M3SSPublisher* publisher) : EnabledState(publisher, "LoweringState") { }

States::Type LoweringState::update(UpdateCommand* command, Model* model) {
	Log.Trace("LoweringState: update()");
	this->startTimer();
	States::Type newState = States::NoStateTransition;
	model->getAutomaticOperationsController()->tryDecrementSupportPercentage();
	EnabledState::update(command, model);
	if (model->getAutomaticOperationsController()->checkLowerOperationComplete()) {
		model->getAutomaticOperationsController()->completeLowerOperation();
		newState = States::ParkedState;
	}
	else if (model->getAutomaticOperationsController()->checkLowerOperationTimeout()) {
		model->getAutomaticOperationsController()->timeoutLowerOperation();
	}
	model->getILC()->publishForceActuatorState();
	this->stopTimer();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
