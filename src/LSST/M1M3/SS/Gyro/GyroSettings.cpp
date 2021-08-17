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

#include <GyroSettings.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void GyroSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        DataRate = doc["DataRate"].as<int>();
        AngularVelocityXOffset = doc["AngularVelocityXOffset"].as<float>();
        AngularVelocityYOffset = doc["AngularVelocityYOffset"].as<float>();
        AngularVelocityZOffset = doc["AngularVelocityZOffset"].as<float>();
        AxesMatrix = doc["AxesMatrix"].as<std::vector<double>>();
    } catch (YAML::Exception &ex) {
        SPDLOG_CRITICAL("YAML Loading {}: {}", filename, ex.what());
        exit(EXIT_FAILURE);
    }

    if (AxesMatrix.size() != 9) {
        SPDLOG_CRITICAL("Invalid AxesMatrix length: {}, expected 9", AxesMatrix.size());
        exit(EXIT_FAILURE);
    }
}
