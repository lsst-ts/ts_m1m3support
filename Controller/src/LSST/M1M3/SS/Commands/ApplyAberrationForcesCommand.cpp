/*
 * ApplyAberrationForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyAberrationForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAberrationForcesCommand::ApplyAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyAberrationForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < FA_COUNT; i++) {
		this->data.zForces[i] = data->zForces[i];
	}
}

bool ApplyAberrationForcesCommand::validate() {
	return true;
}

void ApplyAberrationForcesCommand::execute() {
	this->context->applyAberrationForces(this);
}

void ApplyAberrationForcesCommand::ackInProgress() {
	this->publisher->ackCommandApplyAberrationForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyAberrationForcesCommand::ackComplete() {
	this->publisher->ackCommandApplyAberrationForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyAberrationForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyAberrationForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
