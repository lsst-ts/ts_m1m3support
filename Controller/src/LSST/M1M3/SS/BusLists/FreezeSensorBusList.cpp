/*
 * FreezeSensorBusList.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#include <FreezeSensorBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <RoundRobin.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FreezeSensorBusList::FreezeSensorBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_OuterLoopDataC* outerLoopData)
 : BusList(subnetData, ilcMessageFactory) {
	this->outerLoopData = outerLoopData;
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->freezeSensorCommandIndex[subnetIndex] = -1;
		this->faStatusCommandIndex[subnetIndex] = -1;
		this->roundRobinFAReportServerStatusIndex[subnetIndex] = 0;
	}
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->startSubnet(subnetIndex);
		this->freezeSensorCommandIndex[subnetIndex] = this->buffer.getIndex();
		this->ilcMessageFactory->broadcastFreezeSensorValues(&this->buffer, this->outerLoopData->BroadcastCounter);
		this->buffer.writeTimestamp();
		if (this->subnetData->getFACount(subnetIndex) > 0) {
			for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
				uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
				int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
				this->ilcMessageFactory->pneumaticForceStatus(&this->buffer, address);
				this->expectedFAResponses[dataIndex] = 1;
			}
			int32_t statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, statusIndex).Address;
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
			this->faStatusCommandIndex[subnetIndex] = this->buffer.getIndex();
			this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
			this->expectedFAResponses[dataIndex] = 2;
		}
		for(int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
			uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
			int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
			this->ilcMessageFactory->electromechanicalForceAndStatus(&this->buffer, address);
			this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
			this->expectedHPResponses[dataIndex] = 2;
		}
		for(int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
			uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
			int32_t dataIndex = this->subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
			this->ilcMessageFactory->reportLVDT(&this->buffer, address);
			this->ilcMessageFactory->reportDCAPressure(&this->buffer, address);
			this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
			this->expectedHMResponses[dataIndex] = 3;
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

void FreezeSensorBusList::update() {
	this->outerLoopData->BroadcastCounter = RoundRobin::BroadcastCounter(this->outerLoopData->BroadcastCounter);
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->buffer.setIndex(this->freezeSensorCommandIndex[subnetIndex]);
		this->ilcMessageFactory->broadcastFreezeSensorValues(&this->buffer, this->outerLoopData->BroadcastCounter);
		if (this->subnetData->getFACount(subnetIndex) > 0) {
			int32_t statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
			int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
			this->expectedFAResponses[dataIndex] = 1;
			this->roundRobinFAReportServerStatusIndex[subnetIndex] = RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
			statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
			uint8_t address = this->subnetData->getFAIndex(subnetIndex, statusIndex).Address;
			dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;

			this->buffer.setIndex(this->faStatusCommandIndex[subnetIndex]);
			this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
			this->expectedFAResponses[dataIndex] = 2;
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
