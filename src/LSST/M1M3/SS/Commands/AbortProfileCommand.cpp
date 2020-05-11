#include <AbortProfileCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AbortProfileCommand::AbortProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                         MTM1M3_command_abortProfileC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void AbortProfileCommand::execute() { this->context->abortProfile(this); }

void AbortProfileCommand::ackInProgress() {
    this->publisher->ackCommandabortProfile(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void AbortProfileCommand::ackComplete() {
    this->publisher->ackCommandabortProfile(this->commandID, ACK_COMPLETE, "Completed");
}

void AbortProfileCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandabortProfile(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
