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

ExitEngineeringCommand::ExitEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ExitEngineeringC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.ExitEngineering = data->ExitEngineering;
}

bool ExitEngineeringCommand::validate() {
	return this->data.ExitEngineering;
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
	this->publisher->ackCommandExitEngineering(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
