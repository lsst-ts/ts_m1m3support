/*
 * ILC.cpp
 *
 *  Created on: Oct 1, 2017
 *      Author: ccontaxis
 */

#include <ILC.h>
#include <IPublisher.h>
#include <IFPGA.h>
#include <DataTypes.h>
#include <iostream>
#include <unistd.h>
#include <FPGAAddresses.h>
#include <Timestamp.h>
#include <cstring>
#include <IBusList.h>
#include <ForceConverter.h>

using namespace std;

#define SUBNET_COUNT 5
#define FA_COUNT 156
#define HP_COUNT 6
#define ADDRESS_COUNT 256

namespace LSST {
namespace M1M3 {
namespace SS {

ILC::ILC(IPublisher* publisher, IFPGA* fpga, ILCApplicationSettings ilcApplicationSettings, ForceActuatorApplicationSettings forceActuatorApplicationSettings, HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings)
 : subnetData(forceActuatorApplicationSettings, hardpointActuatorApplicationSettings),
   ilcMessageFactory(&ilcApplicationSettings) {

	this->publisher = publisher;
	this->fpga = fpga;
	this->ilcApplicationSettings = ilcApplicationSettings;
	this->subnetStartIndex = 0;
	cout << "Populate force info" << endl;
	for(int i = 0; i < FA_COUNT; i++) {
		ForceActuatorTableRow row = forceActuatorApplicationSettings.Table[i];
		this->forceInfo.ReferenceId[row.Index] = row.ActuatorID;
		this->forceInfo.ModbusSubnet[row.Index] = row.Subnet;
		this->forceInfo.ModbusAddress[row.Index] = row.Address;
		this->forceInfo.ActuatorType[row.Index] = row.Type;
		this->forceInfo.ActuatorOrientation[row.Index] = row.Orientation;
		this->forceInfo.XPosition[row.Index] = row.XPosition;
		this->forceInfo.YPosition[row.Index] = row.YPosition;
		this->forceInfo.ZPosition[row.Index] = row.ZPosition;
		this->forceInfo.PrimaryCylinderSensorOffset[row.Index] = row.PrimaryAxisSensorOffset;
		this->forceInfo.PrimaryCylinderSensorSensitivity[row.Index] = row.PrimaryAxisSensorSensitivity;
		this->forceInfo.SecondaryCylinderSensorOffset[row.Index] = row.SecondaryAxisSensorOffset;
		this->forceInfo.SecondaryCylinderSensorSensitivity[row.Index] = row.SecondaryAxisSensorSensitivity;
	}
	cout << "Populate hp info" << endl;
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings.Table[i];
		this->hardpointInfo.ReferenceId[row.Index] = row.ActuatorID;
		this->hardpointInfo.ModbusSubnet[row.Index] = row.Subnet;
		this->hardpointInfo.ModbusAddress[row.Index] = row.Address;
		this->hardpointInfo.XPosition[row.Index] = row.XPosition;
		this->hardpointInfo.YPosition[row.Index] = row.YPosition;
		this->hardpointInfo.ZPosition[row.Index] = row.ZPosition;
		this->hardpointInfo.SensorOffset[row.Index] = row.SensorOffset;
		this->hardpointInfo.SensorSensitivity[row.Index] = row.SensorSensitivity;
	}
	this->broadcastCounter = 0;
	this->faStatusIndex[0] = 0;
	this->faStatusIndex[1] = 0;
	this->faStatusIndex[2] = 0;
	this->faStatusIndex[3] = 0;
	this->faStatusIndex[4] = 0;
	this->ilcWarning.Timestamp = 0;
	this->ilcWarning.ActuatorId = -1;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	cout << "Create buffers" << endl;
	this->busListSetADCChannelOffsetAndSensitivity = SetADCChanneOffsetAndSensitivityBusList(&this->subnetData, &this->ilcMessageFactory, &this->forceInfo, &this->hardpointInfo);
	cout << "!" << endl;
	this->busListSetADCScanRate = SetADCScanRateBusList(&this->subnetData, &this->ilcMessageFactory, &this->forceInfo, &this->hardpointInfo);
	cout << "!" << endl;
	this->busListSetBoostValveDCAGains = SetBoostValveDCAGainBusList(&this->subnetData, &this->ilcMessageFactory, &this->forceInfo);
	cout << "!" << endl;
	this->busListReset = ResetBustList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReportServerID = ReportServerIDBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReportServerStatus = ReportServerStatusBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReportADCScanRate = ReportADCScanRateBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReadCalibration = ReadCalibrationBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReadBoostValveDCAGains = ReadBoostValveDCAGainBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReportDCAID = ReportDCAIDBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListReportDCAStatus = ReportDCAStatusBusList(&this->subnetData, &this->ilcMessageFactory);
	cout << "!" << endl;
	this->busListChangeILCModeDisabled = ChangeILCModeBusList(&this->subnetData, &this->ilcMessageFactory, ILCModes::Disabled);
	cout << "!" << endl;
	this->busListChangeILCModeEnabled = ChangeILCModeBusList(&this->subnetData, &this->ilcMessageFactory, ILCModes::Enabled);
	cout << "!" << endl;
	this->busListChangeILCModeStandby = ChangeILCModeBusList(&this->subnetData, &this->ilcMessageFactory, ILCModes::Standby);
	cout << "!" << endl;
	this->busListFreezeSensor = FreezeSensorBusList(&this->subnetData, &this->ilcMessageFactory, this->publisher->getOuterLoopData());
	cout << "!" << endl;
	this->busListRaised = RaisedBusList(&this->subnetData, &this->ilcMessageFactory, this->publisher->getOuterLoopData(), &this->forceData, &this->hardpointData, &this->forceInfo);
}

ILC::~ILC() { }

void ILC::writeCalibrationDataBuffer() {
	this->writeBusList(&this->busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
	this->writeBusList(&this->busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
	this->writeBusList(&this->busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
	this->writeBusList(&this->busListReset);
}

void ILC::writeReportServerIDBuffer() {
	this->writeBusList(&this->busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
	this->writeBusList(&this->busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
	this->writeBusList(&this->busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
	this->writeBusList(&this->busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
	this->writeBusList(&this->busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
	this->writeBusList(&this->busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
	this->writeBusList(&this->busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
	this->writeBusList(&this->busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
	this->writeBusList(&this->busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
	this->writeBusList(&this->busListChangeILCModeStandby);
}

void ILC::writeFreezeSensorListBuffer() {
	this->busListFreezeSensor.update();
	this->writeBusList(&this->busListFreezeSensor);
}

void ILC::writeRaisedListBuffer() {
	this->busListRaised.update();
	this->writeBusList(&this->busListRaised);
}

void ILC::triggerModbus() {
	this->fpga->writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
}

void ILC::waitForSubnet(int32_t subnet, int32_t timeout) {
	this->fpga->waitForModbusIRQ(subnet, timeout);
	this->fpga->ackModbusIRQ(subnet);
}

void ILC::waitForAllSubnets(int32_t timeout) {
	this->waitForSubnet(1, timeout);
	this->waitForSubnet(2, timeout);
	this->waitForSubnet(3, timeout);
	this->waitForSubnet(4, timeout);
	this->waitForSubnet(5, timeout);
}

void ILC::read(uint8_t subnet) {
	// TODO: The expectation is if someone asks to read something they expect something to be there
	// so if something isn't there should be a warning (timeout on responses)
	this->u16Buffer[0] = this->subnetToRxAddress(subnet);
	this->fpga->writeRequestFIFO(this->u16Buffer, 1, 0);
	this->rxBuffer.setIndex(0);
	this->fpga->readU16ResponseFIFO(this->rxBuffer.getBuffer(), 1, 10);
	uint16_t reportedLength = this->rxBuffer.readLength();
	if (reportedLength > 0) {
		this->rxBuffer.setIndex(0);
		this->fpga->readU16ResponseFIFO(this->rxBuffer.getBuffer(), reportedLength, 10);
		this->rxBuffer.setLength(reportedLength);
		this->parse(&this->rxBuffer, subnet);
	}
}

void ILC::readAll() {
	this->read(1);
	this->read(2);
	this->read(3);
	this->read(4);
	this->read(5);
}

void ILC::verifyResponses() {
	double timestamp = this->publisher->getTimestamp();
	cout << "BADFA: ";
	for(int i = 0; i < FA_COUNT; i++) {
		if (this->faExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->forceInfo.ReferenceId[i]);
			cout << this->forceInfo.ReferenceId[i] << " ";
			this->faExpectedResponses[i] = 0;
		}
	}
	cout << endl << "BADHP: ";
	for(int i = 0; i < HP_COUNT; i++) {
		if (this->hpExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->hardpointInfo.ReferenceId[i]);
			cout << this->hardpointInfo.ReferenceId[i] << " ";
			this->hpExpectedResponses[i] = 0;
		}
	}
	cout << endl;
}

void ILC::publishForceActuatorStatus() {
	this->publisher->putForceActuatorStatus(&this->forceStatus);
}

void ILC::publishForceActuatorData() {
	this->publisher->putForceActuatorData(&this->forceData);
}

void ILC::publishHardpointStatus() {
	this->publisher->putHardpointStatus(&this->hardpointStatus);
}

void ILC::publishHardpointData() {
	this->publisher->putHardpointData(&this->hardpointData);
}

void ILC::foo() {
	int count = 0;
	for(int i = 0; i < FA_COUNT; i++) {
		cout << this->faExpectedResponses[i] << " ";
		count += this->faExpectedResponses[i];
	}
	cout << endl;
	for(int i = 0; i < HP_COUNT; i++) {
		cout << this->hpExpectedResponses[i] << " ";
		count += this->hpExpectedResponses[i];
	}
	cout << endl;
	cout << "TOTAL: " << count << endl;
}

uint8_t ILC::subnetToRxAddress(uint8_t subnet) {
	switch(subnet) {
	case 1: return FPGAAddresses::ModbusSubnetARx;
	case 2: return FPGAAddresses::ModbusSubnetBRx;
	case 3: return FPGAAddresses::ModbusSubnetCRx;
	case 4: return FPGAAddresses::ModbusSubnetDRx;
	case 5: return FPGAAddresses::ModbusSubnetERx;
	default: return 0;
	}
}

void ILC::writeBusList(IBusList* busList) {
	this->fpga->writeCommandFIFO(busList->getBuffer(), busList->getLength(), 0);
	this->incExpectedResponses(busList->getExpectedFAResponses(), busList->getExpectedHPResponses());
}

void ILC::parse(ModbusBuffer* buffer, uint8_t subnet) {
	uint64_t a = buffer->readLength();
	uint64_t b = buffer->readLength();
	uint64_t c = buffer->readLength();
	uint64_t d = buffer->readLength();
	double globalTimestamp = Timestamp::fromRaw((a << 48) | (b << 32) | (c << 16) | d);
	this->forceStatus.Timestamp = globalTimestamp;
	this->forceData.Timestamp = globalTimestamp;
	this->hardpointStatus.Timestamp = globalTimestamp;
	this->hardpointData.Timestamp = globalTimestamp;
	while(!buffer->endOfBuffer()) {
		uint16_t length = 0;
		double timestamp = 0;
		if (this->validateCRC(buffer, &length, &timestamp)) {
			if(subnet >= 1 && subnet <= 5) {
				uint8_t address = buffer->readU8();
				uint8_t function = buffer->readU8();
				ILCAddressDataMap map = this->subnetData.getILCDataFromAddress(subnet - 1, address);
				int32_t dataIndex = map.DataIndex;
				switch(map.Type) {
				case ILCTypes::FA:
					this->faExpectedResponses[dataIndex]--;
					switch(function) {
					case 17: this->parseReportFAServerIDResponse(buffer, dataIndex); break;
					case 18: this->parseReportFAServerStatusResponse(buffer, dataIndex); break;
					case 65: this->parseChangeFAILCModeResponse(buffer, dataIndex); break;
					case 73: this->parseSetBoostValveDCAGainsResponse(buffer, dataIndex); break;
					case 74: this->parseReadBoostValveDCAGainsResponse(buffer, dataIndex); break;
					case 75: this->parseForceDemandResponse(buffer, address, dataIndex); break;
					case 76: this->parsePneumaticForceStatusResponse(buffer, address, dataIndex); break;
					case 80: this->parseSetFAADCScanRateResponse(buffer, dataIndex); break;
					case 81: this->parseSetFAADCChannelOffsetAndSensitivityResponse(buffer, dataIndex); break;
					case 107: this->parseFAResetResponse(buffer, dataIndex); break;
					case 110: this->parseReadFACalibrationResponse(buffer, dataIndex); break;
					case 119: this->parseReadDCAPressureValuesResponse(buffer, dataIndex); break;
					case 120: this->parseReportDCAIDResponse(buffer, dataIndex); break;
					case 121: this->parseReportDCAStatusResponse(buffer, dataIndex); break;
					case 145:
					case 146:
					case 193:
					case 201:
					case 202:
					case 203:
					case 204:
					case 208:
					case 209:
					case 235:
					case 238:
					case 247:
					case 248:
					case 249: this->parseErrorResponse(buffer, timestamp, map.ActuatorId); break;
					default: this->warnUnknownFunction(timestamp, map.ActuatorId); break;
					}
					break;
				case ILCTypes::HP:
					this->hpExpectedResponses[dataIndex]--;
					switch(function) {
					case 17: this->parseReportHPServerIDResponse(buffer, dataIndex); break;
					case 18: this->parseReportHPServerStatusResponse(buffer, dataIndex); break;
					case 65: this->parseChangeHPILCModeResponse(buffer, dataIndex); break;
					case 66: this->parseStepMotorResponse(buffer, dataIndex, timestamp); break;
					case 67: this->parseElectromechanicalForceAndStatusResponse(buffer, dataIndex); break;
					case 80: this->parseSetHPADCScanRateResponse(buffer, dataIndex); break;
					case 81: this->parseSetHPADCChannelOffsetAndSensitivityResponse(buffer, dataIndex); break;
					case 107: this->parseHPResetResponse(buffer, dataIndex); break;
					case 110: this->parseReadHPCalibrationResponse(buffer, dataIndex); break;
					case 145:
					case 146:
					case 193:
					case 194:
					case 195:
					case 208:
					case 209:
					case 235:
					case 238: this->parseErrorResponse(buffer, timestamp, map.ActuatorId); break;
					default: this->warnUnknownFunction(timestamp, map.ActuatorId); break;
					}
					break;
				default: this->warnUnknownAddress(timestamp, map.ActuatorId); break;
				}
			}
			else {
				this->warnUnknownSubnet(timestamp);
			}
		}
		else {
			this->warnInvalidCRC(timestamp);
		}
	}
}

bool ILC::validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp) {
	int32_t addressIndex = buffer->getIndex();
	while(!buffer->endOfFrame() && !buffer->endOfBuffer() && (buffer->readLength() & 0xF000) != 0xB000) { }
	int32_t crcIndex = buffer->getIndex() - 3;
	(*length) = crcIndex - addressIndex + 11;
	buffer->setIndex(crcIndex);
	uint16_t calculatedCRC = buffer->calculateCRC(crcIndex - addressIndex);
	uint16_t crc = buffer->readCRC();
	(*timestamp) = buffer->readTimestamp();
	if (crc == calculatedCRC) {
		buffer->setIndex(addressIndex);
		return true;
	}
	else {
		buffer->readEndOfFrame();
		return false;
	}
}

void ILC::parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId) {
	uint8_t exceptionCode = buffer->readU8();
	if (exceptionCode == 1) {
		this->warnIllegalFunction(timestamp, actuatorId);
	}
	else if (exceptionCode == 3) {
		this->warnIllegalDataValue(timestamp, actuatorId);
	}
	else {
		this->warnUnknownProblem(timestamp, actuatorId);
	}
	buffer->skipToNextFrame();
}

void ILC::parseReportHPServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->hardpointInfo.ILCUniqueId[dataIndex] = buffer->readU48();
	this->hardpointInfo.ILCApplicationType[dataIndex] = buffer->readU8();
	this->hardpointInfo.NetworkNodeType[dataIndex] = buffer->readU8();
	this->hardpointInfo.ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->hardpointInfo.NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->hardpointInfo.MajorRevision[dataIndex] = buffer->readU8();
	this->hardpointInfo.MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILC::parseReportFAServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->forceInfo.ILCUniqueId[dataIndex] = buffer->readU48();
	this->forceInfo.ILCApplicationType[dataIndex] = buffer->readU8();
	this->forceInfo.NetworkNodeType[dataIndex] = buffer->readU8();
	this->forceInfo.ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->forceInfo.NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->forceInfo.MajorRevision[dataIndex] = buffer->readU8();
	this->forceInfo.MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILC::parseReportHPServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointStatus.Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointStatus.MajorFault[dataIndex] = ilcStatus & 0x0001;
	this->hardpointStatus.MinorFault[dataIndex] = ilcStatus & 0x0002;
	// 0x0004 is reserved
	this->hardpointStatus.FaultOverridden[dataIndex] = ilcStatus & 0x0008;
	this->hardpointInfo.MainCalibrationError[dataIndex] = ilcStatus & 0x0010;
	this->hardpointInfo.BackupCalibrationError[dataIndex] = ilcStatus & 0x0020;
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointData.CWLimitOperated[dataIndex] = ilcStatus & 0x0100;
	this->hardpointData.CCWLimitOperated[dataIndex] = ilcStatus & 0x0200;
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA (FA only)
	// 0x4000 is DCA (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->hardpointInfo.UniqueIdCRCError[dataIndex] = ilcFaults & 0x0001;
	this->hardpointInfo.ApplicationTypeMismatch[dataIndex] = ilcFaults & 0x0002;
	this->hardpointInfo.ApplicationMissing[dataIndex] = ilcFaults & 0x0004;
	this->hardpointInfo.ApplicationCRCMismatch[dataIndex] = ilcFaults & 0x0008;
	this->hardpointInfo.OneWireMissing[dataIndex] = ilcFaults & 0x0010;
	this->hardpointInfo.OneWire1Mismatch[dataIndex] = ilcFaults & 0x0020;
	this->hardpointInfo.OneWire2Mismatch[dataIndex] = ilcFaults & 0x0040;
	// 0x0080 is reserved
	this->hardpointStatus.WatchdogReset[dataIndex] = ilcFaults & 0x0100;
	this->hardpointStatus.BrownoutDetected[dataIndex] = ilcFaults & 0x0200;
	this->hardpointStatus.EventTrapReset[dataIndex] = ilcFaults & 0x0400;
	this->hardpointStatus.MotorPowerFault[dataIndex] = ilcFaults & 0x0800;
	this->hardpointStatus.SSRPowerFault[dataIndex] = ilcFaults & 0x1000;
	this->hardpointStatus.AUXPowerFault[dataIndex] = ilcFaults & 0x2000;
	this->hardpointStatus.SMCPowerFault[dataIndex] = ilcFaults & 0x4000;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILC::parseReportFAServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceStatus.Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->forceStatus.MajorFault[dataIndex] = ilcStatus & 0x0001;
	this->forceStatus.MinorFault[dataIndex] = ilcStatus & 0x0002;
	// 0x0004 is reserved
	this->forceStatus.FaultOverridden[dataIndex] = ilcStatus & 0x0008;
	this->forceInfo.MainCalibrationError[dataIndex] = ilcStatus & 0x0010;
	this->forceInfo.BackupCalibrationError[dataIndex] = ilcStatus & 0x0020;
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	this->forceStatus.DCAFault[dataIndex] = ilcStatus & 0x2000;
	this->forceInfo.DCAFirmwareUpdate[dataIndex] = ilcStatus & 0x4000;
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->forceInfo.UniqueIdCRCError[dataIndex] = ilcFaults & 0x0001;
	this->forceInfo.ApplicationTypeMismatch[dataIndex] = ilcFaults & 0x0002;
	this->forceInfo.ApplicationMissing[dataIndex] = ilcFaults & 0x0004;
	this->forceInfo.ApplicationCRCMismatch[dataIndex] = ilcFaults & 0x0008;
	this->forceInfo.OneWireMissing[dataIndex] = ilcFaults & 0x0010;
	this->forceInfo.OneWire1Mismatch[dataIndex] = ilcFaults & 0x0020;
	this->forceInfo.OneWire2Mismatch[dataIndex] = ilcFaults & 0x0040;
	// 0x0080 is reserved
	this->forceStatus.WatchdogReset[dataIndex] = ilcFaults & 0x0100;
	this->forceStatus.BrownoutDetected[dataIndex] = ilcFaults & 0x0200;
	this->forceStatus.EventTrapReset[dataIndex] = ilcFaults & 0x0400;
	this->forceStatus.MotorPowerFault[dataIndex] = ilcFaults & 0x0800;
	this->forceStatus.SSRPowerFault[dataIndex] = ilcFaults & 0x1000;
	this->forceStatus.AUXPowerFault[dataIndex] = ilcFaults & 0x2000;
	this->forceStatus.SMCPowerFault[dataIndex] = ilcFaults & 0x4000;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILC::parseChangeHPILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointStatus.Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseChangeFAILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceStatus.Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseStepMotorResponse(ModbusBuffer* buffer, int32_t dataIndex, double timestamp) {
	uint8_t status = buffer->readU8();
	this->hardpointData.Timestamp = timestamp;
	this->hardpointData.StepsCommanded[0] = this->hpStepCommand[0];
	this->hardpointData.StepsCommanded[1] = this->hpStepCommand[1];
	this->hardpointData.StepsCommanded[2] = this->hpStepCommand[2];
	this->hardpointData.StepsCommanded[3] = this->hpStepCommand[3];
	this->hardpointData.StepsCommanded[4] = this->hpStepCommand[4];
	this->hardpointData.StepsCommanded[5] = this->hpStepCommand[5];
	this->hardpointData.ILCFault[dataIndex] = status & 0x01;
	// 0x02 is reserved
	this->hardpointData.CWLimitOperated[dataIndex] = status & 0x04;
	this->hardpointData.CCWLimitOperated[dataIndex] = status & 0x08;
	this->hardpointData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->hardpointData.Encoder[dataIndex] = buffer->readI32();
	this->hardpointData.Force[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->hardpointData.ILCFault[dataIndex] = status & 0x01;
	// 0x02 is reserved
	this->hardpointData.CWLimitOperated[dataIndex] = status & 0x04;
	this->hardpointData.CCWLimitOperated[dataIndex] = status & 0x08;
	this->hardpointData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->hardpointData.Encoder[dataIndex] = buffer->readI32();
	this->hardpointData.Force[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.DCAAxialGain[dataIndex] = buffer->readSGL();
	this->forceInfo.DCALateralGain[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex) {
	if (address <= 16) {
		this->parseSingleAxisForceDemandResponse(buffer, dataIndex);
	}
	else {
		this->parseDualAxisForceDemandResponse(buffer, dataIndex);
	}
}

void ILC::parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceData.SecondaryCylinderForce[dataIndex] = 0;
	ForceConverter::saaToMirror(this->forceData.PrimaryCylinderForce[dataIndex], this->forceData.SecondaryCylinderForce[dataIndex], this->forceData.XForce + dataIndex, this->forceData.YForce + dataIndex, this->forceData.ZForce + dataIndex);
	buffer->skipToNextFrame();
}

void ILC::parseDualAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceData.SecondaryCylinderForce[dataIndex] = buffer->readSGL();
	switch(this->forceInfo.ActuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceData.PrimaryCylinderForce[dataIndex], this->forceData.SecondaryCylinderForce[dataIndex], this->forceData.XForce + dataIndex, this->forceData.YForce + dataIndex, this->forceData.ZForce + dataIndex); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceData.PrimaryCylinderForce[dataIndex], this->forceData.SecondaryCylinderForce[dataIndex], this->forceData.XForce + dataIndex, this->forceData.YForce + dataIndex, this->forceData.ZForce + dataIndex); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceData.PrimaryCylinderForce[dataIndex], this->forceData.SecondaryCylinderForce[dataIndex], this->forceData.XForce + dataIndex, this->forceData.YForce + dataIndex, this->forceData.ZForce + dataIndex); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceData.PrimaryCylinderForce[dataIndex], this->forceData.SecondaryCylinderForce[dataIndex], this->forceData.XForce + dataIndex, this->forceData.YForce + dataIndex, this->forceData.ZForce + dataIndex); break;
	}
	buffer->skipToNextFrame();
}

void ILC::parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex) {
	if (address <= 16) {
		this->parseSingleAxisPneumaticForceStatusResponse(buffer, dataIndex);
	}
	else {
		this->parseDualAxisPneumaticForceStatusResponse(buffer, dataIndex);
	}
}

void ILC::parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.ZForce[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.ZForce[dataIndex] = buffer->readSGL();
	this->forceData.YForce[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseSetHPADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointInfo.ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseSetFAADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseHPResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseFAResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseReadHPCalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointInfo.MainADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo.MainLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo.BackupLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseReadFACalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.MainADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->forceInfo.MainADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->forceInfo.MainADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->forceInfo.MainADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->forceInfo.MainAxialLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceInfo.MainLateralLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo.MainAxialLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceInfo.MainLateralLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupAxialLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupLateralLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo.BackupAxialLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupLateralLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseReportDCAIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.DCAUniqueId[dataIndex] = buffer->readU48();
	this->forceInfo.DCAFirmwareType[dataIndex] = buffer->readU8();
	this->forceInfo.DCAMajorRevision[dataIndex] = buffer->readU8();
	this->forceInfo.DCAMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseReportDCAStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint16_t status = buffer->readU16();
	this->forceStatus.DCAOutputsEnabled[dataIndex] = status & 0x0001;
	this->forceStatus.DCAPowerFault[dataIndex] = status & 0x0002;
	this->forceStatus.DCAAmplifierAFault[dataIndex] = status & 0x0004;
	this->forceStatus.DCAAmplifierBFault[dataIndex] = status & 0x0008;
	this->forceInfo.DCAUniqueIdCRCError[dataIndex] = status & 0x0010;
	// 0x0020 is reserved
	this->forceInfo.DCAMainCalibrationError[dataIndex] = status & 0x0040;
	this->forceInfo.DCABackupCalibrationError[dataIndex] = status & 0x0080;
	this->forceStatus.DCAEventTrapReset[dataIndex] = status & 0x0100;
	// 0x0200 is reserved
	// 0x0400 is reserved
	// 0x0800 is reserved
	this->forceInfo.DCAApplicationMissing[dataIndex] = status & 0x1000;
	this->forceInfo.DCAApplicationCRCMismatch[dataIndex] = status & 0x2000;
	// 0x4000 is reserved
	this->forceInfo.DCABootloaderActive[dataIndex] = status & 0x8000;
	buffer->skipToNextFrame();
}

void ILC::incExpectedResponses(int32_t* fa, int32_t* hp) {
	for(int i = 0; i < FA_COUNT; i++) {
		this->faExpectedResponses[i] += fa[i];
	}
	for (int i = 0; i < HP_COUNT; i++) {
		this->hpExpectedResponses[i] += hp[i];
	}
}

void ILC::incBroadcastCounter() {
	this->broadcastCounter++;
	if (this->broadcastCounter > 15) {
		this->broadcastCounter = 0;
	}
}

void ILC::incFAStatusIndex() {
	for (int i = 0; i < SUBNET_COUNT; i++) {
		if (this->faStatusIndex[i] != -1) {
			this->faStatusIndex[i]++;
			if (this->faStatusIndex[i] >= this->subnetData.getFACount(i)) {
				this->faStatusIndex[i] = 0;
			}
		}
	}
}

void ILC::updateHPSteps() {
	for(int i = 0; i < 6; i++) {
		if (this->hpSteps[i] != 0) {
			if (this->hpSteps[i] > 100) {
				this->hpSteps[i] -= 100;
				this->hpStepCommand[i] = 100;
			}
			else if (this->hpSteps[i] < -100) {
				this->hpSteps[i] += 100;
				this->hpStepCommand[i] = -100;
			}
			else {
				this->hpStepCommand[i] = this->hpSteps[i];
				this->hpSteps[i] = 0;
			}
		}
		else {
			this->hpStepCommand[i] = 0;
		}
	}
}

void ILC::warnResponseTimeout(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = true;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnInvalidCRC(double timestamp) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = -1;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = true;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnIllegalFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = true;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnIllegalDataValue(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = true;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnInvalidLength(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = true;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownSubnet(double timestamp) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = -1;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = true;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownAddress(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = true;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = true;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownProblem(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = true;
	this->publisher->logILCWarning(&this->ilcWarning);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
