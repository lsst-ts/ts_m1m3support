/*
 * RunHardpointCorrectionProfileCommand.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#include <RunHardpointCorrectionProfileCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RunHardpointCorrectionProfileCommand::RunHardpointCorrectionProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_RunHardpointCorrectionProfileC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	for(int i = 0; i < 1000; ++i) {
		this->data.XForce[i] = data->XForce[i];
		this->data.YForce[i] = data->YForce[i];
		this->data.ZForce[i] = data->ZForce[i];
		this->data.XMoment[i] = data->XMoment[i];
		this->data.YMoment[i] = data->YMoment[i];
		this->data.ZMoment[i] = data->ZMoment[i];
	}
}

bool RunHardpointCorrectionProfileCommand::validate() {
	return true;
}

void RunHardpointCorrectionProfileCommand::execute() {
	this->context->runHardpointCorrectionProfile(this);
}

void RunHardpointCorrectionProfileCommand::ackInProgress() {
	this->publisher->ackCommandRunHardpointCorrectionProfile(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void RunHardpointCorrectionProfileCommand::ackComplete() {
	this->publisher->ackCommandRunHardpointCorrectionProfile(this->commandID, ACK_COMPLETE, "Complete");
}

void RunHardpointCorrectionProfileCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandRunHardpointCorrectionProfile(this->commandID, ACK_COMPLETE, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
