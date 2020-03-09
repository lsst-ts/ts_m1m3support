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
	this->HP1EncoderOffset = boost::lexical_cast<int32_t>(doc.select_node("//HardpointActuatorSettings/HP1EncoderOffset").node().child_value());
	this->HP2EncoderOffset = boost::lexical_cast<int32_t>(doc.select_node("//HardpointActuatorSettings/HP2EncoderOffset").node().child_value());
	this->HP3EncoderOffset = boost::lexical_cast<int32_t>(doc.select_node("//HardpointActuatorSettings/HP3EncoderOffset").node().child_value());
	this->HP4EncoderOffset = boost::lexical_cast<int32_t>(doc.select_node("//HardpointActuatorSettings/HP4EncoderOffset").node().child_value());
	this->HP5EncoderOffset = boost::lexical_cast<int32_t>(doc.select_node("//HardpointActuatorSettings/HP5EncoderOffset").node().child_value());
	this->HP6EncoderOffset = boost::lexical_cast<int32_t>(doc.select_node("//HardpointActuatorSettings/HP6EncoderOffset").node().child_value());
	this->HardpointMeasuredForceFaultHigh = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFaultHigh").node().child_value());
	this->HardpointMeasuredForceFaultLow = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFaultLow").node().child_value());
	this->HardpointMeasuredForceFSBWarningHigh = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFSBWarningHigh").node().child_value());
	this->HardpointMeasuredForceFSBWarningLow = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFSBWarningLow").node().child_value());
	this->HardpointMeasuredForceWarningHigh = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceWarningHigh").node().child_value());
	this->HardpointMeasuredForceWarningLow = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceWarningLow").node().child_value());
	this->AirPressureWarningHigh = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/AirPressureWarningHigh").node().child_value());
	this->AirPressureWarningLow = boost::lexical_cast<float>(doc.select_node("//HardpointActuatorSettings/AirPressureWarningLow").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
