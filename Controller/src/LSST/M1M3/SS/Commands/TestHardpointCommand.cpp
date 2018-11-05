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

TestHardpointCommand::TestHardpointCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testHardpointC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.hardpointActuator = data->hardpointActuator;
}

bool TestHardpointCommand::validate() {
	if (!(this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6)) {
		this->publisher->logCommandRejectionWarning("testHardpoint", "The field hardpointActuator must be in range [1, 6].");
	}
	return this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6;
}

void TestHardpointCommand::execute() {
	this->context->testHardpoint(this);
}

void TestHardpointCommand::ackInProgress() {
	this->publisher->ackCommandTestHardpoint(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TestHardpointCommand::ackComplete() {
	this->publisher->ackCommandTestHardpoint(this->commandID, ACK_COMPLETE, "Completed");
}

void TestHardpointCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTestHardpoint(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
