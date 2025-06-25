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
#include <spdlog/spdlog.h>

#include <BoosterValveStatus.h>
#include <M1M3SSPublisher.h>

extern const char *VERSION;

using namespace LSST::M1M3::SS;

M1M3SSPublisher::M1M3SSPublisher(token) : _m1m3SAL(NULL) {
    SPDLOG_DEBUG("M1M3SSPublisher: M1M3SSPublisher()");
    _eventConfigurationApplied.otherInfo = "forceActuatorSettings";
}

void M1M3SSPublisher::setSAL(std::shared_ptr<SAL_MTM1M3> m1m3SAL) {
    _m1m3SAL = m1m3SAL;

    SPDLOG_DEBUG("M1M3SSPublisher: Initializing SAL Telemetry");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_accelerometerData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_forceActuatorData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_gyroData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_hardpointActuatorData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_hardpointMonitorData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_imsData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_inclinometerData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_outerLoopData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_pidData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_powerSupplyData");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedAccelerationForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedAzimuthForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedBalanceForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedCylinderForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedElevationForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedThermalForces");
    _m1m3SAL->salTelemetryPub((char *)"MTM1M3_appliedVelocityForces");

    SPDLOG_DEBUG("M1M3SSPublisher: Initializing SAL Events");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_accelerometerSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_accelerometerWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_airSupplyStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_airSupplyWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_appliedActiveOpticForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_appliedOffsetForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_appliedStaticForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_boosterValveSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_boosterValveStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_configurationsAvailable");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_configurationApplied");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_cellLightStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_cellLightWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_commandRejectionWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_detailedState");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_displacementSensorSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_displacementSensorWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_enabledForceActuators");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_errorCode");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorBumpTestStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorFollowingErrorCounter");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorForceWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorInfo");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorState");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceActuatorWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceControllerState");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_forceSetpointWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_gyroSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_gyroWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointActuatorInfo");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointActuatorSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointActuatorState");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointActuatorWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointMonitorInfo");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointMonitorState");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointMonitorWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_hardpointTestStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_heartbeat");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_ilcWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_inclinometerSensorWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_inclinometerSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_interlockStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_interlockWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_logLevel");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_pidInfo");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_pidSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_positionControllerSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_powerStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_powerSupplyStatus");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_powerWarning");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedAccelerationForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedActiveOpticForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedAzimuthForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedBalanceForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedCylinderForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedElevationForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedOffsetForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedStaticForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedThermalForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_preclippedVelocityForces");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_raisingLoweringInfo");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_simulationMode");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_slewControllerSettings");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_softwareVersions");
    _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_summaryState");
}

void M1M3SSPublisher::reset() {
    BoosterValveStatus::instance().setUserTriggered(false);

    // as all comparision uses != for change detection, this will make the
    // expression true and so updates will be send
    _previousEventAppliedActiveOpticForces.fz = NAN;
    _previousEventAppliedOffsetForces.fx = NAN;
    _previousEventAppliedStaticForces.fx = NAN;
}

void M1M3SSPublisher::putAccelerometerData() { _m1m3SAL->putSample_accelerometerData(&_accelerometerData); }
void M1M3SSPublisher::putGyroData() { _m1m3SAL->putSample_gyroData(&_gyroData); }
void M1M3SSPublisher::putHardpointActuatorData() {
    _m1m3SAL->putSample_hardpointActuatorData(&_hardpointActuatorData);
}
void M1M3SSPublisher::putHardpointMonitorData() {
    _m1m3SAL->putSample_hardpointMonitorData(&_hardpointMonitorData);
}
void M1M3SSPublisher::putIMSData() { _m1m3SAL->putSample_imsData(&_imsData); }
void M1M3SSPublisher::putInclinometerData() { _m1m3SAL->putSample_inclinometerData(&_inclinometerData); }
void M1M3SSPublisher::putOuterLoopData() { _m1m3SAL->putSample_outerLoopData(&_outerLoopData); }
void M1M3SSPublisher::putPIDData() { _m1m3SAL->putSample_pidData(&_pidData); }
void M1M3SSPublisher::putPowerSupplyData() { _m1m3SAL->putSample_powerSupplyData(&_powerSupplyData); }

void M1M3SSPublisher::logAccelerometerWarning() {
    _eventAccelerometerWarning.anyWarning = _eventAccelerometerWarning.responseTimeout;
    _m1m3SAL->logEvent_accelerometerWarning(&_eventAccelerometerWarning, 0);
    _previousEventAccelerometerWarning = _eventAccelerometerWarning;
}

void M1M3SSPublisher::tryLogAccelerometerWarning() {
    if (_eventAccelerometerWarning.responseTimeout != _previousEventAccelerometerWarning.responseTimeout) {
        logAccelerometerWarning();
    }
}

void M1M3SSPublisher::logAirSupplyWarning() {
    _eventAirSupplyWarning.anyWarning =
            _eventAirSupplyWarning.commandOutputMismatch || _eventAirSupplyWarning.commandSensorMismatch;
    _m1m3SAL->logEvent_airSupplyWarning(&_eventAirSupplyWarning, 0);
    _previousEventAirSupplyWarning = _eventAirSupplyWarning;
}

void M1M3SSPublisher::tryLogAirSupplyWarning() {
    if (_eventAirSupplyWarning.commandOutputMismatch !=
                _previousEventAirSupplyWarning.commandOutputMismatch ||
        _eventAirSupplyWarning.commandSensorMismatch !=
                _previousEventAirSupplyWarning.commandSensorMismatch) {
        logAirSupplyWarning();
    }
}

void M1M3SSPublisher::logAppliedAccelerationForces() {
    _m1m3SAL->putSample_appliedAccelerationForces(&_appliedAccelerationForces);
}

void M1M3SSPublisher::logAppliedActiveOpticForces() {
    bool changeDetected = _eventAppliedActiveOpticForces.fz != _previousEventAppliedActiveOpticForces.fz ||
                          _eventAppliedActiveOpticForces.mx != _previousEventAppliedActiveOpticForces.mx ||
                          _eventAppliedActiveOpticForces.my != _previousEventAppliedActiveOpticForces.my;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventAppliedActiveOpticForces.zForces[i] !=
                                                   _previousEventAppliedActiveOpticForces.zForces[i];
    }
    if (changeDetected) {
        _m1m3SAL->logEvent_appliedActiveOpticForces(&_eventAppliedActiveOpticForces, 0);
        _previousEventAppliedActiveOpticForces = _eventAppliedActiveOpticForces;
    }
}

void M1M3SSPublisher::logAppliedAzimuthForces() {
    _m1m3SAL->putSample_appliedAzimuthForces(&_appliedAzimuthForces);
}

void M1M3SSPublisher::logAppliedBalanceForces() {
    _m1m3SAL->putSample_appliedBalanceForces(&_appliedBalanceForces);
}

void M1M3SSPublisher::logAppliedCylinderForces() {
    _m1m3SAL->putSample_appliedCylinderForces(&_appliedCylinderForces);
}

void M1M3SSPublisher::logAppliedElevationForces() {
    _m1m3SAL->putSample_appliedElevationForces(&appliedElevationForces);
}

void M1M3SSPublisher::logAppliedForces() { _m1m3SAL->putSample_appliedForces(&_appliedForces); }

