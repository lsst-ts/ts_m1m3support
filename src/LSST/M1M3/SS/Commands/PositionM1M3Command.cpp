#include <PositionM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PositionM1M3Command::PositionM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                         MTM1M3_command_positionM1M3C* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.xPosition = data->xPosition;
    this->data.yPosition = data->yPosition;
    this->data.zPosition = data->zPosition;
    this->data.xRotation = data->xRotation;
    this->data.yRotation = data->yRotation;
    this->data.zRotation = data->zRotation;
}

bool PositionM1M3Command::validate() { return true; }

void PositionM1M3Command::execute() { this->context->positionM1M3(this); }

void PositionM1M3Command::ackInProgress() {
    this->publisher->ackCommandpositionM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void PositionM1M3Command::ackComplete() {
    this->publisher->ackCommandpositionM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void PositionM1M3Command::ackFailed(std::string reason) {
    this->publisher->ackCommandpositionM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
