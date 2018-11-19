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

ApplyAberrationForcesCommand::ApplyAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyAberrationForcesC* data)
 : SALCommand(context, publisher, commandID, data) { }

void ApplyAberrationForcesCommand::execute() {
	this->context->applyAberrationForces(this);
}

void ApplyAberrationForcesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandApplyAberrationForces(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