void M1M3SSPublisher::logAppliedOffsetForces() {
    bool changeDetected = _eventAppliedOffsetForces.fx != _previousEventAppliedOffsetForces.fx ||
                          _eventAppliedOffsetForces.fy != _previousEventAppliedOffsetForces.fy ||
                          _eventAppliedOffsetForces.fz != _previousEventAppliedOffsetForces.fz ||
                          _eventAppliedOffsetForces.mx != _previousEventAppliedOffsetForces.mx ||
                          _eventAppliedOffsetForces.my != _previousEventAppliedOffsetForces.my ||
                          _eventAppliedOffsetForces.mz != _previousEventAppliedOffsetForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 _eventAppliedOffsetForces.xForces[i] != _previousEventAppliedOffsetForces.xForces[i]) ||
                (i < 100 &&
                 _eventAppliedOffsetForces.yForces[i] != _previousEventAppliedOffsetForces.yForces[i]) ||
                (_eventAppliedOffsetForces.zForces[i] != _previousEventAppliedOffsetForces.zForces[i]);
    }
    if (changeDetected) {
        _m1m3SAL->logEvent_appliedOffsetForces(&_eventAppliedOffsetForces, 0);
        _previousEventAppliedOffsetForces = _eventAppliedOffsetForces;
    }
}

void M1M3SSPublisher::logAppliedStaticForces() {
    bool changeDetected = eventAppliedStaticForces.fx != _previousEventAppliedStaticForces.fx ||
                          eventAppliedStaticForces.fy != _previousEventAppliedStaticForces.fy ||
                          eventAppliedStaticForces.fz != _previousEventAppliedStaticForces.fz ||
                          eventAppliedStaticForces.mx != _previousEventAppliedStaticForces.mx ||
                          eventAppliedStaticForces.my != _previousEventAppliedStaticForces.my ||
                          eventAppliedStaticForces.mz != _previousEventAppliedStaticForces.mz;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 12 &&
                 eventAppliedStaticForces.xForces[i] != _previousEventAppliedStaticForces.xForces[i]) ||
                (i < 100 &&
                 eventAppliedStaticForces.yForces[i] != _previousEventAppliedStaticForces.yForces[i]) ||
                (eventAppliedStaticForces.zForces[i] != _previousEventAppliedStaticForces.zForces[i]);
    }
    if (changeDetected) {
        _m1m3SAL->logEvent_appliedStaticForces(&eventAppliedStaticForces, 0);
        _previousEventAppliedStaticForces = eventAppliedStaticForces;
    }
}

void M1M3SSPublisher::logAppliedThermalForces() {
    _m1m3SAL->putSample_appliedThermalForces(&_appliedThermalForces);
}

void M1M3SSPublisher::logAppliedVelocityForces() {
    _m1m3SAL->putSample_appliedVelocityForces(&_appliedVelocityForces);
}

void M1M3SSPublisher::logCellLightStatus() {
    _m1m3SAL->logEvent_cellLightStatus(&_eventCellLightStatus, 0);
    _previousEventCellLightStatus = _eventCellLightStatus;
}

void M1M3SSPublisher::tryLogCellLightStatus() {
    if (_eventCellLightStatus.cellLightsCommandedOn != _previousEventCellLightStatus.cellLightsCommandedOn ||
        _eventCellLightStatus.cellLightsOutputOn != _previousEventCellLightStatus.cellLightsOutputOn ||
        _eventCellLightStatus.cellLightsOn != _previousEventCellLightStatus.cellLightsOn) {
        logCellLightStatus();
    }
}

void M1M3SSPublisher::logCellLightWarning() {
    _eventCellLightWarning.anyWarning = _eventCellLightWarning.cellLightsOutputMismatch ||
                                        _eventCellLightWarning.cellLightsSensorMismatch;
    _m1m3SAL->logEvent_cellLightWarning(&_eventCellLightWarning, 0);
    _previousEventCellLightWarning = _eventCellLightWarning;
}

void M1M3SSPublisher::tryLogCellLightWarning() {
    if (_eventCellLightWarning.cellLightsOutputMismatch !=
                _previousEventCellLightWarning.cellLightsOutputMismatch ||
        _eventCellLightWarning.cellLightsSensorMismatch !=
                _previousEventCellLightWarning.cellLightsSensorMismatch) {
        logCellLightWarning();
    }
}

void M1M3SSPublisher::logCommandRejectionWarning() {
    _m1m3SAL->logEvent_commandRejectionWarning(&_eventCommandRejectionWarning, 0);
    _previousEventCommandRejectionWarning = _eventCommandRejectionWarning;
}

void M1M3SSPublisher::logCommandRejectionWarning(std::string command, std::string reason) {
    _eventCommandRejectionWarning.timestamp = getTimestamp();
    _eventCommandRejectionWarning.command = command;
    _eventCommandRejectionWarning.reason = reason;
    logCommandRejectionWarning();
}

void M1M3SSPublisher::logDisplacementSensorWarning() {
    _eventDisplacementSensorWarning.anyWarning =
            _eventDisplacementSensorWarning.sensorReportsInvalidCommand ||
            _eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
            _eventDisplacementSensorWarning.sensorReportsDataLengthError ||
            _eventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
            _eventDisplacementSensorWarning.sensorReportsParameterError ||
            _eventDisplacementSensorWarning.sensorReportsCommunicationError ||
            _eventDisplacementSensorWarning.sensorReportsIDNumberError ||
            _eventDisplacementSensorWarning.sensorReportsExpansionLineError ||
            _eventDisplacementSensorWarning.sensorReportsWriteControlError ||
            _eventDisplacementSensorWarning.responseTimeout ||
            _eventDisplacementSensorWarning.invalidLength ||
            _eventDisplacementSensorWarning.invalidResponse ||
            _eventDisplacementSensorWarning.unknownCommand || _eventDisplacementSensorWarning.unknownProblem;
    _m1m3SAL->logEvent_displacementSensorWarning(&_eventDisplacementSensorWarning, 0);
    _previousEventDisplacementSensorWarning = _eventDisplacementSensorWarning;
}

void M1M3SSPublisher::tryLogDisplacementSensorWarning() {
    if (_eventDisplacementSensorWarning.sensorReportsInvalidCommand !=
                _previousEventDisplacementSensorWarning.sensorReportsInvalidCommand ||
        _eventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError !=
                _previousEventDisplacementSensorWarning.sensorReportsCommunicationTimeoutError ||
        _eventDisplacementSensorWarning.sensorReportsDataLengthError !=
                _previousEventDisplacementSensorWarning.sensorReportsDataLengthError ||
        _eventDisplacementSensorWarning.sensorReportsNumberOfParametersError !=
                _previousEventDisplacementSensorWarning.sensorReportsNumberOfParametersError ||
        _eventDisplacementSensorWarning.sensorReportsParameterError !=
                _previousEventDisplacementSensorWarning.sensorReportsParameterError ||
        _eventDisplacementSensorWarning.sensorReportsCommunicationError !=
                _previousEventDisplacementSensorWarning.sensorReportsCommunicationError ||
        _eventDisplacementSensorWarning.sensorReportsIDNumberError !=
                _previousEventDisplacementSensorWarning.sensorReportsIDNumberError ||
        _eventDisplacementSensorWarning.sensorReportsExpansionLineError !=
                _previousEventDisplacementSensorWarning.sensorReportsExpansionLineError ||
        _eventDisplacementSensorWarning.sensorReportsWriteControlError !=
                _previousEventDisplacementSensorWarning.sensorReportsWriteControlError ||
        _eventDisplacementSensorWarning.responseTimeout !=
                _previousEventDisplacementSensorWarning.responseTimeout ||
        _eventDisplacementSensorWarning.invalidLength !=
                _previousEventDisplacementSensorWarning.invalidLength ||
        _eventDisplacementSensorWarning.invalidResponse !=
                _previousEventDisplacementSensorWarning.invalidResponse ||
        _eventDisplacementSensorWarning.unknownCommand !=
                _previousEventDisplacementSensorWarning.unknownCommand ||
        _eventDisplacementSensorWarning.unknownProblem !=
                _previousEventDisplacementSensorWarning.unknownProblem) {
        logDisplacementSensorWarning();
    }
}

void M1M3SSPublisher::logErrorCode() {
    _m1m3SAL->logEvent_errorCode(&_eventErrorCode, 0);
    _previousEventErrorCode = _eventErrorCode;
}

