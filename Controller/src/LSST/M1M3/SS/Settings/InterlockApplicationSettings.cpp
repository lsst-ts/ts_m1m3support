/*
 * InterlockApplicationSettings.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: ccontaxis
 */

#include <InterlockApplicationSettings.h>
#include <SettingReader.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void InterlockApplicationSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	SettingReader::ReadXMLDocumentFromDisk(doc, filename.c_str());
	this->HeartbeatPeriodInSeconds = boost::lexical_cast<double>(doc.select_node("//InterlockApplicationSettings/HeartbeatPeriodInSeconds").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
