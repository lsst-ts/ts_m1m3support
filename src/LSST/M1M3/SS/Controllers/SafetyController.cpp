/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <SafetyController.h>
#include <M1M3SSPublisher.h>
#include <SafetyControllerSettings.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SafetyController::SafetyController(SafetyControllerSettings* safetyControllerSettings) {
    SPDLOG_DEBUG("SafetyController: SafetyController()");
    _safetyControllerSettings = safetyControllerSettings;
    _errorCodeData = M1M3SSPublisher::get().getEventErrorCode();
    _errorCodeData->errorCode = FaultCodes::NoFault;
    M1M3SSPublisher::get().logErrorCode();
    for (int i = 0; i < _safetyControllerSettings->ILC.CommunicationTimeoutPeriod; ++i) {
        _ilcCommunicationTimeoutData.push_back(0);
    }
    for (int i = 0; i < _safetyControllerSettings->ILC.ForceActuatorFollowingErrorPeriod; ++i) {
        for (int j = 0; j < FA_COUNT; ++j) {
            _forceActuatorFollowingErrorData[j].push_back(0);
        }
    }
    for (int i = 0; i < _safetyControllerSettings->ILC.HardpointActuatorMeasuredForcePeriod; ++i) {
        for (int j = 0; j < HP_COUNT; ++j) {
            _hardpointActuatorMeasuredForceData[j].push_back(0);
        }
    }
    for (int i = 0; i < _safetyControllerSettings->ILC.AirPressurePeriod; ++i) {
        for (int j = 0; j < HP_COUNT; ++j) {
            _hardpointActuatorAirPressureData[j].push_back(0);
        }
    }
}

void SafetyController::clearErrorCode() {
    SPDLOG_INFO("SafetyController: clearErrorCode()");
    _errorCodeData->errorCode = FaultCodes::NoFault;
    M1M3SSPublisher::get().logErrorCode();
}

void SafetyController::airControllerNotifyCommandOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::AirControllerCommandOutputMismatch,
                    _safetyControllerSettings->AirController.FaultOnCommandOutputMismatch, conditionFlag);
}
void SafetyController::airControllerNotifyCommandSensorMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::AirControllerCommandSensorMismatch,
                    _safetyControllerSettings->AirController.FaultOnCommandSensorMismatch, conditionFlag);
}

void SafetyController::displacementNotifySensorReportsInvalidCommand(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsInvalidCommand,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsInvalidCommand,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsCommunicationTimeoutError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsCommunicationTimeoutError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsCommunicationTimeoutError,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsDataLengthError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsDataLengthError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsDataLengthError,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsNumberOfParametersError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsNumberOfParametersError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsNumberOfParametersError,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsParameterError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsParameterError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsParameterError,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsCommunicationError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsCommunicationError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsCommunicationError,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsIDNumberError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsIDNumberError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsIDNumberError, conditionFlag);
}
void SafetyController::displacementNotifySensorReportsExpansionLineError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsExpansionLineError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsExpansionLineError,
                    conditionFlag);
}
void SafetyController::displacementNotifySensorReportsWriteControlError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementSensorReportsWriteControlError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsWriteControlError,
                    conditionFlag);
}
void SafetyController::displacementNotifyResponseTimeoutError(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementResponseTimeoutError,
                    _safetyControllerSettings->Displacement.FaultOnResponseTimeoutError, conditionFlag);
}
void SafetyController::displacementNotifyInvalidLength(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementInvalidLength,
                    _safetyControllerSettings->Displacement.FaultOnInvalidLength, conditionFlag);
}
void SafetyController::displacementNotifyInvalidResponse(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementInvalidResponse,
                    _safetyControllerSettings->Displacement.FaultOnInvalidResponse, conditionFlag);
}
void SafetyController::displacementNotifyUnknownCommand(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementUnknownCommand,
                    _safetyControllerSettings->Displacement.FaultOnUnknownCommand, conditionFlag);
}
void SafetyController::displacementNotifyUnknownProblem(bool conditionFlag) {
    _safetyViolated(FaultCodes::DisplacementUnknownProblem,
                    _safetyControllerSettings->Displacement.FaultOnUnknownProblem, conditionFlag);
}

