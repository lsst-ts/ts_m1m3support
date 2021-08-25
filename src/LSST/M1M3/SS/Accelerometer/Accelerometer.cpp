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
#include <Conversion.h>
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
    SPDLOG_DEBUG("Accelerometer: Accelerometer()");
    _accelerometerSettings = accelerometerSettings;

    _accelerometerData = M1M3SSPublisher::get().getAccelerometerData();
    _accelerometerWarning = M1M3SSPublisher::get().getEventAccelerometerWarning();

    memset(_accelerometerData, 0, sizeof(MTM1M3_accelerometerDataC));
    memset(_accelerometerWarning, 0, sizeof(MTM1M3_logevent_accelerometerWarningC));
}

void Accelerometer::processData() {
    // TODO: Handle no data available
    // TODO: Handle acceleration limits, push to safety controller
    SPDLOG_TRACE("Accelerometer: processData()");
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    _accelerometerData->timestamp = Timestamp::fromFPGA(fpgaData->AccelerometerSampleTimestamp);
    for (int i = 0; i < 8; i++) {
        _accelerometerData->rawAccelerometer[i] = fpgaData->AccelerometerRaw[i];
        _accelerometerData->accelerometer[i] =
                G2M_S_2(((_accelerometerData->rawAccelerometer[i] - _accelerometerSettings->bias[i]) *
                         _accelerometerSettings->sensitivity[i]) *
                                _accelerometerSettings->scalar[i] +
                        _accelerometerSettings->accelerometerOffset[i]);
    }
    _accelerometerData->angularAccelerationX =
            (_accelerometerData->accelerometer[7] - _accelerometerData->accelerometer[5]) /
            _accelerometerSettings->angularAccelerationDistance[0];
    _accelerometerData->angularAccelerationY =
            (_accelerometerData->accelerometer[2] - _accelerometerData->accelerometer[0]) /
            _accelerometerSettings->angularAccelerationDistance[1];
    _accelerometerData->angularAccelerationZ =
            (_accelerometerData->accelerometer[0] + _accelerometerData->accelerometer[2] -
             _accelerometerData->accelerometer[4] - _accelerometerData->accelerometer[6]) /
            (_accelerometerSettings->angularAccelerationDistance[2] * 2);
    M1M3SSPublisher::get().putAccelerometerData();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
