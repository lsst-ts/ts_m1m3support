/*
 * ResetPIDCommand.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#include <ResetPIDCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ResetPIDCommand::ResetPIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ResetPIDC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.PID = data->PID;
}

bool ResetPIDCommand::validate() {
	if (this->data.PID < 1 || this->data.PID > 6) {
		this->publisher->logCommandRejectionWarning("ResetPID", "The field PID must be in range [1, 6].");
	}
	return this->data.PID >= 1 && this->data.PID <= 6;
}

void ResetPIDCommand::execute() {
	this->context->resetPID(this);
}

void ResetPIDCommand::ackInProgress() {
	this->publisher->ackCommandResetPID(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ResetPIDCommand::ackComplete() {
	this->publisher->ackCommandResetPID(this->commandID, ACK_COMPLETE, "Complete");
}

void ResetPIDCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandResetPID(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
