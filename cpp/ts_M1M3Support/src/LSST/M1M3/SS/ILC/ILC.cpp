/*
 * ILC.cpp
 *
 *  Created on: Oct 1, 2017
 *      Author: ccontaxis
 */

#include <ILC.h>
#include <IPublisher.h>
#include <IFPGA.h>
#include <DataTypes.h>
#include <iostream>
#include <unistd.h>

using namespace std;

#define SUBNET_COUNT 5
#define FA_COUNT 156
#define HP_COUNT 6
#define ADDRESS_COUNT 256

namespace LSST {
namespace M1M3 {
namespace SS {

ILC::ILC(IPublisher* publisher, IFPGA* fpga, ILCApplicationSettings ilcApplicationSettings, ForceActuatorApplicationSettings forceActuatorApplicationSettings, HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings) {
	this->publisher = publisher;
	this->fpga = fpga;
	this->ilcApplicationSettings = ilcApplicationSettings;
	this->subnetStartIndex = 0;
	for(int i = 0; i < SUBNET_COUNT; i++) {
		for(int j = 0; j < FA_COUNT; j++) {
			this->subnetData[i].FAAddress[j] = 255;
		}
		for(int j = 0; j < HP_COUNT; j++) {
			this->subnetData[i].HPAddress[j] = 255;
		}
		for(int j = 0; j < ADDRESS_COUNT; j++) {
			this->subnetData[i].ILCDataFromAddress[j].ActuatorId = 0;
			this->subnetData[i].ILCDataFromAddress[j].DataIndex = 0;
			this->subnetData[i].ILCDataFromAddress[j].Type = ILCTypes::Unknown;
		}
	}
	for(int i = 0; i < FA_COUNT; i++) {
		ForceActuatorTableRow row = forceActuatorApplicationSettings.Table[i];
		this->subnetData[row.Subnet - 1].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[row.Subnet - 1].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[row.Subnet - 1].ILCDataFromAddress[row.Address].Type = ILCTypes::FA;
		for(int j = 0; j < FA_COUNT; j++) {
			if (this->subnetData[row.Subnet - 1].FAAddress[j] == 255) {
				this->subnetData[row.Subnet - 1].FAAddress[j] = row.Address;
				break;
			}
		}
		this->forceInfo.ReferenceId[row.Index] = row.ActuatorID;
		this->forceInfo.ModbusSubnet[row.Index] = row.Subnet;
		this->forceInfo.ModbusAddress[row.Index] = row.Address;
		this->forceInfo.ActuatorType[row.Index] = row.Type;
		this->forceInfo.ActuatorOrientation[row.Index] = row.Orientation;
		this->forceInfo.XPosition[row.Index] = row.XPosition;
		this->forceInfo.YPosition[row.Index] = row.YPosition;
		this->forceInfo.ZPosition[row.Index] = row.ZPosition;
		this->forceInfo.PrimaryAxisSensorOffset[row.Index] = row.PrimaryAxisSensorOffset;
		this->forceInfo.PrimaryAxisSensorSensitivity[row.Index] = row.PrimaryAxisSensorSensitivity;
		this->forceInfo.SecondaryAxisSensorOffset[row.Index] = row.SecondaryAxisSensorOffset;
		this->forceInfo.SecondaryAxisSensorSensitivity[row.Index] = row.SecondaryAxisSensorSensitivity;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings.Table[i];
		this->subnetData[row.Subnet - 1].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[row.Subnet - 1].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[row.Subnet - 1].ILCDataFromAddress[row.Address].Type = ILCTypes::HP;
		for(int j = 0; j < HP_COUNT; j++) {
			if (this->subnetData[row.Subnet - 1].HPAddress[j] == 255) {
				this->subnetData[row.Subnet - 1].HPAddress[j] = row.Address;
				break;
			}
		}
		this->hardpointInfo.ReferenceId[row.Index] = row.ActuatorID;
		this->hardpointInfo.ModbusSubnet[row.Index] = row.Subnet;
		this->hardpointInfo.ModbusAddress[row.Index] = row.Address;
		this->hardpointInfo.XPosition[row.Index] = row.XPosition;
		this->hardpointInfo.YPosition[row.Index] = row.YPosition;
		this->hardpointInfo.ZPosition[row.Index] = row.ZPosition;
		this->hardpointInfo.SensorOffset[row.Index] = row.SensorOffset;
		this->hardpointInfo.SensorSensitivity[row.Index] = row.SensorSensitivity;
	}
	this->broadcastCounter = 0;
	this->faStatusIndex[0] = 0;
	this->faStatusIndex[1] = 0;
	this->faStatusIndex[2] = 0;
	this->faStatusIndex[3] = 0;
	this->faStatusIndex[4] = 0;
	this->ilcWarning.Timestamp = 0;
	this->ilcWarning.ActuatorId = -1;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->createSetADCChannelOffsetAndSensitivityBuffer(&this->bufferSetADCChannelOffsetAndSensitivity);
	this->createSetADCScanRateBuffer(&this->bufferSetADCScanRate);
	this->createSetBoostValveDCAGains(&this->bufferSetBoostValveDCAGains);
	this->createResetBuffer(&this->bufferReset);
	this->createReportServerIDBuffer(&this->bufferReportServerID);
	this->createReportServerStatusBuffer(&this->bufferReportServerStatus);
	this->createReportADCScanRateBuffer(&this->bufferReportADCScanRate);
	this->createReadCalibrationBuffer(&this->bufferReadCalibration);
	this->createReadBoostValveDCAGainsBuffer(&this->bufferReadBoostValveDCAGains);
	this->createReportDCAIDBuffer(&this->bufferReportDCAID);
	this->createReportDCAStatusBuffer(&this->bufferReportDCAStatus);
	this->createChangeILCModeBuffer(&this->bufferChangeILCModeDisabled, ILCModes::Disabled);
	this->createChangeILCModeBuffer(&this->bufferChangeILCModeEnabled, ILCModes::Enabled);
	this->createChangeILCModeBuffer(&this->bufferChangeILCModeStandby, ILCModes::Standby);
}

ILC::~ILC() { }

void ILC::writeCalibrationDataBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferSetADCChannelOffsetAndSensitivity.getBuffer(), this->bufferSetADCChannelOffsetAndSensitivity.getLength(), 0);
}

void ILC::writeSetADCScanRateBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferSetADCScanRate.getBuffer(), this->bufferSetADCScanRate.getLength(), 0);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferSetBoostValveDCAGains.getBuffer(), this->bufferSetBoostValveDCAGains.getLength(), 0);
}

