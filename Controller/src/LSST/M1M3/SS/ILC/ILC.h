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
#include <ModbusBuffer.h>
#include <SAL_m1m3C.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;

/*!
 * The ILC class used to communicate with the M1M3's 5 subnets.
 */
class ILC: public IILC {
private:
	IPublisher* publisher;
	IFPGA* fpga;

	ILCApplicationSettings ilcApplicationSettings;
	ILCSubnetData subnetData[5];

	struct StaticBusList {
		ModbusBuffer buffer;
		int32_t expectedFAResponses[156];
		int32_t expectedHPResponses[6];
	};
	StaticBusList busListSetADCChannelOffsetAndSensitivity;
	StaticBusList busListSetADCScanRate;
	StaticBusList busListSetBoostValveDCAGains;
	StaticBusList busListReset;
	StaticBusList busListReportServerID;
	StaticBusList busListReportServerStatus;
	StaticBusList busListReportADCScanRate;
	StaticBusList busListReadCalibration;
	StaticBusList busListReadBoostValveDCAGains;
	StaticBusList busListReportDCAID;
	StaticBusList busListReportDCAStatus;
	StaticBusList busListChangeILCModeDisabled;
	StaticBusList busListChangeILCModeEnabled;
	StaticBusList busListChangeILCModeStandby;
	struct FreezeSensorBusList {
		ModbusBuffer buffer;
		int32_t freezeSensorCommandIndex[5];
		int32_t faStatusCommandIndex[5];
		int32_t expectedFAResponses[156];
		int32_t expectedHPResponses[6];
	};
	FreezeSensorBusList busListFreezeSensor;

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
	uint8_t subnetToTxAddress(uint8_t subnet);
	uint8_t subnetToRxAddress(uint8_t subnet);
	void startSubnet(ModbusBuffer* buffer, uint8_t subnet);
	void endSubnet(ModbusBuffer* buffer);
	void createSetADCChannelOffsetAndSensitivityBusList();
	void createSetADCScanRateBuffer();
	void createSetBoostValveDCAGains();
	void createResetBuffer();
	void createReportServerIDBuffer();
	void createReportServerStatusBuffer();
	void createReportADCScanRateBuffer();
	void createReadCalibrationBuffer();
	void createReadBoostValveDCAGainsBuffer();
	void createReportDCAIDBuffer();
	void createReportDCAStatusBuffer();
	void createChangeILCModeBuffer(StaticBusList* busList, ILCModes::Type mode);
	void createFreezeSensorListBuffer();

	void reportServerID(ModbusBuffer* buffer, uint8_t address);
	void reportServerStatus(ModbusBuffer* buffer, uint8_t address);
	void changeILCMode(ModbusBuffer* buffer, uint8_t address, uint16_t mode);
	void reportILCMode(ModbusBuffer* buffer, uint8_t address);
	void broadcastStepMotor(ModbusBuffer* buffer, uint8_t broadcastCounter, int32_t count, int8_t* steps);
	void unicastStepMotor(ModbusBuffer* buffer, uint8_t address, int8_t steps);
	void electromechanicalForceAndStatus(ModbusBuffer* buffer, uint8_t address);
	void broadcastFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter);
	void setBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address, float primaryGain, float secondaryGain);
	void readBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address);
	void broadcastForceDemand(ModbusBuffer* buffer, uint8_t broadcastCounter, bool slewFlag, int32_t saaCount, int32_t* saaPrimarySetpoint, int32_t daaCount, int32_t* daaPrimarySetpoint, int32_t* daaSecondarySetpoint);
	void unicastSingleAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint);
	void unicastDualAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint, int32_t secondarySetpoint);
	void pneumaticForceStatus(ModbusBuffer* buffer, uint8_t address);
	void setADCScanRate(ModbusBuffer* buffer, uint8_t address, uint8_t rate);
	void reportADCScanRate(ModbusBuffer* buffer, uint8_t address);
	void setADCChannelOffsetAndSensitivity(ModbusBuffer* buffer, uint8_t address, uint8_t channel, float offset, float sensitivity);
	void reset(ModbusBuffer* buffer, uint8_t address);
	void readCalibration(ModbusBuffer* buffer, uint8_t address);
	void readDCAPressureValues(ModbusBuffer* buffer, uint8_t address);
	void reportDCAID(ModbusBuffer* buffer, uint8_t address);
	void reportDCAStatus(ModbusBuffer* buffer, uint8_t address);

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

	void writeStaticBusList(StaticBusList* busList);
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
