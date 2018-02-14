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

	this->VoltsToMetersPerSecondSqrd = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/VoltsToMetersPerSecondSqrd").node().child_value());
	this->AngularAccelerationXDistance = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/AngularAccelerationXDistance").node().child_value());
	this->AngularAccelerationYDistance = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/AngularAccelerationYDistance").node().child_value());
	this->AngularAccelerationZDistance = boost::lexical_cast<double>(doc.select_node("//AccelerometerSettings/AngularAccelerationZDistance").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
