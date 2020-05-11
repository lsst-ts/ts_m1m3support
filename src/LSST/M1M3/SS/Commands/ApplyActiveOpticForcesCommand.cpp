#include <ApplyActiveOpticForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyActiveOpticForcesCommand::ApplyActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher,
                                                             int32_t commandID,
                                                             MTM1M3_command_applyActiveOpticForcesC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    for (int i = 0; i < FA_COUNT; i++) {
        this->data.zForces[i] = data->zForces[i];
    }
}

void ApplyActiveOpticForcesCommand::execute() { this->context->applyActiveOpticForces(this); }

void ApplyActiveOpticForcesCommand::ackInProgress() {
    this->publisher->ackCommandapplyActiveOpticForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyActiveOpticForcesCommand::ackComplete() {
    this->publisher->ackCommandapplyActiveOpticForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyActiveOpticForcesCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandapplyActiveOpticForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