void SafetyController::inclinometerNotifyResponseTimeout(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerResponseTimeout,
                    _safetyControllerSettings->Inclinometer.FaultOnResponseTimeout, conditionFlag);
}
void SafetyController::inclinometerNotifyInvalidCRC(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerInvalidCRC,
                    _safetyControllerSettings->Inclinometer.FaultOnInvalidCRC, conditionFlag);
}
void SafetyController::inclinometerNotifyUnknownAddress(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerUnknownAddress,
                    _safetyControllerSettings->Inclinometer.FaultOnUnknownAddress, conditionFlag);
}
void SafetyController::inclinometerNotifyUnknownFunction(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerUnknownFunction,
                    _safetyControllerSettings->Inclinometer.FaultOnUnknownFunction, conditionFlag);
}
void SafetyController::inclinometerNotifyInvalidLength(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerInvalidLength,
                    _safetyControllerSettings->Inclinometer.FaultOnInvalidLength, conditionFlag);
}
void SafetyController::inclinometerNotifySensorReportsIllegalDataAddress(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerSensorReportsIllegalDataAddress,
                    _safetyControllerSettings->Inclinometer.FaultOnSensorReportsIllegalDataAddress,
                    conditionFlag);
}
void SafetyController::inclinometerNotifySensorReportsIllegalFunction(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerSensorReportsIllegalFunction,
                    _safetyControllerSettings->Inclinometer.FaultOnSensorReportsIllegalFunction,
                    conditionFlag);
}
void SafetyController::inclinometerNotifyUnknownProblem(bool conditionFlag) {
    _safetyViolated(FaultCodes::InclinometerUnknownProblem,
                    _safetyControllerSettings->Inclinometer.FaultOnUnknownProblem, conditionFlag);
}

void SafetyController::interlockNotifyHeartbeatStateOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockHeartbeatStateOutputMismatch,
                    _safetyControllerSettings->Interlock.FaultOnHeartbeatStateOutputMismatch, conditionFlag);
}
void SafetyController::interlockNotifyAuxPowerNetworksOff(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockPowerNetworksOff,
                    _safetyControllerSettings->Interlock.FaultOnAuxPowerNetworksOff, conditionFlag);
}
void SafetyController::interlockNotifyThermalEquipmentOff(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockThermalEquipmentOff,
                    _safetyControllerSettings->Interlock.FaultOnThermalEquipmentOff, conditionFlag);
}
void SafetyController::interlockNotifyAirSupplyOff(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockAirSupplyOff,
                    _safetyControllerSettings->Interlock.FaultOnAirSupplyOff, conditionFlag);
}
void SafetyController::interlockNotifyCabinetDoorOpen(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockGISEarthquake,
                    _safetyControllerSettings->Interlock.FaultOnCabinetDoorOpen, conditionFlag);
}
void SafetyController::interlockNotifyTMAMotionStop(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockTMAMotionStop,
                    _safetyControllerSettings->Interlock.FaultOnTMAMotionStop, conditionFlag);
}
void SafetyController::interlockNotifyGISHeartbeatLost(bool conditionFlag) {
    _safetyViolated(FaultCodes::InterlockGISHeartbeatLost,
                    _safetyControllerSettings->Interlock.FaultOnGISHeartbeatLost, conditionFlag);
}

