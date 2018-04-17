/*
 * SafetyController.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <SafetyController.h>
#include <M1M3SSPublisher.h>
#include <SafetyControllerSettings.h>
#include <SAL_m1m3C.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SafetyController::SafetyController(M1M3SSPublisher* publisher, SafetyControllerSettings* safetyControllerSettings) {
	Log.Debug("SafetyController: SafetyController()");
	this->publisher = publisher;
	this->safetyControllerSettings = safetyControllerSettings;
	this->errorCodeData = this->publisher->getEventErrorCode();
	this->errorCodeData->Timestamp = this->publisher->getTimestamp();
	this->errorCodeData->ErrorCode = FaultCodes::NoFault;
	this->errorCodeData->DetailedErrorCode = FaultCodes::NoFault;
	this->publisher->logErrorCode();
}

void SafetyController::clearErrorCode() {
	Log.Info("SafetyController: clearErrorCode()");
	this->errorCodeData->Timestamp = this->publisher->getTimestamp();
	this->errorCodeData->DetailedErrorCode= FaultCodes::NoFault;
	this->errorCodeData->ErrorCode= FaultCodes::NoFault;
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
void SafetyController::displacementNotifyInvalidResponse(bool conditionFlag) { this->updateOverride(FaultCodes::DisplacementInvalidResponse, this->safetyControllerSettings->Displacement.FaultOnInvalidResponse, conditionFlag); }
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
void SafetyController::interlockNotifyAuxPowerNetworksOff(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockPowerNetworksOff, this->safetyControllerSettings->Interlock.FaultOnAuxPowerNetworksOff, conditionFlag); }
void SafetyController::interlockNotifyThermalEquipmentOff(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockThermalEquipmentOff, this->safetyControllerSettings->Interlock.FaultOnThermalEquipmentOff, conditionFlag); }
void SafetyController::interlockNotifyAirSupplyOff(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockAirSupplyOff, this->safetyControllerSettings->Interlock.FaultOnAirSupplyOff, conditionFlag); }
void SafetyController::interlockNotifyCabinetDoorOpen(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockGISEarthquake, this->safetyControllerSettings->Interlock.FaultOnCabinetDoorOpen, conditionFlag); }
void SafetyController::interlockNotifyTMAMotionStop(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockTMAMotionStop, this->safetyControllerSettings->Interlock.FaultOnTMAMotionStop, conditionFlag); }
void SafetyController::interlockNotifyGISHeartbeatLost(bool conditionFlag) { this->updateOverride(FaultCodes::InterlockGISHeartbeatLost, this->safetyControllerSettings->Interlock.FaultOnGISHeartbeatLost, conditionFlag); }

void SafetyController::forceControllerNotifySafetyLimit(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerSafetyLimit, this->safetyControllerSettings->ForceController.FaultOnSafetyLimit, conditionFlag); }
void SafetyController::forceControllerNotifyXMomentLimit(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerXMomentLimit, this->safetyControllerSettings->ForceController.FaultOnXMomentLimit, conditionFlag); }
void SafetyController::forceControllerNotifyYMomentLimit(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerYMomentLimit, this->safetyControllerSettings->ForceController.FaultOnYMomentLimit, conditionFlag); }
void SafetyController::forceControllerNotifyZMomentLimit(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerZMomentLimit, this->safetyControllerSettings->ForceController.FaultOnZMomentLimit, conditionFlag); }
void SafetyController::forceControllerNotifyNearNeighborCheck(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerNearNeighborCheck, this->safetyControllerSettings->ForceController.FaultOnNearNeighborCheck, conditionFlag); }
void SafetyController::forceControllerNotifyMagnitudeLimit(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerMagnitudeLimit, this->safetyControllerSettings->ForceController.FaultOnMagnitudeLimit, conditionFlag); }
void SafetyController::forceControllerNotifyFarNeighborCheck(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerFarNeighborCheck, this->safetyControllerSettings->ForceController.FaultOnFarNeighborCheck, conditionFlag); }
void SafetyController::forceControllerNotifyElevationForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerElevationForceClipping, this->safetyControllerSettings->ForceController.FaultOnElevationForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyAzimuthForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerAzimuthForceClipping, this->safetyControllerSettings->ForceController.FaultOnAzimuthForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyThermalForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerThermalForceClipping, this->safetyControllerSettings->ForceController.FaultOnThermalForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyBalanceForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerBalanceForceClipping, this->safetyControllerSettings->ForceController.FaultOnBalanceForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyAccelerationForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerAccelerationForceClipping, this->safetyControllerSettings->ForceController.FaultOnAccelerationForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyActiveOpticNetForceCheck(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerActiveOpticNetForceCheck, this->safetyControllerSettings->ForceController.FaultOnActiveOpticNetForceCheck, conditionFlag); }
void SafetyController::forceControllerNotifyActiveOpticForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerActiveOpticForceClipping, this->safetyControllerSettings->ForceController.FaultOnActiveOpticForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyStaticForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerStaticForceClipping, this->safetyControllerSettings->ForceController.FaultOnStaticForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyAberrationNetForceCheck(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerAberrationNetForceCheck, this->safetyControllerSettings->ForceController.FaultOnAberrationNetForceCheck, conditionFlag); }
void SafetyController::forceControllerNotifyAberrationForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerAberrationForceClipping, this->safetyControllerSettings->ForceController.FaultOnAberrationForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyOffsetForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerOffsetForceClipping, this->safetyControllerSettings->ForceController.FaultOnOffsetForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyVelocityForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerVelocityForceClipping, this->safetyControllerSettings->ForceController.FaultOnVelocityForceClipping, conditionFlag); }
void SafetyController::forceControllerNotifyForceClipping(bool conditionFlag) { this->updateOverride(FaultCodes::ForceControllerForceClipping, this->safetyControllerSettings->ForceController.FaultOnForceClipping, conditionFlag); }

void SafetyController::cellLightNotifyOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::CellLightOutputMismatch, this->safetyControllerSettings->CellLights.FaultOnOutputMismatch, conditionFlag); }
void SafetyController::cellLightNotifySensorMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::CellLightSensorMismatch, this->safetyControllerSettings->CellLights.FaultOnSensorMismatch, conditionFlag); }

void SafetyController::powerControllerNotifyPowerNetworkAOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerPowerNetworkAOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnPowerNetworkAOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyPowerNetworkBOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerPowerNetworkBOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnPowerNetworkBOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyPowerNetworkCOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerPowerNetworkCOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnPowerNetworkCOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyPowerNetworkDOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerPowerNetworkDOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnPowerNetworkDOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyAuxPowerNetworkAOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerAuxPowerNetworkAOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkAOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyAuxPowerNetworkBOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerAuxPowerNetworkBOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkBOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyAuxPowerNetworkCOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerAuxPowerNetworkCOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkCOutputMismatch, conditionFlag); }
void SafetyController::powerControllerNotifyAuxPowerNetworkDOutputMismatch(bool conditionFlag) { this->updateOverride(FaultCodes::PowerControllerAuxPowerNetworkDOutputMismatch, this->safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkDOutputMismatch, conditionFlag); }

void SafetyController::raiseOperationTimeout(bool conditionFlag) { this->updateOverride(FaultCodes::RaiseOperationTimeout, this->safetyControllerSettings->RaiseOperation.FaultOnTimeout, conditionFlag); }

void SafetyController::lowerOperationTimeout(bool conditionFlag) { this->updateOverride(FaultCodes::RaiseOperationTimeout, this->safetyControllerSettings->LowerOperation.FaultOnTimeout, conditionFlag); }

States::Type SafetyController::checkSafety(States::Type preferredNextState) {
	if (this->errorCodeData->DetailedErrorCode != FaultCodes::NoFault) {
		this->publisher->logErrorCode();
		this->errorCodeData->DetailedErrorCode= FaultCodes::NoFault;
		this->errorCodeData->ErrorCode= FaultCodes::NoFault;
		return States::LoweringFaultState;
	}
	return preferredNextState;
}

void SafetyController::updateOverride(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag) {
	bool faultConditionExists = enabledFlag && conditionFlag;
	if (faultConditionExists && this->errorCodeData->DetailedErrorCode == FaultCodes::NoFault) {
		this->errorCodeData->Timestamp = this->publisher->getTimestamp();
		this->errorCodeData->DetailedErrorCode = (int32_t)faultCode;
		this->errorCodeData->ErrorCode = (int32_t)(((int64_t)faultCode) >> 32);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
