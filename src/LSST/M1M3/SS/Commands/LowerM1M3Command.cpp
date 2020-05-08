#include <LowerM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LowerM1M3Command::LowerM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                   MTM1M3_command_lowerM1M3C*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void LowerM1M3Command::execute() { this->context->lowerM1M3(this); }

void LowerM1M3Command::ackInProgress() {
    this->publisher->ackCommandlowerM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void LowerM1M3Command::ackComplete() {
    this->publisher->ackCommandlowerM1M3(this->commandID, ACK_COMPLETE, "Complete");
}

void LowerM1M3Command::ackFailed(std::string reason) {
    this->publisher->ackCommandlowerM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
