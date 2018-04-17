/*
 * ReportDCAIDBusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <ReportDCAIDBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ReportDCAIDBusList::ReportDCAIDBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory)
 : BusList(subnetData, ilcMessageFactory) {
	Log.Debug("ReportDCAIDBusList: ReportDCAIDBusList()");
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->startSubnet(subnetIndex);
		for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
			this->ilcMessageFactory->reportDCAID(&this->buffer, address);
			this->expectedFAResponses[dataIndex] = 1;
		}
		for(int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
			uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
			int32_t dataIndex = this->subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
			this->ilcMessageFactory->reportDCAID(&this->buffer, address);
			this->expectedHMResponses[dataIndex] = 1;
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
