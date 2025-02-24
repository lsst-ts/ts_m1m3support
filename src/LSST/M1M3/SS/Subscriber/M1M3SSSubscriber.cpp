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

#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>

#include <cRIO/SAL/Command.h>

#include <AbortProfileCommand.h>
#include <AbortRaiseM1M3Command.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <BoosterValveCloseCommand.h>
#include <BoosterValveOpenCommand.h>
#include <ClearActiveOpticForcesCommand.h>
#include <ClearOffsetForcesCommand.h>
#include <ClearSlewFlagCommand.h>
#include <DisableCommand.h>
#include <DisableForceActuatorCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <DisableHardpointCorrectionsCommand.h>
#include <EnableAllForceActuatorsCommand.h>
#include <EnableCommand.h>
#include <EnableDisableForceComponentCommand.h>
#include <EnableForceActuatorCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <EnableHardpointCorrectionsCommand.h>
#include <EnterEngineeringCommand.h>
#include <ExitControlCommand.h>
#include <ExitEngineeringCommand.h>
#include <ForceActuatorBumpTestCommand.h>
#include <KillForceActuatorBumpTestCommand.h>
#include <KillHardpointTestCommand.h>
#include <LowerM1M3Command.h>
#include <MoveHardpointActuatorsCommand.h>
#include <PanicCommand.h>
#include <PauseM1M3RaisingLoweringCommand.h>
#include <PositionM1M3Command.h>
#include <RaiseM1M3Command.h>
#include <ResetPIDCommand.h>
#include <ResumeM1M3RaisingLoweringCommand.h>
#include <RunMirrorForceProfileCommand.h>
#include <SetSlewControllerSettingsCommand.h>
#include <SetSlewFlagCommand.h>
#include <StandbyCommand.h>
#include <StartCommand.h>
#include <StopHardpointMotionCommand.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <TestHardpointCommand.h>
#include <TranslateM1M3Command.h>
#include <TurnAirOffCommand.h>
#include <TurnAirOnCommand.h>
#include <TurnLightsOffCommand.h>
#include <TurnLightsOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <TurnPowerOnCommand.h>
#include <UpdatePIDCommand.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

M1M3SSSubscriber::M1M3SSSubscriber() { SPDLOG_DEBUG("M1M3SSSubscriber: M1M3SSSubscriber()"); }

M1M3SSSubscriber &M1M3SSSubscriber::get() {
    static M1M3SSSubscriber subscriber;
    return subscriber;
}

void M1M3SSSubscriber::setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL, std::shared_ptr<SAL_MTMount> mtMountSAL) {
    _m1m3SAL = m1m3SAL;
    _mtMountSAL = mtMountSAL;
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_setLogLevel");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_start");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enable");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_disable");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_standby");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_exitControl");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_panic");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_setSlewFlag");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_clearSlewFlag");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_turnAirOn");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_turnAirOff");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_boosterValveOpen");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_boosterValveClose");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_applyOffsetForces");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_clearOffsetForces");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_raiseM1M3");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_lowerM1M3");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_pauseM1M3RaisingLowering");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_resumeM1M3RaisingLowering");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_applyActiveOpticForces");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_clearActiveOpticForces");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enterEngineering");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_exitEngineering");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_testHardpoint");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_killHardpointTest");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_moveHardpointActuators");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enableHardpointChase");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_disableHardpointChase");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_abortRaiseM1M3");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_translateM1M3");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_stopHardpointMotion");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_positionM1M3");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_turnLightsOn");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_turnLightsOff");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_turnPowerOn");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_turnPowerOff");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enableHardpointCorrections");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_disableHardpointCorrections");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_runMirrorForceProfile");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_abortProfile");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_applyOffsetForcesByMirrorForce");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_updatePID");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_resetPID");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_forceActuatorBumpTest");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_killForceActuatorBumpTest");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enableForceActuator");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enableAllForceActuators");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_disableForceActuator");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_enableDisableForceComponent");
    _m1m3SAL->salProcessor((char *)"MTM1M3_command_setSlewControllerSettings");

    _mtMountSAL->salTelemetrySub((char *)"MTMount_azimuth");
    _mtMountSAL->salTelemetrySub((char *)"MTMount_elevation");

    _mtMountSAL->flushSamples_azimuth(&_tmaAzimuth);
    _mtMountSAL->flushSamples_elevation(&_tmaElevation);
}

Command *M1M3SSSubscriber::tryAcceptCommandSetLogLevel() {
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
        M1M3SSPublisher::instance().ackCommandsetLogLevel(commandID, ACK_COMPLETE, "Complete");
        M1M3SSPublisher::instance().newLogLevel(logLevel);
    }
    return 0;
}

