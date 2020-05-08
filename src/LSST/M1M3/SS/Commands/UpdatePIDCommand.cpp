#include <UpdatePIDCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

UpdatePIDCommand::UpdatePIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                   MTM1M3_command_updatePIDC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.pid = data->pid;
    this->data.timestep = data->timestep;
    this->data.p = data->p;
    this->data.i = data->i;
    this->data.d = data->d;
    this->data.n = data->n;
}

bool UpdatePIDCommand::validate() {
    if (this->data.pid < 1 || this->data.pid > 6) {
        this->publisher->logCommandRejectionWarning("UpdatePID", "The field PID must be in range [1, 6].");
    }
    return this->data.pid >= 1 && this->data.pid <= 6;
}

void UpdatePIDCommand::execute() { this->context->updatePID(this); }

void UpdatePIDCommand::ackInProgress() {
    this->publisher->ackCommandupdatePID(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void UpdatePIDCommand::ackComplete() {
    this->publisher->ackCommandupdatePID(this->commandID, ACK_COMPLETE, "Complete");
}

void UpdatePIDCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandupdatePID(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
