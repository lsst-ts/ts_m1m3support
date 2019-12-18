/*
 * AbortRaiseM1M3Command.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <AbortRaiseM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AbortRaiseM1M3Command::AbortRaiseM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_abortRaiseM1M3C* data)
        : SALCommand<MTM1M3_command_abortRaiseM1M3C>(context, publisher, commandID, data) {}

void AbortRaiseM1M3Command::execute() {
    this->context->abortRaiseM1M3(this);
}

void AbortRaiseM1M3Command::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandAbortRaiseM1M3(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
