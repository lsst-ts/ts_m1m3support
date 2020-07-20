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

namespace LSST {
namespace M1M3 {
namespace SS {

class StaticStateFactory;
class Model;
class EnterControlCommand;
class StartCommand;
class EnableCommand;
class DisableCommand;
class StandbyCommand;
class ExitControlCommand;
class UpdateCommand;
class TurnAirOnCommand;
class TurnAirOffCommand;
class ApplyOffsetForcesCommand;
class ClearOffsetForcesCommand;
class RaiseM1M3Command;
class LowerM1M3Command;
class ApplyAberrationForcesByBendingModesCommand;
class ApplyAberrationForcesCommand;
class ClearAberrationForcesCommand;
class ApplyActiveOpticForcesByBendingModesCommand;
class ApplyActiveOpticForcesCommand;
class ClearActiveOpticForcesCommand;
class EnterEngineeringCommand;
class ExitEngineeringCommand;
class TestAirCommand;
class TestHardpointCommand;
class TestForceActuatorCommand;
class MoveHardpointActuatorsCommand;
class EnableHardpointChaseCommand;
class DisableHardpointChaseCommand;
class AbortRaiseM1M3Command;
class TranslateM1M3Command;
class StopHardpointMotionCommand;
class TMAAzimuthSampleCommand;
class TMAElevationSampleCommand;
class PositionM1M3Command;
class TurnLightsOnCommand;
class TurnLightsOffCommand;
class TurnPowerOnCommand;
class TurnPowerOffCommand;
class EnableHardpointCorrectionsCommand;
class DisableHardpointCorrectionsCommand;
class RunMirrorForceProfileCommand;
class AbortProfileCommand;
class ApplyOffsetForcesByMirrorForceCommand;
class UpdatePIDCommand;
class ResetPIDCommand;
class ProgramILCCommand;
class ModbusTransmitCommand;

/*!
 * The context used to execute commands against the current state.
 * This class handles merging the model (has no concept of state) and
 * the state (has no knowledge of how to do things) to provide state
 * based functionality.
 */
class Context {
public:
    Context(StaticStateFactory* stateFactory, Model* model);

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
    StaticStateFactory* _stateFactory;
    Model* _model;
    States::Type _currentState;

    void _updateCurrentStateIfRequired(States::Type potentialNewState);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTEXT_H_ */
