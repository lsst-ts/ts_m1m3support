/*
 * ExitEngineeringCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <ExitEngineeringCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExitEngineeringCommand::ExitEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_exitEngineeringC* data)
  : SALCommand(context, publisher, commandID, data) {}

void ExitEngineeringCommand::execute() {
    this->context->exitEngineering(this);
}

void ExitEngineeringCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandExitEngineering(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
