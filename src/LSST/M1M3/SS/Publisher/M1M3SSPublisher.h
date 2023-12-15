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

#ifndef M1M3SSPUBLISHER_H_
#define M1M3SSPUBLISHER_H_

#include <cRIO/DataTypes.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTM1M3C.h>
#include <ccpp_sal_MTM1M3.h>

#include <cRIO/Singleton.h>

#include <EnabledForceActuators.h>
#include <ForceActuatorWarning.h>
#include <PowerSupplyStatus.h>

#include <memory>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * @brief Singleton class for M1M3 SAL communication.
 *
 * Encapsulates methods to send events, telemetry data and commands
 * acknowledgments over SAL.
 *
 * @see M1M3SSSubscriber
 */
class M1M3SSPublisher : public cRIO::Singleton<M1M3SSPublisher> {
public:
    M1M3SSPublisher(token);

    void setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL);

    /**
     * Called when new settigns are loaded to resend 0 events.
     */
    void reset();

    /**
     * @brief Returns pointer to accelerometer data.
     *
     * @return pointer to the accelerometer data
     *
     * @see putAccelerometerData
     */
    MTM1M3_accelerometerDataC* getAccelerometerData() { return &_accelerometerData; }
    MTM1M3_gyroDataC* getGyroData() { return &_gyroData; }
    MTM1M3_hardpointActuatorDataC* getHardpointActuatorData() { return &_hardpointActuatorData; }
    MTM1M3_hardpointMonitorDataC* getHardpointMonitorData() { return &_hardpointMonitorData; }
    MTM1M3_imsDataC* getIMSData() { return &_imsData; }
    MTM1M3_inclinometerDataC* getInclinometerData() { return &_inclinometerData; }
    MTM1M3_outerLoopDataC* getOuterLoopData() { return &_outerLoopData; }
    MTM1M3_pidDataC* getPIDData() { return &_pidData; }
    MTM1M3_powerSupplyDataC* getPowerSupplyData() { return &_powerSupplyData; }

    MTM1M3_logevent_accelerometerWarningC* getEventAccelerometerWarning() {
        return &_eventAccelerometerWarning;
    }
    MTM1M3_logevent_airSupplyWarningC* getEventAirSupplyWarning() { return &_eventAirSupplyWarning; }
    MTM1M3_appliedAccelerationForcesC* getAppliedAccelerationForces() { return &_appliedAccelerationForces; }
    MTM1M3_logevent_appliedActiveOpticForcesC* getEventAppliedActiveOpticForces() {
        return &_eventAppliedActiveOpticForces;
    }
    MTM1M3_appliedAzimuthForcesC* getAppliedAzimuthForces() { return &_appliedAzimuthForces; }
    MTM1M3_appliedBalanceForcesC* getAppliedBalanceForces() { return &_appliedBalanceForces; }
    MTM1M3_appliedCylinderForcesC* getAppliedCylinderForces() { return &_appliedCylinderForces; }
    MTM1M3_appliedElevationForcesC* getAppliedElevationForces() { return &appliedElevationForces; }
    MTM1M3_appliedForcesC* getAppliedForces() { return &_appliedForces; }
    MTM1M3_logevent_appliedOffsetForcesC* getEventAppliedOffsetForces() { return &_eventAppliedOffsetForces; }
    MTM1M3_logevent_appliedStaticForcesC* getEventAppliedStaticForces() { return &eventAppliedStaticForces; }
    MTM1M3_appliedThermalForcesC* getAppliedThermalForces() { return &_appliedThermalForces; }
    MTM1M3_appliedVelocityForcesC* getAppliedVelocityForces() { return &_appliedVelocityForces; }
    MTM1M3_logevent_cellLightStatusC* getEventCellLightStatus() { return &_eventCellLightStatus; }
    MTM1M3_logevent_cellLightWarningC* getEventCellLightWarning() { return &_eventCellLightWarning; }
    MTM1M3_logevent_commandRejectionWarningC* getEventCommandRejectionWarning() {
        return &_eventCommandRejectionWarning;
    }
    MTM1M3_logevent_detailedStateC* getEventDetailedState() { return &_eventDetailedState; }
    MTM1M3_logevent_displacementSensorWarningC* getEventDisplacementSensorWarning() {
        return &_eventDisplacementSensorWarning;
    }
    static EnabledForceActuators* getEnabledForceActuators() { return &(instance()._enabledForceActuators); }
    MTM1M3_logevent_errorCodeC* getEventErrorCode() { return &_eventErrorCode; }
    MTM1M3_logevent_forceActuatorBumpTestStatusC* getEventForceActuatorBumpTestStatus() {
        return &_eventForceActuatorBumpTestStatus;
    }
    MTM1M3_logevent_forceActuatorStateC* getEventForceActuatorState() { return &_eventForceActuatorState; }
    static ForceActuatorWarning* getForceActuatorWarning() { return &(instance()._forceActuatorWarning); }
    MTM1M3_logevent_forceSetpointWarningC* getEventForceSetpointWarning() {
        return &_eventForceSetpointWarning;
    }
    MTM1M3_logevent_gyroWarningC* getEventGyroWarning() { return &_eventGyroWarning; }
    MTM1M3_logevent_hardpointActuatorInfoC* getEventHardpointActuatorInfo() {
        return &_eventHardpointActuatorInfo;
    }
    MTM1M3_logevent_hardpointActuatorStateC* getEventHardpointActuatorState() {
        return &_eventHardpointActuatorState;
    }
    MTM1M3_logevent_hardpointMonitorInfoC* getEventHardpointMonitorInfo() {
        return &_eventHardpointMonitorInfo;
    }
    MTM1M3_logevent_hardpointMonitorStateC* getEventHardpointMonitorState() {
        return &_eventHardpointMonitorState;
    }
    MTM1M3_logevent_hardpointMonitorWarningC* getEventHardpointMonitorWarning() {
        return &_eventHardpointMonitorWarning;
    }
    MTM1M3_logevent_ilcWarningC* getEventILCWarning() { return &_eventILCWarning; }
    MTM1M3_logevent_inclinometerSensorWarningC* getEventInclinometerSensorWarning() {
        return &_eventInclinometerSensorWarning;
    }
    MTM1M3_logevent_interlockWarningC* getEventInterlockWarning() { return &_eventInterlockWarning; }
    MTM1M3_logevent_pidInfoC* getEventPIDInfo() { return &_eventPIDInfo; }
    MTM1M3_logevent_powerStatusC* getEventPowerStatus() { return &_eventPowerStatus; }
    static PowerSupplyStatus* getPowerSupplyStatus() { return &(instance()._powerSupplyStatus); }
    MTM1M3_logevent_powerWarningC* getEventPowerWarning() { return &_eventPowerWarning; }
    MTM1M3_logevent_preclippedAccelerationForcesC* getEventPreclippedAccelerationForces() {
        return &_eventPreclippedAccelerationForces;
    }
    MTM1M3_logevent_preclippedActiveOpticForcesC* getEventPreclippedActiveOpticForces() {
        return &_eventPreclippedActiveOpticForces;
    }
    MTM1M3_logevent_preclippedAzimuthForcesC* getEventPreclippedAzimuthForces() {
        return &_eventPreclippedAzimuthForces;
    }
    MTM1M3_logevent_preclippedBalanceForcesC* getEventPreclippedBalanceForces() {
        return &_eventPreclippedBalanceForces;
    }
    MTM1M3_logevent_preclippedCylinderForcesC* getEventPreclippedCylinderForces() {
        return &_eventPreclippedCylinderForces;
    }
    MTM1M3_logevent_preclippedElevationForcesC* getEventPreclippedElevationForces() {
        return &_eventPreclippedElevationForces;
    }
    MTM1M3_logevent_preclippedForcesC* getEventPreclippedForces() { return &_eventPreclippedForces; }
    MTM1M3_logevent_preclippedOffsetForcesC* getEventPreclippedOffsetForces() {
        return &_eventPreclippedOffsetForces;
    }
    MTM1M3_logevent_preclippedStaticForcesC* getEventPreclippedStaticForces() {
        return &_eventPreclippedStaticForces;
    }
    MTM1M3_logevent_preclippedThermalForcesC* getEventPreclippedThermalForces() {
        return &_eventPreclippedThermalForces;
    }
    MTM1M3_logevent_preclippedVelocityForcesC* getEventPreclippedVelocityForces() {
        return &_eventPreclippedVelocityForces;
    }
    MTM1M3_logevent_configurationsAvailableC* getEventConfigurationsAvailable() {
        return &_eventConfigurationsAvailable;
    }
    MTM1M3_logevent_configurationAppliedC* getEventConfigurationApplied() {
        return &_eventConfigurationApplied;
    }
    MTM1M3_logevent_summaryStateC* getEventSummaryState() { return &_eventSummaryState; }

    /**
     * Returns current timestamp.
     *
     * @return current timestamp (TAI as seconds since 1/1/1970)
     */
    double getTimestamp() { return _m1m3SAL->getCurrentTime(); }

    /**
     * Sends accelerometer data stored in pointer returned by
     * getAccelerometerData().
     */
    void putAccelerometerData();
    void putForceActuatorData(MTM1M3_forceActuatorDataC* data) {
        _m1m3SAL->putSample_forceActuatorData(data);
    }
    void putGyroData();
    void putHardpointActuatorData();
    void putHardpointMonitorData();
    void putIMSData();
    void putInclinometerData();
    void putOuterLoopData();
    void putPIDData();
    void putPowerSupplyData();

    void logAccelerometerSettings(MTM1M3_logevent_accelerometerSettingsC* data) {
        _m1m3SAL->logEvent_accelerometerSettings(data, 0);
    }
    void logPositionControllerSettings(MTM1M3_logevent_positionControllerSettingsC* data) {
        _m1m3SAL->logEvent_positionControllerSettings(data, 0);
    }
    void logSlewControllerSettings(MTM1M3_logevent_slewControllerSettingsC* data) {
        _m1m3SAL->logEvent_slewControllerSettings(data, 0);
    }

    /**
     * @brief Sends AccelerometerWarning event.
     *
     * @see tryLogAccelerometerWarning
     */
    void logAccelerometerWarning();

    /**
     * @brief Sends AccelerometerWarning event if event data changed from last
     * successful (accepted in tryLogAccelerometerWarning) send.
     *
     * Calls logAccelerometerWarning().
     */
    void tryLogAccelerometerWarning();
    void logAirSupplyStatus(MTM1M3_logevent_airSupplyStatusC* data) {
        _m1m3SAL->logEvent_airSupplyStatus(data, 0);
    }
    void logAirSupplyWarning();
    void tryLogAirSupplyWarning();
    void logAppliedAccelerationForces();
    void logAppliedActiveOpticForces();
    void logAppliedAzimuthForces();
    void logAppliedBalanceForces();
    void logAppliedCylinderForces();
    void logAppliedElevationForces();
    void logAppliedForces();
    void logAppliedOffsetForces();
    void logAppliedStaticForces();
    void logAppliedThermalForces();
    void logAppliedVelocityForces();
    void logBoosterValveSettings(MTM1M3_logevent_boosterValveSettingsC* data) {
        _m1m3SAL->logEvent_boosterValveSettings(data, 0);
    }
    void logBoosterValveStatus(MTM1M3_logevent_boosterValveStatusC* data) {
        _m1m3SAL->logEvent_boosterValveStatus(data, 0);
    }
    void logCellLightStatus();
    void tryLogCellLightStatus();
    void logCellLightWarning();
    void tryLogCellLightWarning();
    void logCommandRejectionWarning();
    void logCommandRejectionWarning(std::string command, std::string reason);
    template <typename... Args>
    void commandFailed(std::string command, std::string const& format, Args const&... args) {
        std::string reason = fmt::format(format, args...);
        logCommandRejectionWarning(command, reason);
        throw std::runtime_error(reason);
    }
    void logDetailedState();
    void tryLogDetailedState();
    void logDisplacementSensorSettings(MTM1M3_logevent_displacementSensorSettingsC* data) {
        _m1m3SAL->logEvent_displacementSensorSettings(data, 0);
    }
    void logDisplacementSensorWarning();
    void logEnabledForceActuators(MTM1M3_logevent_enabledForceActuatorsC* data) {
        _m1m3SAL->logEvent_enabledForceActuators(data, 0);
    }
    void tryLogDisplacementSensorWarning();
    void logErrorCode();
    void tryLogErrorCode();
    void logForceActuatorSettings(MTM1M3_logevent_forceActuatorSettingsC* data) {
        _m1m3SAL->logEvent_forceActuatorSettings(data, 0);
    }
    void logForceActuatorBumpTestStatus();
    void logForceActuatorForceWarning(MTM1M3_logevent_forceActuatorForceWarningC* data) {
        _m1m3SAL->logEvent_forceActuatorForceWarning(data, 0);
    }
    void logForceActuatorFollowingErrorCounter(MTM1M3_logevent_forceActuatorFollowingErrorCounterC* data) {
        _m1m3SAL->logEvent_forceActuatorFollowingErrorCounter(data, 0);
    }
    void tryLogForceActuatorForceWarning();
    void logForceActuatorInfo(MTM1M3_logevent_forceActuatorInfoC* data) {
        _m1m3SAL->logEvent_forceActuatorInfo(data, 0);
    }
    void logForceActuatorState();
    void tryLogForceActuatorState();
    void logForceActuatorWarning(MTM1M3_logevent_forceActuatorWarningC* data) {}
    ///        _m1m3SAL->logEvent_forceActuatorWarning(data, 0);
    ///    }

    void logForceControllerState(MTM1M3_logevent_forceControllerStateC* data) {
        _m1m3SAL->logEvent_forceControllerState(data, 0);
    }
    void logForceSetpointWarning();
    void tryLogForceSetpointWarning();
    void logGyroSettings(MTM1M3_logevent_gyroSettingsC* data) { _m1m3SAL->logEvent_gyroSettings(data, 0); }
    void logGyroWarning();
    void tryLogGyroWarning();
    void logHardpointActuatorInfo();
    void tryLogHardpointActuatorInfo();
    void logHardpointActuatorSettings(MTM1M3_logevent_hardpointActuatorSettingsC* data) {
        _m1m3SAL->logEvent_hardpointActuatorSettings(data, 0);
    }
    void logHardpointActuatorState();
    void tryLogHardpointActuatorState();
    void logHardpointActuatorWarning(MTM1M3_logevent_hardpointActuatorWarningC* data) {
        _m1m3SAL->logEvent_hardpointActuatorWarning(data, 0);
    }
    void logHardpointMonitorInfo();
    void tryLogHardpointMonitorInfo();
    void logHardpointMonitorState();
    void tryLogHardpointMonitorState();
    void logHardpointMonitorWarning();
    void tryLogHardpointMonitorWarning();
    void logHardpointTestStatus(MTM1M3_logevent_hardpointTestStatusC* data) {
        _m1m3SAL->logEvent_hardpointTestStatus(data, 0);
    }
    void logHeartbeat(MTM1M3_logevent_heartbeatC* data) { _m1m3SAL->logEvent_heartbeat(data, 0); }
    void logILCWarning(MTM1M3_logevent_ilcWarningC* data) { _m1m3SAL->logEvent_ilcWarning(data, 0); }
    void logInclinometerSettings(MTM1M3_logevent_inclinometerSettingsC* data) {
        _m1m3SAL->logEvent_inclinometerSettings(data, 0);
    }
    void logInclinometerSensorWarning();
    void tryLogInclinometerSensorWarning();
    void logInterlockStatus(MTM1M3_logevent_interlockStatusC* data) {
        _m1m3SAL->logEvent_interlockStatus(data, 0);
    }
    void logInterlockWarning(MTM1M3_logevent_interlockWarningC* data) {
        _m1m3SAL->logEvent_interlockWarning(data, 0);
    };
    void newLogLevel(int newLevel);
    void logPIDInfo();
    void logPIDSettings(MTM1M3_logevent_pidSettingsC* data) { _m1m3SAL->logEvent_pidSettings(data, 0); }
    void logPowerStatus();
    void tryLogPowerStatus();
    void logPowerSupplyStatus(MTM1M3_logevent_powerSupplyStatusC* data) {
        _m1m3SAL->logEvent_powerSupplyStatus(data, 0);
    }
    void logPowerWarning();
    void tryLogPowerWarning();
    void logPreclippedAccelerationForces();
    void logPreclippedActiveOpticForces();
    void logPreclippedAzimuthForces();
    void logPreclippedBalanceForces();
    void logPreclippedCylinderForces();
    void logPreclippedElevationForces();
    void logPreclippedForces();
    void logPreclippedOffsetForces();
    void logPreclippedStaticForces();
    void logPreclippedThermalForces();
    void logPreclippedVelocityForces();
    void logConfigurationsAvailable() {
        _m1m3SAL->logEvent_configurationsAvailable(&_eventConfigurationsAvailable, 0);
    }
    void logConfigurationApplied() {
        _m1m3SAL->logEvent_configurationApplied(&_eventConfigurationApplied, 0);
    }
    void logSimulationMode(MTM1M3_logevent_simulationModeC* data) {
        _m1m3SAL->logEvent_simulationMode(data, 0);
    }
    void logSoftwareVersions();
    void logSummaryState();
    void tryLogSummaryState();

    void logRaisingLoweringInfo(MTM1M3_logevent_raisingLoweringInfoC* data) {
        _m1m3SAL->logEvent_raisingLoweringInfo(data, 0);
    }

    /**
     * @brief Acknowledges start command.
     *
     * @param commandID
     * @param ackCode acknowledgement code. Either ACK_INPROGRESS, ACK_FAILED or ACK_COMPLETE
     * @param description string collected during command execution
     * @param timeout reported command timeout. Should be used for only for ACK_INPROGRESS acknowledgement
     */
