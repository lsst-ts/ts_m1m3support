/*
 * TestForceActuatorCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <TestForceActuatorCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestForceActuatorCommand::TestForceActuatorCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TestForceActuatorC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ForceActuator = data->ForceActuator;
}

bool TestForceActuatorCommand::validate() {
	if (!((this->data.ForceActuator >= 101 && this->data.ForceActuator <= 143) ||
			(this->data.ForceActuator >= 207 && this->data.ForceActuator <= 243) ||
			(this->data.ForceActuator >= 301 && this->data.ForceActuator <= 343) ||
			(this->data.ForceActuator >= 407 && this->data.ForceActuator <= 443)) ) {
		this->publisher->logCommandRejectionWarning("TestForceActuator", "The field ForceActuator must be in range [101, 143] or [207, 243] or [301, 343] or [407, 443].");
	}
	return (this->data.ForceActuator >= 101 && this->data.ForceActuator <= 143) ||
			(this->data.ForceActuator >= 207 && this->data.ForceActuator <= 243) ||
			(this->data.ForceActuator >= 301 && this->data.ForceActuator <= 343) ||
			(this->data.ForceActuator >= 407 && this->data.ForceActuator <= 443);
}

void TestForceActuatorCommand::execute() {
	this->context->testForceActuator(this);
}

void TestForceActuatorCommand::ackInProgress() {
	this->publisher->ackCommandTestForceActuator(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TestForceActuatorCommand::ackComplete() {
	this->publisher->ackCommandTestForceActuator(this->commandID, ACK_COMPLETE, "Completed");
}

void TestForceActuatorCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTestForceActuator(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
