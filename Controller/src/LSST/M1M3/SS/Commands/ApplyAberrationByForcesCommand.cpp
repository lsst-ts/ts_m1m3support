/*
 * ApplyAberrationByForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyAberrationByForcesCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAberrationByForcesCommand::ApplyAberrationByForcesCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ApplyAberrationByForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < FA_COUNT; i++) {
		this->data.ZForces[i] = data->ZForces[i];
	}
}

bool ApplyAberrationByForcesCommand::validate() {
	return true;
}

void ApplyAberrationByForcesCommand::execute() {
	this->context->applyAberrationByForces(this);
}

void ApplyAberrationByForcesCommand::ackInProgress() {
	this->publisher->ackCommandApplyAberrationByForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyAberrationByForcesCommand::ackComplete() {
	this->publisher->ackCommandApplyAberrationByForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyAberrationByForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyAberrationByForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
