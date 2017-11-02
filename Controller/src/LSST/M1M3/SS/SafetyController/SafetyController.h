/*
 * SafetyController.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef SAFETYCONTROLLER_H_
#define SAFETYCONTROLLER_H_

#include <ISafetyController.h>
#include <FaultCodes.h>

struct m1m3_logevent_ErrorCodeC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class SafetyControllerSettings;

class SafetyController: public ISafetyController {
private:
	IPublisher* publisher;
	SafetyControllerSettings* safetyControllerSettings;

	m1m3_logevent_ErrorCodeC* errorCodeData;

public:
	SafetyController(IPublisher* publisher, SafetyControllerSettings* safetyControllerSettings);

	void airControllerNotifyCommandOutputMismatch(bool conditionFlag);
	void airControllerNotifyCommandSensorMismatch(bool conditionFlag);

	void displacementNotifySensorReportsInvalidCommand(bool conditionFlag);
	void displacementNotifySensorReportsCommunicationTimeoutError(bool conditionFlag);
	void displacementNotifySensorReportsDataLengthError(bool conditionFlag);
	void displacementNotifySensorReportsNumberOfParametersError(bool conditionFlag);
	void displacementNotifySensorReportsParameterError(bool conditionFlag);
	void displacementNotifySensorReportsCommunicationError(bool conditionFlag);
	void displacementNotifySensorReportsIDNumberError(bool conditionFlag);
	void displacementNotifySensorReportsExpansionLineError(bool conditionFlag);
	void displacementNotifySensorReportsWriteControlError(bool conditionFlag);
	void displacementNotifyResponseTimeoutError(bool conditionFlag);
	void displacementNotifyInvalidLength(bool conditionFlag);
	void displacementNotifyUnknownCommand(bool conditionFlag);
	void displacementNotifyUnknownProblem(bool conditionFlag);

	void inclinometerNotifyResponseTimeout(bool conditionFlag);
	void inclinometerNotifyInvalidCRC(bool conditionFlag);
	void inclinometerNotifyUnknownAddress(bool conditionFlag);
	void inclinometerNotifyUnknownFunction(bool conditionFlag);
	void inclinometerNotifyInvalidLength(bool conditionFlag);
	void inclinometerNotifySensorReportsIllegalDataAddress(bool conditionFlag);
	void inclinometerNotifySensorReportsIllegalFunction(bool conditionFlag);
	void inclinometerNotifyUnknownProblem(bool conditionFlag);

	void interlockNotifyHeartbeatStateOutputMismatch(bool conditionFlag);
	void interlockNotifyCriticalFaultStateOutputMismatch(bool conditionFlag);
	void interlockNotifyMirrorLoweringRaisingStateOutputMismatch(bool conditionFlag);
	void interlockNotifyMirrorParkedStateOutputMismatch(bool conditionFlag);

	States::Type checkSafety(States::Type preferredNextState);

private:
	void updateOverride(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLER_H_ */
