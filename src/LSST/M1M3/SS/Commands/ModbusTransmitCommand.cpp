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

ModbusTransmitCommand::ModbusTransmitCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_modbusTransmitC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.actuatorId = data->actuatorId;
	this->data.functionCode = data->functionCode;
	for(int i = 0; i < 252; i++) {
		this->data.data[i] = data->data[i];
	}
	this->data.dataLength = data->dataLength;
}

bool ModbusTransmitCommand::validate() {
	return true;
}

void ModbusTransmitCommand::execute() {
	this->context->modbusTransmit(this);
}

void ModbusTransmitCommand::ackInProgress() {
	this->publisher->ackCommandmodbusTransmit(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ModbusTransmitCommand::ackComplete() {
	this->publisher->ackCommandmodbusTransmit(this->commandID, ACK_COMPLETE, "Completed");
}

void ModbusTransmitCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandmodbusTransmit(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}


} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
