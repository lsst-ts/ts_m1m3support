/*
 * ForceActuatorApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ForceActuatorApplicationSettings.h>
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

void ForceActuatorApplicationSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	TableLoader::loadTable(1, 1, 1, &XIndexToZIndex, doc.select_node("//ForceActuatorApplicationSettings/XIndexToZIndexTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &YIndexToZIndex, doc.select_node("//ForceActuatorApplicationSettings/YIndexToZIndexTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &SecondaryCylinderIndexToZIndex, doc.select_node("//ForceActuatorApplicationSettings/SecondaryCylinderIndexToZIndexTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &ZIndexToXIndex, doc.select_node("//ForceActuatorApplicationSettings/ZIndexToXIndexTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &ZIndexToYIndex, doc.select_node("//ForceActuatorApplicationSettings/ZIndexToYIndexTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &ZIndexToSecondaryCylinderIndex, doc.select_node("//ForceActuatorApplicationSettings/ZIndexToSecondaryCylinderIndexTablePath").node().child_value());
	this->loadForceActuatorTable(doc.select_node("//ForceActuatorApplicationSettings/ForceActuatorTablePath").node().child_value());
}

void ForceActuatorApplicationSettings::loadForceActuatorTable(const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
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
			row.Type = (*i) == "SAA" ? ForceActuatorTypes::SAA : ForceActuatorTypes::DAA;
			++i;
			row.Subnet = (uint8_t)boost::lexical_cast<int32_t>(*i);
			++i;
			row.Address = (uint8_t)boost::lexical_cast<int32_t>(*i);
			++i;
			std::string rawOrientation = *i;
			ForceActuatorOrientations::Type orientation = ForceActuatorOrientations::NA;
			if (rawOrientation == "+Y")
				orientation = ForceActuatorOrientations::PositiveY;
			else if (rawOrientation == "-Y")
				orientation = ForceActuatorOrientations::NegativeY;
			else if (rawOrientation == "+X")
				orientation = ForceActuatorOrientations::PositiveX;
			else if (rawOrientation == "-X")
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
