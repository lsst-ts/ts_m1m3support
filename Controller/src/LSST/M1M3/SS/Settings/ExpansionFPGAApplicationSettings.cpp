/*
 * ExpansionFPGAApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ExpansionFPGAApplicationSettings.h>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ExpansionFPGAApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	doc.load_file(filename.c_str());
	this->Enabled = boost::lexical_cast<uint32_t>(doc.select_node("//ExpansionFPGAApplicationSettings/Enabled").node().child_value()) != 0;
	this->Resource = doc.select_node("//ExpansionFPGAApplicationSettings/Resource").node().child_value();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
