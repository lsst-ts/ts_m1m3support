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

	m1m3_AccelerometerDataC accelerometerData;
	m1m3_ForceActuatorDataC forceActuatorData;
	m1m3_ForceActuatorStatusC forceActuatorStatus;
	m1m3_HardpointDataC hardpointData;
	m1m3_HardpointStatusC hardpointStatus;
	m1m3_IMSDataC imsData;
	m1m3_InclinometerDataC inclinometerData;
	m1m3_OuterLoopDataC outerLoopData;

	m1m3_logevent_AccelerometerWarningC eventAccelerometerWarning;
	m1m3_logevent_AirSupplyStatusC eventAirSupplyStatus;
	m1m3_logevent_AirSupplyWarningC eventAirSupplyWarning;
	m1m3_logevent_AppliedForcesC eventAppliedForces;
	m1m3_logevent_DisplacementSensorWarningC eventDisplacementSensorWarning;
	m1m3_logevent_ErrorCodeC eventErrorCode;
	m1m3_logevent_ForceActuatorInfoC eventForceActuatorInfo;
	m1m3_logevent_HardpointActuatorInfoC eventHardpointActuatorInfo;
	m1m3_logevent_HardpointActuatorMotionStateC eventHardpointActuatorMotionState;
	m1m3_logevent_ILCWarningC eventILCWarning;
	m1m3_logevent_InclinometerSensorWarningC eventInclinometerSensorWarning;
	m1m3_logevent_InterlockStatusC eventInterlockStatus;
	m1m3_logevent_InterlockWarningC eventInterlockWarning;
	m1m3_logevent_SettingVersionsC eventSettingVersions;
	m1m3_logevent_SummaryStateC eventSummaryState;

public:
	M1M3SSPublisher(SAL_m1m3* m1m3SAL);

	m1m3_AccelerometerDataC* getAccelerometerData() { return &this->accelerometerData; }
	m1m3_ForceActuatorDataC* getForceActuatorData() { return &this->forceActuatorData; }
	m1m3_ForceActuatorStatusC* getForceActuatorStatus() { return &this->forceActuatorStatus; }
	m1m3_HardpointDataC* getHardpointData() { return &this->hardpointData; }
	m1m3_HardpointStatusC* getHardpointStatus() { return &this->hardpointStatus; }
	m1m3_IMSDataC* getIMSData() { return &this->imsData; }
	m1m3_InclinometerDataC* getInclinometerData() { return &this->inclinometerData; }
	m1m3_OuterLoopDataC* getOuterLoopData() { return &this->outerLoopData; }

	m1m3_logevent_AccelerometerWarningC* getEventAccelerometerWarning() { return &this->eventAccelerometerWarning; }
	m1m3_logevent_AirSupplyStatusC* getEventAirSupplyStatus() { return &this->eventAirSupplyStatus; }
	m1m3_logevent_AirSupplyWarningC* getEventAirSupplyWarning() { return &this->eventAirSupplyWarning; }
	m1m3_logevent_AppliedForcesC* getEventAppliedForces() { return &this->eventAppliedForces; }
	m1m3_logevent_DisplacementSensorWarningC* getEventDisplacementSensorWarning() { return &this->eventDisplacementSensorWarning; }
	m1m3_logevent_ErrorCodeC* getEventErrorCode() { return &this->eventErrorCode; }
	m1m3_logevent_ForceActuatorInfoC* getEventForceActuatorInfo() { return &this->eventForceActuatorInfo; }
	m1m3_logevent_HardpointActuatorInfoC* getEventHardpointActuatorInfo() { return &this->eventHardpointActuatorInfo; }
	m1m3_logevent_HardpointActuatorMotionStateC* getEventHardpointActuatorMotionState() { return &this->eventHardpointActuatorMotionState; }
	m1m3_logevent_ILCWarningC* getEventILCWarning(){ return &this->eventILCWarning; }
	m1m3_logevent_InclinometerSensorWarningC* getEventInclinometerSensorWarning() { return &this->eventInclinometerSensorWarning; }
	m1m3_logevent_InterlockStatusC* getEventInterlockStatus() { return &this->eventInterlockStatus; }
	m1m3_logevent_InterlockWarningC* getEventInterlockWarning() { return &this->eventInterlockWarning; }
	m1m3_logevent_SettingVersionsC* getEventSettingVersions() { return &this->eventSettingVersions; }
	m1m3_logevent_SummaryStateC* getEventSummaryState() { return &this->eventSummaryState; }

	double getTimestamp();

	void putAccelerometerData();
	void putForceActuatorData();
	void putForceActuatorStatus();
	void putHardpointData();
	void putHardpointStatus();
	void putIMSData();
	void putInclinometerData();
	void putOuterLoopData();

	void logAccelerometerWarning();
	void logAirSupplyStatus();
	void logAirSupplyWarning();
	void logAppliedForces();
	void logDisplacementSensorWarning();
	void logErrorCode();
	void logForceActuatorInfo();
	void logHardpointActuatorInfo();
	void logHardpointActuatorMotionState();
	void logILCWarning();
	void logInclinometerSensorWarning();
	void logInterlockStatus();
	void logInterlockWarning();
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
	void ackCommandRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandLowerM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyAOSCorrectionByBendingModes(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyAOSCorrectionByForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandClearAOSCorrection(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyAberrationByBendingModes(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandApplyAberrationByForces(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandClearAberration(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnterEngineering(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandExitEngineering(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTestAir(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTestHardpoint(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTestForceActuator(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandMoveHardpointActuators(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandEnableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandDisableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandAbortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandTranslateM1M3(int32_t commandID, int32_t ackCode, std::string description);
	void ackCommandStopHardpointMotion(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSPUBLISHER_H_ */
