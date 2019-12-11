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

ApplyOffsetForcesCommand::ApplyOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyOffsetForcesC* data)
  : SALCommand(context, publisher, commandID, data) {}

void ApplyOffsetForcesCommand::execute() {
    this->context->applyOffsetForces(this);
}

void ApplyOffsetForcesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandApplyOffsetForces(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
