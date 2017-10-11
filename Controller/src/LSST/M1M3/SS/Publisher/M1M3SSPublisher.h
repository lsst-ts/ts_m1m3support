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

	void putInclinometerData(m1m3_InclinometerDataC* data);
	void putIMSData(m1m3_IMSDataC* data);

	void logStateChange(m1m3_logevent_SummaryStateC* data);
	void logSettingVersions(m1m3_logevent_SettingVersionsC* data);

	void logInclinometerSensorWarning(m1m3_logevent_InclinometerSensorWarningC* data);
	void logDisplacementSensorWarning(m1m3_logevent_DisplacementSensorWarningC* data);

	void logILCWarning(m1m3_logevent_ILCWarningC* data);

	void ackCommandStart(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
