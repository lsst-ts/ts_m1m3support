/*
 * EnableHardpointCorrectionsCommand.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#include <EnableHardpointCorrectionsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointCorrectionsCommand::EnableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_EnableHardpointCorrectionsC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.EnableHardpointCorrections = data->EnableHardpointCorrections;
}

bool EnableHardpointCorrectionsCommand::validate() {
	if (!this->data.EnableHardpointCorrections) {
		this->publisher->logCommandRejectionWarning("EnableHardpointCorrections", "The field EnableHardpointCorrections is not TRUE.");
	}
	return this->data.EnableHardpointCorrections;
}

void EnableHardpointCorrectionsCommand::execute() {
	this->context->enableHardpointCorrections(this);
}

void EnableHardpointCorrectionsCommand::ackInProgress() {
	this->publisher->ackCommandEnableHardpointCorrections(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnableHardpointCorrectionsCommand::ackComplete() {
	this->publisher->ackCommandEnableHardpointCorrections(this->commandID, ACK_COMPLETE, "Completed");
}

void EnableHardpointCorrectionsCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandEnableHardpointCorrections(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
