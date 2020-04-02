#include <ParkedState.h>
#include <ILC.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceController.h>
#include <PositionController.h>
#include <AutomaticOperationsController.h>
#include <RaiseM1M3Command.h>
#include <DigitalInputOutput.h>
#include <PowerController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ParkedState::ParkedState(M1M3SSPublisher* publisher) : EnabledState(publisher, "ParkedState") {}

States::Type ParkedState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("ParkedState: update()");
    this->startTimer();
    EnabledState::update(command, model);
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedState::raiseM1M3(RaiseM1M3Command* command, Model* model) {
    spdlog::info("ParkedState: raiseM1M3()");
    if (command->getData()->bypassReferencePosition) {
        model->getPublisher()->logCommandRejectionWarning(
                "RaiseM1M3",
                "The BypassReferencePosition parameter of the RaiseM1M3 cannot be true in the ParkedState.");
        return model->getSafetyController()->checkSafety(States::NoStateTransition);
    }
    States::Type newState = States::RaisingState;
    model->getAutomaticOperationsController()->startRaiseOperation(false);
    return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedState::enterEngineering(EnterEngineeringCommand* command, Model* model) {
    spdlog::info("ParkedState: enterEngineering()");
    States::Type newState = States::ParkedEngineeringState;
    return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedState::disable(DisableCommand* command, Model* model) {
    spdlog::info("ParkedState: disable()");
    States::Type newState = States::DisabledState;
    model->getILC()->writeSetModeDisableBuffer();
    model->getILC()->triggerModbus();
    model->getILC()->waitForAllSubnets(5000);
    model->getILC()->readAll();
    model->getILC()->verifyResponses();
    model->getForceController()->reset();
    // TODO: Uncomment this when its not so hot outside
    // model->getDigitalInputOutput()->turnAirOff();
    model->getPowerController()->setAllAuxPowerNetworks(false);
    return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
