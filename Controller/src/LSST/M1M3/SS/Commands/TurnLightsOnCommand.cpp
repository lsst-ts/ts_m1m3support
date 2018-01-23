/*
 * TurnLightsOnCommand.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: ccontaxis
 */

#include <TurnLightsOnCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnLightsOnCommand::TurnLightsOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnLightsOnC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnLightsOn = data->TurnLightsOn;
}

bool TurnLightsOnCommand::validate() {
	return this->data.TurnLightsOn;
}

void TurnLightsOnCommand::execute() {
	this->context->turnLightsOn(this);
}

void TurnLightsOnCommand::ackInProgress() {
	this->publisher->ackCommandTurnLightsOn(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnLightsOnCommand::ackComplete() {
	this->publisher->ackCommandTurnLightsOn(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnLightsOnCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTurnLightsOn(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
