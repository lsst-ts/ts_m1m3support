/*
 * TranslateM1M3Command.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <TranslateM1M3Command.h>
#include <IContext.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TranslateM1M3Command::TranslateM1M3Command(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TranslateM1M3C* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.XTranslation = data->XTranslation;
	this->data.YTranslation = data->YTranslation;
	this->data.ZTranslation = data->ZTranslation;
	this->data.XRotation = data->XRotation;
	this->data.YRotation = data->YRotation;
	this->data.ZRotation = data->ZRotation;
}

bool TranslateM1M3Command::validate() {
	return true;
}

void TranslateM1M3Command::execute() {
	this->context->translateM1M3(this);
}

void TranslateM1M3Command::ackInProgress() {
	this->publisher->ackCommandTranslateM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TranslateM1M3Command::ackComplete() {
	this->publisher->ackCommandTranslateM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void TranslateM1M3Command::ackFailed(std::string reason) {
	this->publisher->ackCommandTranslateM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
