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
#include <unistd.h>

#include <spdlog/spdlog.h>

#include <SAL_MTM1M3C.h>

#include <BusList.h>
#include <FPGAAddresses.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorData.h>
#include <ForceActuatorFollowingErrorCounter.h>
#include <ForceActuatorForceWarning.h>
#include <ForceActuatorInfo.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointActuatorSettings.h>
#include <IFPGA.h>
#include <ILCApplicationSettings.h>
#include <M1M3SSPublisher.h>
#include <PositionController.h>
#include <RoundRobin.h>
#include <SSILCs.h>
#include <Timestamp.h>
#include <cRIO/DataTypes.h>

#define ADDRESS_COUNT 256

using namespace LSST::M1M3::SS;

SSILCs::SSILCs(PositionController *positionController,
               ForceActuatorApplicationSettings *forceActuatorApplicationSettings,
               HardpointActuatorApplicationSettings *hardpointActuatorApplicationSettings,
               HardpointMonitorApplicationSettings *hardpointMonitorApplicationSettings,
               SafetyController *safetyController)
        : _subnetData(forceActuatorApplicationSettings, hardpointActuatorApplicationSettings,
                      hardpointMonitorApplicationSettings),
          _ilcMessageFactory(),
          _responseParser(&_subnetData, safetyController),
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
    SPDLOG_DEBUG("SSILCs: SSILCs()");
    _safetyController = safetyController;
    _hardpointActuatorSettings = &HardpointActuatorSettings::instance();
    _hardpointActuatorData = M1M3SSPublisher::instance().getHardpointActuatorData();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _hardpointActuatorInfo = M1M3SSPublisher::instance().getEventHardpointActuatorInfo();
    _controlListToggle = 0;
    _positionController = positionController;

    buildBusLists();
}

SSILCs::~SSILCs() {}

void SSILCs::buildBusLists() {
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

void SSILCs::writeCalibrationDataBuffer() {
    SPDLOG_DEBUG("SSILCs: writeCalibrationDataBuffer()");
    _writeBusList(&_busListSetADCChannelOffsetAndSensitivity);
}

void SSILCs::writeSetADCScanRateBuffer() {
    SPDLOG_DEBUG("SSILCs: writeSetADCScanRateBuffer()");
    _writeBusList(&_busListSetADCScanRate);
}

void SSILCs::writeSetBoostValveDCAGainBuffer() {
    SPDLOG_DEBUG("SSILCs: writeSetBoostValveDCAGainBuffer()");
    _writeBusList(&_busListSetBoostValveDCAGains);
}

void SSILCs::writeResetBuffer() {
    SPDLOG_DEBUG("SSILCs: writeResetBuffer()");
    _writeBusList(&_busListReset);
}

void SSILCs::writeReportServerIDBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReportServerIDBuffer()");
    _writeBusList(&_busListReportServerID);
}

void SSILCs::writeReportServerStatusBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReportServerStatusBuffer()");
    _writeBusList(&_busListReportServerStatus);
}

void SSILCs::writeReportADCScanRateBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReportADCScanRateBuffer()");
    _writeBusList(&_busListReportADCScanRate);
}

void SSILCs::writeReadCalibrationDataBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReadCalibrationDataBuffer()");
    _writeBusList(&_busListReadCalibration);
}

void SSILCs::writeReadBoostValveDCAGainBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReadBoostValveDCAGainBuffer()");
    _writeBusList(&_busListReadBoostValveDCAGains);
}

void SSILCs::writeReportDCAIDBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReportDCAIDBuffer()");
    _writeBusList(&_busListReportDCAID);
}

void SSILCs::writeReportDCAStatusBuffer() {
    SPDLOG_DEBUG("SSILCs: writeReportDCAStatusBuffer()");
    _writeBusList(&_busListReportDCAStatus);
}

void SSILCs::writeSetModeDisableBuffer() {
    SPDLOG_DEBUG("SSILCs: writeSetModeDisableBuffer()");
    _writeBusList(&_busListChangeILCModeDisabled);
}

