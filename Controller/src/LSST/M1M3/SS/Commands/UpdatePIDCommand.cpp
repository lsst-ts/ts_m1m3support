/*
 * UpdatePIDCommand.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#include <UpdatePIDCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

UpdatePIDCommand::UpdatePIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_updatePIDC* data)
        : SALCommand(context, publisher, commandID, data) {}

bool UpdatePIDCommand::validate() {
    if (this->data.pid < 1 || this->data.pid > 6) {
        this->ackInvalidParameter("The field pid must be in range [1, 6].");
        return false;
    }
    return true;
}

void UpdatePIDCommand::execute() {
    this->context->updatePID(this);
}

void UpdatePIDCommand::ack(int32_t ack, int32_t errorCode, std::string reason) {
    this->publisher->ackCommandUpdatePID(this->commandID, ack, errorCode, reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
