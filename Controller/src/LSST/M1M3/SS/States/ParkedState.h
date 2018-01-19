/*
 * ParkedState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef PARKEDSTATE_H_
#define PARKEDSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ParkedState: public EnabledState {
public:
	States::Type update(UpdateCommand* command, Model* model);
	States::Type raiseM1M3(RaiseM1M3Command* command, Model* model);
	States::Type enterEngineering(EnterEngineeringCommand* command, Model* model);
	States::Type disable(DisableCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDSTATE_H_ */
