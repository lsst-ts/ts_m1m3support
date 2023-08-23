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

#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <TMA.h>
#include <Units.h>

using namespace LSST::M1M3::SS;

TMA::TMA(token) {
    _last_azimuth_data.timestamp = 0;
    _last_azimuth_data.actualPosition = NAN;
    _last_azimuth_data.demandPosition = NAN;
    _last_azimuth_data.actualVelocity = NAN;
    _last_azimuth_data.demandVelocity = NAN;

    _last_elevation_data.timestamp = 0;
    _last_elevation_data.actualPosition = NAN;
    _last_elevation_data.demandPosition = NAN;
    _last_elevation_data.actualVelocity = NAN;
    _last_elevation_data.demandVelocity = NAN;

    _azimuth_demand_acceleration = NAN;
    _azimuth_actual_acceleration = NAN;

    _elevation_demand_acceleration = NAN;
    _elevation_actual_acceleration = NAN;
}

void TMA::checkTimestamps(bool checkAzimuth, bool checkElevation) {
    if (ForceActuatorSettings::instance().useInclinometer == false ||
        ForceActuatorSettings::instance().useGyroscope == false ||
        ForceActuatorSettings::instance().useAccelerometers == false) {
        double timestamp = M1M3SSPublisher::instance().getTimestamp();
        if (checkAzimuth) {
            Model::instance().getSafetyController()->tmaAzimuthTimeout(_last_azimuth_data.timestamp - timestamp);
        }
        if (checkElevation) {
            Model::instance().getSafetyController()->tmaElevationTimeout(_last_elevation_data.timestamp -
                                                                    timestamp);
        }
    }
}

void TMA::updateTMAAzimuth(MTMount_azimuthC *data) {
    SPDLOG_TRACE("TMA: updateTMAAzimuth({})", data->actualPosition);

    if (_last_azimuth_data.timestamp != 0) {
        double elapsed = data->timestamp - _last_azimuth_data.timestamp;
        _azimuth_demand_acceleration = (data->demandVelocity - _last_azimuth_data.demandVelocity) / elapsed;
        _azimuth_actual_acceleration = (data->actualVelocity - _last_azimuth_data.actualVelocity) / elapsed;
    }

    memcpy(&_last_azimuth_data, data, sizeof(MTMount_azimuthC));

    Model::instance().getForceController()->updateTMAAzimuthForces(data);
}

void TMA::updateTMAElevation(MTMount_elevationC *data) {
    SPDLOG_TRACE("TMA: updateTMAElevation({})", data->actualPosition);

    if (_last_elevation_data.timestamp != 0) {
        double elapsed = data->timestamp - _last_elevation_data.timestamp;
        _elevation_demand_acceleration =
                (data->demandVelocity - _last_elevation_data.demandVelocity) / elapsed;
        _elevation_actual_acceleration =
                (data->actualVelocity - _last_elevation_data.actualVelocity) / elapsed;
    }

    memcpy(&_last_elevation_data, data, sizeof(MTMount_elevationC));

    Model::instance().getSafetyController()->tmaInclinometerDeviation(
            _last_elevation_data.actualPosition -
            M1M3SSPublisher::instance().getInclinometerData()->inclinometerAngle);
}

double TMA::getElevation(bool forceTelescope) {
    if (ForceActuatorSettings::instance().useInclinometer && forceTelescope == false) {
        return M1M3SSPublisher::instance().getInclinometerData()->inclinometerAngle;
    } else {
        return _last_elevation_data.actualPosition;
    }
}

void TMA::getMirrorAngularVelocities(double &x, double &y, double &z) {
    if (ForceActuatorSettings::instance().useGyroscope) {
        auto gyroData = M1M3SSPublisher::instance().getGyroData();
        x = gyroData->angularVelocityX;
        y = gyroData->angularVelocityY;
        z = gyroData->angularVelocityZ;
    } else {
        x = _last_elevation_data.actualVelocity * D2RAD;
        y = getElevationCos(true) * _last_azimuth_data.actualVelocity * D2RAD;
        z = getElevationSin(true) * _last_azimuth_data.actualVelocity * D2RAD;
    }
}

void TMA::getMirrorAngularAccelerations(double &ax, double &ay, double &az) {
    if (ForceActuatorSettings::instance().useAccelerometers) {
        auto accelerometer = M1M3SSPublisher::instance().getAccelerometerData();
        ax = accelerometer->angularAccelerationX;
        ay = accelerometer->angularAccelerationY;
        az = accelerometer->angularAccelerationZ;
    } else {
        if (isnan(_azimuth_actual_acceleration)) {
            throw std::runtime_error("No TMA azimuth data received, cannot calculate acceleration.");
        }
        if (isnan(_elevation_actual_acceleration)) {
            throw std::runtime_error("No TMA elevation data received, cannot calculate acceleration.");
        }
        ax = _elevation_actual_acceleration * D2RAD;
        ay = getElevationCos(true) * _azimuth_actual_acceleration * D2RAD;
        az = getElevationSin(true) * _azimuth_actual_acceleration * D2RAD;
    }
}
