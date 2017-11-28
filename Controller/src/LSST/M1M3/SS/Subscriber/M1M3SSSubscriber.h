/*
 * M1M3SSSubscriber.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSSUBSCRIBER_H_
#define M1M3SSSUBSCRIBER_H_

#include <ISubscriber.h>
#include <SAL_m1m3C.h>
#include <SAL_MTMountC.h>

class SAL_m1m3;
class SAL_MTMount;

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommandFactory;

/*!
 * A subscriber that utilizes the SAL.
 */
class M1M3SSSubscriber: public ISubscriber {
private:
	SAL_m1m3* m1m3SAL;
	SAL_MTMount* mtMountSAL;
	ICommandFactory* commandFactory;
	m1m3_command_StartC startData;
	m1m3_command_EnableC enableData;
	m1m3_command_DisableC disableData;
	m1m3_command_StandbyC standbyData;
	m1m3_command_ShutdownC shutdownData;
	m1m3_command_TurnAirOnC turnAirOnData;
	m1m3_command_TurnAirOffC turnAirOffData;
	m1m3_command_ApplyOffsetForcesC applyOffsetForcesData;
	m1m3_command_ClearOffsetForcesC clearOffsetForcesData;
	m1m3_command_RaiseM1M3C raiseM1M3Data;
	m1m3_command_LowerM1M3C lowerM1M3Data;
	m1m3_command_ApplyAberrationByBendingModesC applyAberrationByBendingModesData;
	m1m3_command_ApplyAberrationByForcesC applyAberrationByForcesData;
	m1m3_command_ClearAberrationC clearAberrationData;
	m1m3_command_ApplyAOSCorrectionByBendingModesC applyAOSCorrectionByBendingModesData;
	m1m3_command_ApplyAOSCorrectionByForcesC applyAOSCorrectionByForcesData;
	m1m3_command_ClearAOSCorrectionC clearAOSCorrectionData;
	m1m3_command_EnterEngineeringC enterEngineeringData;
	m1m3_command_ExitEngineeringC exitEngineeringData;
	m1m3_command_TestAirC testAirData;
	m1m3_command_TestHardpointC testHardpointData;
	m1m3_command_TestForceActuatorC testForceActuatorData;
	m1m3_command_MoveHardpointActuatorsC moveHardpointActuatorsData;
	m1m3_command_EnableHardpointChaseC enableHardpointChaseData;
	m1m3_command_DisableHardpointChaseC disableHardpointChaseData;
	m1m3_command_AbortRaiseM1M3C abortRaiseM1M3Data;
	m1m3_command_TranslateM1M3C translateM1M3Data;
	m1m3_command_StopHardpointMotionC stopHardpointMotionData;
	m1m3_command_PositionM1M3C positionM1M3Data;
	MTMount_AzC tmaAzimuth;
	MTMount_AltC tmaElevation;


public:
	M1M3SSSubscriber(SAL_m1m3* m1m3SAL, SAL_MTMount* mtMountSAL, ICommandFactory* commandFactory);

	ICommand* tryAcceptCommandStart();
	ICommand* tryAcceptCommandEnable();
	ICommand* tryAcceptCommandDisable();
	ICommand* tryAcceptCommandStandby();
	ICommand* tryAcceptCommandShutdown();
	ICommand* tryAcceptCommandTurnAirOn();
	ICommand* tryAcceptCommandTurnAirOff();
	ICommand* tryAcceptCommandApplyOffsetForces();
	ICommand* tryAcceptCommandClearOffsetForces();
	ICommand* tryAcceptCommandRaiseM1M3();
	ICommand* tryAcceptCommandLowerM1M3();
	ICommand* tryAcceptCommandApplyAberrationByBendingModes();
	ICommand* tryAcceptCommandApplyAberrationByForces();
	ICommand* tryAcceptCommandClearAberration();
	ICommand* tryAcceptCommandApplyAOSCorrectionByBendingModes();
	ICommand* tryAcceptCommandApplyAOSCorrectionByForces();
	ICommand* tryAcceptCommandClearAOSCorrection();
	ICommand* tryAcceptCommandEnterEngineering();
	ICommand* tryAcceptCommandExitEngineering();
	ICommand* tryAcceptCommandTestAir();
	ICommand* tryAcceptCommandTestHardpoint();
	ICommand* tryAcceptCommandTestForceActuator();
	ICommand* tryAcceptCommandMoveHardpointActuators();
	ICommand* tryAcceptCommandEnableHardpointChase();
	ICommand* tryAcceptCommandDisableHardpointChase();
	ICommand* tryAcceptCommandAbortRaiseM1M3();
	ICommand* tryAcceptCommandTranslateM1M3();
	ICommand* tryAcceptCommandStopHardpointMotion();
	ICommand* tryAcceptCommandPositionM1M3();
	ICommand* tryGetSampleTMAAzimuth();
	ICommand* tryGetSampleTMAElevation();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSSUBSCRIBER_H_ */
