/*
 * RaisingState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <RaisingState.h>
#include <Model.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingState::RaisingState(M1M3SSPublisher* publisher) : EnabledState(publisher, "RaisingState") { }

States::Type RaisingState::update(UpdateCommand* command, Model* model) {
	States::Type newState = States::NoStateTransition;
	model->getAutomaticOperationsController()->tryIncrementingSupportPercentage();
	EnabledState::update(command, model);
	if (model->getAutomaticOperationsController()->checkRaiseOperationComplete()) {
		model->getAutomaticOperationsController()->completeRaiseOperation();
		newState = States::ActiveState;
	}
	else if (model->getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
		model->getAutomaticOperationsController()->timeoutRaiseOperation();
	}
	return model->getSafetyController()->checkSafety(newState);
}

States::Type RaisingState::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
	States::Type newState = States::LoweringState;
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
