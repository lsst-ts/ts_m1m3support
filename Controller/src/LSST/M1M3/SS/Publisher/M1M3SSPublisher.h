/*
 * M1M3SSPublisher.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSPUBLISHER_H_
#define M1M3SSPUBLISHER_H_

#include <IPublisher.h>
#include <SAL_m1m3C.h>

class SAL_m1m3;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher : public IPublisher {
private:
	SAL_m1m3* m1m3SAL;

	m1m3_ForceActuatorDataC forceActuatorData;
	m1m3_ForceActuatorStatusC forceActuatorStatus;
	m1m3_HardpointDataC hardpointData;
	m1m3_HardpointStatusC hardpointStatus;
	m1m3_IMSDataC imsData;
	m1m3_InclinometerDataC inclinometerData;
	m1m3_OuterLoopDataC outerLoopData;

	m1m3_logevent_AirSupplyStatusC eventAirSupplyStatus;
	m1m3_logevent_AirSupplyWarningC eventAirSupplyWarning;
	m1m3_logevent_DisplacementSensorWarningC eventDisplacementSensorWarning;
	m1m3_logevent_ForceActuatorInfoC eventForceActuatorInfo;
	m1m3_logevent_HardpointActuatorInfoC eventHardpointActuatorInfo;
	m1m3_logevent_ILCWarningC eventILCWarning;
	m1m3_logevent_InclinometerSensorWarningC eventInclinometerSensorWarning;
	m1m3_logevent_SettingVersionsC eventSettingVersions;
	m1m3_logevent_SummaryStateC eventSummaryState;

public:
	M1M3SSPublisher(SAL_m1m3* m1m3SAL);

	m1m3_ForceActuatorDataC* getForceActuatorData() { return &this->forceActuatorData; }
	m1m3_ForceActuatorStatusC* getForceActuatorStatus() { return &this->forceActuatorStatus; }
	m1m3_HardpointDataC* getHardpointData() { return &this->hardpointData; }
	m1m3_HardpointStatusC* getHardpointStatus() { return &this->hardpointStatus; }
	m1m3_IMSDataC* getIMSData() { return &this->imsData; }
	m1m3_InclinometerDataC* getInclinometerData() { return &this->inclinometerData; }
	m1m3_OuterLoopDataC* getOuterLoopData() { return &this->outerLoopData; }

	m1m3_logevent_AirSupplyStatusC* getEventAirSupplyStatus() { return &this->eventAirSupplyStatus; }
	m1m3_logevent_AirSupplyWarningC* getEventAirSupplyWarning() { return &this->eventAirSupplyWarning; }
	m1m3_logevent_DisplacementSensorWarningC* getEventDisplacementSensorWarning() { return &this->eventDisplacementSensorWarning; }
	m1m3_logevent_ForceActuatorInfoC* getEventForceActuatorInfo() { return &this->eventForceActuatorInfo; }
	m1m3_logevent_HardpointActuatorInfoC* getEventHardpoingActuatorInfo() { return &this->eventHardpointActuatorInfo; }
	m1m3_logevent_ILCWarningC* getEventILCWarning(){ return &this->eventILCWarning; }
	m1m3_logevent_InclinometerSensorWarningC* getEventInclinometerSensorWarning() { return &this->eventInclinometerSensorWarning; }
	m1m3_logevent_SettingVersionsC* getEventSettingVersions() { return &this->eventSettingVersions; }
	m1m3_logevent_SummaryStateC* getEventSummaryState() { return &this->eventSummaryState; }

	double getTimestamp();

	void putForceActuatorData();
	void putForceActuatorStatus();
	void putHardpointData();
	void putHardpointStatus();
	void putIMSData();
	void putInclinometerData();
	void putOuterLoopData();

	void logAirSupplyStatus();
	void logAirSupplyWarning();
	void logDisplacementSensorWarning();
	void logForceActuatorInfo();
	void logHardpointActuatorInfo();
	void logILCWarning();
	void logInclinometerSensorWarning();
	void logSettingVersions();
	void logSummaryState();

	void ackCommandStart(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandClearOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
