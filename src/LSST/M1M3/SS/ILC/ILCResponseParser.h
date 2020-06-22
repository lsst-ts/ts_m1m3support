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

#ifndef ILCRESPONSEPARSER_H_
#define ILCRESPONSEPARSER_H_

#include <DataTypes.h>
#include <ILCDataTypes.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ModbusBuffer;
class HardpointActuatorSettings;
class ForceActuatorSettings;
class M1M3SSPublisher;
class ILCSubnetData;
class SafetyController;

class ILCResponseParser {
private:
    HardpointActuatorSettings* hardpointActuatorSettings;
    ForceActuatorSettings* forceActuatorSettings;
    M1M3SSPublisher* publisher;
    ILCSubnetData* subnetData;
    SafetyController* safetyController;

    int32_t faExpectedResponses[156];
    int32_t hpExpectedResponses[6];
    int32_t hmExpectedResponses[6];

    MTM1M3_logevent_summaryStateC* summaryState;
    MTM1M3_logevent_detailedStateC* detailedState;

    MTM1M3_logevent_hardpointActuatorInfoC* hardpointActuatorInfo;
    MTM1M3_logevent_hardpointActuatorStateC* hardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorWarningC* hardpointActuatorWarning;
    MTM1M3_hardpointActuatorDataC* hardpointActuatorData;

    MTM1M3_logevent_forceActuatorInfoC* forceActuatorInfo;
    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC* forceActuatorWarning;
    MTM1M3_logevent_forceActuatorForceWarningC* forceWarning;
    MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces;
    MTM1M3_forceActuatorDataC* forceActuatorData;

    MTM1M3_logevent_hardpointMonitorInfoC* hardpointMonitorInfo;
    MTM1M3_logevent_hardpointMonitorStateC* hardpointMonitorState;
    MTM1M3_logevent_hardpointMonitorWarningC* hardpointMonitorWarning;
    MTM1M3_hardpointMonitorDataC* hardpointMonitorData;

    MTM1M3_logevent_ilcWarningC* ilcWarning;

    MTM1M3_outerLoopDataC* outerLoopData;

    bool grabResponse;
    MTM1M3_logevent_modbusResponseC* modbusResponse;

public:
    ILCResponseParser();
    ILCResponseParser(ForceActuatorSettings* forceActuatorSettings,
                      HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher,
                      ILCSubnetData* subnetData, SafetyController* safetyController);

    void parse(ModbusBuffer* buffer, uint8_t subnet);
    void incExpectedResponses(int32_t* fa, int32_t* hp, int32_t* hm);
    void clearResponses();
    void verifyResponses();
    void grabNextResponse();

private:
    bool validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp);
    void parseOneOffCommand(ModbusBuffer* buffer, uint16_t length, double timestamp, bool valid);
    void parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId);
    void parseReportHPServerIDResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportFAServerIDResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportHMServerIDResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportHPServerStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportFAServerStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportHMServerStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseChangeHPILCModeResponse(ModbusBuffer* buffer, ILCMap map);
    void parseChangeFAILCModeResponse(ModbusBuffer* buffer, ILCMap map);
    void parseChangeHMILCModeResponse(ModbusBuffer* buffer, ILCMap map);
    void parseStepMotorResponse(ModbusBuffer* buffer, ILCMap map, double timestamp);
    void parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map);
    void parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map);
    void parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map);
    void parseDualAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map);
    void parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map);
    void parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseSetHPADCScanRateResponse(ModbusBuffer* buffer, ILCMap map);
    void parseSetFAADCScanRateResponse(ModbusBuffer* buffer, ILCMap map);
    void parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map);
    void parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map);
    void parseHPResetResponse(ModbusBuffer* buffer, ILCMap map);
    void parseFAResetResponse(ModbusBuffer* buffer, ILCMap map);
    void parseHMResetResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReadHPCalibrationResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReadFACalibrationResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReadHMPressureValuesResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportDCAIDResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportHMMezzanineIDResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportDCAStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, ILCMap map);
    void parseReportLVDTResponse(ModbusBuffer* buffer, ILCMap map);

    void checkForceActuatorMeasuredForce(ILCMap map);
    void checkForceActuatorFollowingError(ILCMap map);
    void checkHardpointActuatorMeasuredForce(int32_t actuatorId);
    void checkHardpointActuatorAirPressure(int32_t actuatorId);

    void publishForceActuatorForceWarning();

    void warnResponseTimeout(double timestamp, int32_t actuatorId);
    void warnInvalidCRC(double timestamp);
    void warnIllegalFunction(double timestamp, int32_t actuatorId);
    void warnIllegalDataValue(double timestamp, int32_t actuatorId);
    void warnInvalidLength(double timestamp, int32_t actuatorId);
    void warnUnknownSubnet(double timestamp);
    void warnUnknownAddress(double timestamp, int32_t actuatorId);
    void warnUnknownFunction(double timestamp, int32_t actuatorId);
    void warnUnknownProblem(double timestamp, int32_t actuatorId);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCRESPONSEPARSER_H_ */
