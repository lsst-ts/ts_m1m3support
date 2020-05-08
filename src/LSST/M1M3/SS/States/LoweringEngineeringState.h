#ifndef LOWERINGENGINEERINGSTATE_H_
#define LOWERINGENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class LoweringEngineeringState : public EngineeringState {
public:
    LoweringEngineeringState(M1M3SSPublisher* publisher);

    States::Type update(UpdateCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERINGENGINEERINGSTATE_H_ */
