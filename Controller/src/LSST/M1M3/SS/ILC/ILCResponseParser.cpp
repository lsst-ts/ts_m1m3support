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

	this->forceWarning->timestamp = 0;
	this->forceWarning->anyWarning = false;
	this->forceWarning->anyPrimaryAxisMeasuredForceWarning= false;
	this->forceWarning->anySecondaryAxisMeasuredForceWarning = false;
	this->forceWarning->anyPrimaryAxisFollowingErrorWarning = false;
	this->forceWarning->anySecondaryAxisFollowingErrorWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceWarning->primaryAxisMeasuredForceWarning[i] = false;
		this->forceWarning->secondaryAxisMeasuredForceWarning[i] = false;
		this->forceWarning->primaryAxisFollowingErrorWarning[i] = false;
		this->forceWarning->secondaryAxisFollowingErrorWarning[i] = false;
	}

	this->ilcWarning->timestamp = 0;
	this->ilcWarning->actuatorId = -1;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;

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
	this->forceActuatorState->timestamp = globalTimestamp;
	this->forceActuatorWarning->timestamp = globalTimestamp;
	this->forceWarning->timestamp = globalTimestamp;
	this->forceActuatorData->timestamp = globalTimestamp;
	this->hardpointActuatorState->timestamp = globalTimestamp;
	this->hardpointActuatorWarning->timestamp = globalTimestamp;
	this->hardpointActuatorData->timestamp = globalTimestamp;
	this->hardpointMonitorState->timestamp = globalTimestamp;
	this->hardpointMonitorWarning->timestamp = globalTimestamp;
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
			this->warnResponseTimeout(timestamp, this->forceActuatorInfo->referenceId[i]);
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
			this->warnResponseTimeout(timestamp, this->hardpointActuatorInfo->referenceId[i]);
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
			this->warnResponseTimeout(timestamp, this->hardpointMonitorInfo->referenceId[i]);
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
	this->modbusResponse->timestamp = timestamp;
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
	this->forceActuatorInfo->ilcUniqueId[dataIndex] = buffer->readU48();
	this->forceActuatorInfo->ilcApplicationType[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->networkNodeType[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->ilcSelectedOptions[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->networkNodeOptions[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->majorRevision[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->minorRevision[dataIndex] = buffer->readU8();
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
	this->hardpointActuatorWarning->majorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->hardpointActuatorWarning->minorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->hardpointActuatorWarning->faultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
	this->hardpointActuatorWarning->mainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
	this->hardpointActuatorWarning->backupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointActuatorWarning->limitSwitch1Operated[dataIndex] = (ilcStatus & 0x0100) != 0;
	this->hardpointActuatorWarning->limitSwitch2Operated[dataIndex] = (ilcStatus & 0x0200) != 0;
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA (FA only)
	// 0x4000 is DCA (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->hardpointActuatorWarning->uniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->hardpointActuatorWarning->applicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->hardpointActuatorWarning->applicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->hardpointActuatorWarning->applicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->hardpointActuatorWarning->oneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->hardpointActuatorWarning->oneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->hardpointActuatorWarning->oneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->hardpointActuatorWarning->watchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->hardpointActuatorWarning->brownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->hardpointActuatorWarning->eventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	this->hardpointActuatorWarning->motorDriverFault[dataIndex] = (ilcFaults & 0x0800) != 0;
	this->hardpointActuatorWarning->ssrPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->hardpointActuatorWarning->auxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	this->hardpointActuatorWarning->smcPowerFault[dataIndex] = (ilcFaults & 0x4000) != 0;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportFAServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->forceActuatorState->ilcState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->forceActuatorWarning->majorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->forceActuatorWarning->minorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->forceActuatorWarning->faultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
	this->forceActuatorWarning->mainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
	this->forceActuatorWarning->backupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	this->forceActuatorWarning->mezzanineError[dataIndex] = (ilcStatus & 0x2000) != 0;
	this->forceActuatorWarning->mezzanineBootloaderActive[dataIndex] = (ilcStatus & 0x4000) != 0;
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->forceActuatorWarning->uniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->forceActuatorWarning->applicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->forceActuatorWarning->applicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->forceActuatorWarning->applicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->forceActuatorWarning->oneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->forceActuatorWarning->oneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->forceActuatorWarning->oneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->forceActuatorWarning->watchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->forceActuatorWarning->brownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->forceActuatorWarning->eventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	// 0x0800 is Motor Driver (HP only)
	this->forceActuatorWarning->ssrPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->forceActuatorWarning->auxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
	// 0x4000 is SMC Power (HP only)
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	this->hardpointMonitorState->ilcState[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointMonitorWarning->majorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
	this->hardpointMonitorWarning->minorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
	// 0x0004 is reserved
	this->hardpointMonitorWarning->faultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
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
	this->hardpointMonitorWarning->uniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
	this->hardpointMonitorWarning->applicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
	this->hardpointMonitorWarning->applicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
	this->hardpointMonitorWarning->applicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
	this->hardpointMonitorWarning->oneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
	this->hardpointMonitorWarning->oneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
	this->hardpointMonitorWarning->oneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
	// 0x0080 is reserved
	this->hardpointMonitorWarning->watchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
	this->hardpointMonitorWarning->brownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
	this->hardpointMonitorWarning->eventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
	// 0x0800 is Motor Driver (HP only)
	this->hardpointMonitorWarning->ssrPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
	this->hardpointMonitorWarning->auxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
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
	this->hardpointActuatorWarning->timestamp = timestamp;
	this->hardpointActuatorWarning->ilcFault[dataIndex] = (status & 0x01) != 0;
	// 0x02 is reserved
	this->hardpointActuatorWarning->limitSwitch1Operated[dataIndex] = (status & 0x04) != 0;
	this->hardpointActuatorWarning->limitSwitch2Operated[dataIndex] = (status & 0x08) != 0;
	this->hardpointActuatorWarning->broadcastCounterWarning[dataIndex] = this->outerLoopData->broadcastCounter == ((status & 0xF0) >> 4);
	this->hardpointActuatorData->encoder[dataIndex] = buffer->readI32();
	this->hardpointActuatorData->measuredForce[dataIndex] = buffer->readSGL();
	this->hardpointActuatorData->displacement[dataIndex] = (this->hardpointActuatorData->encoder[dataIndex] * this->hardpointActuatorSettings->MicrometersPerEncoder) / (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint8_t status = buffer->readU8();
	this->hardpointActuatorWarning->ilcFault[dataIndex] = (status & 0x01) != 0;
	// 0x02 is reserved
	this->hardpointActuatorWarning->limitSwitch1Operated[dataIndex] = (status & 0x04) != 0;
	this->hardpointActuatorWarning->limitSwitch2Operated[dataIndex] = (status & 0x08) != 0;
	this->hardpointActuatorWarning->broadcastCounterWarning[dataIndex] = this->outerLoopData->broadcastCounter != ((status & 0xF0) >> 4);
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
	this->forceActuatorInfo->mezzaninePrimaryCylinderGain[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->mezzanineSecondaryCylinderGain[dataIndex] = buffer->readSGL();
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
	this->forceActuatorWarning->ilcFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->mezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->broadcastCounterWarning[dataIndex] = this->outerLoopData->broadcastCounter != ((status & 0xF0) >> 4);
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
	this->forceActuatorWarning->ilcFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->mezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->broadcastCounterWarning[dataIndex] = this->outerLoopData->broadcastCounter != ((status & 0xF0) >> 4);
	this->forceActuatorData->primaryCylinderForces[dataIndex] = buffer->readSGL();
	this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	switch(this->forceActuatorInfo->actuatorOrientation[dataIndex]) {
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
	this->forceActuatorWarning->ilcFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->mezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->broadcastCounterWarning[dataIndex] = this->outerLoopData->broadcastCounter != ((status & 0xF0) >> 4);
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
	this->forceActuatorWarning->ilcFault[dataIndex] = (status & 0x01) != 0;
	this->forceActuatorWarning->mezzanineError[dataIndex] = (status & 0x02) != 0;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceActuatorWarning->broadcastCounterWarning[dataIndex] = this->outerLoopData->broadcastCounter != ((status & 0xF0) >> 4);
	this->forceActuatorData->primaryCylinderForces[dataIndex] = buffer->readSGL();
	this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex] = buffer->readSGL();
	float x = 0;
	float y = 0;
	float z = 0;
	switch(this->forceActuatorInfo->actuatorOrientation[dataIndex]) {
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
	this->forceActuatorInfo->adcScanRate[dataIndex] = buffer->readU8();
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
	this->forceActuatorInfo->mainPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->mainSecondaryCylinderCoefficient[dataIndex] = this->forceActuatorInfo->mainPrimaryCylinderCoefficient[dataIndex];
	buffer->readSGL(); // Main Coefficient K2
	buffer->readSGL(); // Main Coefficient K3
	buffer->readSGL(); // Main Coefficient K4
	this->forceActuatorInfo->mainPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->mainSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Offset Channel 3
	buffer->readSGL(); // Main Offset Channel 4
	this->forceActuatorInfo->mainPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->mainSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Main Sensitivity Channel 3
	buffer->readSGL(); // Main Sensitivity Channel 4
	this->forceActuatorInfo->backupPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->backupSecondaryCylinderCoefficient[dataIndex] = this->forceActuatorInfo->backupPrimaryCylinderCoefficient[dataIndex];
	buffer->readSGL(); // Backup Coefficient K2
	buffer->readSGL(); // Backup Coefficient K3
	buffer->readSGL(); // Backup Coefficient K4
	this->forceActuatorInfo->backupPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->backupSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL(); // Backup Offset Channel 3
	buffer->readSGL(); // Backup Offset Channel 4
	this->forceActuatorInfo->backupPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceActuatorInfo->backupSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
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
	this->forceActuatorInfo->mezzanineUniqueId[dataIndex] = buffer->readU48();
	this->forceActuatorInfo->mezzanineFirmwareType[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->mezzanineMajorRevision[dataIndex] = buffer->readU8();
	this->forceActuatorInfo->mezzanineMinorRevision[dataIndex] = buffer->readU8();
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
	this->forceActuatorWarning->mezzaninePowerFault[dataIndex] = (status & 0x0002) != 0;
	this->forceActuatorWarning->mezzanineCurrentAmp1Fault[dataIndex] = (status & 0x0004) != 0;
	this->forceActuatorWarning->mezzanineCurrentAmp2Fault[dataIndex] = (status & 0x0008) != 0;
	this->forceActuatorWarning->mezzanineUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
	// 0x0020 is reserved
	this->forceActuatorWarning->mezzanineMainCalibrationError[dataIndex] = (status & 0x0040) != 0;
	this->forceActuatorWarning->mezzanineBackupCalibrationError[dataIndex] = (status & 0x0080) != 0;
	this->forceActuatorWarning->mezzanineEventTrapReset[dataIndex] = (status & 0x0100) != 0;
	// 0x0200 is reserved
	// 0x0400 is reserved
	// 0x0800 is reserved
	this->forceActuatorWarning->mezzanineApplicationMissing[dataIndex] = (status & 0x1000) != 0;
	this->forceActuatorWarning->mezzanineApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
	// 0x4000 is reserved
	this->forceActuatorWarning->mezzanineBootloaderActive[dataIndex] = (status & 0x8000) != 0;
	buffer->skipToNextFrame();
}

void ILCResponseParser::parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, ILCMap map) {
	int32_t dataIndex = map.DataIndex;
	uint16_t status = buffer->readU16();
	this->hardpointMonitorWarning->mezzanineS1AInterface1Fault[dataIndex] = (status & 0x0001) != 0;
	this->hardpointMonitorWarning->mezzanineS1ALVDT1Fault[dataIndex] = (status & 0x0002) != 0;
	this->hardpointMonitorWarning->mezzanineS1AInterface2Fault[dataIndex] = (status & 0x0004) != 0;
	this->hardpointMonitorWarning->mezzanineS1ALVDT2Fault[dataIndex] = (status & 0x0008) != 0;
	this->hardpointMonitorWarning->mezzanineUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
	// 0x0020 is reserved
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointMonitorWarning->mezzanineEventTrapReset[dataIndex] = (status & 0x0100) != 0;
	// 0x0200 is reserved
	this->hardpointMonitorWarning->mezzanineDCPRS422ChipFault[dataIndex] = (status & 0x0400) != 0;
	// 0x0800 is reserved
	this->hardpointMonitorWarning->mezzanineApplicationMissing[dataIndex] = (status & 0x1000) != 0;
	this->hardpointMonitorWarning->mezzanineApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
	// 0x4000 is reserved
	this->hardpointMonitorWarning->mezzanineBootloaderActive[dataIndex] = (status & 0x8000) != 0;
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
	bool previousPrimaryLimit = this->forceWarning->primaryAxisMeasuredForceWarning[dataIndex];
	this->forceWarning->primaryAxisMeasuredForceWarning[dataIndex] = primaryLimitWarning;
	bool anyChange = primaryLimitWarning != previousPrimaryLimit;

	if (secondaryDataIndex != -1) {
		float secondaryForce = this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex];
		float secondaryLowLimit = this->forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].LowFault;
		float secondaryHighLimit = this->forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
		bool secondaryLimitWarning = secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLimit;
		bool previousSecondaryLimit = this->forceWarning->secondaryAxisMeasuredForceWarning[dataIndex];
		this->forceWarning->secondaryAxisMeasuredForceWarning[dataIndex] = secondaryLimitWarning;
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

	float primaryForce = this->forceActuatorData->primaryCylinderForces[dataIndex];
	float primarySetpoint = this->appliedCylinderForces->primaryCylinderForces[dataIndex] / 1000.0;
	float primaryLimit = this->forceActuatorSettings->FollowingErrorPrimaryCylinderLimitTable[dataIndex].HighFault;
	float primaryFollowingError = primaryForce - primarySetpoint;
	bool primaryLimitWarning = std::abs(primaryFollowingError) > primaryLimit;
	bool previousPrimaryWarning = this->forceWarning->primaryAxisFollowingErrorWarning[dataIndex];
	this->forceWarning->primaryAxisFollowingErrorWarning[dataIndex] = primaryLimitWarning;
	bool anyChange = primaryLimitWarning != previousPrimaryWarning;

	if (secondaryDataIndex != -1) {
		float secondaryForce = this->forceActuatorData->secondaryCylinderForces[secondaryDataIndex];
		float secondarySetpoint = this->appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex] / 1000.0;
		float secondaryLimit = this->forceActuatorSettings->FollowingErrorSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
		float secondaryFollowingError = secondaryForce - secondarySetpoint;
		bool secondaryLimitWarning = std::abs(secondaryFollowingError) > secondaryLimit;
		bool previousSecondaryWarning = this->forceWarning->secondaryAxisFollowingErrorWarning[dataIndex];
		this->forceWarning->secondaryAxisFollowingErrorWarning[dataIndex] = secondaryLimitWarning;
		anyChange = anyChange || secondaryLimitWarning != previousSecondaryWarning;
	}

	if (anyChange) {
		this->publishForceActuatorForceWarning();
	}
}

void ILCResponseParser::publishForceActuatorForceWarning() {
	this->forceWarning->timestamp = this->publisher->getTimestamp();
	this->forceWarning->anyPrimaryAxisMeasuredForceWarning = false;
	this->forceWarning->anySecondaryAxisMeasuredForceWarning = false;
	this->forceWarning->anyPrimaryAxisFollowingErrorWarning = false;
	this->forceWarning->anySecondaryAxisFollowingErrorWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceWarning->anyPrimaryAxisMeasuredForceWarning = this->forceWarning->anyPrimaryAxisMeasuredForceWarning || this->forceWarning->primaryAxisMeasuredForceWarning[i];
		this->forceWarning->anySecondaryAxisMeasuredForceWarning = this->forceWarning->anySecondaryAxisMeasuredForceWarning || this->forceWarning->secondaryAxisMeasuredForceWarning[i];
		this->forceWarning->anyPrimaryAxisFollowingErrorWarning = this->forceWarning->anyPrimaryAxisFollowingErrorWarning || this->forceWarning->primaryAxisFollowingErrorWarning[i];
		this->forceWarning->anySecondaryAxisFollowingErrorWarning = this->forceWarning->anySecondaryAxisFollowingErrorWarning || this->forceWarning->secondaryAxisFollowingErrorWarning[i];
	}
	this->forceWarning->anyWarning =
			this->forceWarning->anyPrimaryAxisMeasuredForceWarning ||
			this->forceWarning->anySecondaryAxisMeasuredForceWarning ||
			this->forceWarning->anyPrimaryAxisFollowingErrorWarning ||
			this->forceWarning->anySecondaryAxisFollowingErrorWarning;
	this->publisher->logForceActuatorForceWarning();
}

void ILCResponseParser::warnResponseTimeout(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = true;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnInvalidCRC(double timestamp) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = -1;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = true;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnIllegalFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = true;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnIllegalDataValue(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = true;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnInvalidLength(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = true;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownSubnet(double timestamp) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = -1;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = true;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownAddress(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = true;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = true;
	this->ilcWarning->unknownProblem = false;
	this->publisher->logILCWarning();
}

void ILCResponseParser::warnUnknownProblem(double timestamp, int32_t actuatorId) {
	this->ilcWarning->timestamp = timestamp;
	this->ilcWarning->actuatorId = actuatorId;
	this->ilcWarning->responseTimeout = false;
	this->ilcWarning->invalidCRC = false;
	this->ilcWarning->illegalFunction = false;
	this->ilcWarning->illegalDataValue = false;
	this->ilcWarning->invalidLength = false;
	this->ilcWarning->unknownSubnet = false;
	this->ilcWarning->unknownAddress = false;
	this->ilcWarning->unknownFunction = false;
	this->ilcWarning->unknownProblem = true;
	this->publisher->logILCWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
