/*
 * GyroSettings.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#include <GyroSettings.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <pugixml.hpp>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void GyroSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result load_file_xml_parse_result = doc.load_file(filename.c_str());
	if (!load_file_xml_parse_result) {
		Log.Fatal("Settings file %s could not be loaded", filename.c_str());
		Log.Fatal("Error description: %s", load_file_xml_parse_result.description());
	}
	this->DataRate = boost::lexical_cast<int>(doc.select_node("//GyroSettings/DataRate").node().child_value());
	this->AngularVelocityXOffset = boost::lexical_cast<float>(doc.select_node("//GyroSettings/AngularVelocityXOffset").node().child_value());
	this->AngularVelocityYOffset = boost::lexical_cast<float>(doc.select_node("//GyroSettings/AngularVelocityYOffset").node().child_value());
	this->AngularVelocityZOffset = boost::lexical_cast<float>(doc.select_node("//GyroSettings/AngularVelocityZOffset").node().child_value());
	std::string axesMatrix = doc.select_node("//GyroSettings/AxesMatrix").node().child_value();
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	tokenizer tokenize(axesMatrix);
	this->AxesMatrix.clear();
	for(tokenizer::iterator token = tokenize.begin(); token != tokenize.end(); ++token) {
		this->AxesMatrix.push_back(boost::lexical_cast<double>(*token));
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
