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

ModbusTransmitCommand::ModbusTransmitCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ModbusTransmitC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ActuatorId = data->ActuatorId;
	this->data.FunctionCode = data->FunctionCode;
	for(int i = 0; i < 252; i++) {
		this->data.Data[i] = data->Data[i];
	}
	this->data.DataLength = data->DataLength;
}

bool ModbusTransmitCommand::validate() {
	return true;
}

void ModbusTransmitCommand::execute() {
	this->context->modbusTransmit(this);
}

void ModbusTransmitCommand::ackInProgress() {
	this->publisher->ackCommandModbusTransmit(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ModbusTransmitCommand::ackComplete() {
	this->publisher->ackCommandModbusTransmit(this->commandID, ACK_COMPLETE, "Completed");
}

void ModbusTransmitCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandModbusTransmit(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}


} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