void SSILCs::writeSetModeEnableBuffer() {
    SPDLOG_DEBUG("SSILCs: writeSetModeEnableBuffer()");
    _writeBusList(&_busListChangeILCModeEnabled);
}

void SSILCs::writeSetModeStandbyBuffer() {
    SPDLOG_DEBUG("SSILCs: writeSetModeStandbyBuffer()");
    _writeBusList(&_busListChangeILCModeStandby);
}

void SSILCs::writeSetModeClearFaultsBuffer() {
    SPDLOG_DEBUG("SSILCs: writeSetModeClearFaultsBuffer()");
    _writeBusList(&_busListChangeILCModeClearFaults);
}

void SSILCs::writeFreezeSensorListBuffer() {
    SPDLOG_DEBUG("SSILCs: writeFreezeSensorListBuffer()");
    _busListFreezeSensor.update();
    _writeBusList(&_busListFreezeSensor);
}

void SSILCs::writeRaisedListBuffer() {
    SPDLOG_DEBUG("SSILCs: writeRaisedListBuffer()");
    _positionController->updateSteps();
    _busListRaised.update();
    _writeBusList(&_busListRaised);
}

void SSILCs::writeActiveListBuffer() {
    SPDLOG_DEBUG("SSILCs: writeActiveListBuffer()");
    _busListActive.update();
    _writeBusList(&_busListActive);
}

void SSILCs::writeControlListBuffer() {
    SPDLOG_DEBUG("SSILCs: writeControlListBuffer()");
    if (_controlListToggle == 0) {
        writeRaisedListBuffer();
    } else {
        writeActiveListBuffer();
    }
    _controlListToggle = RoundRobin::Inc(_controlListToggle, 3);
}

void SSILCs::triggerModbus() {
    SPDLOG_DEBUG("SSILCs: triggerModbus()");
    IFPGA::get().writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
}

void SSILCs::waitForAllSubnets(bool realtime_loop) {
    uint32_t error_timeout = realtime_loop ? ILCApplicationSettings::instance().FPGARealtimeLoopTimeout
                                           : ILCApplicationSettings::instance().FPGAConfigTimeout;
    int32_t warning_timeout = realtime_loop ? 18 : error_timeout * 0.75;

    SPDLOG_DEBUG("SSILCs: waitForAllSubnets(warning {:d}, error {:d})", warning_timeout, error_timeout);
    IFPGA::get().waitForModbusIRQs(warning_timeout, error_timeout);
    IFPGA::get().ackModbusIRQs();
}

void SSILCs::read(uint8_t subnet) {
    SPDLOG_DEBUG("SSILCs: read({:d})", subnet);
    uint16_t addr = _subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(&addr, 1, 0);
    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), 1, 10);
    uint16_t reportedLength = _rxBuffer.readLength();
    if (reportedLength == 0) {
        throw std::runtime_error(fmt::format("SSILCs subnet {}: Timeout on response", subnet));
    }

    _rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(_rxBuffer.getBuffer(), reportedLength, 10);
    _rxBuffer.setLength(reportedLength);
    _responseParser.parse(&_rxBuffer, subnet);
}

void SSILCs::readAll() {
    SPDLOG_DEBUG("SSILCs: readAll()");
    read(1);
    read(2);
    read(3);
    read(4);
    read(5);
    ForceActuatorForceWarning::instance().send();
    ForceActuatorFollowingErrorCounter::instance().send();
}

