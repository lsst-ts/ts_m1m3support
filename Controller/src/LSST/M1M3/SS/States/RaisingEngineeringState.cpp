/*
 * RaisingEngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <RaisingEngineeringState.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPublisher.h>
#include <ISafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type RaisingEngineeringState::update(UpdateCommand* command, IModel* model) {
	States::Type newState = States::NoStateTransition;
	EnabledState::update(command, model);
	if (model->getPublisher()->getTimestamp() >= (model->getCachedTimestamp() + 5)) {
		model->getInterlockController()->setMirrorLoweringRaising(false);
		newState = States::ActiveEngineeringState;
	}
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
