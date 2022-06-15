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

#include <AliasApplicationSettings.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void AliasApplicationSettings::load(const std::string &filename) {
    Aliases.clear();
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        for (auto it = doc.begin(); it != doc.end(); ++it) {
            Alias alias;
            alias.Name = it->first.as<std::string>();
            alias.Set = it->second["Set"].as<std::string>();
            alias.Version = it->second["Version"].as<std::string>();
            Aliases.push_back(alias);
            SPDLOG_DEBUG("Alias {}->{}:{}", alias.Name, alias.Set, alias.Version);
        }
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }
}
