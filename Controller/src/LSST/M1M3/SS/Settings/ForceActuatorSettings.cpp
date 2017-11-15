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

#include <iostream>

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	this->loadStaticForceTable(doc.select_node("//ForceActuatorSettings/StaticForceTablePath").node().child_value());
	this->loadLimitTable(doc.select_node("//ForceActuatorSettings/ForceActuatorLimitsTablePath").node().child_value());
	this->loadNeighborsTable(doc.select_node("//ForceActuatorSettings/ForceActuatorNeighborsTablePath").node().child_value());
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
	this->MirrorWeight = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/MirrorWeight").node().child_value());
	this->MirrorXMoment = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/MirrorXMoment").node().child_value());
	this->MirrorYMoment = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/MirrorYMoment").node().child_value());
	this->MirrorZMoment = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/MirrorZMoment").node().child_value());
	this->SetpointXMomentLowLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointXMomentLowLimitPercentage").node().child_value());
	this->SetpointXMomentHighLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointXMomentHighLimitPercentage").node().child_value());
	this->SetpointYMomentLowLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointYMomentLowLimitPercentage").node().child_value());
	this->SetpointYMomentHighLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointYMomentHighLimitPercentage").node().child_value());
	this->SetpointZMomentLowLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointZMomentLowLimitPercentage").node().child_value());
	this->SetpointZMomentHighLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointZMomentHighLimitPercentage").node().child_value());
	this->SetpointNearNeighborLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointNearNeighborLimitPercentage").node().child_value());
	this->SetpointMirrorWeightLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointMirrorWeightLimitPercentage").node().child_value());
	this->SetpointFarNeighborLimitPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/SetpointFarNeighborLimitPercentage").node().child_value());
	this->NetAOSForceTolerance = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/NetAOSForceTolerance").node().child_value());
	this->NetAberrationForceTolerance = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/NetAberrationForceTolerance").node().child_value());
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

void ForceActuatorSettings::loadLimitTable(const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		if (lineNumber >= 1 && !lineText.empty()) {
			// Line Format:
			//     ActuatorID,PrimaryAxisLowLimit,PrimaryAxisHighLimit,SecondaryAxisLowLimit,SecondaryAxisHighLimit
			tokenizer tok(lineText);
			tokenizer::iterator i = tok.begin();
			ForceActuatorLimits limits;
			limits.Id = boost::lexical_cast<int32_t>(*i);
			++i;
			limits.PrimaryAxisTotalLowLimit = (int32_t)(boost::lexical_cast<double>(*i) * 1000);
			++i;
			limits.PrimaryAxisTotalHighLimit = (int32_t)(boost::lexical_cast<double>(*i) * 1000);
			++i;
			limits.SecondaryAxisTotalLowLimit = (int32_t)(boost::lexical_cast<double>(*i) * 1000);
			++i;
			limits.SecondaryAxisTotalHighLimit = (int32_t)(boost::lexical_cast<double>(*i) * 1000);
			++i;
			limits.ElevationXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.ElevationXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.ElevationYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.ElevationYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.ElevationZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.ElevationZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AzimuthXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AzimuthXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AzimuthYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AzimuthYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AzimuthZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AzimuthZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.TemperatureXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.TemperatureXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.TemperatureYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.TemperatureYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.TemperatureZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.TemperatureZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.HardpointOffloadingXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.HardpointOffloadingXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.HardpointOffloadingYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.HardpointOffloadingYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.HardpointOffloadingZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.HardpointOffloadingZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.DynamicXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.DynamicXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.DynamicYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.DynamicYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.DynamicZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.DynamicZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AOSZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AOSZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AberrationZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.AberrationZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.OffsetXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.OffsetXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.OffsetYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.OffsetYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.OffsetZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.OffsetZHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.StaticXLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.StaticXHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.StaticYLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.StaticYHighLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.StaticZLowLimit = boost::lexical_cast<double>(*i);
			++i;
			limits.StaticZHighLimit = boost::lexical_cast<double>(*i);
			SetpointLimits.push_back(limits);
		}
		lineNumber++;
	}
	inputStream.close();
}

void ForceActuatorSettings::loadNeighborsTable(const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		if (lineNumber >= 1 && !lineText.empty()) {
			// Line Format:
			//     ActuatorID,Neighbor1,...,Neighbor12
			tokenizer tok(lineText);
			tokenizer::iterator i = tok.begin();
			ForceActuatorNeighbors neighbors;
			for(int j = 0; j < 12; j++) {
				++i;
				int32_t id = boost::lexical_cast<int32_t>(*i);
				if (id != 0) {
					if (j < 6) {
						neighbors.NearIDs.push_back(id);
					}
					neighbors.FarIDs.push_back(id);
				}
			}
			Neighbors.push_back(neighbors);
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
