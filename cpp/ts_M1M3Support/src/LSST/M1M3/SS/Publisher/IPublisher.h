/*
 * IPublisher.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef IPUBLISHER_H_
#define IPUBLISHER_H_

#include <DataTypes.h>
#include <SAL_m1m3C.h>
#include <string>

struct m1m3_logevent_InclinometerWarningC;
struct m1m3_InclinometerDataC;

struct m1m3_logevent_ILCWarningC;

struct m1m3_logevent_SummaryStateC;
struct m1m3_logevent_SettingVersionsC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher {
public:
	virtual ~IPublisher();

	virtual void putInclinometerData(m1m3_InclinometerDataC* data);


	virtual void logStateChange(m1m3_logevent_SummaryStateC* data);
	virtual void logSettingVersions(m1m3_logevent_SettingVersionsC* data);

	virtual void logInclinometerWarning(m1m3_logevent_InclinometerWarningC* data);
	virtual void logILCWarning(m1m3_logevent_ILCWarningC* data);

	virtual void ackCommandStart(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IPUBLISHER_H_ */
