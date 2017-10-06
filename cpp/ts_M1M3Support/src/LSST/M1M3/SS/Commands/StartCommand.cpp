/*
 * StartCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <StartCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StartCommand::StartCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_StartC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Start = data->Start;
	this->data.SettingsToApply = data->SettingsToApply;
}

void StartCommand::execute() {
	this->context->start(this);
}

void StartCommand::ackInProgress() {
	this->publisher->ackCommandStart(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StartCommand::ackComplete() {
	this->publisher->ackCommandStart(this->commandID, ACK_COMPLETE, "Complete");
}

void StartCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandStart(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
