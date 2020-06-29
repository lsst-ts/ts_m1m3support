/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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

#include <ILC.h>
#include <M1M3SSPublisher.h>
#include <DataTypes.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <iostream>
#include <unistd.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <Timestamp.h>
#include <cstring>
#include <IBusList.h>
#include <SAL_MTM1M3C.h>
#include <ForceConverter.h>
#include <spdlog/spdlog.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <RoundRobin.h>
#include <PositionController.h>
#include <cmath>

#define ADDRESS_COUNT 256

namespace LSST {
namespace M1M3 {
namespace SS {

ILC::ILC(M1M3SSPublisher* publisher, PositionController* positionController,
         ILCApplicationSettings* ilcApplicationSettings,
         ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
         ForceActuatorSettings* forceActuatorSettings,
         HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
         HardpointActuatorSettings* hardpointActuatorSettings,
         HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings,
         SafetyController* safetyController)
        : _subnetData(forceActuatorApplicationSettings, forceActuatorSettings,
                      hardpointActuatorApplicationSettings, hardpointMonitorApplicationSettings),
          _ilcMessageFactory(ilcApplicationSettings),
          _responseParser(forceActuatorSettings, hardpointActuatorSettings, publisher, &_subnetData,
                          safetyController),
          _busListSetADCChannelOffsetAndSensitivity(&_subnetData, &_ilcMessageFactory,
                                                    publisher->getEventForceActuatorInfo(),
                                                    publisher->getEventHardpointActuatorInfo()),
          _busListSetADCScanRate(&_subnetData, &_ilcMessageFactory, publisher->getEventForceActuatorInfo(),
                                 publisher->getEventHardpointActuatorInfo()),
          _busListSetBoostValveDCAGains(&_subnetData, &_ilcMessageFactory,
                                        publisher->getEventForceActuatorInfo()),
          _busListReset(&_subnetData, &_ilcMessageFactory),
          _busListReportServerID(&_subnetData, &_ilcMessageFactory),
          _busListReportServerStatus(&_subnetData, &_ilcMessageFactory),
          _busListReportADCScanRate(&_subnetData, &_ilcMessageFactory),
          _busListReadCalibration(&_subnetData, &_ilcMessageFactory),
          _busListReadBoostValveDCAGains(&_subnetData, &_ilcMessageFactory),
          _busListReportDCAID(&_subnetData, &_ilcMessageFactory),
          _busListReportDCAStatus(&_subnetData, &_ilcMessageFactory),
          _busListChangeILCModeDisabled(&_subnetData, &_ilcMessageFactory, ILCModes::Disabled,
                                        ILCModes::Enabled),
          _busListChangeILCModeEnabled(&_subnetData, &_ilcMessageFactory, ILCModes::Enabled,
                                       ILCModes::Enabled),
          _busListChangeILCModeStandby(&_subnetData, &_ilcMessageFactory, ILCModes::Standby,
                                       ILCModes::Standby),
          _busListChangeILCModeClearFaults(&_subnetData, &_ilcMessageFactory, ILCModes::ClearFaults,
                                           ILCModes::ClearFaults),
          _busListFreezeSensor(&_subnetData, &_ilcMessageFactory, publisher->getOuterLoopData()),
          _busListRaised(&_subnetData, &_ilcMessageFactory, publisher->getOuterLoopData(),
                         publisher->getForceActuatorData(), publisher->getHardpointActuatorData(),
                         publisher->getEventForceActuatorInfo(), publisher->getEventAppliedCylinderForces()),
          _busListActive(&_subnetData, &_ilcMessageFactory, publisher->getOuterLoopData(),
                         publisher->getForceActuatorData(), publisher->getHardpointActuatorData(),
                         publisher->getEventForceActuatorInfo(), publisher->getEventAppliedCylinderForces()),
          _firmwareUpdate(&_subnetData) {
    spdlog::debug("ILC: ILC()");
    _publisher = publisher;
    _safetyController = safetyController;
    _hardpointActuatorSettings = hardpointActuatorSettings;
    _hardpointActuatorData = _publisher->getHardpointActuatorData();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorData = _publisher->getForceActuatorData();
    _hardpointActuatorInfo = _publisher->getEventHardpointActuatorInfo();
    _controlListToggle = 0;
    _positionController = positionController;
}

ILC::~ILC() {}

void ILC::programILC(int32_t actuatorId, std::string filePath) {
    spdlog::debug("ILC: programILC({},{})", actuatorId, filePath);
    _firmwareUpdate.Program(actuatorId, filePath);
}

void ILC::modbusTransmit(int32_t actuatorId, int32_t functionCode, int32_t dataLength, int16_t* data) {
    ILCMap map = _subnetData.getMap(actuatorId);
    int subnet = map.Subnet;
    int address = map.Address;
    if (subnet == 255 || address == 255) {
        spdlog::error("ILC: Modbus Transmit unknown actuator {}", actuatorId);
        return;
    }
    ModbusBuffer buffer;
    buffer.setIndex(0);
    buffer.setLength(0);
    buffer.writeSubnet((uint8_t)_subnetToTxAddress(subnet));
    buffer.writeLength(0);
    buffer.writeSoftwareTrigger();

    buffer.writeU8((uint8_t)address);
    buffer.writeU8((uint8_t)functionCode);
    for (int i = 0; i < dataLength && i < 252; ++i) {
        buffer.writeU8((uint8_t)data[i]);
    }
    buffer.writeCRC(dataLength + 2);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(10000);

    buffer.writeTriggerIRQ();
    buffer.set(1, buffer.getIndex() - 2);
    buffer.setLength(buffer.getIndex());

    _responseParser.grabNextResponse();
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    waitForSubnet(subnet, 5000);
    read(subnet);
}

void ILC::writeCalibrationDataBuffer() {
    spdlog::debug("ILC: writeCalibrationDataBuffer()");
    _writeBusList(&_busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
    spdlog::debug("ILC: writeSetADCScanRateBuffer()");
    _writeBusList(&_busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
    spdlog::debug("ILC: writeSetBoostValveDCAGainBuffer()");
    _writeBusList(&_busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
    spdlog::debug("ILC: writeResetBuffer()");
    _writeBusList(&_busListReset);
}

void ILC::writeReportServerIDBuffer() {
    spdlog::debug("ILC: writeReportServerIDBuffer()");
    _writeBusList(&_busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
    spdlog::debug("ILC: writeReportServerStatusBuffer()");
    _writeBusList(&_busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
    spdlog::debug("ILC: writeReportADCScanRateBuffer()");
    _writeBusList(&_busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
    spdlog::debug("ILC: writeReadCalibrationDataBuffer()");
    _writeBusList(&_busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
    spdlog::debug("ILC: writeReadBoostValveDCAGainBuffer()");
    _writeBusList(&_busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
    spdlog::debug("ILC: writeReportDCAIDBuffer()");
    _writeBusList(&_busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
    spdlog::debug("ILC: writeReportDCAStatusBuffer()");
    _writeBusList(&_busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
    spdlog::debug("ILC: writeSetModeDisableBuffer()");
    _writeBusList(&_busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
    spdlog::debug("ILC: writeSetModeEnableBuffer()");
    _writeBusList(&_busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
    spdlog::debug("ILC: writeSetModeStandbyBuffer()");
    _writeBusList(&_busListChangeILCModeStandby);
}

void ILC::writeSetModeClearFaultsBuffer() {
    spdlog::debug("ILC: writeSetModeClearFaultsBuffer()");
    _writeBusList(&_busListChangeILCModeClearFaults);
}

void ILC::writeFreezeSensorListBuffer() {
    spdlog::debug("ILC: writeFreezeSensorListBuffer()");
    _busListFreezeSensor.update();
    _writeBusList(&_busListFreezeSensor);
}

void ILC::writeRaisedListBuffer() {
    spdlog::debug("ILC: writeRaisedListBuffer()");
    _positionController->updateSteps();
    _busListRaised.update();
    _writeBusList(&_busListRaised);
}

void ILC::writeActiveListBuffer() {
    spdlog::debug("ILC: writeActiveListBuffer()");
    _busListActive.update();
    _writeBusList(&_busListActive);
}

void ILC::writeControlListBuffer() {
    spdlog::debug("ILC: writeControlListBuffer()");
    if (_controlListToggle == 0) {
        writeRaisedListBuffer();
    } else {
        writeActiveListBuffer();
    }
    _controlListToggle = RoundRobin::Inc(_controlListToggle, 3);
}

void ILC::triggerModbus() {
    spdlog::debug("ILC: triggerModbus()");
    IFPGA::get().writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
}

void ILC::waitForSubnet(int32_t subnet, int32_t timeout) {
    spdlog::debug("ILC: waitForSubnet({:d}, {:d})", subnet, timeout);
    IFPGA::get().waitForModbusIRQ(subnet, timeout);
    IFPGA::get().ackModbusIRQ(subnet);
}

void ILC::waitForAllSubnets(int32_t timeout) {
    spdlog::debug("ILC: waitForAllSubnets({:d})", timeout);
    waitForSubnet(1, timeout);
    waitForSubnet(2, timeout);
    waitForSubnet(3, timeout);
    waitForSubnet(4, timeout);
    waitForSubnet(5, timeout);
}

void ILC::read(uint8_t subnet) {
    spdlog::debug("ILC: read({:d})", subnet);
    _u16Buffer[0] = _subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(_u16Buffer, 1, 0);
    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), 1, 10);
    uint16_t reportedLength = _rxBuffer.readLength();
    if (reportedLength == 0) {
        spdlog::error("ILC: Timeout on response");
        return;
    }

    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), reportedLength, 10);
    _rxBuffer.setLength(reportedLength);
    _responseParser.parse(&_rxBuffer, subnet);
}

void ILC::readAll() {
    spdlog::debug("ILC: readAll()");
    read(1);
    read(2);
    read(3);
    read(4);
    read(5);
}

void ILC::flush(uint8_t subnet) {
    spdlog::debug("ILC: flush({:d})", (int32_t)subnet);
    _u16Buffer[0] = _subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(_u16Buffer, 1, 0);
    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), 1, 10);
    uint16_t reportedLength = _rxBuffer.readLength();
    if (reportedLength > 0) {
        _rxBuffer.setIndex(0);
        IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), reportedLength, 10);
        _rxBuffer.setLength(reportedLength);
    }
}

void ILC::flushAll() {
    spdlog::debug("ILC: flushAll()");
    flush(1);
    flush(2);
    flush(3);
    flush(4);
    flush(5);
}

void ILC::calculateHPPostion() {
    double displacement[] = {
            ((_hardpointActuatorData->encoder[0] - _hardpointActuatorInfo->referencePosition[0]) *
             _hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[1] - _hardpointActuatorInfo->referencePosition[1]) *
             _hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[2] - _hardpointActuatorInfo->referencePosition[2]) *
             _hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[3] - _hardpointActuatorInfo->referencePosition[3]) *
             _hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[4] - _hardpointActuatorInfo->referencePosition[4]) *
             _hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[5] - _hardpointActuatorInfo->referencePosition[5]) *
             _hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
    };
    _hardpointActuatorData->xPosition =
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[0] * displacement[2] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[1] * displacement[3] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[2] * displacement[4] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[3] * displacement[5] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[4] * displacement[0] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[5] * displacement[1];
    _hardpointActuatorData->yPosition =
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[6] * displacement[2] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[7] * displacement[3] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[8] * displacement[4] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[9] * displacement[5] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[10] * displacement[0] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[11] * displacement[1];
    _hardpointActuatorData->zPosition =
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[12] * displacement[2] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[13] * displacement[3] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[14] * displacement[4] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[15] * displacement[5] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[16] * displacement[0] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[17] * displacement[1];
    _hardpointActuatorData->xRotation =
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[18] * displacement[2] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[19] * displacement[3] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[20] * displacement[4] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[21] * displacement[5] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[22] * displacement[0] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[23] * displacement[1];
    _hardpointActuatorData->yRotation =
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[24] * displacement[2] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[25] * displacement[3] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[26] * displacement[4] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[27] * displacement[5] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[28] * displacement[0] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[29] * displacement[1];
    _hardpointActuatorData->zRotation =
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[30] * displacement[2] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[31] * displacement[3] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[32] * displacement[4] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[33] * displacement[5] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[34] * displacement[0] +
            _hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[35] * displacement[1];
}

void ILC::calculateHPMirrorForces() {
    std::vector<float> m = _forceActuatorSettings->HardpointForceMomentTable;
    float* force = _hardpointActuatorData->measuredForce;
    _hardpointActuatorData->fx = m[0] * force[0] + m[1] * force[1] + m[2] * force[2] + m[3] * force[3] +
                                 m[4] * force[4] + m[5] * force[5];
    _hardpointActuatorData->fy = m[6] * force[0] + m[7] * force[1] + m[8] * force[2] + m[9] * force[3] +
                                 m[10] * force[4] + m[11] * force[5];
    _hardpointActuatorData->fz = m[12] * force[0] + m[13] * force[1] + m[14] * force[2] + m[15] * force[3] +
                                 m[16] * force[4] + m[17] * force[5];
    _hardpointActuatorData->mx = m[18] * force[0] + m[19] * force[1] + m[20] * force[2] + m[21] * force[3] +
                                 m[22] * force[4] + m[23] * force[5];
    _hardpointActuatorData->my = m[24] * force[0] + m[25] * force[1] + m[26] * force[2] + m[27] * force[3] +
                                 m[28] * force[4] + m[29] * force[5];
    _hardpointActuatorData->mz = m[30] * force[0] + m[31] * force[1] + m[32] * force[2] + m[33] * force[3] +
                                 m[34] * force[4] + m[35] * force[5];
    _hardpointActuatorData->forceMagnitude = sqrt(_hardpointActuatorData->fx * _hardpointActuatorData->fx +
                                                  _hardpointActuatorData->fy * _hardpointActuatorData->fy +
                                                  _hardpointActuatorData->fz * _hardpointActuatorData->fz);
}

void ILC::calculateFAMirrorForces() {
    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _forceActuatorSettings, _forceActuatorData->xForce,
            _forceActuatorData->yForce, _forceActuatorData->zForce);
    _forceActuatorData->fx = fm.Fx;
    _forceActuatorData->fy = fm.Fy;
    _forceActuatorData->fz = fm.Fz;
    _forceActuatorData->mx = fm.Mx;
    _forceActuatorData->my = fm.My;
    _forceActuatorData->mz = fm.Mz;
    _forceActuatorData->forceMagnitude = fm.ForceMagnitude;
}

void ILC::clearResponses() {
    spdlog::debug("ILC: clearResponses()");
    _responseParser.clearResponses();
}

void ILC::verifyResponses() {
    spdlog::debug("ILC: verifyResponses()");
    _responseParser.verifyResponses();
}

void ILC::publishForceActuatorInfo() {
    _publisher->getEventForceActuatorInfo()->timestamp = _publisher->getTimestamp();
    _publisher->logForceActuatorInfo();
}

void ILC::publishForceActuatorStatus() {
    // _publisher->putForceActuatorStatus();
}

void ILC::publishForceActuatorData() { _publisher->putForceActuatorData(); }

void ILC::publishHardpointActuatorInfo() {
    _publisher->getEventHardpointActuatorInfo()->timestamp = _publisher->getTimestamp();
    _publisher->logHardpointActuatorInfo();
}

void ILC::publishHardpointStatus() {
    // _publisher->putHardpointStatus();
}

void ILC::publishHardpointData() { _publisher->putHardpointActuatorData(); }

void ILC::publishHardpointMonitorInfo() {
    _publisher->getEventHardpointMonitorInfo()->timestamp = _publisher->getTimestamp();
    _publisher->logHardpointMonitorInfo();
}

void ILC::publishHardpointMonitorStatus() {
    // _publisher->putHardpointMonitorStatus();
}

void ILC::publishHardpointMonitorData() { _publisher->putHardpointMonitorData(); }

uint8_t ILC::_subnetToRxAddress(uint8_t subnet) {
    switch (subnet) {
        case 1:
            return FPGAAddresses::ModbusSubnetARx;
        case 2:
            return FPGAAddresses::ModbusSubnetBRx;
        case 3:
            return FPGAAddresses::ModbusSubnetCRx;
        case 4:
            return FPGAAddresses::ModbusSubnetDRx;
        case 5:
            return FPGAAddresses::ModbusSubnetERx;
        default:
            return 0;
    }
}

uint8_t ILC::_subnetToTxAddress(uint8_t subnet) {
    switch (subnet) {
        case 1:
            return FPGAAddresses::ModbusSubnetATx;
        case 2:
            return FPGAAddresses::ModbusSubnetBTx;
        case 3:
            return FPGAAddresses::ModbusSubnetCTx;
        case 4:
            return FPGAAddresses::ModbusSubnetDTx;
        case 5:
            return FPGAAddresses::ModbusSubnetETx;
        default:
            return 0;
    }
}

void ILC::_writeBusList(IBusList* busList) {
    IFPGA::get().writeCommandFIFO(busList->getBuffer(), busList->getLength(), 0);
    _responseParser.incExpectedResponses(busList->getExpectedFAResponses(), busList->getExpectedHPResponses(),
                                         busList->getExpectedHMResponses());
}

void ILC::_updateHPSteps() {
    for (int i = 0; i < 6; i++) {
        if (_hpSteps[i] != 0) {
            if (_hpSteps[i] > 100) {
                _hpSteps[i] -= 100;
                _hpStepCommand[i] = 100;
            } else if (_hpSteps[i] < -100) {
                _hpSteps[i] += 100;
                _hpStepCommand[i] = -100;
            } else {
                _hpStepCommand[i] = _hpSteps[i];
                _hpSteps[i] = 0;
            }
        } else {
            _hpStepCommand[i] = 0;
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
