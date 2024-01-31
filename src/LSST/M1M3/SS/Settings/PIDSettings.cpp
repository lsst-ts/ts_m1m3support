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
#include <yaml-cpp/yaml.h>

#include <PIDSettings.h>

using namespace LSST::M1M3::SS;

PIDSettings::PIDSettings() {}

void PIDSettings::load(YAML::Node doc, const char *kind) {
    try {
        settingName = kind;
        SPDLOG_INFO("Loading {} PIDSettings", settingName);

        auto node = doc[kind];

        _parsePID(node["Fx"], 0);
        _parsePID(node["Fy"], 1);
        _parsePID(node["Fz"], 2);
        _parsePID(node["Mx"], 3);
        _parsePID(node["My"], 4);
        _parsePID(node["Mz"], 5);
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading PIDSettings: {}", ex.what()));
    }

    log();
}

PIDParameters PIDSettings::getParameters(int index) {
    return PIDParameters(timestep[index], p[index], i[index], d[index], n[index]);
}

void PIDSettings::_parsePID(const YAML::Node &node, int index) {
    timestep[index] = node["Timestep"].as<double>();
    p[index] = node["P"].as<double>();
    i[index] = node["I"].as<double>();
    d[index] = node["D"].as<double>();
    n[index] = node["N"].as<double>();
}
