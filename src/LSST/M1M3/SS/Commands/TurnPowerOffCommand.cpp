#include <TurnPowerOffCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnPowerOffCommand::TurnPowerOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                         MTM1M3_command_turnPowerOffC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.turnPowerNetworkAOff = data->turnPowerNetworkAOff;
    this->data.turnPowerNetworkBOff = data->turnPowerNetworkBOff;
    this->data.turnPowerNetworkCOff = data->turnPowerNetworkCOff;
    this->data.turnPowerNetworkDOff = data->turnPowerNetworkDOff;
    this->data.turnAuxPowerNetworkAOff = data->turnAuxPowerNetworkAOff;
    this->data.turnAuxPowerNetworkBOff = data->turnAuxPowerNetworkBOff;
    this->data.turnAuxPowerNetworkCOff = data->turnAuxPowerNetworkCOff;
    this->data.turnAuxPowerNetworkDOff = data->turnAuxPowerNetworkDOff;
}

bool TurnPowerOffCommand::validate() {
    if (!(this->data.turnPowerNetworkAOff || this->data.turnPowerNetworkBOff ||
          this->data.turnPowerNetworkCOff || this->data.turnPowerNetworkDOff ||
          this->data.turnAuxPowerNetworkAOff || this->data.turnAuxPowerNetworkBOff ||
          this->data.turnAuxPowerNetworkCOff || this->data.turnAuxPowerNetworkDOff)) {
        this->publisher->logCommandRejectionWarning("TurnPowerOff", "At least one field is not TRUE.");
    }
    return this->data.turnPowerNetworkAOff || this->data.turnPowerNetworkBOff ||
           this->data.turnPowerNetworkCOff || this->data.turnPowerNetworkDOff ||
           this->data.turnAuxPowerNetworkAOff || this->data.turnAuxPowerNetworkBOff ||
           this->data.turnAuxPowerNetworkCOff || this->data.turnAuxPowerNetworkDOff;
}

void TurnPowerOffCommand::execute() { this->context->turnPowerOff(this); }

void TurnPowerOffCommand::ackInProgress() {
    this->publisher->ackCommandturnPowerOff(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnPowerOffCommand::ackComplete() {
    this->publisher->ackCommandturnPowerOff(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnPowerOffCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandturnPowerOff(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