void SafetyController::forceControllerNotifySafetyLimit(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerSafetyLimit,
                    _safetyControllerSettings->ForceController.FaultOnSafetyLimit, conditionFlag);
}
void SafetyController::forceControllerNotifyXMomentLimit(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerXMomentLimit,
                    _safetyControllerSettings->ForceController.FaultOnXMomentLimit, conditionFlag);
}
void SafetyController::forceControllerNotifyYMomentLimit(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerYMomentLimit,
                    _safetyControllerSettings->ForceController.FaultOnYMomentLimit, conditionFlag);
}
void SafetyController::forceControllerNotifyZMomentLimit(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerZMomentLimit,
                    _safetyControllerSettings->ForceController.FaultOnZMomentLimit, conditionFlag);
}
void SafetyController::forceControllerNotifyNearNeighborCheck(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerNearNeighborCheck,
                    _safetyControllerSettings->ForceController.FaultOnNearNeighborCheck, conditionFlag);
}
void SafetyController::forceControllerNotifyMagnitudeLimit(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerMagnitudeLimit,
                    _safetyControllerSettings->ForceController.FaultOnMagnitudeLimit, conditionFlag);
}
void SafetyController::forceControllerNotifyFarNeighborCheck(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerFarNeighborCheck,
                    _safetyControllerSettings->ForceController.FaultOnFarNeighborCheck, conditionFlag);
}
void SafetyController::forceControllerNotifyElevationForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerElevationForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnElevationForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyAzimuthForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerAzimuthForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnAzimuthForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyThermalForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerThermalForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnThermalForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyBalanceForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerBalanceForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnBalanceForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyAccelerationForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerAccelerationForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnAccelerationForceClipping,
                    conditionFlag);
}
void SafetyController::forceControllerNotifyActiveOpticNetForceCheck(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerActiveOpticNetForceCheck,
                    _safetyControllerSettings->ForceController.FaultOnActiveOpticNetForceCheck,
                    conditionFlag);
}
void SafetyController::forceControllerNotifyActiveOpticForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerActiveOpticForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnActiveOpticForceClipping,
                    conditionFlag);
}
void SafetyController::forceControllerNotifyStaticForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerStaticForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnStaticForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyAberrationNetForceCheck(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerAberrationNetForceCheck,
                    _safetyControllerSettings->ForceController.FaultOnAberrationNetForceCheck, conditionFlag);
}
void SafetyController::forceControllerNotifyAberrationForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerAberrationForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnAberrationForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyOffsetForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerOffsetForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnOffsetForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyVelocityForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerVelocityForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnVelocityForceClipping, conditionFlag);
}
void SafetyController::forceControllerNotifyForceClipping(bool conditionFlag) {
    _safetyViolated(FaultCodes::ForceControllerForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnForceClipping, conditionFlag);
}

void SafetyController::cellLightNotifyOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::CellLightOutputMismatch,
                    _safetyControllerSettings->CellLights.FaultOnOutputMismatch, conditionFlag);
}
void SafetyController::cellLightNotifySensorMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::CellLightSensorMismatch,
                    _safetyControllerSettings->CellLights.FaultOnSensorMismatch, conditionFlag);
}

void SafetyController::powerControllerNotifyPowerNetworkAOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerPowerNetworkAOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkAOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyPowerNetworkBOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerPowerNetworkBOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkBOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyPowerNetworkCOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerPowerNetworkCOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkCOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyPowerNetworkDOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerPowerNetworkDOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkDOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyAuxPowerNetworkAOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerAuxPowerNetworkAOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkAOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyAuxPowerNetworkBOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerAuxPowerNetworkBOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkBOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyAuxPowerNetworkCOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerAuxPowerNetworkCOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkCOutputMismatch,
                    conditionFlag);
}
void SafetyController::powerControllerNotifyAuxPowerNetworkDOutputMismatch(bool conditionFlag) {
    _safetyViolated(FaultCodes::PowerControllerAuxPowerNetworkDOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkDOutputMismatch,
                    conditionFlag);
}

void SafetyController::raiseOperationTimeout(bool conditionFlag) {
    _safetyViolated(FaultCodes::RaiseOperationTimeout,
                    _safetyControllerSettings->RaiseOperation.FaultOnTimeout, conditionFlag);
}

void SafetyController::lowerOperationTimeout(bool conditionFlag) {
    _safetyViolated(FaultCodes::LowerOperationTimeout,
                    _safetyControllerSettings->LowerOperation.FaultOnTimeout, conditionFlag);
}

void SafetyController::ilcCommunicationTimeout(bool conditionFlag) {
    _ilcCommunicationTimeoutData.pop_front();
    _ilcCommunicationTimeoutData.push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (std::list<int>::iterator i = _ilcCommunicationTimeoutData.begin();
         i != _ilcCommunicationTimeoutData.end(); ++i) {
        sum += (*i);
    }
    _safetyViolated(FaultCodes::ILCCommunicationTimeout,
                    _safetyControllerSettings->ILC.FaultOnCommunicationTimeout,
                    sum >= _safetyControllerSettings->ILC.CommunicationTimeoutCountThreshold);
}

