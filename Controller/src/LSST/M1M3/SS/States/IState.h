/*
 * IState.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef ISTATE_H_
#define ISTATE_H_

#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IModel;
class BootCommand;
class StartCommand;
class EnableCommand;
class DisableCommand;
class StandbyCommand;
class ShutdownCommand;
class UpdateCommand;
class TurnAirOnCommand;
class TurnAirOffCommand;
class ApplyOffsetForcesCommand;
class ClearOffsetForcesCommand;
class RaiseM1M3Command;
class LowerM1M3Command;

class IState {
public:
	virtual ~IState();

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

#endif /* ISTATE_H_ */
