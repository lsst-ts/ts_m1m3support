/*
 * DisableCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <DisableCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableCommand::DisableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.value = data->value;
}

bool DisableCommand::validate() {
	if (!this->data.value) {
		this->publisher->logCommandRejectionWarning("Disable", "The field Disable is not TRUE.");
	}
	return this->data.value;
}

void DisableCommand::execute() {
	this->context->disable(this);
}

void DisableCommand::ackInProgress() {
	this->publisher->ackCommanddisable(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableCommand::ackComplete() {
	this->publisher->ackCommanddisable(this->commandID, ACK_COMPLETE, "Complete");
}

void DisableCommand::ackFailed(std::string reason) {
	this->publisher->ackCommanddisable(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
