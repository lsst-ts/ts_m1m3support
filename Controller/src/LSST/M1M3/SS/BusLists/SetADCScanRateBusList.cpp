/*
 * SetADCScanRateBusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <SetADCScanRateBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SetADCScanRateBusList::SetADCScanRateBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_logevent_HardpointActuatorInfoC* hardpointInfo)
 : BusList(subnetData, ilcMessageFactory) {
	this->forceInfo = forceInfo;
	this->hardpointInfo = hardpointInfo;
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->startSubnet(subnetIndex);
		for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
			this->ilcMessageFactory->setADCScanRate(&this->buffer, address, this->forceInfo->ADCScanRate[dataIndex]);
			this->expectedFAResponses[dataIndex] = 1;
		}
		for(int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
			uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
			int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
			this->ilcMessageFactory->setADCScanRate(&this->buffer, address, this->hardpointInfo->ADCScanRate[dataIndex]);
			this->expectedHPResponses[dataIndex] = 1;
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