void ILC::writeResetBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReset.getBuffer(), this->bufferReset.getLength(), 0);
}

void ILC::writeReportServerIDBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReportServerID.getBuffer(), this->bufferReportServerID.getLength(), 0);
}

void ILC::writeReportServerStatusBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReportServerStatus.getBuffer(), this->bufferReportServerStatus.getLength(), 0);
}

void ILC::writeReportADCScanRateBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReportADCScanRate.getBuffer(), this->bufferReportADCScanRate.getLength(), 0);
}

void ILC::writeReadCalibrationDataBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReadCalibration.getBuffer(), this->bufferReadCalibration.getLength(), 0);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReadBoostValveDCAGains.getBuffer(), this->bufferReadBoostValveDCAGains.getLength(), 0);
}

void ILC::writeReportDCAIDBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReportDCAID.getBuffer(), this->bufferReportDCAID.getLength(), 0);
}

void ILC::writeReportDCAStatusBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferReportDCAStatus.getBuffer(), this->bufferReportDCAStatus.getLength(), 0);
}

void ILC::writeSetModeDisableBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferChangeILCModeDisabled.getBuffer(), this->bufferChangeILCModeDisabled.getLength(), 0);
}

void ILC::writeSetModeEnableBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferChangeILCModeEnabled.getBuffer(), this->bufferChangeILCModeEnabled.getLength(), 0);
}

void ILC::writeSetModeStandbyBuffer() {
	this->fpga->writeModbusTxFIFO(this->bufferChangeILCModeStandby.getBuffer(), this->bufferChangeILCModeStandby.getLength(), 0);
}

void ILC::writeFreezeSensorListBuffer() {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->bufferFreezeSensorList.buffer.setIndex(this->bufferFreezeSensorList.freezeSensorCommandIndex[i]);
		this->broadcastFreezeSensorValues(&this->bufferFreezeSensorList.buffer, this->broadcastCounter);
		int32_t faStatusCommandIndex = this->bufferFreezeSensorList.faStatusCommandIndex[i];
		if(faStatusCommandIndex != -1) {
			this->bufferFreezeSensorList.buffer.setIndex(faStatusCommandIndex);
			this->reportServerStatus(&this->bufferFreezeSensorList.buffer, this->subnetData[i].FAAddress[this->faStatusIndex[i]]);
		}
	}
	this->incBroadcastCounter();
	this->incFAStatusIndex();
	this->fpga->writeModbusTxFIFO(this->bufferFreezeSensorList.buffer.getBuffer(), this->bufferFreezeSensorList.buffer.getLength(), 0);
}

