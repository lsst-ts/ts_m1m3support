/*
 * Gyro.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#include <Gyro.h>
#include <GyroSettings.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <SAL_m1m3C.h>
#include <Log.h>

#include <boost/lexical_cast.hpp>
#include <cstring>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Gyro::Gyro(GyroSettings* gyroSettings, FPGA* fpga, M1M3SSPublisher* publisher) {
	Log.Debug("Gyro: Gyro()");
	this->gyroSettings = gyroSettings;
	this->fpga = fpga;
	this->fpgaData = this->fpga->getSupportFPGAData();
	this->publisher = publisher;

	this->gyroData = this->publisher->getGyroData();
	this->gyroWarning = this->publisher->getEventGyroWarning();

	this->lastBITTimestamp = 0;
	this->lastErrorTimestamp = 0;
	this->lastSampleTimestamp = 0;

	memset(this->gyroData, 0, sizeof(m1m3_GyroDataC));
	memset(this->gyroWarning, 0, sizeof(m1m3_logevent_GyroWarningC));
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
	if (this->fpgaData->GyroSampleTimestamp != this->lastSampleTimestamp) {
		this->lastSampleTimestamp = this->fpgaData->GyroSampleTimestamp;
		this->gyroData->Timestamp = Timestamp::fromFPGA(this->fpgaData->GyroSampleTimestamp);
		this->gyroData->AngularVelocityX = this->fpgaData->GyroRawX + this->gyroSettings->AngularVelocityXOffset;
		this->gyroData->AngularVelocityY = this->fpgaData->GyroRawY + this->gyroSettings->AngularVelocityYOffset;
		this->gyroData->AngularVelocityZ = this->fpgaData->GyroRawZ + this->gyroSettings->AngularVelocityZOffset;
		this->gyroData->SequenceNumber = this->fpgaData->GyroSequenceNumber;
		this->gyroData->Temperature = this->fpgaData->GyroTemperature;
		uint8_t status = this->fpgaData->GyroStatus;
		this->gyroWarning->GyroXStatusWarning = (status & 0x01) == 0;
		this->gyroWarning->GyroYStatusWarning = (status & 0x02) == 0;
		this->gyroWarning->GyroZStatusWarning = (status & 0x04) == 0;
		this->publisher->putGyroData();
		tryLogWarning = true;
	}
	if (this->fpgaData->GyroBITTimestamp != this->lastBITTimestamp) {
		this->lastBITTimestamp = this->fpgaData->GyroBITTimestamp;
		this->gyroWarning->Timestamp = Timestamp::fromFPGA(this->fpgaData->GyroBITTimestamp);
		uint8_t word0 = this->fpgaData->GyroBIT0;
		this->gyroWarning->GyroXSLDWarning = (word0 & 0x01) == 0;
		this->gyroWarning->GyroXMODDACWarning = (word0 & 0x02) == 0;
		this->gyroWarning->GyroXPhaseWarning = (word0 & 0x04) == 0;
		this->gyroWarning->GyroXFlashWarning = (word0 & 0x08) == 0;
		this->gyroWarning->GyroYSLDWarning = (word0 & 0x10) == 0;
		this->gyroWarning->GyroYMODDACWarning = (word0 & 0x20) == 0;
		this->gyroWarning->GyroYPhaseWarning = (word0 & 0x40) == 0;
		uint8_t word1 = this->fpgaData->GyroBIT1;
		this->gyroWarning->GyroYFlashWarning = (word1 & 0x01) == 0;
		this->gyroWarning->GyroZSLDWarning = (word1 & 0x02) == 0;
		this->gyroWarning->GyroZMODDACWarning = (word1 & 0x04) == 0;
		this->gyroWarning->GyroZPhaseWarning = (word1 & 0x08) == 0;
		uint8_t word2 = this->fpgaData->GyroBIT2;
		this->gyroWarning->GyroXSLDTemperatureStatusWarning = (word2 & 0x04) == 0;
		this->gyroWarning->GyroYSLDTemperatureStatusWarning = (word2 & 0x10) == 0;
		this->gyroWarning->GyroZSLDTemperatureStatusWarning = (word2 & 0x40) == 0;
		uint8_t word3 = this->fpgaData->GyroBIT3;
		this->gyroWarning->GCBTemperatureStatusWarning = (word3 & 0x08) == 0;
		this->gyroWarning->TemperatureStatusWarning = (word3 & 0x10) == 0;
		this->gyroWarning->GCBDSPSPIFlashStatusWarning = (word3 & 0x20) == 0;
		this->gyroWarning->GCBFPGASPIFlashStatusWarning = (word3 & 0x40) == 0;
		uint8_t word4 = this->fpgaData->GyroBIT4;
		this->gyroWarning->DSPSPIFlashStatusWarning = (word4 & 0x01) == 0;
		this->gyroWarning->FPGASPIFlashStatusWarning = (word4 & 0x02) == 0;
		this->gyroWarning->GCB1_2VStatusWarning = (word4 & 0x04) == 0;
		this->gyroWarning->GCB3_3VStatusWarning = (word4 & 0x08) == 0;
		this->gyroWarning->GCB5VStatusWarning = (word4 & 0x10) == 0;
		this->gyroWarning->V1_2StatusWarning = (word4 & 0x20) == 0;
		this->gyroWarning->V3_3StatusWarning = (word4 & 0x40) == 0;
		uint8_t word5 = this->fpgaData->GyroBIT5;
		this->gyroWarning->V5StatusWarning = (word5 & 0x01) == 0;
		this->gyroWarning->GCBFPGAStatusWarning = (word5 & 0x04) == 0;
		this->gyroWarning->FPGAStatusWarning = (word5 & 0x08) == 0;
		this->gyroWarning->HiSpeedSPORTStatusWarning = (word5 & 0x10) == 0;
		this->gyroWarning->AuxSPORTStatusWarning = (word5 & 0x20) == 0;
		this->gyroWarning->SufficientSoftwareResourcesWarning = (word5 & 0x40) == 0;
		uint8_t word6 = this->fpgaData->GyroBIT6;
		this->gyroWarning->GyroEOVoltsPositiveWarning = (word6 & 0x01) == 0;
		this->gyroWarning->GyroEOVoltsNegativeWarning = (word6 & 0x02) == 0;
		this->gyroWarning->GyroXVoltsWarning = (word6 & 0x04) == 0;
		this->gyroWarning->GyroYVoltsWarning = (word6 & 0x08) == 0;
		this->gyroWarning->GyroZVoltsWarning = (word6 & 0x10) == 0;
		uint8_t word7 = this->fpgaData->GyroBIT7;
		this->gyroWarning->GCBADCCommsWarning = (word7 & 0x01) == 0;
		this->gyroWarning->MSYNCExternalTimingWarning = (word7 & 0x02) == 0;
		tryLogWarning = true;
	}
	if (this->fpgaData->GyroErrorTimestamp != this->lastErrorTimestamp) {
		this->lastErrorTimestamp = this->fpgaData->GyroErrorTimestamp;
		this->gyroWarning->Timestamp = Timestamp::fromFPGA(this->fpgaData->GyroErrorTimestamp);
		this->gyroWarning->InvalidHeaderWarning = this->fpgaData->GyroErrorCode == 1;
		this->gyroWarning->CRCMismatchWarning = this->fpgaData->GyroErrorCode == 2 || this->fpgaData->GyroErrorCode == 4;
		this->gyroWarning->IncompleteFrameWarning = this->fpgaData->GyroErrorCode == 3;
		// TODO: Add Checksum Error
		tryLogWarning = true;
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
	this->fpga->writeCommandFIFO(buffer, size + 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
