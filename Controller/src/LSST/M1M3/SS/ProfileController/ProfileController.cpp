/*
 * ProfileController.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#include "ProfileController.h"

namespace LSST {
namespace M1M3 {
namespace SS {

void ProfileController::setupHardpointCorrectionProfile(float* xForce, float* yForce, float* zForce, float* xMoment, float* yMoment, float* zMoment) {
	this->hardpointCorrectionProfileIndex = 0;
	for (int i = 0; i < 1000; ++i) {
		this->hardpointCorrectionProfileXForce[i] = xForce[i];
		this->hardpointCorrectionProfileYForce[i] = yForce[i];
		this->hardpointCorrectionProfileZForce[i] = zForce[i];
		this->hardpointCorrectionProfileXMoment[i] = xMoment[i];
		this->hardpointCorrectionProfileYMoment[i] = yMoment[i];
		this->hardpointCorrectionProfileZMoment[i] = zMoment[i];
	}
}

bool ProfileController::incHardpointCorrectionProfile() {
	++this->hardpointCorrectionProfileIndex;
	return this->hardpointCorrectionProfileIndex >= 1000;
}

HardpointCorrectionProfileForce ProfileController::getHardpointCorrectionProfileData() {
	HardpointCorrectionProfileForce data;
	data.XForce = this->hardpointCorrectionProfileXForce[this->hardpointCorrectionProfileIndex];
	data.YForce = this->hardpointCorrectionProfileYForce[this->hardpointCorrectionProfileIndex];
	data.ZForce = this->hardpointCorrectionProfileZForce[this->hardpointCorrectionProfileIndex];
	data.XMoment = this->hardpointCorrectionProfileXMoment[this->hardpointCorrectionProfileIndex];
	data.YMoment = this->hardpointCorrectionProfileYMoment[this->hardpointCorrectionProfileIndex];
	data.ZMoment = this->hardpointCorrectionProfileZMoment[this->hardpointCorrectionProfileIndex];
	return data;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
