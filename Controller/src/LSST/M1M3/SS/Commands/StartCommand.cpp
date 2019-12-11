/*
 * StartCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <StartCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StartCommand::StartCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_startC* data)
  : SALCommand(context, publisher, commandID, data) {}

bool StartCommand::validate() {
    // TODO : Check folder exists
    return true;
}

void StartCommand::execute() {
    this->context->start(this);
}

void StartCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandStart(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
