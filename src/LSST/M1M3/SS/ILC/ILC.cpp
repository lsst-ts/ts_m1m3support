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
#include <BusList.h>
#include <SAL_MTM1M3C.h>
#include <ForceConverter.h>
#include <spdlog/spdlog.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <RoundRobin.h>
#include <PositionController.h>
#include <cmath>

#define ADDRESS_COUNT 256

using namespace LSST::M1M3::SS;

ILC::ILC(PositionController* positionController, ILCApplicationSettings* ilcApplicationSettings,
         ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
         ForceActuatorSettings* forceActuatorSettings,
         HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
         HardpointActuatorSettings* hardpointActuatorSettings,
         HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings,
         SafetyController* safetyController)
        : _subnetData(forceActuatorApplicationSettings, forceActuatorSettings,
                      hardpointActuatorApplicationSettings, hardpointMonitorApplicationSettings),
          _ilcMessageFactory(ilcApplicationSettings),
          _responseParser(forceActuatorSettings, hardpointActuatorSettings, &_subnetData, safetyController),
          _busListSetADCChannelOffsetAndSensitivity(&_subnetData, &_ilcMessageFactory),
          _busListSetADCScanRate(&_subnetData, &_ilcMessageFactory),
          _busListSetBoostValveDCAGains(&_subnetData, &_ilcMessageFactory),
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
          _busListFreezeSensor(&_subnetData, &_ilcMessageFactory),
          _busListRaised(&_subnetData, &_ilcMessageFactory),
          _busListActive(&_subnetData, &_ilcMessageFactory) {
    SPDLOG_DEBUG("ILC: ILC()");
    _safetyController = safetyController;
    _hardpointActuatorSettings = hardpointActuatorSettings;
    _hardpointActuatorData = M1M3SSPublisher::get().getHardpointActuatorData();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceActuatorSettings = forceActuatorSettings;
    _forceActuatorData = M1M3SSPublisher::get().getForceActuatorData();
    _hardpointActuatorInfo = M1M3SSPublisher::get().getEventHardpointActuatorInfo();
    _controlListToggle = 0;
    _positionController = positionController;

    buildBusLists();
}

ILC::~ILC() {}

void ILC::buildBusLists() {
    _busListSetADCChannelOffsetAndSensitivity.buildBuffer();
    _busListSetADCScanRate.buildBuffer();
    _busListSetBoostValveDCAGains.buildBuffer();
    _busListReset.buildBuffer();
    _busListReportServerID.buildBuffer();
    _busListReportServerStatus.buildBuffer();
    _busListReportADCScanRate.buildBuffer();
    _busListReadCalibration.buildBuffer();
    _busListReadBoostValveDCAGains.buildBuffer();
    _busListReportDCAID.buildBuffer();
    _busListReportDCAStatus.buildBuffer();
    _busListChangeILCModeDisabled.buildBuffer();
    _busListChangeILCModeEnabled.buildBuffer();
    _busListChangeILCModeStandby.buildBuffer();
    _busListChangeILCModeClearFaults.buildBuffer();
    _busListFreezeSensor.buildBuffer();
    _busListRaised.buildBuffer();
    _busListActive.buildBuffer();
}

