#include <ActiveState.h>
#include <Model.h>
#include <Publisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <ForceController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveState::ActiveState(M1M3SSPublisher* publisher) : EnabledState(publisher, "ActiveState") {}

States::Type ActiveState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("ActiveState: update()");
    this->startTimer();
    EnabledState::update(command, model);
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command, Model* model) {
    spdlog::info("ActiveState: enterEngineering()");
    States::Type newState = States::ActiveEngineeringState;
    return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command, Model* model) {
    spdlog::info("ActiveState: lowerM1M3()");
    States::Type newState = States::LoweringState;
    model->getAutomaticOperationsController()->startLowerOperation();
    return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command,
                                                     Model* model) {
    spdlog::info("ActiveState: enableHardpointCorrections()");
    model->getForceController()->applyBalanceForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command,
                                                      Model* model) {
    spdlog::info("ActiveState: disableHardpointCorrections()");
    model->getForceController()->zeroBalanceForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
