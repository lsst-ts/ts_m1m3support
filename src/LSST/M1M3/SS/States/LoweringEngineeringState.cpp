#include <LoweringEngineeringState.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LoweringEngineeringState::LoweringEngineeringState(M1M3SSPublisher* publisher)
        : EngineeringState(publisher, "LoweringEngineeringState") {}

States::Type LoweringEngineeringState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("LoweringEngineeringState: update()");
    this->startTimer();
    States::Type newState = States::NoStateTransition;
    model->getAutomaticOperationsController()->tryDecrementSupportPercentage();
    EnabledState::update(command, model);
    if (model->getAutomaticOperationsController()->checkLowerOperationComplete()) {
        model->getAutomaticOperationsController()->completeLowerOperation();
        newState = States::ParkedEngineeringState;
    } else if (model->getAutomaticOperationsController()->checkLowerOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutLowerOperation();
    }
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
