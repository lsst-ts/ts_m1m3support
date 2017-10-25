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

namespace LSST {
namespace M1M3 {
namespace SS {

RaisedBusList::RaisedBusList()
 : BusList() {
	this->outerLoopData = 0;
	this->forceData = 0;
	this->hardpointData = 0;
	this->forceInfo = 0;
}

RaisedBusList::RaisedBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_OuterLoopDataC* outerLoopData, m1m3_ForceActuatorDataC* forceData, m1m3_HardpointDataC* hardpointData, m1m3_logevent_ForceActuatorInfoC* forceInfo)
 : BusList(subnetData, ilcMessageFactory) {
	this->outerLoopData = outerLoopData;
	this->forceData = forceData;
	this->hardpointData = hardpointData;
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
				int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
				if (address <= 16) {
					saaPrimary[address - 1] = this->forceData->PrimaryCylinderSetpointCommanded[dataIndex];
				}
				else {
					daaPrimary[address - 17] = this->forceData->PrimaryCylinderSetpointCommanded[dataIndex];
					daaSecondary[address - 17] = this->forceData->SecondaryCylinderSetpointCommanded[dataIndex];
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
				steps[address - 1] = this->hardpointData->StepsCommanded[dataIndex];
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
				switch(this->forceInfo->ActuatorOrientation[dataIndex]) {
				case ForceActuatorOrientations::PositiveX: ForceConverter::daaPositiveXToCylinder(this->forceData->XSetpoint[dataIndex], this->forceData->YSetpoint[dataIndex], this->forceData->ZSetpoint[dataIndex], this->forceData->PrimaryCylinderSetpointCommanded + dataIndex, this->forceData->SecondaryCylinderSetpointCommanded + dataIndex); break;
				case ForceActuatorOrientations::NegativeX: ForceConverter::daaNegativeXToCylinder(this->forceData->XSetpoint[dataIndex], this->forceData->YSetpoint[dataIndex], this->forceData->ZSetpoint[dataIndex], this->forceData->PrimaryCylinderSetpointCommanded + dataIndex, this->forceData->SecondaryCylinderSetpointCommanded + dataIndex); break;
				case ForceActuatorOrientations::PositiveY: ForceConverter::daaPositiveYToCylinder(this->forceData->XSetpoint[dataIndex], this->forceData->YSetpoint[dataIndex], this->forceData->ZSetpoint[dataIndex], this->forceData->PrimaryCylinderSetpointCommanded + dataIndex, this->forceData->SecondaryCylinderSetpointCommanded + dataIndex); break;
				case ForceActuatorOrientations::NegativeY: ForceConverter::daaNegativeYToCylinder(this->forceData->XSetpoint[dataIndex], this->forceData->YSetpoint[dataIndex], this->forceData->ZSetpoint[dataIndex], this->forceData->PrimaryCylinderSetpointCommanded + dataIndex, this->forceData->SecondaryCylinderSetpointCommanded + dataIndex); break;
				case ForceActuatorOrientations::NA: ForceConverter::saaToCylinder(this->forceData->XSetpoint[dataIndex], this->forceData->YSetpoint[dataIndex], this->forceData->ZSetpoint[dataIndex], this->forceData->PrimaryCylinderSetpointCommanded + dataIndex, this->forceData->SecondaryCylinderSetpointCommanded + dataIndex); break;
				}
				if (address <= 16) {
					saaPrimary[address - 1] = this->forceData->PrimaryCylinderSetpointCommanded[dataIndex];
				}
				else {
					daaPrimary[address - 17] = this->forceData->PrimaryCylinderSetpointCommanded[dataIndex];
					daaSecondary[address - 17] = this->forceData->SecondaryCylinderSetpointCommanded[dataIndex];
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
				steps[address - 1] = this->hardpointData->StepsCommanded[dataIndex];
			}
			this->buffer.setIndex(this->moveStepCommandIndex[subnetIndex]);
			this->ilcMessageFactory->broadcastStepMotor(&this->buffer, this->outerLoopData->BroadcastCounter, steps);
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
