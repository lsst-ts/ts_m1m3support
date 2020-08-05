/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

Accelerometer::Accelerometer(AccelerometerSettings* accelerometerSettings) {
    spdlog::debug("Accelerometer: Accelerometer()");
    _accelerometerSettings = accelerometerSettings;

    _accelerometerData = M1M3SSPublisher::get().getAccelerometerData();
    _accelerometerWarning = M1M3SSPublisher::get().getEventAccelerometerWarning();

    memset(_accelerometerData, 0, sizeof(MTM1M3_accelerometerDataC));
    memset(_accelerometerWarning, 0, sizeof(MTM1M3_logevent_accelerometerWarningC));
}

void Accelerometer::processData() {
    // TODO: Handle no data available
    // TODO: Handle acceleration limits, push to safety controller
    spdlog::trace("Accelerometer: processData()");
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    _accelerometerData->timestamp = Timestamp::fromFPGA(fpgaData->AccelerometerSampleTimestamp);
    _accelerometerData->rawAccelerometer[0] = fpgaData->AccelerometerRaw1;
    _accelerometerData->rawAccelerometer[1] = fpgaData->AccelerometerRaw2;
    _accelerometerData->rawAccelerometer[2] = fpgaData->AccelerometerRaw3;
    _accelerometerData->rawAccelerometer[3] = fpgaData->AccelerometerRaw4;
    _accelerometerData->rawAccelerometer[4] = fpgaData->AccelerometerRaw5;
    _accelerometerData->rawAccelerometer[5] = fpgaData->AccelerometerRaw6;
    _accelerometerData->rawAccelerometer[6] = fpgaData->AccelerometerRaw7;
    _accelerometerData->rawAccelerometer[7] = fpgaData->AccelerometerRaw8;
    _accelerometerData->accelerometer[0] =
            ((_accelerometerData->rawAccelerometer[0] - _accelerometerSettings->AccelerometerBias[0]) *
             _accelerometerSettings->AccelerometerSensitivity[0]) *
                    _accelerometerSettings->AccelerometerScalars[0] +
            _accelerometerSettings->AccelerometerOffsets[0];
    _accelerometerData->accelerometer[1] =
            ((_accelerometerData->rawAccelerometer[1] - _accelerometerSettings->AccelerometerBias[1]) *
             _accelerometerSettings->AccelerometerSensitivity[1]) *
                    _accelerometerSettings->AccelerometerScalars[1] +
            _accelerometerSettings->AccelerometerOffsets[1];
    _accelerometerData->accelerometer[2] =
            ((_accelerometerData->rawAccelerometer[2] - _accelerometerSettings->AccelerometerBias[2]) *
             _accelerometerSettings->AccelerometerSensitivity[2]) *
                    _accelerometerSettings->AccelerometerScalars[2] +
            _accelerometerSettings->AccelerometerOffsets[2];
    _accelerometerData->accelerometer[3] =
            ((_accelerometerData->rawAccelerometer[3] - _accelerometerSettings->AccelerometerBias[3]) *
             _accelerometerSettings->AccelerometerSensitivity[3]) *
                    _accelerometerSettings->AccelerometerScalars[3] +
            _accelerometerSettings->AccelerometerOffsets[3];
    _accelerometerData->accelerometer[4] =
            ((_accelerometerData->rawAccelerometer[4] - _accelerometerSettings->AccelerometerBias[4]) *
             _accelerometerSettings->AccelerometerSensitivity[4]) *
                    _accelerometerSettings->AccelerometerScalars[4] +
            _accelerometerSettings->AccelerometerOffsets[4];
    _accelerometerData->accelerometer[5] =
            ((_accelerometerData->rawAccelerometer[5] - _accelerometerSettings->AccelerometerBias[5]) *
             _accelerometerSettings->AccelerometerSensitivity[5]) *
                    _accelerometerSettings->AccelerometerScalars[5] +
            _accelerometerSettings->AccelerometerOffsets[5];
    _accelerometerData->accelerometer[6] =
            ((_accelerometerData->rawAccelerometer[6] - _accelerometerSettings->AccelerometerBias[6]) *
             _accelerometerSettings->AccelerometerSensitivity[6]) *
                    _accelerometerSettings->AccelerometerScalars[6] +
            _accelerometerSettings->AccelerometerOffsets[6];
    _accelerometerData->accelerometer[7] =
            ((_accelerometerData->rawAccelerometer[7] - _accelerometerSettings->AccelerometerBias[7]) *
             _accelerometerSettings->AccelerometerSensitivity[7]) *
                    _accelerometerSettings->AccelerometerScalars[7] +
            _accelerometerSettings->AccelerometerOffsets[7];
    _accelerometerData->angularAccelerationX =
            (_accelerometerData->accelerometer[7] - _accelerometerData->accelerometer[5]) /
            _accelerometerSettings->AngularAccelerationXDistance;
    _accelerometerData->angularAccelerationY =
            (_accelerometerData->accelerometer[2] - _accelerometerData->accelerometer[0]) /
            _accelerometerSettings->AngularAccelerationYDistance;
    _accelerometerData->angularAccelerationZ =
            (_accelerometerData->accelerometer[0] + _accelerometerData->accelerometer[2] -
             _accelerometerData->accelerometer[4] - _accelerometerData->accelerometer[6]) /
            (_accelerometerSettings->AngularAccelerationZDistance * 2);
    M1M3SSPublisher::get().putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
