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

StopHardpointMotionCommand::StopHardpointMotionCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_stopHardpointMotionC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.stopHardpointMotion = data->stopHardpointMotion;
}

bool StopHardpointMotionCommand::validate() {
	if (!this->data.stopHardpointMotion) {
		this->publisher->logCommandRejectionWarning("StopHardpointMotion", "The field StopHardpointMotion is not TRUE.");
	}
	return this->data.stopHardpointMotion;
}

void StopHardpointMotionCommand::execute() {
	this->context->stopHardpointMotion(this);
}

void StopHardpointMotionCommand::ackInProgress() {
	this->publisher->ackCommandstopHardpointMotion(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StopHardpointMotionCommand::ackComplete() {
	this->publisher->ackCommandstopHardpointMotion(this->commandID, ACK_COMPLETE, "Complete");
}

void StopHardpointMotionCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandstopHardpointMotion(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
