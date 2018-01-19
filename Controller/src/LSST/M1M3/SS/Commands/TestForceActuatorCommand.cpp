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
	this->data.ActuatorId = data->ActuatorId;
}

bool TestForceActuatorCommand::validate() {
	return (this->data.ActuatorId >= 101 && this->data.ActuatorId <= 143) ||
			(this->data.ActuatorId >= 207 && this->data.ActuatorId <= 243) ||
			(this->data.ActuatorId >= 301 && this->data.ActuatorId <= 343) ||
			(this->data.ActuatorId >= 407 && this->data.ActuatorId <= 434);
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
	this->publisher->ackCommandTestForceActuator(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
