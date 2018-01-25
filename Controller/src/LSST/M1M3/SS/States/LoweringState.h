/*
 * LoweringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef LOWERINGSTATE_H_
#define LOWERINGSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class LoweringState: public EnabledState {
public:
	LoweringState(M1M3SSPublisher* publisher);

	States::Type update(UpdateCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERINGSTATE_H_ */
