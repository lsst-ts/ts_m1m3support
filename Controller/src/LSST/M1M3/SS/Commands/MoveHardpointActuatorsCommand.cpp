/*
 * MoveHardpointActuatorsCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <MoveHardpointActuatorsCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

MoveHardpointActuatorsCommand::MoveHardpointActuatorsCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_MoveHardpointActuatorsC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < 6; i++) {
		this->data.Steps[i] = data->Steps[i];
	}
}

bool MoveHardpointActuatorsCommand::validate() {
	return true;
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
	this->publisher->ackCommandMoveHardpointActuators(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
