/*
 * SetBoostValveDCAGainBusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <SetBoostValveDCAGainBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <SAL_MTM1M3C.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SetBoostValveDCAGainBusList::SetBoostValveDCAGainBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_logevent_forceActuatorInfoC* forceInfo)
 : BusList(subnetData, ilcMessageFactory) {
	Log.Debug("SetBoostValveDCAGainBusList: SetBoostValveDCAGainBusList()");
	this->forceInfo = forceInfo;
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->startSubnet(subnetIndex);
		for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
			bool disabled = this->subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
			if (!disabled) {
				this->ilcMessageFactory->setBoostValveDCAGains(&this->buffer, address, this->forceInfo->mezzaninePrimaryCylinderGain[dataIndex], this->forceInfo->mezzanineSecondaryCylinderGain[dataIndex]);
				this->expectedFAResponses[dataIndex] = 1;
			}
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