#define COMMAND(name, cmd)                                                \
    Command *M1M3SSSubscriber::tryAcceptCommand##name() {                 \
        int32_t commandID = _m1m3SAL->acceptCommand_##cmd(&_##cmd##Data); \
        if (commandID > 0) {                                              \
            return new name##Command(commandID, &_##cmd##Data);           \
        }                                                                 \
        return 0;                                                         \
    }

#define COMMAND_NOPARS(name, cmd)                                         \
    Command *M1M3SSSubscriber::tryAcceptCommand##name() {                 \
        MTM1M3_command_##cmd##C _##cmd##Data;                             \
        int32_t commandID = _m1m3SAL->acceptCommand_##cmd(&_##cmd##Data); \
        if (commandID > 0) {                                              \
            return new name##Command(commandID);                          \
        }                                                                 \
        return 0;                                                         \
    }

COMMAND(Start, start)
COMMAND_NOPARS(Enable, enable)
COMMAND_NOPARS(Disable, disable)
COMMAND_NOPARS(Standby, standby)
COMMAND_NOPARS(ExitControl, exitControl)

COMMAND_NOPARS(Panic, panic)

COMMAND_NOPARS(SetSlewFlag, setSlewFlag)
COMMAND_NOPARS(ClearSlewFlag, clearSlewFlag)

COMMAND_NOPARS(TurnAirOn, turnAirOn)
COMMAND_NOPARS(TurnAirOff, turnAirOff)

COMMAND(ApplyOffsetForces, applyOffsetForces)
COMMAND_NOPARS(ClearOffsetForces, clearOffsetForces)

COMMAND(RaiseM1M3, raiseM1M3)
COMMAND_NOPARS(LowerM1M3, lowerM1M3)

COMMAND_NOPARS(PauseM1M3RaisingLowering, pauseM1M3RaisingLowering)
COMMAND_NOPARS(ResumeM1M3RaisingLowering, resumeM1M3RaisingLowering)

COMMAND(ApplyActiveOpticForces, applyActiveOpticForces)
COMMAND_NOPARS(ClearActiveOpticForces, clearActiveOpticForces)

COMMAND_NOPARS(EnterEngineering, enterEngineering)
COMMAND_NOPARS(ExitEngineering, exitEngineering)

COMMAND_NOPARS(BoosterValveOpen, boosterValveOpen)
COMMAND_NOPARS(BoosterValveClose, boosterValveClose)

COMMAND(TestHardpoint, testHardpoint)
COMMAND(KillHardpointTest, killHardpointTest)

COMMAND(MoveHardpointActuators, moveHardpointActuators)

COMMAND(EnableHardpointChase, enableHardpointChase)
COMMAND(DisableHardpointChase, disableHardpointChase)

COMMAND(AbortRaiseM1M3, abortRaiseM1M3)
COMMAND(TranslateM1M3, translateM1M3)
COMMAND(StopHardpointMotion, stopHardpointMotion)
COMMAND(PositionM1M3, positionM1M3)

COMMAND(TurnLightsOn, turnLightsOn)
COMMAND(TurnLightsOff, turnLightsOff)

COMMAND(TurnPowerOn, turnPowerOn)
COMMAND(TurnPowerOff, turnPowerOff)

COMMAND(EnableHardpointCorrections, enableHardpointCorrections)
COMMAND(DisableHardpointCorrections, disableHardpointCorrections)

COMMAND(RunMirrorForceProfile, runMirrorForceProfile)
COMMAND(AbortProfile, abortProfile)

COMMAND(ApplyOffsetForcesByMirrorForce, applyOffsetForcesByMirrorForce)

COMMAND(UpdatePID, updatePID)
COMMAND(ResetPID, resetPID)

COMMAND(ForceActuatorBumpTest, forceActuatorBumpTest)
COMMAND(KillForceActuatorBumpTest, killForceActuatorBumpTest)

COMMAND(DisableForceActuator, disableForceActuator)
COMMAND(EnableForceActuator, enableForceActuator)
COMMAND_NOPARS(EnableAllForceActuators, enableAllForceActuators)

COMMAND(EnableDisableForceComponent, enableDisableForceComponent)
COMMAND(SetSlewControllerSettings, setSlewControllerSettings)

Command *M1M3SSSubscriber::tryGetSampleTMAAzimuth() {
    int32_t result = _mtMountSAL->getSample_azimuth(&_tmaAzimuth);
    if (result == 0) {
        while (result == 0) {
            result = _mtMountSAL->getSample_azimuth(&_tmaAzimuth);
        }
        return new TMAAzimuthSampleCommand(&_tmaAzimuth);
    }
    return 0;
}

Command *M1M3SSSubscriber::tryGetSampleTMAElevation() {
    int32_t result = _mtMountSAL->getSample_elevation(&_tmaElevation);
    if (result == 0) {
        while (result == 0) {
            result = _mtMountSAL->getSample_elevation(&_tmaElevation);
        }
        return new TMAElevationSampleCommand(&_tmaElevation);
    }
    return 0;
}
