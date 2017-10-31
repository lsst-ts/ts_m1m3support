/*
 * StandbyCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <StandbyCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyCommand::StandbyCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_StandbyC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Standby = data->Standby;
}

bool StandbyCommand::validate() {
	return this->data.Standby;
}

void StandbyCommand::execute() {
	this->context->standby(this);
}

void StandbyCommand::ackInProgress() {
	this->publisher->ackCommandStandby(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StandbyCommand::ackComplete() {
	this->publisher->ackCommandStandby(this->commandID, ACK_COMPLETE, "Complete");
}

void StandbyCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandStandby(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
