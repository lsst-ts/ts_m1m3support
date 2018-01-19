/*
 * TestAirCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <TestAirCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestAirCommand::TestAirCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TestAirC* data) {
	this->context = context;
	this->publisher = publisher;
	this->commandID = commandID;
	this->data.TestAir = data->TestAir;
}

bool TestAirCommand::validate() {
	return this->data.TestAir;
}

void TestAirCommand::execute() {
	this->context->testAir(this);
}

void TestAirCommand::ackInProgress() {
	this->publisher->ackCommandTestAir(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TestAirCommand::ackComplete() {
	this->publisher->ackCommandTestAir(this->commandID, ACK_COMPLETE, "Completed");
}

void TestAirCommand::ackFailed(std::string reason) {
	this->publisher->ackCommandTestAir(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
