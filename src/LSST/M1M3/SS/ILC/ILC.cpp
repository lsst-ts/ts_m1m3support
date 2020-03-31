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
        : subnetData(forceActuatorApplicationSettings, forceActuatorSettings,
                     hardpointActuatorApplicationSettings, hardpointMonitorApplicationSettings),
          ilcMessageFactory(ilcApplicationSettings),
          responseParser(forceActuatorSettings, hardpointActuatorSettings, publisher, &this->subnetData,
                         safetyController),
          busListSetADCChannelOffsetAndSensitivity(&this->subnetData, &this->ilcMessageFactory,
                                                   publisher->getEventForceActuatorInfo(),
                                                   publisher->getEventHardpointActuatorInfo()),
          busListSetADCScanRate(&this->subnetData, &this->ilcMessageFactory,
                                publisher->getEventForceActuatorInfo(),
                                publisher->getEventHardpointActuatorInfo()),
          busListSetBoostValveDCAGains(&this->subnetData, &this->ilcMessageFactory,
                                       publisher->getEventForceActuatorInfo()),
          busListReset(&this->subnetData, &this->ilcMessageFactory),
          busListReportServerID(&this->subnetData, &this->ilcMessageFactory),
          busListReportServerStatus(&this->subnetData, &this->ilcMessageFactory),
          busListReportADCScanRate(&this->subnetData, &this->ilcMessageFactory),
          busListReadCalibration(&this->subnetData, &this->ilcMessageFactory),
          busListReadBoostValveDCAGains(&this->subnetData, &this->ilcMessageFactory),
          busListReportDCAID(&this->subnetData, &this->ilcMessageFactory),
          busListReportDCAStatus(&this->subnetData, &this->ilcMessageFactory),
          busListChangeILCModeDisabled(&this->subnetData, &this->ilcMessageFactory, ILCModes::Disabled,
                                       ILCModes::Enabled),
          busListChangeILCModeEnabled(&this->subnetData, &this->ilcMessageFactory, ILCModes::Enabled,
                                      ILCModes::Enabled),
          busListChangeILCModeStandby(&this->subnetData, &this->ilcMessageFactory, ILCModes::Standby,
                                      ILCModes::Standby),
          busListChangeILCModeClearFaults(&this->subnetData, &this->ilcMessageFactory, ILCModes::ClearFaults,
                                          ILCModes::ClearFaults),
          busListFreezeSensor(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData()),
          busListRaised(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData(),
                        publisher->getForceActuatorData(), publisher->getHardpointActuatorData(),
                        publisher->getEventForceActuatorInfo(), publisher->getEventAppliedCylinderForces()),
          busListActive(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData(),
                        publisher->getForceActuatorData(), publisher->getHardpointActuatorData(),
                        publisher->getEventForceActuatorInfo(), publisher->getEventAppliedCylinderForces()),
          firmwareUpdate(&this->subnetData) {
    spdlog::debug("ILC: ILC()");
    this->publisher = publisher;
    this->safetyController = safetyController;
    this->hardpointActuatorSettings = hardpointActuatorSettings;
    this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorData = this->publisher->getForceActuatorData();
    this->hardpointActuatorInfo = this->publisher->getEventHardpointActuatorInfo();
    this->controlListToggle = 0;
    this->positionController = positionController;
}

ILC::~ILC() {}

void ILC::programILC(int32_t actuatorId, std::string filePath) {
    spdlog::debug("ILC: programILC({},{})", actuatorId, filePath.c_str());
    this->firmwareUpdate.Program(actuatorId, filePath);
}

void ILC::modbusTransmit(int32_t actuatorId, int32_t functionCode, int32_t dataLength, int16_t* data) {
    ILCMap map = this->subnetData.getMap(actuatorId);
    int subnet = map.Subnet;
    int address = map.Address;
    if (subnet == 255 || address == 255) {
        spdlog::error("ILC: Modbus Transmit unknown actuator {}", actuatorId);
        return;
    }
    ModbusBuffer buffer;
    buffer.setIndex(0);
    buffer.setLength(0);
    buffer.writeSubnet((uint8_t)this->subnetToTxAddress(subnet));
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

    this->responseParser.grabNextResponse();
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    this->waitForSubnet(subnet, 5000);
    this->read(subnet);
}

