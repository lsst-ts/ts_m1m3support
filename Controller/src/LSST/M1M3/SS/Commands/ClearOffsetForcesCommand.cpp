/*
 * ClearOffsetForcesCommand.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#include <ClearOffsetForcesCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearOffsetForcesCommand::ClearOffsetForcesCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ClearOffsetForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ClearOffsetForces = data->ClearOffsetForces;
}

bool ClearOffsetForcesCommand::validate() {
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
	this->publisher->ackCommandClearOffsetForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
