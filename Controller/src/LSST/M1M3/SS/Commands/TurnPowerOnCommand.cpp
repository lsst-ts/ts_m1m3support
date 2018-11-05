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

TurnPowerOnCommand::TurnPowerOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnPowerOnC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.turnPowerNetworkAOn = data->turnPowerNetworkAOn;
	this->data.turnPowerNetworkBOn = data->turnPowerNetworkBOn;
	this->data.turnPowerNetworkCOn = data->turnPowerNetworkCOn;
	this->data.turnPowerNetworkDOn = data->turnPowerNetworkDOn;
	this->data.turnAuxPowerNetworkAOn = data->turnAuxPowerNetworkAOn;
	this->data.turnAuxPowerNetworkBOn = data->turnAuxPowerNetworkBOn;
	this->data.turnAuxPowerNetworkCOn = data->turnAuxPowerNetworkCOn;
	this->data.turnAuxPowerNetworkDOn = data->turnAuxPowerNetworkDOn;
}

bool TurnPowerOnCommand::validate() {
	if (!(this->data.turnPowerNetworkAOn ||
			this->data.turnPowerNetworkBOn ||
			this->data.turnPowerNetworkCOn ||
			this->data.turnPowerNetworkDOn ||
			this->data.turnAuxPowerNetworkAOn ||
			this->data.turnAuxPowerNetworkBOn ||
			this->data.turnAuxPowerNetworkCOn ||
			this->data.turnAuxPowerNetworkDOn)) {
		this->publisher->logCommandRejectionWarning("turnPowerOn", "At least one field is not TRUE.");
	}
	return this->data.turnPowerNetworkAOn ||
			this->data.turnPowerNetworkBOn ||
			this->data.turnPowerNetworkCOn ||
			this->data.turnPowerNetworkDOn ||
			this->data.turnAuxPowerNetworkAOn ||
			this->data.turnAuxPowerNetworkBOn ||
			this->data.turnAuxPowerNetworkCOn ||
			this->data.turnAuxPowerNetworkDOn;
}

void TurnPowerOnCommand::execute() {
	this->context->turnPowerOn(this);
}

void TurnPowerOnCommand::ackInProgress() {
	this->publisher->ackCommandTurnPowerOn(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnPowerOnCommand::ackComplete() {
	this->publisher->ackCommandTurnPowerOn(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnPowerOnCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTurnPowerOn(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
