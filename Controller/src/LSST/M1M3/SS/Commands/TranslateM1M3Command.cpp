/*
 * TranslateM1M3Command.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <TranslateM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TranslateM1M3Command::TranslateM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_translateM1M3C* data)
	: SALCommand(context, publisher, commandID, data) { }

void TranslateM1M3Command::execute() {
	this->context->translateM1M3(this);
}

void TranslateM1M3Command::ack(int32_t ack, int32_t errorCode, std::string reason) {
	this->publisher->ackCommandTranslateM1M3(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
