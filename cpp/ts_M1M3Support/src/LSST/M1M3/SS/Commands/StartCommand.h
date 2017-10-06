/*
 * StartCommand.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STARTCOMMAND_H_
#define STARTCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the standby state to the disabled state.
 * This is an external command and can be issued via SAL.
 */
class StartCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_StartC data;

public:
	StartCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_StartC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_StartC* getData() { return &this->data; }

	virtual void execute();
	virtual void ackInProgress();
	virtual void ackComplete();
	virtual void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STARTCOMMAND_H_ */
