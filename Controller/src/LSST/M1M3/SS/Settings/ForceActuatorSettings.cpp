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
#include <TableLoader.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());
	this->loadDisabledActuators(doc.select_node("//ForceActuatorSettings/DisabledActuators").node().child_value());
	TableLoader::loadTable(1, 1, 3, &AccelerationXTable, doc.select_node("//ForceActuatorSettings/AccelerationXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &AccelerationYTable, doc.select_node("//ForceActuatorSettings/AccelerationYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &AccelerationZTable, doc.select_node("//ForceActuatorSettings/AccelerationZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 22, &BendingModeTable, doc.select_node("//ForceActuatorSettings/BendingModeTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &AzimuthXTable, doc.select_node("//ForceActuatorSettings/AzimuthXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &AzimuthYTable, doc.select_node("//ForceActuatorSettings/AzimuthYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &AzimuthZTable, doc.select_node("//ForceActuatorSettings/AzimuthZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &HardpointForceMomentTable, doc.select_node("//ForceActuatorSettings/HardpointForceMomentTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &ForceDistributionXTable, doc.select_node("//ForceActuatorSettings/ForceDistributionXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &ForceDistributionYTable, doc.select_node("//ForceActuatorSettings/ForceDistributionYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &ForceDistributionZTable, doc.select_node("//ForceActuatorSettings/ForceDistributionZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &MomentDistributionXTable, doc.select_node("//ForceActuatorSettings/MomentDistributionXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &MomentDistributionYTable, doc.select_node("//ForceActuatorSettings/MomentDistributionYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &MomentDistributionZTable, doc.select_node("//ForceActuatorSettings/MomentDistributionZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &ElevationXTable, doc.select_node("//ForceActuatorSettings/ElevationXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &ElevationYTable, doc.select_node("//ForceActuatorSettings/ElevationYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &ElevationZTable, doc.select_node("//ForceActuatorSettings/ElevationZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &StaticXTable, doc.select_node("//ForceActuatorSettings/StaticXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &StaticYTable, doc.select_node("//ForceActuatorSettings/StaticYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 1, &StaticZTable, doc.select_node("//ForceActuatorSettings/StaticZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &ThermalXTable, doc.select_node("//ForceActuatorSettings/ThermalXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &ThermalYTable, doc.select_node("//ForceActuatorSettings/ThermalYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 6, &ThermalZTable, doc.select_node("//ForceActuatorSettings/ThermalZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &VelocityXTable, doc.select_node("//ForceActuatorSettings/VelocityXTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &VelocityYTable, doc.select_node("//ForceActuatorSettings/VelocityYTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &VelocityZTable, doc.select_node("//ForceActuatorSettings/VelocityZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &VelocityXZTable, doc.select_node("//ForceActuatorSettings/VelocityXZTablePath").node().child_value());
	TableLoader::loadTable(1, 1, 3, &VelocityYZTable, doc.select_node("//ForceActuatorSettings/VelocityYZTablePath").node().child_value());

	TableLoader::loadLimitTable(1, 1, &AberrationLimitZTable, doc.select_node("//ForceActuatorSettings/AberrationLimitZTablePath").node().child_value());
	this->NetAberrationForceTolerance = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/NetAberrationForceTolerance").node().child_value());
	TableLoader::loadLimitTable(1, 1, &AccelerationLimitXTable, doc.select_node("//ForceActuatorSettings/AccelerationLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &AccelerationLimitYTable, doc.select_node("//ForceActuatorSettings/AccelerationLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &AccelerationLimitZTable, doc.select_node("//ForceActuatorSettings/AccelerationLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ActiveOpticLimitZTable, doc.select_node("//ForceActuatorSettings/ActiveOpticLimitZTablePath").node().child_value());
	this->NetActiveOpticForceTolerance = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/NetActiveOpticForceTolerance").node().child_value());
	TableLoader::loadLimitTable(1, 1, &AzimuthLimitXTable, doc.select_node("//ForceActuatorSettings/AzimuthLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &AzimuthLimitYTable, doc.select_node("//ForceActuatorSettings/AzimuthLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &AzimuthLimitZTable, doc.select_node("//ForceActuatorSettings/AzimuthLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &BalanceLimitXTable, doc.select_node("//ForceActuatorSettings/BalanceLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &BalanceLimitYTable, doc.select_node("//ForceActuatorSettings/BalanceLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &BalanceLimitZTable, doc.select_node("//ForceActuatorSettings/BalanceLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ElevationLimitXTable, doc.select_node("//ForceActuatorSettings/ElevationLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ElevationLimitYTable, doc.select_node("//ForceActuatorSettings/ElevationLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ElevationLimitZTable, doc.select_node("//ForceActuatorSettings/ElevationLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ForceLimitXTable, doc.select_node("//ForceActuatorSettings/ForceLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ForceLimitYTable, doc.select_node("//ForceActuatorSettings/ForceLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ForceLimitZTable, doc.select_node("//ForceActuatorSettings/ForceLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &StaticLimitXTable, doc.select_node("//ForceActuatorSettings/StaticLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &StaticLimitYTable, doc.select_node("//ForceActuatorSettings/StaticLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &StaticLimitZTable, doc.select_node("//ForceActuatorSettings/StaticLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &OffsetLimitXTable, doc.select_node("//ForceActuatorSettings/OffsetLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &OffsetLimitYTable, doc.select_node("//ForceActuatorSettings/OffsetLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &OffsetLimitZTable, doc.select_node("//ForceActuatorSettings/OffsetLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ThermalLimitXTable, doc.select_node("//ForceActuatorSettings/ThermalLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ThermalLimitYTable, doc.select_node("//ForceActuatorSettings/ThermalLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &ThermalLimitZTable, doc.select_node("//ForceActuatorSettings/ThermalLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &VelocityLimitXTable, doc.select_node("//ForceActuatorSettings/VelocityLimitXTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &VelocityLimitYTable, doc.select_node("//ForceActuatorSettings/VelocityLimitYTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &VelocityLimitZTable, doc.select_node("//ForceActuatorSettings/VelocityLimitZTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &CylinderLimitPrimaryTable, doc.select_node("//ForceActuatorSettings/CylinderLimitPrimaryTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &CylinderLimitSecondaryTable, doc.select_node("//ForceActuatorSettings/CylinderLimitSecondaryTablePath").node().child_value());

	TableLoader::loadLimitTable(1, 1, &MeasuredPrimaryCylinderLimitTable, doc.select_node("//ForceActuatorSettings/MeasuredPrimaryCylinderLimitTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &MeasuredSecondaryCylinderLimitTable, doc.select_node("//ForceActuatorSettings/MeasuredSecondaryCylinderLimitTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &FollowingErrorPrimaryCylinderLimitTable, doc.select_node("//ForceActuatorSettings/FollowingErrorPrimaryCylinderLimitTablePath").node().child_value());
	TableLoader::loadLimitTable(1, 1, &FollowingErrorSecondaryCylinderLimitTable, doc.select_node("//ForceActuatorSettings/FollowingErrorSecondaryCylinderLimitTablePath").node().child_value());

	this->Neighbors.clear();
	for(int i = 0; i < FA_COUNT; ++i) {
		ForceActuatorNeighbors neighbors;
		this->Neighbors.push_back(neighbors);
	}
	this->loadNearNeighborZTable(doc.select_node("//ForceActuatorSettings/ForceActuatorNearZNeighborsTablePath").node().child_value());
	this->loadNeighborsTable(doc.select_node("//ForceActuatorSettings/ForceActuatorNeighborsTablePath").node().child_value());

	this->UseInclinometer = boost::lexical_cast<int32_t>(doc.select_node("//ForceActuatorSettings/UseInclinometer").node().child_value()) != 0;
	this->MirrorXMoment = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/MirrorXMoment").node().child_value());
	this->MirrorYMoment = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/MirrorYMoment").node().child_value());
	this->MirrorZMoment = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/MirrorZMoment").node().child_value());
	this->SetpointXMomentLowLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointXMomentLowLimitPercentage").node().child_value());
	this->SetpointXMomentHighLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointXMomentHighLimitPercentage").node().child_value());
	this->SetpointYMomentLowLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointYMomentLowLimitPercentage").node().child_value());
	this->SetpointYMomentHighLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointYMomentHighLimitPercentage").node().child_value());
	this->SetpointZMomentLowLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointZMomentLowLimitPercentage").node().child_value());
	this->SetpointZMomentHighLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointZMomentHighLimitPercentage").node().child_value());
	this->SetpointNearNeighborLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointNearNeighborLimitPercentage").node().child_value());
	this->SetpointMirrorWeightLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointMirrorWeightLimitPercentage").node().child_value());
	this->SetpointFarNeighborLimitPercentage = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/SetpointFarNeighborLimitPercentage").node().child_value());

	this->MirrorCenterOfGravityX = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/MirrorCenterOfGravityX").node().child_value());
	this->MirrorCenterOfGravityY = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/MirrorCenterOfGravityY").node().child_value());
	this->MirrorCenterOfGravityZ = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/MirrorCenterOfGravityZ").node().child_value());

	this->RaiseIncrementPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/RaiseIncrementPercentage").node().child_value());
	this->LowerDecrementPercentage = boost::lexical_cast<double>(doc.select_node("//ForceActuatorSettings/LowerDecrementPercentage").node().child_value());
	this->RaiseLowerFollowingErrorLimit = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/RaiseLowerFollowingErrorLimit").node().child_value());

	this->AberrationComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/AberrationForceComponent/MaxRateOfChange").node().child_value());
	this->AberrationComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/AberrationForceComponent/NearZeroValue").node().child_value());

	this->AccelerationComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/AccelerationForceComponent/MaxRateOfChange").node().child_value());
	this->AccelerationComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/AccelerationForceComponent/NearZeroValue").node().child_value());

	this->ActiveOpticComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/ActiveOpticForceComponent/MaxRateOfChange").node().child_value());
	this->ActiveOpticComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/ActiveOpticForceComponent/NearZeroValue").node().child_value());

	this->AzimuthComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/AzimuthForceComponent/MaxRateOfChange").node().child_value());
	this->AzimuthComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/AzimuthForceComponent/NearZeroValue").node().child_value());

	this->BalanceComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/BalanceForceComponent/MaxRateOfChange").node().child_value());
	this->BalanceComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/BalanceForceComponent/NearZeroValue").node().child_value());

	this->ElevationComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/ElevationForceComponent/MaxRateOfChange").node().child_value());
	this->ElevationComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/ElevationForceComponent/NearZeroValue").node().child_value());

	this->OffsetComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/OffsetForceComponent/MaxRateOfChange").node().child_value());
	this->OffsetComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/OffsetForceComponent/NearZeroValue").node().child_value());

	this->StaticComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/StaticForceComponent/MaxRateOfChange").node().child_value());
	this->StaticComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/StaticForceComponent/NearZeroValue").node().child_value());

	this->ThermalComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/ThermalForceComponent/MaxRateOfChange").node().child_value());
	this->ThermalComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/ThermalForceComponent/NearZeroValue").node().child_value());

	this->VelocityComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/VelocityForceComponent/MaxRateOfChange").node().child_value());
	this->VelocityComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/VelocityForceComponent/NearZeroValue").node().child_value());

	this->FinalComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/FinalForceComponent/MaxRateOfChange").node().child_value());
	this->FinalComponentSettings.NearZeroValue = boost::lexical_cast<float>(doc.select_node("//ForceActuatorSettings/FinalForceComponent/NearZeroValue").node().child_value());
}

bool ForceActuatorSettings::IsActuatorDisabled(int32_t actId) {
	for(unsigned int i = 0; i < this->DisabledActuators.size(); ++i) {
		if (this->DisabledActuators[i] == actId) {
			return true;
		}
	}
	return false;
}

void ForceActuatorSettings::loadDisabledActuators(const std::string line) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	tokenizer tok(line);
	tokenizer::iterator i = tok.begin();
	this->DisabledActuators.clear();
	while (i != tok.end()) {
		this->DisabledActuators.push_back(boost::lexical_cast<int32_t>(*i));
		Log.Warn("ForceActuatorSettings: Disabled Actuator %d", this->DisabledActuators[this->DisabledActuators.size() - 1]);
		++i;
	}
}

void ForceActuatorSettings::loadNearNeighborZTable(const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		// Skip the first ROW (Header)
		if (lineNumber >= 1 && !lineText.empty()) {
			// Line Format:
			//     ActuatorID,Neighbor1,...,Neighbor7
			tokenizer tok(lineText);
			ForceActuatorNeighbors neighbors;
			tokenizer::iterator i = tok.begin();
			// Skip the first COLUMN (Row ID)
			++i;
			for(; i != tok.end(); ++i) {
				int32_t id = boost::lexical_cast<int32_t>(*i);
				if (id != 0) {
					this->Neighbors[lineNumber - 1].NearZIDs.push_back(id);
				}
			}
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
					this->Neighbors[lineNumber - 1].FarIDs.push_back(id);
				}
			}
		}
		lineNumber++;
	}
	inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
