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
#include <M1M3SSPublisher.h>
#include <ModbusBuffer.h>
#include <SAL_m1m3C.h>
#include <boost/lexical_cast.hpp>
#include <cstring>
#include <CRC.h>
#include <Checksum.h>
#include <unistd.h>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Gyro::Gyro(GyroSettings* gyroSettings, FPGA* fpga, M1M3SSPublisher* publisher) {
	this->gyroSettings = gyroSettings;
	this->fpga = fpga;
	this->publisher = publisher;
	this->gyroData = this->publisher->getGyroData();
	this->gyroWarning = this->publisher->getEventGyroWarning();
	this->setBuffer(&this->enterConfigurationBuffer, "=CONFIG,1\r\n");
	this->setBuffer(&this->exitConfigurationBuffer, "=CONFIG,0\r\n");
	this->setBuffer(&this->resetBuffer, "=RSTCFG\r\n");
	this->setBuffer(&this->rotationFormatRateBuffer, "=ROTFMT,RATE\r\n");
	this->setBuffer(&this->rotationUnitsRadiansBuffer, "=ROTUNITS,RAD\r\n");
	this->setBuffer(&this->dataRateBuffer, "=DR," + boost::lexical_cast<std::string>(this->gyroSettings->DataRate) + "\r\n");
	this->setBuffer(&this->bitBuffer, "?BIT,2\r\n");

	std::string axesText = "=AXES";
	for(unsigned int i = 0; i < this->gyroSettings->AxesMatrix.size(); ++i) {
		axesText = axesText + "," + boost::lexical_cast<std::string>(this->gyroSettings->AxesMatrix[i]);
	}
	this->setBuffer(&this->axesBuffer, axesText + "\r\n");

	memset(this->gyroData, 0, sizeof(m1m3_GyroDataC));

	memset(this->gyroWarning, 0, sizeof(m1m3_logevent_GyroWarningC));
	this->gyroWarning->Timestamp = this->publisher->getTimestamp();
	this->publishGyroWarning();
}

void Gyro::bit() {
	this->writeCommand(&this->bitBuffer);
	usleep(10000);
}

void Gyro::enterConfigurationMode() {
	this->writeCommand(&this->enterConfigurationBuffer);
	usleep(10000);
}

void Gyro::exitConfigurationMode() {
	this->writeCommand(&this->exitConfigurationBuffer);
	usleep(10000);
}

