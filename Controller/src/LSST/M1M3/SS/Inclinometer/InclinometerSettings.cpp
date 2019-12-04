/*
 * InclinometerSettings.cpp
 *
 *  Created on: May 11, 2018
 *      Author: ccontaxis
 */

#include <InclinometerSettings.h>
#include <SettingReader.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void InclinometerSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	SettingReader::ReadXMLDocumentFromDisk(doc, filename.c_str());
	this->Offset = boost::lexical_cast<float>(doc.select_node("//InclinometerSettings/Offset").node().child_value());
	this->FaultLow = boost::lexical_cast<float>(doc.select_node("//InclinometerSettings/Limits/FaultLow").node().child_value());
	this->WarningLow = boost::lexical_cast<float>(doc.select_node("//InclinometerSettings/Limits/WarningLow").node().child_value());
	this->WarningHigh = boost::lexical_cast<float>(doc.select_node("//InclinometerSettings/Limits/WarningHigh").node().child_value());
	this->FaultHigh = boost::lexical_cast<float>(doc.select_node("//InclinometerSettings/Limits/FaultHigh").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
