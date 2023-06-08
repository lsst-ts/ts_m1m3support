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

#include <SafetyControllerSettings.h>

using namespace LSST::M1M3::SS;

void SafetyControllerSettings::load(YAML::Node doc) {
    try {
        SPDLOG_INFO("Loading SafetyControllerSettings");

        AirController.set(doc["AirControllerSettings"]);
        Displacement.set(doc["DisplacementSettings"]);
        Inclinometer.set(doc["InclinometerSettings"]);
        Interlock.set(doc["InterlockSettings"]);
        ForceController.set(doc["ForceControllerSettings"]);
        CellLights.set(doc["CellLightSettings"]);
        PositionController.set(doc["PositionControllerSettings"]);
        PowerController.set(doc["PowerControllerSettings"]);
        RaiseOperation.FaultOnTimeout = doc["RaiseOperationSettings"]["FaultOnTimeout"].as<bool>();
        LowerOperation.FaultOnTimeout = doc["LowerOperationSettings"]["FaultOnTimeout"].as<bool>();
        ILC.set(doc["ILCSettings"]);
        TMA.set(doc["TMASettings"]);
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading SafetyControllerSettings: {}", ex.what()));
    }
}
