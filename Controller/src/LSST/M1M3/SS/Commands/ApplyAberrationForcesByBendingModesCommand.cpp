/*
 * ApplyAberrationForcesByBendingModesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ApplyAberrationForcesByBendingModesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAberrationForcesByBendingModesCommand::ApplyAberrationForcesByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyAberrationForcesByBendingModesC* data)
  : SALCommand(context, publisher, commandID, data) {}

void ApplyAberrationForcesByBendingModesCommand::execute() {
    this->context->applyAberrationForcesByBendingModes(this);
}

void ApplyAberrationForcesByBendingModesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandApplyAberrationForcesByBendingModes(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
