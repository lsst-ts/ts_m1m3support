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

ApplyActiveOpticForcesByBendingModesCommand::ApplyActiveOpticForcesByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyActiveOpticForcesByBendingModesC* data)
  : SALCommand(context, publisher, commandID, data) {}

void ApplyActiveOpticForcesByBendingModesCommand::execute() {
    this->context->applyActiveOpticForcesByBendingModes(this);
}

void ApplyActiveOpticForcesByBendingModesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandApplyActiveOpticForcesByBendingModes(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
