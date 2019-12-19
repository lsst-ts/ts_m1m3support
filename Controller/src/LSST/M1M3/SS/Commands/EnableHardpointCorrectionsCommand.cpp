/*
 * EnableHardpointCorrectionsCommand.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#include <EnableHardpointCorrectionsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointCorrectionsCommand::EnableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enableHardpointCorrectionsC* data)
        : SALCommand(context, publisher, commandID, data) {}

void EnableHardpointCorrectionsCommand::execute() {
    this->context->enableHardpointCorrections(this);
}

void EnableHardpointCorrectionsCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandEnableHardpointCorrections(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
