/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <cstring>

#include <spdlog/spdlog.h>

#include <NiFpga_M1M3SupportFPGA.h>

#include <Accelerometer.h>
#include <AccelerometerSettings.h>
#include <Conversion.h>
#include <IFPGA.h>
#include <M1M3SSPublisher.h>
#include <SupportFPGAData.h>
#include <Timestamp.h>
#include <TMA.h>
#include <Units.h>

using namespace LSST::M1M3::SS;

Accelerometer::Accelerometer() {
    SPDLOG_DEBUG("Accelerometer: Accelerometer()");

    _accelerometerData = M1M3SSPublisher::instance().getAccelerometerData();
    _accelerometerWarning = M1M3SSPublisher::instance().getEventAccelerometerWarning();

    memset(_accelerometerData, 0, sizeof(MTM1M3_accelerometerDataC));
    memset(_accelerometerWarning, 0, sizeof(MTM1M3_logevent_accelerometerWarningC));
    _recordRaw = false;
}

void Accelerometer::processData() {
    // TODO: Handle no data available
    // TODO: Handle acceleration limits, push to safety controller
    SPDLOG_TRACE("Accelerometer: processData()");
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    _accelerometerData->timestamp = Timestamp::fromFPGA(fpgaData->AccelerometerSampleTimestamp);

    auto& accelerometerSettings = AccelerometerSettings::instance();

    for (int i = 0; i < 8; i++) {
        _accelerometerData->rawAccelerometer[i] = fpgaData->AccelerometerRaw[i];
        _accelerometerData->accelerometer[i] =
                G2M_S_2(((_accelerometerData->rawAccelerometer[i] - accelerometerSettings.bias[i]) /
                         accelerometerSettings.sensitivity[i]) *
                                accelerometerSettings.scalar[i] +
                        accelerometerSettings.accelerometerOffset[i]);
    }

    _accelerometerData->angularAccelerationX =
            (RAD2D * (_accelerometerData->accelerometer[7] - _accelerometerData->accelerometer[5]) /
             accelerometerSettings.angularAccelerationDistance[0]);
    _accelerometerData->angularAccelerationY =
            (RAD2D * (_accelerometerData->accelerometer[2] - _accelerometerData->accelerometer[0]) /
             accelerometerSettings.angularAccelerationDistance[1]);

    _accelerometerData->angularAccelerationZ =
            (RAD2D * (_accelerometerData->accelerometer[4] - _accelerometerData->accelerometer[0]) /
             (accelerometerSettings.angularAccelerationDistance[2]));

    /** _accelerometerData->angularAccelerationZ =
            RAD2D * (_accelerometerData->accelerometer[0] + _accelerometerData->accelerometer[2] -
             _accelerometerData->accelerometer[4] - _accelerometerData->accelerometer[6]) /
            (accelerometerSettings.angularAccelerationDistance[2] * 2); */
    M1M3SSPublisher::instance().putAccelerometerData();
}

void Accelerometer::recordRaw() {
    while (_recordRaw) {
        double data[8];
        IFPGA::get().readRawAccelerometerFIFO(data);
    }
}
