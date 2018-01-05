/*
 * Gyro.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#include <Gyro.h>
#include <GyroSettings.h>
#include <IFPGA.h>
#include <IPublisher.h>
#include <SAL_m1m3C.h>
#include <boost/lexical_cast.hpp>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Gyro::Gyro(GyroSettings* gyroSettings, IFPGA* fpga, IPublisher* publisher) {
	this->gyroSettings = gyroSettings;
	this->fpga = fpga;
	this->publisher = publisher;
	this->gyroData = this->publisher->getGyroData();
	this->setBuffer(&this->enterConfigurationBuffer, "=CONFIG,1\r\n");
	this->setBuffer(&this->exitConfigurationBuffer, "=CONFIG,0\r\n");
	this->setBuffer(&this->resetBuffer, "=RSTCFG\r\n");
	this->setBuffer(&this->rotationFormatRateBuffer, "=ROTFMT,RATE\r\n");
	this->setBuffer(&this->rotationUnitsRadiansBuffer, "=ROTUNITS,RAD\r\n");
	this->setBuffer(&this->dataRateBuffer, "=DR," + boost::lexical_cast<std::string>(this->gyroSettings->DataRate) + "\r\n");

	cout << "ENTERCONFIG: " << this->enterConfigurationBuffer.Size << " ";
	for(int i = 0; i < this->enterConfigurationBuffer.Size; ++i) {
		cout << this->enterConfigurationBuffer.Buffer[i] << " ";
	}
	cout << endl;

	std::string axesText = "=AXES";
	for(unsigned int i = 0; i < this->gyroSettings->AxesMatrix.size(); ++i) {
		axesText = axesText + "," + boost::lexical_cast<std::string>(this->gyroSettings->AxesMatrix[i]);
	}
	this->setBuffer(&this->axesBuffer, axesText + "\r\n");
}

void Gyro::enterConfigurationMode() {
	this->writeCommand(&this->enterConfigurationBuffer);
}

void Gyro::exitConfigurationMode() {
	this->writeCommand(&this->exitConfigurationBuffer);
}

void Gyro::resetConfiguration() {
	this->writeCommand(&this->resetBuffer);
}

void Gyro::setRotationFormatRate() {
	this->writeCommand(&this->rotationFormatRateBuffer);
}

void Gyro::setRotationUnitsRadians() {
	this->writeCommand(&this->rotationUnitsRadiansBuffer);
}

void Gyro::setAxis() {
	this->writeCommand(&this->axesBuffer);
}

void Gyro::setDataRate() {
	this->writeCommand(&this->dataRateBuffer);
}

void Gyro::setBuffer(GyroBuffer* buffer, std::string text) {
	buffer->Size = text.size() + 1;
	for(int i = 0; i < (buffer->Size - 1); ++i) {
		buffer->Buffer[i] = 0x1000 | (((uint16_t)text[i]) << 1);
	}
	buffer->Buffer[buffer->Size - 1] = 0x2100;
}

void Gyro::publishGyroData() {
	this->gyroData->Timestamp = this->publisher->getTimestamp();
	this->publisher->putGyroData();
}

void Gyro::writeCommand(GyroBuffer* buffer) {
	this->fpga->writeCommandFIFO(buffer->Buffer, buffer->Size, 0);
}

void Gyro::readCommand() {
	this->fpga->writeRequestFIFO(0, 0);
	this->fpga->readU16ResponseFIFO(0, 0, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
