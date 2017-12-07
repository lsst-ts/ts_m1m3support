/*
 * ISafetyController.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <ISafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ISafetyController::~ISafetyController() { }

void ISafetyController::airControllerNotifyCommandOutputMismatch(bool conditionFlag) { }
void ISafetyController::airControllerNotifyCommandSensorMismatch(bool conditionFlag) { }

void ISafetyController::displacementNotifySensorReportsInvalidCommand(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsCommunicationTimeoutError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsDataLengthError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsNumberOfParametersError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsParameterError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsCommunicationError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsIDNumberError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsExpansionLineError(bool conditionFlag) { }
void ISafetyController::displacementNotifySensorReportsWriteControlError(bool conditionFlag) { }
void ISafetyController::displacementNotifyResponseTimeoutError(bool conditionFlag) { }
void ISafetyController::displacementNotifyInvalidLength(bool conditionFlag) { }
void ISafetyController::displacementNotifyUnknownCommand(bool conditionFlag) { }
void ISafetyController::displacementNotifyUnknownProblem(bool conditionFlag) { }

void ISafetyController::inclinometerNotifyResponseTimeout(bool conditionFlag) { }
void ISafetyController::inclinometerNotifyInvalidCRC(bool conditionFlag) { }
void ISafetyController::inclinometerNotifyUnknownAddress(bool conditionFlag) { }
void ISafetyController::inclinometerNotifyUnknownFunction(bool conditionFlag) { }
void ISafetyController::inclinometerNotifyInvalidLength(bool conditionFlag) { }
void ISafetyController::inclinometerNotifySensorReportsIllegalDataAddress(bool conditionFlag) { }
void ISafetyController::inclinometerNotifySensorReportsIllegalFunction(bool conditionFlag) { }
void ISafetyController::inclinometerNotifyUnknownProblem(bool conditionFlag) { }

void ISafetyController::interlockNotifyHeartbeatStateOutputMismatch(bool conditionFlag) { }
void ISafetyController::interlockNotifyCriticalFaultStateOutputMismatch(bool conditionFlag) { }
void ISafetyController::interlockNotifyMirrorLoweringRaisingStateOutputMismatch(bool conditionFlag) { }
void ISafetyController::interlockNotifyMirrorParkedStateOutputMismatch(bool conditionFlag) { }

void ISafetyController::forceControllerNotifySafetyLimit(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyXMomentLimit(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyYMomentLimit(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyZMomentLimit(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyNearNeighborCheck(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyMagnitudeLimit(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyFarNeighborCheck(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyElevationForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyAzimuthForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyTemperatureForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyHardpointOffloadForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyDynamicForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyAOSNetForceCheck(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyAOSForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyStaticForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyAberrationNetForceCheck(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyAberrationForceClipping(bool conditionFlag) { }
void ISafetyController::forceControllerNotifyOffsetForceClipping(bool conditionFlag) { }

void ISafetyController::cellLightNotifyOutputMismatch(bool conditionFlag) { }
void ISafetyController::cellLightNotifySensorMismatch(bool conditionFlag) { }

void ISafetyController::powerControllerNotifyPowerNetworkAOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyPowerNetworkBOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyPowerNetworkCOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyPowerNetworkDOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyAuxPowerNetworkAOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyAuxPowerNetworkBOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyAuxPowerNetworkCOutputMismatch(bool conditionFlag) { }
void ISafetyController::powerControllerNotifyAuxPowerNetworkDOutputMismatch(bool conditionFlag) { }

States::Type ISafetyController::checkSafety(States::Type preferredNextState) { return preferredNextState; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