void SSILCs::flush(uint8_t subnet) {
    SPDLOG_DEBUG("SSILCs: flush({:d})", (int32_t)subnet);
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

void SSILCs::flushAll() {
    SPDLOG_DEBUG("SSILCs: flushAll()");
    flush(1);
    flush(2);
    flush(3);
    flush(4);
    flush(5);
}

void SSILCs::calculateHPPostion() {
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

void SSILCs::calculateHPMirrorForces() {
    std::vector<float> m = ForceActuatorSettings::instance().HardpointForceMomentTable;
#ifdef WITH_SAL_KAFKA
    std::vector<float> force = _hardpointActuatorData->measuredForce;
#else
    float *force = _hardpointActuatorData->measuredForce;
#endif
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

void SSILCs::calculateFAMirrorForces() {
    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _forceActuatorApplicationSettings, ForceActuatorData::instance().xForce,
            ForceActuatorData::instance().yForce, ForceActuatorData::instance().zForce);
    ForceActuatorData::instance().fx = fm.Fx;
    ForceActuatorData::instance().fy = fm.Fy;
    ForceActuatorData::instance().fz = fm.Fz;
    ForceActuatorData::instance().mx = fm.Mx;
    ForceActuatorData::instance().my = fm.My;
    ForceActuatorData::instance().mz = fm.Mz;
    ForceActuatorData::instance().forceMagnitude = fm.ForceMagnitude;
}

void SSILCs::clearResponses() {
    SPDLOG_DEBUG("SSILCs: clearResponses()");
    _responseParser.clearResponses();
}

void SSILCs::verifyResponses() {
    SPDLOG_DEBUG("SSILCs: verifyResponses()");
    _responseParser.verifyResponses();
}

void SSILCs::publishForceActuatorInfo() {
    ForceActuatorInfo::instance().timestamp = M1M3SSPublisher::instance().getTimestamp();
    ForceActuatorInfo::instance().log();
}

void SSILCs::publishForceActuatorStatus() {
    // M1M3SSPublisher::instance().putForceActuatorStatus();
}

void SSILCs::publishHardpointActuatorInfo() {
    M1M3SSPublisher::instance().getEventHardpointActuatorInfo()->timestamp =
            M1M3SSPublisher::instance().getTimestamp();
    M1M3SSPublisher::instance().logHardpointActuatorInfo();
}

void SSILCs::publishHardpointStatus() {
    // M1M3SSPublisher::instance().putHardpointStatus();
}

void SSILCs::publishHardpointData() { M1M3SSPublisher::instance().putHardpointActuatorData(); }

void SSILCs::publishHardpointMonitorInfo() {
    M1M3SSPublisher::instance().getEventHardpointMonitorInfo()->timestamp =
            M1M3SSPublisher::instance().getTimestamp();
    M1M3SSPublisher::instance().logHardpointMonitorInfo();
}

void SSILCs::publishHardpointMonitorStatus() {
    // M1M3SSPublisher::instance().putHardpointMonitorStatus();
}

void SSILCs::publishHardpointMonitorData() { M1M3SSPublisher::instance().putHardpointMonitorData(); }

void SSILCs::disableFA(uint32_t actuatorId) {
    if (hasDisabledFarNeighbor(_forceActuatorApplicationSettings->ActuatorIdToZIndex(actuatorId)) > 0) {
        SPDLOG_CRITICAL("Race condition? Disabling actuator with far neighbor disabled");
        return;
    }
    _subnetData.disableFA(actuatorId);
    M1M3SSPublisher::instance().getEnabledForceActuators()->setEnabled(actuatorId, false);
    buildBusLists();
}

void SSILCs::enableFA(uint32_t actuatorId) {
    _subnetData.enableFA(actuatorId);
    M1M3SSPublisher::instance().getEnabledForceActuators()->setEnabled(actuatorId, true);
    buildBusLists();
}

void SSILCs::enableAllFA() {
    _subnetData.enableAllFA();
    M1M3SSPublisher::instance().getEnabledForceActuators()->setEnabledAll();
    buildBusLists();
}

uint32_t SSILCs::hasDisabledFarNeighbor(uint32_t actuatorIndex) {
    for (auto farID : ForceActuatorSettings::instance().Neighbors[actuatorIndex].FarIDs) {
        if (isDisabled(farID)) {
            return farID;
        }
    }
    return 0;
}

uint8_t SSILCs::_subnetToRxAddress(uint8_t subnet) {
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

uint8_t SSILCs::_subnetToTxAddress(uint8_t subnet) {
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

void SSILCs::_writeBusList(BusList *busList) {
    IFPGA::get().writeCommandFIFO(busList->getBuffer(), busList->getLength(), 0);
    _responseParser.incExpectedResponses(busList->getExpectedFAResponses(), busList->getExpectedHPResponses(),
                                         busList->getExpectedHMResponses());
}

void SSILCs::_updateHPSteps() {
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
