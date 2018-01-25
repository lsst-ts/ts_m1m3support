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
#include <Timestamp.h>
#include <ILCSubnetData.h>
#include <SAL_m1m3C.h>
#include <cmath>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

ILCResponseParser::ILCResponseParser() {
	this->forceActuatorSettings = 0;
	this->hardpointActuatorSettings = 0;
	this->publisher = 0;
	this->subnetData = 0;
	this->hardpointInfo = 0;
	this->hardpointStatus = 0;
	this->hardpointData = 0;
	this->forceInfo = 0;
	this->forceWarning = 0;
	this->forceStatus = 0;
	this->forceData = 0;
	this->hardpointMonitorInfo = 0;
	this->hardpointMonitorStatus = 0;
	this->ilcWarning = 0;
}

ILCResponseParser::ILCResponseParser(ForceActuatorSettings* forceActuatorSettings, HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher, ILCSubnetData* subnetData) {
	this->forceActuatorSettings = forceActuatorSettings;
	this->hardpointActuatorSettings = hardpointActuatorSettings;
	this->publisher = publisher;
	this->subnetData = subnetData;
	this->hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
	this->hardpointStatus = this->publisher->getHardpointStatus();
	this->hardpointData = this->publisher->getHardpointData();
	this->forceInfo = this->publisher->getEventForceActuatorInfo();
	this->forceWarning = this->publisher->getEventForceActuatorForceWarning();
	this->forceStatus = this->publisher->getForceActuatorStatus();
	this->forceData = this->publisher->getForceActuatorData();
	this->hardpointMonitorInfo = this->publisher->getEventHardpointMonitorInfo();
	this->hardpointMonitorStatus = this->publisher->getHardpointMonitorStatus();
	this->ilcWarning = this->publisher->getEventILCWarning();

	this->forceWarning->Timestamp = 0;
	this->forceWarning->AnyWarning = false;
	this->forceWarning->AnyPrimaryAxisMeasuredForceWarning= false;
	this->forceWarning->AnySecondaryAxisMeasuredForceWarning = false;
	this->forceWarning->AnyPrimaryAxisFollowingErrorWarning = false;
	this->forceWarning->AnySecondaryAxisFollowingErrorWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceWarning->PrimaryAxisMeasuredForceWarning[i] = false;
		this->forceWarning->SecondaryAxisMeasuredForceWarning[i] = false;
		this->forceWarning->PrimaryAxisFollowingErrorWarning[i] = false;
		this->forceWarning->SecondaryAxisFollowingErrorWarning[i] = false;
	}

	this->ilcWarning->Timestamp = 0;
	this->ilcWarning->ActuatorId = -1;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
}

