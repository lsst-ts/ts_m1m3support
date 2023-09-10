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

#ifndef INTERLOCKCONTROLLERSAFETYSETTINGS_H_
#define INTERLOCKCONTROLLERSAFETYSETTINGS_H_

#include <yaml-cpp/yaml.h>

struct InterlockControllerSafetySettings {
    bool FaultOnHeartbeatStateOutputMismatch;
    bool FaultOnAuxPowerNetworksOff;
    bool FaultOnThermalEquipmentOff;
    bool FaultOnAirSupplyOff;
    bool FaultOnCabinetDoorOpen;
    bool FaultOnTMAMotionStop;
    bool FaultOnGISFault;

    void set(YAML::Node node) {
        FaultOnHeartbeatStateOutputMismatch = node["FaultOnHeartbeatStateOutputMismatch"].as<bool>();
        FaultOnAuxPowerNetworksOff = node["FaultOnAuxPowerNetworksOff"].as<bool>();
        FaultOnThermalEquipmentOff = node["FaultOnThermalEquipmentOff"].as<bool>();
        FaultOnAirSupplyOff = node["FaultOnAirSupplyOff"].as<bool>();
        FaultOnCabinetDoorOpen = node["FaultOnCabinetDoorOpen"].as<bool>();
        FaultOnTMAMotionStop = node["FaultOnTMAMotionStop"].as<bool>();
        FaultOnGISFault = node["FaultOnGISFault"].as<bool>();
    }
};

#endif /* INTERLOCKCONTROLLERSAFETYSETTINGS_H_ */
