/*
 * ClearAberrationCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ClearAberrationCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearAberrationCommand::ClearAberrationCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ClearAberrationC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ClearAberration = data->ClearAberration;
}

bool ClearAberrationCommand::validate() {
	return this->data.ClearAberration;
}

void ClearAberrationCommand::execute() {
	this->context->clearAberration(this);
}

void ClearAberrationCommand::ackInProgress() {
	this->publisher->ackCommandClearAberration(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearAberrationCommand::ackComplete() {
	this->publisher->ackCommandClearAberration(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearAberrationCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandClearAberration(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
