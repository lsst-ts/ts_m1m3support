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

	virtual void boot(BootCommand* command);
	virtual void start(StartCommand* command);
	virtual void enable(EnableCommand* command);
	virtual void disable(DisableCommand* command);
	virtual void standby(StandbyCommand* command);
	virtual void shutdown(ShutdownCommand* command);
	virtual void update(UpdateCommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTEXT_H_ */
