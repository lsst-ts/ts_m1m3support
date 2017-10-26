/*
 * ApplyAOSCorrectionByForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyAOSCorrectionByForcesCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAOSCorrectionByForcesCommand::ApplyAOSCorrectionByForcesCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ApplyAOSCorrectionByForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < FA_COUNT; i++) {
		this->data.ZForces[i] = data->ZForces[i];
	}
}

bool ApplyAOSCorrectionByForcesCommand::validate() {
	return true;
}

void ApplyAOSCorrectionByForcesCommand::execute() {
	this->context->applyAOSCorrectionByForces(this);
}

void ApplyAOSCorrectionByForcesCommand::ackInProgress() {
	this->publisher->ackCommandApplyAOSCorrectionByForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyAOSCorrectionByForcesCommand::ackComplete() {
	this->publisher->ackCommandApplyAOSCorrectionByForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyAOSCorrectionByForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyAOSCorrectionByForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
