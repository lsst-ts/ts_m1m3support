/*
 * RaisingEngineeringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef RAISINGENGINEERINGSTATE_H_
#define RAISINGENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RaisingEngineeringState: public EngineeringState {
public:
	States::Type update(UpdateCommand* command, Model* model);
	States::Type abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISINGENGINEERINGSTATE_H_ */
