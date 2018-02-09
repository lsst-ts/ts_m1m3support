/*
 * HardpointCorrectionProfileForce.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef HARDPOINTCORRECTIONPROFILEFORCE_H_
#define HARDPOINTCORRECTIONPROFILEFORCE_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct HardpointCorrectionProfileForce {
	float XForce;
	float YForce;
	float ZForce;
	float XMoment;
	float YMoment;
	float ZMoment;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTCORRECTIONPROFILEFORCE_H_ */
