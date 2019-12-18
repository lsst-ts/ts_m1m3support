/*
 * TestForceActuatorCommand.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <TestForceActuatorCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestForceActuatorCommand::TestForceActuatorCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testForceActuatorC* data)
        : SALCommand(context, publisher, commandID, data) {}

bool TestForceActuatorCommand::validate() {
    if (!((this->data.forceActuator >= 101 && this->data.forceActuator <= 143) ||
          (this->data.forceActuator >= 207 && this->data.forceActuator <= 243) ||
          (this->data.forceActuator >= 301 && this->data.forceActuator <= 343) ||
          (this->data.forceActuator >= 407 && this->data.forceActuator <= 443))) {
        this->ackInvalidParameter("The field forceActuator must be in range [101, 143] or [207, 243] or [301, 343] or [407, 443].");
        return false;
    }
    return true;
}

void TestForceActuatorCommand::execute() {
    this->context->testForceActuator(this);
}

void TestForceActuatorCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandTestForceActuator(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
