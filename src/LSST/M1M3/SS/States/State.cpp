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

#include <State.h>
#include <DataTypes.h>
#include <Timestamp.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

State::State(M1M3SSPublisher* publisher, std::string name) {
    this->publisher = publisher;
    this->name = name;
}

State::~State() {}

States::Type State::enterControl(EnterControlCommand* command, Model* model) {
    return States::NoStateTransition;
}
States::Type State::start(StartCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "Start");
}
States::Type State::enable(EnableCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "Enable");
}
States::Type State::disable(DisableCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "Disable");
}
States::Type State::standby(StandbyCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "Standby");
}
States::Type State::exitControl(ExitControlCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ExitControl");
}
States::Type State::turnAirOn(TurnAirOnCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TurnAirOn");
}
States::Type State::turnAirOff(TurnAirOffCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TurnAirOff");
}
States::Type State::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ApplyOffsetForces");
}
States::Type State::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ClearOffsetForces");
}
States::Type State::raiseM1M3(RaiseM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState(command, "RaiseM1M3");
}
States::Type State::lowerM1M3(LowerM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState(command, "LowerM1M3");
}
States::Type State::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command,
                                                        Model* model) {
    return this->rejectCommandInvalidState(command, "ApplyAberrationForcesByBendingModes");
}
States::Type State::applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ApplyAberrationForces");
}
States::Type State::clearAberrationForces(ClearAberrationForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ClearAberrationForces");
}
States::Type State::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command,
                                                         Model* model) {
    return this->rejectCommandInvalidState(command, "ApplyActiveOpticForcesByBendingModes");
}
States::Type State::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ApplyActiveOpticForces");
}
States::Type State::clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ClearActiveOpticForces");
}
States::Type State::enterEngineering(EnterEngineeringCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "EnterEngineering");
}
States::Type State::exitEngineering(ExitEngineeringCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ExitEngineering");
}
States::Type State::testAir(TestAirCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TestAir");
}
States::Type State::testHardpoint(TestHardpointCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TestHardpoint");
}
States::Type State::testForceActuator(TestForceActuatorCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TestForceActuator");
}
States::Type State::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "MoveHardpointActuators");
}
States::Type State::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "EnableHardpointChase");
}
States::Type State::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "DisableHardpointChase");
}
States::Type State::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState(command, "AbortRaiseM1M3");
}
States::Type State::translateM1M3(TranslateM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TranslateM1M3");
}
States::Type State::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "StopHardpointMotion");
}
States::Type State::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) {
    return States::NoStateTransition;
}
States::Type State::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) {
    return States::NoStateTransition;
}
States::Type State::positionM1M3(PositionM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState(command, "PositionM1M3");
}
States::Type State::turnLightsOn(TurnLightsOnCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TurnLightsOn");
}
States::Type State::turnLightsOff(TurnLightsOffCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TurnLightsOff");
}
States::Type State::turnPowerOn(TurnPowerOnCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TurnPowerOn");
}
States::Type State::turnPowerOff(TurnPowerOffCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "TurnPowerOff");
}
States::Type State::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "EnableHardpointCorrections");
}
States::Type State::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "DisableHardpointCorrections");
}
States::Type State::runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "RunMirrorForceProfile");
}
States::Type State::abortProfile(AbortProfileCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "AbortProfile");
}
States::Type State::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command,
                                                   Model* model) {
    return this->rejectCommandInvalidState(command, "ApplyOffsetForcesByMirrorForce");
}
States::Type State::updatePID(UpdatePIDCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "UpdatePID");
}
States::Type State::resetPID(ResetPIDCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ResetPID");
}
States::Type State::programILC(ProgramILCCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ProgramILC");
}
States::Type State::modbusTransmit(ModbusTransmitCommand* command, Model* model) {
    return this->rejectCommandInvalidState(command, "ModbusTransmit");
}

States::Type State::rejectCommandInvalidState(Command* command, std::string cmd_name) {
    std::string reason = "The command " + cmd_name + " is not valid in the " + this->name + ".";
    spdlog::warn(reason);
    this->publisher->logCommandRejectionWarning(cmd_name, reason);
    command->ackFailed(reason);
    return States::NoStateTransition;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
