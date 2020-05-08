#include <TurnAirOffCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnAirOffCommand::TurnAirOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                     MTM1M3_command_turnAirOffC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void TurnAirOffCommand::execute() { this->context->turnAirOff(this); }

void TurnAirOffCommand::ackInProgress() {
    this->publisher->ackCommandturnAirOff(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnAirOffCommand::ackComplete() {
    this->publisher->ackCommandturnAirOff(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnAirOffCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandturnAirOff(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
