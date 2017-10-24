/*
 * ForceActuatorApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ForceActuatorApplicationSettings.h>
#include <pugixml.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>

using namespace std;
using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	xml_parse_result result = doc.load_file(filename.c_str());
	this->loadForceActuatorTable(doc.select_node("//ForceActuatorApplicationSettings/ForceActuatorTablePath").node().child_value());
	this->loadStaticForceTable(doc.select_node("//ForceActuatorApplicationSettings/StaticForceTablePath").node().child_value());
	this->loadTable(1, 1, 6, &ElevationXAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/ElevationXAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &ElevationYAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/ElevationYAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &ElevationZAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/ElevationZAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &AzimuthXAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/AzimuthXAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &AzimuthYAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/AzimuthYAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &AzimuthZAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/AzimuthZAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &TemperatureXAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/TemperatureXAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &TemperatureYAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/TemperatureYAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 6, &TemperatureZAxisCoefficients, doc.select_node("//ForceActuatorApplicationSettings/TemperatureZAxisCoefficientTablePath").node().child_value());
	this->loadTable(1, 1, 22, &BendingModeMatrix, doc.select_node("//ForceActuatorApplicationSettings/BendingModeTablePath").node().child_value());
	this->loadTable(1, 1, 6, &HardpointForceMomentMatrix, doc.select_node("//ForceActuatorApplicationSettings/HardpointForceMomentTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicAccelerationXMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicAccelerationXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicAccelerationYMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicAccelerationYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicAccelerationZMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicAccelerationZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityXMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicVelocityXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityYMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicVelocityYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityZMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicVelocityZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityXZMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicVelocityXZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &DynamicVelocityYZMatrix, doc.select_node("//ForceActuatorApplicationSettings/DynamicVelocityYZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &ForceDistributionXMatrix, doc.select_node("//ForceActuatorApplicationSettings/ForceDistributionXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &ForceDistributionYMatrix, doc.select_node("//ForceActuatorApplicationSettings/ForceDistributionYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &ForceDistributionZMatrix, doc.select_node("//ForceActuatorApplicationSettings/ForceDistributionZTablePath").node().child_value());
	this->loadTable(1, 1, 3, &MomentDistributionXMatrix, doc.select_node("//ForceActuatorApplicationSettings/MomentDistributionXTablePath").node().child_value());
	this->loadTable(1, 1, 3, &MomentDistributionYMatrix, doc.select_node("//ForceActuatorApplicationSettings/MomentDistributionYTablePath").node().child_value());
	this->loadTable(1, 1, 3, &MomentDistributionZMatrix, doc.select_node("//ForceActuatorApplicationSettings/MomentDistributionZTablePath").node().child_value());
}

void ForceActuatorApplicationSettings::loadForceActuatorTable(const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		if (lineNumber != 0 && !lineText.empty()) {
			tokenizer tok(lineText);
			tokenizer::iterator i = tok.begin();
			Table[lineNumber - 1].Index = boost::lexical_cast<int32_t>(*i);
			++i;
			Table[lineNumber - 1].ActuatorID = boost::lexical_cast<int32_t>(*i);
			++i;
			Table[lineNumber - 1].XPosition = boost::lexical_cast<double>(*i);
			++i;
			Table[lineNumber - 1].YPosition = boost::lexical_cast<double>(*i);
			++i;
			Table[lineNumber - 1].ZPosition = boost::lexical_cast<double>(*i);
			++i;
			Table[lineNumber - 1].Type = (*i) == "SAA" ? ForceActuatorTypes::SAA : ForceActuatorTypes::DAA;
			++i;
			Table[lineNumber - 1].Subnet = (uint8_t)boost::lexical_cast<int32_t>(*i);
			++i;
			Table[lineNumber - 1].Address = (uint8_t)boost::lexical_cast<int32_t>(*i);
			++i;
			std::string rawOrientation = *i;
			ForceActuatorOrientations::Type orientation = ForceActuatorOrientations::NA;
			if (rawOrientation == "+Y")
				orientation = ForceActuatorOrientations::PositiveY;
			else if (rawOrientation == "-Y")
				orientation = ForceActuatorOrientations::NegativeY;
			else if (rawOrientation == "+X")
				orientation = ForceActuatorOrientations::PositiveX;
			else if (rawOrientation == "-X")
				orientation = ForceActuatorOrientations::NegativeX;
			Table[lineNumber - 1].Orientation = orientation;
			++i;
			Table[lineNumber - 1].PrimaryAxisSensorOffset = boost::lexical_cast<double>(*i);
			++i;
			Table[lineNumber - 1].PrimaryAxisSensorSensitivity = boost::lexical_cast<double>(*i);
			++i;
			Table[lineNumber - 1].SecondaryAxisSensorOffset = boost::lexical_cast<double>(*i);
			++i;
			Table[lineNumber - 1].SecondaryAxisSensorSensitivity = boost::lexical_cast<double>(*i);
		}
		lineNumber++;
	}
	inputStream.close();
}

void ForceActuatorApplicationSettings::loadStaticForceTable(const std::string &filename) {
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
			StaticForces.X.push_back(boost::lexical_cast<double>(*i));
			++i;
			StaticForces.Y.push_back(boost::lexical_cast<double>(*i));
			++i;
			StaticForces.Z.push_back(boost::lexical_cast<double>(*i));
		}
		lineNumber++;
	}
	inputStream.close();
}

void ForceActuatorApplicationSettings::loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename) {
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

void ForceActuatorApplicationSettings::zeroTable(int items, std::vector<double>* data) {
	for(int i = 0; i < items; i++) {
		data->push_back(0);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