void ILC::waitForSubnet(int32_t subnet, int32_t timeout) {
	this->fpga->waitForModbusIRQ(subnet, timeout);
	this->fpga->ackModbusIRQ(subnet);
}

void ILC::waitForAllSubnets(int32_t timeout) {
	this->waitForSubnet(1, timeout);
	this->waitForSubnet(2, timeout);
	this->waitForSubnet(3, timeout);
	this->waitForSubnet(4, timeout);
	this->waitForSubnet(5, timeout);
}

void ILC::read(uint8_t subnet) {
	// TODO: The expectation is if someone asks to read something they expect something to be there
	// so if something isn't there should be a warning (timeout on responses)
	this->fpga->writeModbusRxCommandFIFO(subnet, 0);
	this->rxBuffer.setIndex(0);
	this->fpga->readModbusRxFIFO(this->rxBuffer.getBuffer(), 2, 1);
	uint8_t reportedSubnet = this->rxBuffer.readSubnet();
	uint16_t reportedLength = this->rxBuffer.readLength();
	cout << "Available " << (int32_t)reportedLength << endl;
	if (reportedLength > 0) {
		this->rxBuffer.setIndex(0);
		this->fpga->readModbusRxFIFO(this->rxBuffer.getBuffer(), reportedLength, 10);
		this->rxBuffer.setLength(reportedLength);
		this->parse(&this->rxBuffer, reportedSubnet);
	}
}

void ILC::readAll() {
	this->read(1);
	this->read(2);
	this->read(3);
	this->read(4);
	this->read(5);
}

void ILC::startSubnet(ModbusBuffer* buffer, uint8_t subnet) {
	buffer->writeSubnet(subnet + 1);
	this->subnetStartIndex = buffer->getIndex();
	buffer->writeLength(0);
	buffer->writeSoftwareTrigger();
}

void ILC::endSubnet(ModbusBuffer* buffer) {
	buffer->writeTriggerIRQ();
	buffer->set(this->subnetStartIndex, buffer->getIndex() - this->subnetStartIndex - 1);
}

