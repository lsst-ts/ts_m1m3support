/*
 * ILCResponseParser.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#include <ILCResponseParser.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <ModbusBuffer.h>
#include <ForceConverter.h>
#include <ForceActuatorApplicationSettings.h>
#include <ILCDataTypes.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <ILCSubnetData.h>
#include <SAL_MTM1M3C.h>
#include <cmath>
#include <cstring>
#include <SALEnumerations.h>
#include <BitHelper.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCResponseParser::ILCResponseParser() {
	this->forceActuatorSettings = 0;
	this->hardpointActuatorSettings = 0;
	this->publisher = 0;
	this->subnetData = 0;
	this->safetyController = 0;
	this->hardpointActuatorInfo = 0;
	this->hardpointActuatorState = 0;
	this->hardpointActuatorWarning = 0;
	this->hardpointActuatorData = 0;
	this->forceActuatorBackupCalibrationInfo = 0;
	this->forceActuatorILCInfo = 0;
	this->forceActuatorIdInfo = 0;
	this->forceActuatorMainCalibrationInfo = 0;
	this->forceActuatorMezzanineCalibrationInfo = 0;
	this->forceActuatorPositionInfo = 0;
	this->forceActuatorState = 0;
	this->forceActuatorWarning = 0;
	this->appliedCylinderForces = 0;
	this->forceActuatorData = 0;
	this->hardpointMonitorInfo = 0;
	this->hardpointMonitorState = 0;
	this->hardpointMonitorWarning = 0;
	this->hardpointMonitorData = 0;
	this->outerLoopData = 0;
	this->modbusResponse = 0;
	this->grabResponse = false;
	this->modbusWarning = 0;
}

ILCResponseParser::ILCResponseParser(ForceActuatorSettings* forceActuatorSettings, HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher, ILCSubnetData* subnetData, SafetyController* safetyController) {
	Log.Debug("ILCResponseParser: ILCResponseParser()");
	this->forceActuatorSettings = forceActuatorSettings;
	this->hardpointActuatorSettings = hardpointActuatorSettings;
	this->publisher = publisher;
	this->subnetData = subnetData;
	this->safetyController = safetyController;
	this->hardpointActuatorInfo = this->publisher->getEventHardpointActuatorInfo();
	this->hardpointActuatorState = this->publisher->getEventHardpointActuatorState();
	this->hardpointActuatorWarning = this->publisher->getEventHardpointActuatorWarning();
	this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
	this->forceActuatorBackupCalibrationInfo = this->publisher->getEventForceActuatorBackupCalibrationInfo();
	this->forceActuatorILCInfo = this->publisher->getEventForceActuatorILCInfo();
	this->forceActuatorIdInfo = this->publisher->getEventForceActuatorIdInfo();
	this->forceActuatorMainCalibrationInfo = this->publisher->getEventForceActuatorMainCalibrationInfo();
	this->forceActuatorMezzanineCalibrationInfo = this->publisher->getEventForceActuatorMezzanineCalibrationInfo();
	this->forceActuatorPositionInfo = this->publisher->getEventForceActuatorPositionInfo();
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
	this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
	this->forceActuatorData = this->publisher->getForceActuatorData();
	this->hardpointMonitorInfo = this->publisher->getEventHardpointMonitorInfo();
	this->hardpointMonitorState = this->publisher->getEventHardpointMonitorState();
	this->hardpointMonitorWarning = this->publisher->getEventHardpointMonitorWarning();
	this->hardpointMonitorData = this->publisher->getHardpointMonitorData();
	this->outerLoopData = this->publisher->getOuterLoopData();
	this->modbusResponse = this->publisher->getEventModbusResponse();
	this->grabResponse = false;
	this->modbusWarning = this->publisher->getEventModbusWarning();

	memset(this->faExpectedResponses, 0, sizeof(this->faExpectedResponses));
	memset(this->hpExpectedResponses, 0, sizeof(this->hpExpectedResponses));
	memset(this->hmExpectedResponses, 0, sizeof(this->hmExpectedResponses));
}

void ILCResponseParser::parse(ModbusBuffer* buffer, uint8_t subnet) {
	uint64_t a = buffer->readLength();
	uint64_t b = buffer->readLength();
	uint64_t c = buffer->readLength();
	uint64_t d = buffer->readLength();
	double globalTimestamp = Timestamp::fromRaw((a << 48) | (b << 32) | (c << 16) | d);
	this->forceActuatorData->timestamp = globalTimestamp;
	this->hardpointActuatorData->timestamp = globalTimestamp;
	this->hardpointMonitorData->timestamp = globalTimestamp;
	while(!buffer->endOfBuffer()) {
		uint16_t length = 0;
		double timestamp = 0;
		if (this->validateCRC(buffer, &length, &timestamp)) {
			if (this->grabResponse) {
				this->parseOneOffCommand(buffer, length, timestamp, true);
			}
			else {
				if(subnet >= 1 && subnet <= 5) {
					uint8_t address = buffer->readU8();
					uint8_t function = buffer->readU8();
					ILCMap map = this->subnetData->getILCDataFromAddress(subnet - 1, address);
					int32_t dataIndex = map.DataIndex;
					switch(map.Type) {
					case ILCTypes::FA:
						this->faExpectedResponses[dataIndex]--;
						switch(function) {
						case 17: this->parseReportFAServerIDResponse(buffer, map); break;
						case 18: this->parseReportFAServerStatusResponse(buffer, map); break;
						case 65: this->parseChangeFAILCModeResponse(buffer, map); break;
						case 73: this->parseSetBoostValveDCAGainsResponse(buffer, map); break;
						case 74: this->parseReadBoostValveDCAGainsResponse(buffer, map); break;
						case 75: this->parseForceDemandResponse(buffer, address, map); break;
						case 76: this->parsePneumaticForceStatusResponse(buffer, address, map); break;
						case 80: this->parseSetFAADCScanRateResponse(buffer, map); break;
						case 81: this->parseSetFAADCChannelOffsetAndSensitivityResponse(buffer, map); break;
						case 107: this->parseFAResetResponse(buffer, map); break;
						case 110: this->parseReadFACalibrationResponse(buffer, map); break;
						case 119: this->parseReadDCAPressureValuesResponse(buffer, map); break;
						case 120: this->parseReportDCAIDResponse(buffer, map); break;
						case 121: this->parseReportDCAStatusResponse(buffer, map); break;
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
						case 249: this->parseFAErrorResponse(buffer, timestamp, map.ActuatorId); break;
						default:
							Log.Warn("ILCResponseParser: Unknown FA function on subnet %d function %d", (int)function, subnet);
							BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownFunction, true);
							break;
						}
						break;
					case ILCTypes::HP:
						this->hpExpectedResponses[dataIndex]--;
						switch(function) {
						case 17: this->parseReportHPServerIDResponse(buffer, map); break;
						case 18: this->parseReportHPServerStatusResponse(buffer, map); break;
						case 65: this->parseChangeHPILCModeResponse(buffer, map); break;
						case 66: this->parseStepMotorResponse(buffer, map, timestamp); break;
						case 67: this->parseElectromechanicalForceAndStatusResponse(buffer, map); break;
						case 80: this->parseSetHPADCScanRateResponse(buffer, map); break;
						case 81: this->parseSetHPADCChannelOffsetAndSensitivityResponse(buffer, map); break;
						case 107: this->parseHPResetResponse(buffer, map); break;
						case 110: this->parseReadHPCalibrationResponse(buffer, map); break;
						case 145:
						case 146:
						case 193:
						case 194:
						case 195:
						case 208:
						case 209:
						case 235:
						case 238: this->parseHPErrorResponse(buffer, timestamp, map.ActuatorId); break;
						default:
							Log.Warn("ILCResponseParser: Unknown HP function %d on subnet %d", (int)function, subnet);
							BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownFunction, true);
							break;
						}
						break;
					case ILCTypes::HM:
						this->hmExpectedResponses[dataIndex]--;
						switch(function) {
						case 17: this->parseReportHMServerIDResponse(buffer, map); break;
						case 18: this->parseReportHMServerStatusResponse(buffer, map); break;
						case 65: this->parseChangeHMILCModeResponse(buffer, map); break;
						case 107: this->parseHMResetResponse(buffer, map); break;
						case 119: this->parseReadHMPressureValuesResponse(buffer, map); break;
						case 120: this->parseReportHMMezzanineIDResponse(buffer, map); break;
						case 121: this->parseReportHMMezzanineStatusResponse(buffer, map); break;
						case 122: this->parseReportLVDTResponse(buffer, map); break;
						case 145:
						case 146:
						case 193:
						case 235:
						case 247:
						case 248:
						case 249:
						case 250: this->parseHMErrorResponse(buffer, timestamp, map.ActuatorId); break;
						default:
							Log.Warn("ILCResponseParser: Unknown HM function %d on subnet %d", (int)function, subnet);
							BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownFunction, true);
							break;
						}
						break;
					default:
						Log.Warn("ILCResponseParser: Unknown address %d on subnet %d for function code %d", (int)address, (int)subnet, (int)function);
						BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownAddress, true);
						break;
					}
				}
				else {
					Log.Warn("ILCResponseParser: Unknown subnet %d", subnet);
					BitHelper::set(&this->modbusWarning->modbusSystemFlags, ModbusSystemFlags::UnknownSubnet, true);
				}
			}
		}
		else {
			Log.Warn("ILCResponseParser: Invalid CRC on subnet %d", subnet);
			if (this->grabResponse) {
				this->parseOneOffCommand(buffer, length, timestamp, false);
			}
			else {
				BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::InvalidCRC, true);
			}
		}
	}
}

void ILCResponseParser::incExpectedResponses(int32_t* fa, int32_t* hp, int32_t* hm) {
	for(int i = 0; i < FA_COUNT; i++) {
		this->faExpectedResponses[i] += fa[i];
	}
	for(int i = 0; i < HP_COUNT; i++) {
		this->hpExpectedResponses[i] += hp[i];
	}
	for(int i = 0; i < HM_COUNT; ++i) {
		this->hmExpectedResponses[i] += hm[i];
	}
}

void ILCResponseParser::clearResponses() {
	for(int i = 0; i < FA_COUNT; i++) {
		this->faExpectedResponses[i] = 0;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		this->hpExpectedResponses[i] = 0;
	}
	for(int i = 0; i < HM_COUNT; ++i) {
		this->hmExpectedResponses[i] = 0;
	}
}

void ILCResponseParser::verifyResponses() {
	bool warn = false;
	bool anyTimeout = false;
	for(int i = 0; i < FA_COUNT; i++) {
		if (this->faExpectedResponses[i] != 0) {
			warn = true;
			BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + i, ForceActuatorFlags::ResponseTimeout, true);
			this->faExpectedResponses[i] = 0;
		}
	}
	if (warn) {
		anyTimeout = true;
		Log.Warn("ILCResponseParser: Force actuator response timeout");
	}
	warn = false;
	for(int i = 0; i < HP_COUNT; i++) {
		if (this->hpExpectedResponses[i] != 0) {
			warn = true;
			BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + i, HardpointActuatorFlags::ResponseTimeout, true);
			this->hpExpectedResponses[i] = 0;
		}
	}
	if (warn) {
		anyTimeout = true;
		Log.Warn("ILCResponseParser: Hardpoint actuator response timeout");
	}
	warn = false;
	for(int i = 0; i < HM_COUNT; ++i) {
		if (this->hmExpectedResponses[i] != 0) {
			warn = true;
			BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + i, HardpointMonitorFlags::ResponseTimeout, true);
			this->hmExpectedResponses[i] = 0;
		}
	}
	if (warn) {
		anyTimeout = true;
		Log.Warn("ILCResponseParser: Hardpoint monitor response timeout");
	}

	this->safetyController->ilcCommunicationTimeout(anyTimeout);
}

void ILCResponseParser::grabNextResponse() {
	this->grabResponse = true;
}

bool ILCResponseParser::validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp) {
	int32_t addressIndex = buffer->getIndex();
	while(!buffer->endOfFrame() && !buffer->endOfBuffer() && (buffer->readLength() & 0xF000) != 0xB000) { }
	int32_t crcIndex = buffer->getIndex() - 3;
	(*length) = crcIndex - addressIndex + 2; // + 11; // This was +11 however it didn't seem to be used. Reusing this length as something helpful.
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

void ILCResponseParser::parseOneOffCommand(ModbusBuffer* buffer, uint16_t length, double timestamp, bool valid) {
	this->grabResponse = false;
	this->modbusResponse->responseValid = valid;
	this->modbusResponse->address = buffer->readU8();
	this->modbusResponse->functionCode = buffer->readU8();
	this->modbusResponse->dataLength = length - 4;
	for(uint16_t i = 0; i < this->modbusResponse->dataLength; ++i) {
		this->modbusResponse->data[i] = buffer->readU8();
	}
	this->modbusResponse->crc = buffer->readCRC();
	buffer->skipToNextFrame();
	this->publisher->logModbusResponse();
}

void ILCResponseParser::parseHPErrorResponse(ModbusBuffer* buffer, int32_t subnet, int32_t dataIndex) {
	uint8_t exceptionCode = buffer->readU8();
	switch(exceptionCode) {
	case 1: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusIllegalFunction, true); break;
	case 2:	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusIllegalDataAddress, true); break;
	case 3: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusIllegalDataValue, true); break;
	case 4: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusSlaveDeviceFailure, true); break;
	case 5: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusAcknowledge, true); break;
	case 6: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusSlaveDeviceBusy, true); break;
	case 7: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusNegativeAcknowledge, true); break;
	case 8: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusMemoryParityError, true); break;
	case 10: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusGatewayPathUnavailable, true); break;
	case 11: BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ModbusGatewayTargetDeviceFailedToRespond, true); break;
	default:
		Log.Warn("ILCResponseParser: Hardpoint actuator %d received exception code %d", dataIndex, (int32_t)exceptionCode);
		BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownProblem, true);
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseFAErrorResponse(ModbusBuffer* buffer, int32_t subnet, int32_t dataIndex) {
	uint8_t exceptionCode = buffer->readU8();
	switch(exceptionCode) {
	case 1: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusIllegalFunction, true); break;
	case 2:	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusIllegalDataAddress, true); break;
	case 3: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusIllegalDataValue, true); break;
	case 4: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusSlaveDeviceFailure, true); break;
	case 5: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusAcknowledge, true); break;
	case 6: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusSlaveDeviceBusy, true); break;
	case 7: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusNegativeAcknowledge, true); break;
	case 8: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusMemoryParityError, true); break;
	case 10: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusGatewayPathUnavailable, true); break;
	case 11: BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ModbusGatewayTargetDeviceFailedToRespond, true); break;
	default:
		Log.Warn("ILCResponseParser: Force actuator %d received exception code %d", dataIndex, (int32_t)exceptionCode);
		BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownProblem, true);
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseHMErrorResponse(ModbusBuffer* buffer, int32_t subnet, int32_t dataIndex) {
	uint8_t exceptionCode = buffer->readU8();
	switch(exceptionCode) {
	case 1: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusIllegalFunction, true); break;
	case 2:	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusIllegalDataAddress, true); break;
	case 3: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusIllegalDataValue, true); break;
	case 4: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusSlaveDeviceFailure, true); break;
	case 5: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusAcknowledge, true); break;
	case 6: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusSlaveDeviceBusy, true); break;
	case 7: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusNegativeAcknowledge, true); break;
	case 8: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusMemoryParityError, true); break;
	case 10: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusGatewayPathUnavailable, true); break;
	case 11: BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ModbusGatewayTargetDeviceFailedToRespond, true); break;
	default:
		Log.Warn("ILCResponseParser: Hardpoint monitor %d received exception code %d", dataIndex, (int32_t)exceptionCode);
		BitHelper::set(this->modbusWarning->subnetFlags + (subnet - 1), ModbusSubnetFlags::UnknownProblem, true);
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHPServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t length = buffer->readU8();
	this->hardpointActuatorInfo->ilcUniqueId[dataIndex] = buffer->readU48();
	this->hardpointActuatorInfo->ilcApplicationType[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->networkNodeType[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->ilcSelectedOptions[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->networkNodeOptions[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->majorRevision[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->minorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t length = buffer->readU8();
	this->forceActuatorIdInfo->ilcUniqueId[dataIndex] = buffer->readU48();
	uint64_t ilcApplicationType = buffer->readU8();
	uint64_t networkNodeType = buffer->readU8();
	uint64_t ilcSelectedOptions = buffer->readU8();
	uint64_t networkNodeOptions = buffer->readU8();
	uint64_t majorRevision = buffer->readU8();
	uint64_t minorRevision = buffer->readU8();
	this->forceActuatorILCInfo->ilcStatus[dataIndex] =
			(0xFFFF000000000000 & this->forceActuatorILCInfo->ilcStatus[dataIndex]) |
			(ilcApplicationType << 0) |
			(networkNodeType << 8) |
			(ilcSelectedOptions << 16) |
			(networkNodeOptions << 24) |
			(majorRevision << 32) |
			(minorRevision << 40);
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t length = buffer->readU8();
	this->hardpointMonitorInfo->ilcUniqueId[dataIndex] = buffer->readU48();
	this->hardpointMonitorInfo->ilcApplicationType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->networkNodeType[dataIndex] = buffer->readU8();
	buffer->readU8(); // ILCSelectedOptions
	buffer->readU8(); // NetworkNodeOptions
	this->hardpointMonitorInfo->majorRevision[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->minorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHPServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointActuatorState->ilcState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCMajorFault, (ilcStatus & 0x0001) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCMinorFault, (ilcStatus & 0x0002) != 0);
	// 0x0004 is reserved
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCFaultOverride, (ilcStatus & 0x0008) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCMainCalibrationError, (ilcStatus & 0x0010) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCBackupCalibrationError, (ilcStatus & 0x0020) != 0);
	// 0x0040 is reserved
	// 0x0080 is reserved
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCLimitSwitch1Operated, (ilcStatus & 0x0100) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCLimitSwitch2Operated, (ilcStatus & 0x0200) != 0);
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA (FA only)
	// 0x4000 is DCA (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCUniqueIdCRCError, (ilcFaults & 0x0001) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCApplicationTypeMismatch, (ilcFaults & 0x0002) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCApplicationMissing, (ilcFaults & 0x0004) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCApplicationCRCMismatch, (ilcFaults & 0x0008) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCOneWireMissing, (ilcFaults & 0x0010) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCOneWire1Mismatch, (ilcFaults & 0x0020) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCOneWire2Mismatch, (ilcFaults & 0x0040) != 0);
	// 0x0080 is reserved
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCWatchdogReset, (ilcFaults & 0x0100) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCBrownOut, (ilcFaults & 0x0200) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCEventTrapReset, (ilcFaults & 0x0400) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCMotorDriverFault, (ilcFaults & 0x0800) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCSSRPowerFault, (ilcFaults & 0x1000) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCAuxPowerFault, (ilcFaults & 0x2000) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCSMCPowerFault, (ilcFaults & 0x4000) != 0);
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorState->ilcState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMajorFault, (ilcStatus & 0x0001) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMinorFault, (ilcStatus & 0x0002) != 0);
	// 0x0004 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCFaultOverride, (ilcStatus & 0x0008) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMainCalibrationError, (ilcStatus & 0x0010) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCBackupCalibrationError, (ilcStatus & 0x0020) != 0);
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineError, (ilcStatus & 0x2000) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineBootloaderActive, (ilcStatus & 0x4000) != 0);
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCUniqueIdCRCError, (ilcFaults & 0x0001) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCApplicationTypeMismatch, (ilcFaults & 0x0002) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCApplicationMissing, (ilcFaults & 0x0004) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCApplicationCRCMismatch, (ilcFaults & 0x0008) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCOneWireMissing, (ilcFaults & 0x0010) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCOneWire1Mismatch, (ilcFaults & 0x0020) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCOneWire2Mismatch, (ilcFaults & 0x0040) != 0);
	// 0x0080 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCWatchdogReset, (ilcFaults & 0x0100) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCBrownOut, (ilcFaults & 0x0200) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCEventTrapReset, (ilcFaults & 0x0400) != 0);
	// 0x0800 is Motor Driver (HP only)
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCSSRPowerFault, (ilcFaults & 0x1000) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCAuxPowerFault, (ilcFaults & 0x2000) != 0);
	// 0x4000 is SMC Power (HP only)
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorState->ilcState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMajorFault, (ilcStatus & 0x0001) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMinorFault, (ilcStatus & 0x0002) != 0);
	// 0x0004 is reserved
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCFaultOverride, (ilcStatus & 0x0008) != 0);
	// 0x0010 is main calibration error (not used by HM)
	// 0x0020 is backup calibration error (not used by HM)
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA fault (FA only)
	// 0x4000 is DCA firmware update (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCUniqueIdCRCError, (ilcFaults & 0x0001) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCApplicationTypeMismatch, (ilcFaults & 0x0002) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCApplicationMissing, (ilcFaults & 0x0004) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCApplicationCRCMismatch, (ilcFaults & 0x0008) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCOneWireMissing, (ilcFaults & 0x0010) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCOneWire1Mismatch, (ilcFaults & 0x0020) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCOneWire2Mismatch, (ilcFaults & 0x0040) != 0);
	// 0x0080 is reserved
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCWatchdogReset, (ilcFaults & 0x0100) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCBrownOut, (ilcFaults & 0x0200) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCEvenTrapReset, (ilcFaults & 0x0400) != 0);
	// 0x0800 is Motor Driver (HP only)
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCSSRPowerFault, (ilcFaults & 0x1000) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCAuxPowerFault, (ilcFaults & 0x2000) != 0);
	// 0x4000 is SMC Power (HP only)
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeHPILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointActuatorState->ilcState[dataIndex] = buffer->readU16();
	//buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeFAILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorState->ilcState[dataIndex] = buffer->readU16();
	//buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeHMILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorState->ilcState[dataIndex] = buffer->readU16();
	//buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseStepMotorResponse(ModbusBuffer* buffer, ILCMap map, double timestamp) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	this->hardpointActuatorData->timestamp = timestamp;
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCFault, (status & 0x01) != 0);
	// 0x02 is reserved
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCLimitSwitch1Operated, (status & 0x04) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCLimitSwitch2Operated, (status & 0x08) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::BroadcastCounterMismatch, this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4));
	this->hardpointActuatorData->encoder[dataIndex] = buffer->readI32();
	this->hardpointActuatorData->measuredForce[dataIndex] = buffer->readSGL();
	this->hardpointActuatorData->displacement[dataIndex] = (this->hardpointActuatorData->encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCFault, (status & 0x01) != 0);
	// 0x02 is reserved
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCLimitSwitch1Operated, (status & 0x04) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::ILCLimitSwitch2Operated, (status & 0x08) != 0);
	BitHelper::set(this->hardpointActuatorWarning->hardpointActuatorFlags + dataIndex, HardpointActuatorFlags::BroadcastCounterMismatch, this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4));
	int32_t offset = 0;
	switch(map.ActuatorId) {
	case 1: offset = this->hardpointActuatorSettings->HP1EncoderOffset; break;
	case 2: offset = this->hardpointActuatorSettings->HP2EncoderOffset; break;
	case 3: offset = this->hardpointActuatorSettings->HP3EncoderOffset; break;
	case 4: offset = this->hardpointActuatorSettings->HP4EncoderOffset; break;
	case 5: offset = this->hardpointActuatorSettings->HP5EncoderOffset; break;
	case 6: offset = this->hardpointActuatorSettings->HP6EncoderOffset; break;
	}
	// Encoder value needs to be swapped to keep with the theme of extension is positive
	// retaction is negative
	this->hardpointActuatorData->encoder[dataIndex] = -buffer->readI32() + offset;
	// Unlike the pneumatic, the electromechanical doesn't reverse compression and tension so we swap it here
	this->hardpointActuatorData->measuredForce[dataIndex] = -buffer->readSGL();
	this->hardpointActuatorData->displacement[dataIndex] = (this->hardpointActuatorData->encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t sDataIndex = map.SecondaryDataIndex;
	this->forceActuatorMezzanineCalibrationInfo->primaryCylinderGain[dataIndex] = buffer->readSGL();
	if (sDataIndex != -1) {
		this->forceActuatorMezzanineCalibrationInfo->secondaryCylinderGain[sDataIndex] = buffer->readSGL();
	}
	else {
		buffer->readSGL();
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map) {
	if (address <= 16) {
		this->parseSingleAxisForceDemandResponse(buffer, map);
	}
	else {
		this->parseDualAxisForceDemandResponse(buffer, map);
	}
	this->checkForceActuatorMeasuredForce(map);
	this->checkForceActuatorFollowingError(map);
}

void ILCResponseParser::parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCFault, (status & 0x01) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineError, (status & 0x02) != 0);
	// 0x04 is reserved
	// 0x08 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::BroadcastCounterMismatch, this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4));
	this->forceActuatorData->primaryCylinderForces[dataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	ForceConverter::saaToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], 0.0, &x, &y, &z);
	this->forceActuatorData->zForces[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseDualAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;
	int xIndex = map.XDataIndex;
	int yIndex = map.YDataIndex;
	uint8_t status = buffer->readU8();
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCFault, (status & 0x01) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineError, (status & 0x02) != 0);
	// 0x04 is reserved
	// 0x08 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::BroadcastCounterMismatch, this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4));
	this->forceActuatorData->primaryCylinderForces[dataIndex] = buffer->readSGL();
	this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	switch(this->forceActuatorPositionInfo->actuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	}
	if (xIndex != -1) {
		this->forceActuatorData->xForces[xIndex] = x;
	}
	if (yIndex != -1) {
		this->forceActuatorData->yForces[yIndex] = y;
	}
	this->forceActuatorData->zForces[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map) {
	if (address <= 16) {
		this->parseSingleAxisPneumaticForceStatusResponse(buffer, map);
	}
	else {
		this->parseDualAxisPneumaticForceStatusResponse(buffer, map);
	}
	this->checkForceActuatorMeasuredForce(map);
	this->checkForceActuatorFollowingError(map);
}

void ILCResponseParser::parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCFault, (status & 0x01) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineError, (status & 0x02) != 0);
	// 0x04 is reserved
	// 0x08 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::BroadcastCounterMismatch, this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4));
	this->forceActuatorData->primaryCylinderForces[dataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	ForceConverter::saaToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], 0.0, &x, &y, &z);
	this->forceActuatorData->zForces[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;
	int xIndex = map.XDataIndex;
	int yIndex = map.YDataIndex;
	uint8_t status = buffer->readU8();
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCFault, (status & 0x01) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineError, (status & 0x02) != 0);
	// 0x04 is reserved
	// 0x08 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::BroadcastCounterMismatch, this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4));
	this->forceActuatorData->primaryCylinderForces[dataIndex] = buffer->readSGL();
	this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	switch(this->forceActuatorPositionInfo->actuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceActuatorData->primaryCylinderForces[dataIndex], this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex], &x, &y, &z); break;
	}
	if (xIndex != -1) {
		this->forceActuatorData->xForces[xIndex] = x;
	}
	if (yIndex != -1) {
		this->forceActuatorData->yForces[yIndex] = y;
	}
	this->forceActuatorData->zForces[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetHPADCScanRateResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointActuatorInfo->adcScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetFAADCScanRateResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint64_t adcScanRate = buffer->readU8();
	this->forceActuatorILCInfo->ilcStatus[dataIndex] =
				(0x0000FFFFFFFFFFFF & this->forceActuatorILCInfo->ilcStatus[dataIndex]) |
				(adcScanRate << 48);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseHPResetResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseFAResetResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseHMResetResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadHPCalibrationResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	buffer->readSGL(); // Main Coefficient K1
	buffer->readSGL(); // Main Coefficient K2
	this->hardpointActuatorInfo->mainLoadCellCoefficient[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Coefficient K4
	this->hardpointActuatorInfo->mainLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Offset Channel 2
	buffer->readSGL(); // Main Offset Channel 3
	buffer->readSGL(); // Main Offset Channel 4
	this->hardpointActuatorInfo->mainLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Sensitivity Channel 2
	buffer->readSGL(); // Main Sensitivity Channel 3
	buffer->readSGL(); // Main Sensitivity Channel 4
	buffer->readSGL(); // Backup Coefficient K1
	buffer->readSGL(); // Backup Coefficient K2
	this->hardpointActuatorInfo->backupLoadCellCoefficient[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Coefficient K4
	this->hardpointActuatorInfo->backupLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Offset Channel 2
	buffer->readSGL(); // Backup Offset Channel 3
	buffer->readSGL(); // Backup Offset Channel 4
	this->hardpointActuatorInfo->backupLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Sensitivity Channel 2
	buffer->readSGL(); // Backup Sensitivity Channel 3
	buffer->readSGL(); // Backup Sensitivity Channel 4
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadFACalibrationResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorMainCalibrationInfo->primaryCoefficient[dataIndex] = buffer->readSGL();
	this->forceActuatorMainCalibrationInfo->secondaryCoefficient[dataIndex] = this->forceActuatorMainCalibrationInfo->primaryCoefficient[dataIndex];
	buffer->readSGL(); // Main Coefficient K2
	buffer->readSGL(); // Main Coefficient K3
	buffer->readSGL(); // Main Coefficient K4
	this->forceActuatorMainCalibrationInfo->primaryOffset[dataIndex] = buffer->readSGL();
	this->forceActuatorMainCalibrationInfo->secondaryOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Offset Channel 3
	buffer->readSGL(); // Main Offset Channel 4
	this->forceActuatorMainCalibrationInfo->primarySensitivity[dataIndex] = buffer->readSGL();
	this->forceActuatorMainCalibrationInfo->secondarySensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Sensitivity Channel 3
	buffer->readSGL(); // Main Sensitivity Channel 4
	this->forceActuatorBackupCalibrationInfo->primaryCoefficient[dataIndex] = buffer->readSGL();
	this->forceActuatorBackupCalibrationInfo->secondaryCoefficient[dataIndex] = this->forceActuatorBackupCalibrationInfo->primaryCoefficient[dataIndex];
	buffer->readSGL(); // Backup Coefficient K2
	buffer->readSGL(); // Backup Coefficient K3
	buffer->readSGL(); // Backup Coefficient K4
	this->forceActuatorBackupCalibrationInfo->primaryOffset[dataIndex] = buffer->readSGL();
	this->forceActuatorBackupCalibrationInfo->secondaryOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Offset Channel 3
	buffer->readSGL(); // Backup Offset Channel 4
	this->forceActuatorBackupCalibrationInfo->primarySensitivity[dataIndex] = buffer->readSGL();
	this->forceActuatorBackupCalibrationInfo->secondarySensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Sensitivity Channel 3
	buffer->readSGL(); // Backup Sensitivity Channel 4
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadHMPressureValuesResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorData->pressureSensor1[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->pressureSensor2[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->pressureSensor3[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->breakawayPressure[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportDCAIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorIdInfo->mezzanineUniqueId[dataIndex] = buffer->readU48();
	uint64_t mezzanineFirmwareType = buffer->readU8();
	uint64_t mezzanineMajorRevision = buffer->readU8();
	uint64_t mezzanineMinorRevision = buffer->readU8();
	this->forceActuatorILCInfo->mezzanineStatus[dataIndex] =
			(mezzanineFirmwareType << 0) |
			(mezzanineMajorRevision << 8) |
			(mezzanineMinorRevision << 16);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorInfo->mezzanineUniqueId[dataIndex] = buffer->readU48();
	this->hardpointMonitorInfo->mezzanineFirmwareType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->mezzanineMajorRevision[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->mezzanineMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportDCAStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint16_t status = buffer->readU16();
	// 0x0001 is DCA Outputs Enabled (wont report)
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzaninePowerFault, (status & 0x0002) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineCurrentAmp1Fault, (status & 0x0004) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineCurrentAmp2Fault, (status & 0x0008) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineUniqueIdCRCError, (status & 0x0010) != 0);
	// 0x0020 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineMainCalibrationError, (status & 0x0040) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineBackupCalibrationError, (status & 0x0080) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineEventTrapReset, (status & 0x0100) != 0);
	// 0x0200 is reserved
	// 0x0400 is reserved
	// 0x0800 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineApplicationMissing, (status & 0x1000) != 0);
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineApplicationCRCMismatch, (status & 0x2000) != 0);
	// 0x4000 is reserved
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::ILCMezzanineBootloaderActive, (status & 0x8000) != 0);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint16_t status = buffer->readU16();
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineS1AInterface1Fault, (status & 0x0001) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineS1ALVDT1Fault, (status & 0x0002) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineS1AInterface2Fault, (status & 0x0004) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineS1ALVDT2Fault, (status & 0x0008) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineUniqueIdCRCError, (status & 0x0010) != 0);
	// 0x0020 is reserved
	// 0x0040 is reserved
	// 0x0080 is reserved
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineEvenTrapReset, (status & 0x0100) != 0);
	// 0x0200 is reserved
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineDCPRS422ChipFault, (status & 0x0400) != 0);
	// 0x0800 is reserved
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineApplicationMissing, (status & 0x1000) != 0);
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineApplicationCRCMismatch, (status & 0x2000) != 0);
	// 0x4000 is reserved
	BitHelper::set(this->hardpointMonitorWarning->hardpointMonitorFlags + dataIndex, HardpointMonitorFlags::ILCMezzanineBootloaderActive, (status & 0x8000) != 0);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportLVDTResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorData->breakawayLVDT[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->displacementLVDT[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::checkForceActuatorMeasuredForce(ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;
	float primaryForce = this->forceActuatorData->primaryCylinderForces[dataIndex];
	float primaryLowLimit = this->forceActuatorSettings->MeasuredPrimaryCylinderLimitTable[dataIndex].LowFault;
	float primaryHighLimit = this->forceActuatorSettings->MeasuredPrimaryCylinderLimitTable[dataIndex].HighFault;
	bool primaryLimitWarning = primaryForce < primaryLowLimit || primaryForce > primaryHighLimit;
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::MeasuredForcePrimaryAxisForceWarning, primaryLimitWarning);

	if (secondaryDataIndex != -1) {
		float secondaryForce = this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex];
		float secondaryLowLimit = this->forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].LowFault;
		float secondaryHighLimit = this->forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
		bool secondaryLimitWarning = secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLimit;
		BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::MeasuredForceSecondaryAxisForceWarning, secondaryLimitWarning);
	}
}

void ILCResponseParser::checkForceActuatorFollowingError(ILCMap map) {
	// TODO: UPDATE
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;

	float primaryForce = this->forceActuatorData->primaryCylinderForces[dataIndex];
	float primarySetpoint = this->appliedCylinderForces->primaryCylinderForces[dataIndex] / 1000.0;
	float primaryLimit = this->forceActuatorSettings->FollowingErrorPrimaryCylinderLimitTable[dataIndex].HighFault;
	float primaryFollowingError = primaryForce - primarySetpoint;
	bool primaryLimitWarning = std::abs(primaryFollowingError) > primaryLimit;
	BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::PrimaryAxisFollowingErrorWarning, primaryLimitWarning);

	if (secondaryDataIndex != -1) {
		float secondaryForce = this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex];
		float secondarySetpoint = this->appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex] / 1000.0;
		float secondaryLimit = this->forceActuatorSettings->FollowingErrorSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
		float secondaryFollowingError = secondaryForce - secondarySetpoint;
		bool secondaryLimitWarning = std::abs(secondaryFollowingError) > secondaryLimit;
		BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + dataIndex, ForceActuatorFlags::SecondaryAxisFollowingErrorWarning, secondaryLimitWarning);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
