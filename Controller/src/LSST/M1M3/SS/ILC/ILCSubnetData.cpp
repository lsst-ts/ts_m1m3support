/*
 * ILCSubnetData.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#include <ILCSubnetData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCSubnetData::ILCSubnetData(ForceActuatorApplicationSettings forceActuatorApplicationSettings, HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings) {
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->subnetData[subnetIndex].FACount = 0;
		this->subnetData[subnetIndex].HPCount = 0;
	}
	for(int i = 0; i < FA_COUNT; i++) {
		ForceActuatorTableRow row = forceActuatorApplicationSettings.Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::FA;
		ILCAddressDataIndexMap map;
		map.Address = row.Address;
		map.DataIndex = row.Index;
		this->subnetData[subnetIndex].FAIndex.push_back(map);
		this->subnetData[subnetIndex].FACount += 1;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings.Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HP;
		ILCAddressDataIndexMap map;
		map.Address = row.Address;
		map.DataIndex = row.Index;
		this->subnetData[subnetIndex].HPIndex.push_back(map);
		this->subnetData[subnetIndex].HPCount += 1;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
