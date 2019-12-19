/*
 * TurnLightsOffCommand.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: ccontaxis
 */

#include <TurnLightsOffCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnLightsOffCommand::TurnLightsOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnLightsOffC* data)
        : SALCommand(context, publisher, commandID, data) {}

void TurnLightsOffCommand::execute() {
    this->context->turnLightsOff(this);
}

void TurnLightsOffCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTurnLightsOff(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
