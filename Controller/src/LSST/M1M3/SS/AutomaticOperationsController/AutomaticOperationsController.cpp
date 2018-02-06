/*
 * AutomaticOperationsController.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: ccontaxis
 */

#include <AutomaticOperationsController.h>
#include <ForceController.h>
#include <PositionController.h>
#include <InterlockController.h>
#include <SafetyController.h>
#include <M1M3SSPublisher.h>
#include <PowerController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AutomaticOperationsController::AutomaticOperationsController(PositionController* positionController, ForceController* forceController, InterlockController* interlockController, SafetyController* safetyController, M1M3SSPublisher* publisher, PowerController* powerController) {
	this->positionController = positionController;
	this->forceController = forceController;
	this->interlockController = interlockController;
	this->safetyController = safetyController;
	this->publisher = publisher;
	this->powerController = powerController;
	this->cachedTimestamp = 0;
}

void AutomaticOperationsController::startRaiseOperation() {
	this->safetyController->raiseOperationTimeout(false);
	this->positionController->stopMotion();
	this->positionController->enableChaseAll();
	this->forceController->applyElevationForces();
	this->forceController->zeroStaticForces();
	this->forceController->zeroAzimuthForces();
	this->forceController->zeroTemperatureForces();
	this->forceController->zeroDynamicForces();
	this->forceController->zeroOffsetForces();
	this->forceController->zeroAberration();
	this->forceController->zeroAOSCorrection();
	this->forceController->zeroSupportPercentage();
	this->interlockController->setMirrorParked(false);
	this->interlockController->setMirrorLoweringRaising(true);
	this->cachedTimestamp = this->publisher->getTimestamp();
}

void AutomaticOperationsController::tryIncrementingSupportPercentage() {
	if (!this->forceController->supportPercentageFilled()) {
		// We are still in the process of transfering the support force from the static supports
		// to the force actuators
		if (this->positionController->forcesInTolerance()) {
			// The forces on the hardpoints are within tolerance, we can continue to transfer the
			// support force from the static supports to the force actuators
			this->forceController->incSupportPercentage();
			if (this->forceController->supportPercentageFilled()) {
				// All of the support force has been transfered from the static supports to the
				// force actuators, stop the hardpoints from chasing and start moving to the
				// reference position
				this->positionController->disableChaseAll();
				this->positionController->moveToReferencePosition();
			}
		}
	}
}

bool AutomaticOperationsController::checkRaiseOperationComplete() {
	return this->forceController->supportPercentageFilled() && this->positionController->motionComplete();
}

void AutomaticOperationsController::completeRaiseOperation() {
	// Transition to the end state (active or active engineering) if all of the support force has been transfered
	// from the static supports to the force actuators and all hardpoints have completed their
	// commanded motions
	this->forceController->applyStaticForces();
	this->forceController->applyAzimuthForces();
	this->forceController->applyTemperatureForces();
	this->forceController->applyDynamicForces();
	this->forceController->fillSupportPercentage();
	this->interlockController->setMirrorLoweringRaising(false);
}

bool AutomaticOperationsController::checkRaiseOperationTimeout() {
	return this->publisher->getTimestamp() >= (this->cachedTimestamp + this->positionController->getRaiseLowerTimeout());
}

void AutomaticOperationsController::timeoutRaiseOperation() {
	this->safetyController->raiseOperationTimeout(true);
}

void AutomaticOperationsController::startLowerOperation() {
	this->safetyController->lowerOperationTimeout(false);
	this->positionController->stopMotion();
	this->positionController->enableChaseAll();
	this->forceController->applyElevationForces();
	this->forceController->zeroStaticForces();
	this->forceController->zeroAzimuthForces();
	this->forceController->zeroTemperatureForces();
	this->forceController->zeroDynamicForces();
	this->forceController->zeroOffsetForces();
	this->forceController->zeroAberration();
	this->forceController->zeroAOSCorrection();
	this->forceController->fillSupportPercentage();
	this->interlockController->setMirrorLoweringRaising(true);
	this->cachedTimestamp = this->publisher->getTimestamp();
}

void AutomaticOperationsController::tryDecrementSupportPercentage() {
	if (!this->forceController->supportPercentageZeroed()) {
		// We are still in the process of transfering the support force from the static supports
		// to the force actuators
		if (this->positionController->forcesInTolerance()) {
			// The forces on the hardpoints are within tolerance, we can continue to transfer the
			// support force from the static supports to the force actuators
			this->forceController->decSupportPercentage();
		}
	}
}

bool AutomaticOperationsController::checkLowerOperationComplete() {
	return this->forceController->supportPercentageZeroed();
}

void AutomaticOperationsController::completeLowerOperation() {
	// All of the support force has been transfered from the static supports to the
	// force actuators, stop the hardpoints from chasing
	// Transition to the end state (parked or parked engineering) if all of the support
	// force has been transfered from the force actuators to the static supports
	this->positionController->disableChaseAll();
	this->forceController->zeroElevationForces();
	this->forceController->zeroAOSCorrection();
	this->forceController->zeroAberration();
	this->forceController->zeroOffsetForces();
	this->forceController->zeroStaticForces();
	this->forceController->zeroAzimuthForces();
	this->forceController->zeroTemperatureForces();
	this->forceController->zeroDynamicForces();
	this->forceController->zeroSupportPercentage();
	this->interlockController->setMirrorParked(true);
	this->interlockController->setMirrorLoweringRaising(false);
}

bool AutomaticOperationsController::checkLowerOperationTimeout() {
	return this->publisher->getTimestamp() >= (this->cachedTimestamp + this->positionController->getRaiseLowerTimeout());
}

void AutomaticOperationsController::timeoutLowerOperation() {
	// TODO: How should the system react if the operation times out?
	//       For now we will assume the worst and fault the system
	this->safetyController->lowerOperationTimeout(true);
}

void AutomaticOperationsController::uncontrolledLowerOperation() {
	this->powerController->setAllAuxPowerNetworks(false);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
