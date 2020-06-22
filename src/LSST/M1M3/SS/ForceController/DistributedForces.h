#ifndef DISTRIBUTEDFORCES_H_
#define DISTRIBUTEDFORCES_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct DistributedForces {
    float XForces[156];
    float YForces[156];
    float ZForces[156];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISTRIBUTEDFORCES_H_ */
