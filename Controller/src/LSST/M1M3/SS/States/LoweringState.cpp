/*
 * LoweringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <LoweringState.h>
#include <InterlockController.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type LoweringState::update(UpdateCommand* command, Model* model) {
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
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
