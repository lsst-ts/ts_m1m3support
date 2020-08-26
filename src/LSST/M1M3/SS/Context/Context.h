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

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <StateTypes.h>

#include <StaticStateFactory.h>
#include <Model.h>
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

/*!
 * @brief The context used to execute commands against the current state.
 *
 * This singleton merges the model (has no concept of state) and the state (has
 * no knowledge of how to do things) to provide state based functionality.
 *
 * State objects, child of State, are responsible for verification the Command
 * is allowed at given state.
 */
class Context {
public:
    Context();

    /**
     * Retrieve Context singleton.
     *
     * @return Context singleton
     */
    static Context& get();

    /**
     * @brief Executes EnterControlCommand actions.
     *
     * @param command pointer to command to execute
     */
    void enterControl(EnterControlCommand* command);
    void start(StartCommand* command);
    void enable(EnableCommand* command);
    void disable(DisableCommand* command);
    void standby(StandbyCommand* command);
    void exitControl(ExitControlCommand* command);
    void update(UpdateCommand* command);
    void turnAirOn(TurnAirOnCommand* command);
    void turnAirOff(TurnAirOffCommand* command);
    void applyOffsetForces(ApplyOffsetForcesCommand* command);
    void clearOffsetForces(ClearOffsetForcesCommand* command);
    void raiseM1M3(RaiseM1M3Command* command);
    void lowerM1M3(LowerM1M3Command* command);
    void applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command);
    void applyAberrationForces(ApplyAberrationForcesCommand* command);
    void clearAberrationForces(ClearAberrationForcesCommand* command);
    void applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command);
    void applyActiveOpticForces(ApplyActiveOpticForcesCommand* command);
    void clearActiveOpticForces(ClearActiveOpticForcesCommand* command);
    void enterEngineering(EnterEngineeringCommand* command);
    void exitEngineering(ExitEngineeringCommand* command);
    void testAir(TestAirCommand* command);
    void testHardpoint(TestHardpointCommand* command);
    void testForceActuator(TestForceActuatorCommand* command);
    void moveHardpointActuators(MoveHardpointActuatorsCommand* command);
    void enableHardpointChase(EnableHardpointChaseCommand* command);
    void disableHardpointChase(DisableHardpointChaseCommand* command);
    void abortRaiseM1M3(AbortRaiseM1M3Command* command);
    void translateM1M3(TranslateM1M3Command* command);
    void stopHardpointMotion(StopHardpointMotionCommand* command);
    void storeTMAAzimuthSample(TMAAzimuthSampleCommand* command);
    void storeTMAElevationSample(TMAElevationSampleCommand* command);
    void positionM1M3(PositionM1M3Command* command);
    void turnLightsOn(TurnLightsOnCommand* command);
    void turnLightsOff(TurnLightsOffCommand* command);
    void turnPowerOn(TurnPowerOnCommand* command);
    void turnPowerOff(TurnPowerOffCommand* command);
    void enableHardpointCorrections(EnableHardpointCorrectionsCommand* command);
    void disableHardpointCorrections(DisableHardpointCorrectionsCommand* command);
    void runMirrorForceProfile(RunMirrorForceProfileCommand* command);
    void abortProfile(AbortProfileCommand* command);
    void applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command);
    void updatePID(UpdatePIDCommand* command);
    void resetPID(ResetPIDCommand* command);
    void programILC(ProgramILCCommand* command);
    void modbusTransmit(ModbusTransmitCommand* command);

private:
    Context& operator=(const Context&) = delete;
    Context(const Context&) = delete;

    States::Type _currentState;

    void _updateCurrentStateIfRequired(States::Type potentialNewState);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTEXT_H_ */
