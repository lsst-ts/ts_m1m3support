/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <cmath>
#include <cstring>
#include <spdlog/spdlog.h>

#include <SAL_MTM1M3C.h>

#include <cRIO/CliApp.h>

#include <ILCResponseParser.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <HardpointActuatorWarning.h>
#include <M1M3SSPublisher.h>
#include <ModbusBuffer.h>
#include <LimitLog.h>
#include <ForceConverter.h>
#include <ForceActuatorApplicationSettings.h>
#include <ILCDataTypes.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <ILCSubnetData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCResponseParser::ILCResponseParser() {
    _forceActuatorSettings = 0;
    _hardpointActuatorSettings = 0;
    _subnetData = 0;
    _safetyController = 0;
    _hardpointActuatorInfo = 0;
    _hardpointActuatorState = 0;
    _hardpointActuatorData = 0;
    _forceActuatorInfo = 0;
    _forceActuatorState = 0;
    _forceWarning = 0;
    _appliedCylinderForces = 0;
    _forceActuatorData = 0;
    _hardpointMonitorInfo = 0;
    _hardpointMonitorState = 0;
    _hardpointMonitorWarning = 0;
    _hardpointMonitorData = 0;
    _ilcWarning = 0;
    _outerLoopData = 0;
    _detailedState = 0;
    _summaryState = 0;
}

ILCResponseParser::ILCResponseParser(ForceActuatorSettings* forceActuatorSettings,
                                     HardpointActuatorSettings* hardpointActuatorSettings,
                                     ILCSubnetData* subnetData, SafetyController* safetyController) {
    SPDLOG_DEBUG("ILCResponseParser: ILCResponseParser()");
    _forceActuatorSettings = forceActuatorSettings;
    _hardpointActuatorSettings = hardpointActuatorSettings;
    _subnetData = subnetData;
    _safetyController = safetyController;
    _hardpointActuatorInfo = M1M3SSPublisher::instance().getEventHardpointActuatorInfo();
    _hardpointActuatorState = M1M3SSPublisher::instance().getEventHardpointActuatorState();
    _hardpointActuatorData = M1M3SSPublisher::instance().getHardpointActuatorData();
    _forceActuatorInfo = M1M3SSPublisher::instance().getEventForceActuatorInfo();
    _forceActuatorState = M1M3SSPublisher::instance().getEventForceActuatorState();
    _forceWarning = M1M3SSPublisher::instance().getEventForceActuatorForceWarning();
    _appliedCylinderForces = M1M3SSPublisher::instance().getAppliedCylinderForces();
    _forceActuatorData = M1M3SSPublisher::instance().getForceActuatorData();
    _hardpointMonitorInfo = M1M3SSPublisher::instance().getEventHardpointMonitorInfo();
    _hardpointMonitorState = M1M3SSPublisher::instance().getEventHardpointMonitorState();
    _hardpointMonitorWarning = M1M3SSPublisher::instance().getEventHardpointMonitorWarning();
    _hardpointMonitorData = M1M3SSPublisher::instance().getHardpointMonitorData();
    _ilcWarning = M1M3SSPublisher::instance().getEventILCWarning();
    _outerLoopData = M1M3SSPublisher::instance().getOuterLoopData();
    _detailedState = M1M3SSPublisher::instance().getEventDetailedState();
    _summaryState = M1M3SSPublisher::instance().getEventSummaryState();

    _forceWarning->timestamp = 0;
    _forceWarning->anyWarning = false;
    _forceWarning->anyPrimaryAxisMeasuredForceWarning = false;
    _forceWarning->anySecondaryAxisMeasuredForceWarning = false;
    _forceWarning->anyPrimaryAxisFollowingErrorWarning = false;
    _forceWarning->anySecondaryAxisFollowingErrorWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        _forceWarning->primaryAxisMeasuredForceWarning[i] = false;
        _forceWarning->secondaryAxisMeasuredForceWarning[i] = false;
        _forceWarning->primaryAxisFollowingErrorWarning[i] = false;
        _forceWarning->secondaryAxisFollowingErrorWarning[i] = false;
    }

    _ilcWarning->timestamp = 0;
    _ilcWarning->actuatorId = -1;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;

    memset(_faExpectedResponses, 0, sizeof(_faExpectedResponses));
    memset(_hpExpectedResponses, 0, sizeof(_hpExpectedResponses));
    memset(_hmExpectedResponses, 0, sizeof(_hmExpectedResponses));
}

bool validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp, uint16_t& receivedCRC,
                 uint16_t& calculatedCRC) {
    int32_t addressIndex = buffer->getIndex();
    while (!buffer->endOfFrame() && !buffer->endOfBuffer() && (buffer->readLength() & 0xF000) != 0xB000) {
    }
    int32_t crcIndex = buffer->getIndex() - 3;
    buffer->setIndex(crcIndex);
    calculatedCRC = buffer->calculateCRC(crcIndex - addressIndex);
    receivedCRC = buffer->readCRC();
    (*timestamp) = buffer->readTimestamp();
    if (receivedCRC == calculatedCRC) {
        (*length) = crcIndex - addressIndex;
        buffer->setIndex(addressIndex);
        return true;
    } else {
        buffer->readEndOfFrame();
        (*length) = buffer->getIndex() - addressIndex;
        return false;
    }
}

