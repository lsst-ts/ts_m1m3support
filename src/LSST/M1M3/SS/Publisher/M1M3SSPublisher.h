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
private:
    std::shared_ptr<SAL_MTM1M3> m1m3SAL;

    MTM1M3_accelerometerDataC accelerometerData;
    MTM1M3_forceActuatorDataC forceActuatorData;
    MTM1M3_gyroDataC gyroData;
    MTM1M3_hardpointActuatorDataC hardpointActuatorData;
    MTM1M3_hardpointMonitorDataC hardpointMonitorData;
    MTM1M3_imsDataC imsData;
    MTM1M3_inclinometerDataC inclinometerData;
    MTM1M3_outerLoopDataC outerLoopData;
    MTM1M3_pidDataC pidData;
    MTM1M3_powerSupplyDataC powerSupplyData;

    MTM1M3_logevent_accelerometerWarningC eventAccelerometerWarning;
    MTM1M3_logevent_airSupplyStatusC eventAirSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC eventAirSupplyWarning;
    MTM1M3_logevent_appliedAberrationForcesC eventAppliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC eventAppliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC eventAppliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC eventAppliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC eventAppliedBalanceForces;
    MTM1M3_logevent_appliedCylinderForcesC eventAppliedCylinderForces;
    MTM1M3_logevent_appliedElevationForcesC eventAppliedElevationForces;
    MTM1M3_logevent_appliedForcesC eventAppliedForces;
    MTM1M3_logevent_appliedOffsetForcesC eventAppliedOffsetForces;
    MTM1M3_logevent_appliedSettingsMatchStartC eventAppliedSettingsMatchStart;
    MTM1M3_logevent_appliedStaticForcesC eventAppliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC eventAppliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC eventAppliedVelocityForces;
    MTM1M3_logevent_cellLightStatusC eventCellLightStatus;
    MTM1M3_logevent_cellLightWarningC eventCellLightWarning;
    MTM1M3_logevent_commandRejectionWarningC eventCommandRejectionWarning;
    MTM1M3_logevent_detailedStateC eventDetailedState;
    MTM1M3_logevent_displacementSensorWarningC eventDisplacementSensorWarning;
    MTM1M3_logevent_errorCodeC eventErrorCode;
    MTM1M3_logevent_forceActuatorForceWarningC eventForceActuatorForceWarning;
    MTM1M3_logevent_forceActuatorInfoC eventForceActuatorInfo;
    MTM1M3_logevent_forceActuatorStateC eventForceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC eventForceActuatorWarning;
    MTM1M3_logevent_forceSetpointWarningC eventForceSetpointWarning;
    MTM1M3_logevent_gyroWarningC eventGyroWarning;
    MTM1M3_logevent_hardpointActuatorInfoC eventHardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC eventHardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorWarningC eventHardpointActuatorWarning;
    MTM1M3_logevent_hardpointMonitorInfoC eventHardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC eventHardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC eventHardpointMonitorWarning;
    MTM1M3_logevent_ilcWarningC eventILCWarning;
    MTM1M3_logevent_inclinometerSensorWarningC eventInclinometerSensorWarning;
    MTM1M3_logevent_interlockStatusC eventInterlockStatus;
    MTM1M3_logevent_interlockWarningC eventInterlockWarning;
    MTM1M3_logevent_modbusResponseC eventModbusResponse;
    MTM1M3_logevent_pidInfoC eventPIDInfo;
    MTM1M3_logevent_powerStatusC eventPowerStatus;
    MTM1M3_logevent_powerSupplyStatusC eventPowerSupplyStatus;
    MTM1M3_logevent_powerWarningC eventPowerWarning;
    MTM1M3_logevent_rejectedAberrationForcesC eventRejectedAberrationForces;
    MTM1M3_logevent_rejectedAccelerationForcesC eventRejectedAccelerationForces;
    MTM1M3_logevent_rejectedActiveOpticForcesC eventRejectedActiveOpticForces;
    MTM1M3_logevent_rejectedAzimuthForcesC eventRejectedAzimuthForces;
    MTM1M3_logevent_rejectedBalanceForcesC eventRejectedBalanceForces;
    MTM1M3_logevent_rejectedCylinderForcesC eventRejectedCylinderForces;
    MTM1M3_logevent_rejectedElevationForcesC eventRejectedElevationForces;
    MTM1M3_logevent_rejectedForcesC eventRejectedForces;
    MTM1M3_logevent_rejectedOffsetForcesC eventRejectedOffsetForces;
    MTM1M3_logevent_rejectedStaticForcesC eventRejectedStaticForces;
    MTM1M3_logevent_rejectedThermalForcesC eventRejectedThermalForces;
    MTM1M3_logevent_rejectedVelocityForcesC eventRejectedVelocityForces;
    MTM1M3_logevent_settingVersionsC eventSettingVersions;
    MTM1M3_logevent_settingsAppliedC eventSettingsApplied;
    MTM1M3_logevent_summaryStateC eventSummaryState;

    MTM1M3_logevent_accelerometerWarningC previousEventAccelerometerWarning;
    MTM1M3_logevent_airSupplyStatusC previousEventAirSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC previousEventAirSupplyWarning;
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

