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

#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

#include <AccelerometerSettings.h>

using namespace LSST::M1M3::SS;

AccelerometerSettings::AccelerometerSettings(token) {
#ifndef WITH_SAL_KAFKA
    memset(angularAccelerationDistance, 0, sizeof(angularAccelerationDistance));
    memset(bias, 0, sizeof(bias));
    memset(sensitivity, 0, sizeof(sensitivity));
    memset(accelerometerOffset, 0, sizeof(accelerometerOffset));
    memset(scalar, 0, sizeof(scalar));
#endif
}

void AccelerometerSettings::load(YAML::Node doc) {
    try {
        SPDLOG_INFO("Loading AccelerometerSettings");

        angularAccelerationDistance[0] = doc["AngularAccelerationXDistance"].as<float>();
        angularAccelerationDistance[1] = doc["AngularAccelerationYDistance"].as<float>();
        angularAccelerationDistance[2] = doc["AngularAccelerationZDistance"].as<float>();
        for (int i = 0; i < 8; i++) {
            auto accNode = doc["Accelerometer" + std::to_string(i + 1)];
            bias[i] = accNode["Bias"].as<float>();
            sensitivity[i] = accNode["Sensitivity"].as<float>();
            accelerometerOffset[i] = accNode["Offset"].as<float>();
            scalar[i] = accNode["Scalar"].as<float>();
        }
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading AccelerometerSettings: {}", ex.what()));
    }

    log();
}
