/*
 * StopHardpointMotionCommand.cpp
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#include <StopHardpointMotionCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StopHardpointMotionCommand::StopHardpointMotionCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_StopHardpointMotionC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.StopHardpointMotion = data->StopHardpointMotion;
}

bool StopHardpointMotionCommand::validate() {
	return this->data.StopHardpointMotion;
}

void StopHardpointMotionCommand::execute() {
	this->context->stopHardpointMotion(this);
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
