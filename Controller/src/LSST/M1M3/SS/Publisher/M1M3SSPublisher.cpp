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

	this->m1m3SAL->salTelemetryPub("m1m3_ForceActuatorData");
	this->m1m3SAL->salTelemetryPub("m1m3_ForceActuatorStatus");
	this->m1m3SAL->salTelemetryPub("m1m3_HardpointData");
	this->m1m3SAL->salTelemetryPub("m1m3_HardpointStatus");
	this->m1m3SAL->salTelemetryPub("m1m3_IMSData");
	this->m1m3SAL->salTelemetryPub("m1m3_InclinometerData");
	this->m1m3SAL->salTelemetryPub("m1m3_OuterLoopData");

	this->m1m3SAL->salEvent("m1m3_logevent_AirSupplyStatus");
	this->m1m3SAL->salEvent("m1m3_logevent_AirSupplyWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_DisplacementSensorWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_HardpointActuatorInfo");
	this->m1m3SAL->salEvent("m1m3_logevent_ILCWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_InclinometerSensorWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_SettingVersions");
	this->m1m3SAL->salEvent("m1m3_logevent_SummaryState");
}

double M1M3SSPublisher::getTimestamp() { return this->m1m3SAL->getCurrentTime(); }

void M1M3SSPublisher::putForceActuatorData() { this->m1m3SAL->putSample_ForceActuatorData(&this->forceActuatorData); }
void M1M3SSPublisher::putForceActuatorStatus() { this->m1m3SAL->putSample_ForceActuatorStatus(&this->forceActuatorStatus); }
void M1M3SSPublisher::putHardpointData() { this->m1m3SAL->putSample_HardpointData(&this->hardpointData); }
void M1M3SSPublisher::putHardpointStatus() { this->m1m3SAL->putSample_HardpointStatus(&this->hardpointStatus); }
void M1M3SSPublisher::putIMSData() { this->m1m3SAL->putSample_IMSData(&this->imsData); }
void M1M3SSPublisher::putInclinometerData() { this->m1m3SAL->putSample_InclinometerData(&this->inclinometerData); }
void M1M3SSPublisher::putOuterLoopData() { this->m1m3SAL->putSample_OuterLoopData(&this->outerLoopData); }

void M1M3SSPublisher::logAirSupplyStatus() { this->m1m3SAL->logEvent_AirSupplyStatus(&this->eventAirSupplyStatus, 0); }
void M1M3SSPublisher::logAirSupplyWarning() { this->m1m3SAL->logEvent_AirSupplyWarning(&this->eventAirSupplyWarning, 0); }
void M1M3SSPublisher::logDisplacementSensorWarning() { this->m1m3SAL->logEvent_DisplacementSensorWarning(&this->eventDisplacementSensorWarning, 0); }
void M1M3SSPublisher::logForceActuatorInfo() { this->m1m3SAL->logEvent_ForceActuatorInfo(&this->eventForceActuatorInfo, 0); }
void M1M3SSPublisher::logHardpointActuatorInfo() { this->m1m3SAL->logEvent_HardpointActuatorInfo(&this->eventHardpointActuatorInfo, 0); }
void M1M3SSPublisher::logILCWarning() { this->m1m3SAL->logEvent_ILCWarning(&this->eventILCWarning, 0); }
void M1M3SSPublisher::logInclinometerSensorWarning() { this->m1m3SAL->logEvent_InclinometerSensorWarning(&this->eventInclinometerSensorWarning, 0); }
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
