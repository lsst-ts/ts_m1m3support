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

#include "ILCWarning.h"
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorData.h>
#include <ForceActuatorForceWarning.h>
#include <ForceActuatorInfo.h>
#include <ForceActuatorSettings.h>
#include <ForceControllerState.h>
#include <ForceConverter.h>
#include <HardpointActuatorSettings.h>
#include <HardpointActuatorWarning.h>
#include <ILCDataTypes.h>
#include <ILCResponseParser.h>
#include <ILCSubnetData.h>
#include <LimitLog.h>
#include <M1M3SSPublisher.h>
#include <ModbusBuffer.h>
#include <RaisingLoweringInfo.h>
#include <SafetyController.h>
#include <Timestamp.h>

using namespace LSST::M1M3::SS;

ILCResponseParser::ILCResponseParser() {
    _subnetData = 0;
    _safetyController = 0;
    _hardpointActuatorInfo = 0;
    _hardpointActuatorState = 0;
    _hardpointActuatorData = 0;
    _forceActuatorState = 0;
    _appliedCylinderForces = 0;
    _hardpointMonitorInfo = 0;
    _hardpointMonitorState = 0;
    _hardpointMonitorWarning = 0;
    _hardpointMonitorData = 0;
    _outerLoopData = 0;
    _detailedState = 0;
}

ILCResponseParser::ILCResponseParser(ILCSubnetData *subnetData, SafetyController *safetyController) {
    SPDLOG_DEBUG("ILCResponseParser: ILCResponseParser()");
    _hardpointActuatorSettings = &HardpointActuatorSettings::instance();
    _subnetData = subnetData;
    _safetyController = safetyController;
    _hardpointActuatorInfo = M1M3SSPublisher::instance().getEventHardpointActuatorInfo();
    _hardpointActuatorState = M1M3SSPublisher::instance().getEventHardpointActuatorState();
    _hardpointActuatorData = M1M3SSPublisher::instance().getHardpointActuatorData();
    _forceActuatorState = M1M3SSPublisher::instance().getEventForceActuatorState();
    _appliedCylinderForces = M1M3SSPublisher::instance().getAppliedCylinderForces();
    _hardpointMonitorInfo = M1M3SSPublisher::instance().getEventHardpointMonitorInfo();
    _hardpointMonitorState = M1M3SSPublisher::instance().getEventHardpointMonitorState();
    _hardpointMonitorWarning = M1M3SSPublisher::instance().getEventHardpointMonitorWarning();
    _hardpointMonitorData = M1M3SSPublisher::instance().getHardpointMonitorData();
    _outerLoopData = M1M3SSPublisher::instance().getOuterLoopData();
    _detailedState = M1M3SSPublisher::instance().getEventDetailedState();

    ForceActuatorForceWarning::instance().reset();

    memset(_faExpectedResponses, 0, sizeof(_faExpectedResponses));
    memset(_hpExpectedResponses, 0, sizeof(_hpExpectedResponses));
    memset(_hmExpectedResponses, 0, sizeof(_hmExpectedResponses));
}