void M1M3SSPublisher::tryLogErrorCode() {
    if (_eventErrorCode.errorCode != _previousEventErrorCode.errorCode) {
        logErrorCode();
    }
}

void M1M3SSPublisher::logForceActuatorState() {
    _m1m3SAL->logEvent_forceActuatorState(&_eventForceActuatorState, 0);
    _previousEventForceActuatorState = _eventForceActuatorState;
}

void M1M3SSPublisher::tryLogForceActuatorState() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventForceActuatorState.ilcState[i] != _previousEventForceActuatorState.ilcState[i];
    }
    if (changeDetected) {
        logForceActuatorState();
    }
}

void M1M3SSPublisher::logForceSetpointWarning() {
    _eventForceSetpointWarning.anySafetyLimitWarning = false;
    _eventForceSetpointWarning.anyNearNeighborWarning = false;
    _eventForceSetpointWarning.anyFarNeighborWarning = false;
    _eventForceSetpointWarning.anyElevationForceWarning = false;
    _eventForceSetpointWarning.anyAzimuthForceWarning = false;
    _eventForceSetpointWarning.anyThermalForceWarning = false;
    _eventForceSetpointWarning.anyBalanceForceWarning = false;
    _eventForceSetpointWarning.anyAccelerationForceWarning = false;
    _eventForceSetpointWarning.anyVelocityForceWarning = false;
    _eventForceSetpointWarning.anyActiveOpticForceWarning = false;
    _eventForceSetpointWarning.anyStaticForceWarning = false;
    _eventForceSetpointWarning.anyOffsetForceWarning = false;
    _eventForceSetpointWarning.anyForceWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        _eventForceSetpointWarning.anySafetyLimitWarning = _eventForceSetpointWarning.anySafetyLimitWarning ||
                                                           _eventForceSetpointWarning.safetyLimitWarning[i];
        _eventForceSetpointWarning.anyNearNeighborWarning =
                _eventForceSetpointWarning.anyNearNeighborWarning ||
                _eventForceSetpointWarning.nearNeighborWarning[i];
        _eventForceSetpointWarning.anyFarNeighborWarning = _eventForceSetpointWarning.anyFarNeighborWarning ||
                                                           _eventForceSetpointWarning.farNeighborWarning[i];
        _eventForceSetpointWarning.anyElevationForceWarning =
                _eventForceSetpointWarning.anyElevationForceWarning ||
                _eventForceSetpointWarning.elevationForceWarning[i];
        _eventForceSetpointWarning.anyAzimuthForceWarning =
                _eventForceSetpointWarning.anyAzimuthForceWarning ||
                _eventForceSetpointWarning.azimuthForceWarning[i];
        _eventForceSetpointWarning.anyThermalForceWarning =
                _eventForceSetpointWarning.anyThermalForceWarning ||
                _eventForceSetpointWarning.thermalForceWarning[i];
        _eventForceSetpointWarning.anyBalanceForceWarning =
                _eventForceSetpointWarning.anyBalanceForceWarning ||
                _eventForceSetpointWarning.balanceForceWarning[i];
        _eventForceSetpointWarning.anyAccelerationForceWarning =
                _eventForceSetpointWarning.anyAccelerationForceWarning ||
                _eventForceSetpointWarning.accelerationForceWarning[i];
        _eventForceSetpointWarning.anyVelocityForceWarning =
                _eventForceSetpointWarning.anyVelocityForceWarning ||
                _eventForceSetpointWarning.velocityForceWarning[i];
        _eventForceSetpointWarning.anyActiveOpticForceWarning =
                _eventForceSetpointWarning.anyActiveOpticForceWarning ||
                _eventForceSetpointWarning.activeOpticForceWarning[i];
        _eventForceSetpointWarning.anyStaticForceWarning = _eventForceSetpointWarning.anyStaticForceWarning ||
                                                           _eventForceSetpointWarning.staticForceWarning[i];
        _eventForceSetpointWarning.anyOffsetForceWarning = _eventForceSetpointWarning.anyOffsetForceWarning ||
                                                           _eventForceSetpointWarning.offsetForceWarning[i];
        _eventForceSetpointWarning.anyForceWarning =
                _eventForceSetpointWarning.anyForceWarning || _eventForceSetpointWarning.forceWarning[i];
    }
    _eventForceSetpointWarning.anyWarning =
            _eventForceSetpointWarning.anySafetyLimitWarning || _eventForceSetpointWarning.xMomentWarning ||
            _eventForceSetpointWarning.yMomentWarning || _eventForceSetpointWarning.zMomentWarning ||
            _eventForceSetpointWarning.anyNearNeighborWarning ||
            _eventForceSetpointWarning.magnitudeWarning || _eventForceSetpointWarning.anyFarNeighborWarning ||
            _eventForceSetpointWarning.anyElevationForceWarning ||
            _eventForceSetpointWarning.anyAzimuthForceWarning ||
            _eventForceSetpointWarning.anyThermalForceWarning ||
            _eventForceSetpointWarning.anyBalanceForceWarning ||
            _eventForceSetpointWarning.anyVelocityForceWarning ||
            _eventForceSetpointWarning.anyAccelerationForceWarning ||
            _eventForceSetpointWarning.activeOpticNetForceWarning ||
            _eventForceSetpointWarning.anyActiveOpticForceWarning ||
            _eventForceSetpointWarning.anyStaticForceWarning ||
            _eventForceSetpointWarning.anyOffsetForceWarning || _eventForceSetpointWarning.anyForceWarning;
    _m1m3SAL->logEvent_forceSetpointWarning(&_eventForceSetpointWarning, 0);
    _previousEventForceSetpointWarning = _eventForceSetpointWarning;
}

void M1M3SSPublisher::tryLogForceSetpointWarning() {
    bool changeDetected =
            _eventForceSetpointWarning.xMomentWarning != _previousEventForceSetpointWarning.xMomentWarning ||
            _eventForceSetpointWarning.yMomentWarning != _previousEventForceSetpointWarning.yMomentWarning ||
            _eventForceSetpointWarning.zMomentWarning != _previousEventForceSetpointWarning.zMomentWarning ||
            _eventForceSetpointWarning.magnitudeWarning !=
                    _previousEventForceSetpointWarning.magnitudeWarning ||
            _eventForceSetpointWarning.activeOpticNetForceWarning !=
                    _previousEventForceSetpointWarning.activeOpticNetForceWarning;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventForceSetpointWarning.safetyLimitWarning[i] !=
                                 _previousEventForceSetpointWarning.safetyLimitWarning[i] ||
                         _eventForceSetpointWarning.nearNeighborWarning[i] !=
                                 _previousEventForceSetpointWarning.nearNeighborWarning[i] ||
                         _eventForceSetpointWarning.farNeighborWarning[i] !=
                                 _previousEventForceSetpointWarning.farNeighborWarning[i] ||
                         _eventForceSetpointWarning.elevationForceWarning[i] !=
                                 _previousEventForceSetpointWarning.elevationForceWarning[i] ||
                         _eventForceSetpointWarning.azimuthForceWarning[i] !=
                                 _previousEventForceSetpointWarning.azimuthForceWarning[i] ||
                         _eventForceSetpointWarning.thermalForceWarning[i] !=
                                 _previousEventForceSetpointWarning.thermalForceWarning[i] ||
                         _eventForceSetpointWarning.balanceForceWarning[i] !=
                                 _previousEventForceSetpointWarning.balanceForceWarning[i] ||
                         _eventForceSetpointWarning.accelerationForceWarning[i] !=
                                 _previousEventForceSetpointWarning.accelerationForceWarning[i] ||
                         _eventForceSetpointWarning.velocityForceWarning[i] !=
                                 _previousEventForceSetpointWarning.velocityForceWarning[i] ||
                         _eventForceSetpointWarning.activeOpticForceWarning[i] !=
                                 _previousEventForceSetpointWarning.activeOpticForceWarning[i] ||
                         _eventForceSetpointWarning.staticForceWarning[i] !=
                                 _previousEventForceSetpointWarning.staticForceWarning[i] ||
                         _eventForceSetpointWarning.offsetForceWarning[i] !=
                                 _previousEventForceSetpointWarning.offsetForceWarning[i] ||
                         _eventForceSetpointWarning.forceWarning[i] !=
                                 _previousEventForceSetpointWarning.forceWarning[i];
    }
    if (changeDetected) {
        logForceSetpointWarning();
    }
}

