/*
 * ApplyOffsetForcesByMirrorForceCommand.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyOffsetForcesByMirrorForceCommand::ApplyOffsetForcesByMirrorForceCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyOffsetForcesByMirrorForceC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.XForce = data->XForce;
	this->data.YForce = data->YForce;
	this->data.ZForce = data->ZForce;
	this->data.XMoment = data->XMoment;
	this->data.YMoment = data->YMoment;
	this->data.ZMoment = data->ZMoment;
}

bool ApplyOffsetForcesByMirrorForceCommand::validate() {
	return true;
}

void ApplyOffsetForcesByMirrorForceCommand::execute() {
	this->context->applyOffsetForcesByMirrorForce(this);
}

void ApplyOffsetForcesByMirrorForceCommand::ackInProgress() {
	this->publisher->ackCommandApplyOffsetForcesByMirrorForce(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyOffsetForcesByMirrorForceCommand::ackComplete() {
	this->publisher->ackCommandApplyOffsetForcesByMirrorForce(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyOffsetForcesByMirrorForceCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandApplyOffsetForcesByMirrorForce(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
