/*
 * MoveHardpointActuatorsCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <MoveHardpointActuatorsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

MoveHardpointActuatorsCommand::MoveHardpointActuatorsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_moveHardpointActuatorsC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < 6; i++) {
		this->data.steps[i] = data->steps[i];
	}
}

bool MoveHardpointActuatorsCommand::validate() {
	if (this->data.steps[0] == 0 && this->data.steps[1] == 0 && this->data.steps[2] == 0 && this->data.steps[3] == 0 && this->data.steps[4] == 0 && this->data.steps[5] == 0) {
		this->publisher->logCommandRejectionWarning("moveHardpointActuators", "The field steps must have at least one index with a step value not equal to zero.");
	}
	return this->data.steps[0] != 0 || this->data.steps[1] != 0 || this->data.steps[2] != 0 || this->data.steps[3] != 0 || this->data.steps[4] != 0 || this->data.steps[5] != 0;
}

void MoveHardpointActuatorsCommand::execute() {
	this->context->moveHardpointActuators(this);
}

void MoveHardpointActuatorsCommand::ackInProgress() {
	this->publisher->ackCommandMoveHardpointActuators(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void MoveHardpointActuatorsCommand::ackComplete() {
	this->publisher->ackCommandMoveHardpointActuators(this->commandID, ACK_COMPLETE, "Completed");
}

void MoveHardpointActuatorsCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandMoveHardpointActuators(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
