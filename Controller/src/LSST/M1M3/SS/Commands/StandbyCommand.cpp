/*
 * StandbyCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <StandbyCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyCommand::StandbyCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_standbyC* data)
	: SALCommand(context, publisher, commandID, data) { }

void StandbyCommand::execute() {
	this->context->standby(this);
}

void StandbyCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandStandby(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
