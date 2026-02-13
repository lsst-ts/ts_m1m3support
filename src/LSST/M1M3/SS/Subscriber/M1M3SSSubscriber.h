/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

/**
 * @brief Subscriber to receive events, telemetry data and commands from SAL.
 *
 * Comntains method to try to read commands (<i>tryAcceptCommamnd*</i>) ,
 * telemetry data (<i>tryGetSample*</i>).
 */
class M1M3SSSubscriber {
public:
    M1M3SSSubscriber();

    /**
     * @brief Retrieves singleton instance.
     *
     * @return singleton instance.
     */
    static M1M3SSSubscriber& get();

    void setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL, std::shared_ptr<SAL_MTMount> mtMountSAL);

    Command* tryAcceptCommandSetLogLevel();

    /**
     * @brief Check for a Start command.
     *
     * @return NULL if the Start command isn't available. Start command if one is
     * available.
     */
    Command* tryAcceptCommandStart();
    Command* tryAcceptCommandEnable();
    Command* tryAcceptCommandDisable();
    Command* tryAcceptCommandStandby();
    Command* tryAcceptCommandExitControl();
    Command* tryAcceptCommandPanic();
    Command* tryAcceptCommandSetSlewFlag();
    Command* tryAcceptCommandClearSlewFlag();
    Command* tryAcceptCommandTurnAirOn();
    Command* tryAcceptCommandTurnAirOff();
    Command* tryAcceptCommandBoosterValveOpen();
    Command* tryAcceptCommandBoosterValveClose();
    Command* tryAcceptCommandApplyOffsetForces();
    Command* tryAcceptCommandClearOffsetForces();
    Command* tryAcceptCommandRaiseM1M3();
    Command* tryAcceptCommandLowerM1M3();
    Command* tryAcceptCommandPauseM1M3RaisingLowering();
    Command* tryAcceptCommandResumeM1M3RaisingLowering();
    Command* tryAcceptCommandApplyActiveOpticForces();
    Command* tryAcceptCommandClearActiveOpticForces();
    Command* tryAcceptCommandEnterEngineering();
    Command* tryAcceptCommandExitEngineering();
    Command* tryAcceptCommandTestHardpoint();
    Command* tryAcceptCommandKillHardpointTest();
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
    Command* tryAcceptCommandForceActuatorBumpTest();
    Command* tryAcceptCommandKillForceActuatorBumpTest();
    Command* tryAcceptCommandDisableForceActuator();
    Command* tryAcceptCommandEnableForceActuator();
    Command* tryAcceptCommandEnableAllForceActuators();
    Command* tryAcceptCommandEnableDisableForceComponent();
    Command* tryAcceptCommandSetSlewControllerSettings();

    Command* tryGetSampleTMAAzimuth();
    Command* tryGetSampleTMAElevation();

private:
    M1M3SSSubscriber& operator=(const M1M3SSSubscriber&) = delete;
    M1M3SSSubscriber(const M1M3SSSubscriber&) = delete;

    std::shared_ptr<SAL_MTM1M3> _m1m3SAL;
    std::shared_ptr<SAL_MTMount> _mtMountSAL;

    MTM1M3_command_startC _startData;
    MTM1M3_command_enableC _enableData;
    MTM1M3_command_disableC _disableData;
    MTM1M3_command_standbyC _standbyData;
    MTM1M3_command_turnAirOnC _turnAirOnData;
    MTM1M3_command_turnAirOffC _turnAirOffData;
    MTM1M3_command_applyOffsetForcesC _applyOffsetForcesData;
    MTM1M3_command_clearOffsetForcesC _clearOffsetForcesData;
    MTM1M3_command_raiseM1M3C _raiseM1M3Data;
    MTM1M3_command_lowerM1M3C _lowerM1M3Data;
    MTM1M3_command_applyActiveOpticForcesC _applyActiveOpticForcesData;
    MTM1M3_command_clearActiveOpticForcesC _clearActiveOpticForcesData;
    MTM1M3_command_enterEngineeringC _enterEngineeringData;
    MTM1M3_command_exitEngineeringC _exitEngineeringData;
    MTM1M3_command_testHardpointC _testHardpointData;
    MTM1M3_command_killHardpointTestC _killHardpointTestData;
    MTM1M3_command_moveHardpointActuatorsC _moveHardpointActuatorsData;
    MTM1M3_command_enableHardpointChaseC _enableHardpointChaseData;
    MTM1M3_command_disableHardpointChaseC _disableHardpointChaseData;
    MTM1M3_command_abortRaiseM1M3C _abortRaiseM1M3Data;
    MTM1M3_command_translateM1M3C _translateM1M3Data;
    MTM1M3_command_stopHardpointMotionC _stopHardpointMotionData;
    MTM1M3_command_positionM1M3C _positionM1M3Data;
    MTM1M3_command_turnLightsOnC _turnLightsOnData;
    MTM1M3_command_turnLightsOffC _turnLightsOffData;
    MTM1M3_command_turnPowerOnC _turnPowerOnData;
    MTM1M3_command_turnPowerOffC _turnPowerOffData;
    MTM1M3_command_enableHardpointCorrectionsC _enableHardpointCorrectionsData;
    MTM1M3_command_disableHardpointCorrectionsC _disableHardpointCorrectionsData;
    MTM1M3_command_runMirrorForceProfileC _runMirrorForceProfileData;
    MTM1M3_command_abortProfileC _abortProfileData;
    MTM1M3_command_applyOffsetForcesByMirrorForceC _applyOffsetForcesByMirrorForceData;
    MTM1M3_command_updatePIDC _updatePIDData;
    MTM1M3_command_resetPIDC _resetPIDData;
    MTM1M3_command_forceActuatorBumpTestC _forceActuatorBumpTestData;
    MTM1M3_command_killForceActuatorBumpTestC _killForceActuatorBumpTestData;
    MTM1M3_command_disableForceActuatorC _disableForceActuatorData;
    MTM1M3_command_enableForceActuatorC _enableForceActuatorData;
    MTM1M3_command_enableDisableForceComponentC _enableDisableForceComponentData;
    MTM1M3_command_setSlewControllerSettingsC _setSlewControllerSettingsData;

    MTMount_azimuthC _tmaAzimuth;
    MTMount_elevationC _tmaElevation;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSSUBSCRIBER_H_ */
