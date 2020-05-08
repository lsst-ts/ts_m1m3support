#include <EnableHardpointCorrectionsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableHardpointCorrectionsCommand::EnableHardpointCorrectionsCommand(
        Context* context, M1M3SSPublisher* publisher, int32_t commandID,
        MTM1M3_command_enableHardpointCorrectionsC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.enableHardpointCorrections = data->enableHardpointCorrections;
}

bool EnableHardpointCorrectionsCommand::validate() {
    if (!this->data.enableHardpointCorrections) {
        this->publisher->logCommandRejectionWarning("EnableHardpointCorrections",
                                                    "The field EnableHardpointCorrections is not TRUE.");
    }
    return this->data.enableHardpointCorrections;
}

void EnableHardpointCorrectionsCommand::execute() { this->context->enableHardpointCorrections(this); }

void EnableHardpointCorrectionsCommand::ackInProgress() {
    this->publisher->ackCommandenableHardpointCorrections(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnableHardpointCorrectionsCommand::ackComplete() {
    this->publisher->ackCommandenableHardpointCorrections(this->commandID, ACK_COMPLETE, "Completed");
}

void EnableHardpointCorrectionsCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandenableHardpointCorrections(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
