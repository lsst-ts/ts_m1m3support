/*
 * ProfileController.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef PROFILECONTROLLER_H_
#define PROFILECONTROLLER_H_

#include <MirrorForceProfile.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ProfileController {
private:
    MirrorForceProfile mirrorForceProfile;

public:
    ProfileController();

    void setupMirrorForceProfile(float* xForce, float* yForce, float* zForce, float* xMoment, float* yMoment,
                                 float* zMoment);
    bool incMirrorForceProfile();
    MirrorForceProfileRecord getMirrorForceProfileData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PROFILECONTROLLER_H_ */
