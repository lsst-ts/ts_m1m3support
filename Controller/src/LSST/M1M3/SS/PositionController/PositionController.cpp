/*
 * PositionController.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <HardpointActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <HardpointActuatorMotionStates.h>
#include <Range.h>
#include <SAL_m1m3C.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionController::PositionController(PositionControllerSettings* positionControllerSettings, HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher) {
	Log.Debug("PositionController: PositionController()");
	this->positionControllerSettings = positionControllerSettings;
	this->hardpointActuatorSettings = hardpointActuatorSettings;
	this->publisher = publisher;
	this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
	this->hardpointActuatorState = this->publisher->getEventHardpointActuatorState();
	this->hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorState->MotionState[i] = HardpointActuatorMotionStates::Standby;
		this->hardpointActuatorData->StepsQueued[i] = 0;
		this->hardpointActuatorData->StepsCommanded[i] = 0;
		this->scaledMaxStepsPerLoop[i] = this->positionControllerSettings->MaxStepsPerLoop;
	}
	this->publisher->logHardpointActuatorState();
}

double PositionController::getRaiseLowerTimeout() {
	return this->positionControllerSettings->RaiseLowerTimeoutInSeconds;
}

bool PositionController::enableChase(int32_t actuatorID) {
	Log.Info("PositionController: enableChase(%d)", actuatorID);
	if (this->hardpointActuatorState->MotionState[actuatorID - 1] != HardpointActuatorMotionStates::Standby) {
		return false;
	}
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	this->hardpointActuatorState->MotionState[actuatorID - 1] = HardpointActuatorMotionStates::Chasing;
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

void PositionController::disableChase(int32_t actuatorID) {
	Log.Info("PositionController: disableChase(%d)", actuatorID);
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	this->hardpointActuatorState->MotionState[actuatorID - 1] = HardpointActuatorMotionStates::Standby;
	this->publisher->tryLogHardpointActuatorState();
}

bool PositionController::enableChaseAll() {
	Log.Info("PositionController: enableChaseAll()");
	if (this->hardpointActuatorState->MotionState[0] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->MotionState[1] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->MotionState[2] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->MotionState[3] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->MotionState[4] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->MotionState[5] != HardpointActuatorMotionStates::Standby) {
		return false;
	}
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorState->MotionState[i] = HardpointActuatorMotionStates::Chasing;
	}
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

void PositionController::disableChaseAll() {
	Log.Info("PositionController: disableChaseAll()");
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorState->MotionState[i] = HardpointActuatorMotionStates::Standby;
	}
	this->publisher->tryLogHardpointActuatorState();
}

bool PositionController::forcesInTolerance() {
	Log.Trace("PositionController: forcesInTolerance()");
	bool inTolerance = true;
	for(int i = 0; i < HP_COUNT; i++) {
		inTolerance = inTolerance && Range::InRange((float)this->positionControllerSettings->RaiseLowerForceLimitLow, (float)this->positionControllerSettings->RaiseLowerForceLimitHigh, this->hardpointActuatorData->MeasuredForce[i]);
	}
	return inTolerance;
}

bool PositionController::motionComplete() {
	Log.Debug("PositionController: motionComplete()");
	return this->hardpointActuatorState->MotionState[0] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->MotionState[1] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->MotionState[2] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->MotionState[3] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->MotionState[4] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->MotionState[5] == HardpointActuatorMotionStates::Standby;
}

bool PositionController::move(int32_t* steps) {
	Log.Info("PositionController: move()");
	if ((this->hardpointActuatorState->MotionState[0] != 0 && steps[0] != 0) ||
			(this->hardpointActuatorState->MotionState[1] != 0 && steps[1] != 0) ||
			(this->hardpointActuatorState->MotionState[2] != 0 && steps[2] != 0) ||
			(this->hardpointActuatorState->MotionState[3] != 0 && steps[3] != 0) ||
			(this->hardpointActuatorState->MotionState[4] != 0 && steps[4] != 0) ||
			(this->hardpointActuatorState->MotionState[5] != 0 && steps[5] != 0)) {
		return false;
	}
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	double loopCycles[6];
	double maxLoopCycles = 0;
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorData->StepsQueued[i] = steps[i];
		this->hardpointActuatorState->MotionState[i] = steps[i] != 0 ? HardpointActuatorMotionStates::Moving : HardpointActuatorMotionStates::Standby;
		loopCycles[i] = this->abs(steps[i]) / (double)this->positionControllerSettings->MaxStepsPerLoop;
		if (loopCycles[i] > maxLoopCycles) {
			maxLoopCycles = loopCycles[i];
		}
	}
	for(int i = 0; i < HP_COUNT; ++i) {
		this->scaledMaxStepsPerLoop[i] = (int32_t)(this->positionControllerSettings->MaxStepsPerLoop/(maxLoopCycles/loopCycles[i]));
	}
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

bool PositionController::moveToEncoder(int32_t* encoderValues) {
	Log.Info("PositionController: moveToEncoder()");
	int32_t steps[6];
	for(int i = 0; i < HP_COUNT; ++i) {
		steps[i] = (int32_t)((encoderValues[i] - this->hardpointActuatorData->Encoder[i]) * (this->hardpointActuatorSettings->MicrometersPerEncoder /  this->hardpointActuatorSettings->MicrometersPerStep));
	}
	return this->move(steps);
}

bool PositionController::moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ) {
	Log.Info("PositionController: moveToAbsolute()");
	int32_t steps[6];
	this->convertToSteps(steps, x, y, z, rX, rY, rZ);
	int32_t encoderValues[6];
	double stepsToEncoder = this->hardpointActuatorSettings->MicrometersPerStep / this->hardpointActuatorSettings->MicrometersPerEncoder;
	for(int i = 0; i < HP_COUNT; ++i) {
		encoderValues[i] = this->hardpointInfo->ReferencePosition[i] + steps[i] * stepsToEncoder;
	}
	return this->moveToEncoder(encoderValues);
}

bool PositionController::moveToReferencePosition() {
	Log.Info("PositionController: moveToReferencePosition()");
	return this->moveToEncoder(this->hardpointInfo->ReferencePosition);
}

bool PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {
	Log.Info("PositionController: translate()");
	int32_t steps[6];
	this->convertToSteps(steps, x, y, z, rX, rY, rZ);
	return this->move(steps);
}

void PositionController::stopMotion() {
	Log.Info("PositionController: stopMotion()");
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorData->StepsQueued[i] = 0;
		this->hardpointActuatorState->MotionState[i] = HardpointActuatorMotionStates::Standby;
	}
	this->publisher->tryLogHardpointActuatorState();
}

void PositionController::updateSteps() {
	Log.Trace("PositionController: updateSteps()");
	this->hardpointActuatorState->Timestamp = this->publisher->getTimestamp();
	bool publishState = false;
	for(int i = 0; i < HP_COUNT; i++) {
		switch(this->hardpointActuatorState->MotionState[i]) {
		case HardpointActuatorMotionStates::Standby:
			this->hardpointActuatorData->StepsCommanded[i] = 0;
			this->hardpointActuatorData->StepsQueued[i] = 0;
			break;
		case HardpointActuatorMotionStates::Chasing:
		{
			float force = this->hardpointActuatorData->MeasuredForce[i];
			int32_t chaseSteps = (int32_t)(force * this->positionControllerSettings->ForceToStepsCoefficient);
			chaseSteps = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, chaseSteps);
			this->hardpointActuatorData->StepsCommanded[i] = (int16_t)chaseSteps;
			break;
		}
		case HardpointActuatorMotionStates::Moving:
		{
			int32_t moveSteps = Range::CoerceIntoRange(-this->scaledMaxStepsPerLoop[i], this->scaledMaxStepsPerLoop[i], this->hardpointActuatorData->StepsQueued[i]);
			this->hardpointActuatorData->StepsQueued[i] -= moveSteps;
			this->hardpointActuatorData->StepsCommanded[i] = (int16_t)moveSteps;
			if(this->hardpointActuatorData->StepsQueued[i] == 0 && this->hardpointActuatorData->StepsCommanded[i] == 0) {
				publishState = true;
				this->hardpointActuatorState->MotionState[i] = HardpointActuatorMotionStates::Standby;
			}
			break;
		}
		}
	}
	if (publishState) {
		this->publisher->tryLogHardpointActuatorState();
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