void ILC::createSetADCChannelOffsetAndSensitivityBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->setADCChannelOffsetAndSensitivity(buffer, this->subnetData[i].FAAddress[j], 1, this->forceInfo.PrimaryAxisSensorOffset[j], this->forceInfo.PrimaryAxisSensorSensitivity[j]);
			this->setADCChannelOffsetAndSensitivity(buffer, this->subnetData[i].FAAddress[j], 2, this->forceInfo.SecondaryAxisSensorOffset[j], this->forceInfo.SecondaryAxisSensorSensitivity[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->setADCChannelOffsetAndSensitivity(buffer, this->subnetData[i].HPAddress[j], 1, this->hardpointInfo.SensorOffset[j], this->hardpointInfo.SensorSensitivity[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createSetADCScanRateBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->setADCScanRate(buffer, this->subnetData[i].FAAddress[j], this->forceInfo.ADCScanRate[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->setADCScanRate(buffer, this->subnetData[i].HPAddress[j], this->hardpointInfo.ADCScanRate[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createSetBoostValveDCAGains(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->setBoostValveDCAGains(buffer, this->subnetData[i].FAAddress[j], this->forceInfo.DCAAxialGain[j], this->forceInfo.DCALateralGain[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createResetBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reset(buffer, this->subnetData[i].FAAddress[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reset(buffer, this->subnetData[i].HPAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReportServerIDBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportServerID(buffer, this->subnetData[i].FAAddress[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reportServerID(buffer, this->subnetData[i].HPAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReportServerStatusBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportServerStatus(buffer, this->subnetData[i].FAAddress[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reportServerStatus(buffer, this->subnetData[i].HPAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReportADCScanRateBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportADCScanRate(buffer, this->subnetData[i].FAAddress[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reportADCScanRate(buffer, this->subnetData[i].HPAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReadCalibrationBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->readCalibration(buffer, this->subnetData[i].FAAddress[j]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->readCalibration(buffer, this->subnetData[i].HPAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReadBoostValveDCAGainsBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->readBoostValveDCAGains(buffer, this->subnetData[i].FAAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReportDCAIDBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportDCAID(buffer, this->subnetData[i].FAAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createReportDCAStatusBuffer(ModbusBuffer* buffer) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportDCAStatus(buffer, this->subnetData[i].FAAddress[j]);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createChangeILCModeBuffer(ModbusBuffer* buffer, ILCModes::Type mode) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->changeILCMode(buffer, this->subnetData[i].FAAddress[j], mode);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->changeILCMode(buffer, this->subnetData[i].HPAddress[j], mode);
		}
		this->endSubnet(buffer);
	}
	buffer->setLength(buffer->getIndex());
}

void ILC::createFreezeSensorListBuffer() {
	this->bufferFreezeSensorList.freezeSensorCommandIndex[0] = -1;
	this->bufferFreezeSensorList.freezeSensorCommandIndex[1] = -1;
	this->bufferFreezeSensorList.freezeSensorCommandIndex[2] = -1;
	this->bufferFreezeSensorList.freezeSensorCommandIndex[3] = -1;
	this->bufferFreezeSensorList.freezeSensorCommandIndex[4] = -1;
	this->bufferFreezeSensorList.faStatusCommandIndex[0] = -1;
	this->bufferFreezeSensorList.faStatusCommandIndex[1] = -1;
	this->bufferFreezeSensorList.faStatusCommandIndex[2] = -1;
	this->bufferFreezeSensorList.faStatusCommandIndex[3] = -1;
	this->bufferFreezeSensorList.faStatusCommandIndex[4] = -1;
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->bufferFreezeSensorList.buffer, i);
		this->bufferFreezeSensorList.freezeSensorCommandIndex[i] = this->bufferFreezeSensorList.buffer.getIndex();
		this->broadcastFreezeSensorValues(&this->bufferFreezeSensorList.buffer, this->broadcastCounter);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->pneumaticForceStatus(&this->bufferFreezeSensorList.buffer, this->subnetData[i].FAAddress[j]);
		}
		if (this->subnetData[i].FAAddress[0] != 255) {
			this->bufferFreezeSensorList.faStatusCommandIndex[i] = this->bufferFreezeSensorList.buffer.getIndex();
			this->reportServerStatus(&this->bufferFreezeSensorList.buffer, this->subnetData[i].FAAddress[0]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->electromechanicalForceAndStatus(&this->bufferFreezeSensorList.buffer, this->subnetData[i].HPAddress[j]);
			this->reportServerStatus(&this->bufferFreezeSensorList.buffer, this->subnetData[i].HPAddress[j]);
		}
		this->endSubnet(&this->bufferFreezeSensorList.buffer);
	}
	this->bufferFreezeSensorList.buffer.setLength(this->bufferFreezeSensorList.buffer.getIndex());
}

void ILC::reportServerID(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(17);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReportServerID);
}

void ILC::reportServerStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(18);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReportServerStatus);
}

void ILC::changeILCMode(ModbusBuffer* buffer, uint8_t address, uint16_t mode) {
	buffer->writeU8(address);
	buffer->writeU8(65);
	buffer->writeU16(mode);
	buffer->writeCRC(4);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ChangeILCMode);
}

void ILC::reportILCMode(ModbusBuffer* buffer, uint8_t address) {
	this->changeILCMode(buffer, address, ILCModes::NoChange);
}

void ILC::broadcastStepMotor(ModbusBuffer* buffer, uint8_t broadcastCounter, int32_t count, int8_t* steps) {
	buffer->writeU8(248);
	buffer->writeU8(66);
	buffer->writeU8(broadcastCounter);
	for(int i = 0; i < 78; i++) {
		buffer->writeI8((i < count) ? steps[i] : 0);
	}
	buffer->writeCRC(81);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings.BroadcastStepMotor);
}

void ILC::unicastStepMotor(ModbusBuffer* buffer, uint8_t address, int8_t steps) {
	buffer->writeU8(address);
	buffer->writeU8(66);
	buffer->writeI8(steps);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.UnicastStepMotor);
}

void ILC::electromechanicalForceAndStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(67);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ElectromechanicalForceAndStatus);
}

void ILC::broadcastFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter) {
	buffer->writeU8(248);
	buffer->writeU8(68);
	buffer->writeU8(broadcastCounter);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings.BroadcastFreezeSensorValues);
}

void ILC::setBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address, float primaryGain, float secondaryGain) {
	buffer->writeU8(address);
	buffer->writeU8(73);
	buffer->writeSGL(primaryGain);
	buffer->writeSGL(secondaryGain);
	buffer->writeCRC(10);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.SetBoostValveDCAGains);
}

void ILC::readBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(74);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReadBoostValveDCAGains);
}

void ILC::broadcastForceDemand(ModbusBuffer* buffer, uint8_t broadcastCounter, bool slewFlag, int32_t saaCount, int32_t* saaPrimarySetpoint, int32_t daaCount, int32_t* daaPrimarySetpoint, int32_t* daaSecondarySetpoint) {
	buffer->writeU8(248);
	buffer->writeU8(75);
	buffer->writeU8(broadcastCounter);
	buffer->writeU8(slewFlag ? 255 : 0);
	for(int i = 0; i < 16; i++) {
		buffer->writeI24((i < saaCount) ? saaPrimarySetpoint[i] : 0);
	}
	for(int i = 0; i < 32; i++) {
		buffer->writeI24((i < daaCount) ? daaPrimarySetpoint[i] : 0);
		buffer->writeI24((i < daaCount) ? daaSecondarySetpoint[i] : 0);
	}
	buffer->writeCRC(244);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings.BroadcastForceDemand);
}

void ILC::unicastSingleAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint) {
	buffer->writeU8(address);
	buffer->writeU8(75);
	buffer->writeU8(slewFlag ? 255 : 0);
	buffer->writeI24(primarySetpoint);
	buffer->writeCRC(6);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.UnicastSingleAxisForceDemand);
}

void ILC::unicastDualAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint, int32_t secondarySetpoint) {
	buffer->writeU8(address);
	buffer->writeU8(75);
	buffer->writeU8(slewFlag ? 255 : 0);
	buffer->writeI24(primarySetpoint);
	buffer->writeI24(secondarySetpoint);
	buffer->writeCRC(9);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.UnicastDualAxisForceDemand);
}

void ILC::pneumaticForceStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(76);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.PneumaticForceAndStatus);
}

void ILC::setADCScanRate(ModbusBuffer* buffer, uint8_t address, uint8_t rate) {
	buffer->writeU8(address);
	buffer->writeU8(80);
	buffer->writeU8(rate);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.SetADCScanRate);
}

