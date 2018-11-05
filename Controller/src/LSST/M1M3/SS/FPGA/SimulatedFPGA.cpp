/*
 * SimulatedFPGA.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#include <SimulatedFPGA.h>
#include <SAL_MTMountC.h>
#include <M1M3SSPublisher.h>
#include <FPGAAddresses.h>
#include <cstring>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SimulatedFPGA::SimulatedFPGA(M1M3SSPublisher* publisher, MTMount_AltC* tmaElevation, MTMount_AzC* tmaAzimuth) {
	Log.Debug("SimulatedFPGA: SimulatedFPGA()");
	this->publisher = publisher;
	this->tmaElevation = tmaElevation;
	this->tmaAzimuth = tmaAzimuth;
	this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
	this->lastRequest = -1;
	memset(&this->supportFPGAData, 0, sizeof(SupportFPGAData));
}

int32_t SimulatedFPGA::initialize() {
	Log.Debug("SimulatedFPGA: initialize()");
	return 0;
}

int32_t SimulatedFPGA::open() {
	Log.Debug("SimulatedFPGA: open()");
	return 0;
}

int32_t SimulatedFPGA::close() {
	Log.Debug("SimulatedFPGA: close()");
	return 0;
}

int32_t SimulatedFPGA::finalize() {
	Log.Debug("SimulatedFPGA: finalize()");
	return 0;
}

bool SimulatedFPGA::isErrorCode(int32_t status) {
	return status != 0;
}

int32_t SimulatedFPGA::waitForOuterLoopClock(int32_t timeout) {
	// Wait 20ms
	return 0;
}

int32_t SimulatedFPGA::ackOuterLoopClock() {
	return 0;
}

int32_t SimulatedFPGA::waitForPPS(int32_t timeout) {
	// Wait 1s
	return 0;
}

int32_t SimulatedFPGA::ackPPS() {
	return 0;
}

int32_t SimulatedFPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) {
	return 0;
}

int32_t SimulatedFPGA::ackModbusIRQ(int32_t subnet) {
	return 0;
}

void SimulatedFPGA::pullTelemetry() {
	Log.Trace("SimulatedFPGA: pullTelemetry()");
	double timestamp = this->publisher->getTimestamp();
	this->supportFPGAData.Reserved = 0;
	this->supportFPGAData.InclinometerTxBytes = 0;
	this->supportFPGAData.InclinometerRxBytes = 0;
	this->supportFPGAData.InclinometerTxFrames = 0;
	this->supportFPGAData.InclinometerRxFrames = 0;
	this->supportFPGAData.InclinometerErrorTimestamp = 0;
	this->supportFPGAData.InclinometerErrorCode = 0;
	this->supportFPGAData.InclinometerSampleTimestamp = timestamp;
	this->supportFPGAData.InclinometerAngleRaw = (int32_t)this->tmaElevation->Angle_Actual * 1000.0;
	this->supportFPGAData.DisplacementTxBytes = 0;
	this->supportFPGAData.DisplacementRxBytes = 0;
	this->supportFPGAData.DisplacementTxFrames = 0;
	this->supportFPGAData.DisplacementRxFrames = 0;
	this->supportFPGAData.DisplacementErrorTimestamp = 0;
	this->supportFPGAData.DisplacementErrorCode = 0;
	this->supportFPGAData.DisplacementSampleTimestamp = timestamp;
	this->supportFPGAData.DisplacementRaw1 = 0;
	this->supportFPGAData.DisplacementRaw2 = 0;
	this->supportFPGAData.DisplacementRaw3 = 0;
	this->supportFPGAData.DisplacementRaw4 = 0;
	this->supportFPGAData.DisplacementRaw5 = 0;
	this->supportFPGAData.DisplacementRaw6 = 0;
	this->supportFPGAData.DisplacementRaw7 = 0;
	this->supportFPGAData.DisplacementRaw8 = 0;
	this->supportFPGAData.AccelerometerSampleCount++;
	this->supportFPGAData.AccelerometerSampleTimestamp = timestamp;
	this->supportFPGAData.AccelerometerRaw1 = 0;
	this->supportFPGAData.AccelerometerRaw2 = 0;
	this->supportFPGAData.AccelerometerRaw3 = 0;
	this->supportFPGAData.AccelerometerRaw4 = 0;
	this->supportFPGAData.AccelerometerRaw5 = 0;
	this->supportFPGAData.AccelerometerRaw6 = 0;
	this->supportFPGAData.AccelerometerRaw7 = 0;
	this->supportFPGAData.AccelerometerRaw8 = 0;
	this->supportFPGAData.GyroTxBytes = 0;
	this->supportFPGAData.GyroRxBytes = 0;
	this->supportFPGAData.GyroTxFrames = 0;
	this->supportFPGAData.GyroRxFrames = 0;
	this->supportFPGAData.GyroErrorTimestamp = 0;
	this->supportFPGAData.GyroErrorCode = 0;
	this->supportFPGAData.GyroSampleTimestamp = timestamp;
	this->supportFPGAData.GyroRawX = 0;
	this->supportFPGAData.GyroRawY = 0;
	this->supportFPGAData.GyroRawZ = 0;
	this->supportFPGAData.GyroStatus = 0x7F;
	this->supportFPGAData.GyroSequenceNumber++;
	if (this->supportFPGAData.GyroSequenceNumber > 127) {
		this->supportFPGAData.GyroSequenceNumber = 0;
	}
	this->supportFPGAData.GyroTemperature = 0;
	this->supportFPGAData.GyroBITTimestamp = timestamp;
	this->supportFPGAData.GyroBIT0 = 0x7F;
	this->supportFPGAData.GyroBIT1 = 0x7F;
	this->supportFPGAData.GyroBIT2 = 0x7F;
	this->supportFPGAData.GyroBIT3 = 0x7F;
	this->supportFPGAData.GyroBIT4 = 0x7F;
	this->supportFPGAData.GyroBIT5 = 0x7F;
	this->supportFPGAData.GyroBIT6 = 0x7F;
	this->supportFPGAData.GyroBIT7 = 0x7F;
	this->supportFPGAData.DigitalInputSampleCount++;
	this->supportFPGAData.DigitalInputTimestamp = timestamp;
	this->supportFPGAData.DigitalInputStates = 0;
	this->supportFPGAData.DigitalOutputSampleCount++;
	this->supportFPGAData.DigitalOutputTimestamp = timestamp;
	this->supportFPGAData.DigitalOutputStates = 0;
	this->supportFPGAData.PowerSupplySampleCount++;
	this->supportFPGAData.PowerSupplyTimestamp = timestamp;
	this->supportFPGAData.PowerSupplyStates = 0;
}

int32_t SimulatedFPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
	return 0;
}

int32_t SimulatedFPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) {
	return 0;
}

int32_t SimulatedFPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
	this->lastRequest = data[0];
	return 0;
}

int32_t SimulatedFPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) {
	this->lastRequest = data;
	return 0;
}

int32_t SimulatedFPGA::writeTimestampFIFO(uint64_t timestamp) {
	return 0;
}

int32_t SimulatedFPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) {
	return 0;
}

int32_t SimulatedFPGA::readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
	if (length == 1) {
		switch(this->lastRequest) {
		case FPGAAddresses::ModbusSubnetARx:
		case FPGAAddresses::ModbusSubnetBRx:
		case FPGAAddresses::ModbusSubnetCRx:
		case FPGAAddresses::ModbusSubnetDRx:
		case FPGAAddresses::ModbusSubnetERx:
			data[0] = 0;
			return 0;
		}
	}
	else if (length != 0) {
		switch(this->lastRequest) {
		case FPGAAddresses::ModbusSubnetARx:
		case FPGAAddresses::ModbusSubnetBRx:
		case FPGAAddresses::ModbusSubnetCRx:
		case FPGAAddresses::ModbusSubnetDRx:
		case FPGAAddresses::ModbusSubnetERx:
			for(int i = 0; i < length; ++i) {
				data[i] = 0;
			}
			return 0;
		}
	}
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
