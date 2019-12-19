/*
 * ClearAberrationForcesCommand.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ClearAberrationForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearAberrationForcesCommand::ClearAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_clearAberrationForcesC* data)
        : SALCommand(context, publisher, commandID, data) {}

void ClearAberrationForcesCommand::execute() {
    this->context->clearAberrationForces(this);
}

void ClearAberrationForcesCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandClearAberrationForces(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
