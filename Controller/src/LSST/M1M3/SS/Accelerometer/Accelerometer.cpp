/*
 * Accelerometer.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include <Accelerometer.h>
#include <IPublisher.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <AccelerometerSettings.h>
#include <U16ArrayUtilities.h>
#include <Timestamp.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Accelerometer::Accelerometer(IPublisher* publisher, IFPGA* fpga, AccelerometerSettings* accelerometerSettings) {
	this->publisher = publisher;
	this->fpga = fpga;
	this->accelerometerSettings = accelerometerSettings;
	this->accelerometerData = this->publisher->getAccelerometerData();
	this->accelerometerWarning = this->publisher->getEventAccelerometerWarning();
}

void Accelerometer::sampleData() {
	this->fpga->writeRequestFIFO(FPGAAddresses::Accelerometers, 0);
	this->fpga->readU16ResponseFIFO(this->timestampBuffer, 4, 20);
	this->fpga->readSGLResponseFIFO(this->dataBuffer, 8, 20);
	this->accelerometerData->Timestamp = Timestamp::fromRaw(U16ArrayUtilities::u64(this->timestampBuffer, 0));
	this->accelerometerData->RawAccelerometerX[0] = this->dataBuffer[0];
	this->accelerometerData->RawAccelerometerZ[0] = this->dataBuffer[1];
	this->accelerometerData->RawAccelerometerY[0] = this->dataBuffer[2];
	this->accelerometerData->RawAccelerometerZ[1] = this->dataBuffer[3];
	this->accelerometerData->RawAccelerometerX[1] = this->dataBuffer[4];
	this->accelerometerData->RawAccelerometerZ[2] = this->dataBuffer[5];
	this->accelerometerData->RawAccelerometerY[1] = this->dataBuffer[6];
	this->accelerometerData->RawAccelerometerZ[3] = this->dataBuffer[7];
	this->accelerometerData->AngularAccelerationX = ((this->accelerometerData->RawAccelerometerZ[0] - this->accelerometerData->RawAccelerometerZ[2]) * this->accelerometerSettings->AngularAccelerationXCoefficient) / 2.0;
	this->accelerometerData->AngularAccelerationY = ((this->accelerometerData->RawAccelerometerZ[1] - this->accelerometerData->RawAccelerometerZ[3]) * this->accelerometerSettings->AngularAccelerationYCoefficient) / 2.0;
	this->accelerometerData->AngularAccelerationZ = ((this->accelerometerData->RawAccelerometerX[0] + this->accelerometerData->RawAccelerometerY[1] - this->accelerometerData->RawAccelerometerX[1] - this->accelerometerData->RawAccelerometerY[0]) * this->accelerometerSettings->AngularAccelerationZCoefficient) / 2.0;
	this->publisher->putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
