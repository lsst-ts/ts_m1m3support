/*
 * PositionController.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <HardpointActuatorSettings.h>
#include <IPublisher.h>
#include <HardpointActuatorMotionStates.h>
#include <Range.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionController::PositionController(PositionControllerSettings* positionControllerSettings, HardpointActuatorSettings* hardpointActuatorSettings, IPublisher* publisher) {
	this->positionControllerSettings = positionControllerSettings;
	this->hardpointActuatorSettings = hardpointActuatorSettings;
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
	// The reason for defining HP 3 first (index 2) is due to the matrix. Review the MirrorPositionToHardpointDisplacementTable
	// for a description of the matrix.
	int32_t steps[6] = {0, 0, 0, 0, 0, 0};
	steps[2] = (this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[0] * x +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[1] * y +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[2] * z +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[3] * rX +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[4] * rY +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[5] * rZ) *
			(MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER / this->hardpointActuatorSettings->MicrometersPerStep);
	steps[3] = (this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[6] * x +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[7] * y +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[8] * z +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[9] * rX +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[10] * rY +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[11] * rZ) *
			(MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER / this->hardpointActuatorSettings->MicrometersPerStep);
	steps[4] = (this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[12] * x +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[13] * y +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[14] * z +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[15] * rX +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[16] * rY +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[17] * rZ) *
			(MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER / this->hardpointActuatorSettings->MicrometersPerStep);
	steps[5] = (this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[18] * x +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[19] * y +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[20] * z +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[21] * rX +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[22] * rY +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[23] * rZ) *
			(MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER / this->hardpointActuatorSettings->MicrometersPerStep);
	steps[0] = (this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[24] * x +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[25] * y +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[26] * z +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[27] * rX +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[28] * rY +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[29] * rZ) *
			(MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER / this->hardpointActuatorSettings->MicrometersPerStep);
	steps[1] = (this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[30] * x +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[31] * y +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[32] * z +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[33] * rX +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[34] * rY +
			this->hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[35] * rZ) *
			(MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER / this->hardpointActuatorSettings->MicrometersPerStep);
	this->move(steps);
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
