#include <Accelerometer.h>
#include <AccelerometerSettings.h>
#include <IFPGA.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Accelerometer::Accelerometer(AccelerometerSettings* accelerometerSettings, M1M3SSPublisher* publisher) {
    spdlog::debug("Accelerometer: Accelerometer()");
    this->accelerometerSettings = accelerometerSettings;
    this->publisher = publisher;

    this->accelerometerData = this->publisher->getAccelerometerData();
    this->accelerometerWarning = this->publisher->getEventAccelerometerWarning();

    memset(this->accelerometerData, 0, sizeof(MTM1M3_accelerometerDataC));
    memset(this->accelerometerWarning, 0, sizeof(MTM1M3_logevent_accelerometerWarningC));
}

void Accelerometer::processData() {
    // TODO: Handle no data available
    // TODO: Handle acceleration limits, push to safety controller
    spdlog::trace("Accelerometer: processData()");
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    this->accelerometerData->timestamp = Timestamp::fromFPGA(fpgaData->AccelerometerSampleTimestamp);
    this->accelerometerData->rawAccelerometer[0] = fpgaData->AccelerometerRaw1;
    this->accelerometerData->rawAccelerometer[1] = fpgaData->AccelerometerRaw2;
    this->accelerometerData->rawAccelerometer[2] = fpgaData->AccelerometerRaw3;
    this->accelerometerData->rawAccelerometer[3] = fpgaData->AccelerometerRaw4;
    this->accelerometerData->rawAccelerometer[4] = fpgaData->AccelerometerRaw5;
    this->accelerometerData->rawAccelerometer[5] = fpgaData->AccelerometerRaw6;
    this->accelerometerData->rawAccelerometer[6] = fpgaData->AccelerometerRaw7;
    this->accelerometerData->rawAccelerometer[7] = fpgaData->AccelerometerRaw8;
    this->accelerometerData->accelerometer[0] = ((this->accelerometerData->rawAccelerometer[0] -
                                                  this->accelerometerSettings->AccelerometerBias[0]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[0]) *
                                                        this->accelerometerSettings->AccelerometerScalars[0] +
                                                this->accelerometerSettings->AccelerometerOffsets[0];
    this->accelerometerData->accelerometer[1] = ((this->accelerometerData->rawAccelerometer[1] -
                                                  this->accelerometerSettings->AccelerometerBias[1]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[1]) *
                                                        this->accelerometerSettings->AccelerometerScalars[1] +
                                                this->accelerometerSettings->AccelerometerOffsets[1];
    this->accelerometerData->accelerometer[2] = ((this->accelerometerData->rawAccelerometer[2] -
                                                  this->accelerometerSettings->AccelerometerBias[2]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[2]) *
                                                        this->accelerometerSettings->AccelerometerScalars[2] +
                                                this->accelerometerSettings->AccelerometerOffsets[2];
    this->accelerometerData->accelerometer[3] = ((this->accelerometerData->rawAccelerometer[3] -
                                                  this->accelerometerSettings->AccelerometerBias[3]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[3]) *
                                                        this->accelerometerSettings->AccelerometerScalars[3] +
                                                this->accelerometerSettings->AccelerometerOffsets[3];
    this->accelerometerData->accelerometer[4] = ((this->accelerometerData->rawAccelerometer[4] -
                                                  this->accelerometerSettings->AccelerometerBias[4]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[4]) *
                                                        this->accelerometerSettings->AccelerometerScalars[4] +
                                                this->accelerometerSettings->AccelerometerOffsets[4];
    this->accelerometerData->accelerometer[5] = ((this->accelerometerData->rawAccelerometer[5] -
                                                  this->accelerometerSettings->AccelerometerBias[5]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[5]) *
                                                        this->accelerometerSettings->AccelerometerScalars[5] +
                                                this->accelerometerSettings->AccelerometerOffsets[5];
    this->accelerometerData->accelerometer[6] = ((this->accelerometerData->rawAccelerometer[6] -
                                                  this->accelerometerSettings->AccelerometerBias[6]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[6]) *
                                                        this->accelerometerSettings->AccelerometerScalars[6] +
                                                this->accelerometerSettings->AccelerometerOffsets[6];
    this->accelerometerData->accelerometer[7] = ((this->accelerometerData->rawAccelerometer[7] -
                                                  this->accelerometerSettings->AccelerometerBias[7]) *
                                                 this->accelerometerSettings->AccelerometerSensitivity[7]) *
                                                        this->accelerometerSettings->AccelerometerScalars[7] +
                                                this->accelerometerSettings->AccelerometerOffsets[7];
    this->accelerometerData->angularAccelerationX =
            (this->accelerometerData->accelerometer[7] - this->accelerometerData->accelerometer[5]) /
            this->accelerometerSettings->AngularAccelerationXDistance;
    this->accelerometerData->angularAccelerationY =
            (this->accelerometerData->accelerometer[2] - this->accelerometerData->accelerometer[0]) /
            this->accelerometerSettings->AngularAccelerationYDistance;
    this->accelerometerData->angularAccelerationZ =
            (this->accelerometerData->accelerometer[0] + this->accelerometerData->accelerometer[2] -
             this->accelerometerData->accelerometer[4] - this->accelerometerData->accelerometer[6]) /
            (this->accelerometerSettings->AngularAccelerationZDistance * 2);
    this->publisher->putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