void ILC::writeCalibrationDataBuffer() {
    spdlog::debug("ILC: writeCalibrationDataBuffer()");
    this->writeBusList(&this->busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
    spdlog::debug("ILC: writeSetADCScanRateBuffer()");
    this->writeBusList(&this->busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
    spdlog::debug("ILC: writeSetBoostValveDCAGainBuffer()");
    this->writeBusList(&this->busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
    spdlog::debug("ILC: writeResetBuffer()");
    this->writeBusList(&this->busListReset);
}

void ILC::writeReportServerIDBuffer() {
    spdlog::debug("ILC: writeReportServerIDBuffer()");
    this->writeBusList(&this->busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
    spdlog::debug("ILC: writeReportServerStatusBuffer()");
    this->writeBusList(&this->busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
    spdlog::debug("ILC: writeReportADCScanRateBuffer()");
    this->writeBusList(&this->busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
    spdlog::debug("ILC: writeReadCalibrationDataBuffer()");
    this->writeBusList(&this->busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
    spdlog::debug("ILC: writeReadBoostValveDCAGainBuffer()");
    this->writeBusList(&this->busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
    spdlog::debug("ILC: writeReportDCAIDBuffer()");
    this->writeBusList(&this->busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
    spdlog::debug("ILC: writeReportDCAStatusBuffer()");
    this->writeBusList(&this->busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
    spdlog::debug("ILC: writeSetModeDisableBuffer()");
    this->writeBusList(&this->busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
    spdlog::debug("ILC: writeSetModeEnableBuffer()");
    this->writeBusList(&this->busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
    spdlog::debug("ILC: writeSetModeStandbyBuffer()");
    this->writeBusList(&this->busListChangeILCModeStandby);
}

void ILC::writeSetModeClearFaultsBuffer() {
    spdlog::debug("ILC: writeSetModeClearFaultsBuffer()");
    this->writeBusList(&this->busListChangeILCModeClearFaults);
}

void ILC::writeFreezeSensorListBuffer() {
    spdlog::debug("ILC: writeFreezeSensorListBuffer()");
    this->busListFreezeSensor.update();
    this->writeBusList(&this->busListFreezeSensor);
}

void ILC::writeRaisedListBuffer() {
    spdlog::debug("ILC: writeRaisedListBuffer()");
    this->positionController->updateSteps();
    this->busListRaised.update();
    this->writeBusList(&this->busListRaised);
}

void ILC::writeActiveListBuffer() {
    spdlog::debug("ILC: writeActiveListBuffer()");
    this->busListActive.update();
    this->writeBusList(&this->busListActive);
}

void ILC::writeControlListBuffer() {
    spdlog::debug("ILC: writeControlListBuffer()");
    if (this->controlListToggle == 0) {
        this->writeRaisedListBuffer();
    } else {
        this->writeActiveListBuffer();
    }
    this->controlListToggle = RoundRobin::Inc(this->controlListToggle, 3);
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
    this->waitForSubnet(1, timeout);
    this->waitForSubnet(2, timeout);
    this->waitForSubnet(3, timeout);
    this->waitForSubnet(4, timeout);
    this->waitForSubnet(5, timeout);
}

void ILC::read(uint8_t subnet) {
    spdlog::debug("ILC: read({:d})", subnet);
    // TODO: The expectation is if someone asks to read something they expect something to be there
    // so if something isn't there should be a warning (timeout on responses)
    this->u16Buffer[0] = this->subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(this->u16Buffer, 1, 0);
    this->rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(this->rxBuffer.getBuffer(), 1, 10);
    uint16_t reportedLength = this->rxBuffer.readLength();
    if (reportedLength > 0) {
        this->rxBuffer.setIndex(0);
        if (IFPGA::get().readU16ResponseFIFO(this->rxBuffer.getBuffer(), reportedLength, 10)) {
            spdlog::warn("ILC: Failed to read all {:d} words", reportedLength);
        }
        this->rxBuffer.setLength(reportedLength);
        this->responseParser.parse(&this->rxBuffer, subnet);
    }
}

void ILC::readAll() {
    spdlog::debug("ILC: readAll()");
    this->read(1);
    this->read(2);
    this->read(3);
    this->read(4);
    this->read(5);
}

void ILC::flush(uint8_t subnet) {
    spdlog::debug("ILC: flush({:d})", (int32_t)subnet);
    this->u16Buffer[0] = this->subnetToRxAddress(subnet);
    IFPGA::get().writeRequestFIFO(this->u16Buffer, 1, 0);
    this->rxBuffer.setIndex(0);
    IFPGA::get().readU16ResponseFIFO(this->rxBuffer.getBuffer(), 1, 10);
    uint16_t reportedLength = this->rxBuffer.readLength();
    if (reportedLength > 0) {
        this->rxBuffer.setIndex(0);
        if (IFPGA::get().readU16ResponseFIFO(this->rxBuffer.getBuffer(), reportedLength, 10)) {
            spdlog::warn("ILC: Failed to read all {:d} words", reportedLength);
        }
        this->rxBuffer.setLength(reportedLength);
    }
}

void ILC::flushAll() {
    spdlog::debug("ILC: flushAll()");
    this->flush(1);
    this->flush(2);
    this->flush(3);
    this->flush(4);
    this->flush(5);
}

void ILC::calculateHPPostion() {
    double displacement[] = {
            ((this->hardpointActuatorData->encoder[0] - this->hardpointActuatorInfo->referencePosition[0]) *
             this->hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((this->hardpointActuatorData->encoder[1] - this->hardpointActuatorInfo->referencePosition[1]) *
             this->hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((this->hardpointActuatorData->encoder[2] - this->hardpointActuatorInfo->referencePosition[2]) *
             this->hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((this->hardpointActuatorData->encoder[3] - this->hardpointActuatorInfo->referencePosition[3]) *
             this->hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((this->hardpointActuatorData->encoder[4] - this->hardpointActuatorInfo->referencePosition[4]) *
             this->hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
            ((this->hardpointActuatorData->encoder[5] - this->hardpointActuatorInfo->referencePosition[5]) *
             this->hardpointActuatorSettings->MicrometersPerEncoder) /
                    (MICROMETERS_PER_MILLIMETER * MILLIMETERS_PER_METER),
    };
    this->hardpointActuatorData->xPosition =
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[0] * displacement[2] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[1] * displacement[3] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[2] * displacement[4] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[3] * displacement[5] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[4] * displacement[0] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[5] * displacement[1];
    this->hardpointActuatorData->yPosition =
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[6] * displacement[2] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[7] * displacement[3] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[8] * displacement[4] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[9] * displacement[5] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[10] * displacement[0] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[11] * displacement[1];
    this->hardpointActuatorData->zPosition =
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[12] * displacement[2] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[13] * displacement[3] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[14] * displacement[4] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[15] * displacement[5] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[16] * displacement[0] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[17] * displacement[1];
    this->hardpointActuatorData->xRotation =
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[18] * displacement[2] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[19] * displacement[3] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[20] * displacement[4] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[21] * displacement[5] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[22] * displacement[0] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[23] * displacement[1];
    this->hardpointActuatorData->yRotation =
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[24] * displacement[2] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[25] * displacement[3] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[26] * displacement[4] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[27] * displacement[5] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[28] * displacement[0] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[29] * displacement[1];
    this->hardpointActuatorData->zRotation =
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[30] * displacement[2] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[31] * displacement[3] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[32] * displacement[4] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[33] * displacement[5] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[34] * displacement[0] +
            this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[35] * displacement[1];
}

void ILC::calculateHPMirrorForces() {
    std::vector<float> m = this->forceActuatorSettings->HardpointForceMomentTable;
    float* force = this->hardpointActuatorData->measuredForce;
    this->hardpointActuatorData->fx = m[0] * force[0] + m[1] * force[1] + m[2] * force[2] + m[3] * force[3] +
                                      m[4] * force[4] + m[5] * force[5];
    this->hardpointActuatorData->fy = m[6] * force[0] + m[7] * force[1] + m[8] * force[2] + m[9] * force[3] +
                                      m[10] * force[4] + m[11] * force[5];
    this->hardpointActuatorData->fz = m[12] * force[0] + m[13] * force[1] + m[14] * force[2] +
                                      m[15] * force[3] + m[16] * force[4] + m[17] * force[5];
    this->hardpointActuatorData->mx = m[18] * force[0] + m[19] * force[1] + m[20] * force[2] +
                                      m[21] * force[3] + m[22] * force[4] + m[23] * force[5];
    this->hardpointActuatorData->my = m[24] * force[0] + m[25] * force[1] + m[26] * force[2] +
                                      m[27] * force[3] + m[28] * force[4] + m[29] * force[5];
    this->hardpointActuatorData->mz = m[30] * force[0] + m[31] * force[1] + m[32] * force[2] +
                                      m[33] * force[3] + m[34] * force[4] + m[35] * force[5];
    this->hardpointActuatorData->forceMagnitude =
            sqrt(this->hardpointActuatorData->fx * this->hardpointActuatorData->fx +
                 this->hardpointActuatorData->fy * this->hardpointActuatorData->fy +
                 this->hardpointActuatorData->fz * this->hardpointActuatorData->fz);
}

void ILC::calculateFAMirrorForces() {
    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->forceActuatorData->xForce, this->forceActuatorData->yForce,
            this->forceActuatorData->zForce);
    this->forceActuatorData->fx = fm.Fx;
    this->forceActuatorData->fy = fm.Fy;
    this->forceActuatorData->fz = fm.Fz;
    this->forceActuatorData->mx = fm.Mx;
    this->forceActuatorData->my = fm.My;
    this->forceActuatorData->mz = fm.Mz;
    this->forceActuatorData->forceMagnitude = fm.ForceMagnitude;
}

void ILC::clearResponses() {
    spdlog::debug("ILC: clearResponses()");
    this->responseParser.clearResponses();
}

void ILC::verifyResponses() {
    spdlog::debug("ILC: verifyResponses()");
    this->responseParser.verifyResponses();
}

void ILC::publishForceActuatorInfo() {
    this->publisher->getEventForceActuatorInfo()->timestamp = this->publisher->getTimestamp();
    this->publisher->logForceActuatorInfo();
}

void ILC::publishForceActuatorStatus() {
    // this->publisher->putForceActuatorStatus();
}

void ILC::publishForceActuatorData() { this->publisher->putForceActuatorData(); }

void ILC::publishHardpointActuatorInfo() {
    this->publisher->getEventHardpointActuatorInfo()->timestamp = this->publisher->getTimestamp();
    this->publisher->logHardpointActuatorInfo();
}

void ILC::publishHardpointStatus() {
    // this->publisher->putHardpointStatus();
}

void ILC::publishHardpointData() { this->publisher->putHardpointActuatorData(); }

void ILC::publishHardpointMonitorInfo() {
    this->publisher->getEventHardpointMonitorInfo()->timestamp = this->publisher->getTimestamp();
    this->publisher->logHardpointMonitorInfo();
}

void ILC::publishHardpointMonitorStatus() {
    // this->publisher->putHardpointMonitorStatus();
}

void ILC::publishHardpointMonitorData() { this->publisher->putHardpointMonitorData(); }

uint8_t ILC::subnetToRxAddress(uint8_t subnet) {
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

uint8_t ILC::subnetToTxAddress(uint8_t subnet) {
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

void ILC::writeBusList(IBusList* busList) {
    IFPGA::get().writeCommandFIFO(busList->getBuffer(), busList->getLength(), 0);
    this->responseParser.incExpectedResponses(busList->getExpectedFAResponses(),
                                              busList->getExpectedHPResponses(),
                                              busList->getExpectedHMResponses());
}

void ILC::updateHPSteps() {
    for (int i = 0; i < 6; i++) {
        if (this->hpSteps[i] != 0) {
            if (this->hpSteps[i] > 100) {
                this->hpSteps[i] -= 100;
                this->hpStepCommand[i] = 100;
            } else if (this->hpSteps[i] < -100) {
                this->hpSteps[i] += 100;
                this->hpStepCommand[i] = -100;
            } else {
                this->hpStepCommand[i] = this->hpSteps[i];
                this->hpSteps[i] = 0;
            }
        } else {
            this->hpStepCommand[i] = 0;
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
