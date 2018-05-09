/*
 * M1M3SSPublisher.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSPUBLISHER_H_
#define M1M3SSPUBLISHER_H_

#include <DataTypes.h>
#include <SAL_m1m3C.h>

class SAL_m1m3;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher {
private:
	SAL_m1m3* m1m3SAL;

	m1m3_AccelerometerDataC accelerometerData;
	m1m3_ForceActuatorDataC forceActuatorData;
	m1m3_GyroDataC gyroData;
	m1m3_HardpointActuatorDataC hardpointActuatorData;
	m1m3_HardpointMonitorDataC hardpointMonitorData;
	m1m3_IMSDataC imsData;
	m1m3_InclinometerDataC inclinometerData;
	m1m3_OuterLoopDataC outerLoopData;
	m1m3_PIDDataC pidData;
	m1m3_PowerSupplyDataC powerSupplyData;

	m1m3_logevent_AccelerometerWarningC eventAccelerometerWarning;
	m1m3_logevent_AirSupplyStatusC eventAirSupplyStatus;
	m1m3_logevent_AirSupplyWarningC eventAirSupplyWarning;
	m1m3_logevent_AppliedAberrationForcesC eventAppliedAberrationForces;
	m1m3_logevent_AppliedAccelerationForcesC eventAppliedAccelerationForces;
	m1m3_logevent_AppliedActiveOpticForcesC eventAppliedActiveOpticForces;
	m1m3_logevent_AppliedAzimuthForcesC eventAppliedAzimuthForces;
	m1m3_logevent_AppliedBalanceForcesC eventAppliedBalanceForces;
	m1m3_logevent_AppliedCylinderForcesC eventAppliedCylinderForces;
	m1m3_logevent_AppliedElevationForcesC eventAppliedElevationForces;
	m1m3_logevent_AppliedForcesC eventAppliedForces;
	m1m3_logevent_AppliedOffsetForcesC eventAppliedOffsetForces;
	m1m3_logevent_AppliedSettingsMatchStartC eventAppliedSettingsMatchStart;
	m1m3_logevent_AppliedStaticForcesC eventAppliedStaticForces;
	m1m3_logevent_AppliedThermalForcesC eventAppliedThermalForces;
	m1m3_logevent_AppliedVelocityForcesC eventAppliedVelocityForces;
	m1m3_logevent_CellLightStatusC eventCellLightStatus;
	m1m3_logevent_CellLightWarningC eventCellLightWarning;
	m1m3_logevent_CommandRejectionWarningC eventCommandRejectionWarning;
	m1m3_logevent_DetailedStateC eventDetailedState;
	m1m3_logevent_DisplacementSensorWarningC eventDisplacementSensorWarning;
	m1m3_logevent_ErrorCodeC eventErrorCode;
	m1m3_logevent_ForceActuatorForceWarningC eventForceActuatorForceWarning;
	m1m3_logevent_ForceActuatorInfoC eventForceActuatorInfo;
	m1m3_logevent_ForceActuatorStateC eventForceActuatorState;
	m1m3_logevent_ForceActuatorWarningC eventForceActuatorWarning;
	m1m3_logevent_ForceSetpointWarningC eventForceSetpointWarning;
	m1m3_logevent_GyroWarningC eventGyroWarning;
	m1m3_logevent_HardpointActuatorInfoC eventHardpointActuatorInfo;
	m1m3_logevent_HardpointActuatorStateC eventHardpointActuatorState;
	m1m3_logevent_HardpointActuatorWarningC eventHardpointActuatorWarning;
	m1m3_logevent_HardpointMonitorInfoC eventHardpointMonitorInfo;
	m1m3_logevent_HardpointMonitorStateC eventHardpointMonitorState;
	m1m3_logevent_HardpointMonitorWarningC eventHardpointMonitorWarning;
	m1m3_logevent_ILCWarningC eventILCWarning;
	m1m3_logevent_InclinometerSensorWarningC eventInclinometerSensorWarning;
	m1m3_logevent_InterlockStatusC eventInterlockStatus;
	m1m3_logevent_InterlockWarningC eventInterlockWarning;
	m1m3_logevent_PIDInfoC eventPIDInfo;
	m1m3_logevent_PowerStatusC eventPowerStatus;
	m1m3_logevent_PowerSupplyStatusC eventPowerSupplyStatus;
	m1m3_logevent_PowerWarningC eventPowerWarning;
	m1m3_logevent_RejectedAberrationForcesC eventRejectedAberrationForces;
	m1m3_logevent_RejectedAccelerationForcesC eventRejectedAccelerationForces;
	m1m3_logevent_RejectedActiveOpticForcesC eventRejectedActiveOpticForces;
	m1m3_logevent_RejectedAzimuthForcesC eventRejectedAzimuthForces;
	m1m3_logevent_RejectedBalanceForcesC eventRejectedBalanceForces;
	m1m3_logevent_RejectedCylinderForcesC eventRejectedCylinderForces;
	m1m3_logevent_RejectedElevationForcesC eventRejectedElevationForces;
	m1m3_logevent_RejectedForcesC eventRejectedForces;
	m1m3_logevent_RejectedOffsetForcesC eventRejectedOffsetForces;
	m1m3_logevent_RejectedStaticForcesC eventRejectedStaticForces;
	m1m3_logevent_RejectedThermalForcesC eventRejectedThermalForces;
	m1m3_logevent_RejectedVelocityForcesC eventRejectedVelocityForces;
	m1m3_logevent_SettingVersionsC eventSettingVersions;
	m1m3_logevent_SettingsAppliedC eventSettingsApplied;
	m1m3_logevent_SummaryStateC eventSummaryState;

	m1m3_logevent_AccelerometerWarningC previousEventAccelerometerWarning;
	m1m3_logevent_AirSupplyStatusC previousEventAirSupplyStatus;
	m1m3_logevent_AirSupplyWarningC previousEventAirSupplyWarning;
	m1m3_logevent_AppliedAberrationForcesC previousEventAppliedAberrationForces;
	m1m3_logevent_AppliedAccelerationForcesC previousEventAppliedAccelerationForces;
	m1m3_logevent_AppliedActiveOpticForcesC previousEventAppliedActiveOpticForces;
	m1m3_logevent_AppliedAzimuthForcesC previousEventAppliedAzimuthForces;
	m1m3_logevent_AppliedBalanceForcesC previousEventAppliedBalanceForces;
	m1m3_logevent_AppliedCylinderForcesC previousEventAppliedCylinderForces;
	m1m3_logevent_AppliedElevationForcesC previousEventAppliedElevationForces;
	m1m3_logevent_AppliedForcesC previousEventAppliedForces;
	m1m3_logevent_AppliedOffsetForcesC previousEventAppliedOffsetForces;
	m1m3_logevent_AppliedSettingsMatchStartC previousEventAppliedSettingsMatchStart;
	m1m3_logevent_AppliedStaticForcesC previousEventAppliedStaticForces;
	m1m3_logevent_AppliedThermalForcesC previousEventAppliedThermalForces;
	m1m3_logevent_AppliedVelocityForcesC previousEventAppliedVelocityForces;
	m1m3_logevent_CellLightStatusC previousEventCellLightStatus;
	m1m3_logevent_CellLightWarningC previousEventCellLightWarning;
	m1m3_logevent_CommandRejectionWarningC previousEventCommandRejectionWarning;
	m1m3_logevent_DetailedStateC previousEventDetailedState;
	m1m3_logevent_DisplacementSensorWarningC previousEventDisplacementSensorWarning;
	m1m3_logevent_ErrorCodeC previousEventErrorCode;
	m1m3_logevent_ForceActuatorForceWarningC previousEventForceActuatorForceWarning;
	m1m3_logevent_ForceActuatorInfoC previousEventForceActuatorInfo;
	m1m3_logevent_ForceActuatorStateC previousEventForceActuatorState;
	m1m3_logevent_ForceActuatorWarningC previousEventForceActuatorWarning;
	m1m3_logevent_ForceSetpointWarningC previousEventForceSetpointWarning;
	m1m3_logevent_GyroWarningC previousEventGyroWarning;
	m1m3_logevent_HardpointActuatorInfoC previousEventHardpointActuatorInfo;
	m1m3_logevent_HardpointActuatorStateC previousEventHardpointActuatorState;
	m1m3_logevent_HardpointActuatorWarningC previousEventHardpointActuatorWarning;
	m1m3_logevent_HardpointMonitorInfoC previousEventHardpointMonitorInfo;
	m1m3_logevent_HardpointMonitorStateC previousEventHardpointMonitorState;
	m1m3_logevent_HardpointMonitorWarningC previousEventHardpointMonitorWarning;
	m1m3_logevent_ILCWarningC previousEventILCWarning;
	m1m3_logevent_InclinometerSensorWarningC previousEventInclinometerSensorWarning;
	m1m3_logevent_InterlockStatusC previousEventInterlockStatus;
	m1m3_logevent_InterlockWarningC previousEventInterlockWarning;
	m1m3_logevent_PIDInfoC previousEventPIDInfo;
	m1m3_logevent_PowerStatusC previousEventPowerStatus;
	m1m3_logevent_PowerSupplyStatusC previousEventPowerSupplyStatus;
	m1m3_logevent_PowerWarningC previousEventPowerWarning;
	m1m3_logevent_RejectedAberrationForcesC previousEventRejectedAberrationForces;
	m1m3_logevent_RejectedAccelerationForcesC previousEventRejectedAccelerationForces;
	m1m3_logevent_RejectedActiveOpticForcesC previousEventRejectedActiveOpticForces;
	m1m3_logevent_RejectedAzimuthForcesC previousEventRejectedAzimuthForces;
	m1m3_logevent_RejectedBalanceForcesC previousEventRejectedBalanceForces;
	m1m3_logevent_RejectedCylinderForcesC previousEventRejectedCylinderForces;
	m1m3_logevent_RejectedElevationForcesC previousEventRejectedElevationForces;
	m1m3_logevent_RejectedForcesC previousEventRejectedForces;
	m1m3_logevent_RejectedOffsetForcesC previousEventRejectedOffsetForces;
	m1m3_logevent_RejectedStaticForcesC previousEventRejectedStaticForces;
	m1m3_logevent_RejectedThermalForcesC previousEventRejectedThermalForces;
	m1m3_logevent_RejectedVelocityForcesC previousEventRejectedVelocityForces;
	m1m3_logevent_SettingVersionsC previousEventSettingVersions;
	m1m3_logevent_SettingsAppliedC previousEventSettingsApplied;
	m1m3_logevent_SummaryStateC previousEventSummaryState;

public:
	M1M3SSPublisher(SAL_m1m3* m1m3SAL);

	m1m3_AccelerometerDataC* getAccelerometerData() { return &this->accelerometerData; }
	m1m3_ForceActuatorDataC* getForceActuatorData() { return &this->forceActuatorData; }
	m1m3_GyroDataC* getGyroData() { return &this->gyroData; }
	m1m3_HardpointActuatorDataC* getHardpointActuatorData() { return &this->hardpointActuatorData; }
	m1m3_HardpointMonitorDataC* getHardpointMonitorData() { return &this->hardpointMonitorData; }
	m1m3_IMSDataC* getIMSData() { return &this->imsData; }
	m1m3_InclinometerDataC* getInclinometerData() { return &this->inclinometerData; }
	m1m3_OuterLoopDataC* getOuterLoopData() { return &this->outerLoopData; }
	m1m3_PIDDataC* getPIDData() { return &this->pidData; }
	m1m3_PowerSupplyDataC* getPowerSupplyData() { return &this->powerSupplyData; }

	m1m3_logevent_AccelerometerWarningC* getEventAccelerometerWarning() { return &this->eventAccelerometerWarning; }
	m1m3_logevent_AirSupplyStatusC* getEventAirSupplyStatus() { return &this->eventAirSupplyStatus; }
	m1m3_logevent_AirSupplyWarningC* getEventAirSupplyWarning() { return &this->eventAirSupplyWarning; }
	m1m3_logevent_AppliedAberrationForcesC* getEventAppliedAberrationForces() { return &this->eventAppliedAberrationForces; }
	m1m3_logevent_AppliedAccelerationForcesC* getEventAppliedAccelerationForces() { return &this->eventAppliedAccelerationForces; }
	m1m3_logevent_AppliedActiveOpticForcesC* getEventAppliedActiveOpticForces() { return &this->eventAppliedActiveOpticForces; }
	m1m3_logevent_AppliedAzimuthForcesC* getEventAppliedAzimuthForces() { return &this->eventAppliedAzimuthForces; }
	m1m3_logevent_AppliedBalanceForcesC* getEventAppliedBalanceForces() { return &this->eventAppliedBalanceForces; }
	m1m3_logevent_AppliedCylinderForcesC* getEventAppliedCylinderForces() { return &this->eventAppliedCylinderForces; }
	m1m3_logevent_AppliedElevationForcesC* getEventAppliedElevationForces() { return &this->eventAppliedElevationForces; }
	m1m3_logevent_AppliedForcesC* getEventAppliedForces() { return &this->eventAppliedForces; }
	m1m3_logevent_AppliedOffsetForcesC* getEventAppliedOffsetForces() { return &this->eventAppliedOffsetForces; }
	m1m3_logevent_AppliedSettingsMatchStartC* getEventAppliedSettingsMatchStart() { return &this->eventAppliedSettingsMatchStart; }
	m1m3_logevent_AppliedStaticForcesC* getEventAppliedStaticForces() { return &this->eventAppliedStaticForces; }
	m1m3_logevent_AppliedThermalForcesC* getEventAppliedThermalForces() { return &this->eventAppliedThermalForces; }
	m1m3_logevent_AppliedVelocityForcesC* getEventAppliedVelocityForces() { return &this->eventAppliedVelocityForces; }
	m1m3_logevent_CellLightStatusC* getEventCellLightStatus() { return &this->eventCellLightStatus; }
	m1m3_logevent_CellLightWarningC* getEventCellLightWarning() { return &this->eventCellLightWarning; }
	m1m3_logevent_CommandRejectionWarningC* getEventCommandRejectionWarning() { return &this->eventCommandRejectionWarning; }
	m1m3_logevent_DetailedStateC* getEventDetailedState() { return &this->eventDetailedState; }
	m1m3_logevent_DisplacementSensorWarningC* getEventDisplacementSensorWarning() { return &this->eventDisplacementSensorWarning; }
	m1m3_logevent_ErrorCodeC* getEventErrorCode() { return &this->eventErrorCode; }
	m1m3_logevent_ForceActuatorForceWarningC* getEventForceActuatorForceWarning() { return &this->eventForceActuatorForceWarning; }
	m1m3_logevent_ForceActuatorInfoC* getEventForceActuatorInfo() { return &this->eventForceActuatorInfo; }
	m1m3_logevent_ForceActuatorStateC* getEventForceActuatorState() { return &this->eventForceActuatorState; }
	m1m3_logevent_ForceActuatorWarningC* getEventForceActuatorWarning() { return &this->eventForceActuatorWarning; }
	m1m3_logevent_ForceSetpointWarningC* getEventForceSetpointWarning() { return &this->eventForceSetpointWarning; }
	m1m3_logevent_GyroWarningC* getEventGyroWarning() { return &this->eventGyroWarning; }
	m1m3_logevent_HardpointActuatorInfoC* getEventHardpointActuatorInfo() { return &this->eventHardpointActuatorInfo; }
	m1m3_logevent_HardpointActuatorStateC* getEventHardpointActuatorState() { return &this->eventHardpointActuatorState; }
	m1m3_logevent_HardpointActuatorWarningC* getEventHardpointActuatorWarning() { return &this->eventHardpointActuatorWarning; }
	m1m3_logevent_HardpointMonitorInfoC* getEventHardpointMonitorInfo() { return &this->eventHardpointMonitorInfo; }
	m1m3_logevent_HardpointMonitorStateC* getEventHardpointMonitorState() { return &this->eventHardpointMonitorState; }
	m1m3_logevent_HardpointMonitorWarningC* getEventHardpointMonitorWarning() { return &this->eventHardpointMonitorWarning; }
	m1m3_logevent_ILCWarningC* getEventILCWarning(){ return &this->eventILCWarning; }
	m1m3_logevent_InclinometerSensorWarningC* getEventInclinometerSensorWarning() { return &this->eventInclinometerSensorWarning; }
	m1m3_logevent_InterlockStatusC* getEventInterlockStatus() { return &this->eventInterlockStatus; }
	m1m3_logevent_InterlockWarningC* getEventInterlockWarning() { return &this->eventInterlockWarning; }
	m1m3_logevent_PIDInfoC* getEventPIDInfo() { return &this->eventPIDInfo; }
	m1m3_logevent_PowerStatusC* getEventPowerStatus() { return &this->eventPowerStatus; }
	m1m3_logevent_PowerSupplyStatusC* getEventPowerSupplyStatus() { return &this->eventPowerSupplyStatus; }
	m1m3_logevent_PowerWarningC* getEventPowerWarning() { return &this->eventPowerWarning; }
	m1m3_logevent_RejectedAberrationForcesC* getEventRejectedAberrationForces() { return &this->eventRejectedAberrationForces; }
	m1m3_logevent_RejectedAccelerationForcesC* getEventRejectedAccelerationForces() { return &this->eventRejectedAccelerationForces; }
	m1m3_logevent_RejectedActiveOpticForcesC* getEventRejectedActiveOpticForces() { return &this->eventRejectedActiveOpticForces; }
	m1m3_logevent_RejectedAzimuthForcesC* getEventRejectedAzimuthForces() { return &this->eventRejectedAzimuthForces; }
	m1m3_logevent_RejectedBalanceForcesC* getEventRejectedBalanceForces() { return &this->eventRejectedBalanceForces; }
	m1m3_logevent_RejectedCylinderForcesC* getEventRejectedCylinderForces() { return &this->eventRejectedCylinderForces; }
	m1m3_logevent_RejectedElevationForcesC* getEventRejectedElevationForces() { return &this->eventRejectedElevationForces; }
	m1m3_logevent_RejectedForcesC* getEventRejectedForces() { return &this->eventRejectedForces; }
	m1m3_logevent_RejectedOffsetForcesC* getEventRejectedOffsetForces() { return &this->eventRejectedOffsetForces; }
	m1m3_logevent_RejectedStaticForcesC* getEventRejectedStaticForces() { return &this->eventRejectedStaticForces; }
	m1m3_logevent_RejectedThermalForcesC* getEventRejectedThermalForces() { return &this->eventRejectedThermalForces; }
	m1m3_logevent_RejectedVelocityForcesC* getEventRejectedVelocityForces() { return &this->eventRejectedVelocityForces; }
	m1m3_logevent_SettingVersionsC* getEventSettingVersions() { return &this->eventSettingVersions; }
	m1m3_logevent_SettingsAppliedC* getEventSettingsApplied() { return &this->eventSettingsApplied; }
	m1m3_logevent_SummaryStateC* getEventSummaryState() { return &this->eventSummaryState; }

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
	void tryLogSettingVersions();
	void logSettingsApplied();
	void tryLogSettingsApplied();
	void logSummaryState();
	void tryLogSummaryState();

	void ackCommandStart(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandClearOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandLowerM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyActiveOpticForcesByBendingModes(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandClearActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyAberrationForcesByBendingModes(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyAberrationForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandClearAberrationForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnterEngineering(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandExitEngineering(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTestAir(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTestHardpoint(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTestForceActuator(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandMoveHardpointActuators(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandDisableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandAbortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTranslateM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandStopHardpointMotion(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandPositionM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnLightsOn(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnLightsOff(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnPowerOn(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnPowerOff(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnableHardpointCorrections(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandDisableHardpointCorrections(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandRunMirrorForceProfile(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandAbortProfile(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyOffsetForcesByMirrorForce(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandUpdatePID(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandResetPID(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandProgramILC(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
