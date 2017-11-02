/*
 * AccelerometerSettings.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#include <AccelerometerSettings.h>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void AccelerometerSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());

	this->AngularAccelerationXCoefficient = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/AngularAccelerationXCoefficient").node().child_value());
	this->AngularAccelerationYCoefficient = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/AngularAccelerationYCoefficient").node().child_value());
	this->AngularAccelerationZCoefficient = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/AngularAccelerationZCoefficient").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
