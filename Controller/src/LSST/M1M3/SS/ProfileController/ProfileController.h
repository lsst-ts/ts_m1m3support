/*
 * ProfileController.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef PROFILECONTROLLER_H_
#define PROFILECONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ProfileController {
private:
	float hardpointCorrectionProfileXForce[1000];
	float hardpointCorrectionProfileYForce[1000];
	float hardpointCorrectionProfileZForce[1000];
	float hardpointCorrectionProfileXMoment[1000];
	float hardpointCorrectionProfileYMoment[1000];
	float hardpointCorrectionProfileZMoment[1000];
	int hardpointCorrectionProfileIndex;

public:
	void setupHardpointCorrectionProfile(float* xForce, float* yForce, float* zForce, float* xMoment, float* yMoment, float* zMoment);
	bool incHardpointCorrectionProfile();
	HardpointCorrectionProfileForce getHardpointCorrectionProfileData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PROFILECONTROLLER_H_ */
