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

#include <PositionControllerSettings.h>

using namespace LSST::M1M3::SS;

PositionControllerSettings::PositionControllerSettings(token) {}

void PositionControllerSettings::load(YAML::Node doc) {
    SPDLOG_INFO("Loading PositionControllerSettings");

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
    lowerPositionOffset = lower["PositionOffset"].as<float>();

    referencePosition = doc["ReferencePosition"].as<std::vector<int32_t>>();
    if (referencePosition.size() != HP_COUNT) {
        throw std::runtime_error(fmt::format("Expecting {} encoder's ReferencePosition, got {}", HP_COUNT,
                                             referencePosition.size()));
    }

    log();
}
