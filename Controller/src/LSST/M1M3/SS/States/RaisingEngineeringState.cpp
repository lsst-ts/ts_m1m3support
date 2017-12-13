/*
 * RaisingEngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <RaisingEngineeringState.h>
#include <IModel.h>
#include <ISafetyController.h>
#include <IAutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type RaisingEngineeringState::update(UpdateCommand* command, IModel* model) {
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
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
