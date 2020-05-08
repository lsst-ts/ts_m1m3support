#include <StandbyCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyCommand::StandbyCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                               MTM1M3_command_standbyC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void StandbyCommand::execute() { this->context->standby(this); }

void StandbyCommand::ackInProgress() {
    this->publisher->ackCommandstandby(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void StandbyCommand::ackComplete() {
    this->publisher->ackCommandstandby(this->commandID, ACK_COMPLETE, "Complete");
}

void StandbyCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandstandby(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
