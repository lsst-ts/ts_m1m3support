/*
 * EngineeringState.h
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#ifndef ENGINEERINGSTATE_H_
#define ENGINEERINGSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class EngineeringState: public EnabledState {
public:
	EngineeringState(M1M3SSPublisher* publisher);
	EngineeringState(M1M3SSPublisher* publisher, std::string name);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENGINEERINGSTATE_H_ */
