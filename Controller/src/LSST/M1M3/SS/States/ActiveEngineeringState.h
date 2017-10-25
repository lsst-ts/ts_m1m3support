/*
 * ActiveEngineeringState.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef ACTIVEENGINEERINGSTATE_H_
#define ACTIVEENGINEERINGSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveEngineeringState: public State {
public:
	States::Type update(UpdateCommand* command, IModel* model);
	States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model);
	States::Type clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model);
	States::Type lowerM1M3(LowerM1M3Command* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEENGINEERINGSTATE_H_ */