bool validateCRC(ModbusBuffer *buffer, uint16_t *length, double *timestamp, uint16_t &receivedCRC,
                 uint16_t &calculatedCRC) {
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

void ILCResponseParser::parse(ModbusBuffer *buffer, uint8_t subnet) {
    uint64_t a = buffer->readLength();
    uint64_t b = buffer->readLength();
    uint64_t c = buffer->readLength();
    uint64_t d = buffer->readLength();
    double globalTimestamp = Timestamp::fromRaw((d << 48) | (c << 32) | (b << 16) | a);
    _forceActuatorState->timestamp = globalTimestamp;
    M1M3SSPublisher::getForceActuatorWarning()->setTimestamp(globalTimestamp);
    ForceActuatorForceWarning::instance().setTimestamp(globalTimestamp);
    ForceActuatorData::instance().timestamp = globalTimestamp;
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
                        "ILCResponseParser: Invalid CRC (or extra byte) on subnet "
                        "{:d} - received {:04X}, "
                        "calculated {:04X}, "
                        "address {:02X}, function {:02X}, data [{}]",
                        subnet, receivedCRC, calculatedCRC, data[0], data[1], data_buf.str());
            ILCWarning::instance().warnInvalidCRC(timestamp);
        } else {
            if (subnet >= 1 && subnet <= 5) {
                uint8_t address = buffer->readU8();
                uint8_t called_function = buffer->readU8();
                const ILCMap &ilc = _subnetData->getILCDataFromAddress(subnet - 1, address);
                switch (ilc.Type) {
                    case ILCTypes::FA:
                        _faExpectedResponses[ilc.DataIndex]--;
                        switch (called_function) {
                            case 17:
                                ForceActuatorInfo::instance().parseServerIDResponse(buffer, ilc);
                                break;
                            case 18:
                                _parseReportFAServerStatusResponse(buffer, ilc);
                                break;
                            case 65:
                                _parseChangeFAILCModeResponse(buffer, ilc);
                                break;
                            case 73:
                                _parseSetBoostValveDCAGainsResponse(buffer, ilc);
                                break;
                            case 74:
                                ForceActuatorInfo::instance().parseBoosterValveDCAGains(buffer, ilc);
                                break;
                            case 75:
                                _parseForceDemandResponse(buffer, address, ilc);
                                break;
                            case 76:
                                _parsePneumaticForceStatusResponse(buffer, address, ilc);
                                break;
                            case 80:
                                ForceActuatorInfo::instance().parseFAADCScanRate(buffer, ilc);
                                break;
                            case 81:
                                _parseSetFAADCChannelOffsetAndSensitivityResponse(buffer, ilc);
                                break;
                            case 107:
                                _parseFAResetResponse(buffer, ilc);
                                break;
                            case 110:
                                ForceActuatorInfo::instance().parseFACalibration(buffer, ilc);
                                break;
                            case 119:
                                _parseReadDCAPressureValuesResponse(buffer, ilc);
                                break;
                            case 120:
                                ForceActuatorInfo::instance().parseSetDCAID(buffer, ilc);
                                break;
                            case 121:
                                _parseReportDCAStatusResponse(buffer, ilc);
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
                                _parseErrorResponse(buffer, called_function, timestamp, ilc.ActuatorId);
                                break;
                            default:
                                SPDLOG_WARN(
                                        "ILCResponseParser: Unknown FA function on subnet {:d} "
                                        "function "
                                        "{:d}",
                                        (int)called_function, subnet);
                                ILCWarning::instance().warnUnknownFunction(timestamp, ilc.ActuatorId);
                                break;
                        }
                        break;
                    case ILCTypes::HP:
                        _hpExpectedResponses[ilc.DataIndex]--;
                        switch (called_function) {
                            case 17:
                                _parseReportHPServerIDResponse(buffer, ilc);
                                break;
                            case 18:
                                _parseReportHPServerStatusResponse(buffer, ilc);
                                break;
                            case 65:
                                _parseChangeHPILCModeResponse(buffer, ilc);
                                break;
                            case 66:
                            case 67:
                                _parseElectromechanicalForceAndStatusResponse(buffer, ilc, timestamp);
                                break;
                            case 80:
                                _parseSetHPADCScanRateResponse(buffer, ilc);
                                break;
                            case 81:
                                _parseSetHPADCChannelOffsetAndSensitivityResponse(buffer, ilc);
                                break;
                            case 107:
                                _parseHPResetResponse(buffer, ilc);
                                break;
                            case 110:
                                _parseReadHPCalibrationResponse(buffer, ilc);
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
                                _parseErrorResponse(buffer, called_function, timestamp, ilc.ActuatorId);
                                break;
                            default:
                                SPDLOG_WARN("ILCResponseParser: Unknown HP function {:d} on subnet {:d}",
                                            (int)called_function, subnet);
                                ILCWarning::instance().warnUnknownFunction(timestamp, ilc.ActuatorId);
                                break;
                        }
                        break;
                    case ILCTypes::HM:
                        _hmExpectedResponses[ilc.DataIndex]--;
                        switch (called_function) {
                            case 17:
                                _parseReportHMServerIDResponse(buffer, ilc);
                                break;
                            case 18:
                                _parseReportHMServerStatusResponse(buffer, ilc);
                                break;
                            case 65:
                                _parseChangeHMILCModeResponse(buffer, ilc);
                                break;
                            case 107:
                                _parseHMResetResponse(buffer, ilc);
                                break;
                            case 119:
                                _parseReadHMPressureValuesResponse(buffer, ilc);
                                break;
                            case 120:
                                _parseReportHMMezzanineIDResponse(buffer, ilc);
                                break;
                            case 121:
                                _parseReportHMMezzanineStatusResponse(buffer, ilc);
                                break;
                            case 122:
                                _parseReportLVDTResponse(buffer, ilc);
                                break;
                            case 145:
                            case 146:
                            case 193:
                            case 235:
                            case 247:
                            case 248:
                            case 249:
                            case 250:
                                _parseErrorResponse(buffer, called_function, timestamp, ilc.ActuatorId);
                                break;
                            default:
                                SPDLOG_WARN("ILCResponseParser: Unknown HM function {:d} on subnet {:d}",
                                            (int)called_function, subnet);
                                ILCWarning::instance().warnUnknownFunction(timestamp, ilc.ActuatorId);
                                break;
                        }
                        break;
                    default:
                        SPDLOG_WARN(
                                "ILCResponseParser: Unknown address {:d} on subnet {:d} "
                                "for function "
                                "code {:d}",
                                (int)address, (int)subnet, (int)called_function);
                        ILCWarning::instance().warnUnknownAddress(timestamp, ilc.ActuatorId);
                        break;
                }
            } else {
                SPDLOG_WARN("ILCResponseParser: Unknown subnet {:d}", subnet);
                ILCWarning::instance().warnUnknownSubnet(timestamp);
            }
        }
    }
    M1M3SSPublisher::getForceActuatorWarning()->log();
}

