/*
 * Context.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <Context.h>
#include <StaticStateFactory.h>
#include <Model.h>
#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Context::Context(StaticStateFactory* stateFactory, Model* model) {
	this->stateFactory = stateFactory;
	this->model = model;
	this->currentState = States::OfflineState;
}

void Context::boot(BootCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->boot(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::start(StartCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->start(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enable(EnableCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enable(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disable(DisableCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disable(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::standby(StandbyCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->standby(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::shutdown(ShutdownCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->shutdown(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::update(UpdateCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->update(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnAirOn(TurnAirOnCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnAirOn(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnAirOff(TurnAirOffCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnAirOff(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyOffsetForces(ApplyOffsetForcesCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyOffsetForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::clearOffsetForces(ClearOffsetForcesCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->clearOffsetForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::raiseM1M3(RaiseM1M3Command* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->raiseM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::lowerM1M3(LowerM1M3Command* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->lowerM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyAberrationByBendingModes(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyAberrationByForces(ApplyAberrationByForcesCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyAberrationByForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::clearAberration(ClearAberrationCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->clearAberration(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyAOSCorrectionByBendingModes(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyAOSCorrectionByForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::clearAOSCorrection(ClearAOSCorrectionCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->clearAOSCorrection(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enterEngineering(EnterEngineeringCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enterEngineering(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::exitEngineering(ExitEngineeringCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->exitEngineering(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::testAir(TestAirCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->testAir(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::testHardpoint(TestHardpointCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->testHardpoint(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::testForceActuator(TestForceActuatorCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->testForceActuator(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->moveHardpointActuators(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enableHardpointChase(EnableHardpointChaseCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enableHardpointChase(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disableHardpointChase(DisableHardpointChaseCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disableHardpointChase(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->abortRaiseM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::translateM1M3(TranslateM1M3Command* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->translateM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::stopHardpointMotion(StopHardpointMotionCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->stopHardpointMotion(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->storeTMAAzimuthSample(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::storeTMAElevationSample(TMAElevationSampleCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->storeTMAElevationSample(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::positionM1M3(PositionM1M3Command* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->positionM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnLightsOn(TurnLightsOnCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnLightsOn(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnLightsOff(TurnLightsOffCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnLightsOff(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnPowerOn(TurnPowerOnCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnPowerOn(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnPowerOff(TurnPowerOffCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnPowerOff(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enableHardpointCorrections(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disableHardpointCorrections(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::updateCurrentStateIfRequired(States::Type potentialNewState) {
	if (potentialNewState != States::NoStateTransition) {
		this->currentState = potentialNewState;
		this->model->publishStateChange(potentialNewState);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
