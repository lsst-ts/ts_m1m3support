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

#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>

#include <AbortRaiseM1M3Command.h>
#include <ApplyAberrationForcesCommand.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <ClearAberrationForcesCommand.h>
#include <ClearActiveOpticForcesCommand.h>
#include <ClearOffsetForcesCommand.h>
#include <DisableCommand.h>
#include <DisableForceActuatorCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <EnableAllForceActuatorsCommand.h>
#include <EnableCommand.h>
#include <EnableForceActuatorCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <EnterEngineeringCommand.h>
#include <ExitEngineeringCommand.h>
#include <ExitControlCommand.h>
#include <MoveHardpointActuatorsCommand.h>
#include <KillForceActuatorBumpTestCommand.h>
#include <LowerM1M3Command.h>
#include <RaiseM1M3Command.h>
#include <StandbyCommand.h>
#include <StartCommand.h>
#include <SetAirSlewFlagCommand.h>
#include <TestHardpointCommand.h>
#include <TranslateM1M3Command.h>
#include <TurnAirOnCommand.h>
#include <TurnAirOffCommand.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSSubscriber::M1M3SSSubscriber() { SPDLOG_DEBUG("M1M3SSSubscriber: M1M3SSSubscriber()"); }

M1M3SSSubscriber& M1M3SSSubscriber::get() {
    static M1M3SSSubscriber subscriber;
    return subscriber;
}

void M1M3SSSubscriber::setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL, std::shared_ptr<SAL_MTMount> mtMountSAL) {
    _m1m3SAL = m1m3SAL;
    _mtMountSAL = mtMountSAL;
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_setLogLevel");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_start");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_enable");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_disable");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_standby");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_exitControl");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_turnAirOn");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_turnAirOff");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_applyOffsetForces");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_clearOffsetForces");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_raiseM1M3");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_lowerM1M3");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_applyAberrationForces");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_clearAberrationForces");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_applyActiveOpticForces");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_clearActiveOpticForces");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_enterEngineering");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_exitEngineering");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_setAirSlewFlag");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_testHardpoint");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_moveHardpointActuators");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_enableHardpointChase");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_disableHardpointChase");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_abortRaiseM1M3");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_translateM1M3");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_stopHardpointMotion");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_positionM1M3");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_turnLightsOn");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_turnLightsOff");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_turnPowerOn");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_turnPowerOff");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_enableHardpointCorrections");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_disableHardpointCorrections");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_runMirrorForceProfile");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_abortProfile");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_applyOffsetForcesByMirrorForce");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_updatePID");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_resetPID");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_programILC");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_modbusTransmit");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_forceActuatorBumpTest");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_killForceActuatorBumpTest");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_enableForceActuator");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_enableAllForceActuators");
    _m1m3SAL->salProcessor((char*)"MTM1M3_command_disableForceActuator");

    _mtMountSAL->salTelemetrySub((char*)"MTMount_azimuth");
    _mtMountSAL->salTelemetrySub((char*)"MTMount_elevation");
}

