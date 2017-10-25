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
#include <IBusList.h>
#include <SAL_m1m3C.h>
#include <ForceConverter.h>

using namespace std;

#define SUBNET_COUNT 5
#define FA_COUNT 156
#define HP_COUNT 6
#define ADDRESS_COUNT 256

namespace LSST {
namespace M1M3 {
namespace SS {

ILC::ILC(IPublisher* publisher, IFPGA* fpga, ILCApplicationSettings* ilcApplicationSettings, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings)
 : subnetData(forceActuatorApplicationSettings, hardpointActuatorApplicationSettings),
   ilcMessageFactory(ilcApplicationSettings) {
	this->publisher = publisher;
	this->fpga = fpga;
	this->hardpointInfo = this->publisher->getEventHardpoingActuatorInfo();
	this->forceInfo = this->publisher->getEventForceActuatorInfo();

	this->ilcApplicationSettings = ilcApplicationSettings;

	for(int i = 0; i < FA_COUNT; i++) {
		ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
		this->forceInfo->ReferenceId[row.Index] = row.ActuatorID;
		this->forceInfo->ModbusSubnet[row.Index] = row.Subnet;
		this->forceInfo->ModbusAddress[row.Index] = row.Address;
		this->forceInfo->ActuatorType[row.Index] = row.Type;
		this->forceInfo->ActuatorOrientation[row.Index] = row.Orientation;
		this->forceInfo->XPosition[row.Index] = row.XPosition;
		this->forceInfo->YPosition[row.Index] = row.YPosition;
		this->forceInfo->ZPosition[row.Index] = row.ZPosition;
		this->forceInfo->PrimaryCylinderSensorOffset[row.Index] = row.PrimaryAxisSensorOffset;
		this->forceInfo->PrimaryCylinderSensorSensitivity[row.Index] = row.PrimaryAxisSensorSensitivity;
		this->forceInfo->SecondaryCylinderSensorOffset[row.Index] = row.SecondaryAxisSensorOffset;
		this->forceInfo->SecondaryCylinderSensorSensitivity[row.Index] = row.SecondaryAxisSensorSensitivity;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings->Table[i];
		this->hardpointInfo->ReferenceId[row.Index] = row.ActuatorID;
		this->hardpointInfo->ModbusSubnet[row.Index] = row.Subnet;
		this->hardpointInfo->ModbusAddress[row.Index] = row.Address;
		this->hardpointInfo->XPosition[row.Index] = row.XPosition;
		this->hardpointInfo->YPosition[row.Index] = row.YPosition;
		this->hardpointInfo->ZPosition[row.Index] = row.ZPosition;
		this->hardpointInfo->SensorOffset[row.Index] = row.SensorOffset;
		this->hardpointInfo->SensorSensitivity[row.Index] = row.SensorSensitivity;
	}
	this->responseParser = ILCResponseParser(this->publisher, &this->subnetData);
	this->busListSetADCChannelOffsetAndSensitivity = SetADCChanneOffsetAndSensitivityBusList(&this->subnetData, &this->ilcMessageFactory, this->forceInfo, this->hardpointInfo);
	this->busListSetADCScanRate = SetADCScanRateBusList(&this->subnetData, &this->ilcMessageFactory, this->forceInfo, this->hardpointInfo);
	this->busListSetBoostValveDCAGains = SetBoostValveDCAGainBusList(&this->subnetData, &this->ilcMessageFactory, this->forceInfo);
	this->busListReset = ResetBustList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReportServerID = ReportServerIDBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReportServerStatus = ReportServerStatusBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReportADCScanRate = ReportADCScanRateBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReadCalibration = ReadCalibrationBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReadBoostValveDCAGains = ReadBoostValveDCAGainBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReportDCAID = ReportDCAIDBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListReportDCAStatus = ReportDCAStatusBusList(&this->subnetData, &this->ilcMessageFactory);
	this->busListChangeILCModeDisabled = ChangeILCModeBusList(&this->subnetData, &this->ilcMessageFactory, ILCModes::Disabled);
	this->busListChangeILCModeEnabled = ChangeILCModeBusList(&this->subnetData, &this->ilcMessageFactory, ILCModes::Enabled);
	this->busListChangeILCModeStandby = ChangeILCModeBusList(&this->subnetData, &this->ilcMessageFactory, ILCModes::Standby);
	this->busListFreezeSensor = FreezeSensorBusList(&this->subnetData, &this->ilcMessageFactory, this->publisher->getOuterLoopData());
	this->busListRaised = RaisedBusList(&this->subnetData, &this->ilcMessageFactory, this->publisher->getOuterLoopData(), this->publisher->getForceActuatorData(), this->publisher->getHardpointData(), this->forceInfo);
}

ILC::~ILC() { }

void ILC::writeCalibrationDataBuffer() {
	this->writeBusList(&this->busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
	this->writeBusList(&this->busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
	this->writeBusList(&this->busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
	this->writeBusList(&this->busListReset);
}

void ILC::writeReportServerIDBuffer() {
	this->writeBusList(&this->busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
	this->writeBusList(&this->busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
	this->writeBusList(&this->busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
	this->writeBusList(&this->busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
	this->writeBusList(&this->busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
	this->writeBusList(&this->busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
	this->writeBusList(&this->busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
	this->writeBusList(&this->busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
	this->writeBusList(&this->busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
	this->writeBusList(&this->busListChangeILCModeStandby);
}

void ILC::writeFreezeSensorListBuffer() {
	this->busListFreezeSensor.update();
	this->writeBusList(&this->busListFreezeSensor);
}

void ILC::writeRaisedListBuffer() {
	this->busListRaised.update();
	this->writeBusList(&this->busListRaised);
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
		this->responseParser.parse(&this->rxBuffer, subnet);
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
	this->responseParser.verifyResponses();
}

void ILC::publishForceActuatorStatus() {
	this->publisher->putForceActuatorStatus();
}

void ILC::publishForceActuatorData() {
	this->publisher->putForceActuatorData();
}

void ILC::publishHardpointStatus() {
	this->publisher->putHardpointStatus();
}

void ILC::publishHardpointData() {
	this->publisher->putHardpointData();
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

void ILC::writeBusList(IBusList* busList) {
	this->fpga->writeCommandFIFO(busList->getBuffer(), busList->getLength(), 0);
	this->responseParser.incExpectedResponses(busList->getExpectedFAResponses(), busList->getExpectedHPResponses());
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
