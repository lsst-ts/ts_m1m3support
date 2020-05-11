#include <StartCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

#include <SAL_defines.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StartCommand::StartCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                           MTM1M3_command_startC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.settingsToApply = data->settingsToApply;
}

bool StartCommand::validate() {
    if (data.settingsToApply.empty()) {
        publisher->logCommandRejectionWarning("StartCommand", "Command is missing settings argument!");
        return false;
    }
    return true;
}

void StartCommand::execute() { this->context->start(this); }

void StartCommand::ackInProgress() {
    this->publisher->ackCommandstart(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StartCommand::ackComplete() {
    this->publisher->ackCommandstart(this->commandID, ACK_COMPLETE, "Complete");
}

void StartCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandstart(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
