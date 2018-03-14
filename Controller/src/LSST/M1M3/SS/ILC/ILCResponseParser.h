/*
 * ILCResponseParser.h
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#ifndef ILCRESPONSEPARSER_H_
#define ILCRESPONSEPARSER_H_

#include <DataTypes.h>
#include <ILCDataTypes.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ModbusBuffer;
class HardpointActuatorSettings;
class ForceActuatorSettings;
class M1M3SSPublisher;
class ILCSubnetData;

class ILCResponseParser {
private:
	HardpointActuatorSettings* hardpointActuatorSettings;
	ForceActuatorSettings* forceActuatorSettings;
	M1M3SSPublisher* publisher;
	ILCSubnetData* subnetData;

	int32_t faExpectedResponses[156];
	int32_t hpExpectedResponses[6];
	int32_t hmExpectedResponses[6];

	m1m3_logevent_HardpointActuatorInfoC* hardpointActuatorInfo;
	m1m3_logevent_HardpointActuatorStateC* hardpointActuatorState;
	m1m3_logevent_HardpointActuatorWarningC* hardpointActuatorWarning;
	m1m3_HardpointActuatorDataC* hardpointActuatorData;
	m1m3_MirrorPositionDataC* mirrorPositionData;

	m1m3_logevent_ForceActuatorInfoC* forceActuatorInfo;
	m1m3_logevent_ForceActuatorStateC* forceActuatorState;
	m1m3_logevent_ForceActuatorWarningC* forceActuatorWarning;
	m1m3_logevent_ForceActuatorForceWarningC* forceWarning;
	m1m3_logevent_AppliedCylinderForcesC* appliedCylinderForces;
	m1m3_ForceActuatorDataC* forceActuatorData;

	m1m3_logevent_HardpointMonitorInfoC* hardpointMonitorInfo;
	m1m3_logevent_HardpointMonitorStateC* hardpointMonitorState;
	m1m3_logevent_HardpointMonitorWarningC* hardpointMonitorWarning;
	m1m3_HardpointMonitorDataC* hardpointMonitorData;

	m1m3_logevent_ILCWarningC* ilcWarning;

	m1m3_OuterLoopDataC* outerLoopData;

public:
	ILCResponseParser();
	ILCResponseParser(ForceActuatorSettings* forceActuatorSettings, HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher, ILCSubnetData* subnetData);

	void parse(ModbusBuffer* buffer, uint8_t subnet);
	void incExpectedResponses(int32_t* fa, int32_t* hp, int32_t* hm);
	void verifyResponses();

private:
	bool validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp);
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

	void calculateHPPostion();

	void checkForceActuatorMeasuredForce(ILCMap map);
	void checkForceActuatorFollowingError(ILCMap map);

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
