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

TurnPowerOnCommand::TurnPowerOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnPowerOnC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnPowerNetworkAOn = data->TurnPowerNetworkAOn;
	this->data.TurnPowerNetworkBOn = data->TurnPowerNetworkBOn;
	this->data.TurnPowerNetworkCOn = data->TurnPowerNetworkCOn;
	this->data.TurnPowerNetworkDOn = data->TurnPowerNetworkDOn;
	this->data.TurnAuxPowerNetworkAOn = data->TurnAuxPowerNetworkAOn;
	this->data.TurnAuxPowerNetworkBOn = data->TurnAuxPowerNetworkBOn;
	this->data.TurnAuxPowerNetworkCOn = data->TurnAuxPowerNetworkCOn;
	this->data.TurnAuxPowerNetworkDOn = data->TurnAuxPowerNetworkDOn;
}

bool TurnPowerOnCommand::validate() {
	if (!(this->data.TurnPowerNetworkAOn ||
			this->data.TurnPowerNetworkBOn ||
			this->data.TurnPowerNetworkCOn ||
			this->data.TurnPowerNetworkDOn ||
			this->data.TurnAuxPowerNetworkAOn ||
			this->data.TurnAuxPowerNetworkBOn ||
			this->data.TurnAuxPowerNetworkCOn ||
			this->data.TurnAuxPowerNetworkDOn)) {
		this->publisher->logCommandRejectionWarning("TurnPowerOn", "At least one field is not TRUE.");
	}
	return this->data.TurnPowerNetworkAOn ||
			this->data.TurnPowerNetworkBOn ||
			this->data.TurnPowerNetworkCOn ||
			this->data.TurnPowerNetworkDOn ||
			this->data.TurnAuxPowerNetworkAOn ||
			this->data.TurnAuxPowerNetworkBOn ||
			this->data.TurnAuxPowerNetworkCOn ||
			this->data.TurnAuxPowerNetworkDOn;
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