void ILCResponseParser::parse(ModbusBuffer* buffer, uint8_t subnet) {
    uint64_t a = buffer->readLength();
    uint64_t b = buffer->readLength();
    uint64_t c = buffer->readLength();
    uint64_t d = buffer->readLength();
    double globalTimestamp = Timestamp::fromRaw((d << 48) | (c << 32) | (b << 16) | a);
    _forceActuatorState->timestamp = globalTimestamp;
    M1M3SSPublisher::getForceActuatorWarning()->setTimestamp(globalTimestamp);
    _forceWarning->timestamp = globalTimestamp;
    _forceActuatorData->timestamp = globalTimestamp;
    _hardpointActuatorState->timestamp = globalTimestamp;
    HardpointActuatorWarning::instance().timestamp = globalTimestamp;
    _hardpointActuatorData->timestamp = globalTimestamp;
    _hardpointMonitorState->timestamp = globalTimestamp;
    _hardpointMonitorWarning->timestamp = globalTimestamp;
    _hardpointMonitorData->timestamp = globalTimestamp;
    while (!buffer->endOfBuffer()) {
        uint16_t length = 0;
        double timestamp = 0;
        uint16_t calculatedCRC;
        uint16_t receivedCRC;
        if (validateCRC(buffer, &length, &timestamp, receivedCRC, calculatedCRC) == false) {
            auto data = buffer->getReadData(length);
            std::ostringstream data_buf;
            LSST::cRIO::CliApp::printHexBuffer(data.data(), length, data_buf);
            TG_LOG_WARN(60s,
                        "ILCResponseParser: Invalid CRC (or extra byte) on subnet {:d} - received {:04X}, "
                        "calculated {:04X}, "
                        "address {:02X}, function {:02X}, data [{}]",
                        subnet, receivedCRC, calculatedCRC, data[0], data[1], data_buf.str());
            _warnInvalidCRC(timestamp);
        } else {
            if (subnet >= 1 && subnet <= 5) {
                uint8_t address = buffer->readU8();
                uint8_t function = buffer->readU8();
                ILCMap map = _subnetData->getILCDataFromAddress(subnet - 1, address);
                int32_t dataIndex = map.DataIndex;
                switch (map.Type) {
                    case ILCTypes::FA:
                        _faExpectedResponses[dataIndex]--;
                        switch (function) {
                            case 17:
                                _parseReportFAServerIDResponse(buffer, map);
                                break;
                            case 18:
                                _parseReportFAServerStatusResponse(buffer, map);
                                break;
                            case 65:
                                _parseChangeFAILCModeResponse(buffer, map);
                                break;
                            case 73:
                                _parseSetBoostValveDCAGainsResponse(buffer, map);
                                break;
                            case 74:
                                _parseReadBoostValveDCAGainsResponse(buffer, map);
                                break;
                            case 75:
                                _parseForceDemandResponse(buffer, address, map);
                                break;
                            case 76:
                                _parsePneumaticForceStatusResponse(buffer, address, map);
                                break;
                            case 80:
                                _parseSetFAADCScanRateResponse(buffer, map);
                                break;
                            case 81:
                                _parseSetFAADCChannelOffsetAndSensitivityResponse(buffer, map);
                                break;
                            case 107:
                                _parseFAResetResponse(buffer, map);
                                break;
                            case 110:
                                _parseReadFACalibrationResponse(buffer, map);
                                break;
                            case 119:
                                _parseReadDCAPressureValuesResponse(buffer, map);
                                break;
                            case 120:
                                _parseReportDCAIDResponse(buffer, map);
                                break;
                            case 121:
                                _parseReportDCAStatusResponse(buffer, map);
                                break;
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
                            case 249:
                                _parseErrorResponse(buffer, timestamp, map.ActuatorId);
                                break;
                            default:
                                SPDLOG_WARN(
                                        "ILCResponseParser: Unknown FA function on subnet {:d} function "
                                        "{:d}",
                                        (int)function, subnet);
                                _warnUnknownFunction(timestamp, map.ActuatorId);
                                break;
                        }
                        break;
                    case ILCTypes::HP:
                        _hpExpectedResponses[dataIndex]--;
                        switch (function) {
                            case 17:
                                _parseReportHPServerIDResponse(buffer, map);
                                break;
                            case 18:
                                _parseReportHPServerStatusResponse(buffer, map);
                                break;
                            case 65:
                                _parseChangeHPILCModeResponse(buffer, map);
                                break;
                            case 66:
                            case 67:
                                _parseElectromechanicalForceAndStatusResponse(buffer, map, timestamp);
                                break;
                            case 80:
                                _parseSetHPADCScanRateResponse(buffer, map);
                                break;
                            case 81:
                                _parseSetHPADCChannelOffsetAndSensitivityResponse(buffer, map);
                                break;
                            case 107:
                                _parseHPResetResponse(buffer, map);
                                break;
                            case 110:
                                _parseReadHPCalibrationResponse(buffer, map);
                                break;
                            case 145:
                            case 146:
                            case 193:
                            case 194:
                            case 195:
                            case 208:
                            case 209:
                            case 235:
                            case 238:
                                _parseErrorResponse(buffer, timestamp, map.ActuatorId);
                                break;
                            default:
                                SPDLOG_WARN("ILCResponseParser: Unknown HP function {:d} on subnet {:d}",
                                            (int)function, subnet);
                                _warnUnknownFunction(timestamp, map.ActuatorId);
                                break;
                        }
                        break;
                    case ILCTypes::HM:
                        _hmExpectedResponses[dataIndex]--;
                        switch (function) {
                            case 17:
                                _parseReportHMServerIDResponse(buffer, map);
                                break;
                            case 18:
                                _parseReportHMServerStatusResponse(buffer, map);
                                break;
                            case 65:
                                _parseChangeHMILCModeResponse(buffer, map);
                                break;
                            case 107:
                                _parseHMResetResponse(buffer, map);
                                break;
                            case 119:
                                _parseReadHMPressureValuesResponse(buffer, map);
                                break;
                            case 120:
                                _parseReportHMMezzanineIDResponse(buffer, map);
                                break;
                            case 121:
                                _parseReportHMMezzanineStatusResponse(buffer, map);
                                break;
                            case 122:
                                _parseReportLVDTResponse(buffer, map);
                                break;
                            case 145:
                            case 146:
                            case 193:
                            case 235:
                            case 247:
                            case 248:
                            case 249:
                            case 250:
                                _parseErrorResponse(buffer, timestamp, map.ActuatorId);
                                break;
                            default:
                                SPDLOG_WARN("ILCResponseParser: Unknown HM function {:d} on subnet {:d}",
                                            (int)function, subnet);
                                _warnUnknownFunction(timestamp, map.ActuatorId);
                                break;
                        }
                        break;
                    default:
                        SPDLOG_WARN(
                                "ILCResponseParser: Unknown address {:d} on subnet {:d} for function "
                                "code {:d}",
                                (int)address, (int)subnet, (int)function);
                        _warnUnknownAddress(timestamp, map.ActuatorId);
                        break;
                }
            } else {
                SPDLOG_WARN("ILCResponseParser: Unknown subnet {:d}", subnet);
                _warnUnknownSubnet(timestamp);
            }
        }
    }
    M1M3SSPublisher::getForceActuatorWarning()->log();
}

