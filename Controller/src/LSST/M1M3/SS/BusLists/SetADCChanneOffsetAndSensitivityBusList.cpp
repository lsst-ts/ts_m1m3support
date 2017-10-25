/*
 * SetADCChanneOffsetAndSensitivityBusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <SetADCChanneOffsetAndSensitivityBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <SAL_m1m3C.h>
#include <iostream>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

SetADCChanneOffsetAndSensitivityBusList::SetADCChanneOffsetAndSensitivityBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_logevent_HardpointActuatorInfoC* hardpointInfo)
 : BusList(subnetData, ilcMessageFactory) {
	this->forceInfo = forceInfo;
	this->hardpointInfo = hardpointInfo;
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; ++subnetIndex) {
		this->startSubnet(subnetIndex);
		for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); ++faIndex) {
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
			this->ilcMessageFactory->setADCChannelOffsetAndSensitivity(&this->buffer, address, 1, this->forceInfo->PrimaryCylinderSensorOffset[dataIndex], this->forceInfo->PrimaryCylinderSensorSensitivity[dataIndex]);
			this->ilcMessageFactory->setADCChannelOffsetAndSensitivity(&this->buffer, address, 2, this->forceInfo->SecondaryCylinderSensorOffset[dataIndex], this->forceInfo->SecondaryCylinderSensorSensitivity[dataIndex]);
			this->expectedFAResponses[dataIndex] = 2;
		}
		for(int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); ++hpIndex) {
			uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
			int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
			this->ilcMessageFactory->setADCChannelOffsetAndSensitivity(&this->buffer, address, 1, this->hardpointInfo->SensorOffset[dataIndex], this->hardpointInfo->SensorSensitivity[dataIndex]);
			this->expectedHPResponses[dataIndex] = 1;
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