void Gyro::enableIgnore() {
	uint16_t buffer[2] = {
		FPGAAddresses::GyroRxIgnore,
		1
	};
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void Gyro::disableIgnore() {
	uint16_t buffer[2] = {
		FPGAAddresses::GyroRxIgnore,
		0
	};
	this->fpga->writeCommandFIFO(buffer, 2, 0);
}

void Gyro::resetConfiguration() {
	this->writeCommand(&this->resetBuffer);
	usleep(10000);
}

void Gyro::setRotationFormatRate() {
	this->writeCommand(&this->rotationFormatRateBuffer);
	usleep(10000);
}

void Gyro::setRotationUnitsRadians() {
	this->writeCommand(&this->rotationUnitsRadiansBuffer);
	usleep(10000);
}

void Gyro::setAxis() {
	this->writeCommand(&this->axesBuffer);
	usleep(10000);
}

void Gyro::setDataRate() {
	this->writeCommand(&this->dataRateBuffer);
	usleep(10000);
}

void Gyro::read() {
	uint16_t lengthBuffer[1];
	ModbusBuffer buffer = ModbusBuffer();
	this->fpga->writeRequestFIFO(FPGAAddresses::GyroRx, 0);
	this->fpga->readU16ResponseFIFO(lengthBuffer, 1, 1000);
	uint16_t length = lengthBuffer[0];
	if (length != 0) {
		if (length > 1024) {
			length = 1024;
		}
		buffer.setIndex(0);
		this->fpga->readU16ResponseFIFO(buffer.getBuffer(), length, 1000);
		buffer.setLength(length);
		while(!buffer.endOfBuffer()) {
			uint8_t peek = buffer.readU8();
			buffer.setIndex(buffer.getIndex() - 1);
			if (peek == 0xFE) {
				uint32_t header = buffer.readU32();
				if (header == 0xFE81FF55 || header == 0xFE8100AA || header == 0xFE8100AB) {
					uint8_t tmpBuffer[36];
					int length = 0;
					switch(header) {
					case 0xFE81FF55: length = 36; break;
					case 0xFE8100AA: length = 11; break;
					case 0xFE8100AB: length = 13; break;
					}
					buffer.setIndex(buffer.getIndex() - 4);
					for(int i = 0; i < length; ++i) {
						tmpBuffer[i] = buffer.readU8();
					}
					bool valid = false;
					switch(header) {
					case 0xFE81FF55: valid = CRC::crc32(tmpBuffer, 0, length - 4) == buffer.readU32(); break;
					case 0xFE8100AA: valid = Checksum::checksum8(tmpBuffer, 0, length - 1) == buffer.readU8(); break;
					case 0xFE8100AB: valid = Checksum::checksum8(tmpBuffer, 0, length - 1) == buffer.readU8(); break;
					}
					if (valid) {
						switch(header) {
						case 0xFE81FF55: this->readData(&buffer); break;
						case 0xFE8100AA: this->readShortBIT(&buffer); break;
						case 0xFE8100AB: this->readLongBIT(&buffer); break;
						}
					}
					double timestamp = buffer.readTimestamp();
					this->gyroData->Timestamp = timestamp;
					this->gyroWarning->Timestamp = timestamp;
					buffer.readEndOfFrame();
					if (!valid) {
						this->gyroWarning->CRCMismatchWarning = true;
					}
				}
				else {
					// TODO: Incomplete Frame it is expected to occur because of how I read data
					this->readToEndOfFrame(&buffer);
//					cout << "Incomplete Frame" << endl;
				}
			}
			else {
				this->readToEndOfFrame(&buffer);
//				cout << "Invalid Header" << endl;
				// TODO: Need to decide how to handle ASCII responses
			}
		}
	}
	else {
//		cout << "Invalid Length " << length << endl;
	}
}

void Gyro::publishGyroData() {
	this->publisher->putGyroData();
}

void Gyro::publishGyroWarningIfRequired() {
	if (this->checkGyroWarningForUpdates()) {
		this->publishGyroWarning();
	}
}

void Gyro::setBuffer(GyroBuffer* buffer, std::string text) {
	buffer->Size = text.size() + 3;
	buffer->Buffer[0] = FPGAAddresses::GyroTx;
	buffer->Buffer[1] = buffer->Size - 2;
	for(int i = 0; i < (buffer->Size - 1); ++i) {
		buffer->Buffer[i + 2] = 0x1200 | (((uint16_t)text[i]) << 1);
	}
	buffer->Buffer[buffer->Size - 1] = 0x2100;
}

void Gyro::writeCommand(GyroBuffer* buffer) {
	this->fpga->writeCommandFIFO(buffer->Buffer, buffer->Size, 0);
}

void Gyro::readShortBIT(ModbusBuffer* buffer) {
	this->readFirst6OfBIT(buffer);
	buffer->readU8(); // Checksum
	this->gyroWarning->Timestamp = buffer->readTimestamp();
	buffer->readEndOfFrame();
}

void Gyro::readFirst6OfBIT(ModbusBuffer* buffer) {
	uint8_t word0 = buffer->readU8();
	this->gyroWarning->GyroXSLDWarning = (word0 & 0x01) == 0;
	this->gyroWarning->GyroXMODDACWarning = (word0 & 0x02) == 0;
	this->gyroWarning->GyroXPhaseWarning = (word0 & 0x04) == 0;
	this->gyroWarning->GyroXFlashWarning = (word0 & 0x08) == 0;
	this->gyroWarning->GyroYSLDWarning = (word0 & 0x10) == 0;
	this->gyroWarning->GyroYMODDACWarning = (word0 & 0x20) == 0;
	this->gyroWarning->GyroYPhaseWarning = (word0 & 0x40) == 0;
	uint8_t word1 = buffer->readU8();
	this->gyroWarning->GyroYFlashWarning = (word1 & 0x01) == 0;
	this->gyroWarning->GyroZSLDWarning = (word1 & 0x02) == 0;
	this->gyroWarning->GyroZMODDACWarning = (word1 & 0x04) == 0;
	this->gyroWarning->GyroZPhaseWarning = (word1 & 0x08) == 0;
	uint8_t word2 = buffer->readU8();
	this->gyroWarning->GyroXSLDTemperatureStatusWarning = (word2 & 0x04) == 0;
	this->gyroWarning->GyroYSLDTemperatureStatusWarning = (word2 & 0x10) == 0;
	this->gyroWarning->GyroZSLDTemperatureStatusWarning = (word2 & 0x40) == 0;
	uint8_t word3 = buffer->readU8();
	this->gyroWarning->GCBTemperatureStatusWarning = (word3 & 0x08) == 0;
	this->gyroWarning->TemperatureStatusWarning = (word3 & 0x10) == 0;
	this->gyroWarning->GCBDSPSPIFlashStatusWarning = (word3 & 0x20) == 0;
	this->gyroWarning->GCBFPGASPIFlashStatusWarning = (word3 & 0x40) == 0;
	uint8_t word4 = buffer->readU8();
	this->gyroWarning->DSPSPIFlashStatusWarning = (word4 & 0x01) == 0;
	this->gyroWarning->FPGASPIFlashStatusWarning = (word4 & 0x02) == 0;
	this->gyroWarning->GCB1_2VStatusWarning = (word4 & 0x04) == 0;
	this->gyroWarning->GCB3_3VStatusWarning = (word4 & 0x08) == 0;
	this->gyroWarning->GCB5VStatusWarning = (word4 & 0x10) == 0;
	this->gyroWarning->V1_2StatusWarning = (word4 & 0x20) == 0;
	this->gyroWarning->V3_3StatusWarning = (word4 & 0x40) == 0;
	uint8_t word5 = buffer->readU8();
	this->gyroWarning->V5StatusWarning = (word5 & 0x01) == 0;
	this->gyroWarning->GCBFPGAStatusWarning = (word5 & 0x04) == 0;
	this->gyroWarning->FPGAStatusWarning = (word5 & 0x08) == 0;
	this->gyroWarning->HiSpeedSPORTStatusWarning = (word5 & 0x10) == 0;
	this->gyroWarning->AuxSPORTStatusWarning = (word5 & 0x20) == 0;
	this->gyroWarning->SufficientSoftwareResourcesWarning = (word5 & 0x40) == 0;
}

void Gyro::readLongBIT(ModbusBuffer* buffer) {
	this->readFirst6OfBIT(buffer);
	uint8_t word6 = buffer->readU8();
	this->gyroWarning->GyroEOVoltsPositiveWarning = (word6 & 0x01) == 0;
	this->gyroWarning->GyroEOVoltsNegativeWarning = (word6 & 0x02) == 0;
	this->gyroWarning->GyroXVoltsWarning = (word6 & 0x04) == 0;
	this->gyroWarning->GyroYVoltsWarning = (word6 & 0x08) == 0;
	this->gyroWarning->GyroZVoltsWarning = (word6 & 0x10) == 0;
	uint8_t word7 = buffer->readU8();
	this->gyroWarning->GCBADCCommsWarning = (word7 & 0x01) == 0;
	this->gyroWarning->MSYNCExternalTimingWarning = (word7 & 0x02) == 0;
	buffer->readU8(); // Checksum
	this->gyroWarning->Timestamp = buffer->readTimestamp();
	buffer->readEndOfFrame();
}

void Gyro::readData(ModbusBuffer* buffer) {
	this->gyroData->AngularVelocityX = buffer->readSGL();
	this->gyroData->AngularVelocityY = buffer->readSGL();
	this->gyroData->AngularVelocityZ = buffer->readSGL();
	buffer->readI32(); // Reserved
	buffer->readI32(); // Reserved
	buffer->readI32(); // Reserved
	uint8_t status = buffer->readU8();
	this->gyroWarning->GyroXStatusWarning = (status & 0x01) == 0;
	this->gyroWarning->GyroYStatusWarning = (status & 0x02) == 0;
	this->gyroWarning->GyroZStatusWarning = (status & 0x04) == 0;
	this->gyroData->SequenceNumber = buffer->readU8();
	this->gyroData->Temperature = buffer->readI16();
	buffer->readU32(); // CRC
	this->gyroData->Timestamp = buffer->readTimestamp();
	buffer->readEndOfFrame();
}

void Gyro::readToEndOfFrame(ModbusBuffer* buffer) {
	buffer->skipToNextFrame();
}

bool Gyro::checkGyroWarningForUpdates() {
	return this->gyroWarning->GyroXStatusWarning != this->previousGyroWarning.GyroXStatusWarning ||
			this->gyroWarning->GyroYStatusWarning != this->previousGyroWarning.GyroYStatusWarning ||
			this->gyroWarning->GyroZStatusWarning != this->previousGyroWarning.GyroZStatusWarning ||
			this->gyroWarning->SequenceNumberWarning != this->previousGyroWarning.SequenceNumberWarning ||
			this->gyroWarning->CRCMismatchWarning != this->previousGyroWarning.CRCMismatchWarning ||
			this->gyroWarning->GyroXSLDWarning != this->previousGyroWarning.GyroXSLDWarning ||
			this->gyroWarning->GyroXMODDACWarning != this->previousGyroWarning.GyroXMODDACWarning ||
			this->gyroWarning->GyroXPhaseWarning != this->previousGyroWarning.GyroXPhaseWarning ||
			this->gyroWarning->GyroXFlashWarning != this->previousGyroWarning.GyroXFlashWarning ||
			this->gyroWarning->GyroYSLDWarning != this->previousGyroWarning.GyroYSLDWarning ||
			this->gyroWarning->GyroYMODDACWarning != this->previousGyroWarning.GyroYMODDACWarning ||
			this->gyroWarning->GyroYPhaseWarning != this->previousGyroWarning.GyroYPhaseWarning ||
			this->gyroWarning->GyroYFlashWarning != this->previousGyroWarning.GyroYFlashWarning ||
			this->gyroWarning->GyroZSLDWarning != this->previousGyroWarning.GyroZSLDWarning ||
			this->gyroWarning->GyroZMODDACWarning != this->previousGyroWarning.GyroZMODDACWarning ||
			this->gyroWarning->GyroZPhaseWarning != this->previousGyroWarning.GyroZPhaseWarning ||
			this->gyroWarning->GyroXSLDTemperatureStatusWarning != this->previousGyroWarning.GyroXSLDTemperatureStatusWarning ||
			this->gyroWarning->GyroYSLDTemperatureStatusWarning != this->previousGyroWarning.GyroYSLDTemperatureStatusWarning ||
			this->gyroWarning->GyroZSLDTemperatureStatusWarning != this->previousGyroWarning.GyroZSLDTemperatureStatusWarning ||
			this->gyroWarning->GCBTemperatureStatusWarning != this->previousGyroWarning.GCBTemperatureStatusWarning ||
			this->gyroWarning->TemperatureStatusWarning != this->previousGyroWarning.TemperatureStatusWarning ||
			this->gyroWarning->GCBDSPSPIFlashStatusWarning != this->previousGyroWarning.GCBDSPSPIFlashStatusWarning ||
			this->gyroWarning->GCBFPGASPIFlashStatusWarning != this->previousGyroWarning.GCBFPGASPIFlashStatusWarning ||
			this->gyroWarning->DSPSPIFlashStatusWarning != this->previousGyroWarning.DSPSPIFlashStatusWarning ||
			this->gyroWarning->FPGASPIFlashStatusWarning != this->previousGyroWarning.FPGASPIFlashStatusWarning ||
			this->gyroWarning->GCB1_2VStatusWarning != this->previousGyroWarning.GCB1_2VStatusWarning ||
			this->gyroWarning->GCB3_3VStatusWarning != this->previousGyroWarning.GCB3_3VStatusWarning ||
			this->gyroWarning->GCB5VStatusWarning != this->previousGyroWarning.GCB5VStatusWarning ||
			this->gyroWarning->V1_2StatusWarning != this->previousGyroWarning.V1_2StatusWarning ||
			this->gyroWarning->V3_3StatusWarning != this->previousGyroWarning.V3_3StatusWarning ||
			this->gyroWarning->V5StatusWarning != this->previousGyroWarning.V5StatusWarning ||
			this->gyroWarning->GCBFPGAStatusWarning != this->previousGyroWarning.GCBFPGAStatusWarning ||
			this->gyroWarning->FPGAStatusWarning != this->previousGyroWarning.FPGAStatusWarning ||
			this->gyroWarning->HiSpeedSPORTStatusWarning != this->previousGyroWarning.HiSpeedSPORTStatusWarning ||
			this->gyroWarning->AuxSPORTStatusWarning != this->previousGyroWarning.AuxSPORTStatusWarning ||
			this->gyroWarning->SufficientSoftwareResourcesWarning != this->previousGyroWarning.SufficientSoftwareResourcesWarning ||
			this->gyroWarning->GyroEOVoltsPositiveWarning != this->previousGyroWarning.GyroEOVoltsPositiveWarning ||
			this->gyroWarning->GyroEOVoltsNegativeWarning != this->previousGyroWarning.GyroEOVoltsNegativeWarning ||
			this->gyroWarning->GyroXVoltsWarning != this->previousGyroWarning.GyroXVoltsWarning ||
			this->gyroWarning->GyroYVoltsWarning != this->previousGyroWarning.GyroYVoltsWarning ||
			this->gyroWarning->GyroZVoltsWarning != this->previousGyroWarning.GyroZVoltsWarning ||
			this->gyroWarning->GCBADCCommsWarning != this->previousGyroWarning.GCBADCCommsWarning ||
			this->gyroWarning->MSYNCExternalTimingWarning != this->previousGyroWarning.MSYNCExternalTimingWarning;
}

void Gyro::publishGyroWarning() {
	this->gyroWarning->AnyWarning = this->gyroWarning->GyroXStatusWarning ||
			this->gyroWarning->GyroYStatusWarning ||
			this->gyroWarning->GyroZStatusWarning ||
			this->gyroWarning->SequenceNumberWarning ||
			this->gyroWarning->CRCMismatchWarning ||
			this->gyroWarning->GyroXSLDWarning ||
			this->gyroWarning->GyroXMODDACWarning ||
			this->gyroWarning->GyroXPhaseWarning ||
			this->gyroWarning->GyroXFlashWarning ||
			this->gyroWarning->GyroYSLDWarning ||
			this->gyroWarning->GyroYMODDACWarning ||
			this->gyroWarning->GyroYPhaseWarning ||
			this->gyroWarning->GyroYFlashWarning ||
			this->gyroWarning->GyroZSLDWarning ||
			this->gyroWarning->GyroZMODDACWarning ||
			this->gyroWarning->GyroZPhaseWarning ||
			this->gyroWarning->GyroXSLDTemperatureStatusWarning ||
			this->gyroWarning->GyroYSLDTemperatureStatusWarning ||
			this->gyroWarning->GyroZSLDTemperatureStatusWarning ||
			this->gyroWarning->GCBTemperatureStatusWarning ||
			this->gyroWarning->TemperatureStatusWarning ||
			this->gyroWarning->GCBDSPSPIFlashStatusWarning ||
			this->gyroWarning->GCBFPGASPIFlashStatusWarning ||
			this->gyroWarning->DSPSPIFlashStatusWarning ||
			this->gyroWarning->FPGASPIFlashStatusWarning ||
			this->gyroWarning->GCB1_2VStatusWarning ||
			this->gyroWarning->GCB3_3VStatusWarning ||
			this->gyroWarning->GCB5VStatusWarning ||
			this->gyroWarning->V1_2StatusWarning ||
			this->gyroWarning->V3_3StatusWarning ||
			this->gyroWarning->V5StatusWarning ||
			this->gyroWarning->GCBFPGAStatusWarning ||
			this->gyroWarning->FPGAStatusWarning ||
			this->gyroWarning->HiSpeedSPORTStatusWarning ||
			this->gyroWarning->AuxSPORTStatusWarning ||
			this->gyroWarning->SufficientSoftwareResourcesWarning ||
			this->gyroWarning->GyroEOVoltsPositiveWarning ||
			this->gyroWarning->GyroEOVoltsNegativeWarning ||
			this->gyroWarning->GyroXVoltsWarning ||
			this->gyroWarning->GyroYVoltsWarning ||
			this->gyroWarning->GyroZVoltsWarning ||
			this->gyroWarning->GCBADCCommsWarning ||
			this->gyroWarning->MSYNCExternalTimingWarning;
	this->publisher->logGyroWarning();
	this->previousGyroWarning = *this->gyroWarning;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
