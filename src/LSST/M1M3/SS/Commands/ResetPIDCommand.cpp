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

ResetPIDCommand::ResetPIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_resetPIDC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.pid = data->pid;
}

bool ResetPIDCommand::validate() {
	if (this->data.pid < 1 || this->data.pid > 6) {
		this->publisher->logCommandRejectionWarning("ResetPID", "The field PID must be in range [1, 6].");
	}
	return this->data.pid >= 1 && this->data.pid <= 6;
}

void ResetPIDCommand::execute() {
	this->context->resetPID(this);
}

void ResetPIDCommand::ackInProgress() {
	this->publisher->ackCommandresetPID(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ResetPIDCommand::ackComplete() {
	this->publisher->ackCommandresetPID(this->commandID, ACK_COMPLETE, "Complete");
}

void ResetPIDCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandresetPID(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
