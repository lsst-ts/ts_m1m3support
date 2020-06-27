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
#include <SAL_MTM1M3C.h>
#include <memory>

class SAL_MTM1M3;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher {
public:
    M1M3SSPublisher(std::shared_ptr<SAL_MTM1M3> m1m3SAL);

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
    MTM1M3_logevent_errorCodeC* getEventErrorCode() { return &_eventErrorCode; }
    MTM1M3_logevent_forceActuatorForceWarningC* getEventForceActuatorForceWarning() {
        return &_eventForceActuatorForceWarning;
    }
    MTM1M3_logevent_forceActuatorInfoC* getEventForceActuatorInfo() { return &_eventForceActuatorInfo; }
    MTM1M3_logevent_forceActuatorStateC* getEventForceActuatorState() { return &_eventForceActuatorState; }
    MTM1M3_logevent_forceActuatorWarningC* getEventForceActuatorWarning() {
        return &_eventForceActuatorWarning;
    }
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
    MTM1M3_logevent_rejectedAberrationForcesC* getEventRejectedAberrationForces() {
        return &_eventRejectedAberrationForces;
    }
    MTM1M3_logevent_rejectedAccelerationForcesC* getEventRejectedAccelerationForces() {
        return &_eventRejectedAccelerationForces;
    }
    MTM1M3_logevent_rejectedActiveOpticForcesC* getEventRejectedActiveOpticForces() {
        return &_eventRejectedActiveOpticForces;
    }
    MTM1M3_logevent_rejectedAzimuthForcesC* getEventRejectedAzimuthForces() {
        return &_eventRejectedAzimuthForces;
    }
    MTM1M3_logevent_rejectedBalanceForcesC* getEventRejectedBalanceForces() {
        return &_eventRejectedBalanceForces;
    }
    MTM1M3_logevent_rejectedCylinderForcesC* getEventRejectedCylinderForces() {
        return &_eventRejectedCylinderForces;
    }
    MTM1M3_logevent_rejectedElevationForcesC* getEventRejectedElevationForces() {
        return &_eventRejectedElevationForces;
    }
    MTM1M3_logevent_rejectedForcesC* getEventRejectedForces() { return &_eventRejectedForces; }
    MTM1M3_logevent_rejectedOffsetForcesC* getEventRejectedOffsetForces() {
        return &_eventRejectedOffsetForces;
    }
    MTM1M3_logevent_rejectedStaticForcesC* getEventRejectedStaticForces() {
        return &_eventRejectedStaticForces;
    }
    MTM1M3_logevent_rejectedThermalForcesC* getEventRejectedThermalForces() {
        return &_eventRejectedThermalForces;
    }
    MTM1M3_logevent_rejectedVelocityForcesC* getEventRejectedVelocityForces() {
        return &_eventRejectedVelocityForces;
    }
    MTM1M3_logevent_settingVersionsC* getEventSettingVersions() { return &_eventSettingVersions; }
    MTM1M3_logevent_settingsAppliedC* getEventSettingsApplied() { return &_eventSettingsApplied; }
    MTM1M3_logevent_summaryStateC* getEventSummaryState() { return &_eventSummaryState; }

    double getTimestamp();

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

