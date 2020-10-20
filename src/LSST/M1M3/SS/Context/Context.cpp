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

Context::Context() {
    spdlog::debug("Context: Context()");
    _currentState = States::OfflineState;
}

Context& Context::get() {
    static Context context;
    return context;
}

void Context::enterControl(EnterControlCommand* command) {
    spdlog::debug("Context: enterControl()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enterControl(command));
}

void Context::start(StartCommand* command) {
    spdlog::debug("Context: start()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->start(command));
}

void Context::enable(EnableCommand* command) {
    spdlog::debug("Context: enable()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enable(command));
}

void Context::disable(DisableCommand* command) {
    spdlog::debug("Context: disable()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disable(command));
}

void Context::standby(StandbyCommand* command) {
    spdlog::debug("Context: standby()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->standby(command));
}

void Context::exitControl(ExitControlCommand* command) {
    spdlog::debug("Context: exitControl()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->exitControl(command));
}

void Context::update(UpdateCommand* command) {
    spdlog::trace("Context: update()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->update(command));
}

void Context::turnAirOn(TurnAirOnCommand* command) {
    spdlog::debug("Context: turnAirOn()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnAirOn(command));
}

void Context::turnAirOff(TurnAirOffCommand* command) {
    spdlog::debug("Context: turnAirOff()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnAirOff(command));
}

void Context::applyOffsetForces(ApplyOffsetForcesCommand* command) {
    spdlog::debug("Context: applyOffsetForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyOffsetForces(command));
}

void Context::clearOffsetForces(ClearOffsetForcesCommand* command) {
    spdlog::debug("Context: clearOffsetForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->clearOffsetForces(command));
}

void Context::raiseM1M3(RaiseM1M3Command* command) {
    spdlog::debug("Context: raiseM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->raiseM1M3(command));
}

void Context::lowerM1M3(LowerM1M3Command* command) {
    spdlog::debug("Context: lowerM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->lowerM1M3(command));
}

void Context::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command) {
    spdlog::debug("Context: applyAberrationForcesByBendingModes()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyAberrationForcesByBendingModes(command));
}

void Context::applyAberrationForces(ApplyAberrationForcesCommand* command) {
    spdlog::debug("Context: applyAberrationForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyAberrationForces(command));
}

void Context::clearAberrationForces(ClearAberrationForcesCommand* command) {
    spdlog::debug("Context: clearAberrationForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->clearAberrationForces(command));
}

void Context::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command) {
    spdlog::debug("Context: applyActiveOpticForcesByBendingModes()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyActiveOpticForcesByBendingModes(command));
}

void Context::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    spdlog::debug("Context: applyActiveOpticForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyActiveOpticForces(command));
}

void Context::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    spdlog::debug("Context: clearActiveOpticForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->clearActiveOpticForces(command));
}

void Context::enterEngineering(EnterEngineeringCommand* command) {
    spdlog::debug("Context: enterEngineering()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enterEngineering(command));
}

void Context::exitEngineering(ExitEngineeringCommand* command) {
    spdlog::debug("Context: exitEngineering()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->exitEngineering(command));
}

void Context::testAir(TestAirCommand* command) {
    spdlog::debug("Context: testAir()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->testAir(command));
}

void Context::testHardpoint(TestHardpointCommand* command) {
    spdlog::debug("Context: testHardpoint()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->testHardpoint(command));
}

void Context::testForceActuator(TestForceActuatorCommand* command) {
    spdlog::debug("Context: testForceActuator()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->testForceActuator(command));
}

void Context::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
    spdlog::debug("Context: moveHardpointActuators()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->moveHardpointActuators(command));
}

void Context::enableHardpointChase(EnableHardpointChaseCommand* command) {
    spdlog::debug("Context: enableHardpointChase()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enableHardpointChase(command));
}

void Context::disableHardpointChase(DisableHardpointChaseCommand* command) {
    spdlog::debug("Context: disableHardpointChase()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disableHardpointChase(command));
}

void Context::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    spdlog::debug("Context: abortRaiseM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->abortRaiseM1M3(command));
}

void Context::translateM1M3(TranslateM1M3Command* command) {
    spdlog::debug("Context: translateM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->translateM1M3(command));
}

void Context::stopHardpointMotion(StopHardpointMotionCommand* command) {
    spdlog::debug("Context: stopHardpointMotion()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->stopHardpointMotion(command));
}

void Context::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    spdlog::debug("Context: storeTMAAzimuthSample()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->storeTMAAzimuthSample(command));
}

void Context::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    spdlog::debug("Context: storeTMAElevationSample()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->storeTMAElevationSample(command));
}

void Context::positionM1M3(PositionM1M3Command* command) {
    spdlog::debug("Context: positionM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->positionM1M3(command));
}

void Context::turnLightsOn(TurnLightsOnCommand* command) {
    spdlog::debug("Context: turnLightsOn()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnLightsOn(command));
}

void Context::turnLightsOff(TurnLightsOffCommand* command) {
    spdlog::debug("Context: turnLightsOff()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnLightsOff(command));
}

void Context::turnPowerOn(TurnPowerOnCommand* command) {
    spdlog::debug("Context: turnPowerOn()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnPowerOn(command));
}

void Context::turnPowerOff(TurnPowerOffCommand* command) {
    spdlog::debug("Context: turnPowerOff()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnPowerOff(command));
}

void Context::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    spdlog::debug("Context: enableHardpointCorrections()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enableHardpointCorrections(command));
}

void Context::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    spdlog::debug("Context: disableHardpointCorrections()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disableHardpointCorrections(command));
}

void Context::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    spdlog::debug("Context: runMirrorForceProfile()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->runMirrorForceProfile(command));
}

void Context::abortProfile(AbortProfileCommand* command) {
    spdlog::debug("Context: abortProfile()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->abortProfile(command));
}

void Context::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) {
    spdlog::debug("Context: applyOffsetForcesByMirrorForce()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyOffsetForcesByMirrorForce(command));
}

void Context::updatePID(UpdatePIDCommand* command) {
    spdlog::debug("Context: updatePID()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->updatePID(command));
}

void Context::resetPID(ResetPIDCommand* command) {
    spdlog::debug("Context: resetPID()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->resetPID(command));
}

void Context::programILC(ProgramILCCommand* command) {
    spdlog::debug("Context: programILC()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->programILC(command));
}

void Context::modbusTransmit(ModbusTransmitCommand* command) {
    spdlog::debug("Context: modbusTransmit()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->modbusTransmit(command));
}

void Context::forceActuatorBumpTest(ForceActuatorBumpTestCommand* command) {
    spdlog::debug("Context: forceActuatorBumpTest()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->forceActuatorBumpTest(command));
}

void Context::_updateCurrentStateIfRequired(States::Type potentialNewState) {
    if (potentialNewState != States::NoStateTransition) {
        _currentState = potentialNewState;
        Model::get().publishStateChange(potentialNewState);
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
