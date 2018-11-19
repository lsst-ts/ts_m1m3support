/*
 * TestHardpointCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <TestHardpointCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestHardpointCommand::TestHardpointCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testHardpointC* data)
	: SALCommand(context, publisher, commandID, data) { }

bool TestHardpointCommand::validate() {
	if (!(this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6)) {
		this->ackInvalidParameter("The field hardpointActuator must be in range [1, 6].");
		return false;
	}
	return true;
}

void TestHardpointCommand::execute() {
	this->context->testHardpoint(this);
}

void TestHardpointCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandTestHardpoint(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
