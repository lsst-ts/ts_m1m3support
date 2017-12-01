/*
 * IState.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef ISTATE_H_
#define ISTATE_H_

#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IModel;
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

class IState {
public:
	virtual ~IState();

	virtual States::Type boot(BootCommand* command, IModel* model);
	virtual States::Type start(StartCommand* command, IModel* model);
	virtual States::Type enable(EnableCommand* command, IModel* model);
	virtual States::Type disable(DisableCommand* command, IModel* model);
	virtual States::Type standby(StandbyCommand* command, IModel* model);
	virtual States::Type shutdown(ShutdownCommand* command, IModel* model);
	virtual States::Type update(UpdateCommand* command, IModel* model);
	virtual States::Type turnAirOn(TurnAirOnCommand* command, IModel* model);
	virtual States::Type turnAirOff(TurnAirOffCommand* command, IModel* model);
	virtual States::Type applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model);
	virtual States::Type clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model);
	virtual States::Type raiseM1M3(RaiseM1M3Command* command, IModel* model);
	virtual States::Type lowerM1M3(LowerM1M3Command* command, IModel* model);
	virtual States::Type applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, IModel* model);
	virtual States::Type applyAberrationByForces(ApplyAberrationByForcesCommand* command, IModel* model);
	virtual States::Type clearAberration(ClearAberrationCommand* command, IModel* model);
	virtual States::Type applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, IModel* model);
	virtual States::Type applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, IModel* model);
	virtual States::Type clearAOSCorrection(ClearAOSCorrectionCommand* command, IModel* model);
	virtual States::Type enterEngineering(EnterEngineeringCommand* command, IModel* model);
	virtual States::Type exitEngineering(ExitEngineeringCommand* command, IModel* model);
	virtual States::Type testAir(TestAirCommand* command, IModel* model);
	virtual States::Type testHardpoint(TestHardpointCommand* command, IModel* model);
	virtual States::Type testForceActuator(TestForceActuatorCommand* command, IModel* model);
	virtual States::Type moveHardpointActuators(MoveHardpointActuatorsCommand* command, IModel* model);
	virtual States::Type enableHardpointChase(EnableHardpointChaseCommand* command, IModel* model);
	virtual States::Type disableHardpointChase(DisableHardpointChaseCommand* command, IModel* model);
	virtual States::Type abortRaiseM1M3(AbortRaiseM1M3Command* command, IModel* model);
	virtual States::Type translateM1M3(TranslateM1M3Command* command, IModel* model);
	virtual States::Type stopHardpointMotion(StopHardpointMotionCommand* command, IModel* model);
	virtual States::Type storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, IModel* model);
	virtual States::Type storeTMAElevationSample(TMAElevationSampleCommand* command, IModel* model);
	virtual States::Type positionM1M3(PositionM1M3Command* command, IModel* model);
	virtual States::Type turnLightsOn(TurnLightsOnCommand* command, IModel* model);
	virtual States::Type turnLightsOff(TurnLightsOffCommand* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISTATE_H_ */
