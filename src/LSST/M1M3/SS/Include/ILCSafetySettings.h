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

#ifndef ILCSAFETYSETTINGS_H_
#define ILCSAFETYSETTINGS_H_

#include <yaml-cpp/yaml.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ILCSafetySettings {
    bool FaultOnCommunicationTimeout;
    int CommunicationTimeoutCountThreshold;
    int CommunicationTimeoutPeriod;
    bool FaultOnForceActuatorFollowingErrorCounting;
    int ForceActuatorFollowingErrorCountThreshold;
    int ForceActuatorFollowingErrorPeriod;
    bool FaultOnForceActuatorFollowingErrorImmediate;
    bool FaultOnHardpointActuatorMeasuredForce;
    int HardpointActuatorMeasuredForceCountThreshold;
    int HardpointActuatorMeasuredForcePeriod;
    bool FaultOnAirPressure;
    int AirPressureCountThreshold;
    int AirPressurePeriod;

    void set(YAML::Node node) {
        FaultOnCommunicationTimeout = node["FaultOnCommunicationTimeout"].as<bool>();
        CommunicationTimeoutCountThreshold = node["CommunicationTimeoutCountThreshold"].as<int>();
        CommunicationTimeoutPeriod = node["CommunicationTimeoutPeriod"].as<int>();
        FaultOnForceActuatorFollowingErrorCounting =
                node["FaultOnForceActuatorFollowingErrorCounting"].as<bool>();
        ForceActuatorFollowingErrorCountThreshold =
                node["ForceActuatorFollowingErrorCountThreshold"].as<int>();
        ForceActuatorFollowingErrorPeriod = node["ForceActuatorFollowingErrorPeriod"].as<int>();
        FaultOnForceActuatorFollowingErrorImmediate =
                node["FaultOnForceActuatorFollowingErrorImmediate"].as<bool>();
        FaultOnHardpointActuatorMeasuredForce = node["FaultOnHardpointActuatorMeasuredForce"].as<bool>();
        HardpointActuatorMeasuredForceCountThreshold =
                node["HardpointActuatorMeasuredForceCountThreshold"].as<int>();
        HardpointActuatorMeasuredForcePeriod = node["HardpointActuatorMeasuredForcePeriod"].as<int>();
        FaultOnAirPressure = node["FaultOnAirPressure"].as<bool>();
        AirPressureCountThreshold = node["AirPressureCountThreshold"].as<int>();
        AirPressurePeriod = node["AirPressurePeriod"].as<int>();
    }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCSAFETYSETTINGS_H_ */
