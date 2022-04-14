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

#include <LoweringFaultState.h>
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
    _clearError();
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
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointLimitLowTriggered[i] = false;
        _hardpointLimitHighTriggered[i] = false;
        _hardpointFeViolations[i] = 0;
    }
}

void SafetyController::clearErrorCode() {
    SPDLOG_INFO("SafetyController: clearErrorCode()");
    _clearError();
    M1M3SSPublisher::get().logErrorCode();
}

void SafetyController::airControllerNotifyCommandOutputMismatch(bool conditionFlag, bool commanded,
                                                                bool sensed) {
    _updateOverride(FaultCodes::AirControllerCommandOutputMismatch,
                    _safetyControllerSettings->AirController.FaultOnCommandOutputMismatch, conditionFlag,
                    "Air controller command output mismatch - is {}, should be {}", sensed, commanded);
}

void SafetyController::airControllerNotifyCommandSensorMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::AirControllerCommandSensorMismatch,
                    _safetyControllerSettings->AirController.FaultOnCommandSensorMismatch, conditionFlag,
                    "Air controller sensor mismatch");
}

void SafetyController::displacementNotifySensorReportsInvalidCommand(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsInvalidCommand,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsInvalidCommand, conditionFlag,
                    "Displacement sensor reports invalid command");
}

void SafetyController::displacementNotifySensorReportsCommunicationTimeoutError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsCommunicationTimeoutError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsCommunicationTimeoutError,
                    conditionFlag, "Displacement sensor reports communication timeout");
}

void SafetyController::displacementNotifySensorReportsDataLengthError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsDataLengthError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsDataLengthError,
                    conditionFlag, "Displacement sensor reports data length error");
}

void SafetyController::displacementNotifySensorReportsNumberOfParametersError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsNumberOfParametersError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsNumberOfParametersError,
                    conditionFlag, "Displacement sensor reports number of parameters error");
}

void SafetyController::displacementNotifySensorReportsParameterError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsParameterError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsParameterError, conditionFlag,
                    "Displacement sensor reports parameter error");
}

void SafetyController::displacementNotifySensorReportsCommunicationError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsCommunicationError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsCommunicationError,
                    conditionFlag, "Displacement sensor reports communication error");
}

void SafetyController::displacementNotifySensorReportsIDNumberError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsIDNumberError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsIDNumberError, conditionFlag,
                    "Displacement sensor reports ID number error");
}

void SafetyController::displacementNotifySensorReportsExpansionLineError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsExpansionLineError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsExpansionLineError,
                    conditionFlag, "Displacement sensor reports expansion line error");
}

void SafetyController::displacementNotifySensorReportsWriteControlError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementSensorReportsWriteControlError,
                    _safetyControllerSettings->Displacement.FaultOnSensorReportsWriteControlError,
                    conditionFlag, "Displacement sensor reports write control error");
}

void SafetyController::displacementNotifyResponseTimeoutError(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementResponseTimeoutError,
                    _safetyControllerSettings->Displacement.FaultOnResponseTimeoutError, conditionFlag,
                    "Displacement response timeout error");
}

void SafetyController::displacementNotifyInvalidLength(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementInvalidLength,
                    _safetyControllerSettings->Displacement.FaultOnInvalidLength, conditionFlag,
                    "Displacement invalid message length");
}

void SafetyController::displacementNotifyInvalidResponse(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementInvalidResponse,
                    _safetyControllerSettings->Displacement.FaultOnInvalidResponse, conditionFlag,
                    "Displacement invalid response");
}

void SafetyController::displacementNotifyUnknownCommand(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementUnknownCommand,
                    _safetyControllerSettings->Displacement.FaultOnUnknownCommand, conditionFlag,
                    "Displacement unknown command");
}

void SafetyController::displacementNotifyUnknownProblem(bool conditionFlag) {
    _updateOverride(FaultCodes::DisplacementUnknownProblem,
                    _safetyControllerSettings->Displacement.FaultOnUnknownProblem, conditionFlag,
                    "Displacement unknown problem");
}

void SafetyController::inclinometerNotifyResponseTimeout(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerResponseTimeout,
                    _safetyControllerSettings->Inclinometer.FaultOnResponseTimeout, conditionFlag,
                    "Inclinometer response timeout");
}

