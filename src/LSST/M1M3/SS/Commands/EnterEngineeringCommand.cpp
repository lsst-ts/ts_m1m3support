#include <EnterEngineeringCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnterEngineeringCommand::EnterEngineeringCommand(Context* context, M1M3SSPublisher* publisher,
                                                 int32_t commandID, MTM1M3_command_enterEngineeringC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void EnterEngineeringCommand::execute() { this->context->enterEngineering(this); }

void EnterEngineeringCommand::ackInProgress() {
    this->publisher->ackCommandenterEngineering(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void EnterEngineeringCommand::ackComplete() {
    this->publisher->ackCommandenterEngineering(this->commandID, ACK_COMPLETE, "Completed");
}

void EnterEngineeringCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandenterEngineering(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
