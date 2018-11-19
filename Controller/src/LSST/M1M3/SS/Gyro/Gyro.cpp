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
#include <SALEnumerations.h>
#include <BitHelper.h>
#include <Log.h>

#include <boost/lexical_cast.hpp>
#include <cstring>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Gyro::Gyro(GyroSettings* gyroSettings, IFPGA* fpga, M1M3SSPublisher* publisher) {
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
	if (this->fpgaData->GyroErrorTimestamp > this->lastErrorTimestamp) {
		this->lastErrorTimestamp = this->fpgaData->GyroErrorTimestamp;
		this->errorCleared = false;
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::InvalidHeaderWarning, this->fpgaData->GyroErrorCode == 1);
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::CRCMismatchWarning, this->fpgaData->GyroErrorCode == 2);
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::IncompleteFrameWarning, this->fpgaData->GyroErrorCode == 3);
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::CRCMismatchWarning, this->fpgaData->GyroErrorCode == 4);
		// TODO: Add Checksum Error
		tryLogWarning = true;
	}
	if (this->fpgaData->GyroBITTimestamp > this->lastBITTimestamp) {
		this->lastBITTimestamp = this->fpgaData->GyroBITTimestamp;
		uint8_t word0 = this->fpgaData->GyroBIT0;
		uint8_t word1 = this->fpgaData->GyroBIT1;
		uint8_t word2 = this->fpgaData->GyroBIT2;
		uint8_t word3 = this->fpgaData->GyroBIT3;
		uint8_t word4 = this->fpgaData->GyroBIT4;
		uint8_t word5 = this->fpgaData->GyroBIT5;
		uint8_t word6 = this->fpgaData->GyroBIT6;
		uint8_t word7 = this->fpgaData->GyroBIT7;
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXSLDWarning, ((word0 & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXMODDACWarning, ((word0 & 0x02) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXPhaseWarning, ((word0 & 0x04) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXFlashWarning, ((word0 & 0x08) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYSLDWarning, ((word0 & 0x10) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYMODDACWarning, ((word0 & 0x20) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYPhaseWarning, ((word0 & 0x40) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYFlashWarning, ((word1 & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZSLDWarning, ((word1 & 0x02) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZMODDACWarning, ((word1 & 0x04) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZPhaseWarning, ((word1 & 0x08) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZFlashWarning, ((word1 & 0x10) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXSLDTemperatureStatusWarning, ((word2 & 0x04) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYSLDTemperatureStatusWarning, ((word2 & 0x10) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZSLDTemperatureStatusWarning, ((word2 & 0x40) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCBTemperatureStatusWarning, ((word3 & 0x08) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::TemperatureStatusWarning, ((word3 & 0x10) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCBDSPSPIFlashStatusWarning, ((word3 & 0x20) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCBFPGASPIFlashStatusWarning, ((word3 & 0x40) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::DSPSPIFlashStatusWarning, ((word4 & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::FPGASPIFlashStatusWarning, ((word4 & 0x02) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCB1_2VStatusWarning, ((word4 & 0x04) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCB3_3VStatusWarning, ((word4 & 0x08) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCB5VStatusWarning, ((word4 & 0x10) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::V1_2StatusWarning, ((word4 & 0x20) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::V3_3StatusWarning, ((word4 & 0x40) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::V5StatusWarning, ((word5 & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCBFPGAStatusWarning, ((word5 & 0x04) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::FPGAStatusWarning, ((word5 & 0x08) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::HiSpeedSPORTStatusWarning, ((word5 & 0x10) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::AuxSPORTStatusWarning, ((word5 & 0x20) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::SufficientSoftwareResourcesWarning, ((word5 & 0x40) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroEOVoltsPositiveWarning, ((word6 & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroEOVoltsNegativeWarning, ((word6 & 0x02) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXVoltsWarning, ((word6 & 0x04) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYVoltsWarning, ((word6 & 0x08) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZVoltsWarning, ((word6 & 0x10) == 0));

		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GCBADCCommsWarning, ((word7 & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::MSYNCExternalTimingWarning, ((word7 & 0x02) == 0));
		tryLogWarning = true;
	}
	if (this->fpgaData->GyroSampleTimestamp > this->lastSampleTimestamp) {
		this->lastSampleTimestamp = this->fpgaData->GyroSampleTimestamp;
		this->gyroData->timestamp = Timestamp::fromFPGA(this->fpgaData->GyroSampleTimestamp);
		this->gyroData->angularVelocityX = this->fpgaData->GyroRawX + this->gyroSettings->AngularVelocityXOffset;
		this->gyroData->angularVelocityY = this->fpgaData->GyroRawY + this->gyroSettings->AngularVelocityYOffset;
		this->gyroData->angularVelocityZ = this->fpgaData->GyroRawZ + this->gyroSettings->AngularVelocityZOffset;
		this->gyroData->sequenceNumber = this->fpgaData->GyroSequenceNumber;
		this->gyroData->temperature = this->fpgaData->GyroTemperature;
		uint8_t status = this->fpgaData->GyroStatus;
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroXStatusWarning, ((status & 0x01) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroYStatusWarning, ((status & 0x02) == 0));
		BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::GyroZStatusWarning, ((status & 0x04) == 0));
		this->publisher->putGyroData();
		tryLogWarning = true;
		if (!this->errorCleared && this->fpgaData->GyroSampleTimestamp > this->lastErrorTimestamp) {
			this->lastErrorTimestamp = this->fpgaData->GyroErrorTimestamp;
			this->errorCleared = true;
			BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::InvalidHeaderWarning, false);
			BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::CRCMismatchWarning, false);
			BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::IncompleteFrameWarning, false);
			BitHelper::set(&this->gyroWarning->gyroSensorFlags, GyroSensorFlags::CRCMismatchWarning, false);
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
	this->fpga->writeCommandFIFO(buffer, size + 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