void SafetyController::inclinometerNotifyInvalidCRC(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerInvalidCRC,
                    _safetyControllerSettings->Inclinometer.FaultOnInvalidCRC, conditionFlag,
                    "Inclinometer invalid CRC");
}

void SafetyController::inclinometerNotifyUnknownAddress(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerUnknownAddress,
                    _safetyControllerSettings->Inclinometer.FaultOnUnknownAddress, conditionFlag,
                    "Inclinometer unknow address");
}

void SafetyController::inclinometerNotifyUnknownFunction(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerUnknownFunction,
                    _safetyControllerSettings->Inclinometer.FaultOnUnknownFunction, conditionFlag,
                    "Inclinometer unknown function");
}

void SafetyController::inclinometerNotifyInvalidLength(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerInvalidLength,
                    _safetyControllerSettings->Inclinometer.FaultOnInvalidLength, conditionFlag,
                    "Inclinometer invalid length");
}

void SafetyController::inclinometerNotifySensorReportsIllegalDataAddress(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerSensorReportsIllegalDataAddress,
                    _safetyControllerSettings->Inclinometer.FaultOnSensorReportsIllegalDataAddress,
                    conditionFlag, "Inclinometer illegal data address");
}

void SafetyController::inclinometerNotifySensorReportsIllegalFunction(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerSensorReportsIllegalFunction,
                    _safetyControllerSettings->Inclinometer.FaultOnSensorReportsIllegalFunction,
                    conditionFlag, "Inclinometer illegal function");
}

void SafetyController::inclinometerNotifyUnknownProblem(bool conditionFlag) {
    _updateOverride(FaultCodes::InclinometerUnknownProblem,
                    _safetyControllerSettings->Inclinometer.FaultOnUnknownProblem, conditionFlag,
                    "Inclinometer unknown problem");
}

void SafetyController::interlockNotifyHeartbeatStateOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockHeartbeatStateOutputMismatch,
                    _safetyControllerSettings->Interlock.FaultOnHeartbeatStateOutputMismatch, conditionFlag,
                    "Interlock heartbeat state output mismatch");
}

void SafetyController::interlockNotifyAuxPowerNetworksOff(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockPowerNetworksOff,
                    _safetyControllerSettings->Interlock.FaultOnAuxPowerNetworksOff, conditionFlag,
                    "Interlock Aux Power Bus Off");
}

void SafetyController::interlockNotifyThermalEquipmentOff(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockThermalEquipmentOff,
                    _safetyControllerSettings->Interlock.FaultOnThermalEquipmentOff, conditionFlag,
                    "Interlock Thermal Equipment Off");
}

void SafetyController::interlockNotifyAirSupplyOff(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockAirSupplyOff,
                    _safetyControllerSettings->Interlock.FaultOnAirSupplyOff, conditionFlag,
                    "Interlock Air Supply Off");
}

void SafetyController::interlockNotifyCabinetDoorOpen(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockGISEarthquake,
                    _safetyControllerSettings->Interlock.FaultOnCabinetDoorOpen, conditionFlag,
                    "Interlock Cabinet Door Opened");
}

void SafetyController::interlockNotifyTMAMotionStop(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockTMAMotionStop,
                    _safetyControllerSettings->Interlock.FaultOnTMAMotionStop, conditionFlag,
                    "Interlock TMA Motion Stop");
}

void SafetyController::interlockNotifyGISHeartbeatLost(bool conditionFlag) {
    _updateOverride(FaultCodes::InterlockGISHeartbeatLost,
                    _safetyControllerSettings->Interlock.FaultOnGISHeartbeatLost, conditionFlag,
                    "Interlock lost GIS Heartbeat");
}

void SafetyController::forceControllerNotifySafetyLimit(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerSafetyLimit,
                    _safetyControllerSettings->ForceController.FaultOnSafetyLimit, conditionFlag,
                    "Force controller safety limit");
}

void SafetyController::forceControllerNotifyXMomentLimit(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerXMomentLimit,
                    _safetyControllerSettings->ForceController.FaultOnXMomentLimit, conditionFlag,
                    "Force controller X Moment Limit");
}

