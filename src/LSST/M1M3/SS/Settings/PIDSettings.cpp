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

#include <PIDParameters.h>
#include <PIDSettings.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void parsePID(const YAML::Node &node, PIDParameters *params) {
    params->Timestep = node["Timestep"].as<double>();
    params->P = node["P"].as<double>();
    params->I = node["I"].as<double>();
    params->D = node["D"].as<double>();
    params->N = node["N"].as<double>();
}

void PIDSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        parsePID(doc["Fx"], &Fx);
        parsePID(doc["Fy"], &Fy);
        parsePID(doc["Fz"], &Fz);
        parsePID(doc["Mx"], &Mx);
        parsePID(doc["My"], &My);
        parsePID(doc["Mz"], &Mz);
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }
}
