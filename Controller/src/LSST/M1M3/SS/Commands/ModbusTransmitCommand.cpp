/*
 * ModbusTransmitCommand.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ccontaxis
 */

#include <ModbusTransmitCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ModbusTransmitCommand::ModbusTransmitCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_modbusTransmitC* data)
        : SALCommand(context, publisher, commandID, data) {}

void ModbusTransmitCommand::execute() {
    this->context->modbusTransmit(this);
}

void ModbusTransmitCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandModbusTransmit(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
