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

ApplyActiveOpticForcesCommand::ApplyActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyActiveOpticForcesC* data)
        : SALCommand(context, publisher, commandID, data) {}

void ApplyActiveOpticForcesCommand::execute() {
    this->context->applyActiveOpticForces(this);
}

void ApplyActiveOpticForcesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandApplyActiveOpticForces(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