#define ACK_COMMAND_H(command)                                                            \
    void ackCommand##command(int32_t commandID, int32_t ackCode, std::string description, \
                             double timeout = 0.0);

    ACK_COMMAND_H(setLogLevel)
    ACK_COMMAND_H(start)
    ACK_COMMAND_H(enable)
    ACK_COMMAND_H(disable)
    ACK_COMMAND_H(standby)
    ACK_COMMAND_H(exitControl)
    ACK_COMMAND_H(panic)
    ACK_COMMAND_H(setSlewFlag)
    ACK_COMMAND_H(clearSlewFlag)
    ACK_COMMAND_H(turnAirOn)
    ACK_COMMAND_H(turnAirOff)
    ACK_COMMAND_H(applyOffsetForces)
    ACK_COMMAND_H(clearOffsetForces)
    ACK_COMMAND_H(raiseM1M3)
    ACK_COMMAND_H(lowerM1M3)
    ACK_COMMAND_H(pauseM1M3RaisingLowering)
    ACK_COMMAND_H(resumeM1M3RaisingLowering)
    ACK_COMMAND_H(applyActiveOpticForces)
    ACK_COMMAND_H(clearActiveOpticForces)
    ACK_COMMAND_H(enterEngineering)
    ACK_COMMAND_H(exitEngineering)
    ACK_COMMAND_H(boosterValveOpen)
    ACK_COMMAND_H(boosterValveClose)
    ACK_COMMAND_H(testHardpoint)
    ACK_COMMAND_H(killHardpointTest)
    ACK_COMMAND_H(moveHardpointActuators)
    ACK_COMMAND_H(enableHardpointChase)
    ACK_COMMAND_H(disableHardpointChase)
    ACK_COMMAND_H(abortRaiseM1M3)
    ACK_COMMAND_H(translateM1M3)
    ACK_COMMAND_H(stopHardpointMotion)
    ACK_COMMAND_H(positionM1M3)
    ACK_COMMAND_H(turnLightsOn)
    ACK_COMMAND_H(turnLightsOff)
    ACK_COMMAND_H(turnPowerOn)
    ACK_COMMAND_H(turnPowerOff)
    ACK_COMMAND_H(enableHardpointCorrections)
    ACK_COMMAND_H(disableHardpointCorrections)
    ACK_COMMAND_H(runMirrorForceProfile)
    ACK_COMMAND_H(abortProfile)
    ACK_COMMAND_H(applyOffsetForcesByMirrorForce)
    ACK_COMMAND_H(updatePID)
    ACK_COMMAND_H(resetPID)
    ACK_COMMAND_H(forceActuatorBumpTest)
    ACK_COMMAND_H(killForceActuatorBumpTest)
    ACK_COMMAND_H(disableForceActuator)
    ACK_COMMAND_H(enableForceActuator)
    ACK_COMMAND_H(enableAllForceActuators)
    ACK_COMMAND_H(enableDisableForceComponent)
    ACK_COMMAND_H(setSlewControllerSettings)

