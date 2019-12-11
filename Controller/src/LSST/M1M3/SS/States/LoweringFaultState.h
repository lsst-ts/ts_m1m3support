/*
 * LoweringFaultState.h
 *
 *  Created on: Feb 7, 2018
 *      Author: ccontaxis
 */

#ifndef LOWERINGFAULTSTATE_H_
#define LOWERINGFAULTSTATE_H_

#include <FaultState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class UpdateCommand;
class Model;

class LoweringFaultState : public FaultState {
   public:
    LoweringFaultState(M1M3SSPublisher* publisher);

    States::Type update(UpdateCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERINGFAULTSTATE_H_ */
