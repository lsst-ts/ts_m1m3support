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

#include <PositionControllerSettings.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void PositionControllerSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        forceToStepsCoefficient = doc["ForceToStepsCoefficient"].as<double>();
        encoderToStepsCoefficient = doc["EncoderToStepsCoefficient"].as<double>();
        maxStepsPerLoop = doc["MaxStepsPerLoop"].as<int32_t>();

        auto raise = doc["Raise"];

        raiseHPForceLimitLow = raise["HPForceLimitLow"].as<int>();
        raiseHPForceLimitHigh = raise["HPForceLimitHigh"].as<int>();
        raiseTimeout = raise["Timeout"].as<int>();

        auto lower = doc["Lower"];

        lowerHPForceLimitLow = lower["HPForceLimitLow"].as<int>();
        lowerHPForceLimitHigh = lower["HPForceLimitHigh"].as<int>();
        lowerTimeout = lower["Timeout"].as<int>();
        lowerPositionOffsetZ = lower["PositionOffset"]["Z"].as<float>();

        auto encoders = doc["ReferencePosition"].as<std::vector<int32_t>>();
        if (encoders.size() != HP_COUNT) {
            throw std::runtime_error(fmt::format("Expecting {} encoder's ReferencePosition, got {}", HP_COUNT,
                                                 encoders.size()));
        }
        for (int i = 0; i < HP_COUNT; i++) {
            referencePosition[i] = encoders[i];
        }
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }

    log();
}
