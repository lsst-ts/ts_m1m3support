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

#include <ForceActuatorApplicationSettings.h>
#include <XMLDocLoad.h>
#include <string>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <spdlog/spdlog.h>
#include <SettingReader.h>
#include <TableLoader.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorApplicationSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);

    this->loadForceActuatorTable(doc.select_node("//ForceActuatorApplicationSettings/ForceActuatorTablePath")
                                         .node()
                                         .child_value());
    this->XIndexToZIndex.clear();
    this->YIndexToZIndex.clear();
    this->SecondaryCylinderIndexToZIndex.clear();
    this->ZIndexToXIndex.clear();
    this->ZIndexToYIndex.clear();
    this->ZIndexToSecondaryCylinderIndex.clear();
    int xIndex = 0;
    int yIndex = 0;
    int sIndex = 0;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        ForceActuatorTypes::Type type = this->Table[zIndex].Type;
        ForceActuatorOrientations::Type orientation = this->Table[zIndex].Orientation;
        if (type == ForceActuatorTypes::DAA) {
            if (orientation == ForceActuatorOrientations::PositiveX ||
                orientation == ForceActuatorOrientations::NegativeX) {
                this->XIndexToZIndex.push_back(zIndex);
                this->ZIndexToXIndex.push_back(xIndex);
                this->ZIndexToYIndex.push_back(-1);
                xIndex += 1;
            }
            if (orientation == ForceActuatorOrientations::PositiveY ||
                orientation == ForceActuatorOrientations::NegativeY) {
                this->YIndexToZIndex.push_back(zIndex);
                this->ZIndexToXIndex.push_back(-1);
                this->ZIndexToYIndex.push_back(yIndex);
                yIndex += 1;
            }
            if (orientation != ForceActuatorOrientations::NA) {
                this->SecondaryCylinderIndexToZIndex.push_back(zIndex);
                this->ZIndexToSecondaryCylinderIndex.push_back(sIndex);
                sIndex += 1;
            }
        } else {
            this->ZIndexToXIndex.push_back(-1);
            this->ZIndexToYIndex.push_back(-1);
            this->ZIndexToSecondaryCylinderIndex.push_back(-1);
        }
    }
    spdlog::debug("ForceActuatorApplicationSettings: Index map sizes {:d} {:d} {:d} {:d} {:d} {:d}",
                  (int)this->XIndexToZIndex.size(), (int)this->YIndexToZIndex.size(),
                  (int)this->SecondaryCylinderIndexToZIndex.size(), (int)this->ZIndexToXIndex.size(),
                  (int)this->ZIndexToYIndex.size(), (int)this->ZIndexToSecondaryCylinderIndex.size());
}

void ForceActuatorApplicationSettings::loadForceActuatorTable(const std::string &filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    std::string fullname = SettingReader::get().getFilePath(filename);
    std::ifstream inputStream(fullname.c_str());
    if (!inputStream.is_open()) {
        throw std::runtime_error("Cannot read " + fullname + ": " + strerror(errno));
    }

    std::string lineText;
    int32_t lineNumber = 0;
    while (std::getline(inputStream, lineText)) {
        boost::trim_right(lineText);
        if (lineNumber != 0 && !lineText.empty()) {
            tokenizer tok(lineText);
            tokenizer::iterator i = tok.begin();
            ForceActuatorTableRow row;
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
            row.Type = (*i)[0] == 'S' ? ForceActuatorTypes::SAA : ForceActuatorTypes::DAA;
            ++i;
            row.Subnet = (uint8_t)boost::lexical_cast<int32_t>(*i);
            ++i;
            row.Address = (uint8_t)boost::lexical_cast<int32_t>(*i);
            ++i;
            std::string rawOrientation = *i;
            ForceActuatorOrientations::Type orientation = ForceActuatorOrientations::NA;
            if (rawOrientation[0] == '+' && rawOrientation[1] == 'Y')
                orientation = ForceActuatorOrientations::PositiveY;
            else if (rawOrientation[0] == '-' && rawOrientation[1] == 'Y')
                orientation = ForceActuatorOrientations::NegativeY;
            else if (rawOrientation[0] == '+' && rawOrientation[1] == 'X')
                orientation = ForceActuatorOrientations::PositiveX;
            else if (rawOrientation[0] == '-' && rawOrientation[1] == 'X')
                orientation = ForceActuatorOrientations::NegativeX;
            row.Orientation = orientation;
            this->Table.push_back(row);
        }
        lineNumber++;
    }
    inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
