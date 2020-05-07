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

ProgramILCCommand::ProgramILCCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_programILCC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.actuatorId = data->actuatorId;
	this->data.filePath = data->filePath;
}

bool ProgramILCCommand::validate() {
	return true;
}

void ProgramILCCommand::execute() {
	this->context->programILC(this);
}

void ProgramILCCommand::ackInProgress() {
	this->publisher->ackCommandprogramILC(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ProgramILCCommand::ackComplete() {
	this->publisher->ackCommandprogramILC(this->commandID, ACK_COMPLETE, "Completed");
}

void ProgramILCCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandprogramILC(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
