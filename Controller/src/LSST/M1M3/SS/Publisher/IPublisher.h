/*
 * IPublisher.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef IPUBLISHER_H_
#define IPUBLISHER_H_

#include <DataTypes.h>
#include <string>

struct m1m3_ForceActuatorDataC;
struct m1m3_ForceActuatorStatusC;
struct m1m3_HardpointDataC;
struct m1m3_HardpointStatusC;
struct m1m3_IMSDataC;
struct m1m3_InclinometerDataC;
struct m1m3_OuterLoopDataC;

struct m1m3_logevent_AirSupplyStatusC;
struct m1m3_logevent_AirSupplyWarningC;
struct m1m3_logevent_DisplacementSensorWarningC;
struct m1m3_logevent_ErrorCodeC;
struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_logevent_HardpointActuatorInfoC;
struct m1m3_logevent_ILCWarningC;
struct m1m3_logevent_InclinometerSensorWarningC;
struct m1m3_logevent_SettingVersionsC;
struct m1m3_logevent_SummaryStateC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher {
public:
	virtual ~IPublisher();

	virtual m1m3_ForceActuatorDataC* getForceActuatorData();
	virtual m1m3_ForceActuatorStatusC* getForceActuatorStatus();
	virtual m1m3_HardpointDataC* getHardpointData();
	virtual m1m3_HardpointStatusC* getHardpointStatus();
	virtual m1m3_IMSDataC* getIMSData();
	virtual m1m3_InclinometerDataC* getInclinometerData();
	virtual m1m3_OuterLoopDataC* getOuterLoopData();

	virtual m1m3_logevent_AirSupplyStatusC* getEventAirSupplyStatus();
	virtual m1m3_logevent_AirSupplyWarningC* getEventAirSupplyWarning();
	virtual m1m3_logevent_DisplacementSensorWarningC* getEventDisplacementSensorWarning();
	virtual m1m3_logevent_ErrorCodeC* getEventErrorCode();
	virtual m1m3_logevent_ForceActuatorInfoC* getEventForceActuatorInfo();
	virtual m1m3_logevent_HardpointActuatorInfoC* getEventHardpointActuatorInfo();
	virtual m1m3_logevent_ILCWarningC* getEventILCWarning();
	virtual m1m3_logevent_InclinometerSensorWarningC* getEventInclinometerSensorWarning();
	virtual m1m3_logevent_SettingVersionsC* getEventSettingVersions();
	virtual m1m3_logevent_SummaryStateC* getEventSummaryState();

	virtual double getTimestamp();

	virtual void putForceActuatorData();
	virtual void putForceActuatorStatus();
	virtual void putHardpointData();
	virtual void putHardpointStatus();
	virtual void putIMSData();
	virtual void putInclinometerData();
	virtual void putOuterLoopData();

	virtual void logAirSupplyStatus();
	virtual void logAirSupplyWarning();
	virtual void logDisplacementSensorWarning();
	virtual void logErrorCode();
	virtual void logForceActuatorInfo();
	virtual void logHardpointActuatorInfo();
	virtual void logILCWarning();
	virtual void logInclinometerSensorWarning();
	virtual void logSettingVersions();
	virtual void logSummaryState();

	virtual void ackCommandStart(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandApplyOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandClearOffsetForces(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandLowerM1M3(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandApplyAOSCorrectionByBendingModes(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandApplyAOSCorrectionByForces(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandClearAOSCorrection(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandApplyAberrationByBendingModes(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandApplyAberrationByForces(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandClearAberration(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandEnterEngineering(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandExitEngineering(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTestAir(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTestHardpoint(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTestForceActuator(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandMoveHardpointActuators(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandEnableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandDisableHardpointChase(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandAbortRaiseM1M3(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandTranslateM1M3(int32_t commandID, int32_t ackCode, std::string description);
	virtual void ackCommandStopHardpointMotion(int32_t commandID, int32_t ackCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IPUBLISHER_H_ */
