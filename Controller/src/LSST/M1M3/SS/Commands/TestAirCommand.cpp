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

TestAirCommand::TestAirCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testAirC* data)
        : SALCommand(context, publisher, commandID, data) {}

void TestAirCommand::execute() {
    this->context->testAir(this);
}

void TestAirCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTestAir(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
