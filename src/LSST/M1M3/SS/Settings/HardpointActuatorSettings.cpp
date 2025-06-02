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

#include "HardpointActuatorSettings.h"
#include "M1M3SSPublisher.h"
#include "TableLoader.h"

using namespace LSST::M1M3::SS;

HardpointActuatorSettings::HardpointActuatorSettings(token) {}

void HardpointActuatorSettings::load(YAML::Node doc) {
    SPDLOG_INFO("Loading HardpointActuatorSettings");

    TableLoader::loadTable(1, 6, &HardpointDisplacementToMirrorPosition,
                           doc["HardpointDisplacementToMirrorPositionTablePath"].as<std::string>());
    TableLoader::loadTable(1, 6, &MirrorPositionToHardpointDisplacement,
                           doc["MirrorPositionToHardpointDisplacementTablePath"].as<std::string>());
    micrometersPerStep = doc["MicrometersPerStep"].as<double>();
    micrometersPerEncoder = doc["MicrometersPerEncoder"].as<double>();

    auto _hpIntSettings = [doc](std::vector<int> &dataVec, const char *field) {
        dataVec = doc[field].as<std::vector<int>>();
        if (dataVec.size() != HP_COUNT) {
            throw std::runtime_error(
                    fmt::format("Invalid {} field in HardpointActuatorSettings, "
                                "expected {}, found {} integers",
                                field, HP_COUNT, dataVec.size()));
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

    hardpointBreakawayFaultHigh = doc["HardpointBreakawayFaultHigh"].as<float>();
    hardpointBreakawayFaultLow = doc["HardpointBreakawayFaultLow"].as<float>();
    ignoreTensionRaisingLoweringElevation = doc["IgnoreTensionRaisingLoweringElevation"].as<float>();
    ignoreCompressionRaisingLoweringElevation = doc["IgnoreCompressionRaisingLoweringElevation"].as<float>();
    inRangeReadoutsToChaseFromWaitingTension = doc["InRangeReadoutsToChaseFromWaitingTension"].as<int>();
    inRangeReadoutsToChaseFromWaitingCompression =
            doc["InRangeReadoutsToChaseFromWaitingCompression"].as<int>();

    if (hardpointMeasuredForceFaultHigh <= hardpointMeasuredForceFaultLow) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceFaultHigh ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceFaultLow ({:.2f})",
                            hardpointMeasuredForceFaultHigh, hardpointMeasuredForceFaultLow));
    }
    if (hardpointMeasuredForceWarningHigh <= hardpointMeasuredForceWarningLow) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceWarningHigh ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceWarningLow ({:.2f})",
                            hardpointMeasuredForceWarningHigh, hardpointMeasuredForceWarningLow));
    }
    if (hardpointMeasuredForceFSBWarningHigh <= hardpointMeasuredForceFSBWarningLow) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceFSBWarningHigh ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceFSBWarningLow ({:.2f})",
                            hardpointMeasuredForceFSBWarningHigh, hardpointMeasuredForceFSBWarningLow));
    }

    if (hardpointMeasuredForceFaultHigh <= hardpointMeasuredForceWarningHigh) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceFaultHigh ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceWarningHigh ({:.2f})",
                            hardpointMeasuredForceFaultHigh, hardpointMeasuredForceWarningHigh));
    }
    if (hardpointMeasuredForceFaultLow >= hardpointMeasuredForceWarningLow) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceFaultLow ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceWarningLow ({:.2f})",
                            hardpointMeasuredForceFaultLow, hardpointMeasuredForceWarningLow));
    }

    if (hardpointMeasuredForceFaultHigh <= hardpointMeasuredForceFSBWarningHigh) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceFaultHigh ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceFSBWarningHigh ({:.2f})",
                            hardpointMeasuredForceFaultHigh, hardpointMeasuredForceFSBWarningHigh));
    }
    if (hardpointMeasuredForceFaultLow >= hardpointMeasuredForceFSBWarningLow) {
        throw std::runtime_error(
                fmt::format("HardpointMeasuredForceFaultLow ({:.2f}) is lower or equal "
                            "HardpointMeasuredForceFSBWarningLow ({:.2f})",
                            hardpointMeasuredForceFaultLow, hardpointMeasuredForceFSBWarningLow));
    }

    if (hardpointBreakawayFaultLow >= hardpointBreakawayFaultHigh) {
        throw std::runtime_error(
                fmt::format("HardpointBreakawayFaultLow ({:.2f}) is lower or equal "
                            "HardpointBreakawayFaultHigh ({:.2f})",
                            hardpointBreakawayFaultLow, hardpointBreakawayFaultHigh));
    }

    inRangeAfterRaiseLoops = doc["InRangeAfterRaiseForce"].as<int>();
    inRangeAfterRaiseTimeoutLoops =
    	doc["InRangeAfterRaiseTimeoutLoops"].as<int>(5 * inRangeAfterRaiseLoops);
    inRangeAfterRaiseForce = doc["InRangeAfterRaiseForce"].as<float>();
    
    if (inRangeAfterRaiseLoops < 0) {
        throw std::runtime_error(fmt::format("InRangeAfterRaiseLoops must be positive number - is {}",
    					 inRangeAfterRaiseLoops));
    }
    if (inRangeAfterRaiseTimeoutLoops < inRangeAfterRaiseLoops) {
        throw std::runtime_error(fmt::format(
    	    "InRangeAfterRaiseTimeoutLoops must be greater than InRangeAfterRaiseLoops, but {} < {}",
    	    inRangeAfterRaiseTimeoutLoops, inRangeAfterRaiseLoops));
    }
    if (inRangeAfterRaiseForce < 0 || inRangeAfterRaiseForce > 200) {
        throw std::runtime_error(
    	    fmt::format("InRangeAfterRaiseForce must be positive and less than 200, is {}",
    			inRangeAfterRaiseForce));
    }

    if (inRangeReadoutsToChaseFromWaitingTension <= 0) {
        throw std::runtime_error(
                fmt::format("InRangeReadoutsToChaseFromWaitingTension must be "
                            "positive number, is {}",
                            inRangeReadoutsToChaseFromWaitingTension));
    }

    airPressureFaultHigh = doc["AirPressureFaultHigh"].as<float>();
    airPressureFaultLow = doc["AirPressureFaultLow"].as<float>();
    airPressureFaultLowRaising = doc["AirPressureFaultLowRaising"].as<float>();

    if (airPressureFaultHigh <= airPressureFaultLow) {
        throw std::runtime_error(
                fmt::format("AirPressureFaultHigh ({:.2f}) is lower or equal "
                            "AirPressureFaultLow ({:.2f})",
                            airPressureFaultHigh, airPressureFaultLow));
    }
    if (airPressureFaultHigh <= airPressureFaultLowRaising) {
        throw std::runtime_error(
                fmt::format("AirPressureFaultHigh ({:.2f}) is lower or equal "
                            "AirPressureFaultLowRaising ({:.2f})",
                            airPressureFaultHigh, airPressureFaultLowRaising));
    }
    for (int i = 0; i < HP_COUNT; i++) {
        if (lowProximityEncoder[i] >= highProximityEncoder[i]) {
            throw std::runtime_error(
                    fmt::format("HardpointActuatorSettings LowProximity ({}) isn't smaller "
                                "than HighProximity ({}) "
                                "for hardpoint {}",
                                lowProximityEncoder[i], highProximityEncoder[i], i + 1));
        }
    }

    log();
}

void HardpointActuatorSettings::log() { M1M3SSPublisher::instance().logHardpointActuatorSettings(this); }
