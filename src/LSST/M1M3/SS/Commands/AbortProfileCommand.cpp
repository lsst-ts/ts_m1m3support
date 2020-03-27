/*
 * AbortProfileCommand.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#include <AbortProfileCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AbortProfileCommand::AbortProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_abortProfileC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data = *data;
	this->data.abortProfile = data->abortProfile;
}

bool AbortProfileCommand::validate() {
	if (!this->data.abortProfile) {
		this->publisher->logCommandRejectionWarning("AbortProfile", "The field AbortProfile is not TRUE.");
	}
	return this->data.abortProfile;
}

void AbortProfileCommand::execute() {
	this->context->abortProfile(this);
}

void AbortProfileCommand::ackInProgress() {
	this->publisher->ackCommandabortProfile(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void AbortProfileCommand::ackComplete() {
	this->publisher->ackCommandabortProfile(this->commandID, ACK_COMPLETE, "Completed");
}

void AbortProfileCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandabortProfile(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