void ILCResponseParser::incExpectedResponses(int32_t* fa, int32_t* hp, int32_t* hm) {
    for (int i = 0; i < FA_COUNT; i++) {
        _faExpectedResponses[i] += fa[i];
    }
    for (int i = 0; i < HP_COUNT; i++) {
        _hpExpectedResponses[i] += hp[i];
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _hmExpectedResponses[i] += hm[i];
    }
}

void ILCResponseParser::clearResponses() {
    for (int i = 0; i < FA_COUNT; i++) {
        _faExpectedResponses[i] = 0;
    }
    for (int i = 0; i < HP_COUNT; i++) {
        _hpExpectedResponses[i] = 0;
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _hmExpectedResponses[i] = 0;
    }
}

void ILCResponseParser::verifyResponses() {
    double timestamp = M1M3SSPublisher::instance().getTimestamp();
    bool warn = false;
    bool anyTimeout = false;
    for (int i = 0; i < FA_COUNT; i++) {
        if (_faExpectedResponses[i] != 0) {
            warn = true;
            TG_LOG_WARN(60s, "ILCResponseParser: Force actuator #{} (ID {})  response timeout", i,
                        _forceActuatorInfo->referenceId[i]);
            _warnResponseTimeout(timestamp, _forceActuatorInfo->referenceId[i]);
            _faExpectedResponses[i] = 0;
        }
    }
    if (warn) {
        anyTimeout = true;
    }
    warn = false;
    for (int i = 0; i < HP_COUNT; i++) {
        if (_hpExpectedResponses[i] != 0) {
            warn = true;
            _warnResponseTimeout(timestamp, _hardpointActuatorInfo->referenceId[i]);
            _hpExpectedResponses[i] = 0;
            TG_LOG_WARN(60s, "ILCResponseParser: Hardpoint {} (ID {}) actuator response timeout", i + 1,
                        _hardpointActuatorInfo->referenceId[i]);
        }
    }
    if (warn) {
        anyTimeout = true;
    }
    warn = false;
    for (int i = 0; i < HP_COUNT; ++i) {
        if (_hmExpectedResponses[i] != 0) {
            warn = true;
            _warnResponseTimeout(timestamp, _hardpointMonitorInfo->referenceId[i]);
            _hmExpectedResponses[i] = 0;
            TG_LOG_WARN(60s, "ILCResponseParser: Hardpoint {} (ID {}) monitor response timeout", i + 1,
                        _hardpointMonitorInfo->referenceId[i]);
        }
    }
    if (warn) {
        anyTimeout = true;
    }

    _safetyController->ilcCommunicationTimeout(anyTimeout);
}

