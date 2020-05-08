#include <ShutdownCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ShutdownCommand::ShutdownCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                 MTM1M3_command_shutdownC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.shutdown = data->shutdown;
}

bool ShutdownCommand::validate() {
    if (!this->data.shutdown) {
        this->publisher->logCommandRejectionWarning("Shutdown", "The field Shutdown is not TRUE.");
    }
    return this->data.shutdown;
}

void ShutdownCommand::execute() { this->context->shutdown(this); }

void ShutdownCommand::ackInProgress() {
    this->publisher->ackCommandshutdown(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ShutdownCommand::ackComplete() {
    this->publisher->ackCommandshutdown(this->commandID, ACK_COMPLETE, "Complete");
}

void ShutdownCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandshutdown(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
