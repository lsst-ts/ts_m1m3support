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

#include <cRIO/DataTypes.h>
#include <HardpointActuatorSettings.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <ILCSubnetData.h>
#include <ILCDataTypes.h>
#include <ModbusBuffer.h>
#include <SafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ILCResponseParser {
public:
    ILCResponseParser();
    ILCResponseParser(ILCSubnetData* subnetData, SafetyController* safetyController);

    void parse(ModbusBuffer* buffer, uint8_t subnet);
    void incExpectedResponses(int32_t* fa, int32_t* hp, int32_t* hm);
    void clearResponses();
    void verifyResponses();

private:
    void _parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId);
    void _parseReportHPServerIDResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportFAServerIDResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportHMServerIDResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportHPServerStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportFAServerStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportHMServerStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseChangeHPILCModeResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseChangeFAILCModeResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseChangeHMILCModeResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, ILCMap map, double timestamp);
    void _parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map);
    void _parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseDualAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map);
    void _parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map);
    void _parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseSetHPADCScanRateResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseSetFAADCScanRateResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseHPResetResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseFAResetResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseHMResetResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReadHPCalibrationResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReadFACalibrationResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReadHMPressureValuesResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportDCAIDResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportHMMezzanineIDResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportDCAStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void _parseReportLVDTResponse(ModbusBuffer* buffer, ILCMap map);

    void _checkForceActuatorForces(ILCMap map);
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

    HardpointActuatorSettings* _hardpointActuatorSettings;
    ILCSubnetData* _subnetData;
    SafetyController* _safetyController;

    int32_t _faExpectedResponses[FA_COUNT];
    int32_t _hpExpectedResponses[HP_COUNT];
    int32_t _hmExpectedResponses[HP_COUNT];

    MTM1M3_logevent_detailedStateC* _detailedState;

    MTM1M3_logevent_hardpointActuatorInfoC* _hardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC* _hardpointActuatorState;
    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;

    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC* _forceActuatorWarning;
    MTM1M3_logevent_forceActuatorForceWarningC* _forceWarning;
    MTM1M3_appliedCylinderForcesC* _appliedCylinderForces;

    MTM1M3_logevent_hardpointMonitorInfoC* _hardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC* _hardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC* _hardpointMonitorWarning;
    MTM1M3_hardpointMonitorDataC* _hardpointMonitorData;

    MTM1M3_logevent_ilcWarningC* _ilcWarning;

    MTM1M3_outerLoopDataC* _outerLoopData;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCRESPONSEPARSER_H_ */
