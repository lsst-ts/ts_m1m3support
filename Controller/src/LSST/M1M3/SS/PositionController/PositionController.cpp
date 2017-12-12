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
	this->hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorMotionState->MotionState[i] = HardpointActuatorMotionStates::Standby;
		this->hardpointData->StepsQueued[i] = 0;
		this->hardpointData->StepsCommanded[i] = 0;
		this->scaledMaxStepsPerLoop[i] = this->positionControllerSettings->MaxStepsPerLoop;
	}
	this->publisher->logHardpointActuatorMotionState();

}

double PositionController::getRaiseLowerTimeout() {
	return this->positionControllerSettings->RaiseLowerTimeoutInSeconds;
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

bool PositionController::forcesInTolerance() {
	bool inTolerance = true;
	for(int i = 0; i < HP_COUNT; i++) {
		inTolerance = inTolerance && Range::InRange(this->positionControllerSettings->RaiseLowerForceLimitLow, this->positionControllerSettings->RaiseLowerForceLimitHigh, this->hardpointData->Force[i]);
	}
	return inTolerance;
}


bool PositionController::motionComplete() {
	return this->hardpointActuatorMotionState->MotionState[0] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorMotionState->MotionState[1] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorMotionState->MotionState[2] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorMotionState->MotionState[3] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorMotionState->MotionState[4] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorMotionState->MotionState[5] == HardpointActuatorMotionStates::Standby;
}

void PositionController::move(int32_t* steps) {
	this->hardpointActuatorMotionState->Timestamp = this->publisher->getTimestamp();
	double loopCycles[6];
	double maxLoopCycles = 0;
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointData->StepsQueued[i] = steps[i];
		this->hardpointActuatorMotionState->MotionState[i] = steps[i] != 0 ? HardpointActuatorMotionStates::Moving : HardpointActuatorMotionStates::Standby;
		loopCycles[i] = this->abs(steps[i]) / (double)this->positionControllerSettings->MaxStepsPerLoop;
		if (loopCycles[i] > maxLoopCycles) {
			maxLoopCycles = loopCycles[i];
		}
	}
	for(int i = 0; i < HP_COUNT; ++i) {
		this->scaledMaxStepsPerLoop[i] = (int32_t)(this->positionControllerSettings->MaxStepsPerLoop/(maxLoopCycles/loopCycles[i]));
	}
	this->publisher->logHardpointActuatorMotionState();
}

void PositionController::moveToEncoder(int32_t* encoderValues) {
	int32_t steps[6];
	for(int i = 0; i < HP_COUNT; ++i) {
		steps[i] = (int32_t)((encoderValues[i] - this->hardpointData->Encoder[i]) * (this->hardpointActuatorSettings->MicrometersPerEncoder /  this->hardpointActuatorSettings->MicrometersPerStep));
	}
	this->move(steps);
}

void PositionController::moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ) {
	int32_t steps[6];
	this->convertToSteps(steps, x, y, z, rX, rY, rZ);
	int32_t encoderValues[6];
	double stepsToEncoder = this->hardpointActuatorSettings->MicrometersPerStep / this->hardpointActuatorSettings->MicrometersPerEncoder;
	for(int i = 0; i < HP_COUNT; ++i) {
		encoderValues[i] = this->hardpointInfo->ReferencePosition[i] + steps[i] * stepsToEncoder;
	}
	this->moveToEncoder(encoderValues);
}

void PositionController::moveToReferencePosition() {
	this->moveToEncoder(this->hardpointInfo->ReferencePosition);
}

void PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {
	int32_t steps[6];
	this->convertToSteps(steps, x, y, z, rX, rY, rZ);
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
			int32_t moveSteps = Range::CoerceIntoRange(-this->scaledMaxStepsPerLoop[i], this->scaledMaxStepsPerLoop[i], this->hardpointData->StepsQueued[i]);
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

void PositionController::convertToSteps(int32_t* steps, double x, double y, double z, double rX, double rY, double rZ) {
	// The reason for defining HP 3 first (index 2) is due to the matrix. Review the MirrorPositionToHardpointDisplacementTable
	// for a description of the matrix.
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
}

int32_t PositionController::abs(int32_t x) {
	if (x < 0)
		return x * -1;
	return x;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