void SafetyController::forceActuatorFollowingError(int actuatorDataIndex, bool conditionFlag) {
    _forceActuatorFollowingErrorData[actuatorDataIndex].pop_front();
    _forceActuatorFollowingErrorData[actuatorDataIndex].push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (std::list<int>::iterator i = _forceActuatorFollowingErrorData[actuatorDataIndex].begin();
         i != _forceActuatorFollowingErrorData[actuatorDataIndex].end(); ++i) {
        sum += (*i);
    }
    _safetyViolated(FaultCodes::ForceActuatorFollowingError,
                    _safetyControllerSettings->ILC.FaultOnForceActuatorFollowingError,
                    sum >= _safetyControllerSettings->ILC.ForceActuatorFollowingErrorCountThreshold);
}

void SafetyController::hardpointActuatorLoadCellError(bool conditionFlag) {
    _safetyViolated(FaultCodes::HardpointActuatorLoadCellError,
                    _safetyControllerSettings->ILC.FaultOnHardpointActuatorLoadCellError, conditionFlag);
}
void SafetyController::hardpointActuatorMeasuredForce(int actuatorDataIndex, bool conditionFlag) {
    _hardpointActuatorMeasuredForceData[actuatorDataIndex].pop_front();
    _hardpointActuatorMeasuredForceData[actuatorDataIndex].push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (std::list<int>::iterator i = _hardpointActuatorMeasuredForceData[actuatorDataIndex].begin();
         i != _hardpointActuatorMeasuredForceData[actuatorDataIndex].end(); ++i) {
        sum += (*i);
    }
    _safetyViolated(FaultCodes::HardpointActuatorMeasuredForceError,
                    _safetyControllerSettings->ILC.FaultOnHardpointActuatorMeasuredForce,
                    sum >= _safetyControllerSettings->ILC.HardpointActuatorMeasuredForceCountThreshold);
}
void SafetyController::hardpointActuatorAirPressure(int actuatorDataIndex, bool conditionFlag) {
    _hardpointActuatorAirPressureData[actuatorDataIndex].pop_front();
    _hardpointActuatorAirPressureData[actuatorDataIndex].push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (std::list<int>::iterator i = _hardpointActuatorAirPressureData[actuatorDataIndex].begin();
         i != _hardpointActuatorAirPressureData[actuatorDataIndex].end(); ++i) {
        sum += (*i);
    }
    _safetyViolated(FaultCodes::HardpointActuatorAirPressure,
                    _safetyControllerSettings->ILC.FaultOnAirPressure,
                    sum >= _safetyControllerSettings->ILC.AirPressureCountThreshold);
}

void SafetyController::tmaAzimuthTimeout(double currentTimeout) {
    _safetyViolated(FaultCodes::TMAAzimuthTimeout, _safetyControllerSettings->TMA.AzimuthTimeout > 0,
                    currentTimeout > _safetyControllerSettings->TMA.AzimuthTimeout);
}

void SafetyController::tmaElevationTimeout(double currentTimeout) {
    if (_safetyViolated(FaultCodes::TMAElevationTimeout, _safetyControllerSettings->TMA.ElevationTimeout > 0,
                        currentTimeout > _safetyControllerSettings->TMA.ElevationTimeout)) {
        SPDLOG_ERROR("TMA timeouted {:.2f}", currentTimeout);
    }
}

void SafetyController::tmaInclinometerDeviation(double currentDeviation) {
    _safetyViolated(FaultCodes::TMAInclinometerDeviation,
                    _safetyControllerSettings->TMA.InclinometerDeviation > 0,
                    currentDeviation > _safetyControllerSettings->TMA.InclinometerDeviation);
}

States::Type SafetyController::checkSafety(States::Type preferredNextState) {
    if (_errorCodeData->errorCode != FaultCodes::NoFault) {
        M1M3SSPublisher::get().logErrorCode();
        _errorCodeData->errorCode = FaultCodes::NoFault;
        return States::LoweringFaultState;
    }
    return preferredNextState;
}

bool SafetyController::_safetyViolated(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag) {
    bool faultConditionExists = enabledFlag && conditionFlag;
    if (faultConditionExists && _errorCodeData->errorCode == FaultCodes::NoFault) {
        _errorCodeData->errorCode = faultCode;
    }
    return faultConditionExists;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
