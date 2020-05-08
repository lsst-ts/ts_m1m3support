#include <TurnLightsOffCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnLightsOffCommand::TurnLightsOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                           MTM1M3_command_turnLightsOffC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.turnLightsOff = data->turnLightsOff;
}

bool TurnLightsOffCommand::validate() {
    if (!this->data.turnLightsOff) {
        this->publisher->logCommandRejectionWarning("TurnLightsOff", "The field TurnLightsOff is not TRUE.");
    }
    return this->data.turnLightsOff;
}

void TurnLightsOffCommand::execute() { this->context->turnLightsOff(this); }

void TurnLightsOffCommand::ackInProgress() {
    this->publisher->ackCommandturnLightsOff(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnLightsOffCommand::ackComplete() {
    this->publisher->ackCommandturnLightsOff(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnLightsOffCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandturnLightsOff(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
