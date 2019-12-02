/*
 * InterlockApplicationSettings.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: ccontaxis
 */

#include <InterlockApplicationSettings.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void InterlockApplicationSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result load_file_xml_parse_result = doc.load_file(filename.c_str());
	if (!load_file_xml_parse_result) {
		Log.Fatal("Settings file %s could not be loaded", filename.c_str());
		Log.Fatal("Error description: %s", load_file_xml_parse_result.description());
	}

	this->HeartbeatPeriodInSeconds = boost::lexical_cast<double>(doc.select_node("//InterlockApplicationSettings/HeartbeatPeriodInSeconds").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
