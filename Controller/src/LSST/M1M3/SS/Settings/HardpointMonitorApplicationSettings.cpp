/*
 * HardpointMonitorApplicationSettings.cpp
 *
 *  Created on: Nov 17, 2017
 *      Author: ccontaxis
 */

#include <HardpointMonitorApplicationSettings.h>
#include <SettingReader.h>
#include <pugixml.hpp>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void HardpointMonitorApplicationSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	SettingReader::ReadXMLDocumentFromDisk(doc, filename.c_str());
	pugi::xpath_node node = doc.select_node("//HardpointMonitorApplicationSettings/HardpointMonitorTablePath");

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
			HardpointMonitorTableRow row;
			row.Index = boost::lexical_cast<int32_t>(*i);
			++i;
			row.ActuatorID = boost::lexical_cast<int32_t>(*i);
			++i;
			row.Subnet = (uint8_t)boost::lexical_cast<int32_t>(*i);
			++i;
			row.Address = (uint8_t)boost::lexical_cast<int32_t>(*i);
			this->Table.push_back(row);
		}
		lineNumber++;
	}
	inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
