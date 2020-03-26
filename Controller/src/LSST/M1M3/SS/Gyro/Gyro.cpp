/*
 * Gyro.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#include <Gyro.h>
#include <GyroSettings.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <Log.h>

#include <boost/lexical_cast.hpp>
#include <cstring>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Gyro::Gyro(GyroSettings* gyroSettings, M1M3SSPublisher* publisher) {
	Log.Debug("Gyro: Gyro()");
	this->gyroSettings = gyroSettings;
	this->publisher = publisher;

	this->gyroData = this->publisher->getGyroData();
	this->gyroWarning = this->publisher->getEventGyroWarning();

	this->lastBITTimestamp = 0;
	this->lastErrorTimestamp = 0;
	this->lastSampleTimestamp = 0;

	this->errorCleared = false;

	memset(this->gyroData, 0, sizeof(MTM1M3_gyroDataC));
	memset(this->gyroWarning, 0, sizeof(MTM1M3_logevent_gyroWarningC));
}

void Gyro::bit() {
	Log.Info("Gyro: bit()");
	this->writeCommand("?BIT,2\r\n");
	usleep(10000);
}

void Gyro::enterConfigurationMode() {
	Log.Info("Gyro: enterConfigurationMode()");
	this->writeCommand("=CONFIG,1\r\n");
	usleep(10000);
}

void Gyro::exitConfigurationMode() {
	Log.Info("Gyro: exitConfigurationMode()");
	this->writeCommand("=CONFIG,0\r\n");
	usleep(10000);
}

void Gyro::resetConfiguration() {
	Log.Info("Gyro: resetConfiguration()");
	this->writeCommand("=RSTCFG\r\n");
	usleep(10000);
}

void Gyro::setRotationFormatRate() {
	Log.Info("Gyro: setRotationFormatRate()");
	this->writeCommand("=ROTFMT,RATE\r\n");
	usleep(10000);
}

void Gyro::setRotationUnitsRadians() {
	Log.Info("Gyro: setRotationUnitsRadians()");
	this->writeCommand("=ROTUNITS,RAD\r\n");
	usleep(10000);
}

void Gyro::setAxis() {
	Log.Info("Gyro: setAxis()");
	std::string command = "=AXES";
	for(unsigned int i = 0; i < this->gyroSettings->AxesMatrix.size(); ++i) {
		command = command + "," + boost::lexical_cast<std::string>(this->gyroSettings->AxesMatrix[i]);
	}
	this->writeCommand(command + "\r\n");
	usleep(10000);
}

void Gyro::setDataRate() {
	Log.Info("Gyro: setDataRate()");
	this->writeCommand("=DR," + boost::lexical_cast<std::string>(this->gyroSettings->DataRate) + "\r\n");
	usleep(10000);
}

void Gyro::processData() {
	// TODO: Handle no data available
	// TODO: Handle limits, push to safety controller
	Log.Trace("Gyro: processData()");
	bool tryLogWarning = false;
        SupportFPGAData *fpgaData = IFPGA::get().getSupportFPGAData();
	if (fpgaData->GyroErrorTimestamp > this->lastErrorTimestamp) {
		this->lastErrorTimestamp = fpgaData->GyroErrorTimestamp;
		this->errorCleared = false;
		this->gyroWarning->timestamp = Timestamp::fromFPGA(fpgaData->GyroErrorTimestamp);
		this->gyroWarning->invalidHeaderWarning = fpgaData->GyroErrorCode == 1;
		this->gyroWarning->crcMismatchWarning = fpgaData->GyroErrorCode == 2 || fpgaData->GyroErrorCode == 4;
		this->gyroWarning->incompleteFrameWarning = fpgaData->GyroErrorCode == 3;
		// TODO: Add Checksum Error
		tryLogWarning = true;
	}
	if (fpgaData->GyroBITTimestamp > this->lastBITTimestamp) {
		this->lastBITTimestamp = fpgaData->GyroBITTimestamp;
		this->gyroWarning->timestamp = Timestamp::fromFPGA(fpgaData->GyroBITTimestamp);
		uint8_t word0 = fpgaData->GyroBIT0;
		this->gyroWarning->gyroXSLDWarning = (word0 & 0x01) == 0;
		this->gyroWarning->gyroXMODDACWarning = (word0 & 0x02) == 0;
		this->gyroWarning->gyroXPhaseWarning = (word0 & 0x04) == 0;
		this->gyroWarning->gyroXFlashWarning = (word0 & 0x08) == 0;
		this->gyroWarning->gyroYSLDWarning = (word0 & 0x10) == 0;
		this->gyroWarning->gyroYMODDACWarning = (word0 & 0x20) == 0;
		this->gyroWarning->gyroYPhaseWarning = (word0 & 0x40) == 0;
		uint8_t word1 = fpgaData->GyroBIT1;
		this->gyroWarning->gyroYFlashWarning = (word1 & 0x01) == 0;
		this->gyroWarning->gyroZSLDWarning = (word1 & 0x02) == 0;
		this->gyroWarning->gyroZMODDACWarning = (word1 & 0x04) == 0;
		this->gyroWarning->gyroZPhaseWarning = (word1 & 0x08) == 0;
		uint8_t word2 = fpgaData->GyroBIT2;
		this->gyroWarning->gyroXSLDTemperatureStatusWarning = (word2 & 0x04) == 0;
		this->gyroWarning->gyroYSLDTemperatureStatusWarning = (word2 & 0x10) == 0;
		this->gyroWarning->gyroZSLDTemperatureStatusWarning = (word2 & 0x40) == 0;
		uint8_t word3 = fpgaData->GyroBIT3;
		this->gyroWarning->gcbTemperatureStatusWarning = (word3 & 0x08) == 0;
		this->gyroWarning->temperatureStatusWarning = (word3 & 0x10) == 0;
		this->gyroWarning->gcbDSPSPIFlashStatusWarning = (word3 & 0x20) == 0;
		this->gyroWarning->gcbFPGASPIFlashStatusWarning = (word3 & 0x40) == 0;
		uint8_t word4 = fpgaData->GyroBIT4;
		this->gyroWarning->dspSPIFlashStatusWarning = (word4 & 0x01) == 0;
		this->gyroWarning->fpgaSPIFlashStatusWarning = (word4 & 0x02) == 0;
		this->gyroWarning->gcb1_2VStatusWarning = (word4 & 0x04) == 0;
		this->gyroWarning->gcb3_3VStatusWarning = (word4 & 0x08) == 0;
		this->gyroWarning->gcb5VStatusWarning = (word4 & 0x10) == 0;
		this->gyroWarning->v1_2StatusWarning = (word4 & 0x20) == 0;
		this->gyroWarning->v3_3StatusWarning = (word4 & 0x40) == 0;
		uint8_t word5 = fpgaData->GyroBIT5;
		this->gyroWarning->v5StatusWarning = (word5 & 0x01) == 0;
		this->gyroWarning->gcbFPGAStatusWarning = (word5 & 0x04) == 0;
		this->gyroWarning->fpgaStatusWarning = (word5 & 0x08) == 0;
		this->gyroWarning->hiSpeedSPORTStatusWarning = (word5 & 0x10) == 0;
		this->gyroWarning->auxSPORTStatusWarning = (word5 & 0x20) == 0;
		this->gyroWarning->sufficientSoftwareResourcesWarning = (word5 & 0x40) == 0;
		uint8_t word6 = fpgaData->GyroBIT6;
		this->gyroWarning->gyroEOVoltsPositiveWarning = (word6 & 0x01) == 0;
		this->gyroWarning->gyroEOVoltsNegativeWarning = (word6 & 0x02) == 0;
		this->gyroWarning->gyroXVoltsWarning = (word6 & 0x04) == 0;
		this->gyroWarning->gyroYVoltsWarning = (word6 & 0x08) == 0;
		this->gyroWarning->gyroZVoltsWarning = (word6 & 0x10) == 0;
		uint8_t word7 = fpgaData->GyroBIT7;
		this->gyroWarning->gcbADCCommsWarning = (word7 & 0x01) == 0;
		this->gyroWarning->mSYNCExternalTimingWarning = (word7 & 0x02) == 0;
		tryLogWarning = true;
	}
	if (fpgaData->GyroSampleTimestamp > this->lastSampleTimestamp) {
		this->lastSampleTimestamp = fpgaData->GyroSampleTimestamp;
		this->gyroData->timestamp = Timestamp::fromFPGA(fpgaData->GyroSampleTimestamp);
		this->gyroData->angularVelocityX = fpgaData->GyroRawX + this->gyroSettings->AngularVelocityXOffset;
		this->gyroData->angularVelocityY = fpgaData->GyroRawY + this->gyroSettings->AngularVelocityYOffset;
		this->gyroData->angularVelocityZ = fpgaData->GyroRawZ + this->gyroSettings->AngularVelocityZOffset;
		this->gyroData->sequenceNumber = fpgaData->GyroSequenceNumber;
		this->gyroData->temperature = fpgaData->GyroTemperature;
		uint8_t status = fpgaData->GyroStatus;
		this->gyroWarning->gyroXStatusWarning = (status & 0x01) == 0;
		this->gyroWarning->gyroYStatusWarning = (status & 0x02) == 0;
		this->gyroWarning->gyroZStatusWarning = (status & 0x04) == 0;
		this->publisher->putGyroData();
		tryLogWarning = true;
		if (!this->errorCleared && fpgaData->GyroSampleTimestamp > this->lastErrorTimestamp) {
			this->lastErrorTimestamp = fpgaData->GyroErrorTimestamp;
			this->errorCleared = true;
			this->gyroWarning->timestamp = Timestamp::fromFPGA(fpgaData->GyroSampleTimestamp);
			this->gyroWarning->invalidHeaderWarning = false;
			this->gyroWarning->crcMismatchWarning = false;
			this->gyroWarning->incompleteFrameWarning = false;
			// TODO: Add Checksum Error
			tryLogWarning = true;
		}
	}
	if (tryLogWarning) {
		this->publisher->tryLogGyroWarning();
	}
}

void Gyro::writeCommand(std::string command) {
	uint16_t buffer[256];
	memset(buffer, 0, sizeof(buffer));
	int32_t size = (int32_t)command.size();
	buffer[0] = FPGAAddresses::GyroTx;
	buffer[1] = (uint16_t)size;
	for(int i = 0; i < size; ++i) {
		buffer[i + 2] = (uint16_t)command[i];
	}
        IFPGA::get().writeCommandFIFO(buffer, size + 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
