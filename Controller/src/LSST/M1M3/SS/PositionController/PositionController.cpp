/*
 * PositionController.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <Range.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionController::PositionController(PositionControllerSettings* positionControllerSettings, m1m3_HardpointDataC* hardpointData) {
	this->positionControllerSettings = positionControllerSettings;
	this->hardpointData = hardpointData;
	this->disableChaseAll();
	this->stopMotion();
}

void PositionController::enableChase(int32_t actuatorIndex) {
	this->chasing[actuatorIndex - 1] = true;
}

void PositionController::disableChase(int32_t actuatorIndex) {
	this->chasing[actuatorIndex - 1] = false;
}

void PositionController::enableChaseAll() {
	enableChase(1);
	enableChase(2);
	enableChase(3);
	enableChase(4);
	enableChase(5);
	enableChase(6);
}

void PositionController::disableChaseAll() {
	disableChase(1);
	disableChase(2);
	disableChase(3);
	disableChase(4);
	disableChase(5);
	disableChase(6);
}

void PositionController::move(int32_t* steps) {
	for(int i = 0; i < HP_COUNT; i++) {
		this->steps[i] = steps[i];
	}
}

void PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {

}

void PositionController::stopMotion() {
	for(int i = 0; i < HP_COUNT; i++) {
		this->steps[i] = 0;
	}
}

void PositionController::updateSteps() {
	for(int i = 0; i < HP_COUNT; i++) {
		if (this->chasing[i]) {
			float force = this->hardpointData->Force[i];
			int32_t steps = (int32_t)(force * this->positionControllerSettings->ForceToStepsCoefficient);
			steps = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, steps);
			this->hardpointData->StepsCommanded[i] = (int16_t)steps;
		}
		else {
			int32_t steps = Range::CoerceIntoRange(-this->positionControllerSettings->MaxStepsPerLoop, this->positionControllerSettings->MaxStepsPerLoop, this->steps[i]);
			this->steps[i] -= steps;
			this->hardpointData->StepsCommanded[i] = (int16_t)steps;
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
