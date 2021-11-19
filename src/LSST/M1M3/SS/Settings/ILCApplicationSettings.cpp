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

#include <ILCApplicationSettings.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void ILCApplicationSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);
        auto timings = doc["Timings"];

        ReportServerID = timings["ReportServerID"].as<uint32_t>();
        ReportServerStatus = timings["ReportServerStatus"].as<uint32_t>();
        ChangeILCMode = timings["ChangeILCMode"].as<uint32_t>();
        BroadcastStepMotor = timings["BroadcastStepMotor"].as<uint32_t>();
        UnicastStepMotor = timings["UnicastStepMotor"].as<uint32_t>();
        ElectromechanicalForceAndStatus = timings["ElectromechanicalForceAndStatus"].as<uint32_t>();
        BroadcastFreezeSensorValues = timings["BroadcastFreezeSensorValues"].as<uint32_t>();
        SetBoostValveDCAGains = timings["SetBoostValveDCAGains"].as<uint32_t>();
        ReadBoostValveDCAGains = timings["ReadBoostValveDCAGains"].as<uint32_t>();
        BroadcastForceDemand = timings["BroadcastForceDemand"].as<uint32_t>();
        UnicastSingleAxisForceDemand = timings["UnicastSingleAxisForceDemand"].as<uint32_t>();
        UnicastDualAxisForceDemand = timings["UnicastDualAxisForceDemand"].as<uint32_t>();
        PneumaticForceAndStatus = timings["PneumaticForceAndStatus"].as<uint32_t>();
        SetADCScanRate = timings["SetADCScanRate"].as<uint32_t>();
        SetADCChannelOffsetAndSensitivity = timings["SetADCChannelOffsetAndSensitivity"].as<uint32_t>();
        Reset = timings["Reset"].as<uint32_t>();
        ReadCalibration = timings["ReadCalibration"].as<uint32_t>();
        ReadDCAPressureValues = timings["ReadDCAPressureValues"].as<uint32_t>();
        ReportDCAID = timings["ReportDCAID"].as<uint32_t>();
        ReportDCAStatus = timings["ReportDCAStatus"].as<uint32_t>();
        ReportDCAPressure = timings["ReportDCAPressure"].as<uint32_t>();
        ReportLVDT = timings["ReportLVDT"].as<uint32_t>();
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }
}
