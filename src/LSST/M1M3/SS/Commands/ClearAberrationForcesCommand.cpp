#include <ClearAberrationForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearAberrationForcesCommand::ClearAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher,
                                                           int32_t commandID,
                                                           MTM1M3_command_clearAberrationForcesC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void ClearAberrationForcesCommand::execute() { this->context->clearAberrationForces(this); }

void ClearAberrationForcesCommand::ackInProgress() {
    this->publisher->ackCommandclearAberrationForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearAberrationForcesCommand::ackComplete() {
    this->publisher->ackCommandclearAberrationForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearAberrationForcesCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandclearAberrationForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
