/*
 * DisableHardpointChaseCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <DisableHardpointChaseCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableHardpointChaseCommand::DisableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_DisableHardpointChaseC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.HardpointActuator = data->HardpointActuator;
}

bool DisableHardpointChaseCommand::validate() {
	if (!(this->data.HardpointActuator >= 1 && this->data.HardpointActuator <= 6)) {
		this->publisher->logCommandRejectionWarning("DisableHardpointChase", "The field HardpointActuator must be in range [1, 6].");
	}
	return this->data.HardpointActuator >= 1 && this->data.HardpointActuator <= 6;
}

void DisableHardpointChaseCommand::execute() {
	this->context->disableHardpointChase(this);
}

void DisableHardpointChaseCommand::ackInProgress() {
	this->publisher->ackCommandDisableHardpointChase(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableHardpointChaseCommand::ackComplete() {
	this->publisher->ackCommandDisableHardpointChase(this->commandID, ACK_COMPLETE, "Completed");
}

void DisableHardpointChaseCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandDisableHardpointChase(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
