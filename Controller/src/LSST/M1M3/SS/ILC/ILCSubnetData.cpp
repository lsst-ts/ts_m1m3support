/*
 * ILCSubnetData.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#include <ILCSubnetData.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCSubnetData::ILCSubnetData(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
	Log.Debug("ILCSubnetData::ILCSubnetData()");
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->subnetData[subnetIndex].FACount = 0;
		this->subnetData[subnetIndex].HPCount = 0;
		this->subnetData[subnetIndex].HMCount = 0;
	}
	for(int i = 0; i < FA_COUNT; i++) {
		ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::FA;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Subnet = row.Subnet;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = forceActuatorApplicationSettings->ZIndexToXIndex[row.Index];
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = forceActuatorApplicationSettings->ZIndexToYIndex[row.Index];
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[row.Index];
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Disabled = forceActuatorSettings->IsActuatorDisabled(row.ActuatorID);
		ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
		this->subnetData[subnetIndex].FAIndex.push_back(map);
		this->subnetData[subnetIndex].FACount += 1;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings->Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HP;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Subnet = row.Subnet;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Disabled = false;
		ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
		this->subnetData[subnetIndex].HPIndex.push_back(map);
		this->subnetData[subnetIndex].HPCount += 1;
	}
	for(int i = 0; i < HM_COUNT; i++) {
		HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HM;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Subnet = row.Subnet;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Disabled = false;
		ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
		this->subnetData[subnetIndex].HMIndex.push_back(map);
		this->subnetData[subnetIndex].HMCount += 1;
	}
}

ILCMap ILCSubnetData::getMap(int32_t actuatorId) {
	for(int subnetIndex = 0; subnetIndex < 5; ++subnetIndex) {
		Container container = this->subnetData[subnetIndex];
		for(int i = 0; i < container.HPCount; ++i) {
			if (container.HPIndex[i].ActuatorId == actuatorId) {
				return container.HPIndex[i];
			}
		}
		for(int i = 0; i < container.FACount; ++i) {
			if (container.FAIndex[i].ActuatorId == actuatorId) {
				return container.FAIndex[i];
			}
		}
		for(int i = 0; i < container.HMCount; ++i) {
			if (container.HMIndex[i].ActuatorId == actuatorId) {
				return container.HMIndex[i];
			}
		}
	}
	ILCMap none;
	none.Address = 255;
	none.Subnet = 255;
	return none;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