void M1M3SSPublisher::logGyroWarning() {
    _eventGyroWarning.anyWarning =
            _eventGyroWarning.gyroXStatusWarning || _eventGyroWarning.gyroYStatusWarning ||
            _eventGyroWarning.gyroZStatusWarning || _eventGyroWarning.sequenceNumberWarning ||
            _eventGyroWarning.crcMismatchWarning || _eventGyroWarning.invalidLengthWarning ||
            _eventGyroWarning.invalidHeaderWarning || _eventGyroWarning.incompleteFrameWarning ||
            _eventGyroWarning.gyroXSLDWarning || _eventGyroWarning.gyroXMODDACWarning ||
            _eventGyroWarning.gyroXPhaseWarning || _eventGyroWarning.gyroXFlashWarning ||
            _eventGyroWarning.gyroYSLDWarning || _eventGyroWarning.gyroYMODDACWarning ||
            _eventGyroWarning.gyroYPhaseWarning || _eventGyroWarning.gyroYFlashWarning ||
            _eventGyroWarning.gyroZSLDWarning || _eventGyroWarning.gyroZMODDACWarning ||
            _eventGyroWarning.gyroZPhaseWarning || _eventGyroWarning.gyroZFlashWarning ||
            _eventGyroWarning.gyroXSLDTemperatureStatusWarning ||
            _eventGyroWarning.gyroYSLDTemperatureStatusWarning ||
            _eventGyroWarning.gyroZSLDTemperatureStatusWarning ||
            _eventGyroWarning.gcbTemperatureStatusWarning || _eventGyroWarning.temperatureStatusWarning ||
            _eventGyroWarning.gcbDSPSPIFlashStatusWarning || _eventGyroWarning.gcbFPGASPIFlashStatusWarning ||
            _eventGyroWarning.dspSPIFlashStatusWarning || _eventGyroWarning.fpgaSPIFlashStatusWarning ||
            _eventGyroWarning.gcb1_2VStatusWarning || _eventGyroWarning.gcb3_3VStatusWarning ||
            _eventGyroWarning.gcb5VStatusWarning || _eventGyroWarning.v1_2StatusWarning ||
            _eventGyroWarning.v3_3StatusWarning || _eventGyroWarning.v5StatusWarning ||
            _eventGyroWarning.gcbFPGAStatusWarning || _eventGyroWarning.fpgaStatusWarning ||
            _eventGyroWarning.hiSpeedSPORTStatusWarning || _eventGyroWarning.auxSPORTStatusWarning ||
            _eventGyroWarning.sufficientSoftwareResourcesWarning ||
            _eventGyroWarning.gyroEOVoltsPositiveWarning || _eventGyroWarning.gyroEOVoltsNegativeWarning ||
            _eventGyroWarning.gyroXVoltsWarning || _eventGyroWarning.gyroYVoltsWarning ||
            _eventGyroWarning.gyroZVoltsWarning || _eventGyroWarning.gcbADCCommsWarning ||
            _eventGyroWarning.mSYNCExternalTimingWarning;
    _m1m3SAL->logEvent_gyroWarning(&_eventGyroWarning, 0);
    _previousEventGyroWarning = _eventGyroWarning;
}

void M1M3SSPublisher::tryLogGyroWarning() {
    if (_eventGyroWarning.gyroXStatusWarning != _previousEventGyroWarning.gyroXStatusWarning ||
        _eventGyroWarning.gyroYStatusWarning != _previousEventGyroWarning.gyroYStatusWarning ||
        _eventGyroWarning.gyroZStatusWarning != _previousEventGyroWarning.gyroZStatusWarning ||
        _eventGyroWarning.sequenceNumberWarning != _previousEventGyroWarning.sequenceNumberWarning ||
        _eventGyroWarning.crcMismatchWarning != _previousEventGyroWarning.crcMismatchWarning ||
        _eventGyroWarning.invalidLengthWarning != _previousEventGyroWarning.invalidLengthWarning ||
        _eventGyroWarning.invalidHeaderWarning != _previousEventGyroWarning.invalidHeaderWarning ||
        _eventGyroWarning.incompleteFrameWarning != _previousEventGyroWarning.incompleteFrameWarning ||
        _eventGyroWarning.gyroXSLDWarning != _previousEventGyroWarning.gyroXSLDWarning ||
        _eventGyroWarning.gyroXMODDACWarning != _previousEventGyroWarning.gyroXMODDACWarning ||
        _eventGyroWarning.gyroXPhaseWarning != _previousEventGyroWarning.gyroXPhaseWarning ||
        _eventGyroWarning.gyroXFlashWarning != _previousEventGyroWarning.gyroXFlashWarning ||
        _eventGyroWarning.gyroYSLDWarning != _previousEventGyroWarning.gyroYSLDWarning ||
        _eventGyroWarning.gyroYMODDACWarning != _previousEventGyroWarning.gyroYMODDACWarning ||
        _eventGyroWarning.gyroYPhaseWarning != _previousEventGyroWarning.gyroYPhaseWarning ||
        _eventGyroWarning.gyroYFlashWarning != _previousEventGyroWarning.gyroYFlashWarning ||
        _eventGyroWarning.gyroZSLDWarning != _previousEventGyroWarning.gyroZSLDWarning ||
        _eventGyroWarning.gyroZMODDACWarning != _previousEventGyroWarning.gyroZMODDACWarning ||
        _eventGyroWarning.gyroZPhaseWarning != _previousEventGyroWarning.gyroZPhaseWarning ||
        _eventGyroWarning.gyroZFlashWarning != _previousEventGyroWarning.gyroZFlashWarning ||
        _eventGyroWarning.gyroXSLDTemperatureStatusWarning !=
                _previousEventGyroWarning.gyroXSLDTemperatureStatusWarning ||
        _eventGyroWarning.gyroYSLDTemperatureStatusWarning !=
                _previousEventGyroWarning.gyroYSLDTemperatureStatusWarning ||
        _eventGyroWarning.gyroZSLDTemperatureStatusWarning !=
                _previousEventGyroWarning.gyroZSLDTemperatureStatusWarning ||
        _eventGyroWarning.gcbTemperatureStatusWarning !=
                _previousEventGyroWarning.gcbTemperatureStatusWarning ||
        _eventGyroWarning.temperatureStatusWarning != _previousEventGyroWarning.temperatureStatusWarning ||
        _eventGyroWarning.gcbDSPSPIFlashStatusWarning !=
                _previousEventGyroWarning.gcbDSPSPIFlashStatusWarning ||
        _eventGyroWarning.gcbFPGASPIFlashStatusWarning !=
                _previousEventGyroWarning.gcbFPGASPIFlashStatusWarning ||
        _eventGyroWarning.dspSPIFlashStatusWarning != _previousEventGyroWarning.dspSPIFlashStatusWarning ||
        _eventGyroWarning.fpgaSPIFlashStatusWarning != _previousEventGyroWarning.fpgaSPIFlashStatusWarning ||
        _eventGyroWarning.gcb1_2VStatusWarning != _previousEventGyroWarning.gcb1_2VStatusWarning ||
        _eventGyroWarning.gcb3_3VStatusWarning != _previousEventGyroWarning.gcb3_3VStatusWarning ||
        _eventGyroWarning.gcb5VStatusWarning != _previousEventGyroWarning.gcb5VStatusWarning ||
        _eventGyroWarning.v1_2StatusWarning != _previousEventGyroWarning.v1_2StatusWarning ||
        _eventGyroWarning.v3_3StatusWarning != _previousEventGyroWarning.v3_3StatusWarning ||
        _eventGyroWarning.v5StatusWarning != _previousEventGyroWarning.v5StatusWarning ||
        _eventGyroWarning.gcbFPGAStatusWarning != _previousEventGyroWarning.gcbFPGAStatusWarning ||
        _eventGyroWarning.fpgaStatusWarning != _previousEventGyroWarning.fpgaStatusWarning ||
        _eventGyroWarning.hiSpeedSPORTStatusWarning != _previousEventGyroWarning.hiSpeedSPORTStatusWarning ||
        _eventGyroWarning.auxSPORTStatusWarning != _previousEventGyroWarning.auxSPORTStatusWarning ||
        _eventGyroWarning.sufficientSoftwareResourcesWarning !=
                _previousEventGyroWarning.sufficientSoftwareResourcesWarning ||
        _eventGyroWarning.gyroEOVoltsPositiveWarning !=
                _previousEventGyroWarning.gyroEOVoltsPositiveWarning ||
        _eventGyroWarning.gyroEOVoltsNegativeWarning !=
                _previousEventGyroWarning.gyroEOVoltsNegativeWarning ||
        _eventGyroWarning.gyroXVoltsWarning != _previousEventGyroWarning.gyroXVoltsWarning ||
        _eventGyroWarning.gyroYVoltsWarning != _previousEventGyroWarning.gyroYVoltsWarning ||
        _eventGyroWarning.gyroZVoltsWarning != _previousEventGyroWarning.gyroZVoltsWarning ||
        _eventGyroWarning.gcbADCCommsWarning != _previousEventGyroWarning.gcbADCCommsWarning ||
        _eventGyroWarning.mSYNCExternalTimingWarning !=
                _previousEventGyroWarning.mSYNCExternalTimingWarning) {
        logGyroWarning();
    }
}

