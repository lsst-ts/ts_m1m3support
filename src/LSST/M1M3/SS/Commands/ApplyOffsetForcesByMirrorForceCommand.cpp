#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyOffsetForcesByMirrorForceCommand::ApplyOffsetForcesByMirrorForceCommand(
        Context* context, M1M3SSPublisher* publisher, int32_t commandID,
        MTM1M3_command_applyOffsetForcesByMirrorForceC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.xForce = data->xForce;
    this->data.yForce = data->yForce;
    this->data.zForce = data->zForce;
    this->data.xMoment = data->xMoment;
    this->data.yMoment = data->yMoment;
    this->data.zMoment = data->zMoment;
}

void ApplyOffsetForcesByMirrorForceCommand::execute() { this->context->applyOffsetForcesByMirrorForce(this); }

void ApplyOffsetForcesByMirrorForceCommand::ackInProgress() {
    this->publisher->ackCommandapplyOffsetForcesByMirrorForce(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyOffsetForcesByMirrorForceCommand::ackComplete() {
    this->publisher->ackCommandapplyOffsetForcesByMirrorForce(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyOffsetForcesByMirrorForceCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandapplyOffsetForcesByMirrorForce(this->commandID, ACK_FAILED,
                                                              "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
