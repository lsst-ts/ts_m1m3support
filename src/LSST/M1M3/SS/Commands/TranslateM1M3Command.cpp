#include <TranslateM1M3Command.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TranslateM1M3Command::TranslateM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                           MTM1M3_command_translateM1M3C* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.xTranslation = data->xTranslation;
    this->data.yTranslation = data->yTranslation;
    this->data.zTranslation = data->zTranslation;
    this->data.xRotation = data->xRotation;
    this->data.yRotation = data->yRotation;
    this->data.zRotation = data->zRotation;
}

bool TranslateM1M3Command::validate() { return true; }

void TranslateM1M3Command::execute() { this->context->translateM1M3(this); }

void TranslateM1M3Command::ackInProgress() {
    this->publisher->ackCommandtranslateM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TranslateM1M3Command::ackComplete() {
    this->publisher->ackCommandtranslateM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void TranslateM1M3Command::ackFailed(std::string reason) {
    this->publisher->ackCommandtranslateM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
