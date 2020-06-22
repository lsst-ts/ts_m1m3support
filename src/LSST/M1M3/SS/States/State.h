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

#ifndef STATE_H_
#define STATE_H_

#include <StateTypes.h>
#include <time.h>
#include <string>

#include <Model.h>
#include <M1M3SSPublisher.h>
#include <Command.h>
#include <BootCommand.h>
#include <StartCommand.h>
#include <EnableCommand.h>
#include <DisableCommand.h>
#include <StandbyCommand.h>
#include <ShutdownCommand.h>
#include <UpdateCommand.h>
#include <TurnAirOnCommand.h>
#include <TurnAirOffCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <ClearOffsetForcesCommand.h>
#include <RaiseM1M3Command.h>
#include <LowerM1M3Command.h>
#include <ApplyAberrationForcesByBendingModesCommand.h>
#include <ApplyAberrationForcesCommand.h>
#include <ClearAberrationForcesCommand.h>
#include <ApplyActiveOpticForcesByBendingModesCommand.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ClearActiveOpticForcesCommand.h>
#include <EnterEngineeringCommand.h>
#include <ExitEngineeringCommand.h>
#include <TestAirCommand.h>
#include <TestHardpointCommand.h>
#include <TestForceActuatorCommand.h>
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
#include <ProgramILCCommand.h>
#include <ModbusTransmitCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class State {
protected:
    M1M3SSPublisher* publisher;
    std::string name;
    timespec startTime;
    timespec stopTime;

public:
    State(M1M3SSPublisher* publisher, std::string name);
    virtual ~State();

    virtual States::Type boot(BootCommand* command, Model* model);
    virtual States::Type start(StartCommand* command, Model* model);
    virtual States::Type enable(EnableCommand* command, Model* model);
    virtual States::Type disable(DisableCommand* command, Model* model);
    virtual States::Type standby(StandbyCommand* command, Model* model);
    virtual States::Type shutdown(ShutdownCommand* command, Model* model);
    virtual States::Type update(UpdateCommand* command, Model* model);
    virtual States::Type turnAirOn(TurnAirOnCommand* command, Model* model);
    virtual States::Type turnAirOff(TurnAirOffCommand* command, Model* model);
    virtual States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model);
    virtual States::Type clearOffsetForces(ClearOffsetForcesCommand* command, Model* model);
    virtual States::Type raiseM1M3(RaiseM1M3Command* command, Model* model);
    virtual States::Type lowerM1M3(LowerM1M3Command* command, Model* model);
    virtual States::Type applyAberrationForcesByBendingModes(
            ApplyAberrationForcesByBendingModesCommand* command, Model* model);
    virtual States::Type applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model);
    virtual States::Type clearAberrationForces(ClearAberrationForcesCommand* command, Model* model);
    virtual States::Type applyActiveOpticForcesByBendingModes(
            ApplyActiveOpticForcesByBendingModesCommand* command, Model* model);
    virtual States::Type applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model);
    virtual States::Type clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model);
    virtual States::Type enterEngineering(EnterEngineeringCommand* command, Model* model);
    virtual States::Type exitEngineering(ExitEngineeringCommand* command, Model* model);
    virtual States::Type testAir(TestAirCommand* command, Model* model);
    virtual States::Type testHardpoint(TestHardpointCommand* command, Model* model);
    virtual States::Type testForceActuator(TestForceActuatorCommand* command, Model* model);
    virtual States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model);
    virtual States::Type enableHardpointChase(EnableHardpointChaseCommand* command, Model* model);
    virtual States::Type disableHardpointChase(DisableHardpointChaseCommand* command, Model* model);
    virtual States::Type abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model);
    virtual States::Type translateM1M3(TranslateM1M3Command* command, Model* model);
    virtual States::Type stopHardpointMotion(StopHardpointMotionCommand* command, Model* model);
    virtual States::Type storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model);
    virtual States::Type storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model);
    virtual States::Type positionM1M3(PositionM1M3Command* command, Model* model);
    virtual States::Type turnLightsOn(TurnLightsOnCommand* command, Model* model);
    virtual States::Type turnLightsOff(TurnLightsOffCommand* command, Model* model);
    virtual States::Type turnPowerOn(TurnPowerOnCommand* command, Model* model);
    virtual States::Type turnPowerOff(TurnPowerOffCommand* command, Model* model);
    virtual States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model);
    virtual States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command,
                                                     Model* model);
    virtual States::Type runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model);
    virtual States::Type abortProfile(AbortProfileCommand* command, Model* model);
    virtual States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command,
                                                        Model* model);
    virtual States::Type updatePID(UpdatePIDCommand* command, Model* model);
    virtual States::Type resetPID(ResetPIDCommand* command, Model* model);
    virtual States::Type programILC(ProgramILCCommand* command, Model* model);
    virtual States::Type modbusTransmit(ModbusTransmitCommand* command, Model* model);

protected:
    void startTimer();
    void stopTimer();
    double getCurrentTimer();
    double getTimer();

    States::Type rejectCommandInvalidState(Command* command, std::string cmd_name);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATE_H_ */
