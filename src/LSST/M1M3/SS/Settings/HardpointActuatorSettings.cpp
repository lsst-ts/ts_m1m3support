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

#include <HardpointActuatorSettings.h>
#include <TableLoader.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

HardpointActuatorSettings::HardpointActuatorSettings() { memset(_encoderOffset, 0, sizeof(_encoderOffset)); }

void HardpointActuatorSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        TableLoader::loadTable(1, 1, 6, &HardpointDisplacementToMirrorPosition,
                               doc["HardpointDisplacementToMirrorPositionTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &MirrorPositionToHardpointDisplacement,
                               doc["MirrorPositionToHardpointDisplacementTablePath"].as<std::string>());
        MicrometersPerStep = doc["MicrometersPerStep"].as<double>();
        MicrometersPerEncoder = doc["MicrometersPerEncoder"].as<double>();

        std::vector<int32_t> offsetVec = doc["HPEncoderOffsets"].as<std::vector<int32_t>>();
        if (offsetVec.size() != 6) {
            throw std::runtime_error(fmt::format("Invalid HP offsets size, expected 6, received {}", offsetVec.size());
        }
        memcpy(_encoderOffset, offsetVec.data(), HP_COUNT * sizeof(int32_t));
        HardpointMeasuredForceFaultHigh = doc["HardpointMeasuredForceFaultHigh"].as<float>();
        HardpointMeasuredForceFaultLow = doc["HardpointMeasuredForceFaultLow"].as<float>();
        HardpointMeasuredForceFSBWarningHigh = doc["HardpointMeasuredForceFSBWarningHigh"].as<float>();
        HardpointMeasuredForceFSBWarningLow = doc["HardpointMeasuredForceFSBWarningLow"].as<float>();
        HardpointMeasuredForceWarningHigh = doc["HardpointMeasuredForceWarningHigh"].as<float>();
        HardpointMeasuredForceWarningLow = doc["HardpointMeasuredForceWarningLow"].as<float>();
        AirPressureWarningHigh = doc["AirPressureWarningHigh"].as<float>();
        AirPressureWarningLow = doc["AirPressureWarningLow"].as<float>();

    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }
}