    void logAccelerometerWarning();
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
    void tryLogCommandRejectionWarning();
    void logCommandRejectionWarning(std::string command, std::string reason);
    void tryLogCommandRejectionWarning(std::string command, std::string reason);
    void logDetailedState();
    void tryLogDetailedState();
    void logDisplacementSensorWarning();
    void tryLogDisplacementSensorWarning();
    void logErrorCode();
    void tryLogErrorCode();
    void logForceActuatorForceWarning();
    void tryLogForceActuatorForceWarning();
    void logForceActuatorInfo();
    void tryLogForceActuatorInfo();
    void logForceActuatorState();
    void tryLogForceActuatorState();
    void logForceActuatorWarning();
    void tryLogForceActuatorWarning();
    void logForceSetpointWarning();
    void tryLogForceSetpointWarning();
    void logGyroWarning();
    void tryLogGyroWarning();
    void logHardpointActuatorInfo();
    void tryLogHardpointActuatorInfo();
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
    void logILCWarning();
    void tryLogILCWarning();
    void logInclinometerSensorWarning();
    void tryLogInclinometerSensorWarning();
    void logInterlockStatus();
    void tryLogInterlockStatus();
    void logInterlockWarning();
    void tryLogInterlockWarning();
    void logModbusResponse();
    void tryLogModbusResponse();
    void logPIDInfo();
    void tryLogPIDInfo();
    void logPowerStatus();
    void tryLogPowerStatus();
    void logPowerSupplyStatus();
    void tryLogPowerSupplyStatus();
    void logPowerWarning();
    void tryLogPowerWarning();
    void logRejectedAberrationForces();
    void tryLogRejectedAberrationForces();
    void logRejectedAccelerationForces();
    void tryLogRejectedAccelerationForces();
    void logRejectedActiveOpticForces();
    void tryLogRejectedActiveOpticForces();
    void logRejectedAzimuthForces();
    void tryLogRejectedAzimuthForces();
    void logRejectedBalanceForces();
    void tryLogRejectedBalanceForces();
    void logRejectedCylinderForces();
    void tryLogRejectedCylinderForces();
    void logRejectedElevationForces();
    void tryLogRejectedElevationForces();
    void logRejectedForces();
    void tryLogRejectedForces();
    void logRejectedOffsetForces();
    void tryLogRejectedOffsetForces();
    void logRejectedStaticForces();
    void tryLogRejectedStaticForces();
    void logRejectedThermalForces();
    void tryLogRejectedThermalForces();
    void logRejectedVelocityForces();
    void tryLogRejectedVelocityForces();
    void logSettingVersions();
    void logSettingsApplied();
    void tryLogSettingsApplied();
    void logSummaryState();
    void tryLogSummaryState();

