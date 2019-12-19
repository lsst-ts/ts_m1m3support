/*
 * TurnPowerOffCommand.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#include <TurnPowerOffCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnPowerOffCommand::TurnPowerOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnPowerOffC* data)
        : SALCommand(context, publisher, commandID, data) {}

bool TurnPowerOffCommand::validate() {
    if (!(this->data.turnPowerNetworkAOff ||
          this->data.turnPowerNetworkBOff ||
          this->data.turnPowerNetworkCOff ||
          this->data.turnPowerNetworkDOff ||
          this->data.turnAuxPowerNetworkAOff ||
          this->data.turnAuxPowerNetworkBOff ||
          this->data.turnAuxPowerNetworkCOff ||
          this->data.turnAuxPowerNetworkDOff)) {
        this->ackInvalidParameter("At least one field is not TRUE.");
        return false;
    }
    return true;
}

void TurnPowerOffCommand::execute() {
    this->context->turnPowerOff(this);
}

void TurnPowerOffCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTurnPowerOff(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
