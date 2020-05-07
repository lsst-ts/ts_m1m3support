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

DisableHardpointChaseCommand::DisableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableHardpointChaseC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.hardpointActuator = data->hardpointActuator;
}

bool DisableHardpointChaseCommand::validate() {
	if (!(this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6)) {
		this->publisher->logCommandRejectionWarning("DisableHardpointChase", "The field HardpointActuator must be in range [1, 6].");
	}
	return this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6;
}

void DisableHardpointChaseCommand::execute() {
	this->context->disableHardpointChase(this);
}

void DisableHardpointChaseCommand::ackInProgress() {
	this->publisher->ackCommanddisableHardpointChase(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableHardpointChaseCommand::ackComplete() {
	this->publisher->ackCommanddisableHardpointChase(this->commandID, ACK_COMPLETE, "Completed");
}

void DisableHardpointChaseCommand::ackFailed(std::string reason) {
	this->publisher->ackCommanddisableHardpointChase(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
