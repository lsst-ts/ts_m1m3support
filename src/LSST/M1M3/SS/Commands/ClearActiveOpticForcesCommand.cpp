#include <ClearActiveOpticForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearActiveOpticForcesCommand::ClearActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher,
                                                             int32_t commandID,
                                                             MTM1M3_command_clearActiveOpticForcesC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.clearActiveOpticForces = data->clearActiveOpticForces;
}

bool ClearActiveOpticForcesCommand::validate() {
    if (!this->data.clearActiveOpticForces) {
        this->publisher->logCommandRejectionWarning("ClearActiveOpticForces",
                                                    "The field ClearActiveOpticForces is not TRUE.");
    }
    return this->data.clearActiveOpticForces;
}

void ClearActiveOpticForcesCommand::execute() { this->context->clearActiveOpticForces(this); }

void ClearActiveOpticForcesCommand::ackInProgress() {
    this->publisher->ackCommandclearActiveOpticForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ClearActiveOpticForcesCommand::ackComplete() {
    this->publisher->ackCommandclearActiveOpticForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ClearActiveOpticForcesCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandclearActiveOpticForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
