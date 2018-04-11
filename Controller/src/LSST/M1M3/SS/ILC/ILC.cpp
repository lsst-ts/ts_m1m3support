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
#include <Log.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorSettings.h>
#include <RoundRobin.h>
#include <PositionController.h>

#define ADDRESS_COUNT 256

namespace LSST {
namespace M1M3 {
namespace SS {

ILC::ILC(M1M3SSPublisher* publisher, FPGA* fpga, PositionController* positionController, ILCApplicationSettings* ilcApplicationSettings, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointActuatorSettings* hardpointActuatorSettings, HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings)
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
   busListRaised(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData(), publisher->getForceActuatorData(), publisher->getHardpointActuatorData(), publisher->getEventForceActuatorInfo(), publisher->getEventAppliedCylinderForces()),
   busListActive(&this->subnetData, &this->ilcMessageFactory, publisher->getOuterLoopData(), publisher->getForceActuatorData(), publisher->getHardpointActuatorData(), publisher->getEventForceActuatorInfo(), publisher->getEventAppliedCylinderForces()) {
	Log.Debug("ILC: ILC()");
	this->publisher = publisher;
	this->fpga = fpga;
	this->hardpointActuatorSettings = hardpointActuatorSettings;
	this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorData = this->publisher->getForceActuatorData();
	this->controlListToggle = 0;
	this->positionController = positionController;
}

ILC::~ILC() { }

void ILC::writeCalibrationDataBuffer() {
	Log.Debug("ILC: writeCalibrationDataBuffer()");
	this->writeBusList(&this->busListSetADCChannelOffsetAndSensitivity);
}

void ILC::writeSetADCScanRateBuffer() {
	Log.Debug("ILC: writeSetADCScanRateBuffer()");
	this->writeBusList(&this->busListSetADCScanRate);
}

void ILC::writeSetBoostValveDCAGainBuffer() {
	Log.Debug("ILC: writeSetBoostValveDCAGainBuffer()");
	this->writeBusList(&this->busListSetBoostValveDCAGains);
}

void ILC::writeResetBuffer() {
	Log.Debug("ILC: writeResetBuffer()");
	this->writeBusList(&this->busListReset);
}

void ILC::writeReportServerIDBuffer() {
	Log.Debug("ILC: writeReportServerIDBuffer()");
	this->writeBusList(&this->busListReportServerID);
}

void ILC::writeReportServerStatusBuffer() {
	Log.Debug("ILC: writeReportServerStatusBuffer()");
	this->writeBusList(&this->busListReportServerStatus);
}

void ILC::writeReportADCScanRateBuffer() {
	Log.Debug("ILC: writeReportADCScanRateBuffer()");
	this->writeBusList(&this->busListReportADCScanRate);
}

void ILC::writeReadCalibrationDataBuffer() {
	Log.Debug("ILC: writeReadCalibrationDataBuffer()");
	this->writeBusList(&this->busListReadCalibration);
}

void ILC::writeReadBoostValveDCAGainBuffer() {
	Log.Debug("ILC: writeReadBoostValveDCAGainBuffer()");
	this->writeBusList(&this->busListReadBoostValveDCAGains);
}

void ILC::writeReportDCAIDBuffer() {
	Log.Debug("ILC: writeReportDCAIDBuffer()");
	this->writeBusList(&this->busListReportDCAID);
}

void ILC::writeReportDCAStatusBuffer() {
	Log.Debug("ILC: writeReportDCAStatusBuffer()");
	this->writeBusList(&this->busListReportDCAStatus);
}

void ILC::writeSetModeDisableBuffer() {
	Log.Debug("ILC: writeSetModeDisableBuffer()");
	this->writeBusList(&this->busListChangeILCModeDisabled);
}

void ILC::writeSetModeEnableBuffer() {
	Log.Debug("ILC: writeSetModeEnableBuffer()");
	this->writeBusList(&this->busListChangeILCModeEnabled);
}

void ILC::writeSetModeStandbyBuffer() {
	Log.Debug("ILC: writeSetModeStandbyBuffer()");
	this->writeBusList(&this->busListChangeILCModeStandby);
}

void ILC::writeFreezeSensorListBuffer() {
	Log.Debug("ILC: writeFreezeSensorListBuffer()");
	this->busListFreezeSensor.update();
	this->writeBusList(&this->busListFreezeSensor);
}

void ILC::writeRaisedListBuffer() {
	Log.Debug("ILC: writeRaisedListBuffer()");
	this->positionController->updateSteps();
	this->busListRaised.update();
	this->writeBusList(&this->busListRaised);
}

void ILC::writeActiveListBuffer() {
	Log.Debug("ILC: writeActiveListBuffer()");
	this->busListActive.update();
	this->writeBusList(&this->busListActive);
}

void ILC::writeControlListBuffer() {
	Log.Debug("ILC: writeControlListBuffer()");
	if (this->controlListToggle == 0) {
		this->writeRaisedListBuffer();
	}
	else {
		this->writeActiveListBuffer();
	}
	this->controlListToggle = RoundRobin::Inc(this->controlListToggle, 3);
}

void ILC::triggerModbus() {
	Log.Debug("ILC: triggerModbus()");
	this->fpga->writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
}

void ILC::waitForSubnet(int32_t subnet, int32_t timeout) {
	Log.Debug("ILC: waitForSubnet(%d, %d)", subnet, timeout);
	this->fpga->waitForModbusIRQ(subnet, timeout);
	this->fpga->ackModbusIRQ(subnet);
}

void ILC::waitForAllSubnets(int32_t timeout) {
	Log.Debug("ILC: waitForAllSubnets(%d)", timeout);
	this->waitForSubnet(1, timeout);
	this->waitForSubnet(2, timeout);
	this->waitForSubnet(3, timeout);
	this->waitForSubnet(4, timeout);
	this->waitForSubnet(5, timeout);
}

void ILC::read(uint8_t subnet) {
	Log.Debug("ILC: read(%d)", subnet);
	// TODO: The expectation is if someone asks to read something they expect something to be there
	// so if something isn't there should be a warning (timeout on responses)
	this->u16Buffer[0] = this->subnetToRxAddress(subnet);
	this->fpga->writeRequestFIFO(this->u16Buffer, 1, 0);
	this->rxBuffer.setIndex(0);
	this->fpga->readU16ResponseFIFO(this->rxBuffer.getBuffer(), 1, 10);
	uint16_t reportedLength = this->rxBuffer.readLength();
	if (reportedLength > 0) {
		this->rxBuffer.setIndex(0);
		if (this->fpga->readU16ResponseFIFO(this->rxBuffer.getBuffer(), reportedLength, 10)) {
			Log.Warn("ILC: Failed to read all %d words", reportedLength);
		}
		this->rxBuffer.setLength(reportedLength);
		this->responseParser.parse(&this->rxBuffer, subnet);
	}
}

void ILC::readAll() {
	Log.Debug("ILC: readAll()");
	this->read(1);
	this->read(2);
	this->read(3);
	this->read(4);
	this->read(5);
}

void ILC::calculateHPPostion() {
	this->hardpointActuatorData->XPosition =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[0] * this->hardpointActuatorData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[1] * this->hardpointActuatorData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[2] * this->hardpointActuatorData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[3] * this->hardpointActuatorData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[4] * this->hardpointActuatorData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[5] * this->hardpointActuatorData->Displacement[1];
	this->hardpointActuatorData->YPosition =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[6] * this->hardpointActuatorData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[7] * this->hardpointActuatorData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[8] * this->hardpointActuatorData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[9] * this->hardpointActuatorData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[10] * this->hardpointActuatorData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[11] * this->hardpointActuatorData->Displacement[1];
	this->hardpointActuatorData->ZPosition =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[12] * this->hardpointActuatorData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[13] * this->hardpointActuatorData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[14] * this->hardpointActuatorData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[15] * this->hardpointActuatorData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[16] * this->hardpointActuatorData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[17] * this->hardpointActuatorData->Displacement[1];
	this->hardpointActuatorData->XRotation =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[18] * this->hardpointActuatorData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[19] * this->hardpointActuatorData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[20] * this->hardpointActuatorData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[21] * this->hardpointActuatorData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[22] * this->hardpointActuatorData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[23] * this->hardpointActuatorData->Displacement[1];
	this->hardpointActuatorData->YRotation =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[24] * this->hardpointActuatorData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[25] * this->hardpointActuatorData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[26] * this->hardpointActuatorData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[27] * this->hardpointActuatorData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[28] * this->hardpointActuatorData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[29] * this->hardpointActuatorData->Displacement[1];
	this->hardpointActuatorData->ZRotation =
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[30] * this->hardpointActuatorData->Displacement[2] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[31] * this->hardpointActuatorData->Displacement[3] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[32] * this->hardpointActuatorData->Displacement[4] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[33] * this->hardpointActuatorData->Displacement[5] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[34] * this->hardpointActuatorData->Displacement[0] +
			this->hardpointActuatorSettings->HardpointDisplacementToMirrorPosition[35] * this->hardpointActuatorData->Displacement[1];
}

void ILC::calculateHPMirrorForces() {
	std::vector<float> m = this->forceActuatorSettings->HardpointForceMomentTable;
	float* force = this->hardpointActuatorData->MeasuredForce;
	this->hardpointActuatorData->Fx = m[0] * force[0] + m[1] * force[1] + m[2] * force[2] + m[3] * force[3] + m[4] * force[4] + m[5] * force[5];
	this->hardpointActuatorData->Fy = m[6] * force[0] + m[7] * force[1] + m[8] * force[2] + m[9] * force[3] + m[10] * force[4] + m[11] * force[5];
	this->hardpointActuatorData->Fz = m[12] * force[0] + m[13] * force[1] + m[14] * force[2] + m[15] * force[3] + m[16] * force[4] + m[17] * force[5];
	this->hardpointActuatorData->Mx = m[18] * force[0] + m[19] * force[1] + m[20] * force[2] + m[21] * force[3] + m[22] * force[4] + m[23] * force[5];
	this->hardpointActuatorData->My = m[24] * force[0] + m[25] * force[1] + m[26] * force[2] + m[27] * force[3] + m[28] * force[4] + m[29] * force[5];
	this->hardpointActuatorData->Mz = m[30] * force[0] + m[31] * force[1] + m[32] * force[2] + m[33] * force[3] + m[34] * force[4] + m[35] * force[5];
}

void ILC::calculateFAMirrorForces() {
	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->forceActuatorData->XForce, this->forceActuatorData->YForce, this->forceActuatorData->ZForce);
	this->forceActuatorData->Fx = fm.Fx;
	this->forceActuatorData->Fy = fm.Fy;
	this->forceActuatorData->Fz = fm.Fz;
	this->forceActuatorData->Mx = fm.Mx;
	this->forceActuatorData->My = fm.My;
	this->forceActuatorData->Mz = fm.Mz;
	this->forceActuatorData->ForceMagnitude = fm.ForceMagnitude;
}

void ILC::clearResponses() {
	Log.Debug("ILC: clearResponses()");
	this->responseParser.clearResponses();
}

void ILC::verifyResponses() {
	Log.Debug("ILC: verifyResponses()");
	this->responseParser.verifyResponses();
}

void ILC::publishForceActuatorInfo() {
	this->publisher->getEventForceActuatorInfo()->Timestamp = this->publisher->getTimestamp();
	this->publisher->logForceActuatorInfo();
}

void ILC::publishForceActuatorStatus() {
	//this->publisher->putForceActuatorStatus();
}

void ILC::publishForceActuatorData() {
	this->publisher->putForceActuatorData();
}

void ILC::publishHardpointActuatorInfo() {
	this->publisher->getEventHardpointActuatorInfo()->Timestamp = this->publisher->getTimestamp();
	this->publisher->logHardpointActuatorInfo();
}

void ILC::publishHardpointStatus() {
	//this->publisher->putHardpointStatus();
}

void ILC::publishHardpointData() {
	this->publisher->putHardpointActuatorData();
}

void ILC::publishHardpointMonitorInfo() {
	this->publisher->getEventHardpointMonitorInfo()->Timestamp = this->publisher->getTimestamp();
	this->publisher->logHardpointMonitorInfo();
}

void ILC::publishHardpointMonitorStatus() {
	//this->publisher->putHardpointMonitorStatus();
}

void ILC::publishHardpointMonitorData() {
	this->publisher->putHardpointMonitorData();
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
	this->responseParser.incExpectedResponses(busList->getExpectedFAResponses(), busList->getExpectedHPResponses(), busList->getExpectedHMResponses());
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
