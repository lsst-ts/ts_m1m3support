/*
 * TurnAirOnCommand.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#include <TurnAirOnCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnAirOnCommand::TurnAirOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnAirOnC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnAirOn = data->TurnAirOn;
}

bool TurnAirOnCommand::validate() {
	if (!this->data.TurnAirOn) {
		this->publisher->logCommandRejectionWarning("TurnAirOn", "The field TurnAirOn is not TRUE.");
	}
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
	this->publisher->ackCommandTurnAirOn(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
