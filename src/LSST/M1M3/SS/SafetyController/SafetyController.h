#ifndef SAFETYCONTROLLER_H_
#define SAFETYCONTROLLER_H_

#include <FaultCodes.h>
#include <StateTypes.h>
#include <SAL_MTM1M3C.h>
#include <list>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyControllerSettings;

class SafetyController {
private:
    M1M3SSPublisher* publisher;
    SafetyControllerSettings* safetyControllerSettings;

    MTM1M3_logevent_errorCodeC* errorCodeData;

    std::list<int> ilcCommunicationTimeoutData;
    std::list<int> forceActuatorFollowingErrorData[156];
    std::list<int> hardpointActuatorMeasuredForceData[6];
    std::list<int> hardpointActuatorAirPressureData[6];

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
    void forceControllerNotifyThermalForceClipping(bool conditionFlag);
    void forceControllerNotifyBalanceForceClipping(bool conditionFlag);
    void forceControllerNotifyAccelerationForceClipping(bool conditionFlag);
    void forceControllerNotifyActiveOpticNetForceCheck(bool conditionFlag);
    void forceControllerNotifyActiveOpticForceClipping(bool conditionFlag);
    void forceControllerNotifyStaticForceClipping(bool conditionFlag);
    void forceControllerNotifyAberrationNetForceCheck(bool conditionFlag);
    void forceControllerNotifyAberrationForceClipping(bool conditionFlag);
    void forceControllerNotifyOffsetForceClipping(bool conditionFlag);
    void forceControllerNotifyVelocityForceClipping(bool conditionFlag);
    void forceControllerNotifyForceClipping(bool conditionFlag);

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

    void ilcCommunicationTimeout(bool conditionFlag);

    void forceActuatorFollowingError(int actuatorDataIndex, bool conditionFlag);

    void hardpointActuatorLoadCellError(bool conditionFlag);
    void hardpointActuatorMeasuredForce(int actuatorDataIndex, bool conditionFlag);
    void hardpointActuatorAirPressure(int actuatorDataIndex, bool conditionFlag);

    States::Type checkSafety(States::Type preferredNextState);

private:
    void updateOverride(FaultCodes::Type faultCode, bool enabledFlag, bool conditionFlag);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLER_H_ */
