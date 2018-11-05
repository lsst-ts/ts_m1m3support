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

TurnAirOnCommand::TurnAirOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOnC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.turnAirOn = data->turnAirOn;
}

bool TurnAirOnCommand::validate() {
	if (!this->data.turnAirOn) {
		this->publisher->logCommandRejectionWarning("turnAirOn", "The field turnAirOn is not TRUE.");
	}
	return this->data.turnAirOn;
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