void ILCResponseParser::_parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId) {
    uint8_t exceptionCode = buffer->readU8();
    switch (exceptionCode) {
        case 1:
            _warnIllegalFunction(timestamp, actuatorId);
            break;
        // case 2:	break; // Illegal Data Address
        case 3:
            _warnIllegalDataValue(timestamp, actuatorId);
            break;
        // case 4: break; // Slave Device Failure
        // case 5: break; // Acknowledge
        // case 6: break; // Slave Device Busy
        // case 7: break; // Negative Acknowledge
        // case 8: break; // Memory Parity Error
        // case 10: break; // Gateway Path Unavailable
        // case 11: break; // Gateway Target Device Failed to Respond
        default:
            SPDLOG_WARN("ILCResponseParser: Actuator {:d} received exception code {:d}", actuatorId,
                        (int32_t)exceptionCode);
            _warnUnknownProblem(timestamp, actuatorId);
            break;
    }
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHPServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    uint8_t length = buffer->readU8();
    _hardpointActuatorInfo->ilcUniqueId[dataIndex] = buffer->readU48();
    _hardpointActuatorInfo->ilcApplicationType[dataIndex] = buffer->readU8();
    _hardpointActuatorInfo->networkNodeType[dataIndex] = buffer->readU8();
    _hardpointActuatorInfo->ilcSelectedOptions[dataIndex] = buffer->readU8();
    _hardpointActuatorInfo->networkNodeOptions[dataIndex] = buffer->readU8();
    _hardpointActuatorInfo->majorRevision[dataIndex] = buffer->readU8();
    _hardpointActuatorInfo->minorRevision[dataIndex] = buffer->readU8();
    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportFAServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    uint8_t length = buffer->readU8();
    _forceActuatorInfo->ilcUniqueId[dataIndex] = buffer->readU48();
    _forceActuatorInfo->ilcApplicationType[dataIndex] = buffer->readU8();
    _forceActuatorInfo->networkNodeType[dataIndex] = buffer->readU8();
    _forceActuatorInfo->ilcSelectedOptions[dataIndex] = buffer->readU8();
    _forceActuatorInfo->networkNodeOptions[dataIndex] = buffer->readU8();
    _forceActuatorInfo->majorRevision[dataIndex] = buffer->readU8();
    _forceActuatorInfo->minorRevision[dataIndex] = buffer->readU8();
    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMServerIDResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    uint8_t length = buffer->readU8();
    _hardpointMonitorInfo->ilcUniqueId[dataIndex] = buffer->readU48();
    _hardpointMonitorInfo->ilcApplicationType[dataIndex] = buffer->readU8();
    _hardpointMonitorInfo->networkNodeType[dataIndex] = buffer->readU8();
    buffer->readU8();  // ILCSelectedOptions
    buffer->readU8();  // NetworkNodeOptions
    _hardpointMonitorInfo->majorRevision[dataIndex] = buffer->readU8();
    _hardpointMonitorInfo->minorRevision[dataIndex] = buffer->readU8();
    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHPServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointActuatorState->ilcState[dataIndex] = buffer->readU8();
    HardpointActuatorWarning::instance().setIlcStatus(dataIndex, buffer->readU16(), buffer->readU16());
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportFAServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _forceActuatorState->ilcState[dataIndex] = buffer->readU8();
    M1M3SSPublisher::getForceActuatorWarning()->parseFAServerStatusResponse(buffer, dataIndex);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMServerStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointMonitorState->ilcState[dataIndex] = buffer->readU8();
    uint16_t ilcStatus = buffer->readU16();
    _hardpointMonitorWarning->majorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
    _hardpointMonitorWarning->minorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
    // 0x0004 is reserved
    _hardpointMonitorWarning->faultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
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
    _hardpointMonitorWarning->uniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
    _hardpointMonitorWarning->applicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
    _hardpointMonitorWarning->applicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
    _hardpointMonitorWarning->applicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
    _hardpointMonitorWarning->oneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
    _hardpointMonitorWarning->oneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
    _hardpointMonitorWarning->oneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
    // 0x0080 is reserved
    _hardpointMonitorWarning->watchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
    _hardpointMonitorWarning->brownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
    _hardpointMonitorWarning->eventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
    // 0x0800 is Motor Driver (HP only)
    _hardpointMonitorWarning->ssrPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
    _hardpointMonitorWarning->auxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
    // 0x4000 is SMC Power (HP only)
    // 0x8000 is reserved
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseChangeHPILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointActuatorState->ilcState[dataIndex] = buffer->readU16();
    // buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseChangeFAILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _forceActuatorState->ilcState[dataIndex] = buffer->readU16();
    // buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseChangeHMILCModeResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointMonitorState->ilcState[dataIndex] = buffer->readU16();
    // buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, ILCMap map,
                                                                      double timestamp) {
    int32_t dataIndex = map.DataIndex;
    uint8_t status = buffer->readU8();
    _hardpointActuatorData->timestamp = timestamp;
    HardpointActuatorWarning::instance().setStatus(dataIndex, timestamp, status,
                                                   _outerLoopData->broadcastCounter);
    // Encoder value needs to be swapped to keep with the theme of extension is positive
    // retaction is negative
    _hardpointActuatorData->encoder[dataIndex] =
            -buffer->readI32() + _hardpointActuatorSettings->getEncoderOffset(dataIndex);
    HardpointActuatorWarning::instance().setProximityWarning(
            dataIndex,
            _hardpointActuatorData
                    ->encoder[dataIndex]<_hardpointActuatorSettings->lowProximityEncoder[dataIndex],
                                         _hardpointActuatorData->encoder[dataIndex]>
                            _hardpointActuatorSettings->highProximityEncoder[dataIndex]);
    // Unlike the pneumatic, the electromechanical doesn't reverse compression and tension so we swap it here
    _hardpointActuatorData->measuredForce[dataIndex] = -buffer->readSGL();
    _hardpointActuatorData->displacement[dataIndex] =
            (_hardpointActuatorData->encoder[dataIndex] * _hardpointActuatorSettings->micrometersPerEncoder) /
            (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
    buffer->skipToNextFrame();
    _checkHardpointActuatorMeasuredForce(dataIndex);
}

void ILCResponseParser::_parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _forceActuatorInfo->mezzaninePrimaryCylinderGain[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->mezzanineSecondaryCylinderGain[dataIndex] = buffer->readSGL();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, ILCMap map) {
    if (address <= 16) {
        _parseSingleAxisForceDemandResponse(buffer, map);
    } else {
        _parseDualAxisForceDemandResponse(buffer, map);
    }
    _checkForceActuatorMeasuredForce(map);
    _checkForceActuatorFollowingError(map);
}

void ILCResponseParser::_parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, dataIndex,
                                                            _outerLoopData->broadcastCounter);
    _forceActuatorData->primaryCylinderForce[dataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    ForceConverter::saaToMirror(_forceActuatorData->primaryCylinderForce[dataIndex],
                                _forceActuatorData->secondaryCylinderForce[dataIndex], &x, &y, &z);
    _forceActuatorData->zForce[dataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseDualAxisForceDemandResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    int32_t secondaryDataIndex = map.SecondaryDataIndex;
    int xIndex = map.XDataIndex;
    int yIndex = map.YDataIndex;
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, dataIndex,
                                                            _outerLoopData->broadcastCounter);
    _forceActuatorData->primaryCylinderForce[dataIndex] = buffer->readSGL();
    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    switch (_forceActuatorInfo->actuatorOrientation[dataIndex]) {
        case ForceActuatorOrientations::PositiveX:
            ForceConverter::daaPositiveXToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeX:
            ForceConverter::daaNegativeXToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::PositiveY:
            ForceConverter::daaPositiveYToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeY:
            ForceConverter::daaNegativeYToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
    }
    if (xIndex != -1) {
        _forceActuatorData->xForce[xIndex] = x;
    }
    if (yIndex != -1) {
        _forceActuatorData->yForce[yIndex] = y;
    }
    _forceActuatorData->zForce[dataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address,
                                                           ILCMap map) {
    if (address <= 16) {
        _parseSingleAxisPneumaticForceStatusResponse(buffer, map);
    } else {
        _parseDualAxisPneumaticForceStatusResponse(buffer, map);
    }
    _checkForceActuatorMeasuredForce(map);
    _checkForceActuatorFollowingError(map);
}

void ILCResponseParser::_parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, dataIndex,
                                                            _outerLoopData->broadcastCounter);
    _forceActuatorData->primaryCylinderForce[dataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    ForceConverter::saaToMirror(_forceActuatorData->primaryCylinderForce[dataIndex],
                                _forceActuatorData->secondaryCylinderForce[dataIndex], &x, &y, &z);
    _forceActuatorData->zForce[dataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    int32_t secondaryDataIndex = map.SecondaryDataIndex;
    int xIndex = map.XDataIndex;
    int yIndex = map.YDataIndex;
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, dataIndex,
                                                            _outerLoopData->broadcastCounter);
    _forceActuatorData->primaryCylinderForce[dataIndex] = buffer->readSGL();
    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    switch (_forceActuatorInfo->actuatorOrientation[dataIndex]) {
        case ForceActuatorOrientations::PositiveX:
            ForceConverter::daaPositiveXToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeX:
            ForceConverter::daaNegativeXToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::PositiveY:
            ForceConverter::daaPositiveYToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeY:
            ForceConverter::daaNegativeYToMirror(
                    _forceActuatorData->primaryCylinderForce[dataIndex],
                    _forceActuatorData->secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
    }
    if (xIndex != -1) {
        _forceActuatorData->xForce[xIndex] = x;
    }
    if (yIndex != -1) {
        _forceActuatorData->yForce[yIndex] = y;
    }
    _forceActuatorData->zForce[dataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetHPADCScanRateResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointActuatorInfo->adcScanRate[dataIndex] = buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetFAADCScanRateResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _forceActuatorInfo->adcScanRate[dataIndex] = buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, ILCMap map) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseHPResetResponse(ModbusBuffer* buffer, ILCMap map) { buffer->skipToNextFrame(); }

void ILCResponseParser::_parseFAResetResponse(ModbusBuffer* buffer, ILCMap map) { buffer->skipToNextFrame(); }

void ILCResponseParser::_parseHMResetResponse(ModbusBuffer* buffer, ILCMap map) { buffer->skipToNextFrame(); }

void ILCResponseParser::_parseReadHPCalibrationResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    buffer->readSGL();  // Main Coefficient K1
    buffer->readSGL();  // Main Coefficient K2
    _hardpointActuatorInfo->mainLoadCellCoefficient[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Coefficient K4
    _hardpointActuatorInfo->mainLoadCellOffset[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Offset Channel 2
    buffer->readSGL();  // Main Offset Channel 3
    buffer->readSGL();  // Main Offset Channel 4
    _hardpointActuatorInfo->mainLoadCellSensitivity[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Sensitivity Channel 2
    buffer->readSGL();  // Main Sensitivity Channel 3
    buffer->readSGL();  // Main Sensitivity Channel 4
    buffer->readSGL();  // Backup Coefficient K1
    buffer->readSGL();  // Backup Coefficient K2
    _hardpointActuatorInfo->backupLoadCellCoefficient[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Coefficient K4
    _hardpointActuatorInfo->backupLoadCellOffset[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Offset Channel 2
    buffer->readSGL();  // Backup Offset Channel 3
    buffer->readSGL();  // Backup Offset Channel 4
    _hardpointActuatorInfo->backupLoadCellSensitivity[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Sensitivity Channel 2
    buffer->readSGL();  // Backup Sensitivity Channel 3
    buffer->readSGL();  // Backup Sensitivity Channel 4
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadFACalibrationResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _forceActuatorInfo->mainPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->mainSecondaryCylinderCoefficient[dataIndex] =
            _forceActuatorInfo->mainPrimaryCylinderCoefficient[dataIndex];
    buffer->readSGL();  // Main Coefficient K2
    buffer->readSGL();  // Main Coefficient K3
    buffer->readSGL();  // Main Coefficient K4
    _forceActuatorInfo->mainPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->mainSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Offset Channel 3
    buffer->readSGL();  // Main Offset Channel 4
    _forceActuatorInfo->mainPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->mainSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Sensitivity Channel 3
    buffer->readSGL();  // Main Sensitivity Channel 4
    _forceActuatorInfo->backupPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->backupSecondaryCylinderCoefficient[dataIndex] =
            _forceActuatorInfo->backupPrimaryCylinderCoefficient[dataIndex];
    buffer->readSGL();  // Backup Coefficient K2
    buffer->readSGL();  // Backup Coefficient K3
    buffer->readSGL();  // Backup Coefficient K4
    _forceActuatorInfo->backupPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->backupSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Offset Channel 3
    buffer->readSGL();  // Backup Offset Channel 4
    _forceActuatorInfo->backupPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    _forceActuatorInfo->backupSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Sensitivity Channel 3
    buffer->readSGL();  // Backup Sensitivity Channel 4
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, ILCMap map) {
    buffer->readSGL();
    buffer->readSGL();
    buffer->readSGL();
    buffer->readSGL();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadHMPressureValuesResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointMonitorData->pressureSensor1[dataIndex] = buffer->readSGL();
    _hardpointMonitorData->pressureSensor2[dataIndex] = buffer->readSGL();
    _hardpointMonitorData->pressureSensor3[dataIndex] = buffer->readSGL();
    _hardpointMonitorData->breakawayPressure[dataIndex] = buffer->readSGL();
    buffer->skipToNextFrame();
    _checkHardpointActuatorAirPressure(dataIndex);
}

void ILCResponseParser::_parseReportDCAIDResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _forceActuatorInfo->mezzanineUniqueId[dataIndex] = buffer->readU48();
    _forceActuatorInfo->mezzanineFirmwareType[dataIndex] = buffer->readU8();
    _forceActuatorInfo->mezzanineMajorRevision[dataIndex] = buffer->readU8();
    _forceActuatorInfo->mezzanineMinorRevision[dataIndex] = buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMMezzanineIDResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointMonitorInfo->mezzanineUniqueId[dataIndex] = buffer->readU48();
    _hardpointMonitorInfo->mezzanineFirmwareType[dataIndex] = buffer->readU8();
    _hardpointMonitorInfo->mezzanineMajorRevision[dataIndex] = buffer->readU8();
    _hardpointMonitorInfo->mezzanineMinorRevision[dataIndex] = buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportDCAStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    M1M3SSPublisher::getForceActuatorWarning()->parseDCAStatus(buffer, map.DataIndex);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMMezzanineStatusResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    uint16_t status = buffer->readU16();
    _hardpointMonitorWarning->mezzanineS1AInterface1Fault[dataIndex] = (status & 0x0001) != 0;
    _hardpointMonitorWarning->mezzanineS1ALVDT1Fault[dataIndex] = (status & 0x0002) != 0;
    _hardpointMonitorWarning->mezzanineS1AInterface2Fault[dataIndex] = (status & 0x0004) != 0;
    _hardpointMonitorWarning->mezzanineS1ALVDT2Fault[dataIndex] = (status & 0x0008) != 0;
    _hardpointMonitorWarning->mezzanineUniqueIdCRCError[dataIndex] = (status & 0x0010) != 0;
    // 0x0020 is reserved
    // 0x0040 is reserved
    // 0x0080 is reserved
    _hardpointMonitorWarning->mezzanineEventTrapReset[dataIndex] = (status & 0x0100) != 0;
    // 0x0200 is reserved
    _hardpointMonitorWarning->mezzanineDCPRS422ChipFault[dataIndex] = (status & 0x0400) != 0;
    // 0x0800 is reserved
    _hardpointMonitorWarning->mezzanineApplicationMissing[dataIndex] = (status & 0x1000) != 0;
    _hardpointMonitorWarning->mezzanineApplicationCRCMismatch[dataIndex] = (status & 0x2000) != 0;
    // 0x4000 is reserved
    _hardpointMonitorWarning->mezzanineBootloaderActive[dataIndex] = (status & 0x8000) != 0;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportLVDTResponse(ModbusBuffer* buffer, ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    _hardpointMonitorData->breakawayLVDT[dataIndex] = buffer->readSGL();
    _hardpointMonitorData->displacementLVDT[dataIndex] = buffer->readSGL();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_checkForceActuatorMeasuredForce(ILCMap map) {
    int32_t dataIndex = map.DataIndex;
    int32_t secondaryDataIndex = map.SecondaryDataIndex;
    float primaryForce = _forceActuatorData->primaryCylinderForce[dataIndex];
    float primaryLowLimit = _forceActuatorSettings->MeasuredPrimaryCylinderLimitTable[dataIndex].LowFault;
    float primaryHighLimit = _forceActuatorSettings->MeasuredPrimaryCylinderLimitTable[dataIndex].HighFault;
    bool primaryLimitWarning = primaryForce < primaryLowLimit || primaryForce > primaryHighLimit;
    bool previousPrimaryLimit = _forceWarning->primaryAxisMeasuredForceWarning[dataIndex];
    _forceWarning->primaryAxisMeasuredForceWarning[dataIndex] = primaryLimitWarning;
    bool anyChange = primaryLimitWarning != previousPrimaryLimit;

    if (secondaryDataIndex != -1) {
        float secondaryForce = _forceActuatorData->secondaryCylinderForce[secondaryDataIndex];
        float secondaryLowLimit =
                _forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].LowFault;
        float secondaryHighLimit =
                _forceActuatorSettings->MeasuredSecondaryCylinderLimitTable[secondaryDataIndex].HighFault;
        bool secondaryLimitWarning =
                secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLimit;
        bool previousSecondaryLimit = _forceWarning->secondaryAxisMeasuredForceWarning[dataIndex];
        _forceWarning->secondaryAxisMeasuredForceWarning[dataIndex] = secondaryLimitWarning;
        anyChange = (secondaryLimitWarning != previousSecondaryLimit) || anyChange;
    }

    if (anyChange) {
        _publishForceActuatorForceWarning();
    }
}

void ILCResponseParser::_checkForceActuatorFollowingError(ILCMap map) {
    // TODO: UPDATE
    int32_t dataIndex = map.DataIndex;
    int32_t secondaryDataIndex = map.SecondaryDataIndex;

    float primaryForce = _forceActuatorData->primaryCylinderForce[dataIndex];
    float primarySetpoint = _appliedCylinderForces->primaryCylinderForces[dataIndex] / 1000.0;
    float primaryLimit = _forceActuatorSettings->FollowingErrorPrimaryCylinderLimitTable[dataIndex].HighFault;
    _forceActuatorData->primaryCylinderFollowingError[dataIndex] = primaryForce - primarySetpoint;
    bool primaryLimitWarning =
            std::abs(_forceActuatorData->primaryCylinderFollowingError[dataIndex]) > primaryLimit;
    bool previousPrimaryWarning = _forceWarning->primaryAxisFollowingErrorWarning[dataIndex];
    _forceWarning->primaryAxisFollowingErrorWarning[dataIndex] = primaryLimitWarning;
    bool anyChange = primaryLimitWarning != previousPrimaryWarning;
    bool secondaryLimitWarning = false;

    if (secondaryDataIndex != -1) {
        float secondaryForce = _forceActuatorData->secondaryCylinderForce[secondaryDataIndex];
        float secondarySetpoint =
                _appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex] / 1000.0;
        float secondaryLimit =
                _forceActuatorSettings->FollowingErrorSecondaryCylinderLimitTable[secondaryDataIndex]
                        .HighFault;
        _forceActuatorData->secondaryCylinderFollowingError[secondaryDataIndex] =
                secondaryForce - secondarySetpoint;
        secondaryLimitWarning =
                std::abs(_forceActuatorData->secondaryCylinderFollowingError[secondaryDataIndex]) >
                secondaryLimit;
        bool previousSecondaryWarning = _forceWarning->secondaryAxisFollowingErrorWarning[dataIndex];
        _forceWarning->secondaryAxisFollowingErrorWarning[dataIndex] = secondaryLimitWarning;
        anyChange = anyChange || secondaryLimitWarning != previousSecondaryWarning;
    }

    _safetyController->forceActuatorFollowingError(dataIndex, primaryLimitWarning || secondaryLimitWarning);

    if (anyChange) {
        _publishForceActuatorForceWarning();
    }
}

void ILCResponseParser::_checkHardpointActuatorMeasuredForce(int32_t actuatorId) {
    float measuredForce = _hardpointActuatorData->measuredForce[actuatorId];
    float loadCellMax = _hardpointActuatorSettings->hardpointMeasuredForceFaultHigh;
    float loadCellMin = _hardpointActuatorSettings->hardpointMeasuredForceFaultLow;
    bool loadCellError = measuredForce > loadCellMax || measuredForce < loadCellMin;
    _safetyController->hardpointActuatorLoadCellError(loadCellError);

    // As soon as mirror is at least a bit raised, tests shall be performed
    // this is software line of defense for excessive forces. Hardpoints shall
    // break if excess force is applied (either compression or tension),
    // protecting the mirror from damage.
    if (_forceActuatorState->supportPercentage > 0) {
        float maxWarningForce = _hardpointActuatorSettings->hardpointMeasuredForceWarningHigh;
        float minWarningForce = _hardpointActuatorSettings->hardpointMeasuredForceWarningLow;
        if (_forceActuatorState->balanceForcesApplied) {
            maxWarningForce = _hardpointActuatorSettings->hardpointMeasuredForceFSBWarningHigh;
            minWarningForce = _hardpointActuatorSettings->hardpointMeasuredForceFSBWarningLow;
        }
        bool measuredForceWarning = measuredForce > maxWarningForce || measuredForce < minWarningForce;

        float maxFaultForce = _hardpointActuatorSettings->hardpointMeasuredForceFaultHigh;
        float minFaultForce = _hardpointActuatorSettings->hardpointMeasuredForceFaultLow;
        bool measuredForceFault = measuredForce > maxFaultForce || measuredForce < minFaultForce;

        _safetyController->hardpointActuatorMeasuredForce(actuatorId, measuredForceWarning,
                                                          measuredForceFault);
    } else {
        _safetyController->hardpointActuatorMeasuredForce(actuatorId, false, false);
    }
}

void ILCResponseParser::_checkHardpointActuatorAirPressure(int32_t actuatorId) {
    float airPressure = _hardpointMonitorData->breakawayPressure[actuatorId];
    float minPressure = _hardpointActuatorSettings->airPressureFaultLow;
    float maxPressure = _hardpointActuatorSettings->airPressureFaultHigh;
    int loadCellError = 0;
    switch (_detailedState->detailedState) {
        case MTM1M3::MTM1M3_shared_DetailedStates_RaisingState:
        case MTM1M3::MTM1M3_shared_DetailedStates_RaisingEngineeringState:
            minPressure = _hardpointActuatorSettings->airPressureFaultLowRaising;
        case MTM1M3::MTM1M3_shared_DetailedStates_ActiveEngineeringState:
        case MTM1M3::MTM1M3_shared_DetailedStates_ActiveState:
        case MTM1M3::MTM1M3_shared_DetailedStates_LoweringState:
        case MTM1M3::MTM1M3_shared_DetailedStates_LoweringEngineeringState:
            if (airPressure < minPressure) loadCellError = -1;
            if (airPressure > maxPressure) loadCellError = 1;
            _safetyController->hardpointActuatorAirPressure(actuatorId, loadCellError, airPressure);
            break;
        default:
            _safetyController->hardpointActuatorAirPressure(actuatorId, 0, airPressure);
            break;
    }
}

void ILCResponseParser::_publishForceActuatorForceWarning() {
    _forceWarning->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _forceWarning->anyPrimaryAxisMeasuredForceWarning = false;
    _forceWarning->anySecondaryAxisMeasuredForceWarning = false;
    _forceWarning->anyPrimaryAxisFollowingErrorWarning = false;
    _forceWarning->anySecondaryAxisFollowingErrorWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        _forceWarning->anyPrimaryAxisMeasuredForceWarning =
                _forceWarning->anyPrimaryAxisMeasuredForceWarning ||
                _forceWarning->primaryAxisMeasuredForceWarning[i];
        _forceWarning->anySecondaryAxisMeasuredForceWarning =
                _forceWarning->anySecondaryAxisMeasuredForceWarning ||
                _forceWarning->secondaryAxisMeasuredForceWarning[i];
        _forceWarning->anyPrimaryAxisFollowingErrorWarning =
                _forceWarning->anyPrimaryAxisFollowingErrorWarning ||
                _forceWarning->primaryAxisFollowingErrorWarning[i];
        _forceWarning->anySecondaryAxisFollowingErrorWarning =
                _forceWarning->anySecondaryAxisFollowingErrorWarning ||
                _forceWarning->secondaryAxisFollowingErrorWarning[i];
    }
    _forceWarning->anyWarning = _forceWarning->anyPrimaryAxisMeasuredForceWarning ||
                                _forceWarning->anySecondaryAxisMeasuredForceWarning ||
                                _forceWarning->anyPrimaryAxisFollowingErrorWarning ||
                                _forceWarning->anySecondaryAxisFollowingErrorWarning;
    M1M3SSPublisher::instance().logForceActuatorForceWarning();
}

void ILCResponseParser::_warnResponseTimeout(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = true;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnInvalidCRC(double timestamp) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = -1;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = true;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnIllegalFunction(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = true;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnIllegalDataValue(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = true;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnInvalidLength(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = true;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnUnknownSubnet(double timestamp) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = -1;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = true;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnUnknownAddress(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = true;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnUnknownFunction(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = true;
    _ilcWarning->unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning();
}

void ILCResponseParser::_warnUnknownProblem(double timestamp, int32_t actuatorId) {
    _ilcWarning->timestamp = timestamp;
    _ilcWarning->actuatorId = actuatorId;
    _ilcWarning->responseTimeout = false;
    _ilcWarning->invalidCRC = false;
    _ilcWarning->illegalFunction = false;
    _ilcWarning->illegalDataValue = false;
    _ilcWarning->invalidLength = false;
    _ilcWarning->unknownSubnet = false;
    _ilcWarning->unknownAddress = false;
    _ilcWarning->unknownFunction = false;
    _ilcWarning->unknownProblem = true;
    M1M3SSPublisher::instance().logILCWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
