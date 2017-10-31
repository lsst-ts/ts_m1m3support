/*
 * DisableHardpointChaseCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <DisableHardpointChaseCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableHardpointChaseCommand::DisableHardpointChaseCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_DisableHardpointChaseC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < 6; i++) {
		this->data.Steps[i] = data->Steps[i];
	}
}

bool DisableHardpointChaseCommand::validate() {
	return true;
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
	this->publisher->ackCommandDisableHardpointChase(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
