/*
 * PositionM1M3Command.cpp
 *
 *  Created on: Nov 27, 2017
 *      Author: ccontaxis
 */

#include <PositionM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionM1M3Command::PositionM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_PositionM1M3C* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.XPosition = data->XPosition;
	this->data.YPosition = data->YPosition;
	this->data.ZPosition = data->ZPosition;
	this->data.XRotation = data->XRotation;
	this->data.YRotation = data->YRotation;
	this->data.ZRotation = data->ZRotation;
}

bool PositionM1M3Command::validate() {
	return true;
}

void PositionM1M3Command::execute() {
	this->context->positionM1M3(this);
}

void PositionM1M3Command::ackInProgress() {
	this->publisher->ackCommandPositionM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void PositionM1M3Command::ackComplete() {
	this->publisher->ackCommandPositionM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void PositionM1M3Command::ackFailed(std::string reason) {
	this->publisher->ackCommandPositionM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
