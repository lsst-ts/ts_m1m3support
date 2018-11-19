/*
 * DisableCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <DisableCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableCommand::DisableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableC* data)
	: SALCommand(context, publisher, commandID, data) { }

void DisableCommand::execute() {
	this->context->disable(this);
}

void DisableCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandDisable(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
