/*
 * EnableHardpointChaseCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <EnableHardpointChaseCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointChaseCommand::EnableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enableHardpointChaseC* data)
  : SALCommand(context, publisher, commandID, data) {}

bool EnableHardpointChaseCommand::validate() {
    if (!(this->data.hardpointActuator >= 0 && this->data.hardpointActuator <= 5)) {
        this->ackInvalidParameter("The field hardpointActuator must be in range [0, 5].");
        return false;
    }
    return true;
}

void EnableHardpointChaseCommand::execute() {
    this->context->enableHardpointChase(this);
}

void EnableHardpointChaseCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandEnableHardpointChase(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
