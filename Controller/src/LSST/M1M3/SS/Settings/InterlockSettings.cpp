/*
 * InterlockSettings.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: ccontaxis
 */

#include <InterlockSettings.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void InterlockSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());

	this->HeartbeatPeriodInSeconds = boost::lexical_cast<double>(doc.select_node("//InterlockSettings/HeartbeatPeriodInSeconds").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
