/*
 * PositionController.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <IPublisher.h>
#include <HardpointActuatorMotionStates.h>
#include <Range.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionController::PositionController(PositionControllerSettings* positionControllerSettings, IPublisher* publisher) {
	this->positionControllerSettings = positionControllerSettings;
	this->publisher = publisher;
	this->hardpointData = this->publisher->getHardpointData();
	this->hardpointActuatorMotionState = this->publisher->getEventHardpointActuatorMotionState();
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorMotionState->MotionState[i] = HardpointActuatorMotionStates::Standby;
		this->hardpointData->StepsQueued[i] = 0;
		this->hardpointData->StepsCommanded[i] = 0;
	}
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::enableChase(int32_t actuatorID) {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	this->hardpointActuatorMotionState->MotionState[actuatorID - 1] = HardpointActuatorMotionStates::Chasing;
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::disableChase(int32_t actuatorID) {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	this->hardpointActuatorMotionState->MotionState[actuatorID - 1] = HardpointActuatorMotionStates::Standby;
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::enableChaseAll() {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorMotionState->MotionState[i] = HardpointActuatorMotionStates::Chasing;
	}
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::disableChaseAll() {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorMotionState->MotionState[i] = HardpointActuatorMotionStates::Standby;
	}
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::move(int32_t* steps) {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointData->StepsQueued[i] = steps[i];
		this->hardpointActuatorMotionState->MotionState[i] = steps[i] != 0 ? HardpointActuatorMotionStates::Moving : HardpointActuatorMotionStates::Standby;
	}
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {

}

void PositionController::stopMotion() {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointData->StepsQueued[i] = 0;
		this->hardpointActuatorMotionState->MotionState[i] = HardpointActuatorMotionStates::Standby;
	}
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::updateSteps() {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	bool publishState = false;
	for(int i = 0; i < HP_COUNT; i++) {
		switch(this->hardpointActuatorMotionState->MotionState[i]) {
		case HardpointActuatorMotionStates::Standby:
			this->hardpointData->StepsCommanded[i] = 0;
			this->hardpointData->StepsQueued[i] = 0;
			break;
		case HardpointActuatorMotionStates::Chasing:
		{
			float force = this->hardpointData->Force[i];
			int32_t chaseSteps = (int32_t)(force * this->positionControllerSettings->ForceToStepsCoefficient);
			chaseSteps = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, chaseSteps);
			this->hardpointData->StepsCommanded[i] = (int16_t)chaseSteps;
			break;
		}
		case HardpointActuatorMotionStates::Moving:
		{
			int32_t moveSteps = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, this->hardpointData->StepsQueued[i]);
			this->hardpointData->StepsQueued[i] -= moveSteps;
			this->hardpointData->StepsCommanded[i] = (int16_t)moveSteps;
			if(this->hardpointData->StepsQueued[i] == 0 && this->hardpointData->StepsCommanded[i] == 0) {
				publishState = true;
				this->hardpointActuatorMotionState->MotionState[i] = HardpointActuatorMotionStates::Standby;
			}
			break;
		}
		}
	}
	if (publishState) {
		this->publisher->logHardpointActuatorMotionState();
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
