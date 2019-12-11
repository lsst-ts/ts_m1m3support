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

TurnAirOnCommand::TurnAirOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOnC* data)
  : SALCommand(context, publisher, commandID, data) {}

void TurnAirOnCommand::execute() {
    this->context->turnAirOn(this);
}

void TurnAirOnCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTurnAirOn(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
