/*
 * ISubscriber.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef ISUBSCRIBER_H_
#define ISUBSCRIBER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommand;

/*!
 * Interface for getting commands to execute (for this subsystem) and telemetry from other subsystems.
 * An example of a command might be the Start command.
 * An example of another subsystems telemetry might be the ElevationAngle from TMA.
 */
class ISubscriber {
public:
	virtual ~ISubscriber();

	virtual ICommand* tryAcceptCommandStart();
	virtual ICommand* tryAcceptCommandEnable();
	virtual ICommand* tryAcceptCommandDisable();
	virtual ICommand* tryAcceptCommandStandby();
	virtual ICommand* tryAcceptCommandShutdown();
	virtual ICommand* tryAcceptCommandTurnAirOn();
	virtual ICommand* tryAcceptCommandTurnAirOff();
	virtual ICommand* tryAcceptCommandApplyOffsetForces();
	virtual ICommand* tryAcceptCommandClearOffsetForces();
	virtual ICommand* tryAcceptCommandRaiseM1M3();
	virtual ICommand* tryAcceptCommandLowerM1M3();
	virtual ICommand* tryAcceptCommandApplyAberrationByBendingModes();
	virtual ICommand* tryAcceptCommandApplyAberrationByForces();
	virtual ICommand* tryAcceptCommandClearAberration();
	virtual ICommand* tryAcceptCommandApplyAOSCorrectionByBendingModes();
	virtual ICommand* tryAcceptCommandApplyAOSCorrectionByForces();
	virtual ICommand* tryAcceptCommandClearAOSCorrection();
	virtual ICommand* tryAcceptCommandEnterEngineering();
	virtual ICommand* tryAcceptCommandExitEngineering();
	virtual ICommand* tryAcceptCommandTestAir();
	virtual ICommand* tryAcceptCommandTestHardpoint();
	virtual ICommand* tryAcceptCommandTestForceActuator();
	virtual ICommand* tryAcceptCommandMoveHardpointActuators();
	virtual ICommand* tryAcceptCommandEnableHardpointChase();
	virtual ICommand* tryAcceptCommandDisableHardpointChase();
	virtual ICommand* tryAcceptCommandAbortRaiseM1M3();
	virtual ICommand* tryAcceptCommandTranslateM1M3();
	virtual ICommand* tryAcceptCommandStopHardpointMotion();
	virtual ICommand* tryAcceptCommandPositionM1M3();
	virtual ICommand* tryAcceptCommandTurnLightsOn();
	virtual ICommand* tryAcceptCommandTurnLightsOff();
	virtual ICommand* tryAcceptCommandTurnPowerOn();
	virtual ICommand* tryAcceptCommandTurnPowerOff();
	virtual ICommand* tryGetSampleTMAAzimuth();
	virtual ICommand* tryGetSampleTMAElevation();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISUBSCRIBER_H_ */
