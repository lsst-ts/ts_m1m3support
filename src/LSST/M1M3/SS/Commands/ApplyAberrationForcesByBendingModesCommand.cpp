#include <ApplyAberrationForcesByBendingModesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyAberrationForcesByBendingModesCommand::ApplyAberrationForcesByBendingModesCommand(
        Context* context, M1M3SSPublisher* publisher, int32_t commandID,
        MTM1M3_command_applyAberrationForcesByBendingModesC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    for (int i = 0; i < BENDING_MODES; i++) {
        this->data.coefficients[i] = data->coefficients[i];
    }
}

bool ApplyAberrationForcesByBendingModesCommand::validate() { return true; }

void ApplyAberrationForcesByBendingModesCommand::execute() {
    this->context->applyAberrationForcesByBendingModes(this);
}

void ApplyAberrationForcesByBendingModesCommand::ackInProgress() {
    this->publisher->ackCommandapplyAberrationForcesByBendingModes(this->commandID, ACK_INPROGRESS,
                                                                   "In-Progress");
}

void ApplyAberrationForcesByBendingModesCommand::ackComplete() {
    this->publisher->ackCommandapplyAberrationForcesByBendingModes(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyAberrationForcesByBendingModesCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandapplyAberrationForcesByBendingModes(this->commandID, ACK_FAILED,
                                                                   "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