void SafetyController::forceControllerNotifyYMomentLimit(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerYMomentLimit,
                    _safetyControllerSettings->ForceController.FaultOnYMomentLimit, conditionFlag,
                    "Force controller Y Moment Limit");
}

void SafetyController::forceControllerNotifyZMomentLimit(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerZMomentLimit,
                    _safetyControllerSettings->ForceController.FaultOnZMomentLimit, conditionFlag,
                    "Force controller Z Moment Limit");
}

void SafetyController::forceControllerNotifyNearNeighborCheck(bool conditionFlag, std::string failed,
                                                              float nominalZ, float nominalZWarning) {
    _updateOverride(FaultCodes::ForceControllerNearNeighborCheck,
                    _safetyControllerSettings->ForceController.FaultOnNearNeighborCheck, conditionFlag,
                    "Force controller Near Neighbor Check failed: {} > {} ({})", failed, nominalZWarning,
                    nominalZ);
}

void SafetyController::forceControllerNotifyMagnitudeLimit(bool conditionFlag, float globalForce) {
    _updateOverride(FaultCodes::ForceControllerMagnitudeLimit,
                    _safetyControllerSettings->ForceController.FaultOnMagnitudeLimit, conditionFlag,
                    "Force controller Magnitude Limit crossed {}", globalForce);
}

void SafetyController::forceControllerNotifyFarNeighborCheck(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerFarNeighborCheck,
                    _safetyControllerSettings->ForceController.FaultOnFarNeighborCheck, conditionFlag,
                    "Force controller Far Neighbor Check failed");
}

void SafetyController::forceControllerNotifyElevationForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerElevationForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnElevationForceClipping, conditionFlag,
                    "Force controller Elevation Force Clipping");
}

void SafetyController::forceControllerNotifyAzimuthForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerAzimuthForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnAzimuthForceClipping, conditionFlag,
                    "Force controller Azimuth Force Clipping");
}

void SafetyController::forceControllerNotifyThermalForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerThermalForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnThermalForceClipping, conditionFlag,
                    "Force controller Thermal Force Clipping");
}

void SafetyController::forceControllerNotifyBalanceForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerBalanceForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnBalanceForceClipping, conditionFlag,
                    "Force controller Balance Force Clipping");
}

void SafetyController::forceControllerNotifyAccelerationForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerAccelerationForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnAccelerationForceClipping,
                    conditionFlag, "Force controller Acceleration Force Clipping");
}

void SafetyController::forceControllerNotifyActiveOpticNetForceCheck(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerActiveOpticNetForceCheck,
                    _safetyControllerSettings->ForceController.FaultOnActiveOpticNetForceCheck, conditionFlag,
                    "Force controller Active Optic Net Force Check failed");
}

void SafetyController::forceControllerNotifyActiveOpticForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerActiveOpticForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnActiveOpticForceClipping, conditionFlag,
                    "Force controller Active Optic Force clipping");
}

void SafetyController::forceControllerNotifyStaticForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerStaticForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnStaticForceClipping, conditionFlag,
                    "Force controller Static Force Clipping");
}

void SafetyController::forceControllerNotifyAberrationNetForceCheck(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerAberrationNetForceCheck,
                    _safetyControllerSettings->ForceController.FaultOnAberrationNetForceCheck, conditionFlag,
                    "Force controller Aberration Net Force Check failed");
}

void SafetyController::forceControllerNotifyAberrationForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerAberrationForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnAberrationForceClipping, conditionFlag,
                    "Force controller Aberration Force Clipping");
}

void SafetyController::forceControllerNotifyOffsetForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerOffsetForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnOffsetForceClipping, conditionFlag,
                    "Force controller offset force clipping");
}

void SafetyController::forceControllerNotifyVelocityForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerVelocityForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnVelocityForceClipping, conditionFlag,
                    "Force controller velocity force clipping");
}

void SafetyController::forceControllerNotifyForceClipping(bool conditionFlag) {
    _updateOverride(FaultCodes::ForceControllerForceClipping,
                    _safetyControllerSettings->ForceController.FaultOnForceClipping, conditionFlag,
                    "Force controller force clipping");
}

