/*
 * TurnLightsOffCommand.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: ccontaxis
 */

#include <TurnLightsOffCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnLightsOffCommand::TurnLightsOffCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TurnLightsOffC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnLightsOff = data->TurnLightsOff;
}

bool TurnLightsOffCommand::validate() {
	return this->data.TurnLightsOff;
}

void TurnLightsOffCommand::execute() {
	this->context->turnLightsOff(this);
}

void TurnLightsOffCommand::ackInProgress() {
	this->publisher->ackCommandTurnLightsOff(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnLightsOffCommand::ackComplete() {
	this->publisher->ackCommandTurnLightsOff(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnLightsOffCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTurnLightsOff(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
