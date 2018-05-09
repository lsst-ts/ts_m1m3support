/*
 * RecommendedApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <RecommendedApplicationSettings.h>
#include <pugixml.hpp>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void RecommendedApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	doc.load_file(filename.c_str());
	xpath_node_set nodes = doc.select_nodes("//RecommendedApplicationSettings/RecommendedSettings/RecommendedSetting");
	this->RecommendedSettings.clear();
	for(xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); ++node) {
			RecommendedSettings.push_back(node->node().child_value());
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
