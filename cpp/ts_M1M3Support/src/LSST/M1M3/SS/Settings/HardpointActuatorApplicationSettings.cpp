/*
 * HardpointActuatorApplicationSettings.cpp
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#include <HardpointActuatorApplicationSettings.h>
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

void HardpointActuatorApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	xml_parse_result result = doc.load_file(filename.c_str());
	xpath_node node = doc.select_node("//HardpointActuatorApplicationSettings/HardpointActuatorTablePath");

	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;

	std::string hardpointActuatorTablePath = node.node().child_value();
	std::ifstream inputStream(hardpointActuatorTablePath.c_str());
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
			sscanf((*i).c_str(), "%hhu", &Table[lineNumber - 1].Subnet);
			++i;
			sscanf((*i).c_str(), "%hhu", &Table[lineNumber - 1].Address);
			++i;
			sscanf((*i).c_str(), "%f", &Table[lineNumber - 1].SensorOffset);
			++i;
			sscanf((*i).c_str(), "%f", &Table[lineNumber - 1].SensorSensitivity);
		}
		lineNumber++;
	}
	inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
