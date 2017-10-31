/*
 * AbortRaiseM1M3Command.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <AbortRaiseM1M3Command.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AbortRaiseM1M3Command::AbortRaiseM1M3Command(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_AbortRaiseM1M3C* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.AbortRaiseM1M3 = data->AbortRaiseM1M3;
}

bool AbortRaiseM1M3Command::validate() {
	return true;
}

void AbortRaiseM1M3Command::execute() {
	this->context->abortRaiseM1M3(this);
}

void AbortRaiseM1M3Command::ackInProgress() {
	this->publisher->ackCommandAbortRaiseM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void AbortRaiseM1M3Command::ackComplete() {
	this->publisher->ackCommandAbortRaiseM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void AbortRaiseM1M3Command::ackFailed(std::string reason) {
	this->publisher->ackCommandAbortRaiseM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
