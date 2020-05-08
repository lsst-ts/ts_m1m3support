#include <EnableCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnableCommand::EnableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                             MTM1M3_command_enableC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void EnableCommand::execute() { this->context->enable(this); }

void EnableCommand::ackInProgress() {
    this->publisher->ackCommandenable(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnableCommand::ackComplete() {
    this->publisher->ackCommandenable(this->commandID, ACK_COMPLETE, "Completed");
}

void EnableCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandenable(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
