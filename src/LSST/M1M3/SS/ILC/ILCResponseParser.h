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

#ifndef ILCRESPONSEPARSER_H_
#define ILCRESPONSEPARSER_H_

#include <SAL_MTM1M3C.h>

#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <ILCDataTypes.h>
#include <ILCSubnetData.h>
#include <ModbusBuffer.h>
#include <SafetyController.h>
#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ILCResponseParser {
public:
    ILCResponseParser();
    ILCResponseParser(ILCSubnetData *subnetData, SafetyController *safetyController);

    void parse(ModbusBuffer *buffer, uint8_t subnet);
    void incExpectedResponses(int32_t *fa, int32_t *hp, int32_t *hm);
    void clearResponses();
    void verifyResponses();

private:
    void _parseErrorResponse(ModbusBuffer *buffer, uint8_t called_function, double timestamp,
                             int32_t actuatorId);
    void _parseReportHPServerIDResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportHMServerIDResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportHPServerStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportFAServerStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportHMServerStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseChangeHPILCModeResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseChangeFAILCModeResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseChangeHMILCModeResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseElectromechanicalForceAndStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc,
                                                       double timestamp);
    void _parseSetBoostValveDCAGainsResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseForceDemandResponse(ModbusBuffer *buffer, uint8_t address, const ILCMap &ilc);
    void _parseSingleAxisForceDemandResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseDualAxisForceDemandResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parsePneumaticForceStatusResponse(ModbusBuffer *buffer, uint8_t address, const ILCMap &ilc);
    void _parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseDualAxisPneumaticForceStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseSetHPADCScanRateResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseHPResetResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseFAResetResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseHMResetResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReadHPCalibrationResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReadDCAPressureValuesResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReadHMPressureValuesResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportHMMezzanineIDResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportDCAStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportHMMezzanineStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc);
    void _parseReportLVDTResponse(ModbusBuffer *buffer, const ILCMap &ilc);

    void _checkForceActuatorForces(const ILCMap &ilc);
    void _checkHardpointActuatorMeasuredForce(int32_t actuatorId);
    void _checkHardpointActuatorAirPressure(int32_t actuatorId);

    void _warnResponseTimeout(double timestamp, int32_t actuatorId);
    void _warnInvalidCRC(double timestamp);
    void _warnIllegalFunction(double timestamp, int32_t actuatorId);
    void _warnIllegalDataValue(double timestamp, int32_t actuatorId);
    void _warnInvalidLength(double timestamp, int32_t actuatorId);
    void _warnUnknownSubnet(double timestamp);
    void _warnUnknownAddress(double timestamp, int32_t actuatorId);
    void _warnUnknownFunction(double timestamp, int32_t actuatorId);
    void _warnUnknownProblem(double timestamp, int32_t actuatorId);

    HardpointActuatorSettings *_hardpointActuatorSettings;
    ILCSubnetData *_subnetData;
    SafetyController *_safetyController;

    int32_t _faExpectedResponses[FA_COUNT];
    int32_t _hpExpectedResponses[HP_COUNT];
    int32_t _hmExpectedResponses[HP_COUNT];

    MTM1M3_logevent_hardpointActuatorInfoC *_hardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC *_hardpointActuatorState;
    MTM1M3_hardpointActuatorDataC *_hardpointActuatorData;

    MTM1M3_logevent_forceActuatorStateC *_forceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC *_forceActuatorWarning;
    MTM1M3_logevent_forceActuatorForceWarningC *_forceWarning;
    MTM1M3_appliedCylinderForcesC *_appliedCylinderForces;

    MTM1M3_logevent_hardpointMonitorInfoC *_hardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC *_hardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC *_hardpointMonitorWarning;
    MTM1M3_hardpointMonitorDataC *_hardpointMonitorData;

    MTM1M3_outerLoopDataC *_outerLoopData;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCRESPONSEPARSER_H_ */
