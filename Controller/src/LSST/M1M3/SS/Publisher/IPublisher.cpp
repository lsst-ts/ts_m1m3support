/*
 * IPublisher.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IPublisher::~IPublisher() { }

m1m3_ForceActuatorDataC* IPublisher::getForceActuatorData() { return 0; }
m1m3_ForceActuatorStatusC* IPublisher::getForceActuatorStatus() { return 0; }
m1m3_HardpointDataC* IPublisher::getHardpointData() { return 0; }
m1m3_HardpointStatusC* IPublisher::getHardpointStatus() { return 0; }
m1m3_IMSDataC* IPublisher::getIMSData() { return 0; }
m1m3_InclinometerDataC* IPublisher::getInclinometerData() { return 0; }
m1m3_OuterLoopDataC* IPublisher::getOuterLoopData() { return 0; }

m1m3_logevent_AirSupplyStatusC* IPublisher::getEventAirSupplyStatus() { return 0; }
m1m3_logevent_AirSupplyWarningC* IPublisher::getEventAirSupplyWarning() { return 0; }
m1m3_logevent_DisplacementSensorWarningC* IPublisher::getEventDisplacementSensorWarning() { return 0; }
m1m3_logevent_ForceActuatorInfoC* IPublisher::getEventForceActuatorInfo() { return 0; }
m1m3_logevent_HardpointActuatorInfoC* IPublisher::getEventHardpointActuatorInfo() { return 0; }
m1m3_logevent_ILCWarningC* IPublisher::getEventILCWarning() { return 0; }
m1m3_logevent_InclinometerSensorWarningC* IPublisher::getEventInclinometerSensorWarning() { return 0; }
m1m3_logevent_SettingVersionsC* IPublisher::getEventSettingVersions() { return 0; }
m1m3_logevent_SummaryStateC* IPublisher::getEventSummaryState() { return 0; }

double IPublisher::getTimestamp() { return 0; }

void IPublisher::putForceActuatorData() { }
void IPublisher::putForceActuatorStatus() { }
void IPublisher::putHardpointData() { }
void IPublisher::putHardpointStatus() { }
void IPublisher::putIMSData() { }
void IPublisher::putInclinometerData() { }
void IPublisher::putOuterLoopData() { }

void IPublisher::logAirSupplyStatus() { }
void IPublisher::logAirSupplyWarning() { }
void IPublisher::logDisplacementSensorWarning() { }
void IPublisher::logForceActuatorInfo() { }
void IPublisher::logHardpointActuatorInfo() { }
void IPublisher::logILCWarning() { }
void IPublisher::logInclinometerSensorWarning() { }
void IPublisher::logSettingVersions() { }
void IPublisher::logSummaryState() { }

void IPublisher::ackCommandStart(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandApplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandClearOffsetForces(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description) { }
void IPublisher::ackCommandLowerM1M3(int32_t commandID, int32_t ackCode, std::string description) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
