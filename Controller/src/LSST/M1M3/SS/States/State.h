/*
 * State.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATE_H_
#define STATE_H_

#include <StateTypes.h>
#include <time.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class Model;
class M1M3SSPublisher;
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

class State {
private:
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
	virtual States::Type applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command, Model* model);
	virtual States::Type applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model);
	virtual States::Type clearAberrationForces(ClearAberrationForcesCommand* command, Model* model);
	virtual States::Type applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command, Model* model);
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
	virtual States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model);
	virtual States::Type runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model);
	virtual States::Type abortProfile(AbortProfileCommand* command, Model* model);
	virtual States::Type applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model);
	virtual States::Type updatePID(UpdatePIDCommand* command, Model* model);
	virtual States::Type resetPID(ResetPIDCommand* command, Model* model);

protected:
	void startTimer();
	void stopTimer();
	double getCurrentTimer();
	double getTimer();

private:
	States::Type rejectCommandInvalidState(std::string command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATE_H_ */