void ILCResponseParser::incExpectedResponses(int32_t *fa, int32_t *hp, int32_t *hm) {
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
            auto &_forceActuatorInfo = ForceActuatorInfo::instance();
            TG_LOG_WARN(60s, "ILCResponseParser: Force actuator #{} (ID {})  response timeout", i,
                        _forceActuatorInfo.referenceId[i]);
            ILCWarning::instance().warnResponseTimeout(timestamp, _forceActuatorInfo.referenceId[i]);
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
            ILCWarning::instance().warnResponseTimeout(timestamp, _hardpointActuatorInfo->referenceId[i]);
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
            ILCWarning::instance().warnResponseTimeout(timestamp, _hardpointMonitorInfo->referenceId[i]);
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

void ILCResponseParser::_parseErrorResponse(ModbusBuffer *buffer, uint8_t called_function, double timestamp,
                                            int32_t actuatorId) {
    uint8_t exceptionCode = buffer->readU8();
    SPDLOG_WARN("ILC Error response received - actuator {}, function {} ({}), code {}", actuatorId,
                called_function & ~0x80, called_function, exceptionCode);
    switch (exceptionCode) {
        case 1:
            ILCWarning::instance().warnIllegalFunction(timestamp, actuatorId);
            break;
        // case 2:	break; // Illegal Data Address
        case 3:
            ILCWarning::instance().warnIllegalDataValue(timestamp, actuatorId);
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
            ILCWarning::instance().warnUnknownProblem(timestamp, actuatorId);
            break;
    }
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHPServerIDResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    uint8_t length = buffer->readU8();
    _hardpointActuatorInfo->ilcUniqueId[ilc.DataIndex] = buffer->readU48();
    _hardpointActuatorInfo->ilcApplicationType[ilc.DataIndex] = buffer->readU8();
    _hardpointActuatorInfo->networkNodeType[ilc.DataIndex] = buffer->readU8();
    _hardpointActuatorInfo->ilcSelectedOptions[ilc.DataIndex] = buffer->readU8();
    _hardpointActuatorInfo->networkNodeOptions[ilc.DataIndex] = buffer->readU8();
    _hardpointActuatorInfo->majorRevision[ilc.DataIndex] = buffer->readU8();
    _hardpointActuatorInfo->minorRevision[ilc.DataIndex] = buffer->readU8();
    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMServerIDResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    uint8_t length = buffer->readU8();
    _hardpointMonitorInfo->ilcUniqueId[ilc.DataIndex] = buffer->readU48();
    _hardpointMonitorInfo->ilcApplicationType[ilc.DataIndex] = buffer->readU8();
    _hardpointMonitorInfo->networkNodeType[ilc.DataIndex] = buffer->readU8();
    buffer->readU8();  // ILCSelectedOptions
    buffer->readU8();  // NetworkNodeOptions
    _hardpointMonitorInfo->majorRevision[ilc.DataIndex] = buffer->readU8();
    _hardpointMonitorInfo->minorRevision[ilc.DataIndex] = buffer->readU8();
    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHPServerStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointActuatorState->ilcState[ilc.DataIndex] = buffer->readU8();
    HardpointActuatorWarning::instance().parseIlcStatus(buffer, ilc.DataIndex);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportFAServerStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _forceActuatorState->ilcState[ilc.DataIndex] = buffer->readU8();
    M1M3SSPublisher::getForceActuatorWarning()->parseFAServerStatusResponse(buffer, ilc.DataIndex);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMServerStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointMonitorState->ilcState[ilc.DataIndex] = buffer->readU8();
    uint16_t ilcStatus = buffer->readU16();
    _hardpointMonitorWarning->majorFault[ilc.DataIndex] = (ilcStatus & 0x0001) != 0;
    _hardpointMonitorWarning->minorFault[ilc.DataIndex] = (ilcStatus & 0x0002) != 0;
    // 0x0004 is reserved
    _hardpointMonitorWarning->faultOverride[ilc.DataIndex] = (ilcStatus & 0x0008) != 0;
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
    _hardpointMonitorWarning->uniqueIdCRCError[ilc.DataIndex] = (ilcFaults & 0x0001) != 0;
    _hardpointMonitorWarning->applicationTypeMismatch[ilc.DataIndex] = (ilcFaults & 0x0002) != 0;
    _hardpointMonitorWarning->applicationMissing[ilc.DataIndex] = (ilcFaults & 0x0004) != 0;
    _hardpointMonitorWarning->applicationCRCMismatch[ilc.DataIndex] = (ilcFaults & 0x0008) != 0;
    _hardpointMonitorWarning->oneWireMissing[ilc.DataIndex] = (ilcFaults & 0x0010) != 0;
    _hardpointMonitorWarning->oneWire1Mismatch[ilc.DataIndex] = (ilcFaults & 0x0020) != 0;
    _hardpointMonitorWarning->oneWire2Mismatch[ilc.DataIndex] = (ilcFaults & 0x0040) != 0;
    // 0x0080 is reserved
    _hardpointMonitorWarning->watchdogReset[ilc.DataIndex] = (ilcFaults & 0x0100) != 0;
    _hardpointMonitorWarning->brownOut[ilc.DataIndex] = (ilcFaults & 0x0200) != 0;
    _hardpointMonitorWarning->eventTrapReset[ilc.DataIndex] = (ilcFaults & 0x0400) != 0;
    // 0x0800 is Motor Driver (HP only)
    _hardpointMonitorWarning->ssrPowerFault[ilc.DataIndex] = (ilcFaults & 0x1000) != 0;
    _hardpointMonitorWarning->auxPowerFault[ilc.DataIndex] = (ilcFaults & 0x2000) != 0;
    // 0x4000 is SMC Power (HP only)
    // 0x8000 is reserved
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseChangeHPILCModeResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointActuatorState->ilcState[ilc.DataIndex] = buffer->readU16();
    // buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseChangeFAILCModeResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _forceActuatorState->ilcState[ilc.DataIndex] = buffer->readU16();
    // buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseChangeHMILCModeResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointMonitorState->ilcState[ilc.DataIndex] = buffer->readU16();
    // buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseElectromechanicalForceAndStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc,
                                                                      double timestamp) {
    uint8_t status = buffer->readU8();
    _hardpointActuatorData->timestamp = timestamp;
    HardpointActuatorWarning::instance().setStatus(ilc.DataIndex, timestamp, status,
                                                   _outerLoopData->broadcastCounter);
    // Encoder value needs to be swapped to keep with the theme of extension is
    // positive retraction is negative
    _hardpointActuatorData->encoder[ilc.DataIndex] =
            -buffer->readI32() + _hardpointActuatorSettings->getEncoderOffset(ilc.DataIndex);
    HardpointActuatorWarning::instance().setProximityWarning(
            ilc.DataIndex,
            _hardpointActuatorData
                    ->encoder[ilc.DataIndex]<_hardpointActuatorSettings->lowProximityEncoder[ilc.DataIndex],
                                             _hardpointActuatorData->encoder[ilc.DataIndex]>
                            _hardpointActuatorSettings->highProximityEncoder[ilc.DataIndex]);
    // Unlike the pneumatic, the electromechanical doesn't reverse compression and
    // tension so we swap it here
    _hardpointActuatorData->measuredForce[ilc.DataIndex] = -buffer->readSGL();
    _hardpointActuatorData->displacement[ilc.DataIndex] =
            (_hardpointActuatorData->encoder[ilc.DataIndex] *
             _hardpointActuatorSettings->micrometersPerEncoder) /
            (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER);
    buffer->skipToNextFrame();
    _checkHardpointActuatorMeasuredForce(ilc.DataIndex);
}

void ILCResponseParser::_parseSetBoostValveDCAGainsResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseForceDemandResponse(ModbusBuffer *buffer, uint8_t address, const ILCMap &ilc) {
    if (address <= 16) {
        _parseSingleAxisForceDemandResponse(buffer, ilc);
    } else {
        _parseDualAxisForceDemandResponse(buffer, ilc);
    }
    _checkForceActuatorForces(ilc);
}

void ILCResponseParser::_parseSingleAxisForceDemandResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, ilc.DataIndex,
                                                            _outerLoopData->broadcastCounter);
    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex];
    ForceConverter::saaToMirror(ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                                ForceActuatorData::instance().secondaryCylinderForce[ilc.DataIndex], &x, &y,
                                &z);
    ForceActuatorData::instance().zForce[ilc.DataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseDualAxisForceDemandResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    int32_t secondaryDataIndex = ilc.SecondaryDataIndex;
    int xIndex = ilc.XDataIndex;
    int yIndex = ilc.YDataIndex;
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, ilc.DataIndex,
                                                            _outerLoopData->broadcastCounter);
    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex] = buffer->readSGL();
    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    switch (ForceActuatorInfo::instance().actuatorOrientation[ilc.DataIndex]) {
        case ForceActuatorOrientations::PositiveX:
            ForceConverter::daaPositiveXToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeX:
            ForceConverter::daaNegativeXToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::PositiveY:
            ForceConverter::daaPositiveYToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeY:
            ForceConverter::daaNegativeYToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
    }
    if (xIndex != -1) {
        ForceActuatorData::instance().xForce[xIndex] = x;
    }
    if (yIndex != -1) {
        ForceActuatorData::instance().yForce[yIndex] = y;
    }
    ForceActuatorData::instance().zForce[ilc.DataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parsePneumaticForceStatusResponse(ModbusBuffer *buffer, uint8_t address,
                                                           const ILCMap &ilc) {
    if (address <= 16) {
        _parseSingleAxisPneumaticForceStatusResponse(buffer, ilc);
    } else {
        _parseDualAxisPneumaticForceStatusResponse(buffer, ilc);
    }
    _checkForceActuatorForces(ilc);
}

void ILCResponseParser::_parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer *buffer,
                                                                     const ILCMap &ilc) {
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, ilc.DataIndex,
                                                            _outerLoopData->broadcastCounter);
    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    ForceConverter::saaToMirror(ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                                ForceActuatorData::instance().secondaryCylinderForce[ilc.DataIndex], &x, &y,
                                &z);
    ForceActuatorData::instance().zForce[ilc.DataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseDualAxisPneumaticForceStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    int32_t secondaryDataIndex = ilc.SecondaryDataIndex;
    int xIndex = ilc.XDataIndex;
    int yIndex = ilc.YDataIndex;
    M1M3SSPublisher::getForceActuatorWarning()->parseStatus(buffer, ilc.DataIndex,
                                                            _outerLoopData->broadcastCounter);
    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex] = buffer->readSGL();
    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex] = buffer->readSGL();
    float x = 0;
    float y = 0;
    float z = 0;
    switch (ForceActuatorInfo::instance().actuatorOrientation[ilc.DataIndex]) {
        case ForceActuatorOrientations::PositiveX:
            ForceConverter::daaPositiveXToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeX:
            ForceConverter::daaNegativeXToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::PositiveY:
            ForceConverter::daaPositiveYToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
        case ForceActuatorOrientations::NegativeY:
            ForceConverter::daaNegativeYToMirror(
                    ForceActuatorData::instance().primaryCylinderForce[ilc.DataIndex],
                    ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex], &x, &y, &z);
            break;
    }
    if (xIndex != -1) {
        ForceActuatorData::instance().xForce[xIndex] = x;
    }
    if (yIndex != -1) {
        ForceActuatorData::instance().yForce[yIndex] = y;
    }
    ForceActuatorData::instance().zForce[ilc.DataIndex] = z;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetHPADCScanRateResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointActuatorInfo->adcScanRate[ilc.DataIndex] = buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer *buffer,
                                                                          const ILCMap &ilc) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer *buffer,
                                                                          const ILCMap &ilc) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseHPResetResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseFAResetResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseHMResetResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadHPCalibrationResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    buffer->readSGL();  // Main Coefficient K1
    buffer->readSGL();  // Main Coefficient K2
    _hardpointActuatorInfo->mainLoadCellCoefficient[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Coefficient K4
    _hardpointActuatorInfo->mainLoadCellOffset[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Offset Channel 2
    buffer->readSGL();  // Main Offset Channel 3
    buffer->readSGL();  // Main Offset Channel 4
    _hardpointActuatorInfo->mainLoadCellSensitivity[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Sensitivity Channel 2
    buffer->readSGL();  // Main Sensitivity Channel 3
    buffer->readSGL();  // Main Sensitivity Channel 4
    buffer->readSGL();  // Backup Coefficient K1
    buffer->readSGL();  // Backup Coefficient K2
    _hardpointActuatorInfo->backupLoadCellCoefficient[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Coefficient K4
    _hardpointActuatorInfo->backupLoadCellOffset[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Offset Channel 2
    buffer->readSGL();  // Backup Offset Channel 3
    buffer->readSGL();  // Backup Offset Channel 4
    _hardpointActuatorInfo->backupLoadCellSensitivity[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Sensitivity Channel 2
    buffer->readSGL();  // Backup Sensitivity Channel 3
    buffer->readSGL();  // Backup Sensitivity Channel 4
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadDCAPressureValuesResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    buffer->readSGL();
    buffer->readSGL();
    buffer->readSGL();
    buffer->readSGL();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReadHMPressureValuesResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointMonitorData->pressureSensor1[ilc.DataIndex] = buffer->readSGL();
    _hardpointMonitorData->pressureSensor2[ilc.DataIndex] = buffer->readSGL();
    _hardpointMonitorData->pressureSensor3[ilc.DataIndex] = buffer->readSGL();
    _hardpointMonitorData->breakawayPressure[ilc.DataIndex] = buffer->readSGL();
    buffer->skipToNextFrame();
    _checkHardpointActuatorAirPressure(ilc.DataIndex);
}

void ILCResponseParser::_parseReportHMMezzanineIDResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointMonitorInfo->mezzanineUniqueId[ilc.DataIndex] = buffer->readU48();
    _hardpointMonitorInfo->mezzanineFirmwareType[ilc.DataIndex] = buffer->readU8();
    _hardpointMonitorInfo->mezzanineMajorRevision[ilc.DataIndex] = buffer->readU8();
    _hardpointMonitorInfo->mezzanineMinorRevision[ilc.DataIndex] = buffer->readU8();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportDCAStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    M1M3SSPublisher::getForceActuatorWarning()->parseDCAStatus(buffer, ilc.DataIndex);
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportHMMezzanineStatusResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    uint16_t status = buffer->readU16();
    _hardpointMonitorWarning->mezzanineS1AInterface1Fault[ilc.DataIndex] = (status & 0x0001) != 0;
    _hardpointMonitorWarning->mezzanineS1ALVDT1Fault[ilc.DataIndex] = (status & 0x0002) != 0;
    _hardpointMonitorWarning->mezzanineS1AInterface2Fault[ilc.DataIndex] = (status & 0x0004) != 0;
    _hardpointMonitorWarning->mezzanineS1ALVDT2Fault[ilc.DataIndex] = (status & 0x0008) != 0;
    _hardpointMonitorWarning->mezzanineUniqueIdCRCError[ilc.DataIndex] = (status & 0x0010) != 0;
    // 0x0020 is reserved
    // 0x0040 is reserved
    // 0x0080 is reserved
    _hardpointMonitorWarning->mezzanineEventTrapReset[ilc.DataIndex] = (status & 0x0100) != 0;
    // 0x0200 is reserved
    _hardpointMonitorWarning->mezzanineDCPRS422ChipFault[ilc.DataIndex] = (status & 0x0400) != 0;
    // 0x0800 is reserved
    _hardpointMonitorWarning->mezzanineApplicationMissing[ilc.DataIndex] = (status & 0x1000) != 0;
    _hardpointMonitorWarning->mezzanineApplicationCRCMismatch[ilc.DataIndex] = (status & 0x2000) != 0;
    // 0x4000 is reserved
    _hardpointMonitorWarning->mezzanineBootloaderActive[ilc.DataIndex] = (status & 0x8000) != 0;
    buffer->skipToNextFrame();
}

void ILCResponseParser::_parseReportLVDTResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    _hardpointMonitorData->breakawayLVDT[ilc.DataIndex] = buffer->readSGL();
    _hardpointMonitorData->displacementLVDT[ilc.DataIndex] = buffer->readSGL();
    buffer->skipToNextFrame();
}

void ILCResponseParser::_checkForceActuatorForces(const ILCMap &ilc) {
    auto &faData = ForceActuatorData::instance();

    float primaryForce = faData.primaryCylinderForce[ilc.DataIndex];
    float primarySetpoint = _appliedCylinderForces->primaryCylinderForces[ilc.DataIndex] / 1000.0f;

    auto &fafWarning = ForceActuatorForceWarning::instance();

    fafWarning.checkPrimary(ilc.DataIndex, ilc.ActuatorId, primaryForce, primarySetpoint);

    int32_t secondaryDataIndex = ilc.SecondaryDataIndex;

    bool countingWarning = fafWarning.primaryAxisFollowingErrorCountingFault[ilc.DataIndex];
    bool immediateFault = fafWarning.primaryAxisFollowingErrorImmediateFault[ilc.DataIndex];

    if (secondaryDataIndex != -1) {
        float secondaryForce = ForceActuatorData::instance().secondaryCylinderForce[secondaryDataIndex];
        float secondarySetpoint =
                _appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex] / 1000.0f;

        fafWarning.checkSecondary(secondaryDataIndex, ilc.ActuatorId, secondaryForce, secondarySetpoint);

        countingWarning =
                countingWarning || fafWarning.secondaryAxisFollowingErrorCountingFault[secondaryDataIndex];
        immediateFault =
                immediateFault || fafWarning.secondaryAxisFollowingErrorImmediateFault[secondaryDataIndex];
    }

    // In disabled state, CSC cannot be controlled. So following error can be
    // high. We will ignore following error calculations until ILC is enabled
    // and commanded forces are send in.
    if (_detailedState->detailedState != MTM1M3::MTM1M3_shared_DetailedStates_DisabledState) {
        _safetyController->forceActuatorFollowingError(ilc.ActuatorId, ilc.DataIndex, countingWarning,
                                                       immediateFault);
    }

    float zForce = faData.zForce[ilc.DataIndex];
    bool zFaulted = fafWarning.checkZMeasuredForce(ilc.DataIndex, ilc.ActuatorId, zForce);
    _safetyController->forceControllerNotifyMeasuredZForceLimit(ilc.ActuatorId, zForce, zFaulted);

    auto yIndex = ilc.YDataIndex;
    if (yIndex >= 0) {
        float yForce = faData.yForce[yIndex];
        bool yFaulted = fafWarning.checkYMeasuredForce(yIndex, ilc.ActuatorId, yForce);
        _safetyController->forceControllerNotifyMeasuredYForceLimit(ilc.ActuatorId, yForce, yFaulted);
    }

    auto xIndex = ilc.XDataIndex;
    if (xIndex >= 0) {
        float xForce = faData.xForce[xIndex];
        bool xFaulted = fafWarning.checkXMeasuredForce(xIndex, ilc.ActuatorId, xForce);
        _safetyController->forceControllerNotifyMeasuredXForceLimit(ilc.ActuatorId, xForce, xFaulted);
    }
}

void ILCResponseParser::_checkHardpointActuatorMeasuredForce(int32_t actuatorId) {
    float measuredForce = _hardpointActuatorData->measuredForce[actuatorId];

    float breakawayMax = _hardpointActuatorSettings->hardpointBreakawayFaultHigh;
    float breakawayMin = _hardpointActuatorSettings->hardpointBreakawayFaultLow;
    bool breakawayFault = measuredForce > breakawayMax || measuredForce < breakawayMin;
    _safetyController->hardpointActuatorBreakawayFault(actuatorId, breakawayFault);

    // As soon as mirror is at least a bit raised, tests shall be performed
    // this is software line of defense for excessive forces. Hardpoints shall
    // break if excess force is applied (either compression or tension),
    // protecting the mirror from damage.
    if (RaisingLoweringInfo::instance().weightSupportedPercent > 0) {
        float maxWarningForce = _hardpointActuatorSettings->hardpointMeasuredForceWarningHigh;
        float minWarningForce = _hardpointActuatorSettings->hardpointMeasuredForceWarningLow;
        if (ForceControllerState::instance().balanceForcesApplied) {
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
    int pressureError = 0;
    switch (_detailedState->detailedState) {
        case MTM1M3::MTM1M3_shared_DetailedStates_RaisingState:
        case MTM1M3::MTM1M3_shared_DetailedStates_RaisingEngineeringState:
            minPressure = _hardpointActuatorSettings->airPressureFaultLowRaising;
            // don't fault if we are waiting for pressure to build up
            if (RaisingLoweringInfo::instance().waitAirPressure == true) break;
        case MTM1M3::MTM1M3_shared_DetailedStates_ActiveEngineeringState:
        case MTM1M3::MTM1M3_shared_DetailedStates_ActiveState:
        case MTM1M3::MTM1M3_shared_DetailedStates_LoweringState:
        case MTM1M3::MTM1M3_shared_DetailedStates_LoweringEngineeringState:
            if (airPressure < minPressure)
                pressureError = -1;
            else if (airPressure > maxPressure)
                pressureError = 1;
        default:
            break;
    }
    _safetyController->hardpointActuatorAirPressure(actuatorId, pressureError, airPressure);

    HardpointActuatorWarning::instance().setAirPressure(actuatorId, (airPressure < minPressure),
                                                        (airPressure > maxPressure), airPressure);
}
