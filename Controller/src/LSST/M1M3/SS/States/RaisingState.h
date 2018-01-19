/*
 * RaisingState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef RAISINGSTATE_H_
#define RAISINGSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RaisingState: public EnabledState {
public:
	States::Type update(UpdateCommand* command, Model* model);
	States::Type abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISINGSTATE_H_ */
