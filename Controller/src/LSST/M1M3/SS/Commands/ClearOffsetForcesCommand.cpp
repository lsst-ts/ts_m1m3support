/*
 * ClearOffsetForcesCommand.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#include <ClearOffsetForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearOffsetForcesCommand::ClearOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearOffsetForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ClearOffsetForces = data->ClearOffsetForces;
}

bool ClearOffsetForcesCommand::validate() {
	if (!this->data.ClearOffsetForces) {
		this->publisher->logCommandRejectionWarning("ClearOffsetForces", "The field ClearOffsetForces is not TRUE.");
	}
	return this->data.ClearOffsetForces;
}

void ClearOffsetForcesCommand::execute() {
	this->context->clearOffsetForces(this);
}

void ClearOffsetForcesCommand::ackInProgress() {
	this->publisher->ackCommandClearOffsetForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearOffsetForcesCommand::ackComplete() {
	this->publisher->ackCommandClearOffsetForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearOffsetForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandClearOffsetForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
