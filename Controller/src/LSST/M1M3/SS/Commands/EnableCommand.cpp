/*
 * EnableCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <EnableCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableCommand::EnableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enableC* data)
        : SALCommand(context, publisher, commandID, data) {}

void EnableCommand::execute() {
    this->context->enable(this);
}

void EnableCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandEnable(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
