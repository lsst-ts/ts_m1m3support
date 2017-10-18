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
#include <FPGAAddresses.h>
#include <Timestamp.h>
#include <cstring>

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
	this->createSetADCChannelOffsetAndSensitivityBusList();
	this->createSetADCScanRateBuffer();
	this->createSetBoostValveDCAGains();
	this->createResetBuffer();
	this->createReportServerIDBuffer();
	this->createReportServerStatusBuffer();
	this->createReportADCScanRateBuffer();
	this->createReadCalibrationBuffer();
	this->createReadBoostValveDCAGainsBuffer();
	this->createReportDCAIDBuffer();
	this->createReportDCAStatusBuffer();
	memset(&this->busListChangeILCModeDisabled, 0, sizeof(this->busListChangeILCModeDisabled));
	this->createChangeILCModeBuffer(&this->busListChangeILCModeDisabled, ILCModes::Disabled);
	memset(&this->busListChangeILCModeEnabled, 0, sizeof(this->busListChangeILCModeEnabled));
	this->createChangeILCModeBuffer(&this->busListChangeILCModeEnabled, ILCModes::Enabled);
	memset(&this->busListChangeILCModeStandby, 0, sizeof(this->busListChangeILCModeStandby));
	this->createChangeILCModeBuffer(&this->busListChangeILCModeStandby, ILCModes::Standby);
	this->createFreezeSensorListBuffer();
}

ILC::~ILC() { }

