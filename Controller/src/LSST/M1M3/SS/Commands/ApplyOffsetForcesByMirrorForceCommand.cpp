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

ApplyOffsetForcesByMirrorForceCommand::ApplyOffsetForcesByMirrorForceCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyOffsetForcesByMirrorForceC* data)
	: SALCommand(context, publisher, commandID, data) { }

void ApplyOffsetForcesByMirrorForceCommand::execute() {
	this->context->applyOffsetForcesByMirrorForce(this);
}

void ApplyOffsetForcesByMirrorForceCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandApplyOffsetForcesByMirrorForce(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
