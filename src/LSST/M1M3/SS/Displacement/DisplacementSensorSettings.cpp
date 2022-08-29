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

#include <DisplacementSensorSettings.h>
#include <TableLoader.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void DisplacementSensorSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        TableLoader::loadTable(1, 1, 8, &this->ConverterMatrix, doc["PositionTablePath"].as<std::string>());
        NPorts = doc["NPorts"].as<std::vector<int32_t>>();
        NOffsets = doc["NOffsets"].as<std::vector<double>>();

        xPositionOffset = doc["XPositionOffset"].as<double>();
        yPositionOffset = doc["YPositionOffset"].as<double>();
        zPositionOffset = doc["ZPositionOffset"].as<double>();

        xRotationOffset = doc["XRotationOffset"].as<double>();
        yRotationOffset = doc["YRotationOffset"].as<double>();
        zRotationOffset = doc["ZRotationOffset"].as<double>();
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }

    if (NPorts.size() != 8) {
        throw std::runtime_error(fmt::format("Invalid NPorts length: {}, expected 8", NPorts.size()));
    }
    if (NOffsets.size() != 8) {
        throw std::runtime_error(fmt::format("Invalid NOffsets length: {}, expected 8", NOffsets.size()));
    }

    log();
}
