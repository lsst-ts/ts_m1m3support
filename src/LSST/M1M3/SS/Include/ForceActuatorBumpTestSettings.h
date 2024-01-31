/*
 * Settings for force actuator bump test.
 *
 * Developed for the LSST Telescope and Site Systems.
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

#ifndef LSST_M1M3_SS_INCLUDE_FORCEACTUATORBUMPTESTSETTINGS_H
#define LSST_M1M3_SS_INCLUDE_FORCEACTUATORBUMPTESTSETTINGS_H

#include <yaml-cpp/yaml.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Store settings for force actuator bump test.
 */
struct ForceActuatorBumpTestSettings {
    /**
     * Warning tolerance. If crossed and below error tolerance, warning log entry
     * will be produced. Test shall not pass.
     */
    float warning;
    /**
     * Error tolerance. If crossed, test shall fail and error log entry with
     * explanation will be produced.
     */
    float error;

    void set(YAML::Node node) {
        warning = node["Warning"].as<float>();
        if (warning < 0)
            throw std::runtime_error("Invalid Force Actuator Bump Test Warning value (" + node.Tag() +
                                     "/Warning: " + std::to_string(warning));

        error = node["Error"].as<float>();
        if (error < 0)
            throw std::runtime_error("Invalid Force Actuator Bump Test Error value (" + node.Tag() +
                                     "/Error: " + std::to_string(warning));
    }
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* LSST_M1M3_SS_INCLUDE_FORCEACTUATORBUMPTESTSETTINGS_H */
