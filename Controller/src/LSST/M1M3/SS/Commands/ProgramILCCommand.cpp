/*
 * ProgramILCCommand.cpp
 *
 *  Created on: May 9, 2018
 *      Author: ccontaxis
 */

#include <ProgramILCCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProgramILCCommand::ProgramILCCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_programILCC* data)
	: SALCommand(context, publisher, commandID, data) { }

void ProgramILCCommand::execute() {
	this->context->programILC(this);
}

void ProgramILCCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandProgramILC(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
