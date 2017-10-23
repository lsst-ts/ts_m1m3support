/*
 * ILC.h
 *
 *  Created on: Oct 1, 2017
 *      Author: ccontaxis
 */

#ifndef ILC_H_
#define ILC_H_

#include <IILC.h>
#include <ILCDataTypes.h>
#include <ILCSubnetData.h>
#include <ModbusBuffer.h>
#include <SAL_m1m3C.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <ILCMessageFactory.h>
#include <SetADCChanneOffsetAndSensitivityBusList.h>
#include <ChangeILCModeBusList.h>
#include <ReadBoostValveDCAGainBusList.h>
#include <ReadCalibrationBusList.h>
#include <ReportADCScanRateBusList.h>
#include <ReportDCAIDBusList.h>
#include <ReportDCAStatusBusList.h>
#include <ReportServerIDBusList.h>
#include <ReportServerStatusBusList.h>
#include <ResetBustList.h>
#include <SetADCScanRateBusList.h>
#include <SetBoostValveDCAGainBusList.h>
#include <FreezeSensorBusList.h>
#include <RaisedBusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;
class IBusList;

/*!
 * The ILC class used to communicate with the M1M3's 5 subnets.
 */
class ILC: public IILC {
private:
	IPublisher* publisher;
	IFPGA* fpga;

	ILCApplicationSettings ilcApplicationSettings;

	ILCSubnetData subnetData;
	ILCMessageFactory ilcMessageFactory;

	SetADCChanneOffsetAndSensitivityBusList busListSetADCChannelOffsetAndSensitivity;
	SetADCScanRateBusList busListSetADCScanRate;
	SetBoostValveDCAGainBusList busListSetBoostValveDCAGains;
	ResetBustList busListReset;
	ReportServerIDBusList busListReportServerID;
	ReportServerStatusBusList busListReportServerStatus;
	ReportADCScanRateBusList busListReportADCScanRate;
	ReadCalibrationBusList busListReadCalibration;
	ReadBoostValveDCAGainBusList busListReadBoostValveDCAGains;
	ReportDCAIDBusList busListReportDCAID;
	ReportDCAStatusBusList busListReportDCAStatus;
	ChangeILCModeBusList busListChangeILCModeDisabled;
	ChangeILCModeBusList busListChangeILCModeEnabled;
	ChangeILCModeBusList busListChangeILCModeStandby;
	FreezeSensorBusList busListFreezeSensor;
	RaisedBusList busListRaised;

	int32_t faExpectedResponses[156];
	int32_t hpExpectedResponses[6];

	uint8_t broadcastCounter;
	int32_t faStatusIndex[5];

	int8_t hpStepCommand[6];
	int32_t hpSteps[6];

	uint16_t u16Buffer[1];
	ModbusBuffer rxBuffer;

	m1m3_logevent_HardpointActuatorInfoC hardpointInfo;
	m1m3_HardpointStatusC hardpointStatus;
	m1m3_HardpointDataC hardpointData;

	m1m3_logevent_ForceActuatorInfoC forceInfo;
	m1m3_ForceActuatorStatusC forceStatus;
	m1m3_ForceActuatorDataC forceData;

	m1m3_logevent_ILCWarningC ilcWarning;

	int subnetStartIndex;


public:
	ILC(IPublisher* publisher, IFPGA* fpga, ILCApplicationSettings ilcApplicationSettings, ForceActuatorApplicationSettings forceActuatorApplicationSettings, HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings);
	virtual ~ILC();

	void writeCalibrationDataBuffer();
	void writeSetADCScanRateBuffer();
	void writeSetBoostValveDCAGainBuffer();
	void writeResetBuffer();
	void writeReportServerIDBuffer();
	void writeReportServerStatusBuffer();
	void writeReportADCScanRateBuffer();
	void writeReadCalibrationDataBuffer();
	void writeReadBoostValveDCAGainBuffer();
	void writeReportDCAIDBuffer();
	void writeReportDCAStatusBuffer();
	void writeSetModeDisableBuffer();
	void writeSetModeEnableBuffer();
	void writeSetModeStandbyBuffer();
	void writeFreezeSensorListBuffer();
	void writeRaisedListBuffer();

	void triggerModbus();

	void waitForSubnet(int32_t subnet, int32_t timeout);
	void waitForAllSubnets(int32_t timeout);

	void read(uint8_t subnet);
	void readAll();

	void verifyResponses();

	void publishForceActuatorStatus();
	void publishForceActuatorData();
	void publishHardpointStatus();
	void publishHardpointData();

	void foo();

private:
	uint8_t subnetToRxAddress(uint8_t subnet);

	void writeBusList(IBusList* busList);

	void parse(ModbusBuffer* buffer, uint8_t subnet);
	bool validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp);
	void parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId);
	void parseReportHPServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportFAServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportHPServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportFAServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseChangeHPILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseChangeFAILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex);
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
	void parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseHPResetResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseFAResetResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadHPCalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadFACalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportDCAIDResponse(ModbusBuffer* buffer, int32_t dataIndex);
	void parseReportDCAStatusResponse(ModbusBuffer* buffer, int32_t dataIndex);

	void incExpectedResponses(int32_t* fa, int32_t* hp);
	void incBroadcastCounter();
	void incFAStatusIndex();
	void updateHPSteps();

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

#endif /* ILC_H_ */
