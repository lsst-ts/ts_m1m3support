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

States::Type ISafetyController::checkSafety(States::Type preferredNextState) { return preferredNextState; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
