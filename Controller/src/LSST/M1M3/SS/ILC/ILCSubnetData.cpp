/*
 * ILCSubnetData.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#include <ILCSubnetData.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCSubnetData::ILCSubnetData(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
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
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = forceActuatorApplicationSettings->ZIndexToXIndex[row.Index];
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = forceActuatorApplicationSettings->ZIndexToYIndex[row.Index];
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[row.Index];
		ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
		this->subnetData[subnetIndex].FAIndex.push_back(map);
		this->subnetData[subnetIndex].FACount += 1;
	}
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings->Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HP;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = -1;
		ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
		this->subnetData[subnetIndex].HPIndex.push_back(map);
		this->subnetData[subnetIndex].HPCount += 1;
	}
	for(int i = 0; i < HM_COUNT; i++) {
		HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
		int32_t subnetIndex = row.Subnet - 1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HM;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = -1;
		this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = -1;
		ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
		this->subnetData[subnetIndex].HMIndex.push_back(map);
		this->subnetData[subnetIndex].HMCount += 1;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */