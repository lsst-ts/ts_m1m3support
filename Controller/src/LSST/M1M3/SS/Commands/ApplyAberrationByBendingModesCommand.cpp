/*
 * ApplyAberrationByBendingModesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyAberrationByBendingModesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAberrationByBendingModesCommand::ApplyAberrationByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyAberrationByBendingModesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < BENDING_MODES; i++) {
		this->data.Coefficients[i] = data->Coefficients[i];
	}
}

bool ApplyAberrationByBendingModesCommand::validate() {
	return true;
}

void ApplyAberrationByBendingModesCommand::execute() {
	this->context->applyAberrationByBendingModes(this);
}

void ApplyAberrationByBendingModesCommand::ackInProgress() {
	this->publisher->ackCommandApplyAberrationByBendingModes(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyAberrationByBendingModesCommand::ackComplete() {
	this->publisher->ackCommandApplyAberrationByBendingModes(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyAberrationByBendingModesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyAberrationByBendingModes(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
