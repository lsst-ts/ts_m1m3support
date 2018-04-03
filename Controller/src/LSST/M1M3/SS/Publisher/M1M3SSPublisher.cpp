/*
 * M1M3SSPublisher.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSPublisher.h>
#include <SAL_m1m3.h>
#include <ccpp_sal_m1m3.h> // Provides access to enumerations

#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSPublisher::M1M3SSPublisher(SAL_m1m3* m1m3SAL) {
	Log.Debug("M1M3SSPublisher: M1M3SSPublisher()");
	this->m1m3SAL = m1m3SAL;

	Log.Debug("M1M3SSPublisher: Initializing SAL Telemetry");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_AccelerometerData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_ForceActuatorData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_GyroData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_HardpointActuatorData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_HardpointMonitorData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_IMSData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_InclinometerData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_OuterLoopData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_PIDData");
	this->m1m3SAL->salTelemetryPub((char*)"m1m3_PowerSupplyData");

	Log.Debug("M1M3SSPublisher: Initializing SAL Events");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AccelerometerWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AccelerometerWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AirSupplyStatus");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AirSupplyWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedAberrationForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedAccelerationForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedActiveOpticForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedAzimuthForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedBalanceForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedCylinderForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedElevationForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedOffsetForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedSettingsMatchStart");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedStaticForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedThermalForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_AppliedVelocityForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_CellLightStatus");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_CellLightWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_CommandRejectionWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_DetailedState");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_DisplacementSensorWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ErrorCode");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ForceActuatorForceWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ForceActuatorInfo");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ForceActuatorState");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ForceActuatorWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ForceSetpointWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_GyroWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_HardpointActuatorInfo");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_HardpointActuatorState");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_HardpointActuatorWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_HardpointMonitorInfo");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_HardpointMonitorState");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_HardpointMonitorWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_ILCWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_InclinometerSensorWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_InterlockStatus");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_InterlockWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_PIDInfo");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_PowerStatus");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_PowerSupplyStatus");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_PowerWarning");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedAberrationForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedAccelerationForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedActiveOpticForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedAzimuthForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedBalanceForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedCylinderForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedElevationForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedOffsetForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedStaticForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedThermalForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_RejectedVelocityForces");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_SettingVersions");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_SettingsApplied");
	this->m1m3SAL->salEvent((char*)"m1m3_logevent_SummaryState");
}

double M1M3SSPublisher::getTimestamp() { return this->m1m3SAL->getCurrentTime(); }

void M1M3SSPublisher::putAccelerometerData() { this->m1m3SAL->putSample_AccelerometerData(&this->accelerometerData); }
void M1M3SSPublisher::putForceActuatorData() { this->m1m3SAL->putSample_ForceActuatorData(&this->forceActuatorData); }
void M1M3SSPublisher::putGyroData() { this->m1m3SAL->putSample_GyroData(&this->gyroData); }
void M1M3SSPublisher::putHardpointActuatorData() { this->m1m3SAL->putSample_HardpointActuatorData(&this->hardpointActuatorData); }
void M1M3SSPublisher::putHardpointMonitorData() { this->m1m3SAL->putSample_HardpointMonitorData(&this->hardpointMonitorData); }
void M1M3SSPublisher::putIMSData() { this->m1m3SAL->putSample_IMSData(&this->imsData); }
void M1M3SSPublisher::putInclinometerData() { this->m1m3SAL->putSample_InclinometerData(&this->inclinometerData); }
void M1M3SSPublisher::putOuterLoopData() {
	// TODO: Remove, just for testing
	uint8_t t = this->outerLoopData.BroadcastCounter;
	this->outerLoopData.BroadcastCounter = 0x30;
	this->m1m3SAL->putSample_OuterLoopData(&this->outerLoopData);
	this->outerLoopData.BroadcastCounter = t;
 }
void M1M3SSPublisher::putPIDData() { this->m1m3SAL->putSample_PIDData(&this->pidData); }
void M1M3SSPublisher::putPowerSupplyData() { this->m1m3SAL->putSample_PowerSupplyData(&this->powerSupplyData); }

void M1M3SSPublisher::logAccelerometerWarning() {
	this->eventAccelerometerWarning.AnyWarning =
		this->eventAccelerometerWarning.ResponseTimeout;
	this->m1m3SAL->logEvent_AccelerometerWarning(&this->eventAccelerometerWarning, 0);
	this->previousEventAccelerometerWarning = this->eventAccelerometerWarning;
}

void M1M3SSPublisher::tryLogAccelerometerWarning() {
	if (this->eventAccelerometerWarning.ResponseTimeout != this->previousEventAccelerometerWarning.ResponseTimeout) {
		this->logAccelerometerWarning();
	}
}

void M1M3SSPublisher::logAirSupplyStatus() {
	this->m1m3SAL->logEvent_AirSupplyStatus(&this->eventAirSupplyStatus, 0);
	this->previousEventAirSupplyStatus = this->eventAirSupplyStatus;
}

void M1M3SSPublisher::tryLogAirSupplyStatus() {
	if (this->eventAirSupplyStatus.AirCommandedOn != this->previousEventAirSupplyStatus.AirCommandedOn ||
		this->eventAirSupplyStatus.AirCommandOutputOn != this->previousEventAirSupplyStatus.AirCommandOutputOn ||
		this->eventAirSupplyStatus.AirValveOpened != this->previousEventAirSupplyStatus.AirValveOpened ||
		this->eventAirSupplyStatus.AirValveClosed != this->previousEventAirSupplyStatus.AirValveClosed) {
		this->logAirSupplyStatus();
	}
}

void M1M3SSPublisher::logAirSupplyWarning() {
	this->eventAirSupplyWarning.AnyWarning =
		this->eventAirSupplyWarning.CommandOutputMismatch ||
		this->eventAirSupplyWarning.CommandSensorMismatch;
	this->m1m3SAL->logEvent_AirSupplyWarning(&this->eventAirSupplyWarning, 0);
	this->previousEventAirSupplyWarning = this->eventAirSupplyWarning;
}

void M1M3SSPublisher::tryLogAirSupplyWarning() {
	if (this->eventAirSupplyWarning.CommandOutputMismatch != this->previousEventAirSupplyWarning.CommandOutputMismatch ||
		this->eventAirSupplyWarning.CommandSensorMismatch != this->previousEventAirSupplyWarning.CommandSensorMismatch) {
		this->logAirSupplyWarning();
	}
}

void M1M3SSPublisher::logAppliedAberrationForces() {
	this->m1m3SAL->logEvent_AppliedAberrationForces(&this->eventAppliedAberrationForces, 0);
	this->previousEventAppliedAberrationForces = this->eventAppliedAberrationForces;
}

void M1M3SSPublisher::tryLogAppliedAberrationForces() {
	bool changeDetected =
		this->eventAppliedAberrationForces.Fz != this->previousEventAppliedAberrationForces.Fz ||
		this->eventAppliedAberrationForces.Mx != this->previousEventAppliedAberrationForces.Mx ||
		this->eventAppliedAberrationForces.My != this->previousEventAppliedAberrationForces.My;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventAppliedAberrationForces.ZForces[i] != this->previousEventAppliedAberrationForces.ZForces[i];
	}
	if (changeDetected) {
		this->logAppliedAberrationForces();
	}
}

void M1M3SSPublisher::logAppliedAccelerationForces() {
	this->m1m3SAL->logEvent_AppliedAccelerationForces(&this->eventAppliedAccelerationForces, 0);
	this->previousEventAppliedAccelerationForces = this->eventAppliedAccelerationForces;
}

void M1M3SSPublisher::tryLogAppliedAccelerationForces() {
	bool changeDetected =
		this->eventAppliedAccelerationForces.Fx != this->previousEventAppliedAccelerationForces.Fx ||
		this->eventAppliedAccelerationForces.Fy != this->previousEventAppliedAccelerationForces.Fy ||
		this->eventAppliedAccelerationForces.Fz != this->previousEventAppliedAccelerationForces.Fz ||
		this->eventAppliedAccelerationForces.Mx != this->previousEventAppliedAccelerationForces.Mx ||
		this->eventAppliedAccelerationForces.My != this->previousEventAppliedAccelerationForces.My ||
		this->eventAppliedAccelerationForces.Mz != this->previousEventAppliedAccelerationForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedAccelerationForces.XForces[i] != this->previousEventAppliedAccelerationForces.XForces[i]) ||
			(i < 100 && this->eventAppliedAccelerationForces.YForces[i] != this->previousEventAppliedAccelerationForces.YForces[i]) ||
			(this->eventAppliedAccelerationForces.ZForces[i] != this->previousEventAppliedAccelerationForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedAccelerationForces();
	}
}

void M1M3SSPublisher::logAppliedActiveOpticForces() {
	this->m1m3SAL->logEvent_AppliedActiveOpticForces(&this->eventAppliedActiveOpticForces, 0);
	this->previousEventAppliedActiveOpticForces = this->eventAppliedActiveOpticForces;
}

void M1M3SSPublisher::tryLogAppliedActiveOpticForces() {
	bool changeDetected =
		this->eventAppliedActiveOpticForces.Fz != this->previousEventAppliedActiveOpticForces.Fz ||
		this->eventAppliedActiveOpticForces.Mx != this->previousEventAppliedActiveOpticForces.Mx ||
		this->eventAppliedActiveOpticForces.My != this->previousEventAppliedActiveOpticForces.My;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventAppliedActiveOpticForces.ZForces[i] != this->previousEventAppliedActiveOpticForces.ZForces[i];
	}
	if (changeDetected) {
		this->logAppliedActiveOpticForces();
	}
}

void M1M3SSPublisher::logAppliedAzimuthForces() {
	this->m1m3SAL->logEvent_AppliedAzimuthForces(&this->eventAppliedAzimuthForces, 0);
	this->previousEventAppliedAzimuthForces = this->eventAppliedAzimuthForces;
}

void M1M3SSPublisher::tryLogAppliedAzimuthForces() {
	bool changeDetected =
		this->eventAppliedAzimuthForces.Fx != this->previousEventAppliedAzimuthForces.Fx ||
		this->eventAppliedAzimuthForces.Fy != this->previousEventAppliedAzimuthForces.Fy ||
		this->eventAppliedAzimuthForces.Fz != this->previousEventAppliedAzimuthForces.Fz ||
		this->eventAppliedAzimuthForces.Mx != this->previousEventAppliedAzimuthForces.Mx ||
		this->eventAppliedAzimuthForces.My != this->previousEventAppliedAzimuthForces.My ||
		this->eventAppliedAzimuthForces.Mz != this->previousEventAppliedAzimuthForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedAzimuthForces.XForces[i] != this->previousEventAppliedAzimuthForces.XForces[i]) ||
			(i < 100 && this->eventAppliedAzimuthForces.YForces[i] != this->previousEventAppliedAzimuthForces.YForces[i]) ||
			(this->eventAppliedAzimuthForces.ZForces[i] != this->previousEventAppliedAzimuthForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedAzimuthForces();
	}
}

void M1M3SSPublisher::logAppliedBalanceForces() {
	this->m1m3SAL->logEvent_AppliedBalanceForces(&this->eventAppliedBalanceForces, 0);
	this->previousEventAppliedBalanceForces = this->eventAppliedBalanceForces;
}

void M1M3SSPublisher::tryLogAppliedBalanceForces() {
	bool changeDetected =
		this->eventAppliedBalanceForces.Fx != this->previousEventAppliedBalanceForces.Fx ||
		this->eventAppliedBalanceForces.Fy != this->previousEventAppliedBalanceForces.Fy ||
		this->eventAppliedBalanceForces.Fz != this->previousEventAppliedBalanceForces.Fz ||
		this->eventAppliedBalanceForces.Mx != this->previousEventAppliedBalanceForces.Mx ||
		this->eventAppliedBalanceForces.My != this->previousEventAppliedBalanceForces.My ||
		this->eventAppliedBalanceForces.Mz != this->previousEventAppliedBalanceForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedBalanceForces.XForces[i] != this->previousEventAppliedBalanceForces.XForces[i]) ||
			(i < 100 && this->eventAppliedBalanceForces.YForces[i] != this->previousEventAppliedBalanceForces.YForces[i]) ||
			(this->eventAppliedBalanceForces.ZForces[i] != this->previousEventAppliedBalanceForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedBalanceForces();
	}
}

void M1M3SSPublisher::logAppliedCylinderForces() {
	this->m1m3SAL->logEvent_AppliedCylinderForces(&this->eventAppliedCylinderForces, 0);
	this->previousEventAppliedCylinderForces = this->eventAppliedCylinderForces;
}

void M1M3SSPublisher::tryLogAppliedCylinderForces() {
	bool changeDetected = false;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 112 && this->eventAppliedCylinderForces.SecondaryCylinderForces[i] != this->previousEventAppliedCylinderForces.SecondaryCylinderForces[i]) ||
			(this->eventAppliedCylinderForces.PrimaryCylinderForces[i] != this->previousEventAppliedCylinderForces.PrimaryCylinderForces[i]);
	}
	if (changeDetected) {
		this->logAppliedCylinderForces();
	}
}

void M1M3SSPublisher::logAppliedElevationForces() {
	this->m1m3SAL->logEvent_AppliedElevationForces(&this->eventAppliedElevationForces, 0);
	this->previousEventAppliedElevationForces = this->eventAppliedElevationForces;
}

void M1M3SSPublisher::tryLogAppliedElevationForces() {
	bool changeDetected =
		this->eventAppliedElevationForces.Fx != this->previousEventAppliedElevationForces.Fx ||
		this->eventAppliedElevationForces.Fy != this->previousEventAppliedElevationForces.Fy ||
		this->eventAppliedElevationForces.Fz != this->previousEventAppliedElevationForces.Fz ||
		this->eventAppliedElevationForces.Mx != this->previousEventAppliedElevationForces.Mx ||
		this->eventAppliedElevationForces.My != this->previousEventAppliedElevationForces.My ||
		this->eventAppliedElevationForces.Mz != this->previousEventAppliedElevationForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedElevationForces.XForces[i] != this->previousEventAppliedElevationForces.XForces[i]) ||
			(i < 100 && this->eventAppliedElevationForces.YForces[i] != this->previousEventAppliedElevationForces.YForces[i]) ||
			(this->eventAppliedElevationForces.ZForces[i] != this->previousEventAppliedElevationForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedElevationForces();
	}
}

void M1M3SSPublisher::logAppliedForces() {
	this->m1m3SAL->logEvent_AppliedForces(&this->eventAppliedForces, 0);
	this->previousEventAppliedForces = this->eventAppliedForces;
}

void M1M3SSPublisher::tryLogAppliedForces() {
	bool changeDetected =
		this->eventAppliedForces.Fx != this->previousEventAppliedForces.Fx ||
		this->eventAppliedForces.Fy != this->previousEventAppliedForces.Fy ||
		this->eventAppliedForces.Fz != this->previousEventAppliedForces.Fz ||
		this->eventAppliedForces.Mx != this->previousEventAppliedForces.Mx ||
		this->eventAppliedForces.My != this->previousEventAppliedForces.My ||
		this->eventAppliedForces.Mz != this->previousEventAppliedForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedForces.XForces[i] != this->previousEventAppliedForces.XForces[i]) ||
			(i < 100 && this->eventAppliedForces.YForces[i] != this->previousEventAppliedForces.YForces[i]) ||
			(this->eventAppliedForces.ZForces[i] != this->previousEventAppliedForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedForces();
	}
}

void M1M3SSPublisher::logAppliedOffsetForces() {
	this->m1m3SAL->logEvent_AppliedOffsetForces(&this->eventAppliedOffsetForces, 0);
	this->previousEventAppliedOffsetForces = this->eventAppliedOffsetForces;
}

void M1M3SSPublisher::tryLogAppliedOffsetForces() {
	bool changeDetected =
		this->eventAppliedOffsetForces.Fx != this->previousEventAppliedOffsetForces.Fx ||
		this->eventAppliedOffsetForces.Fy != this->previousEventAppliedOffsetForces.Fy ||
		this->eventAppliedOffsetForces.Fz != this->previousEventAppliedOffsetForces.Fz ||
		this->eventAppliedOffsetForces.Mx != this->previousEventAppliedOffsetForces.Mx ||
		this->eventAppliedOffsetForces.My != this->previousEventAppliedOffsetForces.My ||
		this->eventAppliedOffsetForces.Mz != this->previousEventAppliedOffsetForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedOffsetForces.XForces[i] != this->previousEventAppliedOffsetForces.XForces[i]) ||
			(i < 100 && this->eventAppliedOffsetForces.YForces[i] != this->previousEventAppliedOffsetForces.YForces[i]) ||
			(this->eventAppliedOffsetForces.ZForces[i] != this->previousEventAppliedOffsetForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedOffsetForces();
	}
}

void M1M3SSPublisher::logAppliedSettingsMatchStart() {
	this->m1m3SAL->logEvent_AppliedSettingsMatchStart(&this->eventAppliedSettingsMatchStart, 0);
	this->previousEventAppliedSettingsMatchStart = this->eventAppliedSettingsMatchStart;
}

void M1M3SSPublisher::tryLogAppliedSettingsMatchStart() {
	if (this->eventAppliedSettingsMatchStart.AppliedSettingsMatchStart != this->previousEventAppliedSettingsMatchStart.AppliedSettingsMatchStart) {
		this->logAppliedSettingsMatchStart();
	}
}

void M1M3SSPublisher::logAppliedStaticForces() {
	this->m1m3SAL->logEvent_AppliedStaticForces(&this->eventAppliedStaticForces, 0);
	this->previousEventAppliedStaticForces = this->eventAppliedStaticForces;
}

void M1M3SSPublisher::tryLogAppliedStaticForces() {
	bool changeDetected =
		this->eventAppliedStaticForces.Fx != this->previousEventAppliedStaticForces.Fx ||
		this->eventAppliedStaticForces.Fy != this->previousEventAppliedStaticForces.Fy ||
		this->eventAppliedStaticForces.Fz != this->previousEventAppliedStaticForces.Fz ||
		this->eventAppliedStaticForces.Mx != this->previousEventAppliedStaticForces.Mx ||
		this->eventAppliedStaticForces.My != this->previousEventAppliedStaticForces.My ||
		this->eventAppliedStaticForces.Mz != this->previousEventAppliedStaticForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedStaticForces.XForces[i] != this->previousEventAppliedStaticForces.XForces[i]) ||
			(i < 100 && this->eventAppliedStaticForces.YForces[i] != this->previousEventAppliedStaticForces.YForces[i]) ||
			(this->eventAppliedStaticForces.ZForces[i] != this->previousEventAppliedStaticForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedStaticForces();
	}
}

void M1M3SSPublisher::logAppliedThermalForces() {
	this->m1m3SAL->logEvent_AppliedThermalForces(&this->eventAppliedThermalForces, 0);
	this->previousEventAppliedThermalForces = this->eventAppliedThermalForces;
}

void M1M3SSPublisher::tryLogAppliedThermalForces() {
	bool changeDetected =
		this->eventAppliedThermalForces.Fx != this->previousEventAppliedThermalForces.Fx ||
		this->eventAppliedThermalForces.Fy != this->previousEventAppliedThermalForces.Fy ||
		this->eventAppliedThermalForces.Fz != this->previousEventAppliedThermalForces.Fz ||
		this->eventAppliedThermalForces.Mx != this->previousEventAppliedThermalForces.Mx ||
		this->eventAppliedThermalForces.My != this->previousEventAppliedThermalForces.My ||
		this->eventAppliedThermalForces.Mz != this->previousEventAppliedThermalForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedThermalForces.XForces[i] != this->previousEventAppliedThermalForces.XForces[i]) ||
			(i < 100 && this->eventAppliedThermalForces.YForces[i] != this->previousEventAppliedThermalForces.YForces[i]) ||
			(this->eventAppliedThermalForces.ZForces[i] != this->previousEventAppliedThermalForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedThermalForces();
	}
}

void M1M3SSPublisher::logAppliedVelocityForces() {
	this->m1m3SAL->logEvent_AppliedVelocityForces(&this->eventAppliedVelocityForces, 0);
	this->previousEventAppliedVelocityForces = this->eventAppliedVelocityForces;
}

void M1M3SSPublisher::tryLogAppliedVelocityForces() {
	bool changeDetected =
		this->eventAppliedVelocityForces.Fx != this->previousEventAppliedVelocityForces.Fx ||
		this->eventAppliedVelocityForces.Fy != this->previousEventAppliedVelocityForces.Fy ||
		this->eventAppliedVelocityForces.Fz != this->previousEventAppliedVelocityForces.Fz ||
		this->eventAppliedVelocityForces.Mx != this->previousEventAppliedVelocityForces.Mx ||
		this->eventAppliedVelocityForces.My != this->previousEventAppliedVelocityForces.My ||
		this->eventAppliedVelocityForces.Mz != this->previousEventAppliedVelocityForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventAppliedVelocityForces.XForces[i] != this->previousEventAppliedVelocityForces.XForces[i]) ||
			(i < 100 && this->eventAppliedVelocityForces.YForces[i] != this->previousEventAppliedVelocityForces.YForces[i]) ||
			(this->eventAppliedVelocityForces.ZForces[i] != this->previousEventAppliedVelocityForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logAppliedVelocityForces();
	}
}

void M1M3SSPublisher::logCellLightStatus() {
	this->m1m3SAL->logEvent_CellLightStatus(&this->eventCellLightStatus, 0);
	this->previousEventCellLightStatus = this->eventCellLightStatus;
}

void M1M3SSPublisher::tryLogCellLightStatus() {
	if (this->eventCellLightStatus.CellLightsCommandedOn != this->previousEventCellLightStatus.CellLightsCommandedOn ||
		this->eventCellLightStatus.CellLightsOutputOn != this->previousEventCellLightStatus.CellLightsOutputOn ||
		this->eventCellLightStatus.CellLightsOn != this->previousEventCellLightStatus.CellLightsOn) {
		this->logCellLightStatus();
	}
}

void M1M3SSPublisher::logCellLightWarning() {
	this->eventCellLightWarning.AnyWarning =
		this->eventCellLightWarning.CellLightsOutputMismatch ||
		this->eventCellLightWarning.CellLightsSensorMismatch;
	this->m1m3SAL->logEvent_CellLightWarning(&this->eventCellLightWarning, 0);
	this->previousEventCellLightWarning = this->eventCellLightWarning;
}

void M1M3SSPublisher::tryLogCellLightWarning() {
	if (this->eventCellLightWarning.CellLightsOutputMismatch != this->previousEventCellLightWarning.CellLightsOutputMismatch ||
		this->eventCellLightWarning.CellLightsSensorMismatch != this->previousEventCellLightWarning.CellLightsSensorMismatch) {
		this->logCellLightWarning();
	}
}

void M1M3SSPublisher::logCommandRejectionWarning() {
	this->m1m3SAL->logEvent_CommandRejectionWarning(&this->eventCommandRejectionWarning, 0);
	this->previousEventCommandRejectionWarning = this->eventCommandRejectionWarning;
}

void M1M3SSPublisher::tryLogCommandRejectionWarning() {
	if (this->eventCommandRejectionWarning.Command.compare(this->previousEventCommandRejectionWarning.Command) != 0 ||
		this->eventCommandRejectionWarning.Reason.compare(this->previousEventCommandRejectionWarning.Reason) != 0) {
		this->logCommandRejectionWarning();
	}
}

void M1M3SSPublisher::logCommandRejectionWarning(std::string command, std::string reason) {
	this->eventCommandRejectionWarning.Timestamp = this->getTimestamp();
	this->eventCommandRejectionWarning.Command = command;
	this->eventCommandRejectionWarning.Reason = reason;
	this->logCommandRejectionWarning();
}

void M1M3SSPublisher::tryLogCommandRejectionWarning(std::string command, std::string reason) {
	this->eventCommandRejectionWarning.Timestamp = this->getTimestamp();
	this->eventCommandRejectionWarning.Command = command;
	this->eventCommandRejectionWarning.Reason = reason;
	this->tryLogCommandRejectionWarning();
}

void M1M3SSPublisher::logDetailedState() {
	this->m1m3SAL->logEvent_DetailedState(&this->eventDetailedState, 0);
	this->previousEventDetailedState = this->eventDetailedState;
}

void M1M3SSPublisher::tryLogDetailedState() {
	if (this->eventDetailedState.DetailedState != this->previousEventDetailedState.DetailedState) {
		this->logDetailedState();
	}
}

void M1M3SSPublisher::logDisplacementSensorWarning() {
	this->eventDisplacementSensorWarning.AnyWarning =
		this->eventDisplacementSensorWarning.SensorReportsInvalidCommand ||
		this->eventDisplacementSensorWarning.SensorReportsCommunicationTimeoutError ||
		this->eventDisplacementSensorWarning.SensorReportsDataLengthError ||
		this->eventDisplacementSensorWarning.SensorReportsNumberOfParametersError ||
		this->eventDisplacementSensorWarning.SensorReportsParameterError ||
		this->eventDisplacementSensorWarning.SensorReportsCommunicationError ||
		this->eventDisplacementSensorWarning.SensorReportsIDNumberError ||
		this->eventDisplacementSensorWarning.SensorReportsExpansionLineError ||
		this->eventDisplacementSensorWarning.SensorReportsWriteControlError ||
		this->eventDisplacementSensorWarning.ResponseTimeout ||
		this->eventDisplacementSensorWarning.InvalidLength ||
		this->eventDisplacementSensorWarning.InvalidResponse ||
		this->eventDisplacementSensorWarning.UnknownCommand ||
		this->eventDisplacementSensorWarning.UnknownProblem;
	this->m1m3SAL->logEvent_DisplacementSensorWarning(&this->eventDisplacementSensorWarning, 0);
	this->previousEventDisplacementSensorWarning = this->eventDisplacementSensorWarning;
}

void M1M3SSPublisher::tryLogDisplacementSensorWarning() {
	if (this->eventDisplacementSensorWarning.SensorReportsInvalidCommand != this->previousEventDisplacementSensorWarning.SensorReportsInvalidCommand ||
		this->eventDisplacementSensorWarning.SensorReportsCommunicationTimeoutError != this->previousEventDisplacementSensorWarning.SensorReportsCommunicationTimeoutError ||
		this->eventDisplacementSensorWarning.SensorReportsDataLengthError != this->previousEventDisplacementSensorWarning.SensorReportsDataLengthError ||
		this->eventDisplacementSensorWarning.SensorReportsNumberOfParametersError != this->previousEventDisplacementSensorWarning.SensorReportsNumberOfParametersError ||
		this->eventDisplacementSensorWarning.SensorReportsParameterError != this->previousEventDisplacementSensorWarning.SensorReportsParameterError ||
		this->eventDisplacementSensorWarning.SensorReportsCommunicationError != this->previousEventDisplacementSensorWarning.SensorReportsCommunicationError ||
		this->eventDisplacementSensorWarning.SensorReportsIDNumberError != this->previousEventDisplacementSensorWarning.SensorReportsIDNumberError ||
		this->eventDisplacementSensorWarning.SensorReportsExpansionLineError != this->previousEventDisplacementSensorWarning.SensorReportsExpansionLineError ||
		this->eventDisplacementSensorWarning.SensorReportsWriteControlError != this->previousEventDisplacementSensorWarning.SensorReportsWriteControlError ||
		this->eventDisplacementSensorWarning.ResponseTimeout != this->previousEventDisplacementSensorWarning.ResponseTimeout ||
		this->eventDisplacementSensorWarning.InvalidLength != this->previousEventDisplacementSensorWarning.InvalidLength ||
		this->eventDisplacementSensorWarning.InvalidResponse != this->previousEventDisplacementSensorWarning.InvalidResponse ||
		this->eventDisplacementSensorWarning.UnknownCommand != this->previousEventDisplacementSensorWarning.UnknownCommand ||
		this->eventDisplacementSensorWarning.UnknownProblem != this->previousEventDisplacementSensorWarning.UnknownProblem ) {
		this->logDisplacementSensorWarning();
	}
}

void M1M3SSPublisher::logErrorCode() {
	this->m1m3SAL->logEvent_ErrorCode(&this->eventErrorCode, 0);
	this->previousEventErrorCode = this->eventErrorCode;
}

void M1M3SSPublisher::tryLogErrorCode() {
	if (this->eventErrorCode.ErrorCode != this->previousEventErrorCode.ErrorCode ||
		this->eventErrorCode.DetailedErrorCode != this->previousEventErrorCode.DetailedErrorCode) {
		this->logErrorCode();
	}
}

void M1M3SSPublisher::logForceActuatorForceWarning() {
	for(int i = 0; i < FA_COUNT; ++i) {
		this->eventForceActuatorForceWarning.AnyPrimaryAxisMeasuredForceWarning = this->eventForceActuatorForceWarning.AnyPrimaryAxisMeasuredForceWarning ||
				this->eventForceActuatorForceWarning.PrimaryAxisMeasuredForceWarning[i];
		this->eventForceActuatorForceWarning.AnySecondaryAxisMeasuredForceWarning = this->eventForceActuatorForceWarning.AnySecondaryAxisMeasuredForceWarning ||
				this->eventForceActuatorForceWarning.SecondaryAxisMeasuredForceWarning[i];
		this->eventForceActuatorForceWarning.AnyPrimaryAxisFollowingErrorWarning = this->eventForceActuatorForceWarning.AnyPrimaryAxisFollowingErrorWarning ||
				this->eventForceActuatorForceWarning.PrimaryAxisFollowingErrorWarning[i];
		this->eventForceActuatorForceWarning.AnySecondaryAxisFollowingErrorWarning = this->eventForceActuatorForceWarning.AnySecondaryAxisFollowingErrorWarning ||
				this->eventForceActuatorForceWarning.SecondaryAxisFollowingErrorWarning[i];
	}
	this->eventForceActuatorForceWarning.AnyWarning =
		this->eventForceActuatorForceWarning.AnyPrimaryAxisMeasuredForceWarning ||
		this->eventForceActuatorForceWarning.AnySecondaryAxisMeasuredForceWarning ||
		this->eventForceActuatorForceWarning.AnyPrimaryAxisFollowingErrorWarning ||
		this->eventForceActuatorForceWarning.AnySecondaryAxisFollowingErrorWarning;
	this->m1m3SAL->logEvent_ForceActuatorForceWarning(&this->eventForceActuatorForceWarning, 0);
	this->previousEventForceActuatorForceWarning = this->eventForceActuatorForceWarning;
}

void M1M3SSPublisher::tryLogForceActuatorForceWarning() {
	bool changeDetected = false;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventForceActuatorForceWarning.PrimaryAxisMeasuredForceWarning[i] != this->previousEventForceActuatorForceWarning.PrimaryAxisMeasuredForceWarning[i] ||
			this->eventForceActuatorForceWarning.SecondaryAxisMeasuredForceWarning[i] != this->previousEventForceActuatorForceWarning.SecondaryAxisMeasuredForceWarning[i] ||
			this->eventForceActuatorForceWarning.PrimaryAxisFollowingErrorWarning[i] != this->previousEventForceActuatorForceWarning.PrimaryAxisFollowingErrorWarning[i] ||
			this->eventForceActuatorForceWarning.SecondaryAxisFollowingErrorWarning[i] != this->previousEventForceActuatorForceWarning.SecondaryAxisFollowingErrorWarning[i];
	}
	if (changeDetected) {
		this->logForceActuatorForceWarning();
	}
}

void M1M3SSPublisher::logForceActuatorInfo() {
	this->m1m3SAL->logEvent_ForceActuatorInfo(&this->eventForceActuatorInfo, 0);
	this->previousEventForceActuatorInfo = this->eventForceActuatorInfo;
}

void M1M3SSPublisher::tryLogForceActuatorInfo() {
	bool changeDetected = false;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventForceActuatorInfo.ReferenceId[i] != this->previousEventForceActuatorInfo.ReferenceId[i] ||
			this->eventForceActuatorInfo.XDataReferenceId[i] != this->previousEventForceActuatorInfo.XDataReferenceId[i] ||
			this->eventForceActuatorInfo.YDataReferenceId[i] != this->previousEventForceActuatorInfo.YDataReferenceId[i] ||
			this->eventForceActuatorInfo.ZDataReferenceId[i] != this->previousEventForceActuatorInfo.ZDataReferenceId[i] ||
			this->eventForceActuatorInfo.ActuatorType[i] != this->previousEventForceActuatorInfo.ActuatorType[i] ||
			this->eventForceActuatorInfo.ActuatorOrientation[i] != this->previousEventForceActuatorInfo.ActuatorOrientation[i] ||
			this->eventForceActuatorInfo.ModbusSubnet[i] != this->previousEventForceActuatorInfo.ModbusSubnet[i] ||
			this->eventForceActuatorInfo.ModbusAddress[i] != this->previousEventForceActuatorInfo.ModbusAddress[i] ||
			this->eventForceActuatorInfo.XPosition[i] != this->previousEventForceActuatorInfo.XPosition[i] ||
			this->eventForceActuatorInfo.YPosition[i] != this->previousEventForceActuatorInfo.YPosition[i] ||
			this->eventForceActuatorInfo.ZPosition[i] != this->previousEventForceActuatorInfo.ZPosition[i] ||
			this->eventForceActuatorInfo.ILCUniqueId[i] != this->previousEventForceActuatorInfo.ILCUniqueId[i] ||
			this->eventForceActuatorInfo.ILCApplicationType[i] != this->previousEventForceActuatorInfo.ILCApplicationType[i] ||
			this->eventForceActuatorInfo.NetworkNodeType[i] != this->previousEventForceActuatorInfo.NetworkNodeType[i] ||
			this->eventForceActuatorInfo.ILCSelectedOptions[i] != this->previousEventForceActuatorInfo.ILCSelectedOptions[i] ||
			this->eventForceActuatorInfo.NetworkNodeOptions[i] != this->previousEventForceActuatorInfo.NetworkNodeOptions[i] ||
			this->eventForceActuatorInfo.MajorRevision[i] != this->previousEventForceActuatorInfo.MajorRevision[i] ||
			this->eventForceActuatorInfo.MinorRevision[i] != this->previousEventForceActuatorInfo.MinorRevision[i] ||
			this->eventForceActuatorInfo.ADCScanRate[i] != this->previousEventForceActuatorInfo.ADCScanRate[i] ||
			this->eventForceActuatorInfo.MainPrimaryCylinderCoefficient[i] != this->previousEventForceActuatorInfo.MainPrimaryCylinderCoefficient[i] ||
			this->eventForceActuatorInfo.MainSecondaryCylinderCoefficient[i] != this->previousEventForceActuatorInfo.MainSecondaryCylinderCoefficient[i] ||
			this->eventForceActuatorInfo.MainPrimaryCylinderLoadCellOffset[i] != this->previousEventForceActuatorInfo.MainPrimaryCylinderLoadCellOffset[i] ||
			this->eventForceActuatorInfo.MainSecondaryCylinderLoadCellOffset[i] != this->previousEventForceActuatorInfo.MainSecondaryCylinderLoadCellOffset[i] ||
			this->eventForceActuatorInfo.MainPrimaryCylinderLoadCellSensitivity[i] != this->previousEventForceActuatorInfo.MainPrimaryCylinderLoadCellSensitivity[i] ||
			this->eventForceActuatorInfo.MainSecondaryCylinderLoadCellSensitivity[i] != this->previousEventForceActuatorInfo.MainSecondaryCylinderLoadCellSensitivity[i] ||
			this->eventForceActuatorInfo.BackupPrimaryCylinderCoefficient[i] != this->previousEventForceActuatorInfo.BackupPrimaryCylinderCoefficient[i] ||
			this->eventForceActuatorInfo.BackupSecondaryCylinderCoefficient[i] != this->previousEventForceActuatorInfo.BackupSecondaryCylinderCoefficient[i] ||
			this->eventForceActuatorInfo.BackupPrimaryCylinderLoadCellOffset[i] != this->previousEventForceActuatorInfo.BackupPrimaryCylinderLoadCellOffset[i] ||
			this->eventForceActuatorInfo.BackupSecondaryCylinderLoadCellOffset[i] != this->previousEventForceActuatorInfo.BackupSecondaryCylinderLoadCellOffset[i] ||
			this->eventForceActuatorInfo.BackupPrimaryCylinderLoadCellSensitivity[i] != this->previousEventForceActuatorInfo.BackupPrimaryCylinderLoadCellSensitivity[i] ||
			this->eventForceActuatorInfo.BackupSecondaryCylinderLoadCellSensitivity[i] != this->previousEventForceActuatorInfo.BackupSecondaryCylinderLoadCellSensitivity[i] ||
			this->eventForceActuatorInfo.MezzaninePrimaryCylinderGain[i] != this->previousEventForceActuatorInfo.MezzaninePrimaryCylinderGain[i] ||
			this->eventForceActuatorInfo.MezzanineSecondaryCylinderGain[i] != this->previousEventForceActuatorInfo.MezzanineSecondaryCylinderGain[i] ||
			this->eventForceActuatorInfo.MezzanineUniqueId[i] != this->previousEventForceActuatorInfo.MezzanineUniqueId[i] ||
			this->eventForceActuatorInfo.MezzanineFirmwareType[i] != this->previousEventForceActuatorInfo.MezzanineFirmwareType[i] ||
			this->eventForceActuatorInfo.MezzanineMajorRevision[i] != this->previousEventForceActuatorInfo.MezzanineMajorRevision[i] ||
			this->eventForceActuatorInfo.MezzanineMinorRevision[i] != this->previousEventForceActuatorInfo.MezzanineMinorRevision[i];
	}
	if (changeDetected) {
		this->logForceActuatorInfo();
	}
}

void M1M3SSPublisher::logForceActuatorState() {
	this->m1m3SAL->logEvent_ForceActuatorState(&this->eventForceActuatorState, 0);
	this->previousEventForceActuatorState = this->eventForceActuatorState;
}

void M1M3SSPublisher::tryLogForceActuatorState() {
	bool changeDetected =
		this->eventForceActuatorState.SlewFlag != this->previousEventForceActuatorState.SlewFlag ||
		this->eventForceActuatorState.StaticForcesApplied != this->previousEventForceActuatorState.StaticForcesApplied ||
		this->eventForceActuatorState.ElevationForcesApplied != this->previousEventForceActuatorState.ElevationForcesApplied ||
		this->eventForceActuatorState.AzimuthForcesApplied != this->previousEventForceActuatorState.AzimuthForcesApplied ||
		this->eventForceActuatorState.ThermalForcesApplied != this->previousEventForceActuatorState.ThermalForcesApplied ||
		this->eventForceActuatorState.OffsetForcesApplied != this->previousEventForceActuatorState.OffsetForcesApplied ||
		this->eventForceActuatorState.AccelerationForcesApplied != this->previousEventForceActuatorState.AccelerationForcesApplied ||
		this->eventForceActuatorState.VelocityForcesApplied != this->previousEventForceActuatorState.VelocityForcesApplied ||
		this->eventForceActuatorState.ActiveOpticForcesApplied != this->previousEventForceActuatorState.ActiveOpticForcesApplied ||
		this->eventForceActuatorState.AberrationForcesApplied != this->previousEventForceActuatorState.AberrationForcesApplied ||
		this->eventForceActuatorState.BalanceForcesApplied != this->previousEventForceActuatorState.BalanceForcesApplied ||
		this->eventForceActuatorState.SupportPercentage != this->previousEventForceActuatorState.SupportPercentage;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventForceActuatorState.ILCState[i] != this->previousEventForceActuatorState.ILCState[i];
	}
	if (changeDetected) {
		this->logForceActuatorState();
	}
}

void M1M3SSPublisher::logForceActuatorWarning() {
	for(int i = 0; i < FA_COUNT; ++i) {
		this->eventForceActuatorWarning.AnyMajorFault = this->eventForceActuatorWarning.AnyMajorFault ||
				this->eventForceActuatorWarning.MajorFault[i];
		this->eventForceActuatorWarning.AnyMinorFault = this->eventForceActuatorWarning.AnyMinorFault ||
				this->eventForceActuatorWarning.MinorFault[i];
		this->eventForceActuatorWarning.AnyFaultOverride = this->eventForceActuatorWarning.AnyFaultOverride ||
				this->eventForceActuatorWarning.FaultOverride[i];
		this->eventForceActuatorWarning.AnyMainCalibrationError = this->eventForceActuatorWarning.AnyMainCalibrationError ||
				this->eventForceActuatorWarning.MainCalibrationError[i];
		this->eventForceActuatorWarning.AnyBackupCalibrationError = this->eventForceActuatorWarning.AnyBackupCalibrationError ||
				this->eventForceActuatorWarning.BackupCalibrationError[i];
		this->eventForceActuatorWarning.AnyMezzanineError = this->eventForceActuatorWarning.AnyMezzanineError ||
				this->eventForceActuatorWarning.MezzanineError[i];
		this->eventForceActuatorWarning.AnyMezzanineBootloaderActive = this->eventForceActuatorWarning.AnyMezzanineBootloaderActive ||
				this->eventForceActuatorWarning.MezzanineBootloaderActive[i];
		this->eventForceActuatorWarning.AnyUniqueIdCRCError = this->eventForceActuatorWarning.AnyUniqueIdCRCError ||
				this->eventForceActuatorWarning.UniqueIdCRCError[i];
		this->eventForceActuatorWarning.AnyApplicationTypeMismatch = this->eventForceActuatorWarning.AnyApplicationTypeMismatch ||
				this->eventForceActuatorWarning.ApplicationTypeMismatch[i];
		this->eventForceActuatorWarning.AnyApplicationMissing = this->eventForceActuatorWarning.AnyApplicationMissing ||
				this->eventForceActuatorWarning.ApplicationMissing[i];
		this->eventForceActuatorWarning.AnyApplicationCRCMismatch = this->eventForceActuatorWarning.AnyApplicationCRCMismatch ||
				this->eventForceActuatorWarning.ApplicationCRCMismatch[i];
		this->eventForceActuatorWarning.AnyOneWireMissing = this->eventForceActuatorWarning.AnyOneWireMissing ||
				this->eventForceActuatorWarning.OneWireMissing[i];
		this->eventForceActuatorWarning.AnyOneWire1Mismatch = this->eventForceActuatorWarning.AnyOneWire1Mismatch ||
				this->eventForceActuatorWarning.OneWire1Mismatch[i];
		this->eventForceActuatorWarning.AnyOneWire2Mismatch = this->eventForceActuatorWarning.AnyOneWire2Mismatch ||
				this->eventForceActuatorWarning.OneWire2Mismatch[i];
		this->eventForceActuatorWarning.AnyWatchdogReset = this->eventForceActuatorWarning.AnyWatchdogReset ||
				this->eventForceActuatorWarning.WatchdogReset[i];
		this->eventForceActuatorWarning.AnyBrownOut = this->eventForceActuatorWarning.AnyBrownOut ||
				this->eventForceActuatorWarning.BrownOut[i];
		this->eventForceActuatorWarning.AnyEventTrapReset = this->eventForceActuatorWarning.AnyEventTrapReset ||
				this->eventForceActuatorWarning.EventTrapReset[i];
		this->eventForceActuatorWarning.AnySSRPowerFault = this->eventForceActuatorWarning.AnySSRPowerFault ||
				this->eventForceActuatorWarning.SSRPowerFault[i];
		this->eventForceActuatorWarning.AnyAuxPowerFault = this->eventForceActuatorWarning.AnyAuxPowerFault ||
				this->eventForceActuatorWarning.AuxPowerFault[i];
		this->eventForceActuatorWarning.AnyMezzaninePowerFault = this->eventForceActuatorWarning.AnyMezzaninePowerFault ||
				this->eventForceActuatorWarning.MezzaninePowerFault[i];
		this->eventForceActuatorWarning.AnyMezzanineCurrentAmp1Fault = this->eventForceActuatorWarning.AnyMezzanineCurrentAmp1Fault ||
				this->eventForceActuatorWarning.MezzanineCurrentAmp1Fault[i];
		this->eventForceActuatorWarning.AnyMezzanineCurrentAmp2Fault = this->eventForceActuatorWarning.AnyMezzanineCurrentAmp2Fault ||
				this->eventForceActuatorWarning.MezzanineCurrentAmp2Fault[i];
		this->eventForceActuatorWarning.AnyMezzanineUniqueIdCRCError = this->eventForceActuatorWarning.AnyMezzanineUniqueIdCRCError ||
				this->eventForceActuatorWarning.MezzanineUniqueIdCRCError[i];
		this->eventForceActuatorWarning.AnyMezzanineMainCalibrationError = this->eventForceActuatorWarning.AnyMezzanineMainCalibrationError ||
				this->eventForceActuatorWarning.MezzanineMainCalibrationError[i];
		this->eventForceActuatorWarning.AnyMezzanineBackupCalibrationError = this->eventForceActuatorWarning.AnyMezzanineBackupCalibrationError ||
				this->eventForceActuatorWarning.MezzanineBackupCalibrationError[i];
		this->eventForceActuatorWarning.AnyMezzanineEventTrapReset = this->eventForceActuatorWarning.AnyMezzanineEventTrapReset ||
				this->eventForceActuatorWarning.MezzanineEventTrapReset[i];
		this->eventForceActuatorWarning.AnyMezzanineApplicationMissing = this->eventForceActuatorWarning.AnyMezzanineApplicationMissing ||
				this->eventForceActuatorWarning.MezzanineApplicationMissing[i];
		this->eventForceActuatorWarning.AnyMezzanineApplicationCRCMismatch = this->eventForceActuatorWarning.AnyMezzanineApplicationCRCMismatch ||
				this->eventForceActuatorWarning.MezzanineApplicationCRCMismatch[i];
		this->eventForceActuatorWarning.AnyILCFault = this->eventForceActuatorWarning.AnyILCFault ||
				this->eventForceActuatorWarning.ILCFault[i];
		this->eventForceActuatorWarning.AnyBroadcastCounterWarning = this->eventForceActuatorWarning.AnyBroadcastCounterWarning ||
				this->eventForceActuatorWarning.BroadcastCounterWarning[i];
	}
	this->eventForceActuatorWarning.AnyWarning =
		this->eventForceActuatorWarning.AnyMajorFault ||
		this->eventForceActuatorWarning.AnyMinorFault ||
		this->eventForceActuatorWarning.AnyFaultOverride ||
		this->eventForceActuatorWarning.AnyMainCalibrationError ||
		this->eventForceActuatorWarning.AnyBackupCalibrationError ||
		this->eventForceActuatorWarning.AnyMezzanineError ||
		this->eventForceActuatorWarning.AnyMezzanineBootloaderActive ||
		this->eventForceActuatorWarning.AnyUniqueIdCRCError ||
		this->eventForceActuatorWarning.AnyApplicationTypeMismatch ||
		this->eventForceActuatorWarning.AnyApplicationMissing ||
		this->eventForceActuatorWarning.AnyApplicationCRCMismatch ||
		this->eventForceActuatorWarning.AnyOneWireMissing ||
		this->eventForceActuatorWarning.AnyOneWire1Mismatch ||
		this->eventForceActuatorWarning.AnyOneWire2Mismatch ||
		this->eventForceActuatorWarning.AnyWatchdogReset ||
		this->eventForceActuatorWarning.AnyBrownOut ||
		this->eventForceActuatorWarning.AnyEventTrapReset ||
		this->eventForceActuatorWarning.AnySSRPowerFault ||
		this->eventForceActuatorWarning.AnyAuxPowerFault ||
		this->eventForceActuatorWarning.AnyMezzaninePowerFault ||
		this->eventForceActuatorWarning.AnyMezzanineCurrentAmp1Fault ||
		this->eventForceActuatorWarning.AnyMezzanineCurrentAmp2Fault ||
		this->eventForceActuatorWarning.AnyMezzanineUniqueIdCRCError ||
		this->eventForceActuatorWarning.AnyMezzanineMainCalibrationError ||
		this->eventForceActuatorWarning.AnyMezzanineBackupCalibrationError ||
		this->eventForceActuatorWarning.AnyMezzanineEventTrapReset ||
		this->eventForceActuatorWarning.AnyMezzanineApplicationMissing ||
		this->eventForceActuatorWarning.AnyMezzanineApplicationCRCMismatch ||
		this->eventForceActuatorWarning.AnyILCFault ||
		this->eventForceActuatorWarning.AnyBroadcastCounterWarning;
	this->m1m3SAL->logEvent_ForceActuatorWarning(&this->eventForceActuatorWarning, 0);
	this->previousEventForceActuatorWarning = this->eventForceActuatorWarning;
}

void M1M3SSPublisher::tryLogForceActuatorWarning() {
	bool changeDetected = false;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventForceActuatorWarning.MajorFault[i] != this->previousEventForceActuatorWarning.MajorFault[i] ||
			this->eventForceActuatorWarning.MinorFault[i] != this->previousEventForceActuatorWarning.MinorFault[i] ||
			this->eventForceActuatorWarning.FaultOverride[i] != this->previousEventForceActuatorWarning.FaultOverride[i] ||
			this->eventForceActuatorWarning.MainCalibrationError[i] != this->previousEventForceActuatorWarning.MainCalibrationError[i] ||
			this->eventForceActuatorWarning.BackupCalibrationError[i] != this->previousEventForceActuatorWarning.BackupCalibrationError[i] ||
			this->eventForceActuatorWarning.MezzanineError[i] != this->previousEventForceActuatorWarning.MezzanineError[i] ||
			this->eventForceActuatorWarning.MezzanineBootloaderActive[i] != this->previousEventForceActuatorWarning.MezzanineBootloaderActive[i] ||
			this->eventForceActuatorWarning.UniqueIdCRCError[i] != this->previousEventForceActuatorWarning.UniqueIdCRCError[i] ||
			this->eventForceActuatorWarning.ApplicationTypeMismatch[i] != this->previousEventForceActuatorWarning.ApplicationTypeMismatch[i] ||
			this->eventForceActuatorWarning.ApplicationMissing[i] != this->previousEventForceActuatorWarning.ApplicationMissing[i] ||
			this->eventForceActuatorWarning.ApplicationCRCMismatch[i] != this->previousEventForceActuatorWarning.ApplicationCRCMismatch[i] ||
			this->eventForceActuatorWarning.OneWireMissing[i] != this->previousEventForceActuatorWarning.OneWireMissing[i] ||
			this->eventForceActuatorWarning.OneWire1Mismatch[i] != this->previousEventForceActuatorWarning.OneWire1Mismatch[i] ||
			this->eventForceActuatorWarning.OneWire2Mismatch[i] != this->previousEventForceActuatorWarning.OneWire2Mismatch[i] ||
			this->eventForceActuatorWarning.WatchdogReset[i] != this->previousEventForceActuatorWarning.WatchdogReset[i] ||
			this->eventForceActuatorWarning.BrownOut[i] != this->previousEventForceActuatorWarning.BrownOut[i] ||
			this->eventForceActuatorWarning.EventTrapReset[i] != this->previousEventForceActuatorWarning.EventTrapReset[i] ||
			this->eventForceActuatorWarning.SSRPowerFault[i] != this->previousEventForceActuatorWarning.SSRPowerFault[i] ||
			this->eventForceActuatorWarning.AuxPowerFault[i] != this->previousEventForceActuatorWarning.AuxPowerFault[i] ||
			this->eventForceActuatorWarning.MezzaninePowerFault[i] != this->previousEventForceActuatorWarning.MezzaninePowerFault[i] ||
			this->eventForceActuatorWarning.MezzanineCurrentAmp1Fault[i] != this->previousEventForceActuatorWarning.MezzanineCurrentAmp1Fault[i] ||
			this->eventForceActuatorWarning.MezzanineCurrentAmp2Fault[i] != this->previousEventForceActuatorWarning.MezzanineCurrentAmp2Fault[i] ||
			this->eventForceActuatorWarning.MezzanineUniqueIdCRCError[i] != this->previousEventForceActuatorWarning.MezzanineUniqueIdCRCError[i] ||
			this->eventForceActuatorWarning.MezzanineMainCalibrationError[i] != this->previousEventForceActuatorWarning.MezzanineMainCalibrationError[i] ||
			this->eventForceActuatorWarning.MezzanineBackupCalibrationError[i] != this->previousEventForceActuatorWarning.MezzanineBackupCalibrationError[i] ||
			this->eventForceActuatorWarning.MezzanineEventTrapReset[i] != this->previousEventForceActuatorWarning.MezzanineEventTrapReset[i] ||
			this->eventForceActuatorWarning.MezzanineApplicationMissing[i] != this->previousEventForceActuatorWarning.MezzanineApplicationMissing[i] ||
			this->eventForceActuatorWarning.MezzanineApplicationCRCMismatch[i] != this->previousEventForceActuatorWarning.MezzanineApplicationCRCMismatch[i] ||
			this->eventForceActuatorWarning.ILCFault[i] != this->previousEventForceActuatorWarning.ILCFault[i] ||
			this->eventForceActuatorWarning.BroadcastCounterWarning[i] != this->previousEventForceActuatorWarning.BroadcastCounterWarning[i];
	}
	if (changeDetected) {
		this->logForceActuatorForceWarning();
	}
}

void M1M3SSPublisher::logForceSetpointWarning() {
	for(int i = 0; i < FA_COUNT; ++i) {
		this->eventForceSetpointWarning.AnySafetyLimitWarning = this->eventForceSetpointWarning.AnySafetyLimitWarning ||
				this->eventForceSetpointWarning.SafetyLimitWarning[i];
		this->eventForceSetpointWarning.AnyNearNeighborWarning = this->eventForceSetpointWarning.AnyNearNeighborWarning ||
				this->eventForceSetpointWarning.NearNeighborWarning[i];
		this->eventForceSetpointWarning.AnyFarNeighborWarning = this->eventForceSetpointWarning.AnyFarNeighborWarning ||
				this->eventForceSetpointWarning.FarNeighborWarning[i];
		this->eventForceSetpointWarning.AnyElevationForceWarning = this->eventForceSetpointWarning.AnyElevationForceWarning ||
				this->eventForceSetpointWarning.ElevationForceWarning[i];
		this->eventForceSetpointWarning.AnyAzimuthForceWarning = this->eventForceSetpointWarning.AnyAzimuthForceWarning ||
				this->eventForceSetpointWarning.AzimuthForceWarning[i];
		this->eventForceSetpointWarning.AnyThermalForceWarning = this->eventForceSetpointWarning.AnyThermalForceWarning ||
				this->eventForceSetpointWarning.ThermalForceWarning[i];
		this->eventForceSetpointWarning.AnyBalanceForceWarning = this->eventForceSetpointWarning.AnyBalanceForceWarning ||
				this->eventForceSetpointWarning.BalanceForceWarning[i];
		this->eventForceSetpointWarning.AnyAccelerationForceWarning = this->eventForceSetpointWarning.AnyAccelerationForceWarning ||
				this->eventForceSetpointWarning.AccelerationForceWarning[i];
		this->eventForceSetpointWarning.AnyVelocityForceWarning = this->eventForceSetpointWarning.AnyVelocityForceWarning ||
				this->eventForceSetpointWarning.VelocityForceWarning[i];
		this->eventForceSetpointWarning.AnyActiveOpticForceWarning = this->eventForceSetpointWarning.AnyActiveOpticForceWarning ||
				this->eventForceSetpointWarning.ActiveOpticForceWarning[i];
		this->eventForceSetpointWarning.AnyStaticForceWarning = this->eventForceSetpointWarning.AnyStaticForceWarning ||
				this->eventForceSetpointWarning.StaticForceWarning[i];
		this->eventForceSetpointWarning.AnyAberrationForceWarning = this->eventForceSetpointWarning.AnyAberrationForceWarning ||
				this->eventForceSetpointWarning.AberrationForceWarning[i];
		this->eventForceSetpointWarning.AnyOffsetForceWarning = this->eventForceSetpointWarning.AnyOffsetForceWarning ||
				this->eventForceSetpointWarning.OffsetForceWarning[i];
	}
	this->eventForceSetpointWarning.AnyWarning =
		this->eventForceSetpointWarning.AnySafetyLimitWarning ||
		this->eventForceSetpointWarning.XMomentWarning ||
		this->eventForceSetpointWarning.YMomentWarning ||
		this->eventForceSetpointWarning.ZMomentWarning ||
		this->eventForceSetpointWarning.AnyNearNeighborWarning ||
		this->eventForceSetpointWarning.MagnitudeWarning ||
		this->eventForceSetpointWarning.AnyFarNeighborWarning ||
		this->eventForceSetpointWarning.AnyElevationForceWarning ||
		this->eventForceSetpointWarning.AnyAzimuthForceWarning ||
		this->eventForceSetpointWarning.AnyThermalForceWarning ||
		this->eventForceSetpointWarning.AnyBalanceForceWarning ||
		this->eventForceSetpointWarning.AnyVelocityForceWarning ||
		this->eventForceSetpointWarning.AnyAccelerationForceWarning ||
		this->eventForceSetpointWarning.ActiveOpticNetForceWarning ||
		this->eventForceSetpointWarning.AnyActiveOpticForceWarning ||
		this->eventForceSetpointWarning.AnyStaticForceWarning ||
		this->eventForceSetpointWarning.AberrationNetForceWarning ||
		this->eventForceSetpointWarning.AnyAberrationForceWarning ||
		this->eventForceSetpointWarning.AnyOffsetForceWarning;
	this->m1m3SAL->logEvent_ForceSetpointWarning(&this->eventForceSetpointWarning, 0);
	this->previousEventForceSetpointWarning = this->eventForceSetpointWarning;
}

void M1M3SSPublisher::tryLogForceSetpointWarning() {
	bool changeDetected =
		this->eventForceSetpointWarning.XMomentWarning != this->previousEventForceSetpointWarning.XMomentWarning ||
		this->eventForceSetpointWarning.YMomentWarning != this->previousEventForceSetpointWarning.YMomentWarning ||
		this->eventForceSetpointWarning.ZMomentWarning != this->previousEventForceSetpointWarning.ZMomentWarning ||
		this->eventForceSetpointWarning.MagnitudeWarning != this->previousEventForceSetpointWarning.MagnitudeWarning ||
		this->eventForceSetpointWarning.ActiveOpticNetForceWarning != this->previousEventForceSetpointWarning.ActiveOpticNetForceWarning ||
		this->eventForceSetpointWarning.AberrationNetForceWarning != this->previousEventForceSetpointWarning.AberrationNetForceWarning;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventForceSetpointWarning.SafetyLimitWarning[i] != this->previousEventForceSetpointWarning.SafetyLimitWarning[i] ||
			this->eventForceSetpointWarning.NearNeighborWarning[i] != this->previousEventForceSetpointWarning.NearNeighborWarning[i] ||
			this->eventForceSetpointWarning.FarNeighborWarning[i] != this->previousEventForceSetpointWarning.FarNeighborWarning[i] ||
			this->eventForceSetpointWarning.ElevationForceWarning[i] != this->previousEventForceSetpointWarning.ElevationForceWarning[i] ||
			this->eventForceSetpointWarning.AzimuthForceWarning[i] != this->previousEventForceSetpointWarning.AzimuthForceWarning[i] ||
			this->eventForceSetpointWarning.ThermalForceWarning[i] != this->previousEventForceSetpointWarning.ThermalForceWarning[i] ||
			this->eventForceSetpointWarning.BalanceForceWarning[i] != this->previousEventForceSetpointWarning.BalanceForceWarning[i] ||
			this->eventForceSetpointWarning.AccelerationForceWarning[i] != this->previousEventForceSetpointWarning.AccelerationForceWarning[i] ||
			this->eventForceSetpointWarning.VelocityForceWarning[i] != this->previousEventForceSetpointWarning.VelocityForceWarning[i] ||
			this->eventForceSetpointWarning.ActiveOpticForceWarning[i] != this->previousEventForceSetpointWarning.ActiveOpticForceWarning[i] ||
			this->eventForceSetpointWarning.StaticForceWarning[i] != this->previousEventForceSetpointWarning.StaticForceWarning[i] ||
			this->eventForceSetpointWarning.AberrationForceWarning[i] != this->previousEventForceSetpointWarning.AberrationForceWarning[i] ||
			this->eventForceSetpointWarning.OffsetForceWarning[i] != this->previousEventForceSetpointWarning.OffsetForceWarning[i];
	}
	if (changeDetected) {
		this->logForceSetpointWarning();
	}
}

void M1M3SSPublisher::logGyroWarning() {
	this->eventGyroWarning.AnyWarning =
		this->eventGyroWarning.GyroXStatusWarning ||
		this->eventGyroWarning.GyroYStatusWarning ||
		this->eventGyroWarning.GyroZStatusWarning ||
		this->eventGyroWarning.SequenceNumberWarning ||
		this->eventGyroWarning.CRCMismatchWarning ||
		this->eventGyroWarning.InvalidLengthWarning ||
		this->eventGyroWarning.InvalidHeaderWarning ||
		this->eventGyroWarning.IncompleteFrameWarning ||
		this->eventGyroWarning.GyroXSLDWarning ||
		this->eventGyroWarning.GyroXMODDACWarning ||
		this->eventGyroWarning.GyroXPhaseWarning ||
		this->eventGyroWarning.GyroXFlashWarning ||
		this->eventGyroWarning.GyroYSLDWarning ||
		this->eventGyroWarning.GyroYMODDACWarning ||
		this->eventGyroWarning.GyroYPhaseWarning ||
		this->eventGyroWarning.GyroYFlashWarning ||
		this->eventGyroWarning.GyroZSLDWarning ||
		this->eventGyroWarning.GyroZMODDACWarning ||
		this->eventGyroWarning.GyroZPhaseWarning ||
		this->eventGyroWarning.GyroZFlashWarning ||
		this->eventGyroWarning.GyroXSLDTemperatureStatusWarning ||
		this->eventGyroWarning.GyroYSLDTemperatureStatusWarning ||
		this->eventGyroWarning.GyroZSLDTemperatureStatusWarning ||
		this->eventGyroWarning.GCBTemperatureStatusWarning ||
		this->eventGyroWarning.TemperatureStatusWarning ||
		this->eventGyroWarning.GCBDSPSPIFlashStatusWarning ||
		this->eventGyroWarning.GCBFPGASPIFlashStatusWarning ||
		this->eventGyroWarning.DSPSPIFlashStatusWarning ||
		this->eventGyroWarning.FPGASPIFlashStatusWarning ||
		this->eventGyroWarning.GCB1_2VStatusWarning ||
		this->eventGyroWarning.GCB3_3VStatusWarning ||
		this->eventGyroWarning.GCB5VStatusWarning ||
		this->eventGyroWarning.V1_2StatusWarning ||
		this->eventGyroWarning.V3_3StatusWarning ||
		this->eventGyroWarning.V5StatusWarning ||
		this->eventGyroWarning.GCBFPGAStatusWarning ||
		this->eventGyroWarning.FPGAStatusWarning ||
		this->eventGyroWarning.HiSpeedSPORTStatusWarning ||
		this->eventGyroWarning.AuxSPORTStatusWarning ||
		this->eventGyroWarning.SufficientSoftwareResourcesWarning ||
		this->eventGyroWarning.GyroEOVoltsPositiveWarning ||
		this->eventGyroWarning.GyroEOVoltsNegativeWarning ||
		this->eventGyroWarning.GyroXVoltsWarning ||
		this->eventGyroWarning.GyroYVoltsWarning ||
		this->eventGyroWarning.GyroZVoltsWarning ||
		this->eventGyroWarning.GCBADCCommsWarning ||
		this->eventGyroWarning.MSYNCExternalTimingWarning;
	this->m1m3SAL->logEvent_GyroWarning(&this->eventGyroWarning, 0);
	this->previousEventGyroWarning = this->eventGyroWarning;
}

void M1M3SSPublisher::tryLogGyroWarning() {
	if (this->eventGyroWarning.GyroXStatusWarning != this->previousEventGyroWarning.GyroXStatusWarning ||
		this->eventGyroWarning.GyroYStatusWarning != this->previousEventGyroWarning.GyroYStatusWarning ||
		this->eventGyroWarning.GyroZStatusWarning != this->previousEventGyroWarning.GyroZStatusWarning ||
		this->eventGyroWarning.SequenceNumberWarning != this->previousEventGyroWarning.SequenceNumberWarning ||
		this->eventGyroWarning.CRCMismatchWarning != this->previousEventGyroWarning.CRCMismatchWarning ||
		this->eventGyroWarning.InvalidLengthWarning != this->previousEventGyroWarning.InvalidLengthWarning ||
		this->eventGyroWarning.InvalidHeaderWarning != this->previousEventGyroWarning.InvalidHeaderWarning ||
		this->eventGyroWarning.IncompleteFrameWarning != this->previousEventGyroWarning.IncompleteFrameWarning ||
		this->eventGyroWarning.GyroXSLDWarning != this->previousEventGyroWarning.GyroXSLDWarning ||
		this->eventGyroWarning.GyroXMODDACWarning != this->previousEventGyroWarning.GyroXMODDACWarning ||
		this->eventGyroWarning.GyroXPhaseWarning != this->previousEventGyroWarning.GyroXPhaseWarning ||
		this->eventGyroWarning.GyroXFlashWarning != this->previousEventGyroWarning.GyroXFlashWarning ||
		this->eventGyroWarning.GyroYSLDWarning != this->previousEventGyroWarning.GyroYSLDWarning ||
		this->eventGyroWarning.GyroYMODDACWarning != this->previousEventGyroWarning.GyroYMODDACWarning ||
		this->eventGyroWarning.GyroYPhaseWarning != this->previousEventGyroWarning.GyroYPhaseWarning ||
		this->eventGyroWarning.GyroYFlashWarning != this->previousEventGyroWarning.GyroYFlashWarning ||
		this->eventGyroWarning.GyroZSLDWarning != this->previousEventGyroWarning.GyroZSLDWarning ||
		this->eventGyroWarning.GyroZMODDACWarning != this->previousEventGyroWarning.GyroZMODDACWarning ||
		this->eventGyroWarning.GyroZPhaseWarning != this->previousEventGyroWarning.GyroZPhaseWarning ||
		this->eventGyroWarning.GyroZFlashWarning != this->previousEventGyroWarning.GyroZFlashWarning ||
		this->eventGyroWarning.GyroXSLDTemperatureStatusWarning != this->previousEventGyroWarning.GyroXSLDTemperatureStatusWarning ||
		this->eventGyroWarning.GyroYSLDTemperatureStatusWarning != this->previousEventGyroWarning.GyroYSLDTemperatureStatusWarning ||
		this->eventGyroWarning.GyroZSLDTemperatureStatusWarning != this->previousEventGyroWarning.GyroZSLDTemperatureStatusWarning ||
		this->eventGyroWarning.GCBTemperatureStatusWarning != this->previousEventGyroWarning.GCBTemperatureStatusWarning ||
		this->eventGyroWarning.TemperatureStatusWarning != this->previousEventGyroWarning.TemperatureStatusWarning ||
		this->eventGyroWarning.GCBDSPSPIFlashStatusWarning != this->previousEventGyroWarning.GCBDSPSPIFlashStatusWarning ||
		this->eventGyroWarning.GCBFPGASPIFlashStatusWarning != this->previousEventGyroWarning.GCBFPGASPIFlashStatusWarning ||
		this->eventGyroWarning.DSPSPIFlashStatusWarning != this->previousEventGyroWarning.DSPSPIFlashStatusWarning ||
		this->eventGyroWarning.FPGASPIFlashStatusWarning != this->previousEventGyroWarning.FPGASPIFlashStatusWarning ||
		this->eventGyroWarning.GCB1_2VStatusWarning != this->previousEventGyroWarning.GCB1_2VStatusWarning ||
		this->eventGyroWarning.GCB3_3VStatusWarning != this->previousEventGyroWarning.GCB3_3VStatusWarning ||
		this->eventGyroWarning.GCB5VStatusWarning != this->previousEventGyroWarning.GCB5VStatusWarning ||
		this->eventGyroWarning.V1_2StatusWarning != this->previousEventGyroWarning.V1_2StatusWarning ||
		this->eventGyroWarning.V3_3StatusWarning != this->previousEventGyroWarning.V3_3StatusWarning ||
		this->eventGyroWarning.V5StatusWarning != this->previousEventGyroWarning.V5StatusWarning ||
		this->eventGyroWarning.GCBFPGAStatusWarning != this->previousEventGyroWarning.GCBFPGAStatusWarning ||
		this->eventGyroWarning.FPGAStatusWarning != this->previousEventGyroWarning.FPGAStatusWarning ||
		this->eventGyroWarning.HiSpeedSPORTStatusWarning != this->previousEventGyroWarning.HiSpeedSPORTStatusWarning ||
		this->eventGyroWarning.AuxSPORTStatusWarning != this->previousEventGyroWarning.AuxSPORTStatusWarning ||
		this->eventGyroWarning.SufficientSoftwareResourcesWarning != this->previousEventGyroWarning.SufficientSoftwareResourcesWarning ||
		this->eventGyroWarning.GyroEOVoltsPositiveWarning != this->previousEventGyroWarning.GyroEOVoltsPositiveWarning ||
		this->eventGyroWarning.GyroEOVoltsNegativeWarning != this->previousEventGyroWarning.GyroEOVoltsNegativeWarning ||
		this->eventGyroWarning.GyroXVoltsWarning != this->previousEventGyroWarning.GyroXVoltsWarning ||
		this->eventGyroWarning.GyroYVoltsWarning != this->previousEventGyroWarning.GyroYVoltsWarning ||
		this->eventGyroWarning.GyroZVoltsWarning != this->previousEventGyroWarning.GyroZVoltsWarning ||
		this->eventGyroWarning.GCBADCCommsWarning != this->previousEventGyroWarning.GCBADCCommsWarning ||
		this->eventGyroWarning.MSYNCExternalTimingWarning != this->previousEventGyroWarning.MSYNCExternalTimingWarning) {
		this->logGyroWarning();
	}
}

void M1M3SSPublisher::logHardpointActuatorInfo() {
	this->m1m3SAL->logEvent_HardpointActuatorInfo(&this->eventHardpointActuatorInfo, 0);
	this->previousEventHardpointActuatorInfo = this->eventHardpointActuatorInfo;
}

void M1M3SSPublisher::tryLogHardpointActuatorInfo() {
	bool changeDetected = false;
	for(int i = 0; i < HP_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventHardpointActuatorInfo.ReferenceId[i] != this->previousEventHardpointActuatorInfo.ReferenceId[i] ||
			this->eventHardpointActuatorInfo.ReferencePosition[i] != this->previousEventHardpointActuatorInfo.ReferencePosition[i] ||
			this->eventHardpointActuatorInfo.ModbusSubnet[i] != this->previousEventHardpointActuatorInfo.ModbusSubnet[i] ||
			this->eventHardpointActuatorInfo.ModbusAddress[i] != this->previousEventHardpointActuatorInfo.ModbusAddress[i] ||
			this->eventHardpointActuatorInfo.XPosition[i] != this->previousEventHardpointActuatorInfo.XPosition[i] ||
			this->eventHardpointActuatorInfo.YPosition[i] != this->previousEventHardpointActuatorInfo.YPosition[i] ||
			this->eventHardpointActuatorInfo.ZPosition[i] != this->previousEventHardpointActuatorInfo.ZPosition[i] ||
			this->eventHardpointActuatorInfo.ILCUniqueId[i] != this->previousEventHardpointActuatorInfo.ILCUniqueId[i] ||
			this->eventHardpointActuatorInfo.ILCApplicationType[i] != this->previousEventHardpointActuatorInfo.ILCApplicationType[i] ||
			this->eventHardpointActuatorInfo.NetworkNodeType[i] != this->previousEventHardpointActuatorInfo.NetworkNodeType[i] ||
			this->eventHardpointActuatorInfo.ILCSelectedOptions[i] != this->previousEventHardpointActuatorInfo.ILCSelectedOptions[i] ||
			this->eventHardpointActuatorInfo.NetworkNodeOptions[i] != this->previousEventHardpointActuatorInfo.NetworkNodeOptions[i] ||
			this->eventHardpointActuatorInfo.MajorRevision[i] != this->previousEventHardpointActuatorInfo.MajorRevision[i] ||
			this->eventHardpointActuatorInfo.MinorRevision[i] != this->previousEventHardpointActuatorInfo.MinorRevision[i] ||
			this->eventHardpointActuatorInfo.ADCScanRate[i] != this->previousEventHardpointActuatorInfo.ADCScanRate[i] ||
			this->eventHardpointActuatorInfo.MainLoadCellCoefficient[i] != this->previousEventHardpointActuatorInfo.MainLoadCellCoefficient[i] ||
			this->eventHardpointActuatorInfo.MainLoadCellOffset[i] != this->previousEventHardpointActuatorInfo.MainLoadCellOffset[i] ||
			this->eventHardpointActuatorInfo.MainLoadCellSensitivity[i] != this->previousEventHardpointActuatorInfo.MainLoadCellSensitivity[i] ||
			this->eventHardpointActuatorInfo.BackupLoadCellCoefficient[i] != this->previousEventHardpointActuatorInfo.BackupLoadCellCoefficient[i] ||
			this->eventHardpointActuatorInfo.BackupLoadCellOffset[i] != this->previousEventHardpointActuatorInfo.BackupLoadCellOffset[i] ||
			this->eventHardpointActuatorInfo.BackupLoadCellSensitivity[i] != this->previousEventHardpointActuatorInfo.BackupLoadCellSensitivity[i];
	}
	if (changeDetected) {
		this->logHardpointActuatorInfo();
	}
}

void M1M3SSPublisher::logHardpointActuatorState() {
	this->m1m3SAL->logEvent_HardpointActuatorState(&this->eventHardpointActuatorState, 0);
	this->previousEventHardpointActuatorState = this->eventHardpointActuatorState;
}

void M1M3SSPublisher::tryLogHardpointActuatorState() {
	bool changeDetected = false;
	for(int i = 0; i < HP_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventHardpointActuatorState.ILCState[i] != this->previousEventHardpointActuatorState.ILCState[i] ||
			this->eventHardpointActuatorState.MotionState[i] != this->previousEventHardpointActuatorState.MotionState[i];
	}
	if (changeDetected) {
		this->logHardpointActuatorState();
	}
}

void M1M3SSPublisher::logHardpointActuatorWarning() {
	for(int i = 0; i < HP_COUNT; ++i) {
		this->eventHardpointActuatorWarning.AnyMajorFault = this->eventHardpointActuatorWarning.AnyMajorFault ||
			this->eventHardpointActuatorWarning.MajorFault[i];
		this->eventHardpointActuatorWarning.AnyMinorFault = this->eventHardpointActuatorWarning.AnyMinorFault ||
			this->eventHardpointActuatorWarning.MinorFault[i];
		this->eventHardpointActuatorWarning.AnyFaultOverride = this->eventHardpointActuatorWarning.AnyFaultOverride ||
			this->eventHardpointActuatorWarning.FaultOverride[i];
		this->eventHardpointActuatorWarning.AnyMainCalibrationError = this->eventHardpointActuatorWarning.AnyMainCalibrationError ||
			this->eventHardpointActuatorWarning.MainCalibrationError[i];
		this->eventHardpointActuatorWarning.AnyBackupCalibrationError = this->eventHardpointActuatorWarning.AnyBackupCalibrationError ||
			this->eventHardpointActuatorWarning.BackupCalibrationError[i];
		this->eventHardpointActuatorWarning.AnyLimitSwitch1Operated = this->eventHardpointActuatorWarning.AnyLimitSwitch1Operated ||
			this->eventHardpointActuatorWarning.LimitSwitch1Operated[i];
		this->eventHardpointActuatorWarning.AnyLimitSwitch2Operated = this->eventHardpointActuatorWarning.AnyLimitSwitch2Operated ||
			this->eventHardpointActuatorWarning.LimitSwitch2Operated[i];
		this->eventHardpointActuatorWarning.AnyUniqueIdCRCError = this->eventHardpointActuatorWarning.AnyUniqueIdCRCError ||
			this->eventHardpointActuatorWarning.UniqueIdCRCError[i];
		this->eventHardpointActuatorWarning.AnyApplicationTypeMismatch = this->eventHardpointActuatorWarning.AnyApplicationTypeMismatch ||
			this->eventHardpointActuatorWarning.ApplicationTypeMismatch[i];
		this->eventHardpointActuatorWarning.AnyApplicationMissing = this->eventHardpointActuatorWarning.AnyApplicationMissing ||
			this->eventHardpointActuatorWarning.ApplicationMissing[i];
		this->eventHardpointActuatorWarning.AnyApplicationCRCMismatch = this->eventHardpointActuatorWarning.AnyApplicationCRCMismatch ||
			this->eventHardpointActuatorWarning.ApplicationCRCMismatch[i];
		this->eventHardpointActuatorWarning.AnyOneWireMissing = this->eventHardpointActuatorWarning.AnyOneWireMissing ||
			this->eventHardpointActuatorWarning.OneWireMissing[i];
		this->eventHardpointActuatorWarning.AnyOneWire1Mismatch = this->eventHardpointActuatorWarning.AnyOneWire1Mismatch ||
			this->eventHardpointActuatorWarning.OneWire1Mismatch[i];
		this->eventHardpointActuatorWarning.AnyOneWire2Mismatch = this->eventHardpointActuatorWarning.AnyOneWire2Mismatch ||
			this->eventHardpointActuatorWarning.OneWire2Mismatch[i];
		this->eventHardpointActuatorWarning.AnyWatchdogReset = this->eventHardpointActuatorWarning.AnyWatchdogReset ||
			this->eventHardpointActuatorWarning.WatchdogReset[i];
		this->eventHardpointActuatorWarning.AnyBrownOut = this->eventHardpointActuatorWarning.AnyBrownOut ||
			this->eventHardpointActuatorWarning.BrownOut[i];
		this->eventHardpointActuatorWarning.AnyEventTrapReset = this->eventHardpointActuatorWarning.AnyEventTrapReset ||
			this->eventHardpointActuatorWarning.EventTrapReset[i];
		this->eventHardpointActuatorWarning.AnyMotorDriverFault = this->eventHardpointActuatorWarning.AnyMotorDriverFault ||
			this->eventHardpointActuatorWarning.MotorDriverFault[i];
		this->eventHardpointActuatorWarning.AnySSRPowerFault = this->eventHardpointActuatorWarning.AnySSRPowerFault ||
			this->eventHardpointActuatorWarning.SSRPowerFault[i];
		this->eventHardpointActuatorWarning.AnyAuxPowerFault = this->eventHardpointActuatorWarning.AnyAuxPowerFault ||
			this->eventHardpointActuatorWarning.AuxPowerFault[i];
		this->eventHardpointActuatorWarning.AnySMCPowerFault = this->eventHardpointActuatorWarning.AnySMCPowerFault ||
			this->eventHardpointActuatorWarning.SMCPowerFault[i];
		this->eventHardpointActuatorWarning.AnyILCFault = this->eventHardpointActuatorWarning.AnyILCFault ||
			this->eventHardpointActuatorWarning.ILCFault[i];
		this->eventHardpointActuatorWarning.AnyBroadcastCounterWarning = this->eventHardpointActuatorWarning.AnyBroadcastCounterWarning ||
			this->eventHardpointActuatorWarning.BroadcastCounterWarning[i];
	}
	this->eventHardpointActuatorWarning.AnyWarning =
		this->eventHardpointActuatorWarning.AnyMajorFault ||
		this->eventHardpointActuatorWarning.AnyMinorFault ||
		this->eventHardpointActuatorWarning.AnyFaultOverride ||
		this->eventHardpointActuatorWarning.AnyMainCalibrationError ||
		this->eventHardpointActuatorWarning.AnyBackupCalibrationError ||
		this->eventHardpointActuatorWarning.AnyLimitSwitch1Operated ||
		this->eventHardpointActuatorWarning.AnyLimitSwitch2Operated ||
		this->eventHardpointActuatorWarning.AnyUniqueIdCRCError ||
		this->eventHardpointActuatorWarning.AnyApplicationTypeMismatch ||
		this->eventHardpointActuatorWarning.AnyApplicationMissing ||
		this->eventHardpointActuatorWarning.AnyApplicationCRCMismatch ||
		this->eventHardpointActuatorWarning.AnyOneWireMissing ||
		this->eventHardpointActuatorWarning.AnyOneWire1Mismatch ||
		this->eventHardpointActuatorWarning.AnyOneWire2Mismatch ||
		this->eventHardpointActuatorWarning.AnyWatchdogReset ||
		this->eventHardpointActuatorWarning.AnyBrownOut ||
		this->eventHardpointActuatorWarning.AnyEventTrapReset ||
		this->eventHardpointActuatorWarning.AnyMotorDriverFault ||
		this->eventHardpointActuatorWarning.AnySSRPowerFault ||
		this->eventHardpointActuatorWarning.AnyAuxPowerFault ||
		this->eventHardpointActuatorWarning.AnySMCPowerFault ||
		this->eventHardpointActuatorWarning.AnyILCFault ||
		this->eventHardpointActuatorWarning.AnyBroadcastCounterWarning;
	this->m1m3SAL->logEvent_HardpointActuatorWarning(&this->eventHardpointActuatorWarning, 0);
	this->previousEventHardpointActuatorWarning = this->eventHardpointActuatorWarning;
}

void M1M3SSPublisher::tryLogHardpointActuatorWarning() {
	bool changeDetected = false;
	for(int i = 0; i < HP_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventHardpointActuatorWarning.MajorFault[i] != this->previousEventHardpointActuatorWarning.MajorFault[i] ||
			this->eventHardpointActuatorWarning.MinorFault[i] != this->previousEventHardpointActuatorWarning.MinorFault[i] ||
			this->eventHardpointActuatorWarning.FaultOverride[i] != this->previousEventHardpointActuatorWarning.FaultOverride[i] ||
			this->eventHardpointActuatorWarning.MainCalibrationError[i] != this->previousEventHardpointActuatorWarning.MainCalibrationError[i] ||
			this->eventHardpointActuatorWarning.BackupCalibrationError[i] != this->previousEventHardpointActuatorWarning.BackupCalibrationError[i] ||
			this->eventHardpointActuatorWarning.LimitSwitch1Operated[i] != this->previousEventHardpointActuatorWarning.LimitSwitch1Operated[i] ||
			this->eventHardpointActuatorWarning.LimitSwitch2Operated[i] != this->previousEventHardpointActuatorWarning.LimitSwitch2Operated[i] ||
			this->eventHardpointActuatorWarning.UniqueIdCRCError[i] != this->previousEventHardpointActuatorWarning.UniqueIdCRCError[i] ||
			this->eventHardpointActuatorWarning.ApplicationTypeMismatch[i] != this->previousEventHardpointActuatorWarning.ApplicationTypeMismatch[i] ||
			this->eventHardpointActuatorWarning.ApplicationMissing[i] != this->previousEventHardpointActuatorWarning.ApplicationMissing[i] ||
			this->eventHardpointActuatorWarning.ApplicationCRCMismatch[i] != this->previousEventHardpointActuatorWarning.ApplicationCRCMismatch[i] ||
			this->eventHardpointActuatorWarning.OneWireMissing[i] != this->previousEventHardpointActuatorWarning.OneWireMissing[i] ||
			this->eventHardpointActuatorWarning.OneWire1Mismatch[i] != this->previousEventHardpointActuatorWarning.OneWire1Mismatch[i] ||
			this->eventHardpointActuatorWarning.OneWire2Mismatch[i] != this->previousEventHardpointActuatorWarning.OneWire2Mismatch[i] ||
			this->eventHardpointActuatorWarning.WatchdogReset[i] != this->previousEventHardpointActuatorWarning.WatchdogReset[i] ||
			this->eventHardpointActuatorWarning.BrownOut[i] != this->previousEventHardpointActuatorWarning.BrownOut[i] ||
			this->eventHardpointActuatorWarning.EventTrapReset[i] != this->previousEventHardpointActuatorWarning.EventTrapReset[i] ||
			this->eventHardpointActuatorWarning.MotorDriverFault[i] != this->previousEventHardpointActuatorWarning.MotorDriverFault[i] ||
			this->eventHardpointActuatorWarning.SSRPowerFault[i] != this->previousEventHardpointActuatorWarning.SSRPowerFault[i] ||
			this->eventHardpointActuatorWarning.AuxPowerFault[i] != this->previousEventHardpointActuatorWarning.AuxPowerFault[i] ||
			this->eventHardpointActuatorWarning.SMCPowerFault[i] != this->previousEventHardpointActuatorWarning.SMCPowerFault[i] ||
			this->eventHardpointActuatorWarning.ILCFault[i] != this->previousEventHardpointActuatorWarning.ILCFault[i] ||
			this->eventHardpointActuatorWarning.BroadcastCounterWarning[i] != this->previousEventHardpointActuatorWarning.BroadcastCounterWarning[i];
	}
	if (changeDetected) {
		this->logHardpointActuatorWarning();
	}
}

void M1M3SSPublisher::logHardpointMonitorInfo() {
	this->m1m3SAL->logEvent_HardpointMonitorInfo(&this->eventHardpointMonitorInfo, 0);
	this->previousEventHardpointMonitorInfo = this->eventHardpointMonitorInfo;
}

void M1M3SSPublisher::tryLogHardpointMonitorInfo() {
	bool changeDetected = false;
	for(int i = 0; i < HM_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventHardpointMonitorInfo.ReferenceId[i] != this->previousEventHardpointMonitorInfo.ReferenceId[i] ||
			this->eventHardpointMonitorInfo.ModbusSubnet[i] != this->previousEventHardpointMonitorInfo.ModbusSubnet[i] ||
			this->eventHardpointMonitorInfo.ModbusAddress[i] != this->previousEventHardpointMonitorInfo.ModbusAddress[i] ||
			this->eventHardpointMonitorInfo.ILCUniqueId[i] != this->previousEventHardpointMonitorInfo.ILCUniqueId[i] ||
			this->eventHardpointMonitorInfo.ILCApplicationType[i] != this->previousEventHardpointMonitorInfo.ILCApplicationType[i] ||
			this->eventHardpointMonitorInfo.NetworkNodeType[i] != this->previousEventHardpointMonitorInfo.NetworkNodeType[i] ||
			this->eventHardpointMonitorInfo.MajorRevision[i] != this->previousEventHardpointMonitorInfo.MajorRevision[i] ||
			this->eventHardpointMonitorInfo.MinorRevision[i] != this->previousEventHardpointMonitorInfo.MinorRevision[i] ||
			this->eventHardpointMonitorInfo.MezzanineUniqueId[i] != this->previousEventHardpointMonitorInfo.MezzanineUniqueId[i] ||
			this->eventHardpointMonitorInfo.MezzanineFirmwareType[i] != this->previousEventHardpointMonitorInfo.MezzanineFirmwareType[i] ||
			this->eventHardpointMonitorInfo.MezzanineMajorRevision[i] != this->previousEventHardpointMonitorInfo.MezzanineMajorRevision[i] ||
			this->eventHardpointMonitorInfo.MezzanineMinorRevision[i] != this->previousEventHardpointMonitorInfo.MezzanineMinorRevision[i];
	}
	if (changeDetected) {
		this->logHardpointMonitorInfo();
	}
}

void M1M3SSPublisher::logHardpointMonitorState() {
	this->m1m3SAL->logEvent_HardpointMonitorState(&this->eventHardpointMonitorState, 0);
	this->previousEventHardpointMonitorState = this->eventHardpointMonitorState;
}

void M1M3SSPublisher::tryLogHardpointMonitorState() {
	bool changeDetected = false;
	for(int i = 0; i < HM_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventHardpointMonitorState.ILCState[i] != this->previousEventHardpointMonitorState.ILCState[i];
	}
	if (changeDetected) {
		this->logHardpointMonitorState();
	}
}

void M1M3SSPublisher::logHardpointMonitorWarning() {
	for(int i = 0; i < HM_COUNT; ++i) {
		this->eventHardpointMonitorWarning.AnyMajorFault = this->eventHardpointMonitorWarning.AnyMajorFault ||
			this->eventHardpointMonitorWarning.MajorFault[i];
		this->eventHardpointMonitorWarning.AnyMinorFault = this->eventHardpointMonitorWarning.AnyMinorFault ||
			this->eventHardpointMonitorWarning.MinorFault[i];
		this->eventHardpointMonitorWarning.AnyFaultOverride = this->eventHardpointMonitorWarning.AnyFaultOverride ||
			this->eventHardpointMonitorWarning.FaultOverride[i];
		this->eventHardpointMonitorWarning.AnyInstrumentError = this->eventHardpointMonitorWarning.AnyInstrumentError ||
			this->eventHardpointMonitorWarning.InstrumentError[i];
		this->eventHardpointMonitorWarning.AnyMezzanineError = this->eventHardpointMonitorWarning.AnyMezzanineError ||
			this->eventHardpointMonitorWarning.MezzanineError[i];
		this->eventHardpointMonitorWarning.AnyMezzanineBootloaderActive = this->eventHardpointMonitorWarning.AnyMezzanineBootloaderActive ||
			this->eventHardpointMonitorWarning.MezzanineBootloaderActive[i];
		this->eventHardpointMonitorWarning.AnyUniqueIdCRCError = this->eventHardpointMonitorWarning.AnyUniqueIdCRCError ||
			this->eventHardpointMonitorWarning.UniqueIdCRCError[i];
		this->eventHardpointMonitorWarning.AnyApplicationTypeMismatch = this->eventHardpointMonitorWarning.AnyApplicationTypeMismatch ||
			this->eventHardpointMonitorWarning.ApplicationTypeMismatch[i];
		this->eventHardpointMonitorWarning.AnyApplicationMissing = this->eventHardpointMonitorWarning.AnyApplicationMissing ||
			this->eventHardpointMonitorWarning.ApplicationMissing[i];
		this->eventHardpointMonitorWarning.AnyApplicationCRCMismatch = this->eventHardpointMonitorWarning.AnyApplicationCRCMismatch ||
			this->eventHardpointMonitorWarning.ApplicationCRCMismatch[i];
		this->eventHardpointMonitorWarning.AnyOneWireMissing = this->eventHardpointMonitorWarning.AnyOneWireMissing ||
			this->eventHardpointMonitorWarning.OneWireMissing[i];
		this->eventHardpointMonitorWarning.AnyOneWire1Mismatch = this->eventHardpointMonitorWarning.AnyOneWire1Mismatch ||
			this->eventHardpointMonitorWarning.OneWire1Mismatch[i];
		this->eventHardpointMonitorWarning.AnyOneWire2Mismatch = this->eventHardpointMonitorWarning.AnyOneWire2Mismatch ||
			this->eventHardpointMonitorWarning.OneWire2Mismatch[i];
		this->eventHardpointMonitorWarning.AnyWatchdogReset = this->eventHardpointMonitorWarning.AnyWatchdogReset ||
			this->eventHardpointMonitorWarning.WatchdogReset[i];
		this->eventHardpointMonitorWarning.AnyBrownOut = this->eventHardpointMonitorWarning.AnyBrownOut ||
			this->eventHardpointMonitorWarning.BrownOut[i];
		this->eventHardpointMonitorWarning.AnyEventTrapReset = this->eventHardpointMonitorWarning.AnyEventTrapReset ||
			this->eventHardpointMonitorWarning.EventTrapReset[i];
		this->eventHardpointMonitorWarning.AnySSRPowerFault = this->eventHardpointMonitorWarning.AnySSRPowerFault ||
			this->eventHardpointMonitorWarning.SSRPowerFault[i];
		this->eventHardpointMonitorWarning.AnyAuxPowerFault = this->eventHardpointMonitorWarning.AnyAuxPowerFault ||
			this->eventHardpointMonitorWarning.AuxPowerFault[i];
		this->eventHardpointMonitorWarning.AnyMezzanineS1AInterface1Fault = this->eventHardpointMonitorWarning.AnyMezzanineS1AInterface1Fault ||
			this->eventHardpointMonitorWarning.MezzanineS1AInterface1Fault[i];
		this->eventHardpointMonitorWarning.AnyMezzanineS1ALVDT1Fault = this->eventHardpointMonitorWarning.AnyMezzanineS1ALVDT1Fault ||
			this->eventHardpointMonitorWarning.MezzanineS1ALVDT1Fault[i];
		this->eventHardpointMonitorWarning.AnyMezzanineS1AInterface2Fault = this->eventHardpointMonitorWarning.AnyMezzanineS1AInterface2Fault ||
			this->eventHardpointMonitorWarning.MezzanineS1AInterface2Fault[i];
		this->eventHardpointMonitorWarning.AnyMezzanineS1ALVDT2Fault = this->eventHardpointMonitorWarning.AnyMezzanineS1ALVDT2Fault ||
			this->eventHardpointMonitorWarning.MezzanineS1ALVDT2Fault[i];
		this->eventHardpointMonitorWarning.AnyMezzanineUniqueIdCRCError = this->eventHardpointMonitorWarning.AnyMezzanineUniqueIdCRCError ||
			this->eventHardpointMonitorWarning.MezzanineUniqueIdCRCError[i];
		this->eventHardpointMonitorWarning.AnyMezzanineEventTrapReset = this->eventHardpointMonitorWarning.AnyMezzanineEventTrapReset ||
			this->eventHardpointMonitorWarning.MezzanineEventTrapReset[i];
		this->eventHardpointMonitorWarning.AnyMezzanineDCPRS422ChipFault = this->eventHardpointMonitorWarning.AnyMezzanineDCPRS422ChipFault ||
			this->eventHardpointMonitorWarning.MezzanineDCPRS422ChipFault[i];
		this->eventHardpointMonitorWarning.AnyMezzanineApplicationMissing = this->eventHardpointMonitorWarning.AnyMezzanineApplicationMissing ||
			this->eventHardpointMonitorWarning.MezzanineApplicationMissing[i];
		this->eventHardpointMonitorWarning.AnyMezzanineApplicationCRCMismatch = this->eventHardpointMonitorWarning.AnyMezzanineApplicationCRCMismatch ||
			this->eventHardpointMonitorWarning.MezzanineApplicationCRCMismatch[i];
	}
	this->eventHardpointMonitorWarning.AnyWarning =
		this->eventHardpointMonitorWarning.AnyMajorFault ||
		this->eventHardpointMonitorWarning.AnyMinorFault ||
		this->eventHardpointMonitorWarning.AnyFaultOverride ||
		this->eventHardpointMonitorWarning.AnyInstrumentError ||
		this->eventHardpointMonitorWarning.AnyMezzanineError ||
		this->eventHardpointMonitorWarning.AnyMezzanineBootloaderActive ||
		this->eventHardpointMonitorWarning.AnyUniqueIdCRCError ||
		this->eventHardpointMonitorWarning.AnyApplicationTypeMismatch ||
		this->eventHardpointMonitorWarning.AnyApplicationMissing ||
		this->eventHardpointMonitorWarning.AnyApplicationCRCMismatch ||
		this->eventHardpointMonitorWarning.AnyOneWireMissing ||
		this->eventHardpointMonitorWarning.AnyOneWire1Mismatch ||
		this->eventHardpointMonitorWarning.AnyOneWire2Mismatch ||
		this->eventHardpointMonitorWarning.AnyWatchdogReset ||
		this->eventHardpointMonitorWarning.AnyBrownOut ||
		this->eventHardpointMonitorWarning.AnyEventTrapReset ||
		this->eventHardpointMonitorWarning.AnySSRPowerFault ||
		this->eventHardpointMonitorWarning.AnyAuxPowerFault ||
		this->eventHardpointMonitorWarning.AnyMezzanineS1AInterface1Fault ||
		this->eventHardpointMonitorWarning.AnyMezzanineS1ALVDT1Fault ||
		this->eventHardpointMonitorWarning.AnyMezzanineS1AInterface2Fault ||
		this->eventHardpointMonitorWarning.AnyMezzanineS1ALVDT2Fault ||
		this->eventHardpointMonitorWarning.AnyMezzanineUniqueIdCRCError ||
		this->eventHardpointMonitorWarning.AnyMezzanineEventTrapReset ||
		this->eventHardpointMonitorWarning.AnyMezzanineDCPRS422ChipFault ||
		this->eventHardpointMonitorWarning.AnyMezzanineApplicationMissing ||
		this->eventHardpointMonitorWarning.AnyMezzanineApplicationCRCMismatch;
	this->m1m3SAL->logEvent_HardpointMonitorWarning(&this->eventHardpointMonitorWarning, 0);
	this->previousEventHardpointMonitorWarning = this->eventHardpointMonitorWarning;
}

void M1M3SSPublisher::tryLogHardpointMonitorWarning() {
	bool changeDetected = false;
	for(int i = 0; i < HM_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventHardpointMonitorWarning.MajorFault[i] != this->previousEventHardpointMonitorWarning.MajorFault[i] ||
			this->eventHardpointMonitorWarning.MinorFault[i] != this->previousEventHardpointMonitorWarning.MinorFault[i] ||
			this->eventHardpointMonitorWarning.FaultOverride[i] != this->previousEventHardpointMonitorWarning.FaultOverride[i] ||
			this->eventHardpointMonitorWarning.InstrumentError[i] != this->previousEventHardpointMonitorWarning.InstrumentError[i] ||
			this->eventHardpointMonitorWarning.MezzanineError[i] != this->previousEventHardpointMonitorWarning.MezzanineError[i] ||
			this->eventHardpointMonitorWarning.MezzanineBootloaderActive[i] != this->previousEventHardpointMonitorWarning.MezzanineBootloaderActive[i] ||
			this->eventHardpointMonitorWarning.UniqueIdCRCError[i] != this->previousEventHardpointMonitorWarning.UniqueIdCRCError[i] ||
			this->eventHardpointMonitorWarning.ApplicationTypeMismatch[i] != this->previousEventHardpointMonitorWarning.ApplicationTypeMismatch[i] ||
			this->eventHardpointMonitorWarning.ApplicationMissing[i] != this->previousEventHardpointMonitorWarning.ApplicationMissing[i] ||
			this->eventHardpointMonitorWarning.ApplicationCRCMismatch[i] != this->previousEventHardpointMonitorWarning.ApplicationCRCMismatch[i] ||
			this->eventHardpointMonitorWarning.OneWireMissing[i] != this->previousEventHardpointMonitorWarning.OneWireMissing[i] ||
			this->eventHardpointMonitorWarning.OneWire1Mismatch[i] != this->previousEventHardpointMonitorWarning.OneWire1Mismatch[i] ||
			this->eventHardpointMonitorWarning.OneWire2Mismatch[i] != this->previousEventHardpointMonitorWarning.OneWire2Mismatch[i] ||
			this->eventHardpointMonitorWarning.WatchdogReset[i] != this->previousEventHardpointMonitorWarning.WatchdogReset[i] ||
			this->eventHardpointMonitorWarning.BrownOut[i] != this->previousEventHardpointMonitorWarning.BrownOut[i] ||
			this->eventHardpointMonitorWarning.EventTrapReset[i] != this->previousEventHardpointMonitorWarning.EventTrapReset[i] ||
			this->eventHardpointMonitorWarning.SSRPowerFault[i] != this->previousEventHardpointMonitorWarning.SSRPowerFault[i] ||
			this->eventHardpointMonitorWarning.AuxPowerFault[i] != this->previousEventHardpointMonitorWarning.AuxPowerFault[i] ||
			this->eventHardpointMonitorWarning.MezzanineS1AInterface1Fault[i] != this->previousEventHardpointMonitorWarning.MezzanineS1AInterface1Fault[i] ||
			this->eventHardpointMonitorWarning.MezzanineS1ALVDT1Fault[i] != this->previousEventHardpointMonitorWarning.MezzanineS1ALVDT1Fault[i] ||
			this->eventHardpointMonitorWarning.MezzanineS1AInterface2Fault[i] != this->previousEventHardpointMonitorWarning.MezzanineS1AInterface2Fault[i] ||
			this->eventHardpointMonitorWarning.MezzanineS1ALVDT2Fault[i] != this->previousEventHardpointMonitorWarning.MezzanineS1ALVDT2Fault[i] ||
			this->eventHardpointMonitorWarning.MezzanineUniqueIdCRCError[i] != this->previousEventHardpointMonitorWarning.MezzanineUniqueIdCRCError[i] ||
			this->eventHardpointMonitorWarning.MezzanineEventTrapReset[i] != this->previousEventHardpointMonitorWarning.MezzanineEventTrapReset[i] ||
			this->eventHardpointMonitorWarning.MezzanineDCPRS422ChipFault[i] != this->previousEventHardpointMonitorWarning.MezzanineDCPRS422ChipFault[i] ||
			this->eventHardpointMonitorWarning.MezzanineApplicationMissing[i] != this->previousEventHardpointMonitorWarning.MezzanineApplicationMissing[i] ||
			this->eventHardpointMonitorWarning.MezzanineApplicationCRCMismatch[i] != this->previousEventHardpointMonitorWarning.MezzanineApplicationCRCMismatch[i];
	}
	if (changeDetected) {
		this->logHardpointMonitorWarning();
	}
}

void M1M3SSPublisher::logILCWarning() {
	this->eventILCWarning.AnyWarning =
		this->eventILCWarning.ResponseTimeout ||
		this->eventILCWarning.InvalidCRC ||
		this->eventILCWarning.IllegalFunction ||
		this->eventILCWarning.IllegalDataValue ||
		this->eventILCWarning.InvalidLength ||
		this->eventILCWarning.UnknownSubnet ||
		this->eventILCWarning.UnknownAddress ||
		this->eventILCWarning.UnknownFunction ||
		this->eventILCWarning.UnknownProblem;
	this->m1m3SAL->logEvent_ILCWarning(&this->eventILCWarning, 0);
	this->previousEventILCWarning = this->eventILCWarning;
}

void M1M3SSPublisher::tryLogILCWarning() {
	if (this->eventILCWarning.ActuatorId != this->previousEventILCWarning.ActuatorId ||
		this->eventILCWarning.ResponseTimeout != this->previousEventILCWarning.ResponseTimeout ||
		this->eventILCWarning.InvalidCRC != this->previousEventILCWarning.InvalidCRC ||
		this->eventILCWarning.IllegalFunction != this->previousEventILCWarning.IllegalFunction ||
		this->eventILCWarning.IllegalDataValue != this->previousEventILCWarning.IllegalDataValue ||
		this->eventILCWarning.InvalidLength != this->previousEventILCWarning.InvalidLength ||
		this->eventILCWarning.UnknownSubnet != this->previousEventILCWarning.UnknownSubnet ||
		this->eventILCWarning.UnknownAddress != this->previousEventILCWarning.UnknownAddress ||
		this->eventILCWarning.UnknownFunction != this->previousEventILCWarning.UnknownFunction ||
		this->eventILCWarning.UnknownProblem != this->previousEventILCWarning.UnknownProblem) {
		this->logILCWarning();
	}
}

void M1M3SSPublisher::logInclinometerSensorWarning() {
	this->eventInclinometerSensorWarning.AnyWarning =
		this->eventInclinometerSensorWarning.SensorReportsIllegalFunction ||
		this->eventInclinometerSensorWarning.SensorReportsIllegalDataAddress ||
		this->eventInclinometerSensorWarning.ResponseTimeout ||
		this->eventInclinometerSensorWarning.InvalidCRC ||
		this->eventInclinometerSensorWarning.InvalidLength ||
		this->eventInclinometerSensorWarning.UnknownAddress ||
		this->eventInclinometerSensorWarning.UnknownFunction ||
		this->eventInclinometerSensorWarning.UnknownProblem;
	this->m1m3SAL->logEvent_InclinometerSensorWarning(&this->eventInclinometerSensorWarning, 0);
	this->previousEventInclinometerSensorWarning = this->eventInclinometerSensorWarning;
}

void M1M3SSPublisher::tryLogInclinometerSensorWarning() {
	if (this->eventInclinometerSensorWarning.SensorReportsIllegalFunction != this->previousEventInclinometerSensorWarning.SensorReportsIllegalFunction ||
		this->eventInclinometerSensorWarning.SensorReportsIllegalDataAddress != this->previousEventInclinometerSensorWarning.SensorReportsIllegalDataAddress ||
		this->eventInclinometerSensorWarning.ResponseTimeout != this->previousEventInclinometerSensorWarning.ResponseTimeout ||
		this->eventInclinometerSensorWarning.InvalidCRC != this->previousEventInclinometerSensorWarning.InvalidCRC ||
		this->eventInclinometerSensorWarning.InvalidLength != this->previousEventInclinometerSensorWarning.InvalidLength ||
		this->eventInclinometerSensorWarning.UnknownAddress != this->previousEventInclinometerSensorWarning.UnknownAddress ||
		this->eventInclinometerSensorWarning.UnknownFunction != this->previousEventInclinometerSensorWarning.UnknownFunction ||
		this->eventInclinometerSensorWarning.UnknownProblem != this->previousEventInclinometerSensorWarning.UnknownProblem) {
		this->logInclinometerSensorWarning();
	}
}

void M1M3SSPublisher::logInterlockStatus() {
	this->m1m3SAL->logEvent_InterlockStatus(&this->eventInterlockStatus, 0);
	this->previousEventInterlockStatus = this->eventInterlockStatus;
}

void M1M3SSPublisher::tryLogInterlockStatus() {
	if (this->eventInterlockStatus.HeartbeatCommandedState != this->previousEventInterlockStatus.HeartbeatCommandedState ||
		this->eventInterlockStatus.HeartbeatOutputState != this->previousEventInterlockStatus.HeartbeatOutputState) {
		this->logInterlockStatus();
	}
}

void M1M3SSPublisher::logInterlockWarning() {
	this->eventInterlockWarning.AnyWarning =
		this->eventInterlockWarning.HeartbeatStateOutputMismatch ||
		this->eventInterlockWarning.AuxPowerNetworksOff ||
		this->eventInterlockWarning.ThermalEquipmentOff ||
		this->eventInterlockWarning.AirSupplyOff ||
		this->eventInterlockWarning.CabinetDoorOpen ||
		this->eventInterlockWarning.TMAMotionStop ||
		this->eventInterlockWarning.GISHeartbeatLost;
	this->m1m3SAL->logEvent_InterlockWarning(&this->eventInterlockWarning, 0);
	this->previousEventInterlockWarning = this->eventInterlockWarning;
}

void M1M3SSPublisher::tryLogInterlockWarning() {
	if (this->eventInterlockWarning.HeartbeatStateOutputMismatch != this->previousEventInterlockWarning.HeartbeatStateOutputMismatch ||
		this->eventInterlockWarning.AuxPowerNetworksOff != this->previousEventInterlockWarning.AuxPowerNetworksOff ||
		this->eventInterlockWarning.ThermalEquipmentOff != this->previousEventInterlockWarning.ThermalEquipmentOff ||
		this->eventInterlockWarning.AirSupplyOff != this->previousEventInterlockWarning.AirSupplyOff ||
		this->eventInterlockWarning.CabinetDoorOpen != this->previousEventInterlockWarning.CabinetDoorOpen ||
		this->eventInterlockWarning.TMAMotionStop != this->previousEventInterlockWarning.TMAMotionStop ||
		this->eventInterlockWarning.GISHeartbeatLost != this->previousEventInterlockWarning.GISHeartbeatLost) {
		this->logInterlockWarning();
	}
}

void M1M3SSPublisher::logPIDInfo() {
	this->m1m3SAL->logEvent_PIDInfo(&this->eventPIDInfo, 0);
	this->previousEventPIDInfo = this->eventPIDInfo;
}

void M1M3SSPublisher::tryLogPIDInfo() {
	bool changeDetected = false;
	for(int i = 0; i < 6 && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventPIDInfo.Timestep[i] != this->previousEventPIDInfo.Timestep[i] ||
			this->eventPIDInfo.P[i] != this->previousEventPIDInfo.P[i] ||
			this->eventPIDInfo.I[i] != this->previousEventPIDInfo.I[i] ||
			this->eventPIDInfo.D[i] != this->previousEventPIDInfo.D[i] ||
			this->eventPIDInfo.N[i] != this->previousEventPIDInfo.N[i] ||
			this->eventPIDInfo.CalculatedA[i] != this->previousEventPIDInfo.CalculatedA[i] ||
			this->eventPIDInfo.CalculatedB[i] != this->previousEventPIDInfo.CalculatedB[i] ||
			this->eventPIDInfo.CalculatedC[i] != this->previousEventPIDInfo.CalculatedC[i] ||
			this->eventPIDInfo.CalculatedD[i] != this->previousEventPIDInfo.CalculatedD[i] ||
			this->eventPIDInfo.CalculatedE[i] != this->previousEventPIDInfo.CalculatedE[i];
	}
	if (changeDetected) {
		this->logPIDInfo();
	}
}

void M1M3SSPublisher::logPowerStatus() {
	this->m1m3SAL->logEvent_PowerStatus(&this->eventPowerStatus, 0);
	this->previousEventPowerStatus = this->eventPowerStatus;
}

void M1M3SSPublisher::tryLogPowerStatus() {
	if (this->eventPowerStatus.PowerNetworkACommandedOn != this->previousEventPowerStatus.PowerNetworkACommandedOn ||
		this->eventPowerStatus.PowerNetworkAOutputOn != this->previousEventPowerStatus.PowerNetworkAOutputOn ||
		this->eventPowerStatus.PowerNetworkBCommandedOn != this->previousEventPowerStatus.PowerNetworkBCommandedOn ||
		this->eventPowerStatus.PowerNetworkBOutputOn != this->previousEventPowerStatus.PowerNetworkBOutputOn ||
		this->eventPowerStatus.PowerNetworkCCommandedOn != this->previousEventPowerStatus.PowerNetworkCCommandedOn ||
		this->eventPowerStatus.PowerNetworkCOutputOn != this->previousEventPowerStatus.PowerNetworkCOutputOn ||
		this->eventPowerStatus.PowerNetworkDCommandedOn != this->previousEventPowerStatus.PowerNetworkDCommandedOn ||
		this->eventPowerStatus.PowerNetworkDOutputOn != this->previousEventPowerStatus.PowerNetworkDOutputOn ||
		this->eventPowerStatus.AuxPowerNetworkACommandedOn != this->previousEventPowerStatus.AuxPowerNetworkACommandedOn ||
		this->eventPowerStatus.AuxPowerNetworkAOutputOn != this->previousEventPowerStatus.AuxPowerNetworkAOutputOn ||
		this->eventPowerStatus.AuxPowerNetworkBCommandedOn != this->previousEventPowerStatus.AuxPowerNetworkBCommandedOn ||
		this->eventPowerStatus.AuxPowerNetworkBOutputOn != this->previousEventPowerStatus.AuxPowerNetworkBOutputOn ||
		this->eventPowerStatus.AuxPowerNetworkCCommandedOn != this->previousEventPowerStatus.AuxPowerNetworkCCommandedOn ||
		this->eventPowerStatus.AuxPowerNetworkCOutputOn != this->previousEventPowerStatus.AuxPowerNetworkCOutputOn ||
		this->eventPowerStatus.AuxPowerNetworkDCommandedOn != this->previousEventPowerStatus.AuxPowerNetworkDCommandedOn ||
		this->eventPowerStatus.AuxPowerNetworkDOutputOn != this->previousEventPowerStatus.AuxPowerNetworkDOutputOn) {
		this->logPowerStatus();
	}
}

void M1M3SSPublisher::logPowerSupplyStatus() {
	this->m1m3SAL->logEvent_PowerSupplyStatus(&this->eventPowerSupplyStatus, 0);
	this->previousEventPowerSupplyStatus = this->eventPowerSupplyStatus;
}

void M1M3SSPublisher::tryLogPowerSupplyStatus() {
	if (this->eventPowerSupplyStatus.RCPMirrorCellUtility220VAC1Status != this->previousEventPowerSupplyStatus.RCPMirrorCellUtility220VAC1Status ||
		this->eventPowerSupplyStatus.RCPCabinetUtility220VACStatus != this->previousEventPowerSupplyStatus.RCPCabinetUtility220VACStatus ||
		this->eventPowerSupplyStatus.RCPExternalEquipment220VACStatus != this->previousEventPowerSupplyStatus.RCPExternalEquipment220VACStatus ||
		this->eventPowerSupplyStatus.RCPMirrorCellUtility220VAC2Status != this->previousEventPowerSupplyStatus.RCPMirrorCellUtility220VAC2Status ||
		this->eventPowerSupplyStatus.RCPMirrorCellUtility220VAC3Status != this->previousEventPowerSupplyStatus.RCPMirrorCellUtility220VAC3Status ||
		this->eventPowerSupplyStatus.PowerNetworkARedundancyControlStatus != this->previousEventPowerSupplyStatus.PowerNetworkARedundancyControlStatus ||
		this->eventPowerSupplyStatus.PowerNetworkBRedundancyControlStatus != this->previousEventPowerSupplyStatus.PowerNetworkBRedundancyControlStatus ||
		this->eventPowerSupplyStatus.PowerNetworkCRedundancyControlStatus != this->previousEventPowerSupplyStatus.PowerNetworkCRedundancyControlStatus ||
		this->eventPowerSupplyStatus.PowerNetworkDRedundancyControlStatus != this->previousEventPowerSupplyStatus.PowerNetworkDRedundancyControlStatus ||
		this->eventPowerSupplyStatus.ControlsPowerNetworkRedundancyControlStatus != this->previousEventPowerSupplyStatus.ControlsPowerNetworkRedundancyControlStatus ||
		this->eventPowerSupplyStatus.PowerNetworkAStatus != this->previousEventPowerSupplyStatus.PowerNetworkAStatus ||
		this->eventPowerSupplyStatus.PowerNetworkARedundantStatus != this->previousEventPowerSupplyStatus.PowerNetworkARedundantStatus ||
		this->eventPowerSupplyStatus.PowerNetworkBStatus != this->previousEventPowerSupplyStatus.PowerNetworkBStatus ||
		this->eventPowerSupplyStatus.PowerNetworkBRedundantStatus != this->previousEventPowerSupplyStatus.PowerNetworkBRedundantStatus ||
		this->eventPowerSupplyStatus.PowerNetworkCStatus != this->previousEventPowerSupplyStatus.PowerNetworkCStatus ||
		this->eventPowerSupplyStatus.PowerNetworkCRedundantStatus != this->previousEventPowerSupplyStatus.PowerNetworkCRedundantStatus ||
		this->eventPowerSupplyStatus.PowerNetworkDStatus != this->previousEventPowerSupplyStatus.PowerNetworkDStatus ||
		this->eventPowerSupplyStatus.PowerNetworkDRedundantStatus != this->previousEventPowerSupplyStatus.PowerNetworkDRedundantStatus ||
		this->eventPowerSupplyStatus.ControlsPowerNetworkStatus != this->previousEventPowerSupplyStatus.ControlsPowerNetworkStatus ||
		this->eventPowerSupplyStatus.ControlsPowerNetworkRedundantStatus != this->previousEventPowerSupplyStatus.ControlsPowerNetworkRedundantStatus ||
		this->eventPowerSupplyStatus.LightPowerNetworkStatus != this->previousEventPowerSupplyStatus.LightPowerNetworkStatus ||
		this->eventPowerSupplyStatus.ExternalEquipmentPowerNetworkStatus != this->previousEventPowerSupplyStatus.ExternalEquipmentPowerNetworkStatus ||
		this->eventPowerSupplyStatus.LaserTrackerPowerNetworkStatus != this->previousEventPowerSupplyStatus.LaserTrackerPowerNetworkStatus) {
		this->logPowerSupplyStatus();
	}
}

void M1M3SSPublisher::logPowerWarning() {
	this->eventPowerWarning.AnyWarning =
		this->eventPowerWarning.PowerNetworkAOutputMismatch ||
		this->eventPowerWarning.PowerNetworkBOutputMismatch ||
		this->eventPowerWarning.PowerNetworkCOutputMismatch ||
		this->eventPowerWarning.PowerNetworkDOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkAOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkBOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkCOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkDOutputMismatch;
	this->m1m3SAL->logEvent_PowerWarning(&this->eventPowerWarning, 0);
	this->previousEventPowerWarning = this->eventPowerWarning;
}

void M1M3SSPublisher::tryLogPowerWarning() {
	if (this->eventPowerWarning.PowerNetworkAOutputMismatch != this->previousEventPowerWarning.PowerNetworkAOutputMismatch ||
		this->eventPowerWarning.PowerNetworkBOutputMismatch != this->previousEventPowerWarning.PowerNetworkBOutputMismatch ||
		this->eventPowerWarning.PowerNetworkCOutputMismatch != this->previousEventPowerWarning.PowerNetworkCOutputMismatch ||
		this->eventPowerWarning.PowerNetworkDOutputMismatch != this->previousEventPowerWarning.PowerNetworkDOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkAOutputMismatch != this->previousEventPowerWarning.AuxPowerNetworkAOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkBOutputMismatch != this->previousEventPowerWarning.AuxPowerNetworkBOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkCOutputMismatch != this->previousEventPowerWarning.AuxPowerNetworkCOutputMismatch ||
		this->eventPowerWarning.AuxPowerNetworkDOutputMismatch != this->previousEventPowerWarning.AuxPowerNetworkDOutputMismatch) {
		this->logPowerWarning();
	}
}

void M1M3SSPublisher::logRejectedAberrationForces() {
	this->m1m3SAL->logEvent_RejectedAberrationForces(&this->eventRejectedAberrationForces, 0);
	this->previousEventRejectedAberrationForces = this->eventRejectedAberrationForces;
}

void M1M3SSPublisher::tryLogRejectedAberrationForces() {
	bool changeDetected =
		this->eventRejectedAberrationForces.Fz != this->previousEventRejectedAberrationForces.Fz ||
		this->eventRejectedAberrationForces.Mx != this->previousEventRejectedAberrationForces.Mx ||
		this->eventRejectedAberrationForces.My != this->previousEventRejectedAberrationForces.My;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventRejectedAberrationForces.ZForces[i] != this->previousEventRejectedAberrationForces.ZForces[i];
	}
	if (changeDetected) {
		this->logRejectedAberrationForces();
	}
}

void M1M3SSPublisher::logRejectedAccelerationForces() {
	this->m1m3SAL->logEvent_RejectedAccelerationForces(&this->eventRejectedAccelerationForces, 0);
	this->previousEventRejectedAccelerationForces = this->eventRejectedAccelerationForces;
}

void M1M3SSPublisher::tryLogRejectedAccelerationForces() {
	bool changeDetected =
		this->eventRejectedAccelerationForces.Fx != this->previousEventRejectedAccelerationForces.Fx ||
		this->eventRejectedAccelerationForces.Fy != this->previousEventRejectedAccelerationForces.Fy ||
		this->eventRejectedAccelerationForces.Fz != this->previousEventRejectedAccelerationForces.Fz ||
		this->eventRejectedAccelerationForces.Mx != this->previousEventRejectedAccelerationForces.Mx ||
		this->eventRejectedAccelerationForces.My != this->previousEventRejectedAccelerationForces.My ||
		this->eventRejectedAccelerationForces.Mz != this->previousEventRejectedAccelerationForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedAccelerationForces.XForces[i] != this->previousEventRejectedAccelerationForces.XForces[i]) ||
			(i < 100 && this->eventRejectedAccelerationForces.YForces[i] != this->previousEventRejectedAccelerationForces.YForces[i]) ||
			(this->eventRejectedAccelerationForces.ZForces[i] != this->previousEventRejectedAccelerationForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedAccelerationForces();
	}
}

void M1M3SSPublisher::logRejectedActiveOpticForces() {
	this->m1m3SAL->logEvent_RejectedActiveOpticForces(&this->eventRejectedActiveOpticForces, 0);
	this->previousEventRejectedActiveOpticForces = this->eventRejectedActiveOpticForces;
}

void M1M3SSPublisher::tryLogRejectedActiveOpticForces() {
	bool changeDetected =
		this->eventRejectedActiveOpticForces.Fz != this->previousEventRejectedActiveOpticForces.Fz ||
		this->eventRejectedActiveOpticForces.Mx != this->previousEventRejectedActiveOpticForces.Mx ||
		this->eventRejectedActiveOpticForces.My != this->previousEventRejectedActiveOpticForces.My;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			this->eventRejectedActiveOpticForces.ZForces[i] != this->previousEventRejectedActiveOpticForces.ZForces[i];
	}
	if (changeDetected) {
		this->logRejectedActiveOpticForces();
	}
}

void M1M3SSPublisher::logRejectedAzimuthForces() {
	this->m1m3SAL->logEvent_RejectedAzimuthForces(&this->eventRejectedAzimuthForces, 0);
	this->previousEventRejectedAzimuthForces = this->eventRejectedAzimuthForces;
}

void M1M3SSPublisher::tryLogRejectedAzimuthForces() {
	bool changeDetected =
		this->eventRejectedAzimuthForces.Fx != this->previousEventRejectedAzimuthForces.Fx ||
		this->eventRejectedAzimuthForces.Fy != this->previousEventRejectedAzimuthForces.Fy ||
		this->eventRejectedAzimuthForces.Fz != this->previousEventRejectedAzimuthForces.Fz ||
		this->eventRejectedAzimuthForces.Mx != this->previousEventRejectedAzimuthForces.Mx ||
		this->eventRejectedAzimuthForces.My != this->previousEventRejectedAzimuthForces.My ||
		this->eventRejectedAzimuthForces.Mz != this->previousEventRejectedAzimuthForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedAzimuthForces.XForces[i] != this->previousEventRejectedAzimuthForces.XForces[i]) ||
			(i < 100 && this->eventRejectedAzimuthForces.YForces[i] != this->previousEventRejectedAzimuthForces.YForces[i]) ||
			(this->eventRejectedAzimuthForces.ZForces[i] != this->previousEventRejectedAzimuthForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedAzimuthForces();
	}
}

void M1M3SSPublisher::logRejectedBalanceForces() {
	this->m1m3SAL->logEvent_RejectedBalanceForces(&this->eventRejectedBalanceForces, 0);
	this->previousEventRejectedBalanceForces = this->eventRejectedBalanceForces;
}

void M1M3SSPublisher::tryLogRejectedBalanceForces() {
	bool changeDetected =
		this->eventRejectedBalanceForces.Fx != this->previousEventRejectedBalanceForces.Fx ||
		this->eventRejectedBalanceForces.Fy != this->previousEventRejectedBalanceForces.Fy ||
		this->eventRejectedBalanceForces.Fz != this->previousEventRejectedBalanceForces.Fz ||
		this->eventRejectedBalanceForces.Mx != this->previousEventRejectedBalanceForces.Mx ||
		this->eventRejectedBalanceForces.My != this->previousEventRejectedBalanceForces.My ||
		this->eventRejectedBalanceForces.Mz != this->previousEventRejectedBalanceForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedBalanceForces.XForces[i] != this->previousEventRejectedBalanceForces.XForces[i]) ||
			(i < 100 && this->eventRejectedBalanceForces.YForces[i] != this->previousEventRejectedBalanceForces.YForces[i]) ||
			(this->eventRejectedBalanceForces.ZForces[i] != this->previousEventRejectedBalanceForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedBalanceForces();
	}
}

void M1M3SSPublisher::logRejectedCylinderForces() {
	this->m1m3SAL->logEvent_RejectedCylinderForces(&this->eventRejectedCylinderForces, 0);
	this->previousEventRejectedCylinderForces = this->eventRejectedCylinderForces;
}

void M1M3SSPublisher::tryLogRejectedCylinderForces() {
	bool changeDetected = false;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 112 && this->eventRejectedCylinderForces.SecondaryCylinderForces[i] != this->previousEventRejectedCylinderForces.SecondaryCylinderForces[i]) ||
			(this->eventRejectedCylinderForces.PrimaryCylinderForces[i] != this->previousEventRejectedCylinderForces.PrimaryCylinderForces[i]);
	}
	if (changeDetected) {
		this->logRejectedCylinderForces();
	}
}

void M1M3SSPublisher::logRejectedElevationForces() {
	this->m1m3SAL->logEvent_RejectedElevationForces(&this->eventRejectedElevationForces, 0);
	this->previousEventRejectedElevationForces = this->eventRejectedElevationForces;
}

void M1M3SSPublisher::tryLogRejectedElevationForces() {
	bool changeDetected =
		this->eventRejectedElevationForces.Fx != this->previousEventRejectedElevationForces.Fx ||
		this->eventRejectedElevationForces.Fy != this->previousEventRejectedElevationForces.Fy ||
		this->eventRejectedElevationForces.Fz != this->previousEventRejectedElevationForces.Fz ||
		this->eventRejectedElevationForces.Mx != this->previousEventRejectedElevationForces.Mx ||
		this->eventRejectedElevationForces.My != this->previousEventRejectedElevationForces.My ||
		this->eventRejectedElevationForces.Mz != this->previousEventRejectedElevationForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedElevationForces.XForces[i] != this->previousEventRejectedElevationForces.XForces[i]) ||
			(i < 100 && this->eventRejectedElevationForces.YForces[i] != this->previousEventRejectedElevationForces.YForces[i]) ||
			(this->eventRejectedElevationForces.ZForces[i] != this->previousEventRejectedElevationForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedElevationForces();
	}
}

void M1M3SSPublisher::logRejectedForces() {
	this->m1m3SAL->logEvent_RejectedForces(&this->eventRejectedForces, 0);
	this->previousEventRejectedForces = this->eventRejectedForces;
}

void M1M3SSPublisher::tryLogRejectedForces() {
	bool changeDetected =
		this->eventRejectedForces.Fx != this->previousEventRejectedForces.Fx ||
		this->eventRejectedForces.Fy != this->previousEventRejectedForces.Fy ||
		this->eventRejectedForces.Fz != this->previousEventRejectedForces.Fz ||
		this->eventRejectedForces.Mx != this->previousEventRejectedForces.Mx ||
		this->eventRejectedForces.My != this->previousEventRejectedForces.My ||
		this->eventRejectedForces.Mz != this->previousEventRejectedForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedForces.XForces[i] != this->previousEventRejectedForces.XForces[i]) ||
			(i < 100 && this->eventRejectedForces.YForces[i] != this->previousEventRejectedForces.YForces[i]) ||
			(this->eventRejectedForces.ZForces[i] != this->previousEventRejectedForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedForces();
	}
}

void M1M3SSPublisher::logRejectedOffsetForces() {
	this->m1m3SAL->logEvent_RejectedOffsetForces(&this->eventRejectedOffsetForces, 0);
	this->previousEventRejectedOffsetForces = this->eventRejectedOffsetForces;
}

void M1M3SSPublisher::tryLogRejectedOffsetForces() {
	bool changeDetected =
		this->eventRejectedOffsetForces.Fx != this->previousEventRejectedOffsetForces.Fx ||
		this->eventRejectedOffsetForces.Fy != this->previousEventRejectedOffsetForces.Fy ||
		this->eventRejectedOffsetForces.Fz != this->previousEventRejectedOffsetForces.Fz ||
		this->eventRejectedOffsetForces.Mx != this->previousEventRejectedOffsetForces.Mx ||
		this->eventRejectedOffsetForces.My != this->previousEventRejectedOffsetForces.My ||
		this->eventRejectedOffsetForces.Mz != this->previousEventRejectedOffsetForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedOffsetForces.XForces[i] != this->previousEventRejectedOffsetForces.XForces[i]) ||
			(i < 100 && this->eventRejectedOffsetForces.YForces[i] != this->previousEventRejectedOffsetForces.YForces[i]) ||
			(this->eventRejectedOffsetForces.ZForces[i] != this->previousEventRejectedOffsetForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedOffsetForces();
	}
}

void M1M3SSPublisher::logRejectedStaticForces() {
	this->m1m3SAL->logEvent_RejectedStaticForces(&this->eventRejectedStaticForces, 0);
	this->previousEventRejectedStaticForces = this->eventRejectedStaticForces;
}

void M1M3SSPublisher::tryLogRejectedStaticForces() {
	bool changeDetected =
		this->eventRejectedStaticForces.Fx != this->previousEventRejectedStaticForces.Fx ||
		this->eventRejectedStaticForces.Fy != this->previousEventRejectedStaticForces.Fy ||
		this->eventRejectedStaticForces.Fz != this->previousEventRejectedStaticForces.Fz ||
		this->eventRejectedStaticForces.Mx != this->previousEventRejectedStaticForces.Mx ||
		this->eventRejectedStaticForces.My != this->previousEventRejectedStaticForces.My ||
		this->eventRejectedStaticForces.Mz != this->previousEventRejectedStaticForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedStaticForces.XForces[i] != this->previousEventRejectedStaticForces.XForces[i]) ||
			(i < 100 && this->eventRejectedStaticForces.YForces[i] != this->previousEventRejectedStaticForces.YForces[i]) ||
			(this->eventRejectedStaticForces.ZForces[i] != this->previousEventRejectedStaticForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedStaticForces();
	}
}

void M1M3SSPublisher::logRejectedThermalForces() {
	this->m1m3SAL->logEvent_RejectedThermalForces(&this->eventRejectedThermalForces, 0);
	this->previousEventRejectedThermalForces = this->eventRejectedThermalForces;
}

void M1M3SSPublisher::tryLogRejectedThermalForces() {
	bool changeDetected =
		this->eventRejectedThermalForces.Fx != this->previousEventRejectedThermalForces.Fx ||
		this->eventRejectedThermalForces.Fy != this->previousEventRejectedThermalForces.Fy ||
		this->eventRejectedThermalForces.Fz != this->previousEventRejectedThermalForces.Fz ||
		this->eventRejectedThermalForces.Mx != this->previousEventRejectedThermalForces.Mx ||
		this->eventRejectedThermalForces.My != this->previousEventRejectedThermalForces.My ||
		this->eventRejectedThermalForces.Mz != this->previousEventRejectedThermalForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedThermalForces.XForces[i] != this->previousEventRejectedThermalForces.XForces[i]) ||
			(i < 100 && this->eventRejectedThermalForces.YForces[i] != this->previousEventRejectedThermalForces.YForces[i]) ||
			(this->eventRejectedThermalForces.ZForces[i] != this->previousEventRejectedThermalForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedThermalForces();
	}
}

void M1M3SSPublisher::logRejectedVelocityForces() {
	this->m1m3SAL->logEvent_RejectedVelocityForces(&this->eventRejectedVelocityForces, 0);
	this->previousEventRejectedVelocityForces = this->eventRejectedVelocityForces;
}

void M1M3SSPublisher::tryLogRejectedVelocityForces() {
	bool changeDetected =
		this->eventRejectedVelocityForces.Fx != this->previousEventRejectedVelocityForces.Fx ||
		this->eventRejectedVelocityForces.Fy != this->previousEventRejectedVelocityForces.Fy ||
		this->eventRejectedVelocityForces.Fz != this->previousEventRejectedVelocityForces.Fz ||
		this->eventRejectedVelocityForces.Mx != this->previousEventRejectedVelocityForces.Mx ||
		this->eventRejectedVelocityForces.My != this->previousEventRejectedVelocityForces.My ||
		this->eventRejectedVelocityForces.Mz != this->previousEventRejectedVelocityForces.Mz;
	for(int i = 0; i < FA_COUNT && !changeDetected; ++i) {
		changeDetected = changeDetected ||
			(i < 12 && this->eventRejectedVelocityForces.XForces[i] != this->previousEventRejectedVelocityForces.XForces[i]) ||
			(i < 100 && this->eventRejectedVelocityForces.YForces[i] != this->previousEventRejectedVelocityForces.YForces[i]) ||
			(this->eventRejectedVelocityForces.ZForces[i] != this->previousEventRejectedVelocityForces.ZForces[i]);
	}
	if (changeDetected) {
		this->logRejectedVelocityForces();
	}
}

void M1M3SSPublisher::logSettingVersions() {
	this->m1m3SAL->logEvent_SettingVersions(&this->eventSettingVersions, 0);
	this->previousEventSettingVersions = this->eventSettingVersions;
}

void M1M3SSPublisher::tryLogSettingVersions() {
	if (this->eventSettingVersions.RecommendedSettingsVersion.compare(this->previousEventSettingVersions.RecommendedSettingsVersion) != 0) {
		this->logSettingVersions();
	}
}

void M1M3SSPublisher::logSettingsApplied() {
	this->m1m3SAL->logEvent_SettingsApplied(&this->eventSettingsApplied, 0);
	this->previousEventSettingsApplied = this->eventSettingsApplied;
}

void M1M3SSPublisher::tryLogSettingsApplied() {
	if (this->eventSettingsApplied.Settings.compare(this->previousEventSettingsApplied.Settings) != 0) {
		this->logSettingsApplied();
	}
}

void M1M3SSPublisher::logSummaryState() {
	this->m1m3SAL->logEvent_SummaryState(&this->eventSummaryState, 0);
	this->previousEventSummaryState = this->eventSummaryState;
}

void M1M3SSPublisher::tryLogSummaryState() {
	if (this->eventSummaryState.SummaryState != this->previousEventSummaryState.SummaryState) {
		this->logSummaryState();
	}
}

void M1M3SSPublisher::ackCommandStart(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Start(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Enable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Disable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Standby(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Shutdown(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnAirOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnAirOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyOffsetForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearOffsetForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ClearOffsetForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_RaiseM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandLowerM1M3(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_LowerM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyActiveOpticForcesByBendingModes(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyActiveOpticForcesByBendingModes(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyActiveOpticForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearActiveOpticForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ClearActiveOpticForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAberrationForcesByBendingModes(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyAberrationForcesByBendingModes(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAberrationForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyAberrationForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearAberrationForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ClearAberrationForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnterEngineering(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_EnterEngineering(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandExitEngineering(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ExitEngineering(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTestAir(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TestAir(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTestHardpoint(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TestHardpoint(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTestForceActuator(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TestForceActuator(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandMoveHardpointActuators(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_MoveHardpointActuators(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnableHardpointChase(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_EnableHardpointChase(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisableHardpointChase(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_DisableHardpointChase(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandAbortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_AbortRaiseM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTranslateM1M3(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TranslateM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandStopHardpointMotion(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_StopHardpointMotion(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandPositionM1M3(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_PositionM1M3(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnLightsOn(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnLightsOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnLightsOff(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnLightsOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnPowerOn(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnPowerOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnPowerOff(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnPowerOff(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnableHardpointCorrections(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_EnableHardpointCorrections(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisableHardpointCorrections(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_DisableHardpointCorrections(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandRunMirrorForceProfile(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_RunMirrorForceProfile(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandAbortProfile(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_AbortProfile(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyOffsetForcesByMirrorForce(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyOffsetForcesByMirrorForce(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandUpdatePID(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_UpdatePID(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandResetPID(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ResetPID(commandID, ackCode, 0, (char*)description.c_str());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
