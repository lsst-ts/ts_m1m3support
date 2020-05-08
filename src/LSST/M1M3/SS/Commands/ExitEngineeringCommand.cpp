#include <ExitEngineeringCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExitEngineeringCommand::ExitEngineeringCommand(Context* context, M1M3SSPublisher* publisher,
                                               int32_t commandID, MTM1M3_command_exitEngineeringC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void ExitEngineeringCommand::execute() { this->context->exitEngineering(this); }

void ExitEngineeringCommand::ackInProgress() {
    this->publisher->ackCommandexitEngineering(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ExitEngineeringCommand::ackComplete() {
    this->publisher->ackCommandexitEngineering(this->commandID, ACK_COMPLETE, "Completed");
}

void ExitEngineeringCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandexitEngineering(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
