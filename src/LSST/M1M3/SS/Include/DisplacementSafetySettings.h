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

#ifndef DISPLACEMENTSAFETYSETTINGS_H_
#define DISPLACEMENTSAFETYSETTINGS_H_

#include <yaml-cpp/yaml.h>

struct DisplacementSafetySettings {
    bool FaultOnSensorReportsInvalidCommand;
    bool FaultOnSensorReportsCommunicationTimeoutError;
    bool FaultOnSensorReportsDataLengthError;
    bool FaultOnSensorReportsNumberOfParametersError;
    bool FaultOnSensorReportsParameterError;
    bool FaultOnSensorReportsCommunicationError;
    bool FaultOnSensorReportsIDNumberError;
    bool FaultOnSensorReportsExpansionLineError;
    bool FaultOnSensorReportsWriteControlError;
    bool FaultOnResponseTimeoutError;
    bool FaultOnInvalidLength;
    bool FaultOnInvalidResponse;
    bool FaultOnUnknownCommand;
    bool FaultOnUnknownProblem;

    void set(YAML::Node node) {
        FaultOnSensorReportsInvalidCommand = node["FaultOnSensorReportsInvalidCommand"].as<bool>();
        FaultOnSensorReportsCommunicationTimeoutError =
                node["FaultOnSensorReportsCommunicationTimeoutError"].as<bool>();
        FaultOnSensorReportsDataLengthError = node["FaultOnSensorReportsDataLengthError"].as<bool>();
        FaultOnSensorReportsNumberOfParametersError =
                node["FaultOnSensorReportsNumberOfParametersError"].as<bool>();
        FaultOnSensorReportsParameterError = node["FaultOnSensorReportsParameterError"].as<bool>();
        FaultOnSensorReportsCommunicationError = node["FaultOnSensorReportsCommunicationError"].as<bool>();
        FaultOnSensorReportsIDNumberError = node["FaultOnSensorReportsIDNumberError"].as<bool>();
        FaultOnSensorReportsExpansionLineError = node["FaultOnSensorReportsExpansionLineError"].as<bool>();
        FaultOnSensorReportsWriteControlError = node["FaultOnSensorReportsWriteControlError"].as<bool>();
        FaultOnResponseTimeoutError = node["FaultOnResponseTimeoutError"].as<bool>();
        FaultOnInvalidLength = node["FaultOnInvalidLength"].as<bool>();
        FaultOnInvalidResponse = node["FaultOnInvalidResponse"].as<bool>();
        FaultOnUnknownCommand = node["FaultOnUnknownCommand"].as<bool>();
        FaultOnUnknownProblem = node["FaultOnUnknownProblem"].as<bool>();
    }
};

#endif /* DISPLACEMENTSAFETYSETTINGS_H_ */
