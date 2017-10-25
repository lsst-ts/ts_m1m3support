/*
 * State.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATE_H_
#define STATE_H_

#include <IState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class State: public IState {
public:
	virtual States::Type boot(BootCommand* command, IModel* model);
	virtual States::Type start(StartCommand* command, IModel* model);
	virtual States::Type enable(EnableCommand* command, IModel* model);
	virtual States::Type disable(DisableCommand* command, IModel* model);
	virtual States::Type standby(StandbyCommand* command, IModel* model);
	virtual States::Type shutdown(ShutdownCommand* command, IModel* model);
	virtual States::Type update(UpdateCommand* command, IModel* model);
	virtual States::Type turnAirOn(TurnAirOnCommand* command, IModel* model);
	virtual States::Type turnAirOff(TurnAirOffCommand* command, IModel* model);
	virtual States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model);
	virtual States::Type clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model);
	virtual States::Type raiseM1M3(RaiseM1M3Command* command, IModel* model);
	virtual States::Type lowerM1M3(LowerM1M3Command* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATE_H_ */
