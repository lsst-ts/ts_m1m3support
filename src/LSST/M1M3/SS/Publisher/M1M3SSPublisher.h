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

#ifndef M1M3SSPUBLISHER_H_
#define M1M3SSPUBLISHER_H_

#include <DataTypes.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTM1M3C.h>
#include <ccpp_sal_MTM1M3.h>

#include <EnabledForceActuators.h>
#include <ForceActuatorWarning.h>

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
class M1M3SSPublisher {
public:
    M1M3SSPublisher();

    /**
     * @brief Retrieve singleton instance.
     *
     * @return singleton instance
     */
    static M1M3SSPublisher& get();

    void setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL);

    /**
     * @brief Returns pointer to accelerometer data.
     *
     * @return pointer to the accelerometer data
     *
     * @see putAccelerometerData
     */
    MTM1M3_accelerometerDataC* getAccelerometerData() { return &_accelerometerData; }
    MTM1M3_forceActuatorDataC* getForceActuatorData() { return &_forceActuatorData; }
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
    MTM1M3_logevent_airSupplyStatusC* getEventAirSupplyStatus() { return &_eventAirSupplyStatus; }
    MTM1M3_logevent_airSupplyWarningC* getEventAirSupplyWarning() { return &_eventAirSupplyWarning; }
    MTM1M3_logevent_appliedAberrationForcesC* getEventAppliedAberrationForces() {
        return &_eventAppliedAberrationForces;
    }
    MTM1M3_logevent_appliedAccelerationForcesC* getEventAppliedAccelerationForces() {
        return &_eventAppliedAccelerationForces;
    }
    MTM1M3_logevent_appliedActiveOpticForcesC* getEventAppliedActiveOpticForces() {
        return &_eventAppliedActiveOpticForces;
    }
    MTM1M3_logevent_appliedAzimuthForcesC* getEventAppliedAzimuthForces() {
        return &_eventAppliedAzimuthForces;
    }
    MTM1M3_logevent_appliedBalanceForcesC* getEventAppliedBalanceForces() {
        return &_eventAppliedBalanceForces;
    }
    MTM1M3_logevent_appliedCylinderForcesC* getEventAppliedCylinderForces() {
        return &_eventAppliedCylinderForces;
    }
    MTM1M3_logevent_appliedElevationForcesC* getEventAppliedElevationForces() {
        return &this->eventAppliedElevationForces;
    }
    MTM1M3_logevent_appliedForcesC* getEventAppliedForces() { return &_eventAppliedForces; }
    MTM1M3_logevent_appliedOffsetForcesC* getEventAppliedOffsetForces() { return &_eventAppliedOffsetForces; }
    MTM1M3_logevent_appliedSettingsMatchStartC* getEventAppliedSettingsMatchStart() {
        return &_eventAppliedSettingsMatchStart;
    }
    MTM1M3_logevent_appliedStaticForcesC* getEventAppliedStaticForces() {
        return &this->eventAppliedStaticForces;
    }
    MTM1M3_logevent_appliedThermalForcesC* getEventAppliedThermalForces() {
        return &_eventAppliedThermalForces;
    }
    MTM1M3_logevent_appliedVelocityForcesC* getEventAppliedVelocityForces() {
        return &_eventAppliedVelocityForces;
    }
    MTM1M3_logevent_cellLightStatusC* getEventCellLightStatus() { return &_eventCellLightStatus; }
    MTM1M3_logevent_cellLightWarningC* getEventCellLightWarning() { return &_eventCellLightWarning; }
    MTM1M3_logevent_commandRejectionWarningC* getEventCommandRejectionWarning() {
        return &_eventCommandRejectionWarning;
    }
    MTM1M3_logevent_detailedStateC* getEventDetailedState() { return &_eventDetailedState; }
    MTM1M3_logevent_displacementSensorWarningC* getEventDisplacementSensorWarning() {
        return &_eventDisplacementSensorWarning;
    }
    static EnabledForceActuators* getEnabledForceActuators() { return &(get()._enabledForceActuators); }
    MTM1M3_logevent_errorCodeC* getEventErrorCode() { return &_eventErrorCode; }
    MTM1M3_logevent_forceActuatorBumpTestStatusC* getEventForceActuatorBumpTestStatus() {
        return &_eventForceActuatorBumpTestStatus;
    }
    MTM1M3_logevent_forceActuatorForceWarningC* getEventForceActuatorForceWarning() {
        return &_eventForceActuatorForceWarning;
    }
    MTM1M3_logevent_forceActuatorInfoC* getEventForceActuatorInfo() { return &_eventForceActuatorInfo; }
    MTM1M3_logevent_forceActuatorStateC* getEventForceActuatorState() { return &_eventForceActuatorState; }
    static ForceActuatorWarning* getForceActuatorWarning() { return &(get()._forceActuatorWarning); }
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
    MTM1M3_logevent_hardpointActuatorWarningC* getEventHardpointActuatorWarning() {
        return &_eventHardpointActuatorWarning;
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
    MTM1M3_logevent_interlockStatusC* getEventInterlockStatus() { return &_eventInterlockStatus; }
    MTM1M3_logevent_interlockWarningC* getEventInterlockWarning() { return &_eventInterlockWarning; }
    MTM1M3_logevent_modbusResponseC* getEventModbusResponse() { return &_eventModbusResponse; }
    MTM1M3_logevent_pidInfoC* getEventPIDInfo() { return &_eventPIDInfo; }
    MTM1M3_logevent_powerStatusC* getEventPowerStatus() { return &_eventPowerStatus; }
    MTM1M3_logevent_powerSupplyStatusC* getEventPowerSupplyStatus() { return &_eventPowerSupplyStatus; }
    MTM1M3_logevent_powerWarningC* getEventPowerWarning() { return &_eventPowerWarning; }
    MTM1M3_logevent_preclippedAberrationForcesC* getEventPreclippedAberrationForces() {
        return &_eventPreclippedAberrationForces;
    }
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
    MTM1M3_logevent_settingVersionsC* getEventSettingVersions() { return &_eventSettingVersions; }
    MTM1M3_logevent_settingsAppliedC* getEventSettingsApplied() { return &_eventSettingsApplied; }
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
    void putForceActuatorData();
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
    void logAirSupplyStatus();
    void tryLogAirSupplyStatus();
    void logAirSupplyWarning();
    void tryLogAirSupplyWarning();
    void logAppliedAberrationForces();
    void tryLogAppliedAberrationForces();
    void logAppliedAccelerationForces();
    void tryLogAppliedAccelerationForces();
    void logAppliedActiveOpticForces();
    void tryLogAppliedActiveOpticForces();
    void logAppliedAzimuthForces();
    void tryLogAppliedAzimuthForces();
    void logAppliedBalanceForces();
    void tryLogAppliedBalanceForces();
    void logAppliedCylinderForces();
    void tryLogAppliedCylinderForces();
    void logAppliedElevationForces();
    void tryLogAppliedElevationForces();
    void logAppliedForces();
    void tryLogAppliedForces();
    void logAppliedOffsetForces();
    void tryLogAppliedOffsetForces();
    void logAppliedSettingsMatchStart();
    void tryLogAppliedSettingsMatchStart();
    void logAppliedStaticForces();
    void tryLogAppliedStaticForces();
    void logAppliedThermalForces();
    void tryLogAppliedThermalForces();
    void logAppliedVelocityForces();
    void tryLogAppliedVelocityForces();
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
    void logForceActuatorForceWarning();
    void tryLogForceActuatorForceWarning();
    void logForceActuatorInfo();
    void tryLogForceActuatorInfo();
    void logForceActuatorState();
    void tryLogForceActuatorState();
    void logForceActuatorWarning(MTM1M3_logevent_forceActuatorWarningC* data) {
        _m1m3SAL->logEvent_forceActuatorWarning(data, 0);
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
    void logHardpointActuatorWarning();
    void tryLogHardpointActuatorWarning();
    void logHardpointMonitorInfo();
    void tryLogHardpointMonitorInfo();
    void logHardpointMonitorState();
    void tryLogHardpointMonitorState();
    void logHardpointMonitorWarning();
    void tryLogHardpointMonitorWarning();
    void logHeartbeat();
    void logILCWarning();
    void tryLogILCWarning();
    void logInclinometerSettings(MTM1M3_logevent_inclinometerSettingsC* data) {
        _m1m3SAL->logEvent_inclinometerSettings(data, 0);
    }
    void logInclinometerSensorWarning();
    void tryLogInclinometerSensorWarning();
    void logInterlockStatus();
    void tryLogInterlockStatus();
    void logInterlockWarning();
    void tryLogInterlockWarning();
    void newLogLevel(int newLevel);
    void logModbusResponse();
    void tryLogModbusResponse();
    void logPIDInfo();
    void tryLogPIDInfo();
    void logPIDSettings(MTM1M3_logevent_pidSettingsC* data) { _m1m3SAL->logEvent_pidSettings(data, 0); }
    void logPowerStatus();
    void tryLogPowerStatus();
    void logPowerSupplyStatus();
    void tryLogPowerSupplyStatus();
    void logPowerWarning();
    void tryLogPowerWarning();
    void logPreclippedAberrationForces();
    void tryLogPreclippedAberrationForces();
    void logPreclippedAccelerationForces();
    void tryLogPreclippedAccelerationForces();
    void logPreclippedActiveOpticForces();
    void tryLogPreclippedActiveOpticForces();
    void logPreclippedAzimuthForces();
    void tryLogPreclippedAzimuthForces();
    void logPreclippedBalanceForces();
    void tryLogPreclippedBalanceForces();
    void logPreclippedCylinderForces();
    void tryLogPreclippedCylinderForces();
    void logPreclippedElevationForces();
    void tryLogPreclippedElevationForces();
    void logPreclippedForces();
    void tryLogPreclippedForces();
    void logPreclippedOffsetForces();
    void tryLogPreclippedOffsetForces();
    void logPreclippedStaticForces();
    void tryLogPreclippedStaticForces();
    void logPreclippedThermalForces();
    void tryLogPreclippedThermalForces();
    void logPreclippedVelocityForces();
    void tryLogPreclippedVelocityForces();
    void logSettingVersions();
    void logSettingsApplied();
    void logSoftwareVersions();
    void logSummaryState();
    void tryLogSummaryState();

    void ackCommandsetLogLevel(int32_t commandID, int32_t ackCode, std::string description);

    /**
     * @brief Acknowledges start command.
     *
     * @param commandID
     * @param ackCode acknowledgement code. Either ACK_INPROGRESS, ACK_FAILED or ACK_COMPLETE
     * @param description string collected during command execution
     */
    void ackCommandstart(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenable(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommanddisable(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandstandby(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandexitControl(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnAirOn(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnAirOff(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandclearOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandraiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandlowerM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandclearActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyAberrationForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandclearAberrationForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenterEngineering(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandexitEngineering(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandsetAirSlewFlag(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandtestHardpoint(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandmoveHardpointActuators(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommanddisableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandabortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandtranslateM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandstopHardpointMotion(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandpositionM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnLightsOn(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnLightsOff(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnPowerOn(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnPowerOff(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenableHardpointCorrections(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommanddisableHardpointCorrections(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandrunMirrorForceProfile(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandabortProfile(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyOffsetForcesByMirrorForce(int32_t commandID, int32_t ackCode,
                                                  std::string description);
    void ackCommandupdatePID(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandresetPID(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandprogramILC(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandmodbusTransmit(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandforceActuatorBumpTest(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandkillForceActuatorBumpTest(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommanddisableForceActuator(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenableForceActuator(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenableAllForceActuators(int32_t commandID, int32_t ackCode, std::string description);

private:
    M1M3SSPublisher& operator=(const M1M3SSPublisher&) = delete;
    M1M3SSPublisher(const M1M3SSPublisher&) = delete;

    std::shared_ptr<SAL_MTM1M3> _m1m3SAL;

    MTM1M3_accelerometerDataC _accelerometerData;
    MTM1M3_forceActuatorDataC _forceActuatorData;
    MTM1M3_gyroDataC _gyroData;
    MTM1M3_hardpointActuatorDataC _hardpointActuatorData;
    MTM1M3_hardpointMonitorDataC _hardpointMonitorData;
    MTM1M3_imsDataC _imsData;
    MTM1M3_inclinometerDataC _inclinometerData;
    MTM1M3_outerLoopDataC _outerLoopData;
    MTM1M3_pidDataC _pidData;
    MTM1M3_powerSupplyDataC _powerSupplyData;

    MTM1M3_logevent_accelerometerWarningC _eventAccelerometerWarning;
    MTM1M3_logevent_airSupplyStatusC _eventAirSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC _eventAirSupplyWarning;
    MTM1M3_logevent_appliedAberrationForcesC _eventAppliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC _eventAppliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC _eventAppliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC _eventAppliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC _eventAppliedBalanceForces;
    MTM1M3_logevent_appliedCylinderForcesC _eventAppliedCylinderForces;
    MTM1M3_logevent_appliedElevationForcesC eventAppliedElevationForces;
    MTM1M3_logevent_appliedForcesC _eventAppliedForces;
    MTM1M3_logevent_appliedOffsetForcesC _eventAppliedOffsetForces;
    MTM1M3_logevent_appliedSettingsMatchStartC _eventAppliedSettingsMatchStart;
    MTM1M3_logevent_appliedStaticForcesC eventAppliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC _eventAppliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC _eventAppliedVelocityForces;
    MTM1M3_logevent_cellLightStatusC _eventCellLightStatus;
    MTM1M3_logevent_cellLightWarningC _eventCellLightWarning;
    MTM1M3_logevent_commandRejectionWarningC _eventCommandRejectionWarning;
    MTM1M3_logevent_detailedStateC _eventDetailedState;
    MTM1M3_logevent_displacementSensorWarningC _eventDisplacementSensorWarning;
    EnabledForceActuators _enabledForceActuators;
    MTM1M3_logevent_errorCodeC _eventErrorCode;
    MTM1M3_logevent_forceActuatorBumpTestStatusC _eventForceActuatorBumpTestStatus;
    MTM1M3_logevent_forceActuatorForceWarningC _eventForceActuatorForceWarning;
    MTM1M3_logevent_forceActuatorInfoC _eventForceActuatorInfo;
    MTM1M3_logevent_forceActuatorStateC _eventForceActuatorState;
    ForceActuatorWarning _forceActuatorWarning;
    MTM1M3_logevent_forceSetpointWarningC _eventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC _eventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC _eventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC _eventHardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorWarningC _eventHardpointActuatorWarning;
    MTM1M3_logevent_hardpointMonitorInfoC _eventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC _eventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC _eventHardpointMonitorWarning;
    MTM1M3_logevent_heartbeatC _eventHeartbeat;
    MTM1M3_logevent_ilcWarningC _eventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC _eventInclinometerSensorWarning;
    MTM1M3_logevent_interlockStatusC _eventInterlockStatus;
    MTM1M3_logevent_interlockWarningC _eventInterlockWarning;
    MTM1M3_logevent_modbusResponseC _eventModbusResponse;
    MTM1M3_logevent_pidInfoC _eventPIDInfo;
    MTM1M3_logevent_powerStatusC _eventPowerStatus;
    MTM1M3_logevent_powerSupplyStatusC _eventPowerSupplyStatus;
    MTM1M3_logevent_powerWarningC _eventPowerWarning;
    MTM1M3_logevent_preclippedAberrationForcesC _eventPreclippedAberrationForces;
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
    MTM1M3_logevent_settingVersionsC _eventSettingVersions;
    MTM1M3_logevent_settingsAppliedC _eventSettingsApplied;
    MTM1M3_logevent_summaryStateC _eventSummaryState;
    MTM1M3_logevent_accelerometerWarningC _previousEventAccelerometerWarning;
    MTM1M3_logevent_airSupplyStatusC _previousEventAirSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC _previousEventAirSupplyWarning;
    MTM1M3_logevent_appliedAberrationForcesC _previousEventAppliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC _previousEventAppliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC _previousEventAppliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC _previousEventAppliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC _previousEventAppliedBalanceForces;
    MTM1M3_logevent_appliedCylinderForcesC _previousEventAppliedCylinderForces;
    MTM1M3_logevent_appliedElevationForcesC _previousEventAppliedElevationForces;
    MTM1M3_logevent_appliedForcesC _previousEventAppliedForces;
    MTM1M3_logevent_appliedOffsetForcesC _previousEventAppliedOffsetForces;
    MTM1M3_logevent_appliedSettingsMatchStartC _previousEventAppliedSettingsMatchStart;
    MTM1M3_logevent_appliedStaticForcesC _previousEventAppliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC _previousEventAppliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC _previousEventAppliedVelocityForces;
    MTM1M3_logevent_cellLightStatusC _previousEventCellLightStatus;
    MTM1M3_logevent_cellLightWarningC _previousEventCellLightWarning;
    MTM1M3_logevent_commandRejectionWarningC _previousEventCommandRejectionWarning;
    MTM1M3_logevent_detailedStateC _previousEventDetailedState;
    MTM1M3_logevent_displacementSensorWarningC _previousEventDisplacementSensorWarning;
    MTM1M3_logevent_errorCodeC _previousEventErrorCode;
    MTM1M3_logevent_forceActuatorBumpTestStatusC _previousEventForceActuatorBumpTestStatus;
    MTM1M3_logevent_forceActuatorForceWarningC _previousEventForceActuatorForceWarning;
    MTM1M3_logevent_forceActuatorInfoC _previousEventForceActuatorInfo;
    MTM1M3_logevent_forceActuatorStateC _previousEventForceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC _previousEventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC _previousEventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC _previousEventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC _previousEventHardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorWarningC _previousEventHardpointActuatorWarning;
    MTM1M3_logevent_hardpointMonitorInfoC _previousEventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC _previousEventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC _previousEventHardpointMonitorWarning;
    MTM1M3_logevent_ilcWarningC _previousEventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC _previousEventInclinometerSensorWarning;
    MTM1M3_logevent_interlockStatusC _previousEventInterlockStatus;
    MTM1M3_logevent_interlockWarningC _previousEventInterlockWarning;
    MTM1M3_logevent_modbusResponseC _previousEventModbusResponse;
    MTM1M3_logevent_pidInfoC _previousEventPIDInfo;
    MTM1M3_logevent_powerStatusC _previousEventPowerStatus;
    MTM1M3_logevent_powerSupplyStatusC _previousEventPowerSupplyStatus;
    MTM1M3_logevent_powerWarningC _previousEventPowerWarning;
    MTM1M3_logevent_preclippedAberrationForcesC _previousEventPreclippedAberrationForces;
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
