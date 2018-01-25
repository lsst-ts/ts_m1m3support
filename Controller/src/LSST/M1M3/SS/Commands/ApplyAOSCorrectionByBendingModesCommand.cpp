/*
 * ApplyAOSCorrectionByBendingModesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyAOSCorrectionByBendingModesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAOSCorrectionByBendingModesCommand::ApplyAOSCorrectionByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyAOSCorrectionByBendingModesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < BENDING_MODES; i++) {
		this->data.Coefficients[i] = data->Coefficients[i];
	}
}

bool ApplyAOSCorrectionByBendingModesCommand::validate() {
	return true;
}

void ApplyAOSCorrectionByBendingModesCommand::execute() {
	this->context->applyAOSCorrectionByBendingModes(this);
}

void ApplyAOSCorrectionByBendingModesCommand::ackInProgress() {
	this->publisher->ackCommandApplyAOSCorrectionByBendingModes(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyAOSCorrectionByBendingModesCommand::ackComplete() {
	this->publisher->ackCommandApplyAOSCorrectionByBendingModes(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyAOSCorrectionByBendingModesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyAOSCorrectionByBendingModes(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