void ILC::writeCalibrationDataBuffer() {
	this->writeStaticBusList(&this->busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
	this->writeStaticBusList(&this->busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
	this->writeStaticBusList(&this->busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
	this->writeStaticBusList(&this->busListReset);
}

void ILC::writeReportServerIDBuffer() {
	this->writeStaticBusList(&this->busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
	this->writeStaticBusList(&this->busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
	this->writeStaticBusList(&this->busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
	this->writeStaticBusList(&this->busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
	this->writeStaticBusList(&this->busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
	this->writeStaticBusList(&this->busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
	this->writeStaticBusList(&this->busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
	this->writeStaticBusList(&this->busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
	this->writeStaticBusList(&this->busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
	this->writeStaticBusList(&this->busListChangeILCModeStandby);
}

void ILC::writeFreezeSensorListBuffer() {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->busListFreezeSensor.buffer.setIndex(this->busListFreezeSensor.freezeSensorCommandIndex[i]);
		this->broadcastFreezeSensorValues(&this->busListFreezeSensor.buffer, this->broadcastCounter);
		int32_t faStatusCommandIndex = this->busListFreezeSensor.faStatusCommandIndex[i];
		if(faStatusCommandIndex != -1) {
			this->busListFreezeSensor.buffer.setIndex(faStatusCommandIndex);
			this->reportServerStatus(&this->busListFreezeSensor.buffer, this->subnetData[i].FAAddress[this->faStatusIndex[i]]);
			this->faExpectedResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[this->faStatusIndex[i]]].DataIndex] += 1;
		}
	}
	this->incBroadcastCounter();
	this->incFAStatusIndex();
	this->incExpectedResponses(this->busListFreezeSensor.expectedFAResponses, this->busListFreezeSensor.expectedHPResponses);
	this->fpga->writeCommandFIFO(this->busListFreezeSensor.buffer.getBuffer(), this->busListFreezeSensor.buffer.getLength(), 0);
}

void ILC::triggerModbus() {
	this->fpga->writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
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
	this->u16Buffer[0] = this->subnetToRxAddress(subnet);
	this->fpga->writeRequestFIFO(this->u16Buffer, 1, 0);
	this->rxBuffer.setIndex(0);
	this->fpga->readU16ResponseFIFO(this->rxBuffer.getBuffer(), 1, 10);
	uint16_t reportedLength = this->rxBuffer.readLength();
	if (reportedLength > 0) {
		this->rxBuffer.setIndex(0);
		this->fpga->readU16ResponseFIFO(this->rxBuffer.getBuffer(), reportedLength, 10);
		this->rxBuffer.setLength(reportedLength);
		this->parse(&this->rxBuffer, subnet);
	}
}

void ILC::readAll() {
	this->read(1);
	this->read(2);
	this->read(3);
	this->read(4);
	this->read(5);
}

void ILC::verifyResponses() {
	double timestamp = this->publisher->getTimestamp();
	for(int i = 0; i < FA_COUNT; i++) {
		if (this->faExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->forceInfo.ReferenceId[i]);
			this->faExpectedResponses[i] = 0;
		}
	}
	for(int i = 0; i < HP_COUNT; i++) {
		if (this->hpExpectedResponses[i] != 0) {
			this->warnResponseTimeout(timestamp, this->hardpointInfo.ReferenceId[i]);
			this->hpExpectedResponses[i] = 0;
		}
	}
}

void ILC::publishForceActuatorStatus() {
	this->publisher->putForceActuatorStatus(&this->forceStatus);
}

void ILC::publishForceActuatorData() {
	this->publisher->putForceActuatorData(&this->forceData);
}

void ILC::publishHardpointStatus() {
	this->publisher->putHardpointStatus(&this->hardpointStatus);
}

void ILC::publishHardpointData() {
	this->publisher->putHardpointData(&this->hardpointData);
}

void ILC::foo() {
	int count = 0;
	for(int i = 0; i < FA_COUNT; i++) {
		cout << this->faExpectedResponses[i] << " ";
		count += this->faExpectedResponses[i];
	}
	cout << endl;
	for(int i = 0; i < HP_COUNT; i++) {
		cout << this->hpExpectedResponses[i] << " ";
		count += this->hpExpectedResponses[i];
	}
	cout << endl;
	cout << "TOTAL: " << count << endl;
}

uint8_t ILC::subnetToTxAddress(uint8_t subnet) {
	switch(subnet + 1) {
	case 1: return FPGAAddresses::ModbusSubnetATx;
	case 2: return FPGAAddresses::ModbusSubnetBTx;
	case 3: return FPGAAddresses::ModbusSubnetCTx;
	case 4: return FPGAAddresses::ModbusSubnetDTx;
	case 5: return FPGAAddresses::ModbusSubnetETx;
	default: return 0;
	}
}

uint8_t ILC::subnetToRxAddress(uint8_t subnet) {
	switch(subnet) {
	case 1: return FPGAAddresses::ModbusSubnetARx;
	case 2: return FPGAAddresses::ModbusSubnetBRx;
	case 3: return FPGAAddresses::ModbusSubnetCRx;
	case 4: return FPGAAddresses::ModbusSubnetDRx;
	case 5: return FPGAAddresses::ModbusSubnetERx;
	default: return 0;
	}
}

void ILC::startSubnet(ModbusBuffer* buffer, uint8_t subnet) {
	subnet = this->subnetToTxAddress(subnet);
	buffer->writeSubnet(subnet);
	this->subnetStartIndex = buffer->getIndex();
	buffer->writeLength(0);
	buffer->writeSoftwareTrigger();
}

void ILC::endSubnet(ModbusBuffer* buffer) {
	buffer->writeTriggerIRQ();
	buffer->set(this->subnetStartIndex, buffer->getIndex() - this->subnetStartIndex - 1);
}

void ILC::createSetADCChannelOffsetAndSensitivityBusList() {
	memset(&this->busListSetADCChannelOffsetAndSensitivity, 0, sizeof(this->busListSetADCChannelOffsetAndSensitivity));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListSetADCChannelOffsetAndSensitivity.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->setADCChannelOffsetAndSensitivity(&this->busListSetADCChannelOffsetAndSensitivity.buffer, this->subnetData[i].FAAddress[j], 1, this->forceInfo.PrimaryAxisSensorOffset[j], this->forceInfo.PrimaryAxisSensorSensitivity[j]);
			this->setADCChannelOffsetAndSensitivity(&this->busListSetADCChannelOffsetAndSensitivity.buffer, this->subnetData[i].FAAddress[j], 2, this->forceInfo.SecondaryAxisSensorOffset[j], this->forceInfo.SecondaryAxisSensorSensitivity[j]);
			this->busListSetADCChannelOffsetAndSensitivity.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 2;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->setADCChannelOffsetAndSensitivity(&this->busListSetADCChannelOffsetAndSensitivity.buffer, this->subnetData[i].HPAddress[j], 1, this->hardpointInfo.SensorOffset[j], this->hardpointInfo.SensorSensitivity[j]);
			this->busListSetADCChannelOffsetAndSensitivity.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListSetADCChannelOffsetAndSensitivity.buffer);
	}
	this->busListSetADCChannelOffsetAndSensitivity.buffer.setLength(this->busListSetADCChannelOffsetAndSensitivity.buffer.getIndex());
}

void ILC::createSetADCScanRateBuffer() {
	memset(&this->busListSetADCScanRate, 0, sizeof(this->busListSetADCScanRate));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListSetADCScanRate.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->setADCScanRate(&this->busListSetADCScanRate.buffer, this->subnetData[i].FAAddress[j], this->forceInfo.ADCScanRate[j]);
			this->busListSetADCScanRate.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->setADCScanRate(&this->busListSetADCScanRate.buffer, this->subnetData[i].HPAddress[j], this->hardpointInfo.ADCScanRate[j]);
			this->busListSetADCScanRate.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListSetADCScanRate.buffer);
	}
	this->busListSetADCScanRate.buffer.setLength(this->busListSetADCScanRate.buffer.getIndex());
}

void ILC::createSetBoostValveDCAGains() {
	memset(&this->busListSetBoostValveDCAGains, 0, sizeof(this->busListSetBoostValveDCAGains));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListSetBoostValveDCAGains.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->setBoostValveDCAGains(&this->busListSetBoostValveDCAGains.buffer, this->subnetData[i].FAAddress[j], this->forceInfo.DCAAxialGain[j], this->forceInfo.DCALateralGain[j]);
			this->busListSetBoostValveDCAGains.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListSetBoostValveDCAGains.buffer);
	}
	this->busListSetBoostValveDCAGains.buffer.setLength(this->busListSetBoostValveDCAGains.buffer.getIndex());
}

void ILC::createResetBuffer() {
	memset(&this->busListReset, 0, sizeof(this->busListReset));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReset.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reset(&this->busListReset.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReset.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reset(&this->busListReset.buffer, this->subnetData[i].HPAddress[j]);
			this->busListReset.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReset.buffer);
	}
	this->busListReset.buffer.setLength(this->busListReset.buffer.getIndex());
}

void ILC::createReportServerIDBuffer() {
	memset(&this->busListReportServerID, 0, sizeof(this->busListReportServerID));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReportServerID.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportServerID(&this->busListReportServerID.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReportServerID.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reportServerID(&this->busListReportServerID.buffer, this->subnetData[i].HPAddress[j]);
			this->busListReportServerID.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReportServerID.buffer);
	}
	this->busListReportServerID.buffer.setLength(this->busListReportServerID.buffer.getIndex());
}

