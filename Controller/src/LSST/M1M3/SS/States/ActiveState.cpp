/*
 * ActiveState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <ActiveState.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPublisher.h>
#include <ISafetyController.h>
#include <IAutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ActiveState::update(UpdateCommand* command, IModel* model) {
	EnabledState::update(command, model);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command, IModel* model) {
	States::Type newState = States::ActiveEngineeringState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command, IModel* model) {
	States::Type newState = States::LoweringState;
	model->getAutomaticOperationsController()->startLowerOperation();
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
