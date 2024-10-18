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

#include <ILCApplicationSettings.h>

using namespace LSST::M1M3::SS;

ILCApplicationSettings::ILCApplicationSettings(token) {}

void ILCApplicationSettings::load(YAML::Node doc) {
    try {
        SPDLOG_INFO("Loading ILCApplicationSettings");

        FPGAConfigTimeout = doc["FPGAConfigTimeout"].as<uint32_t>();
        if (FPGAConfigTimeout > 1000) {
            throw std::runtime_error(fmt::format("FPGAConfigTimeout shall be lower than 1009 ms, is: {} ms",
                                                 FPGAConfigTimeout));
        }
        FPGARealtimeLoopTimeout = doc["FPGARealtimeLoopTimeout"].as<uint32_t>();
        if (FPGARealtimeLoopTimeout > 100) {
            throw std::runtime_error(
                    fmt::format("FPGARealtimeLoopTimeout shall be lower than 100 ms, is: {} ms",
                                FPGARealtimeLoopTimeout));
        }

        ReportServerID = doc["ReportServerID"].as<uint32_t>();
        ReportServerStatus = doc["ReportServerStatus"].as<uint32_t>();
        ChangeILCMode = doc["ChangeILCMode"].as<uint32_t>();
        BroadcastStepMotor = doc["BroadcastStepMotor"].as<uint32_t>();
        UnicastStepMotor = doc["UnicastStepMotor"].as<uint32_t>();
        ElectromechanicalForceAndStatus = doc["ElectromechanicalForceAndStatus"].as<uint32_t>();
        BroadcastFreezeSensorValues = doc["BroadcastFreezeSensorValues"].as<uint32_t>();
        SetBoostValveDCAGains = doc["SetBoostValveDCAGains"].as<uint32_t>();
        ReadBoostValveDCAGains = doc["ReadBoostValveDCAGains"].as<uint32_t>();
        BroadcastForceDemand = doc["BroadcastForceDemand"].as<uint32_t>();
        UnicastSingleAxisForceDemand = doc["UnicastSingleAxisForceDemand"].as<uint32_t>();
        UnicastDualAxisForceDemand = doc["UnicastDualAxisForceDemand"].as<uint32_t>();
        PneumaticForceAndStatus = doc["PneumaticForceAndStatus"].as<uint32_t>();
        SetADCScanRate = doc["SetADCScanRate"].as<uint32_t>();
        SetADCChannelOffsetAndSensitivity = doc["SetADCChannelOffsetAndSensitivity"].as<uint32_t>();
        Reset = doc["Reset"].as<uint32_t>();
        ReadCalibration = doc["ReadCalibration"].as<uint32_t>();
        ReadDCAPressureValues = doc["ReadDCAPressureValues"].as<uint32_t>();
        ReportDCAID = doc["ReportDCAID"].as<uint32_t>();
        ReportDCAStatus = doc["ReportDCAStatus"].as<uint32_t>();
        ReportDCAPressure = doc["ReportDCAPressure"].as<uint32_t>();
        ReportLVDT = doc["ReportLVDT"].as<uint32_t>();
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading ILCApplicationSettings: {}", ex.what()));
    }
}
