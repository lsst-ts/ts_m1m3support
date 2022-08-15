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

#ifndef STATE_H_
#define STATE_H_

#include <StateTypes.h>
#include <time.h>
#include <string>

#include <Command.h>
#include <EnterControlCommand.h>
#include <StartCommand.h>
#include <EnableCommand.h>
#include <DisableCommand.h>
#include <StandbyCommand.h>
#include <ExitControlCommand.h>
#include <UpdateCommand.h>
#include <TurnAirOnCommand.h>
#include <TurnAirOffCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <ClearOffsetForcesCommand.h>
#include <RaiseM1M3Command.h>
#include <LowerM1M3Command.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ClearActiveOpticForcesCommand.h>
#include <EnterEngineeringCommand.h>
#include <ExitEngineeringCommand.h>
#include <SetAirSlewFlagCommand.h>
#include <TestHardpointCommand.h>
#include <KillHardpointTestCommand.h>
#include <MoveHardpointActuatorsCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <AbortRaiseM1M3Command.h>
#include <TranslateM1M3Command.h>
#include <StopHardpointMotionCommand.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <PositionM1M3Command.h>
#include <TurnLightsOnCommand.h>
#include <TurnLightsOffCommand.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <EnableHardpointCorrectionsCommand.h>
#include <DisableHardpointCorrectionsCommand.h>
#include <RunMirrorForceProfileCommand.h>
#include <AbortProfileCommand.h>
#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <UpdatePIDCommand.h>
#include <ResetPIDCommand.h>
#include <ForceActuatorBumpTestCommand.h>
#include <KillForceActuatorBumpTestCommand.h>
#include <DisableForceActuatorCommand.h>
#include <EnableForceActuatorCommand.h>
#include <EnableAllForceActuatorsCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abstract class for M1M3 state. Any M1M3 state should inherit this class and
 * implements command functions it executed. Calls to functions not implemented
 * for the state shall be rejected.
 */
class State {
public:
    State(std::string name);
    virtual ~State();

    /**
     * @brief Executes EnterControlCommand.
     *
     * @param command EnterControlCommand to execute
     *
     * @return next state if M1M3's state has changed or States::NoStateTransition for no M1M3's state change
     */
    virtual States::Type enterControl(EnterControlCommand* command);
    virtual States::Type start(StartCommand* command);
    virtual States::Type enable(EnableCommand* command);
    virtual States::Type disable(DisableCommand* command);
    virtual States::Type standby(StandbyCommand* command);
    virtual States::Type exitControl(ExitControlCommand* command);

    /**
     * @brief Called to retrieve telemetry data.
     *
     * This method is called from OuterLoopClockThread. As every concrete child
     * class must provide update routine, it is pure virtual.
     *
     * @param command UpdateCommand, ignored.
     *
     * @return new state, or States::NoStateTransition if state doesn't change
     */
    virtual States::Type update(UpdateCommand* command) = 0;
    virtual States::Type turnAirOn(TurnAirOnCommand* command);
    virtual States::Type turnAirOff(TurnAirOffCommand* command);
    virtual States::Type applyOffsetForces(ApplyOffsetForcesCommand* command);
    virtual States::Type clearOffsetForces(ClearOffsetForcesCommand* command);
    virtual States::Type raiseM1M3(RaiseM1M3Command* command);
    virtual States::Type lowerM1M3(LowerM1M3Command* command);
    virtual States::Type applyActiveOpticForces(ApplyActiveOpticForcesCommand* command);
    virtual States::Type clearActiveOpticForces(ClearActiveOpticForcesCommand* command);
    virtual States::Type enterEngineering(EnterEngineeringCommand* command);
    virtual States::Type exitEngineering(ExitEngineeringCommand* command);
    virtual States::Type setAirSlewFlag(SetAirSlewFlagCommand* command);
    virtual States::Type testHardpoint(TestHardpointCommand* command);
    virtual States::Type killHardpointTest(KillHardpointTestCommand* command);
    virtual States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command);
    virtual States::Type enableHardpointChase(EnableHardpointChaseCommand* command);
    virtual States::Type disableHardpointChase(DisableHardpointChaseCommand* command);
    virtual States::Type abortRaiseM1M3(AbortRaiseM1M3Command* command);
    virtual States::Type translateM1M3(TranslateM1M3Command* command);
    virtual States::Type stopHardpointMotion(StopHardpointMotionCommand* command);
    virtual States::Type storeTMAAzimuthSample(TMAAzimuthSampleCommand* command);
    virtual States::Type storeTMAElevationSample(TMAElevationSampleCommand* command);
    virtual States::Type positionM1M3(PositionM1M3Command* command);
    virtual States::Type turnLightsOn(TurnLightsOnCommand* command);
    virtual States::Type turnLightsOff(TurnLightsOffCommand* command);
    virtual States::Type turnPowerOn(TurnPowerOnCommand* command);
    virtual States::Type turnPowerOff(TurnPowerOffCommand* command);
    virtual States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand* command);
    virtual States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command);
    virtual States::Type runMirrorForceProfile(RunMirrorForceProfileCommand* command);
    virtual States::Type abortProfile(AbortProfileCommand* command);
    virtual States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command);
    virtual States::Type updatePID(UpdatePIDCommand* command);
    virtual States::Type resetPID(ResetPIDCommand* command);
    virtual States::Type forceActuatorBumpTest(ForceActuatorBumpTestCommand* command);
    virtual States::Type killForceActuatorBumpTest(KillForceActuatorBumpTestCommand* command);
    virtual States::Type disableForceActuator(DisableForceActuatorCommand* command);
    virtual States::Type enableForceActuator(EnableForceActuatorCommand* command);
    virtual States::Type enableAllForceActuators(EnableAllForceActuatorsCommand* command);

protected:
    std::string name;

    States::Type rejectCommandInvalidState(Command* command, std::string cmd_name);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATE_H_ */
