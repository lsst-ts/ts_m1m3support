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
	this->AccelerometerOffsets[0] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer1/Offset").node().child_value());
	this->AccelerometerScalars[0] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer1/Scalar").node().child_value());
	this->AccelerometerOffsets[1] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer2/Offset").node().child_value());
	this->AccelerometerScalars[1] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer2/Scalar").node().child_value());
	this->AccelerometerOffsets[2] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer3/Offset").node().child_value());
	this->AccelerometerScalars[2] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer3/Scalar").node().child_value());
	this->AccelerometerOffsets[3] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer4/Offset").node().child_value());
	this->AccelerometerScalars[3] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer4/Scalar").node().child_value());
	this->AccelerometerOffsets[4] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer5/Offset").node().child_value());
	this->AccelerometerScalars[4] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer5/Scalar").node().child_value());
	this->AccelerometerOffsets[5] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer6/Offset").node().child_value());
	this->AccelerometerScalars[5] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer6/Scalar").node().child_value());
	this->AccelerometerOffsets[6] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer7/Offset").node().child_value());
	this->AccelerometerScalars[6] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer7/Scalar").node().child_value());
	this->AccelerometerOffsets[7] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer8/Offset").node().child_value());
	this->AccelerometerScalars[7] = boost::lexical_cast<float>(doc.select_node("//AccelerometerSettings/Accelerometer8/Scalar").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */