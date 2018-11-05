/*
 * ShutdownCommand.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef SHUTDOWNCOMMAND_H_
#define SHUTDOWNCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the standby state to the offline state.
 * This is an external command and can be issued via SAL.
 * Once this command has been executed the software will stop running.
 */
class ShutdownCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_shutdownC data;

public:
	ShutdownCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_shutdownC* data);

	MTM1M3_command_shutdownC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SHUTDOWNCOMMAND_H_ */
