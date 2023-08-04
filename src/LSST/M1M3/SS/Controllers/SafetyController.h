/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef SAFETYCONTROLLER_H_
#define SAFETYCONTROLLER_H_

#include <list>

#include <spdlog/spdlog.h>

#include <SAL_MTM1M3C.h>

#include <FaultCodes.h>
#include <StateTypes.h>
#include <SafetyControllerSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Controls mirror safety. Sets _errorCodeData->errorCode to the first detected
 * fault. checkSafety() method then sends mirror to fault state if any fault is
 * detected.
 *
 * This is a layer above computational layer. Its either check some safety
 * rules, or is signaled safety rules violation. If mirror safety rules are
 * violated, checkSafety() method sends the mirror to a fault state.
 */
class SafetyController {
public:
    SafetyController(SafetyControllerSettings* safetyControllerSettings);

    void clearErrorCode();

    void airControllerNotifyCommandOutputMismatch(bool conditionFlag, bool commanded, bool sensed);
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
    void displacementNotifyInvalidResponse(bool conditionFlag);
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
    void interlockNotifyAuxPowerNetworksOff(bool conditionFlag);
    void interlockNotifyThermalEquipmentOff(bool conditionFlag);
    void interlockNotifyAirSupplyOff(bool conditionFlag);
    void interlockNotifyCabinetDoorOpen(bool conditionFlag);
    void interlockNotifyTMAMotionStop(bool conditionFlag);
    void interlockNotifyGISFault(bool conditionFlag);

    void forceControllerNotifySafetyLimit(bool conditionFlag);
    void forceControllerNotifyXMomentLimit(bool conditionFlag, std::string failed);
    void forceControllerNotifyYMomentLimit(bool conditionFlag, std::string failed);
    void forceControllerNotifyZMomentLimit(bool conditionFlag, std::string failed);
    void forceControllerNotifyNearNeighborCheck(bool conditionFlag, std::string failed, float nominalZ,
                                                float nominalZWarning);
    void forceControllerNotifyMagnitudeLimit(bool conditionFlag, float globalForce);
    void forceControllerNotifyFarNeighborCheck(bool conditionFlag, std::string failed);
    void forceControllerNotifyElevationForceClipping(bool conditionFlag);
    void forceControllerNotifyAzimuthForceClipping(bool conditionFlag);
    void forceControllerNotifyThermalForceClipping(bool conditionFlag);
    void forceControllerNotifyBalanceForceClipping(bool conditionFlag);
    void forceControllerNotifyAccelerationForceClipping(bool conditionFlag);
    void forceControllerNotifyActiveOpticNetForceCheck(bool conditionFlag);
    void forceControllerNotifyActiveOpticForceClipping(bool conditionFlag);
    void forceControllerNotifyStaticForceClipping(bool conditionFlag);
    void forceControllerNotifyOffsetForceClipping(bool conditionFlag);
    void forceControllerNotifyVelocityForceClipping(bool conditionFlag);
    void forceControllerNotifyForceClipping(bool conditionFlag);
    void forceControllerNotifyMeasuredXForceLimit(int actuatorId, float xForce, bool conditionFlag);
    void forceControllerNotifyMeasuredYForceLimit(int actuatorId, float yForce, bool conditionFlag);
    void forceControllerNotifyMeasuredZForceLimit(int actuatorId, float zForce, bool conditionFlag);

    void positionControllerNotifyLimitLow(int hp, bool conditionFlag);
    void positionControllerNotifyLimitHigh(int hp, bool conditionFlag);
    void positionControllerNotifyUnstable(int hp, int32_t unstableCount, int32_t deltaEncoder);
    void positionControllerHighTension(int hp, float weightSupportedPercent);

    void cellLightNotifyOutputMismatch(bool conditionFlag, bool commanded, bool sensed);
    void cellLightNotifySensorMismatch(bool conditionFlag, bool commanded, bool sensed);

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

    void ilcCommunicationTimeout(bool conditionFlag);

    void forceActuatorFollowingError(int actuatorId, int actuatorDataIndex, bool countingWarning,
                                     bool immediateFault);

    void hardpointActuatorBreakawayFault(int actuatorDataIndex, bool conditionFlag);

    /**
     * Triggers hardpoint measured force warning and faults.
     *
     * @param actuatorDataIndex hardpoint actuator index (0 based)
     * @param warningFlag if true, warning was detected
     * @param faultFlag if true, fault was detected
     */
    void hardpointActuatorMeasuredForce(int actuatorDataIndex, bool warningFlag, bool faultFlag);

    /** Checks hardpoint breakway air pressure. Triggers fault if pressure is
     * outside of bounds for more than
     * SafetyControllerSettings/ILC.AirPressureCountThreshold.
     *
     * @param actuatorDataIndex HP actuator index (0-5)
     * @param conditionFlag -1 if bellow low, 1 if above high, 0 for in bounds
     * @param airPressure current air pressure
     */
    void hardpointActuatorAirPressure(int actuatorDataIndex, int conditionFlag, float airPressure);

    void hardpointActuatorFollowingError(int hp, double fePercent);

    void tmaAzimuthTimeout(double currentTimeout);
    void tmaElevationTimeout(double currentTimeout);
    void tmaInclinometerDeviation(double currentDeviation);

    void userPanic();

    /**
     * Check if mirror safety rules are fulfilled. When safety rules are not
     * fulfilled, returns States::LoweringFaultState.
     *
     * @param preferredNextState returns this state if mirror safety is not violated
     *
     * @return preferredNextState if mirror safety is not violated,
     * States::LoweringFaultState otherwise.
     */
    States::Type checkSafety(States::Type preferredNextState);

private:
    template <typename... Args>
    void _updateOverride(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag,
                         std::string errorReport, Args&&... args) {
        bool faultConditionExists = enabledFlag && conditionFlag;
        if (faultConditionExists && _errorCodeData->errorCode == FaultCodes::NoFault) {
            _errorCodeData->errorCode = faultCode;
            _errorCodeData->errorReport = fmt::format(errorReport, args...);
        }
    }

    void _clearError();

    SafetyControllerSettings* _safetyControllerSettings;

    MTM1M3_logevent_errorCodeC* _errorCodeData;

    std::list<int> _ilcCommunicationTimeoutData;
    std::list<int> _forceActuatorFollowingErrorData[FA_COUNT];
    std::list<int> _hardpointActuatorMeasuredForceData[HP_COUNT];
    std::list<int> _hardpointActuatorAirPressureData[HP_COUNT];
    bool _hardpointLimitLowTriggered[HP_COUNT];
    bool _hardpointLimitHighTriggered[HP_COUNT];
    bool _hardpointMeasuredForceWarning[HP_COUNT];
    int _hardpointFeViolations[HP_COUNT];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLER_H_ */
