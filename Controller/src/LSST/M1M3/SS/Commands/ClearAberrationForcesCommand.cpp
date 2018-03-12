/*
 * ClearAberrationForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ClearAberrationForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearAberrationForcesCommand::ClearAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearAberrationForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ClearAberrationForces = data->ClearAberrationForces;
}

bool ClearAberrationForcesCommand::validate() {
	if (!this->data.ClearAberrationForces) {
		this->publisher->logCommandRejectionWarning("ClearAberrationForces", "The field ClearAberrationForces is not TRUE.");
	}
	return this->data.ClearAberrationForces;
}

void ClearAberrationForcesCommand::execute() {
	this->context->clearAberrationForces(this);
}

void ClearAberrationForcesCommand::ackInProgress() {
	this->publisher->ackCommandClearAberrationForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearAberrationForcesCommand::ackComplete() {
	this->publisher->ackCommandClearAberrationForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearAberrationForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandClearAberrationForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