void ILC::createReportServerStatusBuffer() {
	memset(&this->busListReportServerStatus, 0, sizeof(this->busListReportServerStatus));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReportServerStatus.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportServerStatus(&this->busListReportServerStatus.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReportServerStatus.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reportServerStatus(&this->busListReportServerStatus.buffer, this->subnetData[i].HPAddress[j]);
			this->busListReportServerStatus.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReportServerStatus.buffer);
	}
	this->busListReportServerStatus.buffer.setLength(this->busListReportServerStatus.buffer.getIndex());
}

void ILC::createReportADCScanRateBuffer() {
	memset(&this->busListReportADCScanRate, 0, sizeof(this->busListReportADCScanRate));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReportADCScanRate.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportADCScanRate(&this->busListReportADCScanRate.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReportADCScanRate.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->reportADCScanRate(&this->busListReportADCScanRate.buffer, this->subnetData[i].HPAddress[j]);
			this->busListReportADCScanRate.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReportADCScanRate.buffer);
	}
	this->busListReportADCScanRate.buffer.setLength(this->busListReportADCScanRate.buffer.getIndex());
}

void ILC::createReadCalibrationBuffer() {
	memset(&this->busListReadCalibration, 0, sizeof(this->busListReadCalibration));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReadCalibration.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->readCalibration(&this->busListReadCalibration.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReadCalibration.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->readCalibration(&this->busListReadCalibration.buffer, this->subnetData[i].HPAddress[j]);
			this->busListReadCalibration.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReadCalibration.buffer);
	}
	this->busListReadCalibration.buffer.setLength(this->busListReadCalibration.buffer.getIndex());
}

