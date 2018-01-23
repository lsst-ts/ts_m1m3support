/*
 * ILC.cpp
 *
 *  Created on: Oct 1, 2017
 *      Author: ccontaxis
 */

#include <ILC.h>
#include <M1M3SSPublisher.h>
#include <FPGA.h>
#include <DataTypes.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <iostream>
#include <unistd.h>
#include <FPGAAddresses.h>
#include <Timestamp.h>
#include <cstring>
#include <IBusList.h>
#include <SAL_m1m3C.h>
#include <ForceConverter.h>

using namespace std;

#define ADDRESS_COUNT 256

namespace LSST {
namespace M1M3 {
namespace SS {

ILC::ILC(M1M3SSPublisher* publisher, FPGA* fpga, ILCApplicationSettings* ilcApplicationSettings, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointActuatorSettings* hardpointActuatorSettings, HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings)
 : subnetData(forceActuatorApplicationSettings, hardpointActuatorApplicationSettings, hardpointMonitorApplicationSettings),
   ilcMessageFactory(ilcApplicationSettings),
   responseParser(forceActuatorSettings, hardpointActuatorSettings, publisher, &this->subnetData),
   busListSetADCChannelOffsetAndSensitivity(&this->subnetData, &this->ilcMessageFactory, publisher->getEventForceActuatorInfo(), publisher->getEventHardpointActuatorInfo()),
   busListSetADCScanRate(&this->subnetData, &this->ilcMessageFactory, publisher->getEventForceActuatorInfo(), publisher->getEventHardpointActuatorInfo()),
   busListSetBoostValveDCAGains(&this->subnetData, &this->ilcMessageFactory, publisher->getEventForceActuatorInfo()),
   busListReset(&this->subnetData, &this->ilcMessageFactory),
   busListReportServerID(&this->subnetData, &this->ilcMessageFactory),
   busListReportServerStatus(&this->subnetData, &this->ilcMessageFactory),
   busListReportADCScanRate(&this->subnetData, &this->ilcMessageFactory),
   busListReadCalibration(&this->subnetData, &this->ilcMessageFactory),
   busListReadBoostValveDCAGains(&this->subnetData, &this->ilcMessageFactory),
   busListReportDCAID(&this->subnetData, &this->ilcMessageFactory),
   busListReportDCAStatus(&this->subnetData, &this->ilcMessageFactory),
   busListChangeILCModeDisabled(&this->subnetData, &this->ilcMessageFactory, ILCModes::Disabled),
   busListChangeILCModeEnabled(&this->subnetData, &this->ilcMessageFactory, ILCModes::Enabled),
   busListChangeILCModeStandby(&this->subnetData, &this->ilcMessageFactory, ILCModes::Standby),
   busListFreezeSensor(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData()),
   busListRaised(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData(), publisher->getForceActuatorData(), publisher->getHardpointData(), publisher->getEventForceActuatorInfo()) {
	this->publisher = publisher;
	this->fpga = fpga;
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

void ILC::publishForceActuatorInfo() {
	this->publisher->getEventForceActuatorInfo()->Timestamp = this->publisher->getTimestamp();
	this->publisher->logForceActuatorInfo();
}

void ILC::publishForceActuatorStatus() {
	this->publisher->putForceActuatorStatus();
}

void ILC::publishForceActuatorData() {
	this->publisher->putForceActuatorData();
}

void ILC::publishHardpointActuatorInfo() {
	this->publisher->getEventHardpointActuatorInfo()->Timestamp = this->publisher->getTimestamp();
	this->publisher->logHardpointActuatorInfo();
}

void ILC::publishHardpointStatus() {
	this->publisher->putHardpointStatus();
}

void ILC::publishHardpointData() {
	this->publisher->putHardpointData();
}

void ILC::publishHardpointMonitorInfo() {
	this->publisher->getEventHardpointMonitorInfo()->Timestamp = this->publisher->getTimestamp();
	this->publisher->logHardpointMonitorInfo();
}

void ILC::publishHardpointMonitorStatus() {
	this->publisher->putHardpointMonitorStatus();
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
