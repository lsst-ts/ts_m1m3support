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

#ifndef INCLINOMETERSAFETYSETTINGS_H_
#define INCLINOMETERSAFETYSETTINGS_H_

#include <yaml-cpp/yaml.h>

struct InclinometerSafetySettings {
    bool FaultOnResponseTimeout;
    bool FaultOnInvalidCRC;
    bool FaultOnUnknownAddress;
    bool FaultOnUnknownFunction;
    bool FaultOnInvalidLength;
    bool FaultOnSensorReportsIllegalDataAddress;
    bool FaultOnSensorReportsIllegalFunction;
    bool FaultOnUnknownProblem;

    void set(YAML::Node node) {
        FaultOnResponseTimeout = node["FaultOnResponseTimeout"].as<bool>();
        FaultOnInvalidCRC = node["FaultOnInvalidCRC"].as<bool>();
        FaultOnUnknownAddress = node["FaultOnUnknownAddress"].as<bool>();
        FaultOnUnknownFunction = node["FaultOnUnknownFunction"].as<bool>();
        FaultOnInvalidLength = node["FaultOnInvalidLength"].as<bool>();
        FaultOnSensorReportsIllegalDataAddress = node["FaultOnSensorReportsIllegalDataAddress"].as<bool>();
        FaultOnSensorReportsIllegalFunction = node["FaultOnSensorReportsIllegalFunction"].as<bool>();
        FaultOnUnknownProblem = node["FaultOnUnknownProblem"].as<bool>();
    }
};

#endif /* INCLINOMETERSAFETYSETTINGS_H_ */