void ILC::createReadBoostValveDCAGainsBuffer() {
	memset(&this->busListReadBoostValveDCAGains, 0, sizeof(this->busListReadBoostValveDCAGains));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReadBoostValveDCAGains.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->readBoostValveDCAGains(&this->busListReadBoostValveDCAGains.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReadBoostValveDCAGains.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReadBoostValveDCAGains.buffer);
	}
	this->busListReadBoostValveDCAGains.buffer.setLength(this->busListReadBoostValveDCAGains.buffer.getIndex());
}

void ILC::createReportDCAIDBuffer() {
	memset(&this->busListReportDCAID, 0, sizeof(this->busListReportDCAID));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReportDCAID.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportDCAID(&this->busListReportDCAID.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReportDCAID.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReportDCAID.buffer);
	}
	this->busListReportDCAID.buffer.setLength(this->busListReportDCAID.buffer.getIndex());
}

void ILC::createReportDCAStatusBuffer() {
	memset(&this->busListReportDCAStatus, 0, sizeof(this->busListReportDCAStatus));
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListReportDCAStatus.buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->reportDCAStatus(&this->busListReportDCAStatus.buffer, this->subnetData[i].FAAddress[j]);
			this->busListReportDCAStatus.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&this->busListReportDCAStatus.buffer);
	}
	this->busListReportDCAStatus.buffer.setLength(this->busListReportDCAStatus.buffer.getIndex());
}

void ILC::createChangeILCModeBuffer(StaticBusList* busList, ILCModes::Type mode) {
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&busList->buffer, i);
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->changeILCMode(&busList->buffer, this->subnetData[i].FAAddress[j], mode);
			busList->expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->changeILCMode(&busList->buffer, this->subnetData[i].HPAddress[j], mode);
			busList->expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 1;
		}
		this->endSubnet(&busList->buffer);
	}
	busList->buffer.setLength(busList->buffer.getIndex());
}

