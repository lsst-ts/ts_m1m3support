/*
 * ActiveState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <ActiveState.h>
#include <IModel.h>
#include <ISafetyController.h>
#include <PositionM1M3Command.h>
#include <IPositionController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ActiveState::update(UpdateCommand* command, IModel* model) {
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command, IModel* model) {
	States::Type newState = States::ActiveEngineeringState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command, IModel* model) {
	States::Type newState = States::LoweringState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::positionM1M3(PositionM1M3Command* command, IModel* model) {
	model->getPositionController()->moveToAbsolute(command->getData()->XPosition, command->getData()->YPosition, command->getData()->ZPosition,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