private:
    M1M3SSPublisher& operator=(const M1M3SSPublisher&) = delete;
    M1M3SSPublisher(const M1M3SSPublisher&) = delete;

    std::shared_ptr<SAL_MTM1M3> _m1m3SAL;

    MTM1M3_accelerometerDataC _accelerometerData;
    MTM1M3_gyroDataC _gyroData;
    MTM1M3_hardpointActuatorDataC _hardpointActuatorData;
    MTM1M3_hardpointMonitorDataC _hardpointMonitorData;
    MTM1M3_imsDataC _imsData;
    MTM1M3_inclinometerDataC _inclinometerData;
    MTM1M3_outerLoopDataC _outerLoopData;
    MTM1M3_pidDataC _pidData;
    MTM1M3_powerSupplyDataC _powerSupplyData;

    MTM1M3_logevent_accelerometerWarningC _eventAccelerometerWarning;
    MTM1M3_logevent_airSupplyWarningC _eventAirSupplyWarning;

    MTM1M3_appliedAccelerationForcesC _appliedAccelerationForces;
    MTM1M3_appliedAzimuthForcesC _appliedAzimuthForces;
    MTM1M3_appliedBalanceForcesC _appliedBalanceForces;
    MTM1M3_appliedCylinderForcesC _appliedCylinderForces;
    MTM1M3_appliedElevationForcesC appliedElevationForces;
    MTM1M3_appliedForcesC _appliedForces;
    MTM1M3_appliedThermalForcesC _appliedThermalForces;
    MTM1M3_appliedVelocityForcesC _appliedVelocityForces;

    MTM1M3_logevent_appliedActiveOpticForcesC _eventAppliedActiveOpticForces;
    MTM1M3_logevent_appliedOffsetForcesC _eventAppliedOffsetForces;
    MTM1M3_logevent_appliedStaticForcesC eventAppliedStaticForces;
    MTM1M3_logevent_cellLightStatusC _eventCellLightStatus;
    MTM1M3_logevent_cellLightWarningC _eventCellLightWarning;
    MTM1M3_logevent_commandRejectionWarningC _eventCommandRejectionWarning;
    MTM1M3_logevent_detailedStateC _eventDetailedState;
    MTM1M3_logevent_displacementSensorWarningC _eventDisplacementSensorWarning;
    EnabledForceActuators _enabledForceActuators;
    MTM1M3_logevent_errorCodeC _eventErrorCode;
    MTM1M3_logevent_forceActuatorBumpTestStatusC _eventForceActuatorBumpTestStatus;
    MTM1M3_logevent_forceActuatorStateC _eventForceActuatorState;
    ForceActuatorWarning _forceActuatorWarning;
    MTM1M3_logevent_forceSetpointWarningC _eventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC _eventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC _eventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC _eventHardpointActuatorState;
    MTM1M3_logevent_hardpointMonitorInfoC _eventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC _eventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC _eventHardpointMonitorWarning;
    MTM1M3_logevent_ilcWarningC _eventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC _eventInclinometerSensorWarning;
    MTM1M3_logevent_interlockWarningC _eventInterlockWarning;
    MTM1M3_logevent_pidInfoC _eventPIDInfo;
    MTM1M3_logevent_powerStatusC _eventPowerStatus;
    PowerSupplyStatus _powerSupplyStatus;
    MTM1M3_logevent_powerWarningC _eventPowerWarning;
    MTM1M3_logevent_preclippedAccelerationForcesC _eventPreclippedAccelerationForces;
    MTM1M3_logevent_preclippedActiveOpticForcesC _eventPreclippedActiveOpticForces;
    MTM1M3_logevent_preclippedAzimuthForcesC _eventPreclippedAzimuthForces;
    MTM1M3_logevent_preclippedBalanceForcesC _eventPreclippedBalanceForces;
    MTM1M3_logevent_preclippedCylinderForcesC _eventPreclippedCylinderForces;
    MTM1M3_logevent_preclippedElevationForcesC _eventPreclippedElevationForces;
    MTM1M3_logevent_preclippedForcesC _eventPreclippedForces;
    MTM1M3_logevent_preclippedOffsetForcesC _eventPreclippedOffsetForces;
    MTM1M3_logevent_preclippedStaticForcesC _eventPreclippedStaticForces;
    MTM1M3_logevent_preclippedThermalForcesC _eventPreclippedThermalForces;
    MTM1M3_logevent_preclippedVelocityForcesC _eventPreclippedVelocityForces;
    MTM1M3_logevent_configurationsAvailableC _eventConfigurationsAvailable;
    MTM1M3_logevent_configurationAppliedC _eventConfigurationApplied;
    MTM1M3_logevent_summaryStateC _eventSummaryState;

    MTM1M3_logevent_accelerometerWarningC _previousEventAccelerometerWarning;
    MTM1M3_logevent_airSupplyWarningC _previousEventAirSupplyWarning;
    MTM1M3_logevent_appliedActiveOpticForcesC _previousEventAppliedActiveOpticForces;
    MTM1M3_logevent_appliedOffsetForcesC _previousEventAppliedOffsetForces;
    MTM1M3_logevent_appliedStaticForcesC _previousEventAppliedStaticForces;
    MTM1M3_logevent_cellLightStatusC _previousEventCellLightStatus;
    MTM1M3_logevent_cellLightWarningC _previousEventCellLightWarning;
    MTM1M3_logevent_commandRejectionWarningC _previousEventCommandRejectionWarning;
    MTM1M3_logevent_detailedStateC _previousEventDetailedState;
    MTM1M3_logevent_displacementSensorWarningC _previousEventDisplacementSensorWarning;
    MTM1M3_logevent_errorCodeC _previousEventErrorCode;
    MTM1M3_logevent_forceActuatorBumpTestStatusC _previousEventForceActuatorBumpTestStatus;
    MTM1M3_logevent_forceActuatorStateC _previousEventForceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC _previousEventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC _previousEventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC _previousEventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC _previousEventHardpointActuatorState;
    MTM1M3_logevent_hardpointMonitorInfoC _previousEventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC _previousEventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC _previousEventHardpointMonitorWarning;
    MTM1M3_logevent_ilcWarningC _previousEventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC _previousEventInclinometerSensorWarning;
    MTM1M3_logevent_interlockWarningC _previousEventInterlockWarning;
    MTM1M3_logevent_powerStatusC _previousEventPowerStatus;
    MTM1M3_logevent_powerSupplyStatusC _previousEventPowerSupplyStatus;
    MTM1M3_logevent_powerWarningC _previousEventPowerWarning;
    MTM1M3_logevent_preclippedAccelerationForcesC _previousEventPreclippedAccelerationForces;
    MTM1M3_logevent_preclippedActiveOpticForcesC _previousEventPreclippedActiveOpticForces;
    MTM1M3_logevent_preclippedAzimuthForcesC _previousEventPreclippedAzimuthForces;
    MTM1M3_logevent_preclippedBalanceForcesC _previousEventPreclippedBalanceForces;
    MTM1M3_logevent_preclippedCylinderForcesC _previousEventPreclippedCylinderForces;
    MTM1M3_logevent_preclippedElevationForcesC _previousEventPreclippedElevationForces;
    MTM1M3_logevent_preclippedForcesC _previousEventPreclippedForces;
    MTM1M3_logevent_preclippedOffsetForcesC _previousEventPreclippedOffsetForces;
    MTM1M3_logevent_preclippedStaticForcesC _previousEventPreclippedStaticForces;
    MTM1M3_logevent_preclippedThermalForcesC _previousEventPreclippedThermalForces;
    MTM1M3_logevent_preclippedVelocityForcesC _previousEventPreclippedVelocityForces;
    MTM1M3_logevent_summaryStateC _previousEventSummaryState;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
