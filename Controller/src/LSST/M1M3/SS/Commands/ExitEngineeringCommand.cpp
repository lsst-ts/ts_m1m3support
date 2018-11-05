/*
 * ExitEngineeringCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <ExitEngineeringCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExitEngineeringCommand::ExitEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_exitEngineeringC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.exitEngineering = data->exitEngineering;
}

bool ExitEngineeringCommand::validate() {
	if (!this->data.exitEngineering) {
		this->publisher->logCommandRejectionWarning("exitEngineering", "The field exitEngineering is not TRUE.");
	}
	return this->data.exitEngineering;
}

void ExitEngineeringCommand::execute() {
	this->context->exitEngineering(this);
}

void ExitEngineeringCommand::ackInProgress() {
	this->publisher->ackCommandExitEngineering(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ExitEngineeringCommand::ackComplete() {
	this->publisher->ackCommandExitEngineering(this->commandID, ACK_COMPLETE, "Completed");
}

void ExitEngineeringCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandExitEngineering(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
