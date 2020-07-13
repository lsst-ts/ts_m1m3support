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
    _stateFactory = stateFactory;
    _model = model;
    _currentState = States::OfflineState;
}

void Context::boot(BootCommand* command) {
    spdlog::debug("Context: boot()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->boot(command, _model));
    _stateFactory->destroy(state);
}

void Context::start(StartCommand* command) {
    spdlog::debug("Context: start()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->start(command, _model));
    _stateFactory->destroy(state);
}

void Context::enable(EnableCommand* command) {
    spdlog::debug("Context: enable()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->enable(command, _model));
    _stateFactory->destroy(state);
}

void Context::disable(DisableCommand* command) {
    spdlog::debug("Context: disable()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->disable(command, _model));
    _stateFactory->destroy(state);
}

void Context::standby(StandbyCommand* command) {
    spdlog::debug("Context: standby()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->standby(command, _model));
    _stateFactory->destroy(state);
}

void Context::exitControl(ExitControlCommand* command) {
    spdlog::debug("Context: exitControl()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->exitControl(command, _model));
    _stateFactory->destroy(state);
}

void Context::update(UpdateCommand* command) {
    spdlog::trace("Context: update()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->update(command, _model));
    _stateFactory->destroy(state);
}

void Context::turnAirOn(TurnAirOnCommand* command) {
    spdlog::debug("Context: turnAirOn()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->turnAirOn(command, _model));
    _stateFactory->destroy(state);
}

void Context::turnAirOff(TurnAirOffCommand* command) {
    spdlog::debug("Context: turnAirOff()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->turnAirOff(command, _model));
    _stateFactory->destroy(state);
}

void Context::applyOffsetForces(ApplyOffsetForcesCommand* command) {
    spdlog::debug("Context: applyOffsetForces()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->applyOffsetForces(command, _model));
    _stateFactory->destroy(state);
}

void Context::clearOffsetForces(ClearOffsetForcesCommand* command) {
    spdlog::debug("Context: clearOffsetForces()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->clearOffsetForces(command, _model));
    _stateFactory->destroy(state);
}

void Context::raiseM1M3(RaiseM1M3Command* command) {
    spdlog::debug("Context: raiseM1M3()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->raiseM1M3(command, _model));
    _stateFactory->destroy(state);
}

void Context::lowerM1M3(LowerM1M3Command* command) {
    spdlog::debug("Context: lowerM1M3()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->lowerM1M3(command, _model));
    _stateFactory->destroy(state);
}

void Context::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command) {
    spdlog::debug("Context: applyAberrationForcesByBendingModes()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->applyAberrationForcesByBendingModes(command, _model));
    _stateFactory->destroy(state);
}

void Context::applyAberrationForces(ApplyAberrationForcesCommand* command) {
    spdlog::debug("Context: applyAberrationForces()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->applyAberrationForces(command, _model));
    _stateFactory->destroy(state);
}

void Context::clearAberrationForces(ClearAberrationForcesCommand* command) {
    spdlog::debug("Context: clearAberrationForces()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->clearAberrationForces(command, _model));
    _stateFactory->destroy(state);
}

void Context::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command) {
    spdlog::debug("Context: applyActiveOpticForcesByBendingModes()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->applyActiveOpticForcesByBendingModes(command, _model));
    _stateFactory->destroy(state);
}

void Context::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    spdlog::debug("Context: applyActiveOpticForces()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->applyActiveOpticForces(command, _model));
    _stateFactory->destroy(state);
}

void Context::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    spdlog::debug("Context: clearActiveOpticForces()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->clearActiveOpticForces(command, _model));
    _stateFactory->destroy(state);
}

void Context::enterEngineering(EnterEngineeringCommand* command) {
    spdlog::debug("Context: enterEngineering()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->enterEngineering(command, _model));
    _stateFactory->destroy(state);
}

void Context::exitEngineering(ExitEngineeringCommand* command) {
    spdlog::debug("Context: exitEngineering()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->exitEngineering(command, _model));
    _stateFactory->destroy(state);
}

void Context::testAir(TestAirCommand* command) {
    spdlog::debug("Context: testAir()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->testAir(command, _model));
    _stateFactory->destroy(state);
}

void Context::testHardpoint(TestHardpointCommand* command) {
    spdlog::debug("Context: testHardpoint()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->testHardpoint(command, _model));
    _stateFactory->destroy(state);
}

void Context::testForceActuator(TestForceActuatorCommand* command) {
    spdlog::debug("Context: testForceActuator()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->testForceActuator(command, _model));
    _stateFactory->destroy(state);
}

void Context::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
    spdlog::debug("Context: moveHardpointActuators()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->moveHardpointActuators(command, _model));
    _stateFactory->destroy(state);
}

void Context::enableHardpointChase(EnableHardpointChaseCommand* command) {
    spdlog::debug("Context: enableHardpointChase()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->enableHardpointChase(command, _model));
    _stateFactory->destroy(state);
}

void Context::disableHardpointChase(DisableHardpointChaseCommand* command) {
    spdlog::debug("Context: disableHardpointChase()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->disableHardpointChase(command, _model));
    _stateFactory->destroy(state);
}

void Context::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    spdlog::debug("Context: abortRaiseM1M3()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->abortRaiseM1M3(command, _model));
    _stateFactory->destroy(state);
}

void Context::translateM1M3(TranslateM1M3Command* command) {
    spdlog::debug("Context: translateM1M3()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->translateM1M3(command, _model));
    _stateFactory->destroy(state);
}

void Context::stopHardpointMotion(StopHardpointMotionCommand* command) {
    spdlog::debug("Context: stopHardpointMotion()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->stopHardpointMotion(command, _model));
    _stateFactory->destroy(state);
}

void Context::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    spdlog::debug("Context: storeTMAAzimuthSample()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->storeTMAAzimuthSample(command, _model));
    _stateFactory->destroy(state);
}

void Context::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    spdlog::debug("Context: storeTMAElevationSample()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->storeTMAElevationSample(command, _model));
    _stateFactory->destroy(state);
}

void Context::positionM1M3(PositionM1M3Command* command) {
    spdlog::debug("Context: positionM1M3()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->positionM1M3(command, _model));
    _stateFactory->destroy(state);
}

void Context::turnLightsOn(TurnLightsOnCommand* command) {
    spdlog::debug("Context: turnLightsOn()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->turnLightsOn(command, _model));
    _stateFactory->destroy(state);
}

void Context::turnLightsOff(TurnLightsOffCommand* command) {
    spdlog::debug("Context: turnLightsOff()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->turnLightsOff(command, _model));
    _stateFactory->destroy(state);
}

void Context::turnPowerOn(TurnPowerOnCommand* command) {
    spdlog::debug("Context: turnPowerOn()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->turnPowerOn(command, _model));
    _stateFactory->destroy(state);
}

void Context::turnPowerOff(TurnPowerOffCommand* command) {
    spdlog::debug("Context: turnPowerOff()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->turnPowerOff(command, _model));
    _stateFactory->destroy(state);
}

void Context::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    spdlog::debug("Context: enableHardpointCorrections()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->enableHardpointCorrections(command, _model));
    _stateFactory->destroy(state);
}

void Context::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    spdlog::debug("Context: disableHardpointCorrections()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->disableHardpointCorrections(command, _model));
    _stateFactory->destroy(state);
}

void Context::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    spdlog::debug("Context: runMirrorForceProfile()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->runMirrorForceProfile(command, _model));
    _stateFactory->destroy(state);
}

void Context::abortProfile(AbortProfileCommand* command) {
    spdlog::debug("Context: abortProfile()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->abortProfile(command, _model));
    _stateFactory->destroy(state);
}

void Context::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) {
    spdlog::debug("Context: applyOffsetForcesByMirrorForce()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->applyOffsetForcesByMirrorForce(command, _model));
    _stateFactory->destroy(state);
}

void Context::updatePID(UpdatePIDCommand* command) {
    spdlog::debug("Context: updatePID()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->updatePID(command, _model));
    _stateFactory->destroy(state);
}

void Context::resetPID(ResetPIDCommand* command) {
    spdlog::debug("Context: resetPID()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->resetPID(command, _model));
    _stateFactory->destroy(state);
}

void Context::programILC(ProgramILCCommand* command) {
    spdlog::debug("Context: programILC()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->programILC(command, _model));
    _stateFactory->destroy(state);
}

void Context::modbusTransmit(ModbusTransmitCommand* command) {
    spdlog::debug("Context: modbusTransmit()");
    State* state = _stateFactory->create(_currentState);
    _updateCurrentStateIfRequired(state->modbusTransmit(command, _model));
    _stateFactory->destroy(state);
}

void Context::_updateCurrentStateIfRequired(States::Type potentialNewState) {
    if (potentialNewState != States::NoStateTransition) {
        _currentState = potentialNewState;
        _model->publishStateChange(potentialNewState);
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
