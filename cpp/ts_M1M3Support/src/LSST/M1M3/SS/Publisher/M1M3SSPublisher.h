/*
 * M1M3SSPublisher.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSPUBLISHER_H_
#define M1M3SSPUBLISHER_H_

#include <IPublisher.h>

class SAL_m1m3;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher : public IPublisher {
private:
	SAL_m1m3* m1m3SAL;

public:
	M1M3SSPublisher(SAL_m1m3* m1m3SAL);

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

#endif /* M1M3SSPUBLISHER_H_ */
