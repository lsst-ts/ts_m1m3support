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

void IPublisher::putInclinometerData(m1m3_InclinometerDataC* data) { }

void IPublisher::logStateChange(m1m3_logevent_SummaryStateC* data) { }

void IPublisher::logSettingVersions(m1m3_logevent_SettingVersionsC* data) { }

void IPublisher::logInclinometerWarning(m1m3_logevent_InclinometerWarningC* data) { }

void IPublisher::logILCWarning(m1m3_logevent_ILCWarningC* data) { }

void IPublisher::ackCommandStart(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description) { }

void IPublisher::ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
