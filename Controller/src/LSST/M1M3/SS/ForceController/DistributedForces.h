/*
 * DistributedForces.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef DISTRIBUTEDFORCES_H_
#define DISTRIBUTEDFORCES_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct DistributedForces {
	double XForces[156];
	double YForces[156];
	double ZForces[156];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISTRIBUTEDFORCES_H_ */
