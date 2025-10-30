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

#ifndef LSST_M1M3_SS_INCLUDE_FORCECOMPONENTSETTINGS_H_
#define LSST_M1M3_SS_INCLUDE_FORCECOMPONENTSETTINGS_H_

#include <spdlog/fmt/fmt.h>
#include <yaml-cpp/yaml.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForceComponentSettings {
    float MaxRateOfChange;
    float NearZeroValue;

    ForceComponentSettings() {
        MaxRateOfChange = NAN;
        NearZeroValue = NAN;
    }

    void set(YAML::Node parent, const char* key, float limit_max_rate_of_change = 0) {
        auto node = parent[key];
        MaxRateOfChange = node["MaxRateOfChange"].as<float>();
        if (MaxRateOfChange <= 0) {
            throw std::runtime_error(fmt::format(
                    "{} MaxRateOfChange must be positive, non-zero number. Is {}.", key, MaxRateOfChange));
        }
        if (limit_max_rate_of_change > 0) {
            if (MaxRateOfChange > limit_max_rate_of_change) {
                auto msg = fmt::format("Too large MaxRateOfChange in {}: {}", key, MaxRateOfChange);
                MaxRateOfChange = 0;
                throw std::runtime_error(msg);
            }
        }
        NearZeroValue = node["NearZeroValue"].as<float>();
        if (NearZeroValue <= 0) {
            throw std::runtime_error(
                    fmt::format("{} NearZeroValue must be positive number. Is {}", key, NearZeroValue));
        }
    }
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* LSST_M1M3_SS_INCLUDE_FORCECOMPONENTSETTINGS_H_ */
