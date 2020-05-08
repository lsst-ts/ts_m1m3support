#include <ILCMessageFactory.h>
#include <ModbusBuffer.h>
#include <ILCApplicationSettings.h>
#include <ILCDataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCMessageFactory::ILCMessageFactory(ILCApplicationSettings* ilcApplicationSettings) {
	this->ilcApplicationSettings = ilcApplicationSettings;
}

void ILCMessageFactory::reportServerID(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(17);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReportServerID);
}

void ILCMessageFactory::reportServerStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(18);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReportServerStatus);
}

void ILCMessageFactory::changeILCMode(ModbusBuffer* buffer, uint8_t address, uint16_t mode) {
	buffer->writeU8(address);
	buffer->writeU8(65);
	buffer->writeU8(0);
	buffer->writeU8((uint8_t)mode);
	buffer->writeCRC(4);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ChangeILCMode);
}

void ILCMessageFactory::reportILCMode(ModbusBuffer* buffer, uint8_t address) {
	this->changeILCMode(buffer, address, ILCModes::NoChange);
}

void ILCMessageFactory::broadcastStepMotor(ModbusBuffer* buffer, uint8_t broadcastCounter, int8_t* steps) {
	buffer->writeU8(248);
	buffer->writeU8(66);
	buffer->writeU8(broadcastCounter);
	for(int i = 0; i < 78; i++) {
		buffer->writeI8(steps[i]);
	}
	buffer->writeCRC(81);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings->BroadcastStepMotor);
}

void ILCMessageFactory::unicastStepMotor(ModbusBuffer* buffer, uint8_t address, int8_t steps) {
	buffer->writeU8(address);
	buffer->writeU8(66);
	buffer->writeI8(steps);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->UnicastStepMotor);
}

void ILCMessageFactory::electromechanicalForceAndStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(67);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ElectromechanicalForceAndStatus);
}

void ILCMessageFactory::broadcastElectromechanicalFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter) {
	buffer->writeU8(248);
	buffer->writeU8(68);
	buffer->writeU8(broadcastCounter);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings->BroadcastFreezeSensorValues);
}

void ILCMessageFactory::broadcastPneumaticFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter) {
	buffer->writeU8(249);
	buffer->writeU8(68);
	buffer->writeU8(broadcastCounter);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings->BroadcastFreezeSensorValues);
}

void ILCMessageFactory::setBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address, float primaryGain, float secondaryGain) {
	buffer->writeU8(address);
	buffer->writeU8(73);
	buffer->writeSGL(primaryGain);
	buffer->writeSGL(secondaryGain);
	buffer->writeCRC(10);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->SetBoostValveDCAGains);
}

void ILCMessageFactory::readBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(74);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReadBoostValveDCAGains);
}

void ILCMessageFactory::broadcastForceDemand(ModbusBuffer* buffer, uint8_t broadcastCounter, bool slewFlag, int32_t* saaPrimarySetpoint, int32_t* daaPrimarySetpoint, int32_t* daaSecondarySetpoint) {
	buffer->writeU8(249);
	buffer->writeU8(75);
	buffer->writeU8(broadcastCounter);
	buffer->writeU8(slewFlag ? 255 : 0);
	for(int i = 0; i < 16; i++) {
		buffer->writeI24(saaPrimarySetpoint[i]);
	}
	for(int i = 0; i < 32; i++) {
		buffer->writeI24(daaPrimarySetpoint[i]);
		buffer->writeI24(daaSecondarySetpoint[i]);
	}
	buffer->writeCRC(244);
	buffer->writeEndOfFrame();
	buffer->writeDelay(this->ilcApplicationSettings->BroadcastForceDemand);
}

void ILCMessageFactory::unicastForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint, int32_t secondarySetpoint = 0) {
	if (address <= 16) {
		this->unicastSingleAxisForceDemand(buffer, address, slewFlag, primarySetpoint);
	}
	else {
		this->unicastDualAxisForceDemand(buffer, address, slewFlag, primarySetpoint, secondarySetpoint);
	}
}

void ILCMessageFactory::unicastSingleAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint) {
	buffer->writeU8(address);
	buffer->writeU8(75);
	buffer->writeU8(slewFlag ? 255 : 0);
	buffer->writeI24(primarySetpoint);
	buffer->writeCRC(6);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->UnicastSingleAxisForceDemand);
}

void ILCMessageFactory::unicastDualAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint, int32_t secondarySetpoint) {
	buffer->writeU8(address);
	buffer->writeU8(75);
	buffer->writeU8(slewFlag ? 255 : 0);
	buffer->writeI24(primarySetpoint);
	buffer->writeI24(secondarySetpoint);
	buffer->writeCRC(9);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->UnicastDualAxisForceDemand);
}

void ILCMessageFactory::pneumaticForceStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(76);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->PneumaticForceAndStatus);
}

void ILCMessageFactory::setADCScanRate(ModbusBuffer* buffer, uint8_t address, uint8_t rate) {
	buffer->writeU8(address);
	buffer->writeU8(80);
	buffer->writeU8(rate);
	buffer->writeCRC(3);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->SetADCScanRate);
}

void ILCMessageFactory::reportADCScanRate(ModbusBuffer* buffer, uint8_t address) {
	this->setADCScanRate(buffer, address, ILCScanRates::NoChange);
}

void ILCMessageFactory::setADCChannelOffsetAndSensitivity(ModbusBuffer* buffer, uint8_t address, uint8_t channel, float offset, float sensitivity) {
	buffer->writeU8(address);
	buffer->writeU8(81);
	buffer->writeU8(channel);
	buffer->writeSGL(offset);
	buffer->writeSGL(sensitivity);
	buffer->writeCRC(11);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->SetADCChannelOffsetAndSensitivity);
}

void ILCMessageFactory::reset(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(107);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->Reset);
}

void ILCMessageFactory::readCalibration(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(110);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReadCalibration);
}

void ILCMessageFactory::reportDCAPressure(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(119);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReportDCAPressure);
}

void ILCMessageFactory::reportDCAID(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(120);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReportDCAID);
}

void ILCMessageFactory::reportDCAStatus(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(121);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReportDCAStatus);
}

void ILCMessageFactory::reportLVDT(ModbusBuffer* buffer, uint8_t address) {
	buffer->writeU8(address);
	buffer->writeU8(122);
	buffer->writeCRC(2);
	buffer->writeEndOfFrame();
	buffer->writeWaitForRx(this->ilcApplicationSettings->ReportLVDT);
}

void ILCMessageFactory::nopReportLVDT(ModbusBuffer* buffer, uint8_t address) {
	// We require a means to create a nop instruction for reading LVDTs at 10Hz. If we don't do this then
	// we would have to constantly calculate the buffer size. If we instead fill it with nop then we don't
	buffer->writeLength(0); // Address
	buffer->writeLength(0); // Function Code
	buffer->writeLength(0); // CRC MSB
	buffer->writeLength(0); // CRC LSB
	buffer->writeLength(0); // End of Frame
	buffer->writeLength(0); // Wait for Rx
}



} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
