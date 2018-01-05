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

namespace LSST {
namespace M1M3 {
namespace SS {

void GyroSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	this->DataRate = boost::lexical_cast<int>(doc.select_node("//GyroSettings/DataRate").node().child_value());
	std::string axesMatrix = doc.select_node("//GyroSettings/AxesMatrix").node().child_value();
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	tokenizer tokenize(axesMatrix);
	for(tokenizer::iterator token = tokenize.begin(); token != tokenize.end(); ++token) {
		this->AxesMatrix.push_back(boost::lexical_cast<double>(*token));
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
