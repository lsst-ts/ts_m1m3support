/*
 * TurnAirOnCommand.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#include <TurnAirOnCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnAirOnCommand::TurnAirOnCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TurnAirOnC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnAirOn = data->TurnAirOn;
}

bool TurnAirOnCommand::validate() {
	return this->data.TurnAirOn;
}

void TurnAirOnCommand::execute() {
	this->context->turnAirOn(this);
}

void TurnAirOnCommand::ackInProgress() {
	this->publisher->ackCommandTurnAirOn(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnAirOnCommand::ackComplete() {
	this->publisher->ackCommandTurnAirOn(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnAirOnCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTurnAirOn(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
