/*
 * RaisingEngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <RaisingEngineeringState.h>
#include <IForceController.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPositionController.h>
#include <IPublisher.h>
#include <ISafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type RaisingEngineeringState::update(UpdateCommand* command, IModel* model) {
	States::Type newState = States::NoStateTransition;
	if (!model->getForceController()->supportPercentageFilled()) {
		// We are still in the process of transfering the support force from the static supports
		// to the force actuators
		if (model->getPositionController()->forcesInTolerance()) {
			// The forces on the hardpoints are within tolerance, we can continue to transfer the
			// support force from the static supports to the force actuators
			model->getForceController()->incSupportPercentage();
			if (model->getForceController()->supportPercentageFilled()) {
				// All of the support force has been transfered from the static supports to the
				// force actuators, stop the hardpoints from chasing and start moving to the
				// reference position
				model->getPositionController()->disableChaseAll();
				model->getPositionController()->moveToReferencePosition();
			}
		}
	}
	// Execute the standard update cycle
	EnabledState::update(command, model);
	if (model->getForceController()->supportPercentageFilled() && model->getPositionController()->motionComplete()) {
		// Transition to the active engineering state if all of the support force has been transfered
		// from the static supports to the force actuators and all hardpoints have completed their
		// commanded motions
		model->getForceController()->applyStaticForces();
		model->getForceController()->applyAzimuthForces();
		model->getForceController()->applyTemperatureForces();
		model->getForceController()->applyDynamicForces();
		model->getForceController()->fillSupportPercentage();
		model->getInterlockController()->setMirrorLoweringRaising(false);
		newState = States::ActiveEngineeringState;
	}
	else if (model->getPublisher()->getTimestamp() >= (model->getCachedTimestamp() + model->getPositionController()->getRaiseLowerTimeout())) {
		// TODO: How should the system react if the operation times out?
	}
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