Command* M1M3SSSubscriber::tryAcceptCommandSetLogLevel() {
    MTM1M3_command_setLogLevelC setLogLevelData;
    int32_t commandID = _m1m3SAL->acceptCommand_setLogLevel(&setLogLevelData);
    if (commandID > 0) {
        int logLevel = setLogLevelData.level;
        if (logLevel >= 40)
            spdlog::set_level(spdlog::level::err);
        else if (logLevel >= 30)
            spdlog::set_level(spdlog::level::warn);
        else if (logLevel >= 20)
            spdlog::set_level(spdlog::level::info);
        else if (logLevel >= 10)
            spdlog::set_level(spdlog::level::debug);
        else
            spdlog::set_level(spdlog::level::trace);
        M1M3SSPublisher::get().ackCommandsetLogLevel(commandID, ACK_COMPLETE, "Complete");
        M1M3SSPublisher::get().newLogLevel(logLevel);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStart() {
    int32_t commandID = _m1m3SAL->acceptCommand_start(&_startData);
    if (commandID > 0) {
        return new StartCommand(commandID, &_startData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnable() {
    int32_t commandID = _m1m3SAL->acceptCommand_enable(&_enableData);
    if (commandID > 0) {
        return new EnableCommand(commandID, &_enableData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisable() {
    int32_t commandID = _m1m3SAL->acceptCommand_disable(&_disableData);
    if (commandID > 0) {
        return new DisableCommand(commandID, &_disableData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStandby() {
    int32_t commandID = _m1m3SAL->acceptCommand_standby(&_standbyData);
    if (commandID > 0) {
        return new StandbyCommand(commandID, &_standbyData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandExitControl() {
    int32_t commandID = _m1m3SAL->acceptCommand_exitControl(&_exitControlData);
    if (commandID > 0) {
        return new ExitControlCommand(commandID, &_exitControlData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnAirOn() {
    int32_t commandID = _m1m3SAL->acceptCommand_turnAirOn(&_turnAirOnData);
    if (commandID > 0) {
        return new TurnAirOnCommand(commandID, &_turnAirOnData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnAirOff() {
    int32_t commandID = _m1m3SAL->acceptCommand_turnAirOff(&_turnAirOffData);
    if (commandID > 0) {
        return new TurnAirOffCommand(commandID, &_turnAirOffData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForces() {
    int32_t commandID = _m1m3SAL->acceptCommand_applyOffsetForces(&_applyOffsetForcesData);
    if (commandID > 0) {
        return new ApplyOffsetForcesCommand(commandID, &_applyOffsetForcesData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearOffsetForces() {
    int32_t commandID = _m1m3SAL->acceptCommand_clearOffsetForces(&_clearOffsetForcesData);
    if (commandID > 0) {
        return new ClearOffsetForcesCommand(commandID, &_clearOffsetForcesData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandRaiseM1M3() {
    int32_t commandID = _m1m3SAL->acceptCommand_raiseM1M3(&_raiseM1M3Data);
    if (commandID > 0) {
        return new RaiseM1M3Command(commandID, &_raiseM1M3Data);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandLowerM1M3() {
    int32_t commandID = _m1m3SAL->acceptCommand_lowerM1M3(&_lowerM1M3Data);
    if (commandID > 0) {
        return new LowerM1M3Command(commandID, &_lowerM1M3Data);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyAberrationForces() {
    int32_t commandID = _m1m3SAL->acceptCommand_applyAberrationForces(&_applyAberrationForcesData);
    if (commandID > 0) {
        return new ApplyAberrationForcesCommand(commandID, &_applyAberrationForcesData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearAberrationForces() {
    int32_t commandID = _m1m3SAL->acceptCommand_clearAberrationForces(&_clearAberrationForcesData);
    if (commandID > 0) {
        return new ClearAberrationForcesCommand(commandID, &_clearAberrationForcesData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyActiveOpticForces() {
    int32_t commandID = _m1m3SAL->acceptCommand_applyActiveOpticForces(&_applyActiveOpticForcesData);
    if (commandID > 0) {
        return new ApplyActiveOpticForcesCommand(commandID, &_applyActiveOpticForcesData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearActiveOpticForces() {
    int32_t commandID = _m1m3SAL->acceptCommand_clearActiveOpticForces(&_clearActiveOpticForcesData);
    if (commandID > 0) {
        return new ClearActiveOpticForcesCommand(commandID, &_clearActiveOpticForcesData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnterEngineering() {
    int32_t commandID = _m1m3SAL->acceptCommand_enterEngineering(&_enterEngineeringData);
    if (commandID > 0) {
        return new EnterEngineeringCommand(commandID, &_enterEngineeringData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandExitEngineering() {
    int32_t commandID = _m1m3SAL->acceptCommand_exitEngineering(&_exitEngineeringData);
    if (commandID > 0) {
        return new ExitEngineeringCommand(commandID, &_exitEngineeringData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandSetAirSlewFlag() {
    int32_t commandID = _m1m3SAL->acceptCommand_setAirSlewFlag(&_setAirSlewFlagData);
    if (commandID > 0) {
        return new SetAirSlewFlagCommand(commandID, &_setAirSlewFlagData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestHardpoint() {
    int32_t commandID = _m1m3SAL->acceptCommand_testHardpoint(&_testHardpointData);
    if (commandID > 0) {
        return new TestHardpointCommand(commandID, &_testHardpointData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandMoveHardpointActuators() {
    int32_t commandID = _m1m3SAL->acceptCommand_moveHardpointActuators(&_moveHardpointActuatorsData);
    if (commandID > 0) {
        return new MoveHardpointActuatorsCommand(commandID, &_moveHardpointActuatorsData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableHardpointChase() {
    int32_t commandID = _m1m3SAL->acceptCommand_enableHardpointChase(&_enableHardpointChaseData);
    if (commandID > 0) {
        return new EnableHardpointChaseCommand(commandID, &_enableHardpointChaseData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableHardpointChase() {
    int32_t commandID = _m1m3SAL->acceptCommand_disableHardpointChase(&_disableHardpointChaseData);
    if (commandID > 0) {
        return new DisableHardpointChaseCommand(commandID, &_disableHardpointChaseData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandAbortRaiseM1M3() {
    int32_t commandID = _m1m3SAL->acceptCommand_abortRaiseM1M3(&_abortRaiseM1M3Data);
    if (commandID > 0) {
        return new AbortRaiseM1M3Command(commandID, &_abortRaiseM1M3Data);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTranslateM1M3() {
    int32_t commandID = _m1m3SAL->acceptCommand_translateM1M3(&_translateM1M3Data);
    if (commandID > 0) {
        return new TranslateM1M3Command(commandID, &_translateM1M3Data);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStopHardpointMotion() {
    int32_t commandID = _m1m3SAL->acceptCommand_stopHardpointMotion(&_stopHardpointMotionData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::StopHardpointMotionCommand, &_stopHardpointMotionData,
                                      commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandPositionM1M3() {
    int32_t commandID = _m1m3SAL->acceptCommand_positionM1M3(&_positionM1M3Data);
    if (commandID > 0) {
        return CommandFactory::create(Commands::PositionM1M3Command, &_positionM1M3Data, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnLightsOn() {
    int32_t commandID = _m1m3SAL->acceptCommand_turnLightsOn(&_turnLightsOnData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::TurnLightsOnCommand, &_turnLightsOnData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnLightsOff() {
    int32_t commandID = _m1m3SAL->acceptCommand_turnLightsOff(&_turnLightsOffData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::TurnLightsOffCommand, &_turnLightsOffData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnPowerOn() {
    int32_t commandID = _m1m3SAL->acceptCommand_turnPowerOn(&_turnPowerOnData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::TurnPowerOnCommand, &_turnPowerOnData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnPowerOff() {
    int32_t commandID = _m1m3SAL->acceptCommand_turnPowerOff(&_turnPowerOffData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::TurnPowerOffCommand, &_turnPowerOffData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableHardpointCorrections() {
    int32_t commandID = _m1m3SAL->acceptCommand_enableHardpointCorrections(&_enableHardpointCorrectionsData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::EnableHardpointCorrectionsCommand,
                                      &_enableHardpointCorrectionsData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableHardpointCorrections() {
    int32_t commandID =
            _m1m3SAL->acceptCommand_disableHardpointCorrections(&_disableHardpointCorrectionsData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::DisableHardpointCorrectionsCommand,
                                      &_disableHardpointCorrectionsData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandRunMirrorForceProfile() {
    int32_t commandID = _m1m3SAL->acceptCommand_runMirrorForceProfile(&_runMirrorForceProfileData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::RunMirrorForceProfileCommand, &_runMirrorForceProfileData,
                                      commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandAbortProfile() {
    int32_t commandID = _m1m3SAL->acceptCommand_abortProfile(&_abortProfileData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::AbortProfileCommand, &_abortProfileData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForcesByMirrorForce() {
    int32_t commandID =
            _m1m3SAL->acceptCommand_applyOffsetForcesByMirrorForce(&_applyOffsetForcesByMirrorForceData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::ApplyOffsetForcesByMirrorForceCommand,
                                      &_applyOffsetForcesByMirrorForceData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandUpdatePID() {
    int32_t commandID = _m1m3SAL->acceptCommand_updatePID(&_updatePIDData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::UpdatePIDCommand, &_updatePIDData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandResetPID() {
    int32_t commandID = _m1m3SAL->acceptCommand_resetPID(&_resetPIDData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::ResetPIDCommand, &_resetPIDData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandProgramILC() {
    int32_t commandID = _m1m3SAL->acceptCommand_programILC(&_programILCData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::ProgramILCCommand, &_programILCData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandModbusTransmit() {
    int32_t commandID = _m1m3SAL->acceptCommand_modbusTransmit(&_modbusTransmitData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::ModbusTransmitCommand, &_modbusTransmitData, commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandForceActuatorBumpTest() {
    int32_t commandID = _m1m3SAL->acceptCommand_forceActuatorBumpTest(&_forceActuatorBumpTestData);
    if (commandID > 0) {
        return CommandFactory::create(Commands::ForceActuatorBumpTestCommand, &_forceActuatorBumpTestData,
                                      commandID);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandKillForceActuatorBumpTest() {
    int32_t commandID = _m1m3SAL->acceptCommand_killForceActuatorBumpTest(&_killForceActuatorBumpTestData);
    if (commandID > 0) {
        return new KillForceActuatorBumpTestCommand(commandID, &_killForceActuatorBumpTestData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableForceActuator() {
    int32_t commandID = _m1m3SAL->acceptCommand_disableForceActuator(&_disableForceActuatorData);
    if (commandID > 0) {
        return new DisableForceActuatorCommand(commandID, &_disableForceActuatorData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableForceActuator() {
    int32_t commandID = _m1m3SAL->acceptCommand_enableForceActuator(&_enableForceActuatorData);
    if (commandID > 0) {
        return new EnableForceActuatorCommand(commandID, &_enableForceActuatorData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableAllForceActuators() {
    int32_t commandID = _m1m3SAL->acceptCommand_enableAllForceActuators(&_enableAllForceActuatorsData);
    if (commandID > 0) {
        return new EnableAllForceActuatorsCommand(commandID, &_enableAllForceActuatorsData);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryGetSampleTMAAzimuth() {
    int32_t result = _mtMountSAL->getSample_azimuth(&_tmaAzimuth);
    if (result == 0) {
        return new TMAAzimuthSampleCommand(&_tmaAzimuth);
    }
    return 0;
}

Command* M1M3SSSubscriber::tryGetSampleTMAElevation() {
    int32_t result = _mtMountSAL->getSample_elevation(&_tmaElevation);
    if (result == 0) {
        return new TMAElevationSampleCommand(&_tmaElevation);
    }
    return 0;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