void M1M3SSPublisher::logHardpointActuatorInfo() {
    _m1m3SAL->logEvent_hardpointActuatorInfo(&_eventHardpointActuatorInfo, 0);
    _previousEventHardpointActuatorInfo = _eventHardpointActuatorInfo;
}

void M1M3SSPublisher::tryLogHardpointActuatorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorInfo.referenceId[i] !=
                                 _previousEventHardpointActuatorInfo.referenceId[i] ||
                         _eventHardpointActuatorInfo.referencePosition[i] !=
                                 _previousEventHardpointActuatorInfo.referencePosition[i] ||
                         _eventHardpointActuatorInfo.modbusSubnet[i] !=
                                 _previousEventHardpointActuatorInfo.modbusSubnet[i] ||
                         _eventHardpointActuatorInfo.modbusAddress[i] !=
                                 _previousEventHardpointActuatorInfo.modbusAddress[i] ||
                         _eventHardpointActuatorInfo.xPosition[i] !=
                                 _previousEventHardpointActuatorInfo.xPosition[i] ||
                         _eventHardpointActuatorInfo.yPosition[i] !=
                                 _previousEventHardpointActuatorInfo.yPosition[i] ||
                         _eventHardpointActuatorInfo.zPosition[i] !=
                                 _previousEventHardpointActuatorInfo.zPosition[i] ||
                         _eventHardpointActuatorInfo.ilcUniqueId[i] !=
                                 _previousEventHardpointActuatorInfo.ilcUniqueId[i] ||
                         _eventHardpointActuatorInfo.ilcApplicationType[i] !=
                                 _previousEventHardpointActuatorInfo.ilcApplicationType[i] ||
                         _eventHardpointActuatorInfo.networkNodeType[i] !=
                                 _previousEventHardpointActuatorInfo.networkNodeType[i] ||
                         _eventHardpointActuatorInfo.ilcSelectedOptions[i] !=
                                 _previousEventHardpointActuatorInfo.ilcSelectedOptions[i] ||
                         _eventHardpointActuatorInfo.networkNodeOptions[i] !=
                                 _previousEventHardpointActuatorInfo.networkNodeOptions[i] ||
                         _eventHardpointActuatorInfo.majorRevision[i] !=
                                 _previousEventHardpointActuatorInfo.majorRevision[i] ||
                         _eventHardpointActuatorInfo.minorRevision[i] !=
                                 _previousEventHardpointActuatorInfo.minorRevision[i] ||
                         _eventHardpointActuatorInfo.adcScanRate[i] !=
                                 _previousEventHardpointActuatorInfo.adcScanRate[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellCoefficient[i] !=
                                 _previousEventHardpointActuatorInfo.mainLoadCellCoefficient[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellOffset[i] !=
                                 _previousEventHardpointActuatorInfo.mainLoadCellOffset[i] ||
                         _eventHardpointActuatorInfo.mainLoadCellSensitivity[i] !=
                                 _previousEventHardpointActuatorInfo.mainLoadCellSensitivity[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellCoefficient[i] !=
                                 _previousEventHardpointActuatorInfo.backupLoadCellCoefficient[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellOffset[i] !=
                                 _previousEventHardpointActuatorInfo.backupLoadCellOffset[i] ||
                         _eventHardpointActuatorInfo.backupLoadCellSensitivity[i] !=
                                 _previousEventHardpointActuatorInfo.backupLoadCellSensitivity[i];
    }
    if (changeDetected) {
        logHardpointActuatorInfo();
    }
}

void M1M3SSPublisher::logHardpointActuatorState() {
    _m1m3SAL->logEvent_hardpointActuatorState(&_eventHardpointActuatorState, 0);
    _previousEventHardpointActuatorState = _eventHardpointActuatorState;
}

void M1M3SSPublisher::tryLogHardpointActuatorState() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointActuatorState.ilcState[i] !=
                                 _previousEventHardpointActuatorState.ilcState[i] ||
                         _eventHardpointActuatorState.motionState[i] !=
                                 _previousEventHardpointActuatorState.motionState[i];
    }
    if (changeDetected) {
        logHardpointActuatorState();
    }
}

void M1M3SSPublisher::logHardpointMonitorInfo() {
    _m1m3SAL->logEvent_hardpointMonitorInfo(&_eventHardpointMonitorInfo, 0);
    _previousEventHardpointMonitorInfo = _eventHardpointMonitorInfo;
}

void M1M3SSPublisher::tryLogHardpointMonitorInfo() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointMonitorInfo.referenceId[i] !=
                                 _previousEventHardpointMonitorInfo.referenceId[i] ||
                         _eventHardpointMonitorInfo.modbusSubnet[i] !=
                                 _previousEventHardpointMonitorInfo.modbusSubnet[i] ||
                         _eventHardpointMonitorInfo.modbusAddress[i] !=
                                 _previousEventHardpointMonitorInfo.modbusAddress[i] ||
                         _eventHardpointMonitorInfo.ilcUniqueId[i] !=
                                 _previousEventHardpointMonitorInfo.ilcUniqueId[i] ||
                         _eventHardpointMonitorInfo.ilcApplicationType[i] !=
                                 _previousEventHardpointMonitorInfo.ilcApplicationType[i] ||
                         _eventHardpointMonitorInfo.networkNodeType[i] !=
                                 _previousEventHardpointMonitorInfo.networkNodeType[i] ||
                         _eventHardpointMonitorInfo.majorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.majorRevision[i] ||
                         _eventHardpointMonitorInfo.minorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.minorRevision[i] ||
                         _eventHardpointMonitorInfo.mezzanineUniqueId[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineUniqueId[i] ||
                         _eventHardpointMonitorInfo.mezzanineFirmwareType[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineFirmwareType[i] ||
                         _eventHardpointMonitorInfo.mezzanineMajorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineMajorRevision[i] ||
                         _eventHardpointMonitorInfo.mezzanineMinorRevision[i] !=
                                 _previousEventHardpointMonitorInfo.mezzanineMinorRevision[i];
    }
    if (changeDetected) {
        logHardpointMonitorInfo();
    }
}

