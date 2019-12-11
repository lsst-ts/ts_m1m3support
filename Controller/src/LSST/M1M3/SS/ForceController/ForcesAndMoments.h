/*
 * ForcesAndMoments.h
 *
 *  Created on: Mar 13, 2018
 *      Author: ccontaxis
 */

#ifndef FORCESANDMOMENTS_H_
#define FORCESANDMOMENTS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForcesAndMoments {
    float Fx;
    float Fy;
    float Fz;
    float Mx;
    float My;
    float Mz;
    float ForceMagnitude;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCESANDMOMENTS_H_ */
