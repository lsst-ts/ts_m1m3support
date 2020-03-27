/*
 * MirrorForceProfile.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef MIRRORFORCEPROFILE_H_
#define MIRRORFORCEPROFILE_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct MirrorForceProfileRecord {
	float XForce;
	float YForce;
	float ZForce;
	float XMoment;
	float YMoment;
	float ZMoment;
};

struct MirrorForceProfile {
	MirrorForceProfileRecord Records[1000];
	int Index;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MIRRORFORCEPROFILE_H_ */
