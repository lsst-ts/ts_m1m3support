/*
 * LoweringEngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <LoweringEngineeringState.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPublisher.h>
#include <ISafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type LoweringEngineeringState::update(UpdateCommand* command, IModel* model) {
	States::Type newState = States::NoStateTransition;
	EnabledState::update(command, model);
	if (model->getPublisher()->getTimestamp() >= (model->getCachedTimestamp() + 5)) {
		model->getInterlockController()->setMirrorParked(true);
		model->getInterlockController()->setMirrorLoweringRaising(false);
		newState = States::ParkedEngineeringState;
	}
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
