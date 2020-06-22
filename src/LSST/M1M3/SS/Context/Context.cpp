/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Context.h>
#include <StaticStateFactory.h>
#include <Model.h>
#include <State.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Context::Context(StaticStateFactory* stateFactory, Model* model) {
    spdlog::debug("Context: Context()");
    this->stateFactory = stateFactory;
    this->model = model;
    this->currentState = States::OfflineState;
}

void Context::boot(BootCommand* command) {
    spdlog::debug("Context: boot()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->boot(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::start(StartCommand* command) {
    spdlog::debug("Context: start()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->start(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::enable(EnableCommand* command) {
    spdlog::debug("Context: enable()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->enable(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::disable(DisableCommand* command) {
    spdlog::debug("Context: disable()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->disable(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::standby(StandbyCommand* command) {
    spdlog::debug("Context: standby()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->standby(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::shutdown(ShutdownCommand* command) {
    spdlog::debug("Context: shutdown()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->shutdown(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::update(UpdateCommand* command) {
    spdlog::trace("Context: update()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->update(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::turnAirOn(TurnAirOnCommand* command) {
    spdlog::debug("Context: turnAirOn()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->turnAirOn(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::turnAirOff(TurnAirOffCommand* command) {
    spdlog::debug("Context: turnAirOff()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->turnAirOff(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::applyOffsetForces(ApplyOffsetForcesCommand* command) {
    spdlog::debug("Context: applyOffsetForces()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->applyOffsetForces(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::clearOffsetForces(ClearOffsetForcesCommand* command) {
    spdlog::debug("Context: clearOffsetForces()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->clearOffsetForces(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::raiseM1M3(RaiseM1M3Command* command) {
    spdlog::debug("Context: raiseM1M3()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->raiseM1M3(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::lowerM1M3(LowerM1M3Command* command) {
    spdlog::debug("Context: lowerM1M3()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->lowerM1M3(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command) {
    spdlog::debug("Context: applyAberrationForcesByBendingModes()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->applyAberrationForcesByBendingModes(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::applyAberrationForces(ApplyAberrationForcesCommand* command) {
    spdlog::debug("Context: applyAberrationForces()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->applyAberrationForces(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::clearAberrationForces(ClearAberrationForcesCommand* command) {
    spdlog::debug("Context: clearAberrationForces()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->clearAberrationForces(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command) {
    spdlog::debug("Context: applyActiveOpticForcesByBendingModes()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->applyActiveOpticForcesByBendingModes(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    spdlog::debug("Context: applyActiveOpticForces()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->applyActiveOpticForces(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    spdlog::debug("Context: clearActiveOpticForces()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->clearActiveOpticForces(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::enterEngineering(EnterEngineeringCommand* command) {
    spdlog::debug("Context: enterEngineering()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->enterEngineering(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::exitEngineering(ExitEngineeringCommand* command) {
    spdlog::debug("Context: exitEngineering()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->exitEngineering(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::testAir(TestAirCommand* command) {
    spdlog::debug("Context: testAir()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->testAir(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::testHardpoint(TestHardpointCommand* command) {
    spdlog::debug("Context: testHardpoint()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->testHardpoint(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::testForceActuator(TestForceActuatorCommand* command) {
    spdlog::debug("Context: testForceActuator()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->testForceActuator(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
    spdlog::debug("Context: moveHardpointActuators()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->moveHardpointActuators(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::enableHardpointChase(EnableHardpointChaseCommand* command) {
    spdlog::debug("Context: enableHardpointChase()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->enableHardpointChase(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::disableHardpointChase(DisableHardpointChaseCommand* command) {
    spdlog::debug("Context: disableHardpointChase()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->disableHardpointChase(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    spdlog::debug("Context: abortRaiseM1M3()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->abortRaiseM1M3(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::translateM1M3(TranslateM1M3Command* command) {
    spdlog::debug("Context: translateM1M3()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->translateM1M3(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::stopHardpointMotion(StopHardpointMotionCommand* command) {
    spdlog::debug("Context: stopHardpointMotion()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->stopHardpointMotion(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    spdlog::debug("Context: storeTMAAzimuthSample()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->storeTMAAzimuthSample(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    spdlog::debug("Context: storeTMAElevationSample()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->storeTMAElevationSample(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::positionM1M3(PositionM1M3Command* command) {
    spdlog::debug("Context: positionM1M3()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->positionM1M3(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::turnLightsOn(TurnLightsOnCommand* command) {
    spdlog::debug("Context: turnLightsOn()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->turnLightsOn(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::turnLightsOff(TurnLightsOffCommand* command) {
    spdlog::debug("Context: turnLightsOff()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->turnLightsOff(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::turnPowerOn(TurnPowerOnCommand* command) {
    spdlog::debug("Context: turnPowerOn()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->turnPowerOn(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::turnPowerOff(TurnPowerOffCommand* command) {
    spdlog::debug("Context: turnPowerOff()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->turnPowerOff(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    spdlog::debug("Context: enableHardpointCorrections()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->enableHardpointCorrections(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    spdlog::debug("Context: disableHardpointCorrections()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->disableHardpointCorrections(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    spdlog::debug("Context: runMirrorForceProfile()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->runMirrorForceProfile(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::abortProfile(AbortProfileCommand* command) {
    spdlog::debug("Context: abortProfile()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->abortProfile(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) {
    spdlog::debug("Context: applyOffsetForcesByMirrorForce()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->applyOffsetForcesByMirrorForce(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::updatePID(UpdatePIDCommand* command) {
    spdlog::debug("Context: updatePID()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->updatePID(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::resetPID(ResetPIDCommand* command) {
    spdlog::debug("Context: resetPID()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->resetPID(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::programILC(ProgramILCCommand* command) {
    spdlog::debug("Context: programILC()");
    State* state = this->stateFactory->create(this->currentState);
    this->updateCurrentStateIfRequired(state->programILC(command, this->model));
    this->stateFactory->destroy(state);
}

void Context::modbusTransmit(ModbusTransmitCommand* command) {
    spdlog::debug("Context: modbusTransmit()");
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