void ILC::writeCalibrationDataBuffer() {
    SPDLOG_DEBUG("ILC: writeCalibrationDataBuffer()");
    _writeBusList(&_busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
    SPDLOG_DEBUG("ILC: writeSetADCScanRateBuffer()");
    _writeBusList(&_busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
    SPDLOG_DEBUG("ILC: writeSetBoostValveDCAGainBuffer()");
    _writeBusList(&_busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
    SPDLOG_DEBUG("ILC: writeResetBuffer()");
    _writeBusList(&_busListReset);
}

void ILC::writeReportServerIDBuffer() {
    SPDLOG_DEBUG("ILC: writeReportServerIDBuffer()");
    _writeBusList(&_busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
    SPDLOG_DEBUG("ILC: writeReportServerStatusBuffer()");
    _writeBusList(&_busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
    SPDLOG_DEBUG("ILC: writeReportADCScanRateBuffer()");
    _writeBusList(&_busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
    SPDLOG_DEBUG("ILC: writeReadCalibrationDataBuffer()");
    _writeBusList(&_busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
    SPDLOG_DEBUG("ILC: writeReadBoostValveDCAGainBuffer()");
    _writeBusList(&_busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
    SPDLOG_DEBUG("ILC: writeReportDCAIDBuffer()");
    _writeBusList(&_busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
    SPDLOG_DEBUG("ILC: writeReportDCAStatusBuffer()");
    _writeBusList(&_busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
    SPDLOG_DEBUG("ILC: writeSetModeDisableBuffer()");
    _writeBusList(&_busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
    SPDLOG_DEBUG("ILC: writeSetModeEnableBuffer()");
    _writeBusList(&_busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
    SPDLOG_DEBUG("ILC: writeSetModeStandbyBuffer()");
    _writeBusList(&_busListChangeILCModeStandby);
}

void ILC::writeSetModeClearFaultsBuffer() {
    SPDLOG_DEBUG("ILC: writeSetModeClearFaultsBuffer()");
    _writeBusList(&_busListChangeILCModeClearFaults);
}

void ILC::writeFreezeSensorListBuffer() {
    SPDLOG_DEBUG("ILC: writeFreezeSensorListBuffer()");
    _busListFreezeSensor.update();
    _writeBusList(&_busListFreezeSensor);
}

void ILC::writeRaisedListBuffer() {
    SPDLOG_DEBUG("ILC: writeRaisedListBuffer()");
    _positionController->updateSteps();
    _busListRaised.update();
    _writeBusList(&_busListRaised);
}

void ILC::writeActiveListBuffer() {
    SPDLOG_DEBUG("ILC: writeActiveListBuffer()");
    _busListActive.update();
    _writeBusList(&_busListActive);
}

void ILC::writeControlListBuffer() {
    SPDLOG_DEBUG("ILC: writeControlListBuffer()");
    if (_controlListToggle == 0) {
        writeRaisedListBuffer();
    } else {
        writeActiveListBuffer();
    }
    _controlListToggle = RoundRobin::Inc(_controlListToggle, 3);
}

void ILC::triggerModbus() {
    SPDLOG_DEBUG("ILC: triggerModbus()");
    IFPGA::get().writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
}

void ILC::waitForSubnet(int32_t subnet, int32_t timeout) {
    SPDLOG_DEBUG("ILC: waitForSubnet({:d}, {:d})", subnet, timeout);
    IFPGA::get().waitForModbusIRQ(subnet, timeout);
    IFPGA::get().ackModbusIRQ(subnet);
}

void ILC::waitForAllSubnets(int32_t timeout) {
    SPDLOG_DEBUG("ILC: waitForAllSubnets({:d})", timeout);
    waitForSubnet(1, timeout);
    waitForSubnet(2, timeout);
    waitForSubnet(3, timeout);
    waitForSubnet(4, timeout);
    waitForSubnet(5, timeout);
}

void ILC::read(uint8_t subnet) {
    SPDLOG_DEBUG("ILC: read({:d})", subnet);
    uint16_t addr = _subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(&addr, 1, 0);
    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), 1, 10);
    uint16_t reportedLength = _rxBuffer.readLength();
    if (reportedLength == 0) {
        SPDLOG_ERROR("ILC: Timeout on response");
        return;
    }

    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), reportedLength, 10);
    _rxBuffer.setLength(reportedLength);
    _responseParser.parse(&_rxBuffer, subnet);
}

void ILC::readAll() {
    SPDLOG_DEBUG("ILC: readAll()");
    read(1);
    read(2);
    read(3);
    read(4);
    read(5);
}

void ILC::flush(uint8_t subnet) {
    SPDLOG_DEBUG("ILC: flush({:d})", (int32_t)subnet);
    uint16_t add = _subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(&add, 1, 0);
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
    SPDLOG_DEBUG("ILC: flushAll()");
    flush(1);
    flush(2);
    flush(3);
    flush(4);
    flush(5);
}

void ILC::calculateHPPostion() {
    double displacement[] = {
            ((_hardpointActuatorData->encoder[0] - _hardpointActuatorInfo->referencePosition[0]) *
             _hardpointActuatorSettings->micrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[1] - _hardpointActuatorInfo->referencePosition[1]) *
             _hardpointActuatorSettings->micrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[2] - _hardpointActuatorInfo->referencePosition[2]) *
             _hardpointActuatorSettings->micrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[3] - _hardpointActuatorInfo->referencePosition[3]) *
             _hardpointActuatorSettings->micrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[4] - _hardpointActuatorInfo->referencePosition[4]) *
             _hardpointActuatorSettings->micrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((_hardpointActuatorData->encoder[5] - _hardpointActuatorInfo->referencePosition[5]) *
             _hardpointActuatorSettings->micrometersPerEncoder) /
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
    SPDLOG_DEBUG("ILC: clearResponses()");
    _responseParser.clearResponses();
}

void ILC::verifyResponses() {
    SPDLOG_DEBUG("ILC: verifyResponses()");
    _responseParser.verifyResponses();
}

void ILC::publishForceActuatorInfo() {
    M1M3SSPublisher::get().getEventForceActuatorInfo()->timestamp = M1M3SSPublisher::get().getTimestamp();
    M1M3SSPublisher::get().logForceActuatorInfo();
}

void ILC::publishForceActuatorStatus() {
    // M1M3SSPublisher::get().putForceActuatorStatus();
}

void ILC::publishForceActuatorData() { M1M3SSPublisher::get().putForceActuatorData(); }

void ILC::publishHardpointActuatorInfo() {
    M1M3SSPublisher::get().getEventHardpointActuatorInfo()->timestamp = M1M3SSPublisher::get().getTimestamp();
    M1M3SSPublisher::get().logHardpointActuatorInfo();
}

void ILC::publishHardpointStatus() {
    // M1M3SSPublisher::get().putHardpointStatus();
}

void ILC::publishHardpointData() { M1M3SSPublisher::get().putHardpointActuatorData(); }

void ILC::publishHardpointMonitorInfo() {
    M1M3SSPublisher::get().getEventHardpointMonitorInfo()->timestamp = M1M3SSPublisher::get().getTimestamp();
    M1M3SSPublisher::get().logHardpointMonitorInfo();
}

void ILC::publishHardpointMonitorStatus() {
    // M1M3SSPublisher::get().putHardpointMonitorStatus();
}

void ILC::publishHardpointMonitorData() { M1M3SSPublisher::get().putHardpointMonitorData(); }

void ILC::disableFA(uint32_t actuatorId) {
    if (hasDisabledFarNeighbor(_forceActuatorApplicationSettings->ActuatorIdToZIndex(actuatorId)) > 0) {
        SPDLOG_CRITICAL("Race condition? Disabling actuator with far neighbor disabled");
        return;
    }
    _subnetData.disableFA(actuatorId);
    M1M3SSPublisher::get().getEnabledForceActuators()->setEnabled(actuatorId, false);
    buildBusLists();
}

void ILC::enableFA(uint32_t actuatorId) {
    _subnetData.enableFA(actuatorId);
    M1M3SSPublisher::get().getEnabledForceActuators()->setEnabled(actuatorId, true);
    buildBusLists();
}

void ILC::enableAllFA() {
    _subnetData.enableAllFA();
    M1M3SSPublisher::get().getEnabledForceActuators()->setEnabledAll();
    buildBusLists();
}

uint32_t ILC::hasDisabledFarNeighbor(uint32_t actuatorIndex) {
    for (auto farID : _forceActuatorSettings->Neighbors[actuatorIndex].FarIDs) {
        if (isDisabled(farID)) {
            return farID;
        }
    }
    return 0;
}

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

void ILC::_writeBusList(BusList* busList) {
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
