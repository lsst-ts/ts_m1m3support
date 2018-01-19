/*
 * LowerM1M3Command.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <LowerM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LowerM1M3Command::LowerM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_LowerM1M3C* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.LowerM1M3 = data->LowerM1M3;
}

bool LowerM1M3Command::validate() {
	return true;
}

void LowerM1M3Command::execute() {
	this->context->lowerM1M3(this);
}

void LowerM1M3Command::ackInProgress() {
	this->publisher->ackCommandLowerM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void LowerM1M3Command::ackComplete() {
	this->publisher->ackCommandLowerM1M3(this->commandID, ACK_COMPLETE, "Complete");
}

void LowerM1M3Command::ackFailed(std::string reason) {
	this->publisher->ackCommandLowerM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
