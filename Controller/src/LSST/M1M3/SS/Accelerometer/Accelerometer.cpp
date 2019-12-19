/*
 * Accelerometer.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include "Accelerometer.h"

#include <cstring>

#include "SAL_MTM1M3C.h"

#include "AccelerometerSettings.h"
#include "Log.h"
#include "M1M3SSPublisher.h"
#include "SALEnumerations.h"
#include "SupportFPGAData.h"
#include "Timestamp.h"

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
    memset(this->accelerometerData, 0, sizeof(MTM1M3_accelerometerDataC));
    memset(this->accelerometerWarning, 0, sizeof(MTM1M3_logevent_accelerometerWarningC));
}

void Accelerometer::processData() {
    // TODO: Handle no data available
    // TODO: Handle acceleration limits, push to safety controller
    Log.Trace("Accelerometer: processData()");
    this->accelerometerData->timestamp = Timestamp::fromFPGA(this->fpgaData->AccelerometerSampleTimestamp);
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer1X] = this->fpgaData->AccelerometerRaw1;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer1Y] = this->fpgaData->AccelerometerRaw2;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer2X] = this->fpgaData->AccelerometerRaw3;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer2Y] = this->fpgaData->AccelerometerRaw4;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer3X] = this->fpgaData->AccelerometerRaw5;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer3Y] = this->fpgaData->AccelerometerRaw6;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer4X] = this->fpgaData->AccelerometerRaw7;
    this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer4Y] = this->fpgaData->AccelerometerRaw8;
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer1X] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer1X] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer1X]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer1X]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer1X] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer1X];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer1Y] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer1Y] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer1Y]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer1Y]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer1Y] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer1Y];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer2X] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer2X] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer2X]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer2X]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer2X] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer2X];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer2Y] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer2Y] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer2Y]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer2Y]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer2Y] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer2Y];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer3X] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer3X] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer3X]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer3X]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer3X] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer3X];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer3Y] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer3Y] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer3Y]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer3Y]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer3Y] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer3Y];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer4X] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer4X] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer4X]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer4X]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer4X] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer4X];
    this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer4Y] = ((this->accelerometerData->rawAccelerometers[AccelerometerIndexMap::Accelerometer4Y] - this->accelerometerSettings->AccelerometerBias[AccelerometerIndexMap::Accelerometer4Y]) * this->accelerometerSettings->AccelerometerSensitivity[AccelerometerIndexMap::Accelerometer4Y]) * this->accelerometerSettings->AccelerometerScalars[AccelerometerIndexMap::Accelerometer4Y] + this->accelerometerSettings->AccelerometerOffsets[AccelerometerIndexMap::Accelerometer4Y];
    this->accelerometerData->angularAccelerationX = (this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer4Y] - this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer3Y]) / this->accelerometerSettings->AngularAccelerationXDistance;
    this->accelerometerData->angularAccelerationY = (this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer2X] - this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer1X]) / this->accelerometerSettings->AngularAccelerationYDistance;
    this->accelerometerData->angularAccelerationZ = (this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer1X] + this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer2X] - this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer3X] - this->accelerometerData->accelerometers[AccelerometerIndexMap::Accelerometer4X]) / (this->accelerometerSettings->AngularAccelerationZDistance * 2);
    this->publisher->putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
