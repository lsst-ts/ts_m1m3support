#include <DisableHardpointCorrectionsCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableHardpointCorrectionsCommand::DisableHardpointCorrectionsCommand(
        Context* context, M1M3SSPublisher* publisher, int32_t commandID,
        MTM1M3_command_disableHardpointCorrectionsC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.disableHardpointCorrections = data->disableHardpointCorrections;
}

bool DisableHardpointCorrectionsCommand::validate() {
    if (!this->data.disableHardpointCorrections) {
        this->publisher->logCommandRejectionWarning("DisableHardpointCorrections",
                                                    "The field DisableHardpointCorrections is not TRUE.");
    }
    return this->data.disableHardpointCorrections;
}

void DisableHardpointCorrectionsCommand::execute() { this->context->disableHardpointCorrections(this); }

void DisableHardpointCorrectionsCommand::ackInProgress() {
    this->publisher->ackCommanddisableHardpointCorrections(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableHardpointCorrectionsCommand::ackComplete() {
    this->publisher->ackCommanddisableHardpointCorrections(this->commandID, ACK_COMPLETE, "Completed");
}

void DisableHardpointCorrectionsCommand::ackFailed(std::string reason) {
    this->publisher->ackCommanddisableHardpointCorrections(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
