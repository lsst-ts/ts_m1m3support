/*
 * LowerM1M3Command.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <LowerM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LowerM1M3Command::LowerM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_lowerM1M3C* data)
  : SALCommand(context, publisher, commandID, data) {}

void LowerM1M3Command::execute() {
    this->context->lowerM1M3(this);
}

void LowerM1M3Command::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandLowerM1M3(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
