/*
 * TurnAirOffCommand.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#include <TurnAirOffCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnAirOffCommand::TurnAirOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOffC* data)
	: SALCommand(context, publisher, commandID, data) { }

void TurnAirOffCommand::execute() {
	this->context->turnAirOff(this);
}

void TurnAirOffCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandTurnAirOff(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
