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

TurnPowerOffCommand::TurnPowerOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnPowerOffC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TurnPowerNetworkAOff = data->TurnPowerNetworkAOff;
	this->data.TurnPowerNetworkBOff = data->TurnPowerNetworkBOff;
	this->data.TurnPowerNetworkCOff = data->TurnPowerNetworkCOff;
	this->data.TurnPowerNetworkDOff = data->TurnPowerNetworkDOff;
	this->data.TurnAuxPowerNetworkAOff = data->TurnAuxPowerNetworkAOff;
	this->data.TurnAuxPowerNetworkBOff = data->TurnAuxPowerNetworkBOff;
	this->data.TurnAuxPowerNetworkCOff = data->TurnAuxPowerNetworkCOff;
	this->data.TurnAuxPowerNetworkDOff = data->TurnAuxPowerNetworkDOff;
}

bool TurnPowerOffCommand::validate() {
	return this->data.TurnPowerNetworkAOff ||
			this->data.TurnPowerNetworkBOff ||
			this->data.TurnPowerNetworkCOff ||
			this->data.TurnPowerNetworkDOff ||
			this->data.TurnAuxPowerNetworkAOff ||
			this->data.TurnAuxPowerNetworkBOff ||
			this->data.TurnAuxPowerNetworkCOff ||
			this->data.TurnAuxPowerNetworkDOff;
}

void TurnPowerOffCommand::execute() {
	this->context->turnPowerOff(this);
}

void TurnPowerOffCommand::ackInProgress() {
	this->publisher->ackCommandTurnPowerOff(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnPowerOffCommand::ackComplete() {
	this->publisher->ackCommandTurnPowerOff(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnPowerOffCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTurnPowerOff(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
