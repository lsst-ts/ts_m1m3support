/*
 * LoweringEngineeringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef LOWERINGENGINEERINGSTATE_H_
#define LOWERINGENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class LoweringEngineeringState: public EngineeringState {
public:
	States::Type update(UpdateCommand* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERINGENGINEERINGSTATE_H_ */
