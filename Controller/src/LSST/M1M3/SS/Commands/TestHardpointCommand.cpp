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

TestHardpointCommand::TestHardpointCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TestHardpointC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.HardpointActuator = data->HardpointActuator;
}

bool TestHardpointCommand::validate() {
	if (!(this->data.HardpointActuator >= 1 && this->data.HardpointActuator <= 6)) {
		this->publisher->logCommandRejectionWarning("TestHardpoint", "The field HardpointActuator must be in range [1, 6].");
	}
	return this->data.HardpointActuator >= 1 && this->data.HardpointActuator <= 6;
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
