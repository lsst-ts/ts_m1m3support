/*
 * ApplyOffsetForcesCommand.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#include <ApplyOffsetForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyOffsetForcesCommand::ApplyOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyOffsetForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < FA_COUNT; i++) {
		this->data.XForces[i] = data->XForces[i];
		this->data.YForces[i] = data->YForces[i];
		this->data.ZForces[i] = data->ZForces[i];
	}
}

bool ApplyOffsetForcesCommand::validate() {
	return true;
}

void ApplyOffsetForcesCommand::execute() {
	this->context->applyOffsetForces(this);
}

void ApplyOffsetForcesCommand::ackInProgress() {
	this->publisher->ackCommandApplyOffsetForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyOffsetForcesCommand::ackComplete() {
	this->publisher->ackCommandApplyOffsetForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyOffsetForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyOffsetForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
