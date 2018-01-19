/*
 * SafetyController.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef SAFETYCONTROLLER_H_
#define SAFETYCONTROLLER_H_

#include <FaultCodes.h>
#include <StateTypes.h>

struct m1m3_logevent_ErrorCodeC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyControllerSettings;

class SafetyController {
private:
	M1M3SSPublisher* publisher;
	SafetyControllerSettings* safetyControllerSettings;

	m1m3_logevent_ErrorCodeC* errorCodeData;

public:
	SafetyController(M1M3SSPublisher* publisher, SafetyControllerSettings* safetyControllerSettings);

	void clearErrorCode();

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
	void interlockNotifyPowerNetworksOff(bool conditionFlag);
	void interlockNotifyThermalEquipmentOff(bool conditionFlag);
	void interlockNotifyLaserTrackerOff(bool conditionFlag);
	void interlockNotifyAirSupplyOff(bool conditionFlag);
	void interlockNotifyGISEarthquake(bool conditionFlag);
	void interlockNotifyGISEStop(bool conditionFlag);
	void interlockNotifyTMAMotionStop(bool conditionFlag);
	void interlockNotifyGISHeartbeatLost(bool conditionFlag);

	void forceControllerNotifySafetyLimit(bool conditionFlag);
	void forceControllerNotifyXMomentLimit(bool conditionFlag);
	void forceControllerNotifyYMomentLimit(bool conditionFlag);
	void forceControllerNotifyZMomentLimit(bool conditionFlag);
	void forceControllerNotifyNearNeighborCheck(bool conditionFlag);
	void forceControllerNotifyMagnitudeLimit(bool conditionFlag);
	void forceControllerNotifyFarNeighborCheck(bool conditionFlag);
	void forceControllerNotifyElevationForceClipping(bool conditionFlag);
	void forceControllerNotifyAzimuthForceClipping(bool conditionFlag);
	void forceControllerNotifyTemperatureForceClipping(bool conditionFlag);
	void forceControllerNotifyHardpointOffloadForceClipping(bool conditionFlag);
	void forceControllerNotifyDynamicForceClipping(bool conditionFlag);
	void forceControllerNotifyAOSNetForceCheck(bool conditionFlag);
	void forceControllerNotifyAOSForceClipping(bool conditionFlag);
	void forceControllerNotifyStaticForceClipping(bool conditionFlag);
	void forceControllerNotifyAberrationNetForceCheck(bool conditionFlag);
	void forceControllerNotifyAberrationForceClipping(bool conditionFlag);
	void forceControllerNotifyOffsetForceClipping(bool conditionFlag);

	void cellLightNotifyOutputMismatch(bool conditionFlag);
	void cellLightNotifySensorMismatch(bool conditionFlag);

	void powerControllerNotifyPowerNetworkAOutputMismatch(bool conditionFlag);
	void powerControllerNotifyPowerNetworkBOutputMismatch(bool conditionFlag);
	void powerControllerNotifyPowerNetworkCOutputMismatch(bool conditionFlag);
	void powerControllerNotifyPowerNetworkDOutputMismatch(bool conditionFlag);
	void powerControllerNotifyAuxPowerNetworkAOutputMismatch(bool conditionFlag);
	void powerControllerNotifyAuxPowerNetworkBOutputMismatch(bool conditionFlag);
	void powerControllerNotifyAuxPowerNetworkCOutputMismatch(bool conditionFlag);
	void powerControllerNotifyAuxPowerNetworkDOutputMismatch(bool conditionFlag);

	void raiseOperationTimeout(bool conditionFlag);

	void lowerOperationTimeout(bool conditionFlag);

	States::Type checkSafety(States::Type preferredNextState);

private:
	void updateOverride(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLER_H_ */
