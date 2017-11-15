/*
 * ISafetyController.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef ISAFETYCONTROLLER_H_
#define ISAFETYCONTROLLER_H_

#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISafetyController {
public:
	virtual ~ISafetyController();

	virtual void airControllerNotifyCommandOutputMismatch(bool conditionFlag);
	virtual void airControllerNotifyCommandSensorMismatch(bool conditionFlag);

	virtual void displacementNotifySensorReportsInvalidCommand(bool conditionFlag);
	virtual void displacementNotifySensorReportsCommunicationTimeoutError(bool conditionFlag);
	virtual void displacementNotifySensorReportsDataLengthError(bool conditionFlag);
	virtual void displacementNotifySensorReportsNumberOfParametersError(bool conditionFlag);
	virtual void displacementNotifySensorReportsParameterError(bool conditionFlag);
	virtual void displacementNotifySensorReportsCommunicationError(bool conditionFlag);
	virtual void displacementNotifySensorReportsIDNumberError(bool conditionFlag);
	virtual void displacementNotifySensorReportsExpansionLineError(bool conditionFlag);
	virtual void displacementNotifySensorReportsWriteControlError(bool conditionFlag);
	virtual void displacementNotifyResponseTimeoutError(bool conditionFlag);
	virtual void displacementNotifyInvalidLength(bool conditionFlag);
	virtual void displacementNotifyUnknownCommand(bool conditionFlag);
	virtual void displacementNotifyUnknownProblem(bool conditionFlag);

	virtual void inclinometerNotifyResponseTimeout(bool conditionFlag);
	virtual void inclinometerNotifyInvalidCRC(bool conditionFlag);
	virtual void inclinometerNotifyUnknownAddress(bool conditionFlag);
	virtual void inclinometerNotifyUnknownFunction(bool conditionFlag);
	virtual void inclinometerNotifyInvalidLength(bool conditionFlag);
	virtual void inclinometerNotifySensorReportsIllegalDataAddress(bool conditionFlag);
	virtual void inclinometerNotifySensorReportsIllegalFunction(bool conditionFlag);
	virtual void inclinometerNotifyUnknownProblem(bool conditionFlag);

	virtual void interlockNotifyHeartbeatStateOutputMismatch(bool conditionFlag);
	virtual void interlockNotifyCriticalFaultStateOutputMismatch(bool conditionFlag);
	virtual void interlockNotifyMirrorLoweringRaisingStateOutputMismatch(bool conditionFlag);
	virtual void interlockNotifyMirrorParkedStateOutputMismatch(bool conditionFlag);

	virtual void forceControllerNotifySafetyLimit(bool conditionFlag);
	virtual void forceControllerNotifyXMomentLimit(bool conditionFlag);
	virtual void forceControllerNotifyYMomentLimit(bool conditionFlag);
	virtual void forceControllerNotifyZMomentLimit(bool conditionFlag);
	virtual void forceControllerNotifyNearNeighborCheck(bool conditionFlag);
	virtual void forceControllerNotifyMagnitudeLimit(bool conditionFlag);
	virtual void forceControllerNotifyFarNeighborCheck(bool conditionFlag);
	virtual void forceControllerNotifyElevationForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyAzimuthForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyTemperatureForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyHardpointOffloadForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyDynamicForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyAOSNetForceCheck(bool conditionFlag);
	virtual void forceControllerNotifyAOSForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyStaticForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyAberrationNetForceCheck(bool conditionFlag);
	virtual void forceControllerNotifyAberrationForceClipping(bool conditionFlag);
	virtual void forceControllerNotifyOffsetForceClipping(bool conditionFlag);

	virtual States::Type checkSafety(States::Type preferredNextState);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISAFETYCONTROLLER_H_ */