    void ackCommandstart(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenable(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommanddisable(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandstandby(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandshutdown(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnAirOn(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandturnAirOff(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandclearOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandraiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandlowerM1M3(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyActiveOpticForcesByBendingModes(int32_t commandID, int32_t ackCode,
                                                        std::string description);
    void ackCommandapplyActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandclearActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandapplyAberrationForcesByBendingModes(int32_t commandID, int32_t ackCode,
                                                       std::string description);
    void ackCommandapplyAberrationForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandclearAberrationForces(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandenterEngineering(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandexitEngineering(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandtestAir(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandtestHardpoint(int32_t commandID, int32_t ackCode, std::string description);
    void ackCommandtestForceActuator(int32_t commandID, int32_t ackCode, std::string description);
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

private:
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
    MTM1M3_logevent_errorCodeC _eventErrorCode;
    MTM1M3_logevent_forceActuatorForceWarningC _eventForceActuatorForceWarning;
    MTM1M3_logevent_forceActuatorInfoC _eventForceActuatorInfo;
    MTM1M3_logevent_forceActuatorStateC _eventForceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC _eventForceActuatorWarning;
    MTM1M3_logevent_forceSetpointWarningC _eventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC _eventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC _eventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC _eventHardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorWarningC _eventHardpointActuatorWarning;
    MTM1M3_logevent_hardpointMonitorInfoC _eventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC _eventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC _eventHardpointMonitorWarning;
    MTM1M3_logevent_ilcWarningC _eventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC _eventInclinometerSensorWarning;
    MTM1M3_logevent_interlockStatusC _eventInterlockStatus;
    MTM1M3_logevent_interlockWarningC _eventInterlockWarning;
    MTM1M3_logevent_modbusResponseC _eventModbusResponse;
    MTM1M3_logevent_pidInfoC _eventPIDInfo;
    MTM1M3_logevent_powerStatusC _eventPowerStatus;
    MTM1M3_logevent_powerSupplyStatusC _eventPowerSupplyStatus;
    MTM1M3_logevent_powerWarningC _eventPowerWarning;
    MTM1M3_logevent_rejectedAberrationForcesC _eventRejectedAberrationForces;
    MTM1M3_logevent_rejectedAccelerationForcesC _eventRejectedAccelerationForces;
    MTM1M3_logevent_rejectedActiveOpticForcesC _eventRejectedActiveOpticForces;
    MTM1M3_logevent_rejectedAzimuthForcesC _eventRejectedAzimuthForces;
    MTM1M3_logevent_rejectedBalanceForcesC _eventRejectedBalanceForces;
    MTM1M3_logevent_rejectedCylinderForcesC _eventRejectedCylinderForces;
    MTM1M3_logevent_rejectedElevationForcesC _eventRejectedElevationForces;
    MTM1M3_logevent_rejectedForcesC _eventRejectedForces;
    MTM1M3_logevent_rejectedOffsetForcesC _eventRejectedOffsetForces;
    MTM1M3_logevent_rejectedStaticForcesC _eventRejectedStaticForces;
    MTM1M3_logevent_rejectedThermalForcesC _eventRejectedThermalForces;
    MTM1M3_logevent_rejectedVelocityForcesC _eventRejectedVelocityForces;
    MTM1M3_logevent_settingVersionsC _eventSettingVersions;
    MTM1M3_logevent_settingsAppliedC _eventSettingsApplied;
    MTM1M3_logevent_summaryStateC _eventSummaryState;
    MTM1M3_logevent_accelerometerWarningC _previousEventAccelerometerWarning;
    MTM1M3_logevent_airSupplyStatusC _previousEventAirSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC _previousEventAirSupplyWarning;
    MTM1M3_logevent_appliedAberrationForcesC previousEventAppliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC previousEventAppliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC previousEventAppliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC previousEventAppliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC previousEventAppliedBalanceForces;
    MTM1M3_logevent_appliedCylinderForcesC previousEventAppliedCylinderForces;
    MTM1M3_logevent_appliedElevationForcesC previousEventAppliedElevationForces;
    MTM1M3_logevent_appliedForcesC previousEventAppliedForces;
    MTM1M3_logevent_appliedOffsetForcesC previousEventAppliedOffsetForces;
    MTM1M3_logevent_appliedSettingsMatchStartC previousEventAppliedSettingsMatchStart;
    MTM1M3_logevent_appliedStaticForcesC previousEventAppliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC previousEventAppliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC previousEventAppliedVelocityForces;
    MTM1M3_logevent_cellLightStatusC previousEventCellLightStatus;
    MTM1M3_logevent_cellLightWarningC previousEventCellLightWarning;
    MTM1M3_logevent_commandRejectionWarningC previousEventCommandRejectionWarning;
    MTM1M3_logevent_detailedStateC previousEventDetailedState;
    MTM1M3_logevent_displacementSensorWarningC previousEventDisplacementSensorWarning;
    MTM1M3_logevent_errorCodeC previousEventErrorCode;
    MTM1M3_logevent_forceActuatorForceWarningC previousEventForceActuatorForceWarning;
    MTM1M3_logevent_forceActuatorInfoC previousEventForceActuatorInfo;
    MTM1M3_logevent_forceActuatorStateC previousEventForceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC previousEventForceActuatorWarning;
    MTM1M3_logevent_forceSetpointWarningC previousEventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC previousEventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC previousEventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC previousEventHardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorWarningC previousEventHardpointActuatorWarning;
    MTM1M3_logevent_hardpointMonitorInfoC previousEventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC previousEventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC previousEventHardpointMonitorWarning;
    MTM1M3_logevent_ilcWarningC previousEventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC previousEventInclinometerSensorWarning;
    MTM1M3_logevent_interlockStatusC previousEventInterlockStatus;
    MTM1M3_logevent_interlockWarningC previousEventInterlockWarning;
    MTM1M3_logevent_modbusResponseC previousEventModbusResponse;
    MTM1M3_logevent_pidInfoC previousEventPIDInfo;
    MTM1M3_logevent_powerStatusC previousEventPowerStatus;
    MTM1M3_logevent_powerSupplyStatusC previousEventPowerSupplyStatus;
    MTM1M3_logevent_powerWarningC previousEventPowerWarning;
    MTM1M3_logevent_rejectedAberrationForcesC previousEventRejectedAberrationForces;
    MTM1M3_logevent_rejectedAccelerationForcesC previousEventRejectedAccelerationForces;
    MTM1M3_logevent_rejectedActiveOpticForcesC previousEventRejectedActiveOpticForces;
    MTM1M3_logevent_rejectedAzimuthForcesC previousEventRejectedAzimuthForces;
    MTM1M3_logevent_rejectedBalanceForcesC previousEventRejectedBalanceForces;
    MTM1M3_logevent_rejectedCylinderForcesC previousEventRejectedCylinderForces;
    MTM1M3_logevent_rejectedElevationForcesC previousEventRejectedElevationForces;
    MTM1M3_logevent_rejectedForcesC previousEventRejectedForces;
    MTM1M3_logevent_rejectedOffsetForcesC previousEventRejectedOffsetForces;
    MTM1M3_logevent_rejectedStaticForcesC previousEventRejectedStaticForces;
    MTM1M3_logevent_rejectedThermalForcesC previousEventRejectedThermalForces;
    MTM1M3_logevent_rejectedVelocityForcesC previousEventRejectedVelocityForces;
    MTM1M3_logevent_settingsAppliedC previousEventSettingsApplied;
    MTM1M3_logevent_summaryStateC previousEventSummaryState;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
