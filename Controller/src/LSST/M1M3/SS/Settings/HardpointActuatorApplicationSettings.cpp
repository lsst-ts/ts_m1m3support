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
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void HardpointActuatorApplicationSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	pugi::xpath_node node = doc.select_node("//HardpointActuatorApplicationSettings/HardpointActuatorTablePath");

	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;

	std::string hardpointActuatorTablePath = node.node().child_value();
	std::ifstream inputStream(hardpointActuatorTablePath.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	this->Table.clear();
	while(std::getline(inputStream, lineText)) {
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
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
