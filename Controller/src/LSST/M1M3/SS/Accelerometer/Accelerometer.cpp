/*
 * Accelerometer.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include <Accelerometer.h>
#include <AccelerometerSettings.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <M1M3SSPublisher.h>
#include <SAL_m1m3C.h>
#include <Timestamp.h>
#include <U16ArrayUtilities.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Accelerometer::Accelerometer(M1M3SSPublisher* publisher, FPGA* fpga, AccelerometerSettings* accelerometerSettings) {
	Log.Debug("Accelerometer: Accelerometer()");
	this->publisher = publisher;
	this->fpga = fpga;
	this->accelerometerSettings = accelerometerSettings;
	this->accelerometerData = this->publisher->getAccelerometerData();
	this->accelerometerWarning = this->publisher->getEventAccelerometerWarning();
}

void Accelerometer::sampleData() {
	Log.Trace("Accelerometer: sampleData()");
	this->fpga->writeRequestFIFO(FPGAAddresses::Accelerometers, 0);
	this->fpga->readU16ResponseFIFO(this->timestampBuffer, 4, 20);
	this->fpga->readSGLResponseFIFO(this->dataBuffer, 8, 20);
	this->accelerometerData->Timestamp = Timestamp::fromRaw(U16ArrayUtilities::u64(this->timestampBuffer, 0));
	this->accelerometerData->RawAccelerometer[0] = this->dataBuffer[0];
	this->accelerometerData->RawAccelerometer[1] = this->dataBuffer[1];
	this->accelerometerData->RawAccelerometer[2] = this->dataBuffer[2];
	this->accelerometerData->RawAccelerometer[3] = this->dataBuffer[3];
	this->accelerometerData->RawAccelerometer[4] = this->dataBuffer[4];
	this->accelerometerData->RawAccelerometer[5] = this->dataBuffer[5];
	this->accelerometerData->RawAccelerometer[6] = this->dataBuffer[6];
	this->accelerometerData->RawAccelerometer[7] = this->dataBuffer[7];
	this->accelerometerData->Accelerometer[0] = this->accelerometerData->RawAccelerometer[0] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[1] = this->accelerometerData->RawAccelerometer[1] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[2] = this->accelerometerData->RawAccelerometer[2] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[3] = this->accelerometerData->RawAccelerometer[3] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[4] = this->accelerometerData->RawAccelerometer[4] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[5] = this->accelerometerData->RawAccelerometer[5] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[6] = this->accelerometerData->RawAccelerometer[6] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->Accelerometer[7] = this->accelerometerData->RawAccelerometer[7] * this->accelerometerSettings->VoltsToMetersPerSecondSqrd;
	this->accelerometerData->AngularAccelerationX = (this->accelerometerData->Accelerometer[7] - this->accelerometerData->Accelerometer[5]) / this->accelerometerSettings->AngularAccelerationXDistance;
	this->accelerometerData->AngularAccelerationY = (this->accelerometerData->Accelerometer[2] - this->accelerometerData->Accelerometer[0]) / this->accelerometerSettings->AngularAccelerationYDistance;
	this->accelerometerData->AngularAccelerationZ = (this->accelerometerData->Accelerometer[0] + this->accelerometerData->Accelerometer[2] - this->accelerometerData->Accelerometer[4] - this->accelerometerData->Accelerometer[6]) / (this->accelerometerSettings->AngularAccelerationZDistance * 2);
	this->publisher->putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
