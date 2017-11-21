/*
 * HardpointActuatorSettings.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <HardpointActuatorSettings.h>
#include <TableLoader.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void HardpointActuatorSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	TableLoader::loadTable(1, 1, 6, &this->HardpointDisplacementToMirrorPosition, doc.select_node("//HardpointActuatorSettings/HardpointDisplacementToMirrorPositionTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &this->MirrorPositionToHardpointDisplacement, doc.select_node("//HardpointActuatorSettings/MirrorPositionToHardpointDisplacementTablePath").node().child_value());
	this->MicrometersPerStep = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/MicrometersPerStep").node().child_value());
	this->MicrometersPerEncoder = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/MicrometersPerEncoder").node().child_value());
	this->DisplacementOffset1 = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/DisplacementOffset1").node().child_value());
	this->DisplacementOffset2 = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/DisplacementOffset2").node().child_value());
	this->DisplacementOffset3 = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/DisplacementOffset3").node().child_value());
	this->DisplacementOffset4 = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/DisplacementOffset4").node().child_value());
	this->DisplacementOffset5 = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/DisplacementOffset5").node().child_value());
	this->DisplacementOffset6 = boost::lexical_cast<double>(doc.select_node("//HardpointActuatorSettings/DisplacementOffset6").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
