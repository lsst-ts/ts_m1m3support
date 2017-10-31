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
	this->data.ActuatorId = data->ActuatorId;
}

bool EnableHardpointChaseCommand::validate() {
	return this->data.ActuatorId >= 1 && this->data.ActuatorId <= 6;
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
