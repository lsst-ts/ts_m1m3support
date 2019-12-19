/*
 * ClearActiveOpticForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ClearActiveOpticForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearActiveOpticForcesCommand::ClearActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_clearActiveOpticForcesC* data)
        : SALCommand(context, publisher, commandID, data) {}

void ClearActiveOpticForcesCommand::execute() {
    this->context->clearActiveOpticForces(this);
}

void ClearActiveOpticForcesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandClearActiveOpticForces(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