void M1M3SSPublisher::logHardpointMonitorState() {
    _m1m3SAL->logEvent_hardpointMonitorState(&_eventHardpointMonitorState, 0);
    _previousEventHardpointMonitorState = _eventHardpointMonitorState;
}

void M1M3SSPublisher::tryLogHardpointMonitorState() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected || _eventHardpointMonitorState.ilcState[i] !=
                                                   _previousEventHardpointMonitorState.ilcState[i];
    }
    if (changeDetected) {
        logHardpointMonitorState();
    }
}

void M1M3SSPublisher::logHardpointMonitorWarning() {
    _eventHardpointMonitorWarning.anyMajorFault = false;
    _eventHardpointMonitorWarning.anyMinorFault = false;
    _eventHardpointMonitorWarning.anyFaultOverride = false;
    _eventHardpointMonitorWarning.anyInstrumentError = false;
    _eventHardpointMonitorWarning.anyMezzanineError = false;
    _eventHardpointMonitorWarning.anyMezzanineBootloaderActive = false;
    _eventHardpointMonitorWarning.anyUniqueIdCRCError = false;
    _eventHardpointMonitorWarning.anyApplicationTypeMismatch = false;
    _eventHardpointMonitorWarning.anyApplicationMissing = false;
    _eventHardpointMonitorWarning.anyApplicationCRCMismatch = false;
    _eventHardpointMonitorWarning.anyOneWireMissing = false;
    _eventHardpointMonitorWarning.anyOneWire1Mismatch = false;
    _eventHardpointMonitorWarning.anyOneWire2Mismatch = false;
    _eventHardpointMonitorWarning.anyWatchdogReset = false;
    _eventHardpointMonitorWarning.anyBrownOut = false;
    _eventHardpointMonitorWarning.anyEventTrapReset = false;
    _eventHardpointMonitorWarning.anySSRPowerFault = false;
    _eventHardpointMonitorWarning.anyAuxPowerFault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault = false;
    _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError = false;
    _eventHardpointMonitorWarning.anyMezzanineEventTrapReset = false;
    _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault = false;
    _eventHardpointMonitorWarning.anyMezzanineApplicationMissing = false;
    _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch = false;
    for (int i = 0; i < HP_COUNT; ++i) {
        _eventHardpointMonitorWarning.anyMajorFault =
                _eventHardpointMonitorWarning.anyMajorFault || _eventHardpointMonitorWarning.majorFault[i];
        _eventHardpointMonitorWarning.anyMinorFault =
                _eventHardpointMonitorWarning.anyMinorFault || _eventHardpointMonitorWarning.minorFault[i];
        _eventHardpointMonitorWarning.anyFaultOverride = _eventHardpointMonitorWarning.anyFaultOverride ||
                                                         _eventHardpointMonitorWarning.faultOverride[i];
        _eventHardpointMonitorWarning.anyInstrumentError = _eventHardpointMonitorWarning.anyInstrumentError ||
                                                           _eventHardpointMonitorWarning.instrumentError[i];
        _eventHardpointMonitorWarning.anyMezzanineError = _eventHardpointMonitorWarning.anyMezzanineError ||
                                                          _eventHardpointMonitorWarning.mezzanineError[i];
        _eventHardpointMonitorWarning.anyMezzanineBootloaderActive =
                _eventHardpointMonitorWarning.anyMezzanineBootloaderActive ||
                _eventHardpointMonitorWarning.mezzanineBootloaderActive[i];
        _eventHardpointMonitorWarning.anyUniqueIdCRCError =
                _eventHardpointMonitorWarning.anyUniqueIdCRCError ||
                _eventHardpointMonitorWarning.uniqueIdCRCError[i];
        _eventHardpointMonitorWarning.anyApplicationTypeMismatch =
                _eventHardpointMonitorWarning.anyApplicationTypeMismatch ||
                _eventHardpointMonitorWarning.applicationTypeMismatch[i];
        _eventHardpointMonitorWarning.anyApplicationMissing =
                _eventHardpointMonitorWarning.anyApplicationMissing ||
                _eventHardpointMonitorWarning.applicationMissing[i];
        _eventHardpointMonitorWarning.anyApplicationCRCMismatch =
                _eventHardpointMonitorWarning.anyApplicationCRCMismatch ||
                _eventHardpointMonitorWarning.applicationCRCMismatch[i];
        _eventHardpointMonitorWarning.anyOneWireMissing = _eventHardpointMonitorWarning.anyOneWireMissing ||
                                                          _eventHardpointMonitorWarning.oneWireMissing[i];
        _eventHardpointMonitorWarning.anyOneWire1Mismatch =
                _eventHardpointMonitorWarning.anyOneWire1Mismatch ||
                _eventHardpointMonitorWarning.oneWire1Mismatch[i];
        _eventHardpointMonitorWarning.anyOneWire2Mismatch =
                _eventHardpointMonitorWarning.anyOneWire2Mismatch ||
                _eventHardpointMonitorWarning.oneWire2Mismatch[i];
        _eventHardpointMonitorWarning.anyWatchdogReset = _eventHardpointMonitorWarning.anyWatchdogReset ||
                                                         _eventHardpointMonitorWarning.watchdogReset[i];
        _eventHardpointMonitorWarning.anyBrownOut =
                _eventHardpointMonitorWarning.anyBrownOut || _eventHardpointMonitorWarning.brownOut[i];
        _eventHardpointMonitorWarning.anyEventTrapReset = _eventHardpointMonitorWarning.anyEventTrapReset ||
                                                          _eventHardpointMonitorWarning.eventTrapReset[i];
        _eventHardpointMonitorWarning.anySSRPowerFault = _eventHardpointMonitorWarning.anySSRPowerFault ||
                                                         _eventHardpointMonitorWarning.ssrPowerFault[i];
        _eventHardpointMonitorWarning.anyAuxPowerFault = _eventHardpointMonitorWarning.anyAuxPowerFault ||
                                                         _eventHardpointMonitorWarning.auxPowerFault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault ||
                _eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault ||
                _eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault ||
                _eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault =
                _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault ||
                _eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i];
        _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError =
                _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError ||
                _eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i];
        _eventHardpointMonitorWarning.anyMezzanineEventTrapReset =
                _eventHardpointMonitorWarning.anyMezzanineEventTrapReset ||
                _eventHardpointMonitorWarning.mezzanineEventTrapReset[i];
        _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault =
                _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault ||
                _eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i];
        _eventHardpointMonitorWarning.anyMezzanineApplicationMissing =
                _eventHardpointMonitorWarning.anyMezzanineApplicationMissing ||
                _eventHardpointMonitorWarning.mezzanineApplicationMissing[i];
        _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch =
                _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch ||
                _eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    _eventHardpointMonitorWarning.anyWarning =
            _eventHardpointMonitorWarning.anyMajorFault || _eventHardpointMonitorWarning.anyMinorFault ||
            _eventHardpointMonitorWarning.anyFaultOverride ||
            _eventHardpointMonitorWarning.anyInstrumentError ||
            _eventHardpointMonitorWarning.anyMezzanineError ||
            _eventHardpointMonitorWarning.anyMezzanineBootloaderActive ||
            _eventHardpointMonitorWarning.anyUniqueIdCRCError ||
            _eventHardpointMonitorWarning.anyApplicationTypeMismatch ||
            _eventHardpointMonitorWarning.anyApplicationMissing ||
            _eventHardpointMonitorWarning.anyApplicationCRCMismatch ||
            _eventHardpointMonitorWarning.anyOneWireMissing ||
            _eventHardpointMonitorWarning.anyOneWire1Mismatch ||
            _eventHardpointMonitorWarning.anyOneWire2Mismatch ||
            _eventHardpointMonitorWarning.anyWatchdogReset || _eventHardpointMonitorWarning.anyBrownOut ||
            _eventHardpointMonitorWarning.anyEventTrapReset ||
            _eventHardpointMonitorWarning.anySSRPowerFault ||
            _eventHardpointMonitorWarning.anyAuxPowerFault ||
            _eventHardpointMonitorWarning.anyMezzanineS1AInterface1Fault ||
            _eventHardpointMonitorWarning.anyMezzanineS1ALVDT1Fault ||
            _eventHardpointMonitorWarning.anyMezzanineS1AInterface2Fault ||
            _eventHardpointMonitorWarning.anyMezzanineS1ALVDT2Fault ||
            _eventHardpointMonitorWarning.anyMezzanineUniqueIdCRCError ||
            _eventHardpointMonitorWarning.anyMezzanineEventTrapReset ||
            _eventHardpointMonitorWarning.anyMezzanineDCPRS422ChipFault ||
            _eventHardpointMonitorWarning.anyMezzanineApplicationMissing ||
            _eventHardpointMonitorWarning.anyMezzanineApplicationCRCMismatch;
    _m1m3SAL->logEvent_hardpointMonitorWarning(&_eventHardpointMonitorWarning, 0);
    _previousEventHardpointMonitorWarning = _eventHardpointMonitorWarning;
}

