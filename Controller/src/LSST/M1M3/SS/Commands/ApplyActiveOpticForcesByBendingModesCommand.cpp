/*
 * ApplyActiveOpticForcesByBendingModesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyActiveOpticForcesByBendingModesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyActiveOpticForcesByBendingModesCommand::ApplyActiveOpticForcesByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyActiveOpticForcesByBendingModesC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < BENDING_MODES; i++) {
		this->data.coefficients[i] = data->coefficients[i];
	}
}

bool ApplyActiveOpticForcesByBendingModesCommand::validate() {
	return true;
}

void ApplyActiveOpticForcesByBendingModesCommand::execute() {
	this->context->applyActiveOpticForcesByBendingModes(this);
}

void ApplyActiveOpticForcesByBendingModesCommand::ackInProgress() {
	this->publisher->ackCommandapplyActiveOpticForcesByBendingModes(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyActiveOpticForcesByBendingModesCommand::ackComplete() {
	this->publisher->ackCommandapplyActiveOpticForcesByBendingModes(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyActiveOpticForcesByBendingModesCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandapplyActiveOpticForcesByBendingModes(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
