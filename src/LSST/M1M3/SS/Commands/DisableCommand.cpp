#include <DisableCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisableCommand::DisableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                               MTM1M3_command_disableC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void DisableCommand::execute() { this->context->disable(this); }

void DisableCommand::ackInProgress() {
    this->publisher->ackCommanddisable(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableCommand::ackComplete() {
    this->publisher->ackCommanddisable(this->commandID, ACK_COMPLETE, "Complete");
}

void DisableCommand::ackFailed(std::string reason) {
    this->publisher->ackCommanddisable(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
