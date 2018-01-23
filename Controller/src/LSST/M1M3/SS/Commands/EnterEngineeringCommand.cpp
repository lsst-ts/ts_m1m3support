/*
 * EnterEngineeringCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <EnterEngineeringCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnterEngineeringCommand::EnterEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_EnterEngineeringC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.EnterEngineering = data->EnterEngineering;
}

bool EnterEngineeringCommand::validate() {
	return this->data.EnterEngineering;
}

void EnterEngineeringCommand::execute() {
	this->context->enterEngineering(this);
}

void EnterEngineeringCommand::ackInProgress() {
	this->publisher->ackCommandEnterEngineering(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnterEngineeringCommand::ackComplete() {
	this->publisher->ackCommandEnterEngineering(this->commandID, ACK_COMPLETE, "Completed");
}

void EnterEngineeringCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandEnterEngineering(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
