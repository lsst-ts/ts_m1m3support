/*
 * StandbyState.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef STANDBYSTATE_H_
#define STANDBYSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StandbyState: public State {
public:
	virtual States::Type update(UpdateCommand* command, IModel* model);
	virtual States::Type start(StartCommand* command, IModel* model);
	virtual States::Type shutdown(ShutdownCommand* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STANDBYSTATE_H_ */
