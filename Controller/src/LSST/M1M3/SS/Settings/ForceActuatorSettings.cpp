/*
 * ForceActuatorSettings.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <ForceActuatorSettings.h>
#include <pugixml.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	this->loadStaticForceTable(doc.select_node("//ForceActuatorSettings/StaticForceTablePath").node().child_value());
	this->loadTable(1, 1, 6, &ElevationXAxisCoefficients, doc.select_node("//ForceActuatorSettings/ElevationXAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &ElevationYAxisCoefficients, doc.select_node("//ForceActuatorSettings/ElevationYAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &ElevationZAxisCoefficients, doc.select_node("//ForceActuatorSettings/ElevationZAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &AzimuthXAxisCoefficients, doc.select_node("//ForceActuatorSettings/AzimuthXAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &AzimuthYAxisCoefficients, doc.select_node("//ForceActuatorSettings/AzimuthYAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &AzimuthZAxisCoefficients, doc.select_node("//ForceActuatorSettings/AzimuthZAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &TemperatureXAxisCoefficients, doc.select_node("//ForceActuatorSettings/TemperatureXAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &TemperatureYAxisCoefficients, doc.select_node("//ForceActuatorSettings/TemperatureYAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &TemperatureZAxisCoefficients, doc.select_node("//ForceActuatorSettings/TemperatureZAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 22, &BendingModeMatrix, doc.select_node("//ForceActuatorSettings/BendingModeTablePath").node().child_value());
	this->loadTable(1, 1, 6, &HardpointForceMomentMatrix, doc.select_node("//ForceActuatorSettings/HardpointForceMomentTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicAccelerationXMatrix, doc.select_node("//ForceActuatorSettings/DynamicAccelerationXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicAccelerationYMatrix, doc.select_node("//ForceActuatorSettings/DynamicAccelerationYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicAccelerationZMatrix, doc.select_node("//ForceActuatorSettings/DynamicAccelerationZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityXMatrix, doc.select_node("//ForceActuatorSettings/DynamicVelocityXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityYMatrix, doc.select_node("//ForceActuatorSettings/DynamicVelocityYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityZMatrix, doc.select_node("//ForceActuatorSettings/DynamicVelocityZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityXZMatrix, doc.select_node("//ForceActuatorSettings/DynamicVelocityXZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityYZMatrix, doc.select_node("//ForceActuatorSettings/DynamicVelocityYZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &ForceDistributionXMatrix, doc.select_node("//ForceActuatorSettings/ForceDistributionXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &ForceDistributionYMatrix, doc.select_node("//ForceActuatorSettings/ForceDistributionYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &ForceDistributionZMatrix, doc.select_node("//ForceActuatorSettings/ForceDistributionZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &MomentDistributionXMatrix, doc.select_node("//ForceActuatorSettings/MomentDistributionXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &MomentDistributionYMatrix, doc.select_node("//ForceActuatorSettings/MomentDistributionYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &MomentDistributionZMatrix, doc.select_node("//ForceActuatorSettings/MomentDistributionZTablePath").node().child_value());
	this->UseInclinometer = boost::lexical_cast<int32_t>(doc.select_node("//ForceActuatorSettings/UseInclinometer").node().child_value()) != 0;
}

void ForceActuatorSettings::loadStaticForceTable(const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		if (lineNumber >= 1 && !lineText.empty()) {
			// Line Format:
			//     ActuatorID,X,Y,Z
			tokenizer tok(lineText);
			tokenizer::iterator i = tok.begin();
			++i;
			StaticXForces.push_back(boost::lexical_cast<double>(*i));
			++i;
			StaticYForces.push_back(boost::lexical_cast<double>(*i));
			++i;
			StaticZForces.push_back(boost::lexical_cast<double>(*i));
		}
		lineNumber++;
	}
	inputStream.close();
}

void ForceActuatorSettings::loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		if (lineNumber >= rowsToSkip && !lineText.empty()) {
			tokenizer tok(lineText);
			tokenizer::iterator i = tok.begin();
			for(int j = 0; j < columnsToSkip; j++) {
				++i;
			}
			for(int j = 0; j < columnsToKeep; j++) {
				data->push_back(boost::lexical_cast<double>(*i));
				++i;
			}
		}
		lineNumber++;
	}
	inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
