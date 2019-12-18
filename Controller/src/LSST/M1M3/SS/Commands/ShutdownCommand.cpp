/*
 * ShutdownCommand.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <ShutdownCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ShutdownCommand::ShutdownCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_shutdownC* data)
        : SALCommand(context, publisher, commandID, data) {}

void ShutdownCommand::execute() {
    this->context->shutdown(this);
}

void ShutdownCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandShutdown(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
