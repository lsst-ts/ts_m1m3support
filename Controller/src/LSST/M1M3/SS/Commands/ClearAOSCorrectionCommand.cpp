/*
 * ClearAOSCorrectionCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ClearAOSCorrectionCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearAOSCorrectionCommand::ClearAOSCorrectionCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ClearAOSCorrectionC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ClearAOSCorrection = data->ClearAOSCorrection;
}

bool ClearAOSCorrectionCommand::validate() {
	return this->data.ClearAOSCorrection;
}

void ClearAOSCorrectionCommand::execute() {
	this->context->clearAOSCorrection(this);
}

void ClearAOSCorrectionCommand::ackInProgress() {
	this->publisher->ackCommandClearAOSCorrection(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearAOSCorrectionCommand::ackComplete() {
	this->publisher->ackCommandClearAOSCorrection(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearAOSCorrectionCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandClearAOSCorrection(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
