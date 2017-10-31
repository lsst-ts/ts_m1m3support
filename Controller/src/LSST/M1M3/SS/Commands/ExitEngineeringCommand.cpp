/*
 * ExitEngineeringCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <ExitEngineeringCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExitEngineeringCommand::ExitEngineeringCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ExitEngineeringC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Enable = data->Enable;
}

bool ExitEngineeringCommand::validate() {
	return this->data.Enable;
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
