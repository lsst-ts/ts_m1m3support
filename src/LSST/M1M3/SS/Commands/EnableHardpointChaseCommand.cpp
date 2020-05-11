#include <EnableHardpointChaseCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointChaseCommand::EnableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher,
                                                         int32_t commandID,
                                                         MTM1M3_command_enableHardpointChaseC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.hardpointActuator = data->hardpointActuator;
}

bool EnableHardpointChaseCommand::validate() {
    if (!(this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6)) {
        this->publisher->logCommandRejectionWarning("EnableHardpointChase",
                                                    "The field HardpointActuator must be in range [1, 6].");
    }
    return this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6;
}

void EnableHardpointChaseCommand::execute() { this->context->enableHardpointChase(this); }

void EnableHardpointChaseCommand::ackInProgress() {
    this->publisher->ackCommandenableHardpointChase(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnableHardpointChaseCommand::ackComplete() {
    this->publisher->ackCommandenableHardpointChase(this->commandID, ACK_COMPLETE, "Completed");
}

void EnableHardpointChaseCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandenableHardpointChase(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
