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

#ifndef TMASAFETYSETTINGS_H_
#define TMASAFETYSETTINGS_H_

#include <yaml-cpp/yaml.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Settings for Telescope Mount Assembly timeouts and elevation deviation from
 * inclinometer value.
 */
struct TMASafetySettings {
    /**
     * Initializes settings to -1.
     */
    TMASafetySettings() {
        AzimuthTimeout = -1;
        ElevationTimeout = -1;
        InclinometerDeviation = -1;
    }

    /**
     * Timeout for azimuth telemetry. When last received value is older than
     * timeout, CsC shall fault the mirror. <0 for no timeout.
     */
    float AzimuthTimeout;

    /**
     * Timeout for elevation telemetry. When last received value is older than
     * timeout, CsC shall fault the mirror. <0 for no timeout.
     */
    float ElevationTimeout;

    /**
     * Allowed deviation (in degrees) between inclinometer values and reported
     * elevation. When deviation is outside of the allowed range, CsC shall
     * fault the mirror. <0 for no deviation checks.
     */
    float InclinometerDeviation;

    /**
     * Sets value from YAML node.
     *
     * @param node YAML node
     */
    void set(YAML::Node node) {
        AzimuthTimeout = node["AzimuthTimeout"].as<float>();
        ElevationTimeout = node["ElevationTimeout"].as<float>();
        InclinometerDeviation = node["InclinometerDeviationDeg"].as<float>();
    }
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* TMASAFETYSETTINGS_H_ */
