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

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include <GyroSettings.h>

using namespace LSST::M1M3::SS;

GyroSettings::GyroSettings(token) {}

void GyroSettings::load(YAML::Node doc) {
    try {
        SPDLOG_INFO("Loading GyroSettings");

        dataRate = doc["DataRate"].as<int>();
        angularVelocityOffset[0] = doc["AngularVelocityXOffset"].as<float>();
        angularVelocityOffset[1] = doc["AngularVelocityYOffset"].as<float>();
        angularVelocityOffset[2] = doc["AngularVelocityZOffset"].as<float>();
        AxesMatrix = doc["AxesMatrix"].as<std::vector<double>>();
    } catch (YAML::Exception& ex) {
        throw std::runtime_error(fmt::format("YAML Loading GyroSettings: {}", ex.what()));
    }

    if (AxesMatrix.size() != 9) {
        throw std::runtime_error(fmt::format("Invalid AxesMatrix length: {}, expected 9", AxesMatrix.size()));
    }

    log();
}
