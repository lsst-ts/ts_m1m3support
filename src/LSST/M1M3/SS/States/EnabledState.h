/*
 * EnabledState.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef ENABLEDSTATE_H_
#define ENABLEDSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class EnabledState: public State {
public:
	EnabledState(M1M3SSPublisher* publisher);
	EnabledState(M1M3SSPublisher* publisher, std::string name);

	virtual States::Type update(UpdateCommand* command, Model* model);
	States::Type storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model);
	States::Type storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model);
	States::Type testAir(TestAirCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENABLEDSTATE_H_ */