void ILC::createFreezeSensorListBuffer() {
	this->busListFreezeSensor.freezeSensorCommandIndex[0] = -1;
	this->busListFreezeSensor.freezeSensorCommandIndex[1] = -1;
	this->busListFreezeSensor.freezeSensorCommandIndex[2] = -1;
	this->busListFreezeSensor.freezeSensorCommandIndex[3] = -1;
	this->busListFreezeSensor.freezeSensorCommandIndex[4] = -1;
	this->busListFreezeSensor.faStatusCommandIndex[0] = -1;
	this->busListFreezeSensor.faStatusCommandIndex[1] = -1;
	this->busListFreezeSensor.faStatusCommandIndex[2] = -1;
	this->busListFreezeSensor.faStatusCommandIndex[3] = -1;
	this->busListFreezeSensor.faStatusCommandIndex[4] = -1;
	for(int i = 0; i < SUBNET_COUNT; i++) {
		this->startSubnet(&this->busListFreezeSensor.buffer, i);
		this->busListFreezeSensor.freezeSensorCommandIndex[i] = this->busListFreezeSensor.buffer.getIndex();
		this->broadcastFreezeSensorValues(&this->busListFreezeSensor.buffer, this->broadcastCounter);
		this->busListFreezeSensor.buffer.writeTimestamp();
		for(int j = 0; j < FA_COUNT && this->subnetData[i].FAAddress[j] != 255; j++) {
			this->pneumaticForceStatus(&this->busListFreezeSensor.buffer, this->subnetData[i].FAAddress[j]);
			this->busListFreezeSensor.expectedFAResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].FAAddress[j]].DataIndex] = 1;
		}
		if (this->subnetData[i].FAAddress[0] != 255) {
			this->busListFreezeSensor.faStatusCommandIndex[i] = this->busListFreezeSensor.buffer.getIndex();
			this->reportServerStatus(&this->busListFreezeSensor.buffer, this->subnetData[i].FAAddress[0]);
		}
		for(int j = 0; j < HP_COUNT && this->subnetData[i].HPAddress[j] != 255; j++) {
			this->electromechanicalForceAndStatus(&this->busListFreezeSensor.buffer, this->subnetData[i].HPAddress[j]);
			this->reportServerStatus(&this->busListFreezeSensor.buffer, this->subnetData[i].HPAddress[j]);
			this->busListFreezeSensor.expectedHPResponses[this->subnetData[i].ILCDataFromAddress[this->subnetData[i].HPAddress[j]].DataIndex] = 2;
		}
		this->endSubnet(&this->busListFreezeSensor.buffer);
	}
	this->busListFreezeSensor.buffer.setLength(this->busListFreezeSensor.buffer.getIndex());
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
	uint64_t a = buffer->readLength();
	uint64_t b = buffer->readLength();
	uint64_t c = buffer->readLength();
	uint64_t d = buffer->readLength();
	double globalTimestamp = Timestamp::fromRaw((a << 48) | (b << 32) | (c << 16) | d);
	this->forceStatus.Timestamp = globalTimestamp;
	this->forceData.Timestamp = globalTimestamp;
	this->hardpointStatus.Timestamp = globalTimestamp;
	this->hardpointData.Timestamp = globalTimestamp;
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

void ILC::parseStepMotorResponse(ModbusBuffer* buffer, int32_t dataIndex, double timestamp) {
	uint8_t status = buffer->readU8();
	this->hardpointData.Timestamp = timestamp;
	this->hardpointData.StepsCommanded[0] = this->hpStepCommand[0];
	this->hardpointData.StepsCommanded[1] = this->hpStepCommand[1];
	this->hardpointData.StepsCommanded[2] = this->hpStepCommand[2];
	this->hardpointData.StepsCommanded[3] = this->hpStepCommand[3];
	this->hardpointData.StepsCommanded[4] = this->hpStepCommand[4];
	this->hardpointData.StepsCommanded[5] = this->hpStepCommand[5];
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

void ILC::writeStaticBusList(StaticBusList* busList) {
	this->fpga->writeCommandFIFO(busList->buffer.getBuffer(), busList->buffer.getLength(), 0);
	this->incExpectedResponses(busList->expectedFAResponses, busList->expectedHPResponses);
}

void ILC::incExpectedResponses(int32_t* fa, int32_t* hp) {
	for(int i = 0; i < FA_COUNT; i++) {
		this->faExpectedResponses[i] += fa[i];
	}
	for (int i = 0; i < HP_COUNT; i++) {
		this->hpExpectedResponses[i] += hp[i];
	}
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

void ILC::updateHPSteps() {
	for(int i = 0; i < 6; i++) {
		if (this->hpSteps[i] != 0) {
			if (this->hpSteps[i] > 100) {
				this->hpSteps[i] -= 100;
				this->hpStepCommand[i] = 100;
			}
			else if (this->hpSteps[i] < -100) {
				this->hpSteps[i] += 100;
				this->hpStepCommand[i] = -100;
			}
			else {
				this->hpStepCommand[i] = this->hpSteps[i];
				this->hpSteps[i] = 0;
			}
		}
		else {
			this->hpStepCommand[i] = 0;
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
