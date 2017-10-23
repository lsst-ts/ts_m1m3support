/*
 * ReportDCAIDBusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <ReportDCAIDBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ReportDCAIDBusList::ReportDCAIDBusList()
 : BusList() { }

ReportDCAIDBusList::ReportDCAIDBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory)
 : BusList(subnetData, ilcMessageFactory) {
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->startSubnet(subnetIndex);
		for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
			this->ilcMessageFactory->reportDCAID(&this->buffer, address);
			this->expectedFAResponses[dataIndex] = 1;
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
