/*
 * EnableCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <EnableCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableCommand::EnableCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_EnableC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Enable = data->Enable;
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
	this->publisher->ackCommandEnable(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
