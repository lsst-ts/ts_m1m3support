/*
 * DisableHardpointCorrectionsCommand.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#include <DisableHardpointCorrectionsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableHardpointCorrectionsCommand::DisableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_DisableHardpointCorrectionsC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.DisableHardpointCorrections = data->DisableHardpointCorrections;
}

bool DisableHardpointCorrectionsCommand::validate() {
	if (!this->data.DisableHardpointCorrections) {
		this->publisher->logCommandRejectionWarning("DisableHardpointCorrections", "The field DisableHardpointCorrections is not TRUE.");
	}
	return this->data.DisableHardpointCorrections;
}

void DisableHardpointCorrectionsCommand::execute() {
	this->context->disableHardpointCorrections(this);
}

void DisableHardpointCorrectionsCommand::ackInProgress() {
	this->publisher->ackCommandDisableHardpointCorrections(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableHardpointCorrectionsCommand::ackComplete() {
	this->publisher->ackCommandDisableHardpointCorrections(this->commandID, ACK_COMPLETE, "Completed");
}

void DisableHardpointCorrectionsCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandDisableHardpointCorrections(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
