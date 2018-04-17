/*
 * ClearActiveOpticForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ClearActiveOpticForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearActiveOpticForcesCommand::ClearActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearActiveOpticForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ClearActiveOpticForces = data->ClearActiveOpticForces;
}

bool ClearActiveOpticForcesCommand::validate() {
	if (!this->data.ClearActiveOpticForces) {
		this->publisher->logCommandRejectionWarning("ClearActiveOpticForces", "The field ClearActiveOpticForces is not TRUE.");
	}
	return this->data.ClearActiveOpticForces;
}

void ClearActiveOpticForcesCommand::execute() {
	this->context->clearActiveOpticForces(this);
}

void ClearActiveOpticForcesCommand::ackInProgress() {
	this->publisher->ackCommandClearActiveOpticForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearActiveOpticForcesCommand::ackComplete() {
	this->publisher->ackCommandClearActiveOpticForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearActiveOpticForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandClearActiveOpticForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
