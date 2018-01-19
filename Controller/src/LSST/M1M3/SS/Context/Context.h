/*
 * Context.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StaticStateFactory;
class Model;
class BootCommand;
class StartCommand;
class EnableCommand;
class DisableCommand;
class StandbyCommand;
class ShutdownCommand;
class UpdateCommand;
class TurnAirOnCommand;
class TurnAirOffCommand;
class ApplyOffsetForcesCommand;
class ClearOffsetForcesCommand;
class RaiseM1M3Command;
class LowerM1M3Command;
class ApplyAberrationByBendingModesCommand;
class ApplyAberrationByForcesCommand;
class ClearAberrationCommand;
class ApplyAOSCorrectionByBendingModesCommand;
class ApplyAOSCorrectionByForcesCommand;
class ClearAOSCorrectionCommand;
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

/*!
 * The context used to execute commands against the current state.
 * This class handles merging the model (has no concept of state) and
 * the state (has no knowledge of how to do things) to provide state
 * based functionality.
 */
class Context {
private:
	StaticStateFactory* stateFactory;
	Model* model;
	States::Type currentState;

	void updateCurrentStateIfRequired(States::Type potentialNewState);

public:
	Context(StaticStateFactory* stateFactory, Model* model);

	void boot(BootCommand* command);
	void start(StartCommand* command);
	void enable(EnableCommand* command);
	void disable(DisableCommand* command);
	void standby(StandbyCommand* command);
	void shutdown(ShutdownCommand* command);
	void update(UpdateCommand* command);
	void turnAirOn(TurnAirOnCommand* command);
	void turnAirOff(TurnAirOffCommand* command);
	void applyOffsetForces(ApplyOffsetForcesCommand* command);
	void clearOffsetForces(ClearOffsetForcesCommand* command);
	void raiseM1M3(RaiseM1M3Command* command);
	void lowerM1M3(LowerM1M3Command* command);
	void applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command);
	void applyAberrationByForces(ApplyAberrationByForcesCommand* command);
	void clearAberration(ClearAberrationCommand* command);
	void applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command);
	void applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command);
	void clearAOSCorrection(ClearAOSCorrectionCommand* command);
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
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTEXT_H_ */
