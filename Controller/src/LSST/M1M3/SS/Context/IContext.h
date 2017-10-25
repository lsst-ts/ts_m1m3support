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
class TurnAirOnCommand;
class TurnAirOffCommand;
class ApplyOffsetForcesCommand;
class ClearOffsetForcesCommand;
class RaiseM1M3Command;
class LowerM1M3Command;

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
	/*!
	 * Executes the command to turn air on.
	 * @param[in] command The turn on air command to execute.
	 */
	virtual void turnAirOn(TurnAirOnCommand* command);
	/*!
	 * Executes the command to turn air off.
	 * @param[in] command The turn off air command to execute.
	 */
	virtual void turnAirOff(TurnAirOffCommand* command);
	/*!
	 * Executes the command to apply offset forces.
	 * @param[in] command The apply offset forces command to execute.
	 */
	virtual void applyOffsetForces(ApplyOffsetForcesCommand* command);
	/*!
	 * Executes the command to clear offset forces.
	 * @param[in] command The clear offset forces command to execute.
	 */
	virtual void clearOffsetForces(ClearOffsetForcesCommand* command);
	/*!
	 * Executes the command to raise M1M3.
	 * @param[in] command The raise M1M3 command to execute.
	 */
	virtual void raiseM1M3(RaiseM1M3Command* command);
	/*!
	 * Executes the command to lower M1M3.
	 * @param[in] command The lower M1M3 command to execute.
	 */
	virtual void lowerM1M3(LowerM1M3Command* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICONTEXT_H_ */
