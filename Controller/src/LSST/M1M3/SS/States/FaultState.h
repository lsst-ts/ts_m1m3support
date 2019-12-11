/*
 * FaultState.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef FAULTSTATE_H_
#define FAULTSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class FaultState : public State {
   public:
    FaultState(M1M3SSPublisher* publisher);
    FaultState(M1M3SSPublisher* publisher, std::string name);

    States::Type update(UpdateCommand* command, Model* model);
    States::Type standby(StandbyCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FAULTSTATE_H_ */
