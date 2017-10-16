/*
 * DisableCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <DisableCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableCommand::DisableCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_DisableC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Disable = data->Disable;
}

bool DisableCommand::validate() {
	return this->data.Disable;
}

void DisableCommand::execute() {
	this->context->disable(this);
}

void DisableCommand::ackInProgress() {
	this->publisher->ackCommandDisable(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableCommand::ackComplete() {
	this->publisher->ackCommandDisable(this->commandID, ACK_COMPLETE, "Complete");
}

void DisableCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandDisable(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
