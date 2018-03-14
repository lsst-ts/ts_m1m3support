/*
 * RaisedBusList.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#include <RaisedBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <RoundRobin.h>
#include <ForceConverter.h>
#include <SAL_m1m3C.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisedBusList::RaisedBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_OuterLoopDataC* outerLoopData, m1m3_ForceActuatorDataC* forceActuatorData, m1m3_HardpointActuatorDataC* hardpointActuatorData, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_logevent_AppliedCylinderForcesC* appliedCylinderForces)
 : BusList(subnetData, ilcMessageFactory) {
	Log.Debug("RaisedBusList: RaisedBusList()");
	this->outerLoopData = outerLoopData;
	this->forceActuatorData = forceActuatorData;
	this->appliedCylinderForces = appliedCylinderForces;
	this->hardpointActuatorData = hardpointActuatorData;
	this->forceInfo = forceInfo;
	for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->setForceCommandIndex[subnetIndex] = -1;
		this->moveStepCommandIndex[subnetIndex] = -1;
		this->faStatusCommandIndex[subnetIndex] = -1;
		this->roundRobinFAReportServerStatusIndex[subnetIndex] = 0;
	}
	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		this->startSubnet(subnetIndex);
		if (this->subnetData->getFACount(subnetIndex) > 0) {
			this->setForceCommandIndex[subnetIndex] = this->buffer.getIndex();
			int32_t saaPrimary[16];
			int32_t daaPrimary[32];
			int32_t daaSecondary[32];
			for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
				uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
				int32_t primaryDataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
				int32_t secondaryDataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).SecondaryDataIndex;

				if (address <= 16) {
					saaPrimary[address - 1] = this->appliedCylinderForces->PrimaryCylinderForces[primaryDataIndex];
				}
				else {
					daaPrimary[address - 17] = this->appliedCylinderForces->PrimaryCylinderForces[primaryDataIndex];
					daaSecondary[address - 17] = this->appliedCylinderForces->SecondaryCylinderForces[secondaryDataIndex];
				}
			}
			this->ilcMessageFactory->broadcastForceDemand(&this->buffer, this->outerLoopData->BroadcastCounter, this->outerLoopData->SlewFlag, saaPrimary, daaPrimary, daaSecondary);
			this->buffer.writeTimestamp();
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
		if (this->subnetData->getHPCount(subnetIndex) > 0) {
			this->moveStepCommandIndex[subnetIndex] = this->buffer.getIndex();
			int8_t steps[78];
			for(int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
				uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
				int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
				steps[address - 1] = this->hardpointActuatorData->StepsCommanded[dataIndex];
			}
			this->ilcMessageFactory->broadcastStepMotor(&this->buffer, this->outerLoopData->BroadcastCounter, steps);
			this->buffer.writeTimestamp();
			for(int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
				uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
				int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
				this->ilcMessageFactory->electromechanicalForceAndStatus(&this->buffer, address);
				this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
				this->expectedHPResponses[dataIndex] = 2;
			}
		}
		for(int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
			uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
			int32_t dataIndex = this->subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
			this->ilcMessageFactory->reportLVDT(&this->buffer, address);
			this->ilcMessageFactory->reportDCAPressure(&this->buffer, address);
			this->ilcMessageFactory->reportDCAStatus(&this->buffer, address);
			this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
			this->expectedHMResponses[dataIndex] = 4;
		}
		this->endSubnet();
	}
	this->buffer.setLength(this->buffer.getIndex());
}

void RaisedBusList::update() {
	this->outerLoopData->BroadcastCounter = RoundRobin::BroadcastCounter(this->outerLoopData->BroadcastCounter);

	for(int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
		if (this->subnetData->getFACount(subnetIndex) > 0) {
			int32_t saaPrimary[16];
			int32_t daaPrimary[32];
			int32_t daaSecondary[32];
			for(int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
				uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
				int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
				if (address <= 16) {
					saaPrimary[address - 1] = this->appliedCylinderForces->PrimaryCylinderForces[dataIndex];
				}
				else {
					daaPrimary[address - 17] = this->appliedCylinderForces->PrimaryCylinderForces[dataIndex];
					daaSecondary[address - 17] = this->appliedCylinderForces->SecondaryCylinderForces[dataIndex];
				}
			}
			this->buffer.setIndex(this->setForceCommandIndex[subnetIndex]);
			this->ilcMessageFactory->broadcastForceDemand(&this->buffer, this->outerLoopData->BroadcastCounter, this->outerLoopData->SlewFlag, saaPrimary, daaPrimary, daaSecondary);

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
		if (this->subnetData->getHPCount(subnetIndex) > 0) {
			int8_t steps[78];
			for(int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
				uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
				int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
				steps[address - 1] = this->hardpointActuatorData->StepsCommanded[dataIndex];
			}
			this->buffer.setIndex(this->moveStepCommandIndex[subnetIndex]);
			this->ilcMessageFactory->broadcastStepMotor(&this->buffer, this->outerLoopData->BroadcastCounter, steps);
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
