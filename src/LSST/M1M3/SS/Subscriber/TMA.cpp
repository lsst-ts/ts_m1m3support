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

#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>
#include <TMA.h>

using namespace LSST::M1M3::SS;

TMA::TMA(token) {
    _azimuth_Timestamp = 0;
    _elevation_Timestamp = 0;
    _elevation_Actual = NAN;
}

void TMA::checkTimestamps(bool checkAzimuth, bool checkElevation) {
    if (SettingReader::instance().getForceActuatorSettings()->useInclinometer == false) {
        double timestamp = M1M3SSPublisher::get().getTimestamp();
        if (checkAzimuth) {
            Model::get().getSafetyController()->tmaAzimuthTimeout(_azimuth_Timestamp - timestamp);
        }
        if (checkElevation) {
            Model::get().getSafetyController()->tmaElevationTimeout(_elevation_Timestamp - timestamp);
        }
    }
}

void TMA::updateTMAAzimuth(MTMount_azimuthC* data) {
    SPDLOG_TRACE("TMA: updateTMAAzimuth({})", data->actualPosition);
    _azimuth_Actual = data->actualPosition;
    _azimuth_Timestamp = data->timestamp;

    Model::get().getForceController()->updateTMAAzimuthForces(data);
}

void TMA::updateTMAElevation(MTMount_elevationC* data) {
    SPDLOG_TRACE("TMA: updateTMAElevation({})", data->actualPosition);
    _elevation_Actual = data->actualPosition;
    _elevation_Timestamp = data->timestamp;

    Model::get().getSafetyController()->tmaInclinometerDeviation(
            _elevation_Actual - M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle);
}

double TMA::getElevation() {
    if (SettingReader::instance().getForceActuatorSettings()->useInclinometer) {
        return M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle;
    } else {
        return _elevation_Actual;
    }
}
