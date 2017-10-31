/*
 * EnableHardpointChaseCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <EnableHardpointChaseCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointChaseCommand::EnableHardpointChaseCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_EnableHardpointChaseC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < 6; i++) {
		this->data.Steps[i] = data->Steps[i];
	}
}

bool EnableHardpointChaseCommand::validate() {
	return true;
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
	this->publisher->ackCommandEnableHardpointChase(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
