/*
 * TurnAirOffCommand.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#include <TurnAirOffCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnAirOffCommand::TurnAirOffCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TurnAirOffC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnAirOff = data->TurnAirOff;
}

bool TurnAirOffCommand::validate() {
	return this->data.TurnAirOff;
}

void TurnAirOffCommand::execute() {
	this->context->turnAirOff(this);
}

void TurnAirOffCommand::ackInProgress() {
	this->publisher->ackCommandTurnAirOff(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnAirOffCommand::ackComplete() {
	this->publisher->ackCommandTurnAirOff(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnAirOffCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTurnAirOff(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
