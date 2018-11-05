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
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Context::Context(StaticStateFactory* stateFactory, Model* model) {
	Log.Debug("Context: Context()");
	this->stateFactory = stateFactory;
	this->model = model;
	this->currentState = States::OfflineState;
}

void Context::boot(BootCommand* command) {
	Log.Debug("Context: boot()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->boot(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::start(StartCommand* command) {
	Log.Debug("Context: start()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->start(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enable(EnableCommand* command) {
	Log.Debug("Context: enable()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enable(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disable(DisableCommand* command) {
	Log.Debug("Context: disable()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disable(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::standby(StandbyCommand* command) {
	Log.Debug("Context: standby()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->standby(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::shutdown(ShutdownCommand* command) {
	Log.Debug("Context: shutdown()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->shutdown(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::update(UpdateCommand* command) {
	Log.Trace("Context: update()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->update(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnAirOn(TurnAirOnCommand* command) {
	Log.Debug("Context: turnAirOn()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnAirOn(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnAirOff(TurnAirOffCommand* command) {
	Log.Debug("Context: turnAirOff()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnAirOff(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyOffsetForces(ApplyOffsetForcesCommand* command) {
	Log.Debug("Context: applyOffsetForces()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyOffsetForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::clearOffsetForces(ClearOffsetForcesCommand* command) {
	Log.Debug("Context: clearOffsetForces()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->clearOffsetForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::raiseM1M3(RaiseM1M3Command* command) {
	Log.Debug("Context: raiseM1M3()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->raiseM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::lowerM1M3(LowerM1M3Command* command) {
	Log.Debug("Context: lowerM1M3()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->lowerM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command) {
	Log.Debug("Context: applyAberrationForcesByBendingModes()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyAberrationForcesByBendingModes(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyAberrationForces(ApplyAberrationForcesCommand* command) {
	Log.Debug("Context: applyAberrationForces()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyAberrationForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::clearAberrationForces(ClearAberrationForcesCommand* command) {
	Log.Debug("Context: clearAberrationForces()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->clearAberrationForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command) {
	Log.Debug("Context: applyActiveOpticForcesByBendingModes()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyActiveOpticForcesByBendingModes(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
	Log.Debug("Context: applyActiveOpticForces()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyActiveOpticForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
	Log.Debug("Context: clearActiveOpticForces()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->clearActiveOpticForces(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enterEngineering(EnterEngineeringCommand* command) {
	Log.Debug("Context: enterEngineering()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enterEngineering(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::exitEngineering(ExitEngineeringCommand* command) {
	Log.Debug("Context: exitEngineering()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->exitEngineering(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::testAir(TestAirCommand* command) {
	Log.Debug("Context: testAir()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->testAir(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::testHardpoint(TestHardpointCommand* command) {
	Log.Debug("Context: testHardpoint()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->testHardpoint(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::testForceActuator(TestForceActuatorCommand* command) {
	Log.Debug("Context: testForceActuator()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->testForceActuator(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
	Log.Debug("Context: moveHardpointActuators()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->moveHardpointActuators(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enableHardpointChase(EnableHardpointChaseCommand* command) {
	Log.Debug("Context: enableHardpointChase()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enableHardpointChase(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disableHardpointChase(DisableHardpointChaseCommand* command) {
	Log.Debug("Context: disableHardpointChase()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disableHardpointChase(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
	Log.Debug("Context: abortRaiseM1M3()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->abortRaiseM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::translateM1M3(TranslateM1M3Command* command) {
	Log.Debug("Context: translateM1M3()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->translateM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::stopHardpointMotion(StopHardpointMotionCommand* command) {
	Log.Debug("Context: stopHardpointMotion()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->stopHardpointMotion(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
	Log.Debug("Context: storeTMAAzimuthSample()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->storeTMAAzimuthSample(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::storeTMAElevationSample(TMAElevationSampleCommand* command) {
	Log.Debug("Context: storeTMAElevationSample()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->storeTMAElevationSample(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::positionM1M3(PositionM1M3Command* command) {
	Log.Debug("Context: positionM1M3()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->positionM1M3(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnLightsOn(TurnLightsOnCommand* command) {
	Log.Debug("Context: turnLightsOn()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnLightsOn(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnLightsOff(TurnLightsOffCommand* command) {
	Log.Debug("Context: turnLightsOff()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnLightsOff(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnPowerOn(TurnPowerOnCommand* command) {
	Log.Debug("Context: turnPowerOn()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnPowerOn(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::turnPowerOff(TurnPowerOffCommand* command) {
	Log.Debug("Context: turnPowerOff()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->turnPowerOff(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
	Log.Debug("Context: enableHardpointCorrections()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enableHardpointCorrections(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
	Log.Debug("Context: disableHardpointCorrections()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disableHardpointCorrections(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) {
	Log.Debug("Context: applyOffsetForcesByMirrorForce()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->applyOffsetForcesByMirrorForce(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::updatePID(UpdatePIDCommand* command) {
	Log.Debug("Context: updatePID()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->updatePID(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::resetPID(ResetPIDCommand* command) {
	Log.Debug("Context: resetPID()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->resetPID(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::programILC(ProgramILCCommand* command) {
	Log.Debug("Context: programILC()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->programILC(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::modbusTransmit(ModbusTransmitCommand* command) {
	Log.Debug("Context: modbusTransmit()");
	State* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->modbusTransmit(command, this->model));
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
