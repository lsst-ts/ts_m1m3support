/*
 * ParkedEngineeringState.h
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
 */

#ifndef PARKEDENGINEERINGSTATE_H_
#define PARKEDENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ParkedEngineeringState : public EngineeringState {
   public:
    ParkedEngineeringState(M1M3SSPublisher* publisher);

    States::Type update(UpdateCommand* command, Model* model);
    States::Type raiseM1M3(RaiseM1M3Command* command, Model* model);
    States::Type exitEngineering(ExitEngineeringCommand* command, Model* model);
    States::Type disable(DisableCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDENGINEERINGSTATE_H_ */
