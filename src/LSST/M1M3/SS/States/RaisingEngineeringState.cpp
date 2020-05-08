#include <RaisingEngineeringState.h>
#include <Model.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingEngineeringState::RaisingEngineeringState(M1M3SSPublisher* publisher)
        : EngineeringState(publisher, "RaisingEngineeringState") {}

States::Type RaisingEngineeringState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("RaisingEngineeringState: update()");
    this->startTimer();
    States::Type newState = States::NoStateTransition;
    model->getAutomaticOperationsController()->tryIncrementingSupportPercentage();
    EnabledState::update(command, model);
    if (model->getAutomaticOperationsController()->checkRaiseOperationComplete()) {
        model->getAutomaticOperationsController()->completeRaiseOperation();
        newState = States::ActiveEngineeringState;
    } else if (model->getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutRaiseOperation();
    }
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(newState);
}

States::Type RaisingEngineeringState::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
    spdlog::info("RaisingEngineeringState: abortRaiseM1M3()");
    States::Type newState = States::LoweringEngineeringState;
    model->getAutomaticOperationsController()->abortRaiseM1M3();
    return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
