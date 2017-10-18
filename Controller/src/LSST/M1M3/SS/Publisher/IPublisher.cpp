/*
 * IPublisher.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <IPublisher.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IPublisher::~IPublisher() { }

double IPublisher::getTimestamp() { return 0; }

void IPublisher::putInclinometerData(m1m3_InclinometerDataC* data) { }

void IPublisher::putIMSData(m1m3_IMSDataC* data) { }

void IPublisher::logStateChange(m1m3_logevent_SummaryStateC* data) { }

void IPublisher::logSettingVersions(m1m3_logevent_SettingVersionsC* data) { }

void IPublisher::logInclinometerSensorWarning(m1m3_logevent_InclinometerSensorWarningC* data) { }

void IPublisher::logDisplacementSensorWarning(m1m3_logevent_DisplacementSensorWarningC* data) { }

void IPublisher::logILCWarning(m1m3_logevent_ILCWarningC* data) { }

void IPublisher::logAirSupplyStatus(m1m3_logevent_AirSupplyStatusC* data) { }

void IPublisher::logAirSupplyWarning(m1m3_logevent_AirSupplyWarningC* data) { }

void IPublisher::putForceActuatorStatus(m1m3_ForceActuatorStatusC* data) { }

void IPublisher::putForceActuatorData(m1m3_ForceActuatorDataC* data) { }

void IPublisher::putHardpointStatus(m1m3_HardpointStatusC* data) { }

void IPublisher::putHardpointData(m1m3_HardpointDataC* data) { }

void IPublisher::ackCommandStart(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
