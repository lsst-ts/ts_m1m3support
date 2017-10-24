/*
 * Context.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <IContext.h>
#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IStateFactory;
class IModel;

/*!
 * The context used to execute commands against the current state.
 * This class handles merging the model (has no concept of state) and
 * the state (has no knowledge of how to do things) to provide state
 * based functionality.
 */
class Context: public IContext {
private:
	IStateFactory* stateFactory;
	IModel* model;
	States::Type currentState;

	void updateCurrentStateIfRequired(States::Type potentialNewState);

public:
	Context(IStateFactory* stateFactory, IModel* model);

	void boot(BootCommand* command);
	void start(StartCommand* command);
	void enable(EnableCommand* command);
	void disable(DisableCommand* command);
	void standby(StandbyCommand* command);
	void shutdown(ShutdownCommand* command);
	void update(UpdateCommand* command);
	void turnAirOn(TurnAirOnCommand* command);
	void turnAirOff(TurnAirOffCommand* command);
	void applyOffsetForces(ApplyOffsetForcesCommand* command);
	void clearOffsetForces(ClearOffsetForcesCommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTEXT_H_ */
