/*
 * UpdatePIDCommand.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#include <UpdatePIDCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

UpdatePIDCommand::UpdatePIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_UpdatePIDC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Id = data->Id;
	this->data.Timestep = data->Timestep;
	this->data.P = data->P;
	this->data.I = data->I;
	this->data.D = data->D;
	this->data.N = data->N;
}

bool UpdatePIDCommand::validate() {
	if (this->data.Id < 1 || this->data.Id > 6) {
		this->publisher->logCommandRejectionWarning("UpdatePID", "The field Id is must be in range [1, 6].");
	}
	return this->data.Id >= 1 && this->data.Id <= 6;
}

void UpdatePIDCommand::execute() {
	this->context->updatePID(this);
}

void UpdatePIDCommand::ackInProgress() {
	this->publisher->ackCommandUpdatePID(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void UpdatePIDCommand::ackComplete() {
	this->publisher->ackCommandUpdatePID(this->commandID, ACK_COMPLETE, "Complete");
}

void UpdatePIDCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandUpdatePID(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