void M1M3SSPublisher::tryLogHardpointMonitorWarning() {
    bool changeDetected = false;
    for (int i = 0; i < HP_COUNT && !changeDetected; ++i) {
        changeDetected = changeDetected ||
                         _eventHardpointMonitorWarning.majorFault[i] !=
                                 _previousEventHardpointMonitorWarning.majorFault[i] ||
                         _eventHardpointMonitorWarning.minorFault[i] !=
                                 _previousEventHardpointMonitorWarning.minorFault[i] ||
                         _eventHardpointMonitorWarning.faultOverride[i] !=
                                 _previousEventHardpointMonitorWarning.faultOverride[i] ||
                         _eventHardpointMonitorWarning.instrumentError[i] !=
                                 _previousEventHardpointMonitorWarning.instrumentError[i] ||
                         _eventHardpointMonitorWarning.mezzanineError[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineError[i] ||
                         _eventHardpointMonitorWarning.mezzanineBootloaderActive[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineBootloaderActive[i] ||
                         _eventHardpointMonitorWarning.uniqueIdCRCError[i] !=
                                 _previousEventHardpointMonitorWarning.uniqueIdCRCError[i] ||
                         _eventHardpointMonitorWarning.applicationTypeMismatch[i] !=
                                 _previousEventHardpointMonitorWarning.applicationTypeMismatch[i] ||
                         _eventHardpointMonitorWarning.applicationMissing[i] !=
                                 _previousEventHardpointMonitorWarning.applicationMissing[i] ||
                         _eventHardpointMonitorWarning.applicationCRCMismatch[i] !=
                                 _previousEventHardpointMonitorWarning.applicationCRCMismatch[i] ||
                         _eventHardpointMonitorWarning.oneWireMissing[i] !=
                                 _previousEventHardpointMonitorWarning.oneWireMissing[i] ||
                         _eventHardpointMonitorWarning.oneWire1Mismatch[i] !=
                                 _previousEventHardpointMonitorWarning.oneWire1Mismatch[i] ||
                         _eventHardpointMonitorWarning.oneWire2Mismatch[i] !=
                                 _previousEventHardpointMonitorWarning.oneWire2Mismatch[i] ||
                         _eventHardpointMonitorWarning.watchdogReset[i] !=
                                 _previousEventHardpointMonitorWarning.watchdogReset[i] ||
                         _eventHardpointMonitorWarning.brownOut[i] !=
                                 _previousEventHardpointMonitorWarning.brownOut[i] ||
                         _eventHardpointMonitorWarning.eventTrapReset[i] !=
                                 _previousEventHardpointMonitorWarning.eventTrapReset[i] ||
                         _eventHardpointMonitorWarning.ssrPowerFault[i] !=
                                 _previousEventHardpointMonitorWarning.ssrPowerFault[i] ||
                         _eventHardpointMonitorWarning.auxPowerFault[i] !=
                                 _previousEventHardpointMonitorWarning.auxPowerFault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1AInterface1Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1ALVDT1Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1AInterface2Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineS1ALVDT2Fault[i] ||
                         _eventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineUniqueIdCRCError[i] ||
                         _eventHardpointMonitorWarning.mezzanineEventTrapReset[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineEventTrapReset[i] ||
                         _eventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineDCPRS422ChipFault[i] ||
                         _eventHardpointMonitorWarning.mezzanineApplicationMissing[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineApplicationMissing[i] ||
                         _eventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i] !=
                                 _previousEventHardpointMonitorWarning.mezzanineApplicationCRCMismatch[i];
    }
    if (changeDetected) {
        logHardpointMonitorWarning();
    }
}

void M1M3SSPublisher::logInclinometerSensorWarning() {
    _eventInclinometerSensorWarning.anyWarning =
            _eventInclinometerSensorWarning.sensorReportsIllegalFunction ||
            _eventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
            _eventInclinometerSensorWarning.responseTimeout || _eventInclinometerSensorWarning.invalidCRC ||
            _eventInclinometerSensorWarning.invalidLength || _eventInclinometerSensorWarning.unknownAddress ||
            _eventInclinometerSensorWarning.unknownFunction || _eventInclinometerSensorWarning.unknownProblem;
    _m1m3SAL->logEvent_inclinometerSensorWarning(&_eventInclinometerSensorWarning, 0);
    _previousEventInclinometerSensorWarning = _eventInclinometerSensorWarning;
}

void M1M3SSPublisher::tryLogInclinometerSensorWarning() {
    if (_eventInclinometerSensorWarning.sensorReportsIllegalFunction !=
                _previousEventInclinometerSensorWarning.sensorReportsIllegalFunction ||
        _eventInclinometerSensorWarning.sensorReportsIllegalDataAddress !=
                _previousEventInclinometerSensorWarning.sensorReportsIllegalDataAddress ||
        _eventInclinometerSensorWarning.responseTimeout !=
                _previousEventInclinometerSensorWarning.responseTimeout ||
        _eventInclinometerSensorWarning.invalidCRC != _previousEventInclinometerSensorWarning.invalidCRC ||
        _eventInclinometerSensorWarning.invalidLength !=
                _previousEventInclinometerSensorWarning.invalidLength ||
        _eventInclinometerSensorWarning.unknownAddress !=
                _previousEventInclinometerSensorWarning.unknownAddress ||
        _eventInclinometerSensorWarning.unknownFunction !=
                _previousEventInclinometerSensorWarning.unknownFunction ||
        _eventInclinometerSensorWarning.unknownProblem !=
                _previousEventInclinometerSensorWarning.unknownProblem) {
        logInclinometerSensorWarning();
    }
}

void M1M3SSPublisher::newLogLevel(int newLevel) {
    MTM1M3_logevent_logLevelC logLevel;
    logLevel.level = newLevel;
    _m1m3SAL->logEvent_logLevel(&logLevel, 0);
}

void M1M3SSPublisher::logPIDInfo() { _m1m3SAL->logEvent_pidInfo(&_eventPIDInfo, 0); }

void M1M3SSPublisher::logPowerStatus() {
    _m1m3SAL->logEvent_powerStatus(&_eventPowerStatus, 0);
    _previousEventPowerStatus = _eventPowerStatus;
}

void M1M3SSPublisher::tryLogPowerStatus() {
    if (_eventPowerStatus.powerNetworkACommandedOn != _previousEventPowerStatus.powerNetworkACommandedOn ||
        _eventPowerStatus.powerNetworkAOutputOn != _previousEventPowerStatus.powerNetworkAOutputOn ||
        _eventPowerStatus.powerNetworkBCommandedOn != _previousEventPowerStatus.powerNetworkBCommandedOn ||
        _eventPowerStatus.powerNetworkBOutputOn != _previousEventPowerStatus.powerNetworkBOutputOn ||
        _eventPowerStatus.powerNetworkCCommandedOn != _previousEventPowerStatus.powerNetworkCCommandedOn ||
        _eventPowerStatus.powerNetworkCOutputOn != _previousEventPowerStatus.powerNetworkCOutputOn ||
        _eventPowerStatus.powerNetworkDCommandedOn != _previousEventPowerStatus.powerNetworkDCommandedOn ||
        _eventPowerStatus.powerNetworkDOutputOn != _previousEventPowerStatus.powerNetworkDOutputOn ||
        _eventPowerStatus.auxPowerNetworkACommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkACommandedOn ||
        _eventPowerStatus.auxPowerNetworkAOutputOn != _previousEventPowerStatus.auxPowerNetworkAOutputOn ||
        _eventPowerStatus.auxPowerNetworkBCommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkBCommandedOn ||
        _eventPowerStatus.auxPowerNetworkBOutputOn != _previousEventPowerStatus.auxPowerNetworkBOutputOn ||
        _eventPowerStatus.auxPowerNetworkCCommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkCCommandedOn ||
        _eventPowerStatus.auxPowerNetworkCOutputOn != _previousEventPowerStatus.auxPowerNetworkCOutputOn ||
        _eventPowerStatus.auxPowerNetworkDCommandedOn !=
                _previousEventPowerStatus.auxPowerNetworkDCommandedOn ||
        _eventPowerStatus.auxPowerNetworkDOutputOn != _previousEventPowerStatus.auxPowerNetworkDOutputOn) {
        logPowerStatus();
    }
}

void M1M3SSPublisher::logPowerWarning() {
    _eventPowerWarning.anyWarning = _eventPowerWarning.powerNetworkAOutputMismatch ||
                                    _eventPowerWarning.powerNetworkBOutputMismatch ||
                                    _eventPowerWarning.powerNetworkCOutputMismatch ||
                                    _eventPowerWarning.powerNetworkDOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkAOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkBOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkCOutputMismatch ||
                                    _eventPowerWarning.auxPowerNetworkDOutputMismatch;
    _m1m3SAL->logEvent_powerWarning(&_eventPowerWarning, 0);
    _previousEventPowerWarning = _eventPowerWarning;
}

void M1M3SSPublisher::tryLogPowerWarning() {
    if (_eventPowerWarning.powerNetworkAOutputMismatch !=
                _previousEventPowerWarning.powerNetworkAOutputMismatch ||
        _eventPowerWarning.powerNetworkBOutputMismatch !=
                _previousEventPowerWarning.powerNetworkBOutputMismatch ||
        _eventPowerWarning.powerNetworkCOutputMismatch !=
                _previousEventPowerWarning.powerNetworkCOutputMismatch ||
        _eventPowerWarning.powerNetworkDOutputMismatch !=
                _previousEventPowerWarning.powerNetworkDOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkAOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkAOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkBOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkBOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkCOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkCOutputMismatch ||
        _eventPowerWarning.auxPowerNetworkDOutputMismatch !=
                _previousEventPowerWarning.auxPowerNetworkDOutputMismatch) {
        logPowerWarning();
    }
}

void M1M3SSPublisher::logPreclippedCylinderForces() {
    bool changeDetected = false;
    for (int i = 0; i < FA_COUNT && !changeDetected; ++i) {
        changeDetected =
                changeDetected ||
                (i < 112 && _eventPreclippedCylinderForces.secondaryCylinderForces[i] !=
                                    _previousEventPreclippedCylinderForces.secondaryCylinderForces[i]) ||
                (_eventPreclippedCylinderForces.primaryCylinderForces[i] !=
                 _previousEventPreclippedCylinderForces.primaryCylinderForces[i]);
    }
    if (changeDetected) {
        _m1m3SAL->logEvent_preclippedCylinderForces(&_eventPreclippedCylinderForces, 0);
        _previousEventPreclippedCylinderForces = _eventPreclippedCylinderForces;
    }
}

void M1M3SSPublisher::logSoftwareVersions() {
    MTM1M3_logevent_softwareVersionsC versions;
    versions.salVersion = SAL_MTM1M3::getSALVersion();
    versions.xmlVersion = SAL_MTM1M3::getXMLVersion();
    versions.openSpliceVersion = SAL_MTM1M3::getOSPLVersion();
    versions.cscVersion = VERSION;
    versions.subsystemVersions = "";
    _m1m3SAL->logEvent_softwareVersions(&versions, 0);
}

void M1M3SSPublisher::logSummaryState() {
    _m1m3SAL->logEvent_summaryState(&_eventSummaryState, 0);
    _previousEventSummaryState = _eventSummaryState;
}

void M1M3SSPublisher::tryLogSummaryState() {
    if (_eventSummaryState.summaryState != _previousEventSummaryState.summaryState) {
        logSummaryState();
    }
}

// macro generating ackCommand method
#define ACK_COMMAND(command)                                                                               \
    void M1M3SSPublisher::ackCommand##command(int32_t commandID, int32_t ackCode, std::string description, \
                                              double timeout) {                                            \
        _m1m3SAL->ackCommand_##command(commandID, ackCode, 0, (char *)description.c_str(), timeout);       \
    }

ACK_COMMAND(setLogLevel)
ACK_COMMAND(start)
ACK_COMMAND(enable)
ACK_COMMAND(disable)
ACK_COMMAND(standby)
ACK_COMMAND(exitControl)
ACK_COMMAND(panic)
ACK_COMMAND(setSlewFlag)
ACK_COMMAND(clearSlewFlag)
ACK_COMMAND(turnAirOn)
ACK_COMMAND(turnAirOff)
ACK_COMMAND(applyOffsetForces)
ACK_COMMAND(clearOffsetForces)
ACK_COMMAND(raiseM1M3)
ACK_COMMAND(lowerM1M3)
ACK_COMMAND(pauseM1M3RaisingLowering)
ACK_COMMAND(resumeM1M3RaisingLowering)
ACK_COMMAND(applyActiveOpticForces)
ACK_COMMAND(clearActiveOpticForces)
ACK_COMMAND(enterEngineering)
ACK_COMMAND(exitEngineering)
ACK_COMMAND(boosterValveOpen)
ACK_COMMAND(boosterValveClose)
ACK_COMMAND(testHardpoint)
ACK_COMMAND(killHardpointTest)
ACK_COMMAND(moveHardpointActuators)
ACK_COMMAND(enableHardpointChase)
ACK_COMMAND(disableHardpointChase)
ACK_COMMAND(abortRaiseM1M3)
ACK_COMMAND(translateM1M3)
ACK_COMMAND(stopHardpointMotion)
ACK_COMMAND(positionM1M3)
ACK_COMMAND(turnLightsOn)
ACK_COMMAND(turnLightsOff)
ACK_COMMAND(turnPowerOn)
ACK_COMMAND(turnPowerOff)
ACK_COMMAND(enableHardpointCorrections)
ACK_COMMAND(disableHardpointCorrections)
ACK_COMMAND(runMirrorForceProfile)
ACK_COMMAND(abortProfile)
ACK_COMMAND(applyOffsetForcesByMirrorForce)
ACK_COMMAND(updatePID)
ACK_COMMAND(resetPID)
ACK_COMMAND(forceActuatorBumpTest)
ACK_COMMAND(killForceActuatorBumpTest)
ACK_COMMAND(disableForceActuator)
ACK_COMMAND(enableForceActuator)
ACK_COMMAND(enableAllForceActuators)
ACK_COMMAND(enableDisableForceComponent)
ACK_COMMAND(setSlewControllerSettings)
