/*
 * PositionM1M3Command.cpp
 *
 *  Created on: Nov 27, 2017
 *      Author: ccontaxis
 */

#include <PositionM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionM1M3Command::PositionM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_positionM1M3C* data)
        : SALCommand(context, publisher, commandID, data) {}

void PositionM1M3Command::execute() {
    this->context->positionM1M3(this);
}

void PositionM1M3Command::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandPositionM1M3(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
