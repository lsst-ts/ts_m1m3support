/*
 * ActiveState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef ACTIVESTATE_H_
#define ACTIVESTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveState: public EnabledState {
public:
	ActiveState(M1M3SSPublisher* publisher);

	States::Type update(UpdateCommand* command, Model* model);
	States::Type enterEngineering(EnterEngineeringCommand* command, Model* model);
	States::Type lowerM1M3(LowerM1M3Command* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVESTATE_H_ */
