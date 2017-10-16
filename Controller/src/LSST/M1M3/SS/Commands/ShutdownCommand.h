/*
 * ShutdownCommand.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef SHUTDOWNCOMMAND_H_
#define SHUTDOWNCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
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
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_ShutdownC data;

public:
	ShutdownCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ShutdownC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ShutdownC* getData() { return &this->data; }

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
