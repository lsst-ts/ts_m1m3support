/*
 * ShutdownCommand.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <ShutdownCommand.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ShutdownCommand::ShutdownCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ShutdownC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.Shutdown = data->Shutdown;
}

bool ShutdownCommand::validate() {
	return this->data.Shutdown;
}

void ShutdownCommand::execute() {
	this->context->shutdown(this);
}

void ShutdownCommand::ackInProgress() {
	this->publisher->ackCommandShutdown(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ShutdownCommand::ackComplete() {
	this->publisher->ackCommandShutdown(this->commandID, ACK_COMPLETE, "Complete");
}

void ShutdownCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandShutdown(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