public:
    M1M3SSPublisher(std::shared_ptr<SAL_MTM1M3> m1m3SAL);

    MTM1M3_accelerometerDataC* getAccelerometerData() { return &this->accelerometerData; }
    MTM1M3_forceActuatorDataC* getForceActuatorData() { return &this->forceActuatorData; }
    MTM1M3_gyroDataC* getGyroData() { return &this->gyroData; }
    MTM1M3_hardpointActuatorDataC* getHardpointActuatorData() { return &this->hardpointActuatorData; }
    MTM1M3_hardpointMonitorDataC* getHardpointMonitorData() { return &this->hardpointMonitorData; }
    MTM1M3_imsDataC* getIMSData() { return &this->imsData; }
    MTM1M3_inclinometerDataC* getInclinometerData() { return &this->inclinometerData; }
    MTM1M3_outerLoopDataC* getOuterLoopData() { return &this->outerLoopData; }
    MTM1M3_pidDataC* getPIDData() { return &this->pidData; }
    MTM1M3_powerSupplyDataC* getPowerSupplyData() { return &this->powerSupplyData; }

    MTM1M3_logevent_accelerometerWarningC* getEventAccelerometerWarning() {
        return &this->eventAccelerometerWarning;
    }
    MTM1M3_logevent_airSupplyStatusC* getEventAirSupplyStatus() { return &this->eventAirSupplyStatus; }
    MTM1M3_logevent_airSupplyWarningC* getEventAirSupplyWarning() { return &this->eventAirSupplyWarning; }
    MTM1M3_logevent_appliedAberrationForcesC* getEventAppliedAberrationForces() {
        return &this->eventAppliedAberrationForces;
    }
    MTM1M3_logevent_appliedAccelerationForcesC* getEventAppliedAccelerationForces() {
        return &this->eventAppliedAccelerationForces;
    }
    MTM1M3_logevent_appliedActiveOpticForcesC* getEventAppliedActiveOpticForces() {
        return &this->eventAppliedActiveOpticForces;
    }
    MTM1M3_logevent_appliedAzimuthForcesC* getEventAppliedAzimuthForces() {
        return &this->eventAppliedAzimuthForces;
    }
    MTM1M3_logevent_appliedBalanceForcesC* getEventAppliedBalanceForces() {
        return &this->eventAppliedBalanceForces;
    }
    MTM1M3_logevent_appliedCylinderForcesC* getEventAppliedCylinderForces() {
        return &this->eventAppliedCylinderForces;
    }
    MTM1M3_logevent_appliedElevationForcesC* getEventAppliedElevationForces() {
        return &this->eventAppliedElevationForces;
    }
    MTM1M3_logevent_appliedForcesC* getEventAppliedForces() { return &this->eventAppliedForces; }
    MTM1M3_logevent_appliedOffsetForcesC* getEventAppliedOffsetForces() {
        return &this->eventAppliedOffsetForces;
    }
    MTM1M3_logevent_appliedSettingsMatchStartC* getEventAppliedSettingsMatchStart() {
        return &this->eventAppliedSettingsMatchStart;
    }
    MTM1M3_logevent_appliedStaticForcesC* getEventAppliedStaticForces() {
        return &this->eventAppliedStaticForces;
    }
    MTM1M3_logevent_appliedThermalForcesC* getEventAppliedThermalForces() {
        return &this->eventAppliedThermalForces;
    }
    MTM1M3_logevent_appliedVelocityForcesC* getEventAppliedVelocityForces() {
        return &this->eventAppliedVelocityForces;
    }
    MTM1M3_logevent_cellLightStatusC* getEventCellLightStatus() { return &this->eventCellLightStatus; }
    MTM1M3_logevent_cellLightWarningC* getEventCellLightWarning() { return &this->eventCellLightWarning; }
    MTM1M3_logevent_commandRejectionWarningC* getEventCommandRejectionWarning() {
        return &this->eventCommandRejectionWarning;
    }
    MTM1M3_logevent_detailedStateC* getEventDetailedState() { return &this->eventDetailedState; }
    MTM1M3_logevent_displacementSensorWarningC* getEventDisplacementSensorWarning() {
        return &this->eventDisplacementSensorWarning;
    }
    MTM1M3_logevent_errorCodeC* getEventErrorCode() { return &this->eventErrorCode; }
    MTM1M3_logevent_forceActuatorForceWarningC* getEventForceActuatorForceWarning() {
        return &this->eventForceActuatorForceWarning;
    }
    MTM1M3_logevent_forceActuatorInfoC* getEventForceActuatorInfo() { return &this->eventForceActuatorInfo; }
    MTM1M3_logevent_forceActuatorStateC* getEventForceActuatorState() {
        return &this->eventForceActuatorState;
    }
    MTM1M3_logevent_forceActuatorWarningC* getEventForceActuatorWarning() {
        return &this->eventForceActuatorWarning;
    }
    MTM1M3_logevent_forceSetpointWarningC* getEventForceSetpointWarning() {
        return &this->eventForceSetpointWarning;
    }
    MTM1M3_logevent_gyroWarningC* getEventGyroWarning() { return &this->eventGyroWarning; }
    MTM1M3_logevent_hardpointActuatorInfoC* getEventHardpointActuatorInfo() {
        return &this->eventHardpointActuatorInfo;
    }
    MTM1M3_logevent_hardpointActuatorStateC* getEventHardpointActuatorState() {
        return &this->eventHardpointActuatorState;
    }
    MTM1M3_logevent_hardpointActuatorWarningC* getEventHardpointActuatorWarning() {
        return &this->eventHardpointActuatorWarning;
    }
    MTM1M3_logevent_hardpointMonitorInfoC* getEventHardpointMonitorInfo() {
        return &this->eventHardpointMonitorInfo;
    }
    MTM1M3_logevent_hardpointMonitorStateC* getEventHardpointMonitorState() {
        return &this->eventHardpointMonitorState;
    }
    MTM1M3_logevent_hardpointMonitorWarningC* getEventHardpointMonitorWarning() {
        return &this->eventHardpointMonitorWarning;
    }
    MTM1M3_logevent_ilcWarningC* getEventILCWarning() { return &this->eventILCWarning; }
    MTM1M3_logevent_inclinometerSensorWarningC* getEventInclinometerSensorWarning() {
        return &this->eventInclinometerSensorWarning;
    }
    MTM1M3_logevent_interlockStatusC* getEventInterlockStatus() { return &this->eventInterlockStatus; }
    MTM1M3_logevent_interlockWarningC* getEventInterlockWarning() { return &this->eventInterlockWarning; }
    MTM1M3_logevent_modbusResponseC* getEventModbusResponse() { return &this->eventModbusResponse; }
    MTM1M3_logevent_pidInfoC* getEventPIDInfo() { return &this->eventPIDInfo; }
    MTM1M3_logevent_powerStatusC* getEventPowerStatus() { return &this->eventPowerStatus; }
    MTM1M3_logevent_powerSupplyStatusC* getEventPowerSupplyStatus() { return &this->eventPowerSupplyStatus; }
    MTM1M3_logevent_powerWarningC* getEventPowerWarning() { return &this->eventPowerWarning; }
    MTM1M3_logevent_rejectedAberrationForcesC* getEventRejectedAberrationForces() {
        return &this->eventRejectedAberrationForces;
    }
    MTM1M3_logevent_rejectedAccelerationForcesC* getEventRejectedAccelerationForces() {
        return &this->eventRejectedAccelerationForces;
    }
    MTM1M3_logevent_rejectedActiveOpticForcesC* getEventRejectedActiveOpticForces() {
        return &this->eventRejectedActiveOpticForces;
    }
    MTM1M3_logevent_rejectedAzimuthForcesC* getEventRejectedAzimuthForces() {
        return &this->eventRejectedAzimuthForces;
    }
    MTM1M3_logevent_rejectedBalanceForcesC* getEventRejectedBalanceForces() {
        return &this->eventRejectedBalanceForces;
    }
    MTM1M3_logevent_rejectedCylinderForcesC* getEventRejectedCylinderForces() {
        return &this->eventRejectedCylinderForces;
    }
    MTM1M3_logevent_rejectedElevationForcesC* getEventRejectedElevationForces() {
        return &this->eventRejectedElevationForces;
    }
    MTM1M3_logevent_rejectedForcesC* getEventRejectedForces() { return &this->eventRejectedForces; }
    MTM1M3_logevent_rejectedOffsetForcesC* getEventRejectedOffsetForces() {
        return &this->eventRejectedOffsetForces;
    }
    MTM1M3_logevent_rejectedStaticForcesC* getEventRejectedStaticForces() {
        return &this->eventRejectedStaticForces;
    }
    MTM1M3_logevent_rejectedThermalForcesC* getEventRejectedThermalForces() {
        return &this->eventRejectedThermalForces;
    }
    MTM1M3_logevent_rejectedVelocityForcesC* getEventRejectedVelocityForces() {
        return &this->eventRejectedVelocityForces;
    }
    MTM1M3_logevent_settingVersionsC* getEventSettingVersions() { return &this->eventSettingVersions; }
    MTM1M3_logevent_settingsAppliedC* getEventSettingsApplied() { return &this->eventSettingsApplied; }
    MTM1M3_logevent_summaryStateC* getEventSummaryState() { return &this->eventSummaryState; }

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
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
