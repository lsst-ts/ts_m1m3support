/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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
    SPDLOG_DEBUG("Context: Context()");
    _currentState = States::OfflineState;
}

Context& Context::get() {
    static Context context;
    return context;
}

void Context::enterControl(EnterControlCommand* command) {
    SPDLOG_DEBUG("Context: enterControl()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enterControl(command));
}

void Context::start(StartCommand* command) {
    SPDLOG_DEBUG("Context: start()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->start(command));
}

void Context::enable(EnableCommand* command) {
    SPDLOG_DEBUG("Context: enable()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enable(command));
}

void Context::disable(DisableCommand* command) {
    SPDLOG_DEBUG("Context: disable()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disable(command));
}

void Context::standby(StandbyCommand* command) {
    SPDLOG_DEBUG("Context: standby()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->standby(command));
}

void Context::exitControl(ExitControlCommand* command) {
    SPDLOG_DEBUG("Context: exitControl()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->exitControl(command));
}

void Context::update(UpdateCommand* command) {
    SPDLOG_TRACE("Context: update()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->update(command));
}

void Context::turnAirOn(TurnAirOnCommand* command) {
    SPDLOG_DEBUG("Context: turnAirOn()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnAirOn(command));
}

void Context::turnAirOff(TurnAirOffCommand* command) {
    SPDLOG_DEBUG("Context: turnAirOff()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnAirOff(command));
}

void Context::applyOffsetForces(ApplyOffsetForcesCommand* command) {
    SPDLOG_DEBUG("Context: applyOffsetForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyOffsetForces(command));
}

void Context::clearOffsetForces(ClearOffsetForcesCommand* command) {
    SPDLOG_DEBUG("Context: clearOffsetForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->clearOffsetForces(command));
}

void Context::raiseM1M3(RaiseM1M3Command* command) {
    SPDLOG_DEBUG("Context: raiseM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->raiseM1M3(command));
}

void Context::lowerM1M3(LowerM1M3Command* command) {
    SPDLOG_DEBUG("Context: lowerM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->lowerM1M3(command));
}

void Context::applyAberrationForces(ApplyAberrationForcesCommand* command) {
    SPDLOG_DEBUG("Context: applyAberrationForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyAberrationForces(command));
}

void Context::clearAberrationForces(ClearAberrationForcesCommand* command) {
    SPDLOG_DEBUG("Context: clearAberrationForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->clearAberrationForces(command));
}

void Context::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    SPDLOG_DEBUG("Context: applyActiveOpticForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyActiveOpticForces(command));
}

void Context::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    SPDLOG_DEBUG("Context: clearActiveOpticForces()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->clearActiveOpticForces(command));
}

void Context::enterEngineering(EnterEngineeringCommand* command) {
    SPDLOG_DEBUG("Context: enterEngineering()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enterEngineering(command));
}

void Context::exitEngineering(ExitEngineeringCommand* command) {
    SPDLOG_DEBUG("Context: exitEngineering()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->exitEngineering(command));
}

void Context::setAirSlewFlag(SetAirSlewFlagCommand* command) {
    SPDLOG_DEBUG("Context: setAirSlewFlag()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->setAirSlewFlag(command));
}

void Context::testHardpoint(TestHardpointCommand* command) {
    SPDLOG_DEBUG("Context: testHardpoint()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->testHardpoint(command));
}

void Context::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
    SPDLOG_DEBUG("Context: moveHardpointActuators()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->moveHardpointActuators(command));
}

void Context::enableHardpointChase(EnableHardpointChaseCommand* command) {
    SPDLOG_DEBUG("Context: enableHardpointChase()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enableHardpointChase(command));
}

void Context::disableHardpointChase(DisableHardpointChaseCommand* command) {
    SPDLOG_DEBUG("Context: disableHardpointChase()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disableHardpointChase(command));
}

void Context::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    SPDLOG_DEBUG("Context: abortRaiseM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->abortRaiseM1M3(command));
}

void Context::translateM1M3(TranslateM1M3Command* command) {
    SPDLOG_DEBUG("Context: translateM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->translateM1M3(command));
}

void Context::stopHardpointMotion(StopHardpointMotionCommand* command) {
    SPDLOG_DEBUG("Context: stopHardpointMotion()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->stopHardpointMotion(command));
}

void Context::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    SPDLOG_DEBUG("Context: storeTMAAzimuthSample()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->storeTMAAzimuthSample(command));
}

void Context::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    SPDLOG_DEBUG("Context: storeTMAElevationSample()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->storeTMAElevationSample(command));
}

void Context::positionM1M3(PositionM1M3Command* command) {
    SPDLOG_DEBUG("Context: positionM1M3()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->positionM1M3(command));
}

void Context::turnLightsOn(TurnLightsOnCommand* command) {
    SPDLOG_DEBUG("Context: turnLightsOn()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnLightsOn(command));
}

void Context::turnLightsOff(TurnLightsOffCommand* command) {
    SPDLOG_DEBUG("Context: turnLightsOff()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnLightsOff(command));
}

void Context::turnPowerOn(TurnPowerOnCommand* command) {
    SPDLOG_DEBUG("Context: turnPowerOn()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnPowerOn(command));
}

void Context::turnPowerOff(TurnPowerOffCommand* command) {
    SPDLOG_DEBUG("Context: turnPowerOff()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->turnPowerOff(command));
}

void Context::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    SPDLOG_DEBUG("Context: enableHardpointCorrections()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enableHardpointCorrections(command));
}

void Context::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    SPDLOG_DEBUG("Context: disableHardpointCorrections()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disableHardpointCorrections(command));
}

void Context::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    SPDLOG_DEBUG("Context: runMirrorForceProfile()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->runMirrorForceProfile(command));
}

void Context::abortProfile(AbortProfileCommand* command) {
    SPDLOG_DEBUG("Context: abortProfile()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->abortProfile(command));
}

void Context::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) {
    SPDLOG_DEBUG("Context: applyOffsetForcesByMirrorForce()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->applyOffsetForcesByMirrorForce(command));
}

void Context::updatePID(UpdatePIDCommand* command) {
    SPDLOG_DEBUG("Context: updatePID()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->updatePID(command));
}

void Context::resetPID(ResetPIDCommand* command) {
    SPDLOG_DEBUG("Context: resetPID()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->resetPID(command));
}

void Context::forceActuatorBumpTest(ForceActuatorBumpTestCommand* command) {
    SPDLOG_DEBUG("Context: forceActuatorBumpTest()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->forceActuatorBumpTest(command));
}

void Context::killForceActuatorBumpTest(KillForceActuatorBumpTestCommand* command) {
    SPDLOG_DEBUG("Context: killForceActuatorBumpTest()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->killForceActuatorBumpTest(command));
}

void Context::disableForceActuator(DisableForceActuatorCommand* command) {
    SPDLOG_DEBUG("Context: disableForceActuator({})", command->actuatorIndex);
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->disableForceActuator(command));
}

void Context::enableForceActuator(EnableForceActuatorCommand* command) {
    SPDLOG_DEBUG("Context: enableForceActuator({})", command->actuatorIndex);
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enableForceActuator(command));
}

void Context::enableAllForceActuators(EnableAllForceActuatorsCommand* command) {
    SPDLOG_DEBUG("Context: enableAllForceActuators()");
    State* state = StaticStateFactory::get().create(_currentState);
    _updateCurrentStateIfRequired(state->enableAllForceActuators(command));
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
