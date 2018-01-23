/*
 * EnableCommand.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef ENABLECOMMAND_H_
#define ENABLECOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the disabled state to the enabled state.
 * This is an external command and can be issued via SAL.
 */
class EnableCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_EnableC data;

public:
	EnableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_EnableC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_EnableC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENABLECOMMAND_H_ */