void SafetyController::positionControllerNotifyLimitLow(int hp, bool conditionFlag) {
    if (conditionFlag) {
        if (_hardpointLimitLowTriggered[hp] == false) {
            _updateOverride(FaultCodes::HardpointActuatorLimitLowError,
                            M1M3SSPublisher::get().getEventDetailedState()->detailedState !=
                                    MTM1M3::MTM1M3_shared_DetailedStates_ParkedEngineeringState,
                            conditionFlag, "Hardpoint #{} hit low limit", hp + 1);
            _hardpointLimitLowTriggered[hp] = true;
        }

    } else {
        if (_hardpointLimitLowTriggered[hp] == true) {
            SPDLOG_INFO("Hardpoint #{} low limit cleared", hp + 1);
            _hardpointLimitLowTriggered[hp] = false;
        }
    }
}

void SafetyController::positionControllerNotifyLimitHigh(int hp, bool conditionFlag) {
    if (conditionFlag) {
        if (_hardpointLimitHighTriggered[hp] == false) {
            _updateOverride(FaultCodes::HardpointActuatorLimitHighError,
                            M1M3SSPublisher::get().getEventDetailedState()->detailedState !=
                                    MTM1M3::MTM1M3_shared_DetailedStates_ParkedEngineeringState,
                            conditionFlag, "Hardpoint #{} hit high limit", hp + 1);
            _hardpointLimitHighTriggered[hp] = true;
        }

    } else {
        if (_hardpointLimitHighTriggered[hp] == true) {
            SPDLOG_INFO("Hardpoint #{} high limit cleared", hp + 1);
            _hardpointLimitHighTriggered[hp] = false;
        }
    }
}

void SafetyController::positionControllerNotifyUnstable(int hp, int32_t unstableCount, int32_t deltaEncoder) {
    _updateOverride(FaultCodes::HardpointUnstableError,
                    _safetyControllerSettings->PositionController.FaultOnUnstableCount > 0,
                    unstableCount >= _safetyControllerSettings->PositionController.FaultOnUnstableCount,
                    "Hardpoint #{} unstable during fine positioning {} times, delta {}", hp + 1,
                    unstableCount, deltaEncoder);
}

void SafetyController::cellLightNotifyOutputMismatch(bool conditionFlag, bool commanded, bool sensed) {
    _updateOverride(FaultCodes::CellLightOutputMismatch,
                    _safetyControllerSettings->CellLights.FaultOnOutputMismatch, conditionFlag,
                    "Cell light output mismatch - is {}, should be {}", sensed, commanded);
}

void SafetyController::cellLightNotifySensorMismatch(bool conditionFlag, bool commanded, bool sensed) {
    _updateOverride(FaultCodes::CellLightSensorMismatch,
                    _safetyControllerSettings->CellLights.FaultOnSensorMismatch, conditionFlag,
                    "Cell light sensor mismatch - is {}, should be {}", sensed, commanded);
}

void SafetyController::powerControllerNotifyPowerNetworkAOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerPowerNetworkAOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkAOutputMismatch,
                    conditionFlag, "Power controller Main Power Bus A mismatch");
}

void SafetyController::powerControllerNotifyPowerNetworkBOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerPowerNetworkBOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkBOutputMismatch,
                    conditionFlag, "Power controller Main Power Bus B mismatch");
}

void SafetyController::powerControllerNotifyPowerNetworkCOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerPowerNetworkCOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkCOutputMismatch,
                    conditionFlag, "Power controller Main Power Bus C mismatch");
}

void SafetyController::powerControllerNotifyPowerNetworkDOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerPowerNetworkDOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnPowerNetworkDOutputMismatch,
                    conditionFlag, "Power controller Main Power Bus D mismatch");
}

void SafetyController::powerControllerNotifyAuxPowerNetworkAOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerAuxPowerNetworkAOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkAOutputMismatch,
                    conditionFlag, "Power controller Auxiliary Power Bus A mismatch");
}

