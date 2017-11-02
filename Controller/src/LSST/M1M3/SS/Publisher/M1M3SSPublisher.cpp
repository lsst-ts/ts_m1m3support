/*
 * M1M3SSPublisher.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSPublisher.h>
#include <SAL_m1m3.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSPublisher::M1M3SSPublisher(SAL_m1m3* m1m3SAL) {
	this->m1m3SAL = m1m3SAL;

	this->m1m3SAL->salTelemetryPub("m1m3_AccelerometerData");
	this->m1m3SAL->salTelemetryPub("m1m3_ForceActuatorData");
	this->m1m3SAL->salTelemetryPub("m1m3_ForceActuatorStatus");
	this->m1m3SAL->salTelemetryPub("m1m3_HardpointData");
	this->m1m3SAL->salTelemetryPub("m1m3_HardpointStatus");
	this->m1m3SAL->salTelemetryPub("m1m3_IMSData");
	this->m1m3SAL->salTelemetryPub("m1m3_InclinometerData");
	this->m1m3SAL->salTelemetryPub("m1m3_OuterLoopData");

	this->m1m3SAL->salEvent("m1m3_logevent_AccelerometerWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_AirSupplyStatus");
	this->m1m3SAL->salEvent("m1m3_logevent_AirSupplyWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_AppliedForces");
	this->m1m3SAL->salEvent("m1m3_logevent_DisplacementSensorWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_ErrorCode");
	this->m1m3SAL->salEvent("m1m3_logevent_ForceActuatorInfo");
	this->m1m3SAL->salEvent("m1m3_logevent_HardpointActuatorInfo");
	this->m1m3SAL->salEvent("m1m3_logevent_HardpointActuatorMotionState");
	this->m1m3SAL->salEvent("m1m3_logevent_ILCWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_InclinometerSensorWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_InterlockStatus");
	this->m1m3SAL->salEvent("m1m3_logevent_InterlockWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_SettingVersions");
	this->m1m3SAL->salEvent("m1m3_logevent_SummaryState");
}

double M1M3SSPublisher::getTimestamp() { return this->m1m3SAL->getCurrentTime(); }

void M1M3SSPublisher::putAccelerometerData() { this->m1m3SAL->putSample_AccelerometerData(&this->accelerometerData); }
void M1M3SSPublisher::putForceActuatorData() { this->m1m3SAL->putSample_ForceActuatorData(&this->forceActuatorData); }
void M1M3SSPublisher::putForceActuatorStatus() { this->m1m3SAL->putSample_ForceActuatorStatus(&this->forceActuatorStatus); }
void M1M3SSPublisher::putHardpointData() { this->m1m3SAL->putSample_HardpointData(&this->hardpointData); }
void M1M3SSPublisher::putHardpointStatus() { this->m1m3SAL->putSample_HardpointStatus(&this->hardpointStatus); }
void M1M3SSPublisher::putIMSData() { this->m1m3SAL->putSample_IMSData(&this->imsData); }
void M1M3SSPublisher::putInclinometerData() { this->m1m3SAL->putSample_InclinometerData(&this->inclinometerData); }
void M1M3SSPublisher::putOuterLoopData() { this->m1m3SAL->putSample_OuterLoopData(&this->outerLoopData); }

void M1M3SSPublisher::logAccelerometerWarning() { this->m1m3SAL->logEvent_AccelerometerWarning(&this->eventAccelerometerWarning, 0); }
void M1M3SSPublisher::logAirSupplyStatus() { this->m1m3SAL->logEvent_AirSupplyStatus(&this->eventAirSupplyStatus, 0); }
void M1M3SSPublisher::logAirSupplyWarning() { this->m1m3SAL->logEvent_AirSupplyWarning(&this->eventAirSupplyWarning, 0); }
void M1M3SSPublisher::logAppliedForces() { this->m1m3SAL->logEvent_AppliedForces(&this->eventAppliedForces, 0); }
void M1M3SSPublisher::logDisplacementSensorWarning() { this->m1m3SAL->logEvent_DisplacementSensorWarning(&this->eventDisplacementSensorWarning, 0); }
void M1M3SSPublisher::logErrorCode() { this->m1m3SAL->logEvent_ErrorCode(&this->eventErrorCode, 0); }
void M1M3SSPublisher::logForceActuatorInfo() { this->m1m3SAL->logEvent_ForceActuatorInfo(&this->eventForceActuatorInfo, 0); }
void M1M3SSPublisher::logHardpointActuatorInfo() { this->m1m3SAL->logEvent_HardpointActuatorInfo(&this->eventHardpointActuatorInfo, 0); }
void M1M3SSPublisher::logHardpointActuatorMotionState() { this->m1m3SAL->logEvent_HardpointActuatorMotionState(&this->eventHardpointActuatorMotionState, 0); }
void M1M3SSPublisher::logILCWarning() { this->m1m3SAL->logEvent_ILCWarning(&this->eventILCWarning, 0); }
void M1M3SSPublisher::logInclinometerSensorWarning() { this->m1m3SAL->logEvent_InclinometerSensorWarning(&this->eventInclinometerSensorWarning, 0); }
void M1M3SSPublisher::logInterlockStatus() { this->m1m3SAL->logEvent_InterlockStatus(&this->eventInterlockStatus, 0); }
void M1M3SSPublisher::logInterlockWarning() { this->m1m3SAL->logEvent_InterlockWarning(&this->eventInterlockWarning, 0); }
void M1M3SSPublisher::logSettingVersions() { this->m1m3SAL->logEvent_SettingVersions(&this->eventSettingVersions, 0); }
void M1M3SSPublisher::logSummaryState() { this->m1m3SAL->logEvent_SummaryState(&this->eventSummaryState, 0); }

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

void M1M3SSPublisher::ackCommandApplyAOSCorrectionByBendingModes(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyAOSCorrectionByBendingModes(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAOSCorrectionByForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyAOSCorrectionByForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearAOSCorrection(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ClearAOSCorrection(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAberrationByBendingModes(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyAberrationByBendingModes(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandApplyAberrationByForces(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ApplyAberrationByForces(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandClearAberration(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_ClearAberration(commandID, ackCode, 0, (char*)description.c_str());
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
