/*
 * EnterEngineeringCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <EnterEngineeringCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnterEngineeringCommand::EnterEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enterEngineeringC* data)
  : SALCommand(context, publisher, commandID, data) {}

void EnterEngineeringCommand::execute() {
    this->context->enterEngineering(this);
}

void EnterEngineeringCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandEnterEngineering(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
