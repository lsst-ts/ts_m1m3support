/*
 * ILCResponseParser.h
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#ifndef ILCRESPONSEPARSER_H_
#define ILCRESPONSEPARSER_H_

#include <DataTypes.h>

struct m1m3_logevent_HardpointActuatorInfoC;
struct m1m3_HardpointStatusC;
struct m1m3_HardpointDataC;

struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_ForceActuatorStatusC;
struct m1m3_ForceActuatorDataC;

struct m1m3_logevent_HardpointMonitorInfoC;
struct m1m3_HardpointMonitorStatusC;

struct m1m3_logevent_ILCWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class ModbusBuffer;
class HardpointActuatorSettings;
class IPublisher;
class ILCSubnetData;

class ILCResponseParser {
private:
	HardpointActuatorSettings* hardpointActuatorSettings;
	IPublisher* publisher;
	ILCSubnetData* subnetData;

	int32_t faExpectedResponses[156];
	int32_t hpExpectedResponses[6];
	int32_t hmExpectedResponses[6];

	m1m3_logevent_HardpointActuatorInfoC* hardpointInfo;
	m1m3_HardpointStatusC* hardpointStatus;
	m1m3_HardpointDataC* hardpointData;

	m1m3_logevent_ForceActuatorInfoC* forceInfo;
	m1m3_ForceActuatorStatusC* forceStatus;
	m1m3_ForceActuatorDataC* forceData;

	m1m3_logevent_HardpointMonitorInfoC* hardpointMonitorInfo;
	m1m3_HardpointMonitorStatusC* hardpointMonitorStatus;

	m1m3_logevent_ILCWarningC* ilcWarning;

public:
	ILCResponseParser();
	ILCResponseParser(HardpointActuatorSettings* hardpointActuatorSettings, IPublisher* publisher, ILCSubnetData* subnetData);

	void parse(ModbusBuffer* buffer, uint8_t subnet);
	void incExpectedResponses(int32_t* fa, int32_t* hp);
	void verifyResponses();

private:
	bool validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp);
	void parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId);
	void parseReportHPServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportFAServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportHMServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportHPServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportFAServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportHMServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseChangeHPILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseChangeFAILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseChangeHMILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseStepMotorResponse(ModbusBuffer* buffer, int32_t dataIndex, double timestamp);
	void parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex);
	void parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseDualAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex);
	void parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetHPADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetFAADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetHMADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetHMADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseHPResetResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseFAResetResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseHMResetResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadHPCalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadFACalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadHMCalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportDCAIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportDCAStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportLVDTResponse(ModbusBuffer* buffer, int32_t dataIndex);

	void calculateHPPostion();

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
