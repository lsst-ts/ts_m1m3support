/*
 * EnableHardpointChaseCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <EnableHardpointChaseCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointChaseCommand::EnableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_EnableHardpointChaseC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.HardpointActuator = data->HardpointActuator;
}

bool EnableHardpointChaseCommand::validate() {
	if (!(this->data.HardpointActuator >= 1 && this->data.HardpointActuator <= 6)) {
		this->publisher->logCommandRejectionWarning("EnableHardpointChase", "The field HardpointActuator must be in range [1, 6].");
	}
	return this->data.HardpointActuator >= 1 && this->data.HardpointActuator <= 6;
}

void EnableHardpointChaseCommand::execute() {
	this->context->enableHardpointChase(this);
}

void EnableHardpointChaseCommand::ackInProgress() {
	this->publisher->ackCommandEnableHardpointChase(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnableHardpointChaseCommand::ackComplete() {
	this->publisher->ackCommandEnableHardpointChase(this->commandID, ACK_COMPLETE, "Completed");
}

void EnableHardpointChaseCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandEnableHardpointChase(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
