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
	States::Type update(UpdateCommand* command, IModel* model);
	States::Type raiseM1M3(RaiseM1M3Command* command, IModel* model);
	States::Type enterEngineering(EnterEngineeringCommand* command, IModel* model);
	States::Type disable(DisableCommand* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDSTATE_H_ */
