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

#include <HardpointActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <TableLoader.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

HardpointActuatorSettings::HardpointActuatorSettings() {
    memset(encoderOffset, 0, sizeof(encoderOffset));
    memset(lowProximityEncoder, 0, sizeof(lowProximityEncoder));
    memset(highProximityEncoder, 0, sizeof(highProximityEncoder));
}

void HardpointActuatorSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        TableLoader::loadTable(1, 6, &HardpointDisplacementToMirrorPosition,
                               doc["HardpointDisplacementToMirrorPositionTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &MirrorPositionToHardpointDisplacement,
                               doc["MirrorPositionToHardpointDisplacementTablePath"].as<std::string>());
        micrometersPerStep = doc["MicrometersPerStep"].as<double>();
        micrometersPerEncoder = doc["MicrometersPerEncoder"].as<double>();

        auto _hpIntSettings = [doc](int32_t *data, const char *field) {
            std::vector<int32_t> dataVec = doc[field].as<std::vector<int32_t>>();
            if (dataVec.size() != HP_COUNT) {
                throw std::runtime_error(fmt::format(
                        "Invalid {} field in HardpointActuatorSettings, expected {}, found {} integers",
                        field, HP_COUNT, dataVec.size()));
            }
            for (int i = 0; i < HP_COUNT; i++) {
                data[i] = dataVec[i];
            }
        };

        _hpIntSettings(encoderOffset, "HPEncoderOffsets");
        _hpIntSettings(lowProximityEncoder, "LowProximity");
        _hpIntSettings(highProximityEncoder, "HighProximity");

        hardpointMeasuredForceFaultHigh = doc["HardpointMeasuredForceFaultHigh"].as<float>();
        hardpointMeasuredForceFaultLow = doc["HardpointMeasuredForceFaultLow"].as<float>();
        hardpointMeasuredForceFSBWarningHigh = doc["HardpointMeasuredForceFSBWarningHigh"].as<float>();
        hardpointMeasuredForceFSBWarningLow = doc["HardpointMeasuredForceFSBWarningLow"].as<float>();
        hardpointMeasuredForceWarningHigh = doc["HardpointMeasuredForceWarningHigh"].as<float>();
        hardpointMeasuredForceWarningLow = doc["HardpointMeasuredForceWarningLow"].as<float>();

        if (hardpointMeasuredForceFaultHigh <= hardpointMeasuredForceFaultLow) {
            throw std::runtime_error(
                    fmt::format("{} HardpointMeasuredForceFaultHigh ({:.2f}) is lower or equal "
                                "HardpointMeasuredForceFaultLow ({:.2f})",
                                filename, hardpointMeasuredForceFaultHigh, hardpointMeasuredForceFaultLow));
        }
        if (hardpointMeasuredForceWarningHigh <= hardpointMeasuredForceWarningLow) {
            throw std::runtime_error(fmt::format(
                    "{} HardpointMeasuredForceWarningHigh ({:.2f}) is lower or equal "
                    "HardpointMeasuredForceWarningLow ({:.2f})",
                    filename, hardpointMeasuredForceWarningHigh, hardpointMeasuredForceWarningLow));
        }
        if (hardpointMeasuredForceFSBWarningHigh <= hardpointMeasuredForceFSBWarningLow) {
            throw std::runtime_error(fmt::format(
                    "{} HardpointMeasuredForceFSBWarningHigh ({:.2f}) is lower or equal "
                    "HardpointMeasuredForceFSBWarningLow ({:.2f})",
                    filename, hardpointMeasuredForceFSBWarningHigh, hardpointMeasuredForceFSBWarningLow));
        }

        if (hardpointMeasuredForceFaultHigh <= hardpointMeasuredForceWarningHigh) {
            throw std::runtime_error(fmt::format(
                    "{} HardpointMeasuredForceFaultHigh ({:.2f}) is lower or equal "
                    "HardpointMeasuredForceWarningHigh ({:.2f})",
                    filename, hardpointMeasuredForceFaultHigh, hardpointMeasuredForceWarningHigh));
        }
        if (hardpointMeasuredForceFaultLow >= hardpointMeasuredForceWarningLow) {
            throw std::runtime_error(
                    fmt::format("{} HardpointMeasuredForceFaultLow ({:.2f}) is lower or equal "
                                "HardpointMeasuredForceWarningLow ({:.2f})",
                                filename, hardpointMeasuredForceFaultLow, hardpointMeasuredForceWarningLow));
        }

        if (hardpointMeasuredForceFaultHigh <= hardpointMeasuredForceFSBWarningHigh) {
            throw std::runtime_error(fmt::format(
                    "{} HardpointMeasuredForceFaultHigh ({:.2f}) is lower or equal "
                    "HardpointMeasuredForceFSBWarningHigh ({:.2f})",
                    filename, hardpointMeasuredForceFaultHigh, hardpointMeasuredForceFSBWarningHigh));
        }
        if (hardpointMeasuredForceFaultLow >= hardpointMeasuredForceFSBWarningLow) {
            throw std::runtime_error(fmt::format(
                    "{} HardpointMeasuredForceFaultLow ({:.2f}) is lower or equal "
                    "HardpointMeasuredForceFSBWarningLow ({:.2f})",
                    filename, hardpointMeasuredForceFaultLow, hardpointMeasuredForceFSBWarningLow));
        }

        airPressureFaultHigh = doc["AirPressureFaultHigh"].as<float>();
        airPressureFaultLow = doc["AirPressureFaultLow"].as<float>();
        airPressureFaultLowRaising = doc["AirPressureFaultLowRaising"].as<float>();

        if (airPressureFaultHigh <= airPressureFaultLow) {
            throw std::runtime_error(fmt::format(
                    "{} AirPressureFaultHigh ({:.2f}) is lower or equal AirPressureFaultLow ({:.2f})",
                    filename, airPressureFaultHigh, airPressureFaultLow));
        }
        if (airPressureFaultHigh <= airPressureFaultLowRaising) {
            throw std::runtime_error(fmt::format(
                    "{} AirPressureFaultHigh ({:.2f}) is lower or equal AirPressureFaultLowRaising ({:.2f})",
                    filename, airPressureFaultHigh, airPressureFaultLowRaising));
        }
        for (int i = 0; i < HP_COUNT; i++) {
            if (lowProximityEncoder[i] >= highProximityEncoder[i]) {
                throw std::runtime_error(
                        fmt::format("HardpointActuatorSettings LowProximity isn't smaller than HighProximity "
                                    "for hardpoint {}",
                                    i + 1));
            }
        }
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }

    log();
}

void HardpointActuatorSettings::log() { M1M3SSPublisher::instance().logHardpointActuatorSettings(this); }
