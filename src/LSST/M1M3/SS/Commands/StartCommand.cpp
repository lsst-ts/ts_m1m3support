/*
 * StartCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <StartCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StartCommand::StartCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_startC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.settingsToApply = data->settingsToApply;
}

bool StartCommand::validate() {
	return true;
}

void StartCommand::execute() {
	this->context->start(this);
}

void StartCommand::ackInProgress() {
	this->publisher->ackCommandstart(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StartCommand::ackComplete() {
	this->publisher->ackCommandstart(this->commandID, ACK_COMPLETE, "Complete");
}

void StartCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandstart(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
