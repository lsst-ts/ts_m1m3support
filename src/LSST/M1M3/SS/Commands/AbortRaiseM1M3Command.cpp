#include <AbortRaiseM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AbortRaiseM1M3Command::AbortRaiseM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                             MTM1M3_command_abortRaiseM1M3C*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void AbortRaiseM1M3Command::execute() { this->context->abortRaiseM1M3(this); }

void AbortRaiseM1M3Command::ackInProgress() {
    this->publisher->ackCommandabortRaiseM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void AbortRaiseM1M3Command::ackComplete() {
    this->publisher->ackCommandabortRaiseM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void AbortRaiseM1M3Command::ackFailed(std::string reason) {
    this->publisher->ackCommandabortRaiseM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
