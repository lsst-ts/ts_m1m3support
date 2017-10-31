/*
 * TestHardpointCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef TESTHARDPOINTCOMMAND_H_
#define TESTHARDPOINTCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestHardpointCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_TestHardpointC data;

public:
	TestHardpointCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TestHardpointC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_TestHardpointC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTHARDPOINTCOMMAND_H_ */
