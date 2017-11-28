/*
 * IContext.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef ICONTEXT_H_
#define ICONTEXT_H_

namespace LSST {
namespace M1M3 {
namespace SS {

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

/*!
 * An interface responsible for the context of a commands execution.
 */
class IContext {
public:
	virtual ~IContext();

	/*!
	 * Executes the boot command against the current state.
	 * @param[in] command The boot command to execute.
	 */
	virtual void boot(BootCommand* command);
	/*!
	 * Executes the start command against the current state.
	 * @param[in] command The start command to execute.
	 */
	virtual void start(StartCommand* command);
	/*!
	 * Executes the enable command against the current state.
	 * @param[in] command The enable command to execute.
	 */
	virtual void enable(EnableCommand* command);
	/*!
	 * Executes the disable command against the current state.
	 * @param[in] command The disable command to execute.
	 */
	virtual void disable(DisableCommand* command);
	/*!
	 * Executes the standby command against the current state.
	 * @param[in] command The standby command to execute.
	 */
	virtual void standby(StandbyCommand* command);
	/*!
	 * Executes the shutdown command against the current state.
	 * @param[in] command The shutdown command to execute.
	 */
	virtual void shutdown(ShutdownCommand* command);
	/*!
	 * Executes the update command against the current state.
	 * @param[in] command The update command to execute.
	 */
	virtual void update(UpdateCommand* command);
	/*!
	 * Executes the command to turn air on.
	 * @param[in] command The turn on air command to execute.
	 */
	virtual void turnAirOn(TurnAirOnCommand* command);
	/*!
	 * Executes the command to turn air off.
	 * @param[in] command The turn off air command to execute.
	 */
	virtual void turnAirOff(TurnAirOffCommand* command);
	/*!
	 * Executes the command to apply offset forces.
	 * @param[in] command The apply offset forces command to execute.
	 */
	virtual void applyOffsetForces(ApplyOffsetForcesCommand* command);
	/*!
	 * Executes the command to clear offset forces.
	 * @param[in] command The clear offset forces command to execute.
	 */
	virtual void clearOffsetForces(ClearOffsetForcesCommand* command);
	/*!
	 * Executes the command to raise M1M3.
	 * @param[in] command The raise M1M3 command to execute.
	 */
	virtual void raiseM1M3(RaiseM1M3Command* command);
	/*!
	 * Executes the command to lower M1M3.
	 * @param[in] command The lower M1M3 command to execute.
	 */
	virtual void lowerM1M3(LowerM1M3Command* command);
	/*!
	 * Executes the command to apply an aberration by bending modes.
	 * @param[in] command The apply aberration by bending modes command to execute.
	 */
	virtual void applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command);
	/*!
	 * Executes the command to apply an aberration by forces.
	 * @param[in] command The apply aberration by forces command to execute.
	 */
	virtual void applyAberrationByForces(ApplyAberrationByForcesCommand* command);
	/*!
	 * Executes the command to clear an aberration.
	 * @param[in] command The clear aberration command to execute.
	 */
	virtual void clearAberration(ClearAberrationCommand* command);
	/*!
	 * Executes the command to apply an AOS correction by bending modes.
	 * @param[in] command The apply an AOS correction by bending modes command to execute.
	 */
	virtual void applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command);
	/*!
	 * Executes the command to apply an AOS correction by forces.
	 * @param[in] command The apply an AOS correction by forces command to execute.
	 */
	virtual void applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command);
	/*!
	 * Executes the command to clear AOS corrections.
	 * @param[in] command The clear AOS corrections command to execute.
	 */
	virtual void clearAOSCorrection(ClearAOSCorrectionCommand* command);
	/*!
	 * Executes the command to enter engineering state.
	 * @param[in] command The enter engineering command to execute.
	 */
	virtual void enterEngineering(EnterEngineeringCommand* command);
	/*!
	 * Executes the command to exit engineering state.
	 * @param[in] command The exit engineering command to execute.
	 */
	virtual void exitEngineering(ExitEngineeringCommand* command);
	/*!
	 * Executes the command to test air.
	 * @param[in] command The test air command to execute.
	 */
	virtual void testAir(TestAirCommand* command);
	/*!
	 * Executes the command to test hardpoints.
	 * @param[in] command The test hardpoints command to execute.
	 */
	virtual void testHardpoint(TestHardpointCommand* command);
	/*!
	 * Executes the command to test force actuators.
	 * @param[in] command The test force actuators command to execute.
	 */
	virtual void testForceActuator(TestForceActuatorCommand* command);
	/*!
	 * Executes the command to move hardpoint actuators.
	 * @param[in] command The move hardpoint actuators command to execute.
	 */
	virtual void moveHardpointActuators(MoveHardpointActuatorsCommand* command);
	/*!
	 * Executes the command to enable hardpoint chase.
	 * @param[in] command The enable hardpoint chase command to execute.
	 */
	virtual void enableHardpointChase(EnableHardpointChaseCommand* command);
	/*!
	 * Executes the command to disable hardpoint chase.
	 * @param[in] command The disable hardpoint chase command to execute.
	 */
	virtual void disableHardpointChase(DisableHardpointChaseCommand* command);
	/*!
	 * Executes the command to abort raise M1M3.
	 * @param[in] command The abort raise M1M3 command to execute.
	 */
	virtual void abortRaiseM1M3(AbortRaiseM1M3Command* command);
	/*!
	 * Executes the command to translate M1M3.
	 * @param[in] command The translate M1M3 command to execute.
	 */
	virtual void translateM1M3(TranslateM1M3Command* command);
	/*!
	 * Executes the command to stop hardpoint motion.
	 * @param[in] command The stop hardpoint motion command to execute.
	 */
	virtual void stopHardpointMotion(StopHardpointMotionCommand* command);
	/*!
	 * Executes the command to store TMA azimuth data.
	 * @param[in] command The TMA azimuth sample command to execute.
	 */
	virtual void storeTMAAzimuthSample(TMAAzimuthSampleCommand* command);
	/*!
	 * Executes the command to store TMA elevation data.
	 * @param[in] command The TMA elevation sample command to execute.
	 */
	virtual void storeTMAElevationSample(TMAElevationSampleCommand* command);
	/*!
	 * Executes the command to position M1M3.
	 * @param[in] command The position M1M3 command to execute.
	 */
	virtual void positionM1M3(PositionM1M3Command* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICONTEXT_H_ */
