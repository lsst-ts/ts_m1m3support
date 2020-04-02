#include <RaisingState.h>
#include <Model.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingState::RaisingState(M1M3SSPublisher* publisher) : EnabledState(publisher, "RaisingState") {}

States::Type RaisingState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("RaisingState: update()");
    this->startTimer();
    States::Type newState = States::NoStateTransition;
    model->getAutomaticOperationsController()->tryIncrementingSupportPercentage();
    EnabledState::update(command, model);
    if (model->getAutomaticOperationsController()->checkRaiseOperationComplete()) {
        model->getAutomaticOperationsController()->completeRaiseOperation();
        newState = States::ActiveState;
    } else if (model->getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutRaiseOperation();
    }
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(newState);
}

States::Type RaisingState::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
    spdlog::info("RaisingState: abortRaiseM1M3()");
    States::Type newState = States::LoweringState;
    model->getAutomaticOperationsController()->abortRaiseM1M3();
    return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
