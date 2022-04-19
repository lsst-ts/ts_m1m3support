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

#ifndef FORCECONTROLLERSAFETYSETTINGS_H_
#define FORCECONTROLLERSAFETYSETTINGS_H_

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

/**
 * Force Controller Safety settings - which force checks should be run.
 */
class ForceControllerSafetySettings {
public:
    bool FaultOnSafetyLimit;
    bool FaultOnXMomentLimit;
    bool FaultOnYMomentLimit;
    bool FaultOnZMomentLimit;
    bool FaultOnNearNeighborCheck;
    bool FaultOnMagnitudeLimit;
    bool FaultOnFarNeighborCheck;
    bool FaultOnElevationForceClipping;
    bool FaultOnAzimuthForceClipping;
    bool FaultOnThermalForceClipping;
    bool FaultOnBalanceForceClipping;
    bool FaultOnAccelerationForceClipping;
    bool FaultOnActiveOpticNetForceCheck;
    bool FaultOnActiveOpticForceClipping;
    bool FaultOnStaticForceClipping;
    bool FaultOnAberrationNetForceCheck;
    bool FaultOnAberrationForceClipping;
    bool FaultOnOffsetForceClipping;
    bool FaultOnVelocityForceClipping;
    bool FaultOnForceClipping;

    void set(YAML::Node node) {
        FaultOnSafetyLimit = node["FaultOnSafetyLimit"].as<bool>();
        FaultOnXMomentLimit = node["FaultOnXMomentLimit"].as<bool>();
        FaultOnYMomentLimit = node["FaultOnYMomentLimit"].as<bool>();
        FaultOnZMomentLimit = node["FaultOnZMomentLimit"].as<bool>();
        FaultOnNearNeighborCheck = node["FaultOnNearNeighborCheck"].as<bool>();
        FaultOnMagnitudeLimit = node["FaultOnMagnitudeLimit"].as<bool>();
        configured_FaultOnFarNeighborCheck = FaultOnFarNeighborCheck =
                node["FaultOnFarNeighborCheck"].as<bool>();
        FaultOnElevationForceClipping = node["FaultOnElevationForceClipping"].as<bool>();
        FaultOnAzimuthForceClipping = node["FaultOnAzimuthForceClipping"].as<bool>();
        FaultOnThermalForceClipping = node["FaultOnThermalForceClipping"].as<bool>();
        FaultOnBalanceForceClipping = node["FaultOnBalanceForceClipping"].as<bool>();
        FaultOnAccelerationForceClipping = node["FaultOnAccelerationForceClipping"].as<bool>();
        FaultOnActiveOpticNetForceCheck = node["FaultOnActiveOpticNetForceCheck"].as<bool>();
        FaultOnActiveOpticForceClipping = node["FaultOnActiveOpticForceClipping"].as<bool>();
        FaultOnStaticForceClipping = node["FaultOnStaticForceClipping"].as<bool>();
        FaultOnAberrationNetForceCheck = node["FaultOnAberrationNetForceCheck"].as<bool>();
        FaultOnAberrationForceClipping = node["FaultOnAberrationForceClipping"].as<bool>();
        FaultOnOffsetForceClipping = node["FaultOnOffsetForceClipping"].as<bool>();
        FaultOnVelocityForceClipping = node["FaultOnVelocityForceClipping"].as<bool>();
        FaultOnForceClipping = node["FaultOnForceClipping"].as<bool>();
    }

    /**
     * Sets safety settings for FA bump testing.
     */
    void enterBumpTesting() {
        SPDLOG_TRACE("ForceControllerSafetySettings: enterBumpTesting()");
        FaultOnFarNeighborCheck = false;
    }

    /**
     * Retores settings for nominal, non-FA bump testing operation.
     */
    void exitBumpTesting() {
        SPDLOG_TRACE("ForceControllerSafetySettings: exitBumpTesting()");
        FaultOnFarNeighborCheck = configured_FaultOnFarNeighborCheck;
    }

private:
    bool configured_FaultOnFarNeighborCheck;
};

#endif /* FORCECONTROLLERSAFETYSETTINGS_H_ */
