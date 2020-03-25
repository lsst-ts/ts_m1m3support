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

ClearOffsetForcesCommand::ClearOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_clearOffsetForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.clearOffsetForces = data->clearOffsetForces;
}

bool ClearOffsetForcesCommand::validate() {
	if (!this->data.clearOffsetForces) {
		this->publisher->logCommandRejectionWarning("ClearOffsetForces", "The field ClearOffsetForces is not TRUE.");
	}
	return this->data.clearOffsetForces;
}

void ClearOffsetForcesCommand::execute() {
	this->context->clearOffsetForces(this);
}

void ClearOffsetForcesCommand::ackInProgress() {
	this->publisher->ackCommandclearOffsetForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearOffsetForcesCommand::ackComplete() {
	this->publisher->ackCommandclearOffsetForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearOffsetForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandclearOffsetForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
