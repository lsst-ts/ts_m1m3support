/*
 * EnabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <EnabledState.h>
#include <IAccelerometer.h>
#include <IAirController.h>
#include <IDisplacement.h>
#include <IForceController.h>
#include <IILC.h>
#include <IInclinometer.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPositionController.h>
#include <ISafetyController.h>
#include <IPowerController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type EnabledState::update(UpdateCommand* command, IModel* model) {
	model->getPositionController()->updateSteps();
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getAccelerometer()->sampleData();
	model->getAirController()->checkStatus();
	model->getDisplacement()->readDataResponse();
	model->getInclinometer()->readDataResponse();
	model->getForceController()->updateAppliedForces();
	model->getForceController()->processAppliedForces();
	model->getILC()->publishForceActuatorStatus();
	model->getILC()->publishForceActuatorData();
	model->getILC()->publishHardpointStatus();
	model->getILC()->publishHardpointData();
	//model->getPowerController()->checkPowerStatus();
	//model->getInterlockController()->checkInterlockStatus();
	model->getInterlockController()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, IModel* model) {
	model->getForceController()->updateTMAAzimuthData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command, IModel* model) {
	model->getForceController()->updateTMAElevationData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::performRaiseM1M3Actions(UpdateCommand* command, IModel* model) {
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
		//       For now we will assume the worst and fault the system
		model->getSafetyController()->raiseOperationTimeout(true);
	}
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