void ILCResponseParser::parse(ModbusBuffer* buffer, uint8_t subnet) {
	uint64_t a = buffer->readLength();
	uint64_t b = buffer->readLength();
	uint64_t c = buffer->readLength();
	uint64_t d = buffer->readLength();
	double globalTimestamp = Timestamp::fromRaw((a << 48) | (b << 32) | (c << 16) | d);
	this->forceStatus->Timestamp = globalTimestamp;
	this->forceData->Timestamp = globalTimestamp;
	this->hardpointStatus->Timestamp = globalTimestamp;
	this->hardpointData->Timestamp = globalTimestamp;
	this->hardpointMonitorStatus->Timestamp = globalTimestamp;
	while(!buffer->endOfBuffer()) {
		uint16_t length = 0;
		double timestamp = 0;
		if (this->validateCRC(buffer, &length, &timestamp)) {
			if(subnet >= 1 && subnet <= 5) {
				uint8_t address = buffer->readU8();
				uint8_t function = buffer->readU8();
				ILCAddressDataMap map = this->subnetData->getILCDataFromAddress(subnet - 1, address);
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
				case ILCTypes::HM:
					this->hmExpectedResponses[dataIndex]--;
					switch(function) {
					case 17: this->parseReportHMServerIDResponse(buffer, dataIndex); break;
					case 18: this->parseReportHMServerStatusResponse(buffer, dataIndex); break;
					case 65: this->parseChangeHMILCModeResponse(buffer, dataIndex); break;
					case 107: this->parseHMResetResponse(buffer, dataIndex); break;
					case 119: this->parseReadHMPressureValuesResponse(buffer, dataIndex); break;
					case 120: this->parseReportHMMezzanineIDResponse(buffer, dataIndex); break;
					case 121: this->parseReportHMMezzanineStatusResponse(buffer, dataIndex); break;
					case 122: this->parseReportLVDTResponse(buffer, dataIndex); break;
					case 145:
					case 146:
					case 193:
					case 208:
					case 209:
					case 235:
					case 238:
					case 250: this->parseErrorResponse(buffer, timestamp, map.ActuatorId); break;
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

void ILCResponseParser::verifyResponses() {
	double timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < FA_COUNT; i++) {
		if (this->faExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->forceInfo->ReferenceId[i]);
			this->faExpectedResponses[i] = 0;
		}
	}
	for(int i = 0; i < HP_COUNT; i++) {
		if (this->hpExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->hardpointInfo->ReferenceId[i]);
			this->hpExpectedResponses[i] = 0;
		}
	}
	for(int i = 0; i < HM_COUNT; ++i) {
		if (this->hmExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->hardpointMonitorInfo->ReferenceId[i]);
			this->hmExpectedResponses[i] = 0;
		}
	}
}

bool ILCResponseParser::validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp) {
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

void ILCResponseParser::parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId) {
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

void ILCResponseParser::parseReportHPServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->hardpointInfo->ILCUniqueId[dataIndex] = buffer->readU48();
	this->hardpointInfo->ILCApplicationType[dataIndex] = buffer->readU8();
	this->hardpointInfo->NetworkNodeType[dataIndex] = buffer->readU8();
	this->hardpointInfo->ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->hardpointInfo->NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->hardpointInfo->MajorRevision[dataIndex] = buffer->readU8();
	this->hardpointInfo->MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->forceInfo->ILCUniqueId[dataIndex] = buffer->readU48();
	this->forceInfo->ILCApplicationType[dataIndex] = buffer->readU8();
	this->forceInfo->NetworkNodeType[dataIndex] = buffer->readU8();
	this->forceInfo->ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->forceInfo->NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->forceInfo->MajorRevision[dataIndex] = buffer->readU8();
	this->forceInfo->MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->hardpointMonitorInfo->ILCUniqueId[dataIndex] = buffer->readU48();
	this->hardpointMonitorInfo->ILCApplicationType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->NetworkNodeType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MajorRevision[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHPServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointStatus->Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointStatus->MajorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->hardpointStatus->MinorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->hardpointStatus->FaultOverridden[dataIndex] = (ilcStatus & 0x0008) != 0;
	this->hardpointInfo->MainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
	this->hardpointInfo->BackupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointData->CWLimitOperated[dataIndex] = (ilcStatus & 0x0100) != 0;
	this->hardpointData->CCWLimitOperated[dataIndex] = (ilcStatus & 0x0200) != 0;
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA (FA only)
	// 0x4000 is DCA (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->hardpointInfo->UniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->hardpointInfo->ApplicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->hardpointInfo->ApplicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->hardpointInfo->ApplicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->hardpointInfo->OneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->hardpointInfo->OneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->hardpointInfo->OneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->hardpointStatus->WatchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->hardpointStatus->BrownoutDetected[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->hardpointStatus->EventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	this->hardpointStatus->MotorPowerFault[dataIndex] = (ilcFaults & 0x0800) != 0;
	this->hardpointStatus->SSRPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->hardpointStatus->AUXPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	this->hardpointStatus->SMCPowerFault[dataIndex] = (ilcFaults & 0x4000) != 0;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceStatus->Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->forceStatus->MajorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->forceStatus->MinorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->forceStatus->FaultOverridden[dataIndex] = (ilcStatus & 0x0008) != 0;
	this->forceInfo->MainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
	this->forceInfo->BackupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	this->forceStatus->DCAFault[dataIndex] = (ilcStatus & 0x2000) != 0;
	this->forceInfo->DCAFirmwareUpdate[dataIndex] = (ilcStatus & 0x4000) != 0;
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->forceInfo->UniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->forceInfo->ApplicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->forceInfo->ApplicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->forceInfo->ApplicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->forceInfo->OneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->forceInfo->OneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->forceInfo->OneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->forceStatus->WatchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->forceStatus->BrownoutDetected[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->forceStatus->EventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	this->forceStatus->MotorPowerFault[dataIndex] = (ilcFaults & 0x0800) != 0;
	this->forceStatus->SSRPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->forceStatus->AUXPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	this->forceStatus->SMCPowerFault[dataIndex] = (ilcFaults & 0x4000) != 0;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointMonitorStatus->Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointMonitorStatus->MajorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->hardpointMonitorStatus->MinorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->hardpointMonitorStatus->FaultOverridden[dataIndex] = (ilcStatus & 0x0008) != 0;
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
	this->hardpointMonitorInfo->UniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->hardpointMonitorInfo->ApplicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->hardpointMonitorInfo->ApplicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->hardpointMonitorInfo->ApplicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->hardpointMonitorInfo->OneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->hardpointMonitorInfo->OneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->hardpointMonitorInfo->OneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->hardpointMonitorStatus->WatchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->hardpointMonitorStatus->BrownoutDetected[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->hardpointMonitorStatus->EventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	this->hardpointMonitorStatus->MotorPowerFault[dataIndex] = (ilcFaults & 0x0800) != 0;
	this->hardpointMonitorStatus->SSRPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->hardpointMonitorStatus->AUXPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	this->hardpointMonitorStatus->SMCPowerFault[dataIndex] = (ilcFaults & 0x4000) != 0;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeHPILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointStatus->Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeFAILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceStatus->Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeHMILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointMonitorStatus->Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseStepMotorResponse(ModbusBuffer* buffer, int32_t dataIndex, double timestamp) {
	uint8_t status = buffer->readU8();
	this->hardpointData->Timestamp = timestamp;
	this->hardpointData->ILCFault[dataIndex] = (status & 0x01) != 0;
	// 0x02 is reserved
	this->hardpointData->CWLimitOperated[dataIndex] = (status & 0x04) != 0;
	this->hardpointData->CCWLimitOperated[dataIndex] = (status & 0x08) != 0;
	this->hardpointData->BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->hardpointData->Encoder[dataIndex] = buffer->readI32();
	this->hardpointData->Force[dataIndex] = buffer->readSGL();
	this->hardpointData->Displacement[dataIndex] = (this->hardpointData->Encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
	this->calculateHPPostion();
}

void ILCResponseParser::parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->hardpointData->ILCFault[dataIndex] = (status & 0x01) != 0;
	// 0x02 is reserved
	this->hardpointData->CWLimitOperated[dataIndex] = (status & 0x04) != 0;
	this->hardpointData->CCWLimitOperated[dataIndex] = (status & 0x08) != 0;
	this->hardpointData->BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->hardpointData->Encoder[dataIndex] = buffer->readI32();
	this->hardpointData->Force[dataIndex] = buffer->readSGL();
	this->hardpointData->Displacement[dataIndex] = (this->hardpointData->Encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
	this->calculateHPPostion();
}

void ILCResponseParser::parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo->DCAPrimaryCylinderGain[dataIndex] = buffer->readSGL();
	this->forceInfo->DCASecondaryCylinderGain[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex) {
	if (address <= 16) {
		this->parseSingleAxisForceDemandResponse(buffer, dataIndex);
	}
	else {
		this->parseDualAxisForceDemandResponse(buffer, dataIndex);
	}
	this->checkForceActuatorMeasuredForce(dataIndex);
	this->checkForceActuatorFollowingError(dataIndex);
}

void ILCResponseParser::parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceData->DCAFault[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData->BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceData->SecondaryCylinderForce[dataIndex] = 0;
	ForceConverter::saaToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseDualAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceData->DCAFault[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData->BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceData->SecondaryCylinderForce[dataIndex] = buffer->readSGL();
	switch(this->forceInfo->ActuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex) {
	if (address <= 16) {
		this->parseSingleAxisPneumaticForceStatusResponse(buffer, dataIndex);
	}
	else {
		this->parseDualAxisPneumaticForceStatusResponse(buffer, dataIndex);
	}
	this->checkForceActuatorMeasuredForce(dataIndex);
	this->checkForceActuatorFollowingError(dataIndex);
}

void ILCResponseParser::parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceData->DCAFault[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData->BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	ForceConverter::saaToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceData->DCAFault[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData->BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceData->SecondaryCylinderForce[dataIndex] = buffer->readSGL();
	switch(this->forceInfo->ActuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceData->PrimaryCylinderForce[dataIndex], this->forceData->SecondaryCylinderForce[dataIndex], this->forceData->XForce + dataIndex, this->forceData->YForce + dataIndex, this->forceData->ZForce + dataIndex); break;
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetHPADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointInfo->ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetFAADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo->ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseHPResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseFAResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseHMResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadHPCalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointInfo->MainADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->hardpointInfo->MainADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->hardpointInfo->MainADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->hardpointInfo->MainADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->hardpointInfo->MainLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo->MainLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo->BackupADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->hardpointInfo->BackupADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->hardpointInfo->BackupADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->hardpointInfo->BackupADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->hardpointInfo->BackupLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo->BackupLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadFACalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo->MainADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->forceInfo->MainADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->forceInfo->MainADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->forceInfo->MainADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->forceInfo->MainPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceInfo->MainSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo->MainPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceInfo->MainSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo->BackupADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->forceInfo->BackupADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->forceInfo->BackupADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->forceInfo->BackupADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->forceInfo->BackupPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceInfo->BackupSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo->BackupPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceInfo->BackupSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadHMPressureValuesResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointData->BreakawayPressure[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportDCAIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo->DCAUniqueId[dataIndex] = buffer->readU48();
	this->forceInfo->DCAFirmwareType[dataIndex] = buffer->readU8();
	this->forceInfo->DCAMajorRevision[dataIndex] = buffer->readU8();
	this->forceInfo->DCAMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointMonitorInfo->MezzanineUniqueId[dataIndex] = buffer->readU48();
	this->hardpointMonitorInfo->MezzanineFirmwareType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MezzanineMajorRevision[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MezzanineMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportDCAStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint16_t status = buffer->readU16();
	this->forceStatus->DCAOutputsEnabled[dataIndex] = (status & 0x0001) != 0;
	this->forceStatus->DCAPowerFault[dataIndex] = (status & 0x0002) != 0;
	this->forceStatus->DCAAmplifierAFault[dataIndex] = (status & 0x0004) != 0;
	this->forceStatus->DCAAmplifierBFault[dataIndex] = (status & 0x0008) != 0;
	this->forceInfo->DCAUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
	// 0x0020 is reserved
	this->forceInfo->DCAMainCalibrationError[dataIndex] = (status & 0x0040) != 0;
	this->forceInfo->DCABackupCalibrationError[dataIndex] = (status & 0x0080) != 0;
	this->forceStatus->DCAEventTrapReset[dataIndex] = (status & 0x0100) != 0;
	// 0x0200 is reserved
	// 0x0400 is reserved
	// 0x0800 is reserved
	this->forceInfo->DCAApplicationMissing[dataIndex] = (status & 0x1000) != 0;
	this->forceInfo->DCAApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
	// 0x4000 is reserved
	this->forceInfo->DCABootloaderActive[dataIndex] = (status & 0x8000) != 0;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint16_t status = buffer->readU16();
	this->hardpointMonitorStatus->MezzanineS1A1InterfaceFault[dataIndex] = (status & 0x0001) != 0;
	this->hardpointMonitorStatus->MezzanineS1A1LVDTFault[dataIndex] = (status & 0x0002) != 0;
	this->hardpointMonitorStatus->MezzanineS1A2InterfaceFault[dataIndex] = (status & 0x0004) != 0;
	this->hardpointMonitorStatus->MezzanineS1A2LVDTFault[dataIndex] = (status & 0x0008) != 0;
	this->hardpointMonitorInfo->MezzanineUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
	// 0x0020 is reserved
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointMonitorStatus->MezzanineEventTrapReset[dataIndex] = (status & 0x0100) != 0;
	// 0x0200 is reserved
	this->hardpointMonitorInfo->MezzanineDCPRS422ChipFault[dataIndex] = (status & 0x0400) != 0;
	// 0x0800 is reserved
	this->hardpointMonitorInfo->MezzanineApplicationMissing[dataIndex] = (status & 0x1000) != 0;
	this->hardpointMonitorInfo->MezzanineApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
	// 0x4000 is reserved
	this->hardpointMonitorInfo->MezzanineBootloaderActive[dataIndex] = (status & 0x8000) != 0;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportLVDTResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointData->BreakawayLVDT[dataIndex] = buffer->readSGL();
	this->hardpointData->DisplacementLVDT[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::calculateHPPostion() {
	this->hardpointData->XPosition =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[0] * this->hardpointData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[1] * this->hardpointData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[2] * this->hardpointData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[3] * this->hardpointData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[4] * this->hardpointData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[5] * this->hardpointData->Displacement[1];
	this->hardpointData->YPosition =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[6] * this->hardpointData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[7] * this->hardpointData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[8] * this->hardpointData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[9] * this->hardpointData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[10] * this->hardpointData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[11] * this->hardpointData->Displacement[1];
	this->hardpointData->ZPosition =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[12] * this->hardpointData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[13] * this->hardpointData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[14] * this->hardpointData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[15] * this->hardpointData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[16] * this->hardpointData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[17] * this->hardpointData->Displacement[1];
	this->hardpointData->XRotation =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[18] * this->hardpointData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[19] * this->hardpointData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[20] * this->hardpointData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[21] * this->hardpointData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[22] * this->hardpointData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[23] * this->hardpointData->Displacement[1];
	this->hardpointData->YRotation =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[24] * this->hardpointData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[25] * this->hardpointData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[26] * this->hardpointData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[27] * this->hardpointData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[28] * this->hardpointData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[29] * this->hardpointData->Displacement[1];
	this->hardpointData->ZRotation =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[30] * this->hardpointData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[31] * this->hardpointData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[32] * this->hardpointData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[33] * this->hardpointData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[34] * this->hardpointData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[35] * this->hardpointData->Displacement[1];
}

void ILCResponseParser::checkForceActuatorMeasuredForce(int32_t dataIndex) {
	float primaryForce = this->forceData->PrimaryCylinderForce[dataIndex];
	float secondaryForce = this->forceData->SecondaryCylinderForce[dataIndex];
	double primaryLowLimit = this->forceActuatorSettings->Limits[dataIndex].MeasuredPrimaryAxisLowLimit;
	double primaryHighLimit = this->forceActuatorSettings->Limits[dataIndex].MeasuredPrimaryAxisHighLimit;
	double secondaryLowLimit = this->forceActuatorSettings->Limits[dataIndex].MeasuredSecondaryAxisLowLimit;
	double secondaryHighLimit = this->forceActuatorSettings->Limits[dataIndex].MeasuredSecondaryAxisHighLimit;
	bool primaryLimitWarning = primaryForce < primaryLowLimit || primaryForce > primaryHighLimit;
	bool secondaryLimitWarning = secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLimit;
	bool previousPrimaryLimit = this->forceWarning->PrimaryAxisMeasuredForceWarning[dataIndex];
	bool previousSecondaryLimit = this->forceWarning->SecondaryAxisMeasuredForceWarning[dataIndex];
	bool anyChange = primaryLimitWarning != previousPrimaryLimit || secondaryLimitWarning !=  previousSecondaryLimit;
	this->forceWarning->PrimaryAxisMeasuredForceWarning[dataIndex] = primaryLimitWarning;
	this->forceWarning->SecondaryAxisMeasuredForceWarning[dataIndex] = secondaryLimitWarning;
	if (anyChange) {
		this->publishForceActuatorForceWarning();
	}
}

void ILCResponseParser::checkForceActuatorFollowingError(int32_t dataIndex) {
	float primaryForce = this->forceData->PrimaryCylinderForce[dataIndex];
	double primarySetpoint = this->forceData->PrimaryCylinderSetpointCommanded[dataIndex] / 1000.0;
	float secondaryForce = this->forceData->SecondaryCylinderForce[dataIndex];
	double secondarySetpoint = this->forceData->SecondaryCylinderSetpointCommanded[dataIndex] / 1000.0;
	double primaryLimit = this->forceActuatorSettings->Limits[dataIndex].FollowingErrorPrimaryAxisLimit;
	double secondaryLimit = this->forceActuatorSettings->Limits[dataIndex].FollowingErrorSecondaryAxisLimit;
	double primaryFollowingError = primaryForce - primarySetpoint;
	double secondaryFollowingError = secondaryForce - secondarySetpoint;
	bool primaryLimitWarning = std::abs(primaryFollowingError) > primaryLimit;
	bool secondaryLimitWarning = std::abs(secondaryFollowingError) > secondaryLimit;
	bool previousPrimaryWarning = this->forceWarning->PrimaryAxisFollowingErrorWarning[dataIndex];
	bool previousSecondaryWarning = this->forceWarning->SecondaryAxisFollowingErrorWarning[dataIndex];
	bool anyChange = primaryLimitWarning != previousPrimaryWarning || secondaryLimitWarning != previousSecondaryWarning;
	this->forceWarning->PrimaryAxisFollowingErrorWarning[dataIndex] = primaryLimitWarning;
	this->forceWarning->SecondaryAxisFollowingErrorWarning[dataIndex] = secondaryLimitWarning;
	if (anyChange) {
		this->publishForceActuatorForceWarning();
	}
}

void ILCResponseParser::publishForceActuatorForceWarning() {
	this->forceWarning->Timestamp = this->publisher->getTimestamp();
	this->forceWarning->AnyPrimaryAxisMeasuredForceWarning = false;
	this->forceWarning->AnySecondaryAxisMeasuredForceWarning = false;
	this->forceWarning->AnyPrimaryAxisFollowingErrorWarning = false;
	this->forceWarning->AnySecondaryAxisFollowingErrorWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceWarning->AnyPrimaryAxisMeasuredForceWarning = this->forceWarning->AnyPrimaryAxisMeasuredForceWarning || this->forceWarning->PrimaryAxisMeasuredForceWarning[i];
		this->forceWarning->AnySecondaryAxisMeasuredForceWarning = this->forceWarning->AnySecondaryAxisMeasuredForceWarning || this->forceWarning->SecondaryAxisMeasuredForceWarning[i];
		this->forceWarning->AnyPrimaryAxisFollowingErrorWarning = this->forceWarning->AnyPrimaryAxisFollowingErrorWarning || this->forceWarning->PrimaryAxisFollowingErrorWarning[i];
		this->forceWarning->AnySecondaryAxisFollowingErrorWarning = this->forceWarning->AnySecondaryAxisFollowingErrorWarning || this->forceWarning->SecondaryAxisFollowingErrorWarning[i];
	}
	this->forceWarning->AnyWarning =
			this->forceWarning->AnyPrimaryAxisMeasuredForceWarning ||
			this->forceWarning->AnySecondaryAxisMeasuredForceWarning ||
			this->forceWarning->AnyPrimaryAxisFollowingErrorWarning ||
			this->forceWarning->AnySecondaryAxisFollowingErrorWarning;
	this->publisher->logForceActuatorForceWarning();
}

void ILCResponseParser::warnResponseTimeout(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = true;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnInvalidCRC(double timestamp) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = -1;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = true;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnIllegalFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = true;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnIllegalDataValue(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = true;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnInvalidLength(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = true;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownSubnet(double timestamp) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = -1;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = true;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownAddress(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = true;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = true;
	this->ilcWarning->UnknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownProblem(double timestamp, int32_t actuatorId) {
	this->ilcWarning->Timestamp = timestamp;
	this->ilcWarning->ActuatorId = actuatorId;
	this->ilcWarning->ResponseTimeout = false;
	this->ilcWarning->InvalidCRC = false;
	this->ilcWarning->IllegalFunction = false;
	this->ilcWarning->IllegalDataValue = false;
	this->ilcWarning->InvalidLength = false;
	this->ilcWarning->UnknownSubnet = false;
	this->ilcWarning->UnknownAddress = false;
	this->ilcWarning->UnknownFunction = false;
	this->ilcWarning->UnknownProblem = true;
	this->publisher->logILCWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
