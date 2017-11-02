/*
 * SafetyController.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <SafetyController.h>
#include <IPublisher.h>
#include <SafetyControllerSettings.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SafetyController::SafetyController(IPublisher* publisher, SafetyControllerSettings* safetyControllerSettings) {
	this->publisher = publisher;
	this->safetyControllerSettings = safetyControllerSettings;
	this->errorCodeData = this->publisher->getEventErrorCode();
	this->errorCodeData->Timestamp = this->publisher->getTimestamp();
	this->errorCodeData->ErrorCode = FaultCodes::NoFault;
	this->errorCodeData->DetailedErrorCode = FaultCodes::NoFault;
	this->publisher->logErrorCode();
}

void SafetyController::airControllerNotifyCommandOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::AirControllerCommandOutputMismatch, this->safetyControllerSettings->AirController.FaultOnCommandOutputMismatch, conditionFlag); }
void SafetyController::airControllerNotifyCommandSensorMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::AirControllerCommandSensorMismatch, this->safetyControllerSettings->AirController.FaultOnCommandSensorMismatch, conditionFlag); }

void SafetyController::displacementNotifySensorReportsInvalidCommand(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsInvalidCommand, this->safetyControllerSettings->Displacement.FaultOnSensorReportsInvalidCommand, conditionFlag); }
void SafetyController::displacementNotifySensorReportsCommunicationTimeoutError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsCommunicationTimeoutError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsCommunicationTimeoutError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsDataLengthError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsDataLengthError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsDataLengthError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsNumberOfParametersError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsNumberOfParametersError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsNumberOfParametersError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsParameterError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsParameterError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsParameterError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsCommunicationError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsCommunicationError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsCommunicationError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsIDNumberError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsIDNumberError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsIDNumberError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsExpansionLineError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsExpansionLineError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsExpansionLineError, conditionFlag); }
void SafetyController::displacementNotifySensorReportsWriteControlError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementSensorReportsWriteControlError, this->safetyControllerSettings->Displacement.FaultOnSensorReportsWriteControlError, conditionFlag); }
void SafetyController::displacementNotifyResponseTimeoutError(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementResponseTimeoutError, this->safetyControllerSettings->Displacement.FaultOnResponseTimeoutError, conditionFlag); }
void SafetyController::displacementNotifyInvalidLength(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementInvalidLength, this->safetyControllerSettings->Displacement.FaultOnInvalidLength, conditionFlag); }
void SafetyController::displacementNotifyUnknownCommand(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementUnknownCommand, this->safetyControllerSettings->Displacement.FaultOnUnknownCommand, conditionFlag); }
void SafetyController::displacementNotifyUnknownProblem(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementUnknownProblem, this->safetyControllerSettings->Displacement.FaultOnUnknownProblem, conditionFlag); }

void SafetyController::inclinometerNotifyResponseTimeout(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerResponseTimeout, this->safetyControllerSettings->Inclinometer.FaultOnResponseTimeout, conditionFlag); }
void SafetyController::inclinometerNotifyInvalidCRC(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerInvalidCRC, this->safetyControllerSettings->Inclinometer.FaultOnInvalidCRC, conditionFlag); }
void SafetyController::inclinometerNotifyUnknownAddress(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerUnknownAddress, this->safetyControllerSettings->Inclinometer.FaultOnUnknownAddress, conditionFlag); }
void SafetyController::inclinometerNotifyUnknownFunction(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerUnknownFunction, this->safetyControllerSettings->Inclinometer.FaultOnUnknownFunction, conditionFlag); }
void SafetyController::inclinometerNotifyInvalidLength(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerInvalidLength, this->safetyControllerSettings->Inclinometer.FaultOnInvalidLength, conditionFlag); }
void SafetyController::inclinometerNotifySensorReportsIllegalDataAddress(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerSensorReportsIllegalDataAddress, this->safetyControllerSettings->Inclinometer.FaultOnSensorReportsIllegalDataAddress, conditionFlag); }
void SafetyController::inclinometerNotifySensorReportsIllegalFunction(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerSensorReportsIllegalFunction, this->safetyControllerSettings->Inclinometer.FaultOnSensorReportsIllegalFunction, conditionFlag); }
void SafetyController::inclinometerNotifyUnknownProblem(bool conditionFlag) { this->updateOverride(FaultCodes::InclinometerUnknownProblem, this->safetyControllerSettings->Inclinometer.FaultOnUnknownProblem, conditionFlag); }

void SafetyController::interlockNotifyHeartbeatStateOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockHeartbeatStateOutputMismatch, this->safetyControllerSettings->Interlock.FaultOnHeartbeatStateOutputMismatch, conditionFlag); }
void SafetyController::interlockNotifyCriticalFaultStateOutputMismatch(bool conditionFlag){ this->updateOverride(FaultCodes::InterlockCriticalFaultStateOutputMismatch, this->safetyControllerSettings->Interlock.FaultOnCriticalFaultStateOutputMismatch, conditionFlag); }
void SafetyController::interlockNotifyMirrorLoweringRaisingStateOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockMirrorLoweringRaisingStateOutputMismatch, this->safetyControllerSettings->Interlock.FaultOnMirrorLoweringRaisingStateOutputMismatch, conditionFlag); }
void SafetyController::interlockNotifyMirrorParkedStateOutputMismatch(bool conditionFlag){ this->updateOverride(FaultCodes::InterlockMirrorParkedStateOutputMismatch, this->safetyControllerSettings->Interlock.FaultOnMirrorParkedStateOutputMismatch, conditionFlag); }

States::Type SafetyController::checkSafety(States::Type preferredNextState) {
	if (this->errorCodeData->DetailedErrorCode != FaultCodes::NoFault) {
		this->publisher->logErrorCode();
		this->errorCodeData->DetailedErrorCode= FaultCodes::NoFault;
		this->errorCodeData->ErrorCode= FaultCodes::NoFault;
		return States::FaultState;
	}
	return preferredNextState;
}

void SafetyController::updateOverride(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag) {
	bool faultConditionExists = enabledFlag && conditionFlag;
	if (faultConditionExists && this->errorCodeData->DetailedErrorCode == FaultCodes::NoFault) {
		this->errorCodeData->Timestamp = this->publisher->getTimestamp();
		this->errorCodeData->DetailedErrorCode = (int64_t)faultCode;
		this->errorCodeData->ErrorCode = (int32_t)(((int64_t)faultCode) >> 32);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