void ILC::reportADCScanRate(ModbusBuffer* buffer, uint8_t address) {
	this->setADCScanRate(buffer, address, ILCScanRates::NoChange);
}

void ILC::setADCChannelOffsetAndSensitivity(ModbusBuffer* buffer, uint8_t address, uint8_t channel, float offset, float sensitivity) {
	buffer->writeU8(address);
	buffer->writeU8(81);
	buffer->writeU8(channel);
	buffer->writeSGL(offset);
	buffer->writeSGL(sensitivity);
	buffer->writeCRC(11);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.SetADCChannelOffsetAndSensitivity);
}

void ILC::reset(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(107);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.Reset);
}

void ILC::readCalibration(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(110);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReadCalibration);
}

void ILC::readDCAPressureValues(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(119);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReadDCAPressureValues);
}

void ILC::reportDCAID(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(120);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReportDCAID);
}

void ILC::reportDCAStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(121);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings.ReportDCAStatus);
}

void ILC::parse(ModbusBuffer* buffer, uint8_t subnet) {
	while(!buffer->endOfBuffer()) {
		uint16_t length = 0;
		double timestamp = 0;
		if (this->validateCRC(buffer, &length, &timestamp)) {
			if(subnet >= 1 && subnet <= 5) {
				uint8_t address = buffer->readU8();
				uint8_t function = buffer->readU8();
				ILCMap map = this->subnetData[subnet - 1].ILCDataFromAddress[address];
				int32_t dataIndex = map.DataIndex;
				switch(map.Type) {
				case ILCTypes::FA:
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
					switch(function) {
					case 17: this->parseReportHPServerIDResponse(buffer, dataIndex); break;
					case 18: this->parseReportHPServerStatusResponse(buffer, dataIndex); break;
					case 65: this->parseChangeHPILCModeResponse(buffer, dataIndex); break;
					case 66: this->parseStepMotorResponse(buffer, dataIndex); break;
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
				default: this->warnUnknownAddress(timestamp, map.ActuatorId); break;
				}
				break;
			}
			else {
				this->warnUnknownSubnet(timestamp);
			}
		}
		else {
			cout << "Timestamp " << timestamp << " Subnet " << subnet << endl;
			this->warnInvalidCRC(timestamp);
		}
	}
}

bool ILC::validateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp) {
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

void ILC::parseErrorResponse(ModbusBuffer* buffer, double timestamp, int32_t actuatorId) {
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

void ILC::parseReportHPServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->hardpointInfo.ILCUniqueId[dataIndex] = buffer->readU48();
	this->hardpointInfo.ILCApplicationType[dataIndex] = buffer->readU8();
	this->hardpointInfo.NetworkNodeType[dataIndex] = buffer->readU8();
	this->hardpointInfo.ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->hardpointInfo.NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->hardpointInfo.MajorRevision[dataIndex] = buffer->readU8();
	this->hardpointInfo.MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILC::parseReportFAServerIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t length = buffer->readU8();
	this->forceInfo.ILCUniqueId[dataIndex] = buffer->readU48();
	this->forceInfo.ILCApplicationType[dataIndex] = buffer->readU8();
	this->forceInfo.NetworkNodeType[dataIndex] = buffer->readU8();
	this->forceInfo.ILCSelectedOptions[dataIndex] = buffer->readU8();
	this->forceInfo.NetworkNodeOptions[dataIndex] = buffer->readU8();
	this->forceInfo.MajorRevision[dataIndex] = buffer->readU8();
	this->forceInfo.MinorRevision[dataIndex] = buffer->readU8();
	buffer->incIndex(length - 12);
	buffer->skipToNextFrame();
}

void ILC::parseReportHPServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointStatus.Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->hardpointStatus.MajorFault[dataIndex] = ilcStatus & 0x0001;
	this->hardpointStatus.MinorFault[dataIndex] = ilcStatus & 0x0002;
	// 0x0004 is reserved
	this->hardpointStatus.FaultOverridden[dataIndex] = ilcStatus & 0x0008;
	this->hardpointInfo.MainCalibrationError[dataIndex] = ilcStatus & 0x0010;
	this->hardpointInfo.BackupCalibrationError[dataIndex] = ilcStatus & 0x0020;
	// 0x0040 is reserved
	// 0x0080 is reserved
	this->hardpointData.CWLimitOperated[dataIndex] = ilcStatus & 0x0100;
	this->hardpointData.CCWLimitOperated[dataIndex] = ilcStatus & 0x0200;
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	// 0x2000 is DCA (FA only)
	// 0x4000 is DCA (FA only)
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->hardpointInfo.UniqueIdCRCError[dataIndex] = ilcFaults & 0x0001;
	this->hardpointInfo.ApplicationTypeMismatch[dataIndex] = ilcFaults & 0x0002;
	this->hardpointInfo.ApplicationMissing[dataIndex] = ilcFaults & 0x0004;
	this->hardpointInfo.ApplicationCRCMismatch[dataIndex] = ilcFaults & 0x0008;
	this->hardpointInfo.OneWireMissing[dataIndex] = ilcFaults & 0x0010;
	this->hardpointInfo.OneWire1Mismatch[dataIndex] = ilcFaults & 0x0020;
	this->hardpointInfo.OneWire2Mismatch[dataIndex] = ilcFaults & 0x0040;
	// 0x0080 is reserved
	this->hardpointStatus.WatchdogReset[dataIndex] = ilcFaults & 0x0100;
	this->hardpointStatus.BrownoutDetected[dataIndex] = ilcFaults & 0x0200;
	this->hardpointStatus.EventTrapReset[dataIndex] = ilcFaults & 0x0400;
	this->hardpointStatus.MotorPowerFault[dataIndex] = ilcFaults & 0x0800;
	this->hardpointStatus.SSRPowerFault[dataIndex] = ilcFaults & 0x1000;
	this->hardpointStatus.AUXPowerFault[dataIndex] = ilcFaults & 0x2000;
	this->hardpointStatus.SMCPowerFault[dataIndex] = ilcFaults & 0x4000;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILC::parseReportFAServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceStatus.Mode[dataIndex] = buffer->readU8();
	uint16_t ilcStatus = buffer->readU16();
	this->forceStatus.MajorFault[dataIndex] = ilcStatus & 0x0001;
	this->forceStatus.MinorFault[dataIndex] = ilcStatus & 0x0002;
	// 0x0004 is reserved
	this->forceStatus.FaultOverridden[dataIndex] = ilcStatus & 0x0008;
	this->forceInfo.MainCalibrationError[dataIndex] = ilcStatus & 0x0010;
	this->forceInfo.BackupCalibrationError[dataIndex] = ilcStatus & 0x0020;
	// 0x0040 is reserved
	// 0x0080 is reserved
	// 0x0100 is limit switch (HP only)
	// 0x0200 is limit switch (HP only)
	// 0x0400 is reserved
	// 0x0800 is reserved
	// 0x1000 is reserved
	this->forceStatus.DCAFault[dataIndex] = ilcStatus & 0x2000;
	this->forceInfo.DCAFirmwareUpdate[dataIndex] = ilcStatus & 0x4000;
	// 0x8000 is reserved
	uint16_t ilcFaults = buffer->readU16();
	this->forceInfo.UniqueIdCRCError[dataIndex] = ilcFaults & 0x0001;
	this->forceInfo.ApplicationTypeMismatch[dataIndex] = ilcFaults & 0x0002;
	this->forceInfo.ApplicationMissing[dataIndex] = ilcFaults & 0x0004;
	this->forceInfo.ApplicationCRCMismatch[dataIndex] = ilcFaults & 0x0008;
	this->forceInfo.OneWireMissing[dataIndex] = ilcFaults & 0x0010;
	this->forceInfo.OneWire1Mismatch[dataIndex] = ilcFaults & 0x0020;
	this->forceInfo.OneWire2Mismatch[dataIndex] = ilcFaults & 0x0040;
	// 0x0080 is reserved
	this->forceStatus.WatchdogReset[dataIndex] = ilcFaults & 0x0100;
	this->forceStatus.BrownoutDetected[dataIndex] = ilcFaults & 0x0200;
	this->forceStatus.EventTrapReset[dataIndex] = ilcFaults & 0x0400;
	this->forceStatus.MotorPowerFault[dataIndex] = ilcFaults & 0x0800;
	this->forceStatus.SSRPowerFault[dataIndex] = ilcFaults & 0x1000;
	this->forceStatus.AUXPowerFault[dataIndex] = ilcFaults & 0x2000;
	this->forceStatus.SMCPowerFault[dataIndex] = ilcFaults & 0x4000;
	// 0x8000 is reserved
	buffer->skipToNextFrame();
}

