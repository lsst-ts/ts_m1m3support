/*
 * StopHardpointMotionCommand.cpp
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#include <StopHardpointMotionCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StopHardpointMotionCommand::StopHardpointMotionCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_StopHardpointMotionC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Start = data->Start;
	this->data.SettingsToApply = data->SettingsToApply;
}

bool StopHardpointMotionCommand::validate() {
	return true;
}

void StopHardpointMotionCommand::execute() {
	this->context->start(this);
}

void StopHardpointMotionCommand::ackInProgress() {
	this->publisher->ackCommandStopHardpointMotion(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StopHardpointMotionCommand::ackComplete() {
	this->publisher->ackCommandStopHardpointMotion(this->commandID, ACK_COMPLETE, "Complete");
}

void StopHardpointMotionCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandStopHardpointMotion(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
