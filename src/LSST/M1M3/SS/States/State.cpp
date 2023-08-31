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

#include <spdlog/spdlog.h>

#include <DataTypes.h>
#include <M1M3SSPublisher.h>
#include <State.h>
#include <Timestamp.h>

namespace LSST {
namespace M1M3 {
namespace SS {

State::State(std::string name) { name = name; }

State::~State() {}

States::Type State::enterControl(EnterControlCommand* command) { return States::NoStateTransition; }
States::Type State::start(StartCommand* command) { return rejectCommandInvalidState(command, "Start"); }
States::Type State::enable(EnableCommand* command) { return rejectCommandInvalidState(command, "Enable"); }
States::Type State::disable(DisableCommand* command) { return rejectCommandInvalidState(command, "Disable"); }
States::Type State::standby(StandbyCommand* command) { return rejectCommandInvalidState(command, "Standby"); }
States::Type State::exitControl(ExitControlCommand* command) {
    return rejectCommandInvalidState(command, "ExitControl");
}
States::Type State::setSlewFlag(SetSlewFlagCommand* command) {
    return rejectCommandInvalidState(command, "SetSlewFlag");
}
States::Type State::clearSlewFlag(ClearSlewFlagCommand* command) {
    return rejectCommandInvalidState(command, "ClearSlewFlag");
}
States::Type State::turnAirOn(TurnAirOnCommand* command) {
    return rejectCommandInvalidState(command, "TurnAirOn");
}
States::Type State::turnAirOff(TurnAirOffCommand* command) {
    return rejectCommandInvalidState(command, "TurnAirOff");
}
States::Type State::boosterValveOpen(BoosterValveOpenCommand* command) {
    return rejectCommandInvalidState(command, "BoosterValveOpen");
}
States::Type State::boosterValveClose(BoosterValveCloseCommand* command) {
    return rejectCommandInvalidState(command, "BoosterValveClose");
}
States::Type State::applyOffsetForces(ApplyOffsetForcesCommand* command) {
    return rejectCommandInvalidState(command, "ApplyOffsetForces");
}
States::Type State::clearOffsetForces(ClearOffsetForcesCommand* command) {
    return rejectCommandInvalidState(command, "ClearOffsetForces");
}
States::Type State::raiseM1M3(RaiseM1M3Command* command) {
    return rejectCommandInvalidState(command, "RaiseM1M3");
}
States::Type State::lowerM1M3(LowerM1M3Command* command) {
    return rejectCommandInvalidState(command, "LowerM1M3");
}
States::Type State::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    return rejectCommandInvalidState(command, "ApplyActiveOpticForces");
}
States::Type State::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    return rejectCommandInvalidState(command, "ClearActiveOpticForces");
}
States::Type State::enterEngineering(EnterEngineeringCommand* command) {
    return rejectCommandInvalidState(command, "EnterEngineering");
}
States::Type State::exitEngineering(ExitEngineeringCommand* command) {
    return rejectCommandInvalidState(command, "ExitEngineering");
}
States::Type State::testHardpoint(TestHardpointCommand* command) {
    return rejectCommandInvalidState(command, "TestHardpoint");
}
States::Type State::killHardpointTest(KillHardpointTestCommand* command) {
    return rejectCommandInvalidState(command, "KillHardpointTest");
}
States::Type State::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
    return rejectCommandInvalidState(command, "MoveHardpointActuators");
}
States::Type State::enableHardpointChase(EnableHardpointChaseCommand* command) {
    return rejectCommandInvalidState(command, "EnableHardpointChase");
}
States::Type State::disableHardpointChase(DisableHardpointChaseCommand* command) {
    return rejectCommandInvalidState(command, "DisableHardpointChase");
}
States::Type State::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    return rejectCommandInvalidState(command, "AbortRaiseM1M3");
}
States::Type State::translateM1M3(TranslateM1M3Command* command) {
    return rejectCommandInvalidState(command, "TranslateM1M3");
}
States::Type State::stopHardpointMotion(StopHardpointMotionCommand* command) {
    return rejectCommandInvalidState(command, "StopHardpointMotion");
}
States::Type State::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    return States::NoStateTransition;
}
States::Type State::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    return States::NoStateTransition;
}
States::Type State::positionM1M3(PositionM1M3Command* command) {
    return rejectCommandInvalidState(command, "PositionM1M3");
}
States::Type State::turnLightsOn(TurnLightsOnCommand* command) {
    return rejectCommandInvalidState(command, "TurnLightsOn");
}
States::Type State::turnLightsOff(TurnLightsOffCommand* command) {
    return rejectCommandInvalidState(command, "TurnLightsOff");
}
States::Type State::turnPowerOn(TurnPowerOnCommand* command) {
    return rejectCommandInvalidState(command, "TurnPowerOn");
}
States::Type State::turnPowerOff(TurnPowerOffCommand* command) {
    return rejectCommandInvalidState(command, "TurnPowerOff");
}
States::Type State::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    return rejectCommandInvalidState(command, "EnableHardpointCorrections");
}
States::Type State::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    return rejectCommandInvalidState(command, "DisableHardpointCorrections");
}
States::Type State::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    return rejectCommandInvalidState(command, "RunMirrorForceProfile");
}
States::Type State::abortProfile(AbortProfileCommand* command) {
    return rejectCommandInvalidState(command, "AbortProfile");
}
States::Type State::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command) {
    return rejectCommandInvalidState(command, "ApplyOffsetForcesByMirrorForce");
}
States::Type State::updatePID(UpdatePIDCommand* command) {
    return rejectCommandInvalidState(command, "UpdatePID");
}
States::Type State::resetPID(ResetPIDCommand* command) {
    return rejectCommandInvalidState(command, "ResetPID");
}
States::Type State::forceActuatorBumpTest(ForceActuatorBumpTestCommand* command) {
    return rejectCommandInvalidState(command, "ForceActuatorBumpTest");
}
States::Type State::killForceActuatorBumpTest(KillForceActuatorBumpTestCommand* command) {
    return rejectCommandInvalidState(command, "KillForceActuatorBumpTest");
}
States::Type State::disableForceActuator(DisableForceActuatorCommand* command) {
    return rejectCommandInvalidState(command, "DisableForceActuator");
}
States::Type State::enableForceActuator(EnableForceActuatorCommand* command) {
    return rejectCommandInvalidState(command, "EnableForceActuator");
}
States::Type State::enableAllForceActuators(EnableAllForceActuatorsCommand* command) {
    return rejectCommandInvalidState(command, "EnableAllForceActuators");
}
States::Type State::enableDisableForceComponent(EnableDisableForceComponentCommand* command) {
    return rejectCommandInvalidState(command, "EnableDisableForceComponent");
}
States::Type State::setSlewControllerSettings(SetSlewControllerSettingsCommand* command) {
    return rejectCommandInvalidState(command, "SetSlewControllerSettings");
}

States::Type State::rejectCommandInvalidState(Command* command, std::string cmd_name) {
    std::string reason = "The command " + cmd_name + " is not valid in the " + name + ".";
    SPDLOG_WARN(reason);
    M1M3SSPublisher::instance().logCommandRejectionWarning(cmd_name, reason);
    command->ackFailed(reason);
    return States::NoStateTransition;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
