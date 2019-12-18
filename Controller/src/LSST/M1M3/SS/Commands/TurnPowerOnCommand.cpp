/*
 * TurnPowerOnCommand.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#include <TurnPowerOnCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnPowerOnCommand::TurnPowerOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnPowerOnC* data)
        : SALCommand(context, publisher, commandID, data) {}

bool TurnPowerOnCommand::validate() {
    if (!(this->data.turnPowerNetworkAOn ||
          this->data.turnPowerNetworkBOn ||
          this->data.turnPowerNetworkCOn ||
          this->data.turnPowerNetworkDOn ||
          this->data.turnAuxPowerNetworkAOn ||
          this->data.turnAuxPowerNetworkBOn ||
          this->data.turnAuxPowerNetworkCOn ||
          this->data.turnAuxPowerNetworkDOn)) {
        this->ackInvalidParameter("At least one field is not TRUE.");
        return false;
    }
    return true;
}

void TurnPowerOnCommand::execute() {
    this->context->turnPowerOn(this);
}

void TurnPowerOnCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTurnPowerOn(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