void SafetyController::powerControllerNotifyAuxPowerNetworkBOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerAuxPowerNetworkBOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkBOutputMismatch,
                    conditionFlag, "Power controller Auxiliary Power Bus B mismatch");
}

void SafetyController::powerControllerNotifyAuxPowerNetworkCOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerAuxPowerNetworkCOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkCOutputMismatch,
                    conditionFlag, "Power controller Auxiliary Power Bus C mismatch");
}

void SafetyController::powerControllerNotifyAuxPowerNetworkDOutputMismatch(bool conditionFlag) {
    _updateOverride(FaultCodes::PowerControllerAuxPowerNetworkDOutputMismatch,
                    _safetyControllerSettings->PowerController.FaultOnAuxPowerNetworkDOutputMismatch,
                    conditionFlag, "Power controller Auxiliary Power Bus D mismatch");
}

void SafetyController::raiseOperationTimeout(bool conditionFlag) {
    _updateOverride(FaultCodes::RaiseOperationTimeout,
                    _safetyControllerSettings->RaiseOperation.FaultOnTimeout, conditionFlag,
                    "Raise operation timeouted");
}

void SafetyController::lowerOperationTimeout(bool conditionFlag) {
    _updateOverride(FaultCodes::LowerOperationTimeout,
                    _safetyControllerSettings->LowerOperation.FaultOnTimeout, conditionFlag,
                    "Lower operation timeouted");
}

void SafetyController::ilcCommunicationTimeout(bool conditionFlag) {
    _ilcCommunicationTimeoutData.pop_front();
    _ilcCommunicationTimeoutData.push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (auto i : _ilcCommunicationTimeoutData) {
        sum += i;
    }
    _updateOverride(FaultCodes::ILCCommunicationTimeout,
                    _safetyControllerSettings->ILC.FaultOnCommunicationTimeout,
                    sum >= _safetyControllerSettings->ILC.CommunicationTimeoutCountThreshold,
                    "ILC communication timeouted: {}", sum);
}

void SafetyController::forceActuatorFollowingError(int actuatorDataIndex, bool conditionFlag) {
    _forceActuatorFollowingErrorData[actuatorDataIndex].pop_front();
    _forceActuatorFollowingErrorData[actuatorDataIndex].push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (auto i : _forceActuatorFollowingErrorData[actuatorDataIndex]) {
        sum += i;
    }
    _updateOverride(FaultCodes::ForceActuatorFollowingError,
                    _safetyControllerSettings->ILC.FaultOnForceActuatorFollowingError,
                    sum >= _safetyControllerSettings->ILC.ForceActuatorFollowingErrorCountThreshold,
                    "Force Actuator #{} Following Error {}", actuatorDataIndex + 1, sum);
}

void SafetyController::hardpointActuatorLoadCellError(bool conditionFlag) {
    _updateOverride(FaultCodes::HardpointActuatorLoadCellError,
                    _safetyControllerSettings->ILC.FaultOnHardpointActuatorLoadCellError, conditionFlag,
                    "Hardpoint Actuator Load Cell Error");
}

void SafetyController::hardpointActuatorMeasuredForce(int actuatorDataIndex, bool conditionFlag) {
    _hardpointActuatorMeasuredForceData[actuatorDataIndex].pop_front();
    _hardpointActuatorMeasuredForceData[actuatorDataIndex].push_back(conditionFlag ? 1 : 0);
    int sum = 0;
    for (auto i : _hardpointActuatorMeasuredForceData[actuatorDataIndex]) {
        sum += i;
    }
    _updateOverride(FaultCodes::HardpointActuatorMeasuredForceError,
                    _safetyControllerSettings->ILC.FaultOnHardpointActuatorMeasuredForce,
                    sum >= _safetyControllerSettings->ILC.HardpointActuatorMeasuredForceCountThreshold,
                    "Hardpoint Actuator #{} Measured Forces {}", actuatorDataIndex + 1, sum);
}

