/*
 * DisableHardpointCorrectionsCommand.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#include <DisableHardpointCorrectionsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableHardpointCorrectionsCommand::DisableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableHardpointCorrectionsC* data)
	: SALCommand(context, publisher, commandID, data) { }

void DisableHardpointCorrectionsCommand::execute() {
	this->context->disableHardpointCorrections(this);
}

void DisableHardpointCorrectionsCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandDisableHardpointCorrections(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