void ILC::parseChangeHPILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointStatus.Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseChangeFAILCModeResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceStatus.Mode[dataIndex] = buffer->readU8();
	buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseStepMotorResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->hardpointData.ILCFault[dataIndex] = status & 0x01;
	// 0x02 is reserved
	this->hardpointData.CWLimitOperated[dataIndex] = status & 0x04;
	this->hardpointData.CCWLimitOperated[dataIndex] = status & 0x08;
	this->hardpointData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->hardpointData.Encoder[dataIndex] = buffer->readI32();
	this->hardpointData.Force[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseElectromechanicalForceAndStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->hardpointData.ILCFault[dataIndex] = status & 0x01;
	// 0x02 is reserved
	this->hardpointData.CWLimitOperated[dataIndex] = status & 0x04;
	this->hardpointData.CCWLimitOperated[dataIndex] = status & 0x08;
	this->hardpointData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->hardpointData.Encoder[dataIndex] = buffer->readI32();
	this->hardpointData.Force[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseSetBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseReadBoostValveDCAGainsResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.DCAAxialGain[dataIndex] = buffer->readSGL();
	this->forceInfo.DCALateralGain[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseForceDemandResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex) {
	if (address <= 16) {
		this->parseSingleAxisForceDemandResponse(buffer, dataIndex);
	}
	else {
		this->parseDualAxisForceDemandResponse(buffer, dataIndex);
	}
}

void ILC::parseSingleAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.ZForce[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseDualAxisForceDemandResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.ZForce[dataIndex] = buffer->readSGL();
	this->forceData.YForce[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parsePneumaticForceStatusResponse(ModbusBuffer* buffer, uint8_t address, int32_t dataIndex) {
	if (address <= 16) {
		this->parseSingleAxisPneumaticForceStatusResponse(buffer, dataIndex);
	}
	else {
		this->parseDualAxisPneumaticForceStatusResponse(buffer, dataIndex);
	}
}

void ILC::parseSingleAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.ZForce[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseDualAxisPneumaticForceStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint8_t status = buffer->readU8();
	this->forceData.ILCFault[dataIndex] = status & 0x01;
	this->forceData.DCAFault[dataIndex] = status & 0x02;
	// 0x04 is reserved
	// 0x08 is reserved
	this->forceData.BroadcastCounter[dataIndex] = (status & 0xF0) >> 4;
	this->forceData.ZForce[dataIndex] = buffer->readSGL();
	this->forceData.YForce[dataIndex] = buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseSetHPADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointInfo.ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseSetFAADCScanRateResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.ADCScanRate[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseSetHPADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseSetFAADCChannelOffsetAndSensitivityResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseHPResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseFAResetResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->skipToNextFrame();
}

void ILC::parseReadHPCalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->hardpointInfo.MainADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->hardpointInfo.MainLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo.MainLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->hardpointInfo.BackupLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->hardpointInfo.BackupLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseReadFACalibrationResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.MainADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->forceInfo.MainADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->forceInfo.MainADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->forceInfo.MainADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->forceInfo.MainAxialLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceInfo.MainLateralLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo.MainAxialLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceInfo.MainLateralLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK1[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK2[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK3[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupADCCalibrationK4[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupAxialLoadCellOffset[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupLateralLoadCellOffset[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	this->forceInfo.BackupAxialLoadCellSensitivity[dataIndex] = buffer->readSGL();
	this->forceInfo.BackupLateralLoadCellSensitivity[dataIndex] = buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->readSGL();
	buffer->skipToNextFrame();
}

void ILC::parseReportDCAIDResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	this->forceInfo.DCAUniqueId[dataIndex] = buffer->readU48();
	this->forceInfo.DCAFirmwareType[dataIndex] = buffer->readU8();
	this->forceInfo.DCAMajorRevision[dataIndex] = buffer->readU8();
	this->forceInfo.DCAMinorRevision[dataIndex] = buffer->readU8();
	buffer->skipToNextFrame();
}

void ILC::parseReportDCAStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
	uint16_t status = buffer->readU16();
	this->forceStatus.DCAOutputsEnabled[dataIndex] = status & 0x0001;
	this->forceStatus.DCAPowerFault[dataIndex] = status & 0x0002;
	this->forceStatus.DCAAmplifierAFault[dataIndex] = status & 0x0004;
	this->forceStatus.DCAAmplifierBFault[dataIndex] = status & 0x0008;
	this->forceInfo.DCAUniqueIdCRCError[dataIndex] = status & 0x0010;
	// 0x0020 is reserved
	this->forceInfo.DCAMainCalibrationError[dataIndex] = status & 0x0040;
	this->forceInfo.DCABackupCalibrationError[dataIndex] = status & 0x0080;
	this->forceStatus.DCAEventTrapReset[dataIndex] = status & 0x0100;
	// 0x0200 is reserved
	// 0x0400 is reserved
	// 0x0800 is reserved
	this->forceInfo.DCAApplicationMissing[dataIndex] = status & 0x1000;
	this->forceInfo.DCAApplicationCRCMismatch[dataIndex] = status & 0x2000;
	// 0x4000 is reserved
	this->forceInfo.DCABootloaderActive[dataIndex] = status & 0x8000;
	buffer->skipToNextFrame();
}

void ILC::incBroadcastCounter() {
	this->broadcastCounter++;
	if (this->broadcastCounter > 15) {
		this->broadcastCounter = 0;
	}
}

void ILC::incFAStatusIndex() {
	for (int i = 0; i < SUBNET_COUNT; i++) {
		if (this->faStatusIndex[i] != -1) {
			this->faStatusIndex[i]++;
			if(this->subnetData[i].FAAddress[this->faStatusIndex[i]] == 255) {
				this->faStatusIndex[i] = 0;
			}
		}
	}
}

void ILC::warnResponseTimeout(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = true;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnInvalidCRC(double timestamp) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = -1;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = true;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnIllegalFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = true;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnIllegalDataValue(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = true;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnInvalidLength(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = true;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownSubnet(double timestamp) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = -1;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = true;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownAddress(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = true;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownFunction(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = true;
	this->ilcWarning.UnknownProblem = false;
	this->publisher->logILCWarning(&this->ilcWarning);
}

void ILC::warnUnknownProblem(double timestamp, int32_t actuatorId) {
	this->ilcWarning.Timestamp = timestamp;
	this->ilcWarning.ActuatorId = actuatorId;
	this->ilcWarning.ResponseTimeout = false;
	this->ilcWarning.InvalidCRC = false;
	this->ilcWarning.IllegalFunction = false;
	this->ilcWarning.IllegalDataValue = false;
	this->ilcWarning.InvalidLength = false;
	this->ilcWarning.UnknownSubnet = false;
	this->ilcWarning.UnknownAddress = false;
	this->ilcWarning.UnknownFunction = false;
	this->ilcWarning.UnknownProblem = true;
	this->publisher->logILCWarning(&this->ilcWarning);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
