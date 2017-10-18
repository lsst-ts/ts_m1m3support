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

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher {
public:
	virtual ~IPublisher();

	virtual double getTimestamp();

	virtual void putInclinometerData(m1m3_InclinometerDataC* data);
	virtual void putIMSData(m1m3_IMSDataC* data);

	virtual void logStateChange(m1m3_logevent_SummaryStateC* data);
	virtual void logSettingVersions(m1m3_logevent_SettingVersionsC* data);

	virtual void logInclinometerSensorWarning(m1m3_logevent_InclinometerSensorWarningC* data);
	virtual void logDisplacementSensorWarning(m1m3_logevent_DisplacementSensorWarningC* data);
	virtual void logILCWarning(m1m3_logevent_ILCWarningC* data);

	virtual void logAirSupplyStatus(m1m3_logevent_AirSupplyStatusC* data);
	virtual void logAirSupplyWarning(m1m3_logevent_AirSupplyWarningC* data);

	virtual void putForceActuatorStatus(m1m3_ForceActuatorStatusC* data);
	virtual void putForceActuatorData(m1m3_ForceActuatorDataC* data);
	virtual void putHardpointStatus(m1m3_HardpointStatusC* data);
	virtual void putHardpointData(m1m3_HardpointDataC* data);

	virtual void ackCommandStart(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IPUBLISHER_H_ */
