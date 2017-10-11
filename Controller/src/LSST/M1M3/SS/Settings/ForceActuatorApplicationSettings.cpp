/*
 * ForceActuatorApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ForceActuatorApplicationSettings.h>
#include <pugixml.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <stdio.h>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	xml_parse_result result = doc.load_file(filename.c_str());
	xpath_node node = doc.select_node("//ForceActuatorApplicationSettings/ForceActuatorTablePath");

	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;

	std::string forceActuatorTablePath = node.node().child_value();
	std::ifstream inputStream(forceActuatorTablePath.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		tokenizer tok(lineText);
		if (lineNumber != 0) {
			tokenizer::iterator i = tok.begin();
			sscanf((*i).c_str(), "%d", &Table[lineNumber - 1].Index);
			++i;
			sscanf((*i).c_str(), "%d", &Table[lineNumber - 1].ActuatorID);
			++i;
			sscanf((*i).c_str(), "%lf", &Table[lineNumber - 1].XPosition);
			++i;
			sscanf((*i).c_str(), "%lf", &Table[lineNumber - 1].YPosition);
			++i;
			sscanf((*i).c_str(), "%lf", &Table[lineNumber - 1].ZPosition);
			++i;
			Table[lineNumber - 1].Type = (*i) == "SAA" ? ForceActuatorTypes::SAA : ForceActuatorTypes::DAA;
			++i;
			sscanf((*i).c_str(), "%hhu", &Table[lineNumber - 1].Subnet);
			++i;
			sscanf((*i).c_str(), "%hhu", &Table[lineNumber - 1].Address);
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
			Table[lineNumber - 1].Orientation = orientation;
			++i;
			sscanf((*i).c_str(), "%f", &Table[lineNumber - 1].PrimaryAxisSensorOffset);
			++i;
			sscanf((*i).c_str(), "%f", &Table[lineNumber - 1].PrimaryAxisSensorSensitivity);
			++i;
			sscanf((*i).c_str(), "%f", &Table[lineNumber - 1].SecondaryAxisSensorOffset);
			++i;
			sscanf((*i).c_str(), "%f", &Table[lineNumber - 1].SecondaryAxisSensorSensitivity);
		}
		lineNumber++;
	}
	inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
