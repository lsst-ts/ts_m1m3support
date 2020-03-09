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

ApplyOffsetForcesCommand::ApplyOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyOffsetForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < FA_COUNT; i++) {
		if (i < FA_X_COUNT) {
			this->data.xForces[i] = data->xForces[i];
		}
		if (i < FA_Y_COUNT) {
			this->data.yForces[i] = data->yForces[i];
		}
		this->data.zForces[i] = data->zForces[i];
	}
}

bool ApplyOffsetForcesCommand::validate() {
	return true;
}

void ApplyOffsetForcesCommand::execute() {
	this->context->applyOffsetForces(this);
}

void ApplyOffsetForcesCommand::ackInProgress() {
	this->publisher->ackCommandapplyOffsetForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyOffsetForcesCommand::ackComplete() {
	this->publisher->ackCommandapplyOffsetForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyOffsetForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandapplyOffsetForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
