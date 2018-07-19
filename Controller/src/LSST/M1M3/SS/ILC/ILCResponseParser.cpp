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
#include <SAL_m1m3C.h>
#include <cmath>
#include <cstring>
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
	this->forceActuatorInfo = 0;
	this->forceActuatorState = 0;
	this->forceActuatorWarning = 0;
	this->forceWarning = 0;
	this->appliedCylinderForces = 0;
	this->forceActuatorData = 0;
	this->hardpointMonitorInfo = 0;
	this->hardpointMonitorState = 0;
	this->hardpointMonitorWarning = 0;
	this->hardpointMonitorData = 0;
	this->ilcWarning = 0;
	this->outerLoopData = 0;
	this->modbusResponse = 0;
	this->grabResponse = false;
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
	this->forceActuatorInfo = this->publisher->getEventForceActuatorInfo();
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
	this->forceWarning = this->publisher->getEventForceActuatorForceWarning();
	this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
	this->forceActuatorData = this->publisher->getForceActuatorData();
	this->hardpointMonitorInfo = this->publisher->getEventHardpointMonitorInfo();
	this->hardpointMonitorState = this->publisher->getEventHardpointMonitorState();
	this->hardpointMonitorWarning = this->publisher->getEventHardpointMonitorWarning();
	this->hardpointMonitorData = this->publisher->getHardpointMonitorData();
	this->ilcWarning = this->publisher->getEventILCWarning();
	this->outerLoopData = this->publisher->getOuterLoopData();
	this->modbusResponse = this->publisher->getEventModbusResponse();
	this->grabResponse = false;

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
	this->forceActuatorState->Timestamp = globalTimestamp;
	this->forceActuatorWarning->Timestamp = globalTimestamp;
	this->forceWarning->Timestamp = globalTimestamp;
	this->forceActuatorData->Timestamp = globalTimestamp;
	this->hardpointActuatorState->Timestamp = globalTimestamp;
	this->hardpointActuatorWarning->Timestamp = globalTimestamp;
	this->hardpointActuatorData->Timestamp = globalTimestamp;
	this->hardpointMonitorState->Timestamp = globalTimestamp;
	this->hardpointMonitorWarning->Timestamp = globalTimestamp;
	this->hardpointMonitorData->Timestamp = globalTimestamp;
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
						case 249: this->parseErrorResponse(buffer, timestamp, map.ActuatorId); break;
						default:
							Log.Warn("ILCResponseParser: Unknown FA function on subnet %d function %d", (int)function, subnet);
							this->warnUnknownFunction(timestamp, map.ActuatorId);
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
						case 238: this->parseErrorResponse(buffer, timestamp, map.ActuatorId); break;
						default:
							Log.Warn("ILCResponseParser: Unknown HP function %d on subnet %d", (int)function, subnet);
							this->warnUnknownFunction(timestamp, map.ActuatorId);
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
						case 250: this->parseErrorResponse(buffer, timestamp, map.ActuatorId); break;
						default:
							Log.Warn("ILCResponseParser: Unknown HM function %d on subnet %d", (int)function, subnet);
							this->warnUnknownFunction(timestamp, map.ActuatorId);
							break;
						}
						break;
					default:
						Log.Warn("ILCResponseParser: Unknown address %d on subnet %d for function code %d", (int)address, (int)subnet, (int)function);
						this->warnUnknownAddress(timestamp, map.ActuatorId);
						break;
					}
				}
				else {
					Log.Warn("ILCResponseParser: Unknown subnet %d", subnet);
					this->warnUnknownSubnet(timestamp);
				}
			}
		}
		else {
			Log.Warn("ILCResponseParser: Invalid CRC on subnet %d", subnet);
			if (this->grabResponse) {
				this->parseOneOffCommand(buffer, length, timestamp, false);
			}
			else {
				this->warnInvalidCRC(timestamp);
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
	double timestamp = this->publisher->getTimestamp();
	bool warn = false;
	bool anyTimeout = false;
	for(int i = 0; i < FA_COUNT; i++) {
		if (this->faExpectedResponses[i] != 0) {
			warn = true;
			this->warnResponseTimeout(timestamp, this->forceActuatorInfo->ReferenceId[i]);
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
			this->warnResponseTimeout(timestamp, this->hardpointActuatorInfo->ReferenceId[i]);
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
			this->warnResponseTimeout(timestamp, this->hardpointMonitorInfo->ReferenceId[i]);
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
	this->modbusResponse->Timestamp = timestamp;
	this->modbusResponse->ResponseValid = valid;
	this->modbusResponse->Address = buffer->readU8();
	this->modbusResponse->FunctionCode = buffer->readU8();
	this->modbusResponse->DataLength = length - 4;
	for(uint16_t i = 0; i < this->modbusResponse->DataLength; ++i) {
		this->modbusResponse->Data[i] = buffer->readU8();
	}
	this->modbusResponse->CRC = buffer->readCRC();
	buffer->skipToNextFrame();
	this->publisher->logModbusResponse();
}

void ILCResponseParser::parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId) {
	uint8_t exceptionCode = buffer->readU8();
	switch(exceptionCode) {
	case 1: this->warnIllegalFunction(timestamp, actuatorId); break;
	//case 2:	break; // Illegal Data Address
	case 3: this->warnIllegalDataValue(timestamp, actuatorId); break;
	//case 4: break; // Slave Device Failure
	//case 5: break; // Acknowledge
	//case 6: break; // Slave Device Busy
	//case 7: break; // Negative Acknowledge
	//case 8: break; // Memory Parity Error
	//case 10: break; // Gateway Path Unavailable
	//case 11: break; // Gateway Target Device Failed to Respond
	default:
		Log.Warn("ILCResponseParser: Actuator %d received exception code %d", actuatorId, (int32_t)exceptionCode);
		this->warnUnknownProblem(timestamp, actuatorId); break;
	}
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHPServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t length = buffer->readU8();
	this->hardpointActuatorInfo->ILCUniqueId[dataIndex] = buffer->readU48();
	this->hardpointActuatorInfo->ILCApplicationType[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->NetworkNodeType[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->MajorRevision[dataIndex] = buffer->readU8();
	this->hardpointActuatorInfo->MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t length = buffer->readU8();
	this->forceActuatorInfo->ILCUniqueId[dataIndex] = buffer->readU48();
	this->forceActuatorInfo->ILCApplicationType[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->NetworkNodeType[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->MajorRevision[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t length = buffer->readU8();
	this->hardpointMonitorInfo->ILCUniqueId[dataIndex] = buffer->readU48();
	this->hardpointMonitorInfo->ILCApplicationType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->NetworkNodeType[dataIndex] = buffer->readU8();
	buffer->readU8(); // ILCSelectedOptions
	buffer->readU8(); // NetworkNodeOptions
	this->hardpointMonitorInfo->MajorRevision[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHPServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointActuatorState->ILCState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointActuatorWarning->MajorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->hardpointActuatorWarning->MinorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->hardpointActuatorWarning->FaultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
	this->hardpointActuatorWarning->MainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
	this->hardpointActuatorWarning->BackupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointActuatorWarning->LimitSwitch1Operated[dataIndex] = (ilcStatus & 0x0100) != 0;
	this->hardpointActuatorWarning->LimitSwitch2Operated[dataIndex] = (ilcStatus & 0x0200) != 0;
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA (FA only)
	// 0x4000 is DCA (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->hardpointActuatorWarning->UniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->hardpointActuatorWarning->ApplicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->hardpointActuatorWarning->ApplicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->hardpointActuatorWarning->ApplicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->hardpointActuatorWarning->OneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->hardpointActuatorWarning->OneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->hardpointActuatorWarning->OneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->hardpointActuatorWarning->WatchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->hardpointActuatorWarning->BrownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->hardpointActuatorWarning->EventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	this->hardpointActuatorWarning->MotorDriverFault[dataIndex] = (ilcFaults & 0x0800) != 0;
	this->hardpointActuatorWarning->SSRPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->hardpointActuatorWarning->AuxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	this->hardpointActuatorWarning->SMCPowerFault[dataIndex] = (ilcFaults & 0x4000) != 0;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorState->ILCState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->forceActuatorWarning->MajorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->forceActuatorWarning->MinorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->forceActuatorWarning->FaultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
	this->forceActuatorWarning->MainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
	this->forceActuatorWarning->BackupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	this->forceActuatorWarning->MezzanineError[dataIndex] = (ilcStatus & 0x2000) != 0;
	this->forceActuatorWarning->MezzanineBootloaderActive[dataIndex] = (ilcStatus & 0x4000) != 0;
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->forceActuatorWarning->UniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->forceActuatorWarning->ApplicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->forceActuatorWarning->ApplicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->forceActuatorWarning->ApplicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->forceActuatorWarning->OneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->forceActuatorWarning->OneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->forceActuatorWarning->OneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->forceActuatorWarning->WatchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->forceActuatorWarning->BrownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->forceActuatorWarning->EventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	// 0x0800 is Motor Driver (HP only)
	this->forceActuatorWarning->SSRPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->forceActuatorWarning->AuxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	// 0x4000 is SMC Power (HP only)
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorState->ILCState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointMonitorWarning->MajorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->hardpointMonitorWarning->MinorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->hardpointMonitorWarning->FaultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
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
	this->hardpointMonitorWarning->UniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->hardpointMonitorWarning->ApplicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->hardpointMonitorWarning->ApplicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->hardpointMonitorWarning->ApplicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->hardpointMonitorWarning->OneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->hardpointMonitorWarning->OneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->hardpointMonitorWarning->OneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->hardpointMonitorWarning->WatchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->hardpointMonitorWarning->BrownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->hardpointMonitorWarning->EventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	// 0x0800 is Motor Driver (HP only)
	this->hardpointMonitorWarning->SSRPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->hardpointMonitorWarning->AuxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	// 0x4000 is SMC Power (HP only)
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeHPILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointActuatorState->ILCState[dataIndex] = buffer->readU16();
	//buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeFAILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorState->ILCState[dataIndex] = buffer->readU16();
	//buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseChangeHMILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorState->ILCState[dataIndex] = buffer->readU16();
	//buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseStepMotorResponse(ModbusBuffer* buffer, ILCMap map, double timestamp) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	this->hardpointActuatorData->Timestamp = timestamp;
	this->hardpointActuatorWarning->Timestamp = timestamp;
	this->hardpointActuatorWarning->ILCFault[dataIndex] = (status & 0x01) != 0;
	// 0x02 is reserved
	this->hardpointActuatorWarning->LimitSwitch1Operated[dataIndex] = (status & 0x04) != 0;
	this->hardpointActuatorWarning->LimitSwitch2Operated[dataIndex] = (status & 0x08) != 0;
	this->hardpointActuatorWarning->BroadcastCounterWarning[dataIndex] = this->outerLoopData->BroadcastCounter == ((status & 0xF0) >> 4);
	this->hardpointActuatorData->Encoder[dataIndex] = buffer->readI32();
	this->hardpointActuatorData->MeasuredForce[dataIndex] = buffer->readSGL();
	this->hardpointActuatorData->Displacement[dataIndex] = (this->hardpointActuatorData->Encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	this->hardpointActuatorWarning->ILCFault[dataIndex] = (status & 0x01) != 0;
	// 0x02 is reserved
	this->hardpointActuatorWarning->LimitSwitch1Operated[dataIndex] = (status & 0x04) != 0;
	this->hardpointActuatorWarning->LimitSwitch2Operated[dataIndex] = (status & 0x08) != 0;
	this->hardpointActuatorWarning->BroadcastCounterWarning[dataIndex] = this->outerLoopData->BroadcastCounter != ((status & 0xF0) >> 4);
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
	this->hardpointActuatorData->Encoder[dataIndex] = -buffer->readI32() + offset;
	// Unlike the pneumatic, the electromechanical doesn't reverse compression and tension so we swap it here
	this->hardpointActuatorData->MeasuredForce[dataIndex] = -buffer->readSGL();
	this->hardpointActuatorData->Displacement[dataIndex] = (this->hardpointActuatorData->Encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map) {
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorInfo->MezzaninePrimaryCylinderGain[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->MezzanineSecondaryCylinderGain[dataIndex] = buffer->readSGL();
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
	this->forceActuatorWarning->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->MezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->BroadcastCounterWarning[dataIndex] = this->outerLoopData->BroadcastCounter != ((status & 0xF0) >> 4);
	this->forceActuatorData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	ForceConverter::saaToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[dataIndex], &x, &y, &z);
	this->forceActuatorData->ZForce[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseDualAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;
	int xIndex = map.XDataIndex;
	int yIndex = map.YDataIndex;
	uint8_t status = buffer->readU8();
	this->forceActuatorWarning->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->MezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->BroadcastCounterWarning[dataIndex] = this->outerLoopData->BroadcastCounter != ((status & 0xF0) >> 4);
	this->forceActuatorData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	switch(this->forceActuatorInfo->ActuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	}
	if (xIndex != -1) {
		this->forceActuatorData->XForce[xIndex] = x;
	}
	if (yIndex != -1) {
		this->forceActuatorData->YForce[yIndex] = y;
	}
	this->forceActuatorData->ZForce[dataIndex] = z;
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
	this->forceActuatorWarning->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->MezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->BroadcastCounterWarning[dataIndex] = this->outerLoopData->BroadcastCounter != ((status & 0xF0) >> 4);
	this->forceActuatorData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	ForceConverter::saaToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[dataIndex], &x, &y, &z);
	this->forceActuatorData->ZForce[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;
	int xIndex = map.XDataIndex;
	int yIndex = map.YDataIndex;
	uint8_t status = buffer->readU8();
	this->forceActuatorWarning->ILCFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->MezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->BroadcastCounterWarning[dataIndex] = this->outerLoopData->BroadcastCounter != ((status & 0xF0) >> 4);
	this->forceActuatorData->PrimaryCylinderForce[dataIndex] = buffer->readSGL();
	this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	switch(this->forceActuatorInfo->ActuatorOrientation[dataIndex]) {
	case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToMirror(this->forceActuatorData->PrimaryCylinderForce[dataIndex], this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex], &x, &y, &z); break;
	}
	if (xIndex != -1) {
		this->forceActuatorData->XForce[xIndex] = x;
	}
	if (yIndex != -1) {
		this->forceActuatorData->YForce[yIndex] = y;
	}
	this->forceActuatorData->ZForce[dataIndex] = z;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetHPADCScanRateResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointActuatorInfo->ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseSetFAADCScanRateResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorInfo->ADCScanRate[dataIndex] = buffer->readU8();
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
	this->hardpointActuatorInfo->MainLoadCellCoefficient[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Coefficient K4
	this->hardpointActuatorInfo->MainLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Offset Channel 2
	buffer->readSGL(); // Main Offset Channel 3
	buffer->readSGL(); // Main Offset Channel 4
	this->hardpointActuatorInfo->MainLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Sensitivity Channel 2
	buffer->readSGL(); // Main Sensitivity Channel 3
	buffer->readSGL(); // Main Sensitivity Channel 4
	buffer->readSGL(); // Backup Coefficient K1
	buffer->readSGL(); // Backup Coefficient K2
	this->hardpointActuatorInfo->BackupLoadCellCoefficient[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Coefficient K4
	this->hardpointActuatorInfo->BackupLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Offset Channel 2
	buffer->readSGL(); // Backup Offset Channel 3
	buffer->readSGL(); // Backup Offset Channel 4
	this->hardpointActuatorInfo->BackupLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Sensitivity Channel 2
	buffer->readSGL(); // Backup Sensitivity Channel 3
	buffer->readSGL(); // Backup Sensitivity Channel 4
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReadFACalibrationResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorInfo->MainPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->MainSecondaryCylinderCoefficient[dataIndex] = this->forceActuatorInfo->MainPrimaryCylinderCoefficient[dataIndex];
	buffer->readSGL(); // Main Coefficient K2
	buffer->readSGL(); // Main Coefficient K3
	buffer->readSGL(); // Main Coefficient K4
	this->forceActuatorInfo->MainPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->MainSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Offset Channel 3
	buffer->readSGL(); // Main Offset Channel 4
	this->forceActuatorInfo->MainPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->MainSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Sensitivity Channel 3
	buffer->readSGL(); // Main Sensitivity Channel 4
	this->forceActuatorInfo->BackupPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->BackupSecondaryCylinderCoefficient[dataIndex] = this->forceActuatorInfo->BackupPrimaryCylinderCoefficient[dataIndex];
	buffer->readSGL(); // Backup Coefficient K2
	buffer->readSGL(); // Backup Coefficient K3
	buffer->readSGL(); // Backup Coefficient K4
	this->forceActuatorInfo->BackupPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->BackupSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Offset Channel 3
	buffer->readSGL(); // Backup Offset Channel 4
	this->forceActuatorInfo->BackupPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->BackupSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
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
	this->hardpointMonitorData->PressureSensor1[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->PressureSensor2[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->PressureSensor3[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->BreakawayPressure[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportDCAIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorInfo->MezzanineUniqueId[dataIndex] = buffer->readU48();
	this->forceActuatorInfo->MezzanineFirmwareType[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->MezzanineMajorRevision[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->MezzanineMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineIDResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorInfo->MezzanineUniqueId[dataIndex] = buffer->readU48();
	this->hardpointMonitorInfo->MezzanineFirmwareType[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MezzanineMajorRevision[dataIndex] = buffer->readU8();
	this->hardpointMonitorInfo->MezzanineMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportDCAStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint16_t status = buffer->readU16();
	// 0x0001 is DCA Outputs Enabled (wont report)
	this->forceActuatorWarning->MezzaninePowerFault[dataIndex] = (status & 0x0002) != 0;
	this->forceActuatorWarning->MezzanineCurrentAmp1Fault[dataIndex] = (status & 0x0004) != 0;
	this->forceActuatorWarning->MezzanineCurrentAmp2Fault[dataIndex] = (status & 0x0008) != 0;
	this->forceActuatorWarning->MezzanineUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
	// 0x0020 is reserved
	this->forceActuatorWarning->MezzanineMainCalibrationError[dataIndex] = (status & 0x0040) != 0;
	this->forceActuatorWarning->MezzanineBackupCalibrationError[dataIndex] = (status & 0x0080) != 0;
	this->forceActuatorWarning->MezzanineEventTrapReset[dataIndex] = (status & 0x0100) != 0;
	// 0x0200 is reserved
	// 0x0400 is reserved
	// 0x0800 is reserved
	this->forceActuatorWarning->MezzanineApplicationMissing[dataIndex] = (status & 0x1000) != 0;
	this->forceActuatorWarning->MezzanineApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
	// 0x4000 is reserved
	this->forceActuatorWarning->MezzanineBootloaderActive[dataIndex] = (status & 0x8000) != 0;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint16_t status = buffer->readU16();
	this->hardpointMonitorWarning->MezzanineS1AInterface1Fault[dataIndex] = (status & 0x0001) != 0;
	this->hardpointMonitorWarning->MezzanineS1ALVDT1Fault[dataIndex] = (status & 0x0002) != 0;
	this->hardpointMonitorWarning->MezzanineS1AInterface2Fault[dataIndex] = (status & 0x0004) != 0;
	this->hardpointMonitorWarning->MezzanineS1ALVDT2Fault[dataIndex] = (status & 0x0008) != 0;
	this->hardpointMonitorWarning->MezzanineUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
	// 0x0020 is reserved
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointMonitorWarning->MezzanineEventTrapReset[dataIndex] = (status & 0x0100) != 0;
	// 0x0200 is reserved
	this->hardpointMonitorWarning->MezzanineDCPRS422ChipFault[dataIndex] = (status & 0x0400) != 0;
	// 0x0800 is reserved
	this->hardpointMonitorWarning->MezzanineApplicationMissing[dataIndex] = (status & 0x1000) != 0;
	this->hardpointMonitorWarning->MezzanineApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
	// 0x4000 is reserved
	this->hardpointMonitorWarning->MezzanineBootloaderActive[dataIndex] = (status & 0x8000) != 0;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportLVDTResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorData->BreakawayLVDT[dataIndex] = buffer->readSGL();
	this->hardpointMonitorData->DisplacementLVDT[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILCResponseParser::checkForceActuatorMeasuredForce(ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;
	float primaryForce = this->forceActuatorData->PrimaryCylinderForce[dataIndex];
	float primaryLowLimit = this->forceActuatorSettings->MeasuredPrimaryCylinderLimitTable[dataIndex].LowFault;
	float primaryHighLimit = this->forceActuatorSettings->MeasuredPrimaryCylinderLimitTable[dataIndex].HighFault;
	bool primaryLimitWarning = primaryForce < primaryLowLimit || primaryForce > primaryHighLimit;
	bool previousPrimaryLimit = this->forceWarning->PrimaryAxisMeasuredForceWarning[dataIndex];
	this->forceWarning->PrimaryAxisMeasuredForceWarning[dataIndex] = primaryLimitWarning;
	bool anyChange = primaryLimitWarning != previousPrimaryLimit;

	if (secondaryDataIndex != -1) {
		float secondaryForce = this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex];
		float secondaryLowLimit = this->forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].LowFault;
		float secondaryHighLimit = this->forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
		bool secondaryLimitWarning = secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLimit;
		bool previousSecondaryLimit = this->forceWarning->SecondaryAxisMeasuredForceWarning[dataIndex];
		this->forceWarning->SecondaryAxisMeasuredForceWarning[dataIndex] = secondaryLimitWarning;
		anyChange = anyChange || secondaryLimitWarning !=  previousSecondaryLimit;
	}

	if (anyChange) {
		this->publishForceActuatorForceWarning();
	}
}

void ILCResponseParser::checkForceActuatorFollowingError(ILCMap map) {
	// TODO: UPDATE
	int32_t dataIndex = map.DataIndex;
	int32_t secondaryDataIndex = map.SecondaryDataIndex;

	float primaryForce = this->forceActuatorData->PrimaryCylinderForce[dataIndex];
	float primarySetpoint = this->appliedCylinderForces->PrimaryCylinderForces[dataIndex] / 1000.0;
	float primaryLimit = this->forceActuatorSettings->FollowingErrorPrimaryCylinderLimitTable[dataIndex].HighFault;
	float primaryFollowingError = primaryForce - primarySetpoint;
	bool primaryLimitWarning = std::abs(primaryFollowingError) > primaryLimit;
	bool previousPrimaryWarning = this->forceWarning->PrimaryAxisFollowingErrorWarning[dataIndex];
	this->forceWarning->PrimaryAxisFollowingErrorWarning[dataIndex] = primaryLimitWarning;
	bool anyChange = primaryLimitWarning != previousPrimaryWarning;

	if (secondaryDataIndex != -1) {
		float secondaryForce = this->forceActuatorData->SecondaryCylinderForce[secondaryDataIndex];
		float secondarySetpoint = this->appliedCylinderForces->SecondaryCylinderForces[secondaryDataIndex] / 1000.0;
		float secondaryLimit = this->forceActuatorSettings->FollowingErrorSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
		float secondaryFollowingError = secondaryForce - secondarySetpoint;
		bool secondaryLimitWarning = std::abs(secondaryFollowingError) > secondaryLimit;
		bool previousSecondaryWarning = this->forceWarning->SecondaryAxisFollowingErrorWarning[dataIndex];
		this->forceWarning->SecondaryAxisFollowingErrorWarning[dataIndex] = secondaryLimitWarning;
		anyChange = anyChange || secondaryLimitWarning != previousSecondaryWarning;
	}

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
