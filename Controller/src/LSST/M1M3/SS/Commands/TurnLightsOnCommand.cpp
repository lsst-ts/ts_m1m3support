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

TurnLightsOnCommand::TurnLightsOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnLightsOnC* data)
        : SALCommand(context, publisher, commandID, data) {}

void TurnLightsOnCommand::execute() {
    this->context->turnLightsOn(this);
}

void TurnLightsOnCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTurnLightsOn(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
