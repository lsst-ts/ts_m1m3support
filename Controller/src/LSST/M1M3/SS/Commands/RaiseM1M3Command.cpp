/*
 * RaiseM1M3Command.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <RaiseM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaiseM1M3Command::RaiseM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_raiseM1M3C* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.raiseM1M3 = data->raiseM1M3;
	this->data.bypassReferencePosition = data->bypassReferencePosition;
}

bool RaiseM1M3Command::validate() {
	if (!this->data.raiseM1M3) {
		this->publisher->logCommandRejectionWarning("raiseM1M3", "The field raiseM1M3 is not TRUE.");
	}
	return this->data.raiseM1M3;
}

void RaiseM1M3Command::execute() {
	this->context->raiseM1M3(this);
}

void RaiseM1M3Command::ackInProgress() {
	this->publisher->ackCommandRaiseM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void RaiseM1M3Command::ackComplete() {
	this->publisher->ackCommandRaiseM1M3(this->commandID, ACK_COMPLETE, "Complete");
}

void RaiseM1M3Command::ackFailed(std::string reason) {
	this->publisher->ackCommandRaiseM1M3(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
