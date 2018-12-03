/*
 * DisableHardpointChaseCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <DisableHardpointChaseCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableHardpointChaseCommand::DisableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableHardpointChaseC* data)
	: SALCommand(context, publisher, commandID, data) { }

bool DisableHardpointChaseCommand::validate() {
	if (!(this->data.hardpointActuator >= 0 && this->data.hardpointActuator <= 5)) {
		this->ackInvalidParameter("The field hardpointActuator must be in range [0, 5].");
		return false;
	}
	return true;
}

void DisableHardpointChaseCommand::execute() {
	this->context->disableHardpointChase(this);
}

void DisableHardpointChaseCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandDisableHardpointChase(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
