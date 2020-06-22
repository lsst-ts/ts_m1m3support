#ifndef M1M3SSSUBSCRIBER_H_
#define M1M3SSSUBSCRIBER_H_

#include <SAL_MTM1M3C.h>
#include <SAL_MTMountC.h>
#include <memory>

class SAL_MTM1M3;
class SAL_MTMount;

namespace LSST {
namespace M1M3 {
namespace SS {

class CommandFactory;
class Command;

/*!
 * A subscriber that utilizes the SAL.
 */
class M1M3SSSubscriber {
private:
    std::shared_ptr<SAL_MTM1M3> m1m3SAL;
    std::shared_ptr<SAL_MTMount> mtMountSAL;
    CommandFactory* commandFactory;
    MTM1M3_command_startC startData;
    MTM1M3_command_enableC enableData;
    MTM1M3_command_disableC disableData;
    MTM1M3_command_standbyC standbyData;
    MTM1M3_command_shutdownC shutdownData;
    MTM1M3_command_turnAirOnC turnAirOnData;
    MTM1M3_command_turnAirOffC turnAirOffData;
    MTM1M3_command_applyOffsetForcesC applyOffsetForcesData;
    MTM1M3_command_clearOffsetForcesC clearOffsetForcesData;
    MTM1M3_command_raiseM1M3C raiseM1M3Data;
    MTM1M3_command_lowerM1M3C lowerM1M3Data;
    MTM1M3_command_applyAberrationForcesByBendingModesC applyAberrationForcesByBendingModesData;
    MTM1M3_command_applyAberrationForcesC applyAberrationForcesData;
    MTM1M3_command_clearAberrationForcesC clearAberrationForcesData;
    MTM1M3_command_applyActiveOpticForcesByBendingModesC applyActiveOpticForcesByBendingModesData;
    MTM1M3_command_applyActiveOpticForcesC applyActiveOpticForcesData;
    MTM1M3_command_clearActiveOpticForcesC clearActiveOpticForcesData;
    MTM1M3_command_enterEngineeringC enterEngineeringData;
    MTM1M3_command_exitEngineeringC exitEngineeringData;
    MTM1M3_command_testAirC testAirData;
    MTM1M3_command_testHardpointC testHardpointData;
    MTM1M3_command_testForceActuatorC testForceActuatorData;
    MTM1M3_command_moveHardpointActuatorsC moveHardpointActuatorsData;
    MTM1M3_command_enableHardpointChaseC enableHardpointChaseData;
    MTM1M3_command_disableHardpointChaseC disableHardpointChaseData;
    MTM1M3_command_abortRaiseM1M3C abortRaiseM1M3Data;
    MTM1M3_command_translateM1M3C translateM1M3Data;
    MTM1M3_command_stopHardpointMotionC stopHardpointMotionData;
    MTM1M3_command_positionM1M3C positionM1M3Data;
    MTM1M3_command_turnLightsOnC turnLightsOnData;
    MTM1M3_command_turnLightsOffC turnLightsOffData;
    MTM1M3_command_turnPowerOnC turnPowerOnData;
    MTM1M3_command_turnPowerOffC turnPowerOffData;
    MTM1M3_command_enableHardpointCorrectionsC enableHardpointCorrectionsData;
    MTM1M3_command_disableHardpointCorrectionsC disableHardpointCorrectionsData;
    MTM1M3_command_runMirrorForceProfileC runMirrorForceProfileData;
    MTM1M3_command_abortProfileC abortProfileData;
    MTM1M3_command_applyOffsetForcesByMirrorForceC applyOffsetForcesByMirrorForceData;
    MTM1M3_command_updatePIDC updatePIDData;
    MTM1M3_command_resetPIDC resetPIDData;
    MTM1M3_command_programILCC programILCData;
    MTM1M3_command_modbusTransmitC modbusTransmitData;
    MTMount_AzimuthC tmaAzimuth;
    MTMount_ElevationC tmaElevation;

public:
    M1M3SSSubscriber(std::shared_ptr<SAL_MTM1M3> m1m3SAL, std::shared_ptr<SAL_MTMount> mtMountSAL,
                     CommandFactory* commandFactory);

    Command* tryAcceptCommandStart();
    Command* tryAcceptCommandEnable();
    Command* tryAcceptCommandDisable();
    Command* tryAcceptCommandStandby();
    Command* tryAcceptCommandShutdown();
    Command* tryAcceptCommandTurnAirOn();
    Command* tryAcceptCommandTurnAirOff();
    Command* tryAcceptCommandApplyOffsetForces();
    Command* tryAcceptCommandClearOffsetForces();
    Command* tryAcceptCommandRaiseM1M3();
    Command* tryAcceptCommandLowerM1M3();
    Command* tryAcceptCommandApplyAberrationForcesByBendingModes();
    Command* tryAcceptCommandApplyAberrationForces();
    Command* tryAcceptCommandClearAberrationForces();
    Command* tryAcceptCommandApplyActiveOpticForcesByBendingModes();
    Command* tryAcceptCommandApplyActiveOpticForces();
    Command* tryAcceptCommandClearActiveOpticForces();
    Command* tryAcceptCommandEnterEngineering();
    Command* tryAcceptCommandExitEngineering();
    Command* tryAcceptCommandTestAir();
    Command* tryAcceptCommandTestHardpoint();
    Command* tryAcceptCommandTestForceActuator();
    Command* tryAcceptCommandMoveHardpointActuators();
    Command* tryAcceptCommandEnableHardpointChase();
    Command* tryAcceptCommandDisableHardpointChase();
    Command* tryAcceptCommandAbortRaiseM1M3();
    Command* tryAcceptCommandTranslateM1M3();
    Command* tryAcceptCommandStopHardpointMotion();
    Command* tryAcceptCommandPositionM1M3();
    Command* tryAcceptCommandTurnLightsOn();
    Command* tryAcceptCommandTurnLightsOff();
    Command* tryAcceptCommandTurnPowerOn();
    Command* tryAcceptCommandTurnPowerOff();
    Command* tryAcceptCommandEnableHardpointCorrections();
    Command* tryAcceptCommandDisableHardpointCorrections();
    Command* tryAcceptCommandRunMirrorForceProfile();
    Command* tryAcceptCommandAbortProfile();
    Command* tryAcceptCommandApplyOffsetForcesByMirrorForce();
    Command* tryAcceptCommandUpdatePID();
    Command* tryAcceptCommandResetPID();
    Command* tryAcceptCommandProgramILC();
    Command* tryAcceptCommandModbusTransmit();
    Command* tryGetSampleTMAAzimuth();
    Command* tryGetSampleTMAElevation();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSSUBSCRIBER_H_ */
