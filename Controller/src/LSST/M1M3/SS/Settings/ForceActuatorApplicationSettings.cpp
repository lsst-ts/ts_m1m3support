/*
 * ForceActuatorApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ForceActuatorApplicationSettings.h>
#include <SettingReader.h>
#include <pugixml.hpp>
#include <string>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <TableLoader.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorApplicationSettings::load(const std::string& filename) {
    pugi::xml_document doc;
    SettingReader::ReadXMLDocumentFromDisk(doc, filename.c_str());
    this->loadForceActuatorTable(doc.select_node("//ForceActuatorApplicationSettings/ForceActuatorTablePath").node().child_value());
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
            if (orientation == ForceActuatorOrientations::PositiveX || orientation == ForceActuatorOrientations::NegativeX) {
                this->XIndexToZIndex.push_back(zIndex);
                this->ZIndexToXIndex.push_back(xIndex);
                this->ZIndexToYIndex.push_back(-1);
                xIndex += 1;
            }
            if (orientation == ForceActuatorOrientations::PositiveY || orientation == ForceActuatorOrientations::NegativeY) {
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
    Log.Debug("ForceActuatorApplicationSettings: Index map sizes %d %d %d %d %d %d",
              (int)this->XIndexToZIndex.size(),
              (int)this->YIndexToZIndex.size(),
              (int)this->SecondaryCylinderIndexToZIndex.size(),
              (int)this->ZIndexToXIndex.size(),
              (int)this->ZIndexToYIndex.size(),
              (int)this->ZIndexToSecondaryCylinderIndex.size());
}

void ForceActuatorApplicationSettings::loadForceActuatorTable(const std::string& filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
    std::ifstream inputStream(filename.c_str());
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
