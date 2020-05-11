#include <RunMirrorForceProfileCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RunMirrorForceProfileCommand::RunMirrorForceProfileCommand(Context* context, M1M3SSPublisher* publisher,
                                                           int32_t commandID,
                                                           MTM1M3_command_runMirrorForceProfileC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    for (int i = 0; i < 1000; ++i) {
        this->data.xForce[i] = data->xForce[i];
        this->data.yForce[i] = data->yForce[i];
        this->data.zForce[i] = data->zForce[i];
        this->data.xMoment[i] = data->xMoment[i];
        this->data.yMoment[i] = data->yMoment[i];
        this->data.zMoment[i] = data->zMoment[i];
    }
}

bool RunMirrorForceProfileCommand::validate() { return true; }

void RunMirrorForceProfileCommand::execute() { this->context->runMirrorForceProfile(this); }

void RunMirrorForceProfileCommand::ackInProgress() {
    this->publisher->ackCommandrunMirrorForceProfile(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void RunMirrorForceProfileCommand::ackComplete() {
    this->publisher->ackCommandrunMirrorForceProfile(this->commandID, ACK_COMPLETE, "Complete");
}

void RunMirrorForceProfileCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandrunMirrorForceProfile(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
