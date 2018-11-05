/*
 * ApplyActiveOpticForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyActiveOpticForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyActiveOpticForcesCommand::ApplyActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyActiveOpticForcesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < FA_COUNT; i++) {
		this->data.zForces[i] = data->zForces[i];
	}
}

bool ApplyActiveOpticForcesCommand::validate() {
	return true;
}

void ApplyActiveOpticForcesCommand::execute() {
	this->context->applyActiveOpticForces(this);
}

void ApplyActiveOpticForcesCommand::ackInProgress() {
	this->publisher->ackCommandApplyActiveOpticForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyActiveOpticForcesCommand::ackComplete() {
	this->publisher->ackCommandApplyActiveOpticForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyActiveOpticForcesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyActiveOpticForces(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
