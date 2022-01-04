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

#include <HardpointActuatorApplicationSettings.h>
#include <SettingReader.h>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

void HardpointActuatorApplicationSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;

        std::string hardpointActuatorTablePath =
                SettingReader::instance().getFilePath(doc["HardpointActuatorTablePath"].as<std::string>());
        std::ifstream inputStream(hardpointActuatorTablePath.c_str());
        if (!inputStream.is_open()) {
            throw std::runtime_error("Cannot read " + hardpointActuatorTablePath + ": " + strerror(errno));
        }

        std::string lineText;
        int32_t lineNumber = 0;
        this->Table.clear();
        while (std::getline(inputStream, lineText)) {
            boost::trim_right(lineText);
            if (lineNumber != 0) {
                tokenizer tok(lineText);
                tokenizer::iterator i = tok.begin();
                HardpointActuatorTableRow row;
                row.Index = boost::lexical_cast<int32_t>(*i);
                ++i;
                row.ActuatorID = boost::lexical_cast<int32_t>(*i);
                ++i;
                row.XPosition = boost::lexical_cast<double>(*i);
                ++i;
                row.YPosition = boost::lexical_cast<double>(*i);
                ++i;
                row.ZPosition = boost::lexical_cast<double>(*i);
                ++i;
                row.Subnet = (uint8_t)boost::lexical_cast<int32_t>(*i);
                ++i;
                row.Address = (uint8_t)boost::lexical_cast<int32_t>(*i);
                ++i;
                row.SensorOffset = boost::lexical_cast<float>(*i);
                ++i;
                row.SensorSensitivity = boost::lexical_cast<float>(*i);
                this->Table.push_back(row);
            }
            lineNumber++;
        }
        inputStream.close();
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }
}