void SafetyController::hardpointActuatorAirPressure(int actuatorDataIndex, int conditionFlag,
                                                    float airPressure) {
    _hardpointActuatorAirPressureData[actuatorDataIndex].pop_front();
    _hardpointActuatorAirPressureData[actuatorDataIndex].push_back(conditionFlag);
    int absSum = 0;
    int sum = 0;
    for (auto i : _hardpointActuatorAirPressureData[actuatorDataIndex]) {
        absSum += abs(i);
        sum += i;
    }
    _updateOverride(
            FaultCodes::HardpointActuatorAirPressureLow, _safetyControllerSettings->ILC.FaultOnAirPressure,
            -sum >= _safetyControllerSettings->ILC.AirPressureCountThreshold,
            "Hardpoint Actuator #{} Air Pressure Low {} sum {}", actuatorDataIndex + 1, airPressure, sum);
    _updateOverride(
            FaultCodes::HardpointActuatorAirPressureHigh, _safetyControllerSettings->ILC.FaultOnAirPressure,
            sum >= _safetyControllerSettings->ILC.AirPressureCountThreshold,
            "Hardpoint Actuator #{} Air Pressure High {} sum {}", actuatorDataIndex + 1, airPressure, sum);
    _updateOverride(FaultCodes::HardpointActuatorAirPressureOutside,
                    _safetyControllerSettings->ILC.FaultOnAirPressure,
                    absSum >= _safetyControllerSettings->ILC.AirPressureCountThreshold,
                    "Hardpoint Actuator #{} Air Pressure Oscillates {} absSum {} sum {}",
                    actuatorDataIndex + 1, airPressure, absSum, sum);
}

void SafetyController::hardpointActuatorFollowingError(int hp, double fePercent) {
    double feRange = _safetyControllerSettings->PositionController.FollowingErrorPercentage;
    int feCounts = _safetyControllerSettings->PositionController.FaultNumberOfFollowingErrors;
    double feObserved = fabs(100 - fePercent);
    _updateOverride(FaultCodes::HardpointActuatorFollowingError,
                    (feCounts >= 0) && (_hardpointFeViolations[hp] > feCounts), feObserved > feRange,
                    "Hardpoint {} following error out of range: {:.2f}", hp, fePercent);
    if (feObserved > feRange) {
        _hardpointFeViolations[hp]++;
    } else {
        _hardpointFeViolations[hp] = 0;
    }
}

void SafetyController::tmaAzimuthTimeout(double currentTimeout) {
    _updateOverride(FaultCodes::TMAAzimuthTimeout, _safetyControllerSettings->TMA.AzimuthTimeout > 0,
                    fabs(currentTimeout) > _safetyControllerSettings->TMA.AzimuthTimeout,
                    "TMA Azimuth timeouted ({:.3f}s)", currentTimeout);
}

void SafetyController::tmaElevationTimeout(double currentTimeout) {
    _updateOverride(FaultCodes::TMAElevationTimeout, _safetyControllerSettings->TMA.ElevationTimeout > 0,
                    fabs(currentTimeout) > _safetyControllerSettings->TMA.ElevationTimeout,
                    "TMA Elevation timeouted ({:.3f}s)", currentTimeout);
}

void SafetyController::tmaInclinometerDeviation(double currentDeviation) {
    _updateOverride(FaultCodes::TMAInclinometerDeviation,
                    _safetyControllerSettings->TMA.InclinometerDeviation > 0,
                    fabs(currentDeviation) > _safetyControllerSettings->TMA.InclinometerDeviation,
                    "TMA Elevation - inclinometer mismatch: {:.3f} deg", currentDeviation);
}

void SafetyController::userPanic() {
    _updateOverride(FaultCodes::UserPanic, true, true,
                    "User submitted Panic command (pushed panic button,..)");
}

States::Type SafetyController::checkSafety(States::Type preferredNextState) {
    if (_errorCodeData->errorCode != FaultCodes::NoFault) {
        // shall first make sure mirror is faulted, before performing anything else (logging,..)
        LoweringFaultState::ensureFaulted();
        M1M3SSPublisher::get().logErrorCode();
        SPDLOG_ERROR("Faulted ({}): {}", _errorCodeData->errorCode, _errorCodeData->errorReport);
        _clearError();
        return States::LoweringFaultState;
    }
    return preferredNextState;
}

void SafetyController::_clearError() {
    _errorCodeData->errorCode = FaultCodes::NoFault;
    _errorCodeData->errorReport = "Error cleared";
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
