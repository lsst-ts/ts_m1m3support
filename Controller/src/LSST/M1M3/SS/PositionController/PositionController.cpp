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
#include <SAL_MTM1M3C.h>
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
	this->appliedHardpointSteps = this->publisher->getEventAppliedHardpointSteps();
	this->hardpointActuatorState = this->publisher->getEventHardpointActuatorState();
	this->hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
		this->appliedHardpointSteps->targetEncoderValues[i] = 0;
		this->appliedHardpointSteps->queuedSteps[i] = 0;
		this->appliedHardpointSteps->commandedSteps[i] = 0;
		this->scaledMaxStepsPerLoop[i] = this->positionControllerSettings->MaxStepsPerLoop;
		this->stableEncoderCount[i] = 0;
	}
	this->publisher->logHardpointActuatorState();
}

double PositionController::getRaiseLowerTimeout() {
	return this->positionControllerSettings->RaiseLowerTimeoutInSeconds;
}

bool PositionController::enableChase(int32_t actuatorID) {
	Log.Info("PositionController: enableChase(%d)", actuatorID);
	if (this->hardpointActuatorState->motionState[actuatorID] != HardpointActuatorMotionStates::Standby) {
		return false;
	}
	this->hardpointActuatorState->motionState[actuatorID] = HardpointActuatorMotionStates::Chasing;
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

void PositionController::disableChase(int32_t actuatorID) {
	Log.Info("PositionController: disableChase(%d)", actuatorID);
	this->hardpointActuatorState->motionState[actuatorID] = HardpointActuatorMotionStates::Standby;
	this->publisher->tryLogHardpointActuatorState();
}

bool PositionController::enableChaseAll() {
	Log.Info("PositionController: enableChaseAll()");
	if (this->hardpointActuatorState->motionState[0] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->motionState[1] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->motionState[2] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->motionState[3] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->motionState[4] != HardpointActuatorMotionStates::Standby ||
			this->hardpointActuatorState->motionState[5] != HardpointActuatorMotionStates::Standby) {
		return false;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Chasing;
	}
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

void PositionController::disableChaseAll() {
	Log.Info("PositionController: disableChaseAll()");
	for(int i = 0; i < HP_COUNT; i++) {
		this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
	}
	this->publisher->tryLogHardpointActuatorState();
}

bool PositionController::forcesInTolerance() {
	Log.Trace("PositionController: forcesInTolerance()");
	bool inTolerance = true;
	for(int i = 0; i < HP_COUNT; i++) {
		inTolerance = inTolerance && Range::InRange((float)this->positionControllerSettings->RaiseLowerForceLimitLow, (float)this->positionControllerSettings->RaiseLowerForceLimitHigh, this->hardpointActuatorData->measuredForce[i]);
	}
	return inTolerance;
}

bool PositionController::motionComplete() {
	Log.Debug("PositionController: motionComplete()");
	return this->hardpointActuatorState->motionState[0] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->motionState[1] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->motionState[2] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->motionState[3] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->motionState[4] == HardpointActuatorMotionStates::Standby &&
			this->hardpointActuatorState->motionState[5] == HardpointActuatorMotionStates::Standby;
}

bool PositionController::move(int32_t* steps) {
	Log.Info("PositionController: move(%d, %d, %d, %d, %d, %d)", steps[0], steps[1], steps[2], steps[3], steps[4], steps[5]);
	if ((this->hardpointActuatorState->motionState[0] != HardpointActuatorMotionStates::Standby && steps[0] != 0) ||
			(this->hardpointActuatorState->motionState[1] != HardpointActuatorMotionStates::Standby && steps[1] != 0) ||
			(this->hardpointActuatorState->motionState[2] != HardpointActuatorMotionStates::Standby && steps[2] != 0) ||
			(this->hardpointActuatorState->motionState[3] != HardpointActuatorMotionStates::Standby && steps[3] != 0) ||
			(this->hardpointActuatorState->motionState[4] != HardpointActuatorMotionStates::Standby && steps[4] != 0) ||
			(this->hardpointActuatorState->motionState[5] != HardpointActuatorMotionStates::Standby && steps[5] != 0)) {
		return false;
	}
	double loopCycles[6];
	double maxLoopCycles = 0;
	for(int i = 0; i < HP_COUNT; i++) {
		this->appliedHardpointSteps->queuedSteps[i] = steps[i];
		this->hardpointActuatorState->motionState[i] = steps[i] != 0 ? HardpointActuatorMotionStates::Stepping : HardpointActuatorMotionStates::Standby;
		loopCycles[i] = this->abs(steps[i]) / (double)this->positionControllerSettings->MaxStepsPerLoop;
		if (loopCycles[i] > maxLoopCycles) {
			maxLoopCycles = loopCycles[i];
		}
	}
	for(int i = 0; i < HP_COUNT; ++i) {
		this->scaledMaxStepsPerLoop[i] = (int32_t)(this->positionControllerSettings->MaxStepsPerLoop /(maxLoopCycles/loopCycles[i]));
		if (this->scaledMaxStepsPerLoop[i] == 0) {
			this->scaledMaxStepsPerLoop[i] = 1;
		}
	}
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

bool PositionController::moveToEncoder(int32_t* encoderValues) {
	Log.Info("PositionController: moveToEncoder(%d, %d, %d, %d, %d, %d)", encoderValues[0], encoderValues[1], encoderValues[2], encoderValues[3], encoderValues[4], encoderValues[5]);
	if ((this->hardpointActuatorState->motionState[0] != HardpointActuatorMotionStates::Standby && encoderValues[0] != this->hardpointActuatorData->encoder[0]) ||
			(this->hardpointActuatorState->motionState[1] != HardpointActuatorMotionStates::Standby && encoderValues[1] != this->hardpointActuatorData->encoder[1]) ||
			(this->hardpointActuatorState->motionState[2] != HardpointActuatorMotionStates::Standby && encoderValues[2] != this->hardpointActuatorData->encoder[2]) ||
			(this->hardpointActuatorState->motionState[3] != HardpointActuatorMotionStates::Standby && encoderValues[3] != this->hardpointActuatorData->encoder[3]) ||
			(this->hardpointActuatorState->motionState[4] != HardpointActuatorMotionStates::Standby && encoderValues[4] != this->hardpointActuatorData->encoder[4]) ||
			(this->hardpointActuatorState->motionState[5] != HardpointActuatorMotionStates::Standby && encoderValues[5] != this->hardpointActuatorData->encoder[5])) {
		return false;
	}
	double loopCycles[6];
	double maxLoopCycles = 0;
	for(int i = 0; i < HP_COUNT; i++) {
		this->appliedHardpointSteps->targetEncoderValues[i] = encoderValues[i];
		this->stableEncoderCount[i] = 0;
		int deltaEncoder = this->appliedHardpointSteps->targetEncoderValues[i] - this->hardpointActuatorData->encoder[i];
		// If we overshoot our target encoder value we have to clear what appears to be quite a bit of backlash
		// So lets not overshoot our target
		if (deltaEncoder > 0) {
			deltaEncoder -= 4;
			// We are already very close to our target so lets not do anything during the quick positioning phase
			if (deltaEncoder < 0) {
				deltaEncoder = 0;
			}
		}
		else if (deltaEncoder < 0) {
			deltaEncoder += 4;
			if (deltaEncoder > 0) {
				deltaEncoder = 0;
			}
		}
		this->appliedHardpointSteps->queuedSteps[i] = deltaEncoder * this->positionControllerSettings->EncoderToStepsCoefficient;
		this->hardpointActuatorState->motionState[i] = this->appliedHardpointSteps->queuedSteps[i] != 0 ? HardpointActuatorMotionStates::QuickPositioning : HardpointActuatorMotionStates::Standby;
		loopCycles[i] = this->abs(this->appliedHardpointSteps->queuedSteps[i]) / (double)this->positionControllerSettings->MaxStepsPerLoop;
		if (loopCycles[i] > maxLoopCycles) {
			maxLoopCycles = loopCycles[i];
		}
	}
	for(int i = 0; i < HP_COUNT; ++i) {
		this->scaledMaxStepsPerLoop[i] = (int32_t)(this->positionControllerSettings->MaxStepsPerLoop /(maxLoopCycles/loopCycles[i]));
		if (this->scaledMaxStepsPerLoop[i] == 0) {
			this->scaledMaxStepsPerLoop[i] = 1;
		}
	}
	this->publisher->tryLogHardpointActuatorState();
	return true;
}

bool PositionController::moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ) {
	Log.Info("PositionController: moveToAbsolute(%f, %f, %f, %f, %f, %f)", x, y, z, rX, rY, rZ);
	int32_t steps[6];
	this->convertToSteps(steps, x, y, z, rX, rY, rZ);
	int32_t encoderValues[6];
	double stepsToEncoder = this->hardpointActuatorSettings->MicrometersPerStep / this->hardpointActuatorSettings->MicrometersPerEncoder;
	for(int i = 0; i < HP_COUNT; ++i) {
		encoderValues[i] = this->hardpointInfo->referencePosition[i] + steps[i] * stepsToEncoder;
	}
	return this->moveToEncoder(encoderValues);
}

bool PositionController::moveToReferencePosition() {
	Log.Info("PositionController: moveToReferencePosition()");
	return this->moveToEncoder(this->hardpointInfo->referencePosition);
}

bool PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {
	Log.Info("PositionController: translate(%f, %f, %f, %f, %f, %f)", x, y, z, rX, rY, rZ);
	int32_t steps[6];
	this->convertToSteps(steps, x, y, z, rX, rY, rZ);
	int32_t encoderValues[6];
	double stepsToEncoder = this->hardpointActuatorSettings->MicrometersPerStep / this->hardpointActuatorSettings->MicrometersPerEncoder;
	for(int i = 0; i < HP_COUNT; ++i) {
		encoderValues[i] = this->hardpointActuatorData->encoder[i] + steps[i] * stepsToEncoder;
	}
	return this->moveToEncoder(encoderValues);
}

void PositionController::stopMotion() {
	Log.Info("PositionController: stopMotion()");
	for(int i = 0; i < HP_COUNT; i++) {
		this->appliedHardpointSteps->queuedSteps[i] = 0;
		this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
	}
	this->publisher->tryLogHardpointActuatorState();
}

void PositionController::updateSteps() {
	Log.Trace("PositionController: updateSteps()");
	bool publishState = false;
	for(int i = 0; i < HP_COUNT; i++) {
		switch(this->hardpointActuatorState->motionState[i]) {
			case HardpointActuatorMotionStates::Standby:
				this->appliedHardpointSteps->commandedSteps[i] = 0;
				this->appliedHardpointSteps->queuedSteps[i] = 0;
				break;
			case HardpointActuatorMotionStates::Chasing:
			{
				float force = this->hardpointActuatorData->measuredForce[i];
				int32_t chaseSteps = (int32_t)(force * this->positionControllerSettings->ForceToStepsCoefficient);
				chaseSteps = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, chaseSteps);
				this->appliedHardpointSteps->commandedSteps[i] = (int16_t)chaseSteps;
				break;
			}
			case HardpointActuatorMotionStates::Stepping:
			{
				int32_t moveSteps = Range::CoerceIntoRange(-this->scaledMaxStepsPerLoop[i], this->scaledMaxStepsPerLoop[i], this->appliedHardpointSteps->queuedSteps[i]);
				this->appliedHardpointSteps->queuedSteps[i] -= moveSteps;
				this->appliedHardpointSteps->commandedSteps[i] = (int16_t)moveSteps;
				if(this->appliedHardpointSteps->queuedSteps[i] == 0 && this->appliedHardpointSteps->commandedSteps[i] == 0) {
					publishState = true;
					this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
				}
				break;
			}
			case HardpointActuatorMotionStates::QuickPositioning:
			{
				int32_t moveSteps = Range::CoerceIntoRange(-this->scaledMaxStepsPerLoop[i], this->scaledMaxStepsPerLoop[i], this->appliedHardpointSteps->queuedSteps[i]);
				this->appliedHardpointSteps->queuedSteps[i] -= moveSteps;
				this->appliedHardpointSteps->commandedSteps[i] = (int16_t)moveSteps;
				if(this->appliedHardpointSteps->queuedSteps[i] == 0 && this->appliedHardpointSteps->commandedSteps[i] == 0) {
					publishState = true;
					this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::FinePositioning;
				}
				break;
			}
			case HardpointActuatorMotionStates::FinePositioning:
			{
				int32_t deltaEncoder = this->appliedHardpointSteps->targetEncoderValues[i] - this->hardpointActuatorData->encoder[i];
				int32_t encoderSteps = (int32_t)(deltaEncoder * this->positionControllerSettings->EncoderToStepsCoefficient);
				if (deltaEncoder <= 2 && deltaEncoder >= -2) {
					if (deltaEncoder < 0) {
						encoderSteps = -1;
						this->stableEncoderCount[i] = 0;
					}
					else if (deltaEncoder > 0) {
						encoderSteps = 1;
						this->stableEncoderCount[i] = 0;

					}
					else {
						encoderSteps = 0;
						this->stableEncoderCount[i]++;
					}
				}
				this->appliedHardpointSteps->commandedSteps[i] = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, encoderSteps);
				if (deltaEncoder == 0 && this->stableEncoderCount[i] >= 2) {
					publishState = true;
					this->appliedHardpointSteps->commandedSteps[i] = 0;
					this->hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
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
