/*
 * IContext.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef ICONTEXT_H_
#define ICONTEXT_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class BootCommand;
class StartCommand;
class EnableCommand;
class DisableCommand;
class StandbyCommand;
class ShutdownCommand;
class UpdateCommand;

/*!
 * An interface responsible for the context of a commands execution.
 */
class IContext {
public:
	virtual ~IContext();

	/*!
	 * Executes the boot command against the current state.
	 * @param[in] command The boot command to execute.
	 */
	virtual void boot(BootCommand* command);
	/*!
	 * Executes the start command against the current state.
	 * @param[in] command The start command to execute.
	 */
	virtual void start(StartCommand* command);
	/*!
	 * Executes the enable command against the current state.
	 * @param[in] command The enable command to execute.
	 */
	virtual void enable(EnableCommand* command);
	/*!
	 * Executes the disable command against the current state.
	 * @param[in] command The disable command to execute.
	 */
	virtual void disable(DisableCommand* command);
	/*!
	 * Executes the standby command against the current state.
	 * @param[in] command The standby command to execute.
	 */
	virtual void standby(StandbyCommand* command);
	/*!
	 * Executes the shutdown command against the current state.
	 * @param[in] command The shutdown command to execute.
	 */
	virtual void shutdown(ShutdownCommand* command);
	/*!
	 * Executes the update command against the current state.
	 * @param[in] command The update command to execute.
	 */
	virtual void update(UpdateCommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICONTEXT_H_ */
