/*
 * EnableCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <EnableCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableCommand::EnableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enableC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.value = data->value;
}

bool EnableCommand::validate() {
	if (!this->data.value) {
		this->publisher->logCommandRejectionWarning("enable", "The field value is not TRUE.");
	}
	return this->data.value;
}

void EnableCommand::execute() {
	this->context->enable(this);
}

void EnableCommand::ackInProgress() {
	this->publisher->ackCommandEnable(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnableCommand::ackComplete() {
	this->publisher->ackCommandEnable(this->commandID, ACK_COMPLETE, "Completed");
}

void EnableCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandEnable(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
