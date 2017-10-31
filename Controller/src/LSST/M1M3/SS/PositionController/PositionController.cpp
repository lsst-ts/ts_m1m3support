/*
 * PositionController.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <PositionController.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionController::PositionController(PositionControllerSettings* positionControllerSettings, m1m3_HardpointDataC* hardpointData) {
	this->positionControllerSettings = positionControllerSettings;
	this->hardpointData = hardpointData;
	this->disableChaseAll();
}

void PositionController::enableChase(int32_t actuatorIndex) {
	this->chasing[actuatorIndex] = true;
}

void PositionController::disableChase(int32_t actuatorIndex) {
	this->chasing[actuatorIndex] = false;
}

void PositionController::enableChaseAll() {
	enableChase(0);
	enableChase(1);
	enableChase(2);
	enableChase(3);
	enableChase(4);
	enableChase(5);
}

void PositionController::disableChaseAll() {
	disableChase(0);
	disableChase(1);
	disableChase(2);
	disableChase(3);
	disableChase(4);
	disableChase(5);
}

void PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {

}

void PositionController::updateSteps() {
	for(int i = 0; i < HP_COUNT; i++) {
		if (this->chasing[i]) {
			float force = this->hardpointData->Force[i];
			int32_t steps = (int32_t)(force * this->positionControllerSettings->ForceToStepsCoefficient);
			if (steps > this->positionControllerSettings->MaxStepsPerLoop) {
				steps = this->positionControllerSettings->MaxStepsPerLoop;
			}
			else if (steps < -this->positionControllerSettings->MaxStepsPerLoop) {
				steps = -this->positionControllerSettings->MaxStepsPerLoop;
			}
			this->hardpointData->StepsCommanded[i] = (int16_t)steps;
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
