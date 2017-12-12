/*
 * PositionControllerSettings.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <PositionControllerSettings.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void PositionControllerSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	this->ForceToStepsCoefficient = boost::lexical_cast<double>(doc.select_node("//PositionControllerSettings/ForceToStepsCoefficient").node().child_value());
	this->MaxStepsPerLoop = boost::lexical_cast<int32_t>(doc.select_node("//PositionControllerSettings/MaxStepsPerLoop").node().child_value());
	this->RaiseLowerForceLimitLow = boost::lexical_cast<double>(doc.select_node("//PositionControllerSettings/RaiseLowerForceLimitLow").node().child_value());
	this->RaiseLowerForceLimitHigh = boost::lexical_cast<double>(doc.select_node("//PositionControllerSettings/RaiseLowerForceLimitHigh").node().child_value());
	this->RaiseLowerTimeoutInSeconds = boost::lexical_cast<double>(doc.select_node("//PositionControllerSettings/RaiseLowerTimeoutInSeconds").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
