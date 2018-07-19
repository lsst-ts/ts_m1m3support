/*
 * Accelerometer.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include <Accelerometer.h>
#include <AccelerometerSettings.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <SAL_m1m3C.h>
#include <Log.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Accelerometer::Accelerometer(AccelerometerSettings* accelerometerSettings, SupportFPGAData* fpgaData, M1M3SSPublisher* publisher) {
	Log.Debug("Accelerometer: Accelerometer()");
	this->accelerometerSettings = accelerometerSettings;
	this->fpgaData = fpgaData;
	this->publisher = publisher;

	this->accelerometerData = this->publisher->getAccelerometerData();
	this->accelerometerWarning = this->publisher->getEventAccelerometerWarning();

	memset(this->accelerometerData, 0, sizeof(m1m3_AccelerometerDataC));
	memset(this->accelerometerWarning, 0, sizeof(m1m3_logevent_AccelerometerWarningC));
}

void Accelerometer::processData() {
	// TODO: Handle no data available
	// TODO: Handle acceleration limits, push to safety controller
	Log.Trace("Accelerometer: processData()");
	this->accelerometerData->Timestamp = Timestamp::fromFPGA(this->fpgaData->AccelerometerSampleTimestamp);
	this->accelerometerData->RawAccelerometer[0] = this->fpgaData->AccelerometerRaw1;
	this->accelerometerData->RawAccelerometer[1] = this->fpgaData->AccelerometerRaw2;
	this->accelerometerData->RawAccelerometer[2] = this->fpgaData->AccelerometerRaw3;
	this->accelerometerData->RawAccelerometer[3] = this->fpgaData->AccelerometerRaw4;
	this->accelerometerData->RawAccelerometer[4] = this->fpgaData->AccelerometerRaw5;
	this->accelerometerData->RawAccelerometer[5] = this->fpgaData->AccelerometerRaw6;
	this->accelerometerData->RawAccelerometer[6] = this->fpgaData->AccelerometerRaw7;
	this->accelerometerData->RawAccelerometer[7] = this->fpgaData->AccelerometerRaw8;
	this->accelerometerData->Accelerometer[0] = ((this->accelerometerData->RawAccelerometer[0] - this->accelerometerSettings->AccelerometerBias[0]) * this->accelerometerSettings->AccelerometerSensitivity[0]) * this->accelerometerSettings->AccelerometerScalars[0] + this->accelerometerSettings->AccelerometerOffsets[0];
	this->accelerometerData->Accelerometer[1] = ((this->accelerometerData->RawAccelerometer[1] - this->accelerometerSettings->AccelerometerBias[1]) * this->accelerometerSettings->AccelerometerSensitivity[1]) * this->accelerometerSettings->AccelerometerScalars[1] + this->accelerometerSettings->AccelerometerOffsets[1];
	this->accelerometerData->Accelerometer[2] = ((this->accelerometerData->RawAccelerometer[2] - this->accelerometerSettings->AccelerometerBias[2]) * this->accelerometerSettings->AccelerometerSensitivity[2]) * this->accelerometerSettings->AccelerometerScalars[2] + this->accelerometerSettings->AccelerometerOffsets[2];
	this->accelerometerData->Accelerometer[3] = ((this->accelerometerData->RawAccelerometer[3] - this->accelerometerSettings->AccelerometerBias[3]) * this->accelerometerSettings->AccelerometerSensitivity[3]) * this->accelerometerSettings->AccelerometerScalars[3] + this->accelerometerSettings->AccelerometerOffsets[3];
	this->accelerometerData->Accelerometer[4] = ((this->accelerometerData->RawAccelerometer[4] - this->accelerometerSettings->AccelerometerBias[4]) * this->accelerometerSettings->AccelerometerSensitivity[4]) * this->accelerometerSettings->AccelerometerScalars[4] + this->accelerometerSettings->AccelerometerOffsets[4];
	this->accelerometerData->Accelerometer[5] = ((this->accelerometerData->RawAccelerometer[5] - this->accelerometerSettings->AccelerometerBias[5]) * this->accelerometerSettings->AccelerometerSensitivity[5]) * this->accelerometerSettings->AccelerometerScalars[5] + this->accelerometerSettings->AccelerometerOffsets[5];
	this->accelerometerData->Accelerometer[6] = ((this->accelerometerData->RawAccelerometer[6] - this->accelerometerSettings->AccelerometerBias[6]) * this->accelerometerSettings->AccelerometerSensitivity[6]) * this->accelerometerSettings->AccelerometerScalars[6] + this->accelerometerSettings->AccelerometerOffsets[6];
	this->accelerometerData->Accelerometer[7] = ((this->accelerometerData->RawAccelerometer[7] - this->accelerometerSettings->AccelerometerBias[7]) * this->accelerometerSettings->AccelerometerSensitivity[7]) * this->accelerometerSettings->AccelerometerScalars[7] + this->accelerometerSettings->AccelerometerOffsets[7];
	this->accelerometerData->AngularAccelerationX = (this->accelerometerData->Accelerometer[7] - this->accelerometerData->Accelerometer[5]) / this->accelerometerSettings->AngularAccelerationXDistance;
	this->accelerometerData->AngularAccelerationY = (this->accelerometerData->Accelerometer[2] - this->accelerometerData->Accelerometer[0]) / this->accelerometerSettings->AngularAccelerationYDistance;
	this->accelerometerData->AngularAccelerationZ = (this->accelerometerData->Accelerometer[0] + this->accelerometerData->Accelerometer[2] - this->accelerometerData->Accelerometer[4] - this->accelerometerData->Accelerometer[6]) / (this->accelerometerSettings->AngularAccelerationZDistance * 2);
	this->publisher->putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
