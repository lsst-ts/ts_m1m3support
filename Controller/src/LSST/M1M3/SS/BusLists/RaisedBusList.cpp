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
#include <SAL_MTM1M3C.h>
#include <cstring>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisedBusList::RaisedBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_outerLoopDataC* outerLoopData, MTM1M3_forceActuatorDataC* forceActuatorData, MTM1M3_logevent_appliedHardpointStepsC* appliedHardpointSteps, MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces, MTM1M3_logevent_forceActuatorStateC* forceActuatorState)
        : BusList(subnetData, ilcMessageFactory) {
    Log.Debug("RaisedBusList: RaisedBusList()");
    this->outerLoopData = outerLoopData;
    this->forceActuatorData = forceActuatorData;
    this->appliedCylinderForces = appliedCylinderForces;
    this->appliedHardpointSteps = appliedHardpointSteps;
    this->forceActuatorState = forceActuatorState;
    this->lvdtSampleClock = 0;
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->setForceCommandIndex[subnetIndex] = -1;
        this->moveStepCommandIndex[subnetIndex] = -1;
        this->faStatusCommandIndex[subnetIndex] = -1;
        this->roundRobinFAReportServerStatusIndex[subnetIndex] = 0;
    }
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->startSubnet(subnetIndex);
        if (this->subnetData->getFACount(subnetIndex) > 0) {
            this->setForceCommandIndex[subnetIndex] = this->buffer.getIndex();
            int32_t saaPrimary[16];
            int32_t daaPrimary[32];
            int32_t daaSecondary[32];
            memset(saaPrimary, 0, sizeof(saaPrimary));
            memset(daaPrimary, 0, sizeof(daaPrimary));
            memset(daaSecondary, 0, sizeof(daaSecondary));
            for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
                uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
                int32_t primaryDataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
                int32_t secondaryDataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).SecondaryDataIndex;

                if (address <= 16) {
                    saaPrimary[address - 1] = this->appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                } else {
                    daaPrimary[address - 17] = this->appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                    daaSecondary[address - 17] = this->appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex];
                }
            }
            this->ilcMessageFactory->broadcastForceDemand(&this->buffer, this->outerLoopData->broadcastCounter, this->forceActuatorState->slewFlag, saaPrimary, daaPrimary, daaSecondary);
            this->buffer.writeTimestamp();
            for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
                uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
                int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
                bool disabled = this->subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
                if (!disabled) {
                    this->ilcMessageFactory->pneumaticForceStatus(&this->buffer, address);
                    this->expectedFAResponses[dataIndex] = 1;
                }
            }
            int32_t statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            while (this->subnetData->getFAIndex(subnetIndex, statusIndex).Disabled) {
                this->roundRobinFAReportServerStatusIndex[subnetIndex] = RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
                statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            }
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, statusIndex).Address;
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
            this->faStatusCommandIndex[subnetIndex] = this->buffer.getIndex();
            this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
            this->expectedFAResponses[dataIndex] = 2;
        }
        if (this->subnetData->getHPCount(subnetIndex) > 0) {
            this->moveStepCommandIndex[subnetIndex] = this->buffer.getIndex();
            int8_t steps[78];
            for (int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
                uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
                int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
                // Steps are swapped because negative steps extend and positive steps retract
                // This doesn't match what most people would expect so we are swapping it
                steps[address - 1] = -this->appliedHardpointSteps->commandedSteps[dataIndex];
            }
            this->ilcMessageFactory->broadcastStepMotor(&this->buffer, this->outerLoopData->broadcastCounter, steps);
            this->buffer.writeTimestamp();
            for (int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
                uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
                int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
                bool disabled = this->subnetData->getHPIndex(subnetIndex, hpIndex).Disabled;
                if (!disabled) {
                    this->ilcMessageFactory->electromechanicalForceAndStatus(&this->buffer, address);
                    this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
                    this->expectedHPResponses[dataIndex] = 2;
                }
            }
        }
        for (int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
            uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
            int32_t dataIndex = this->subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
            bool disabled = this->subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->reportDCAPressure(&this->buffer, address);
                this->ilcMessageFactory->reportDCAStatus(&this->buffer, address);
                this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
                this->expectedHMResponses[dataIndex] = 3;
            }
        }
        if (this->subnetData->getHMCount(subnetIndex) > 0) {
            this->hmLVDTCommandIndex[subnetIndex] = this->buffer.getIndex();
            for (int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
                uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
                bool disabled = this->subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
                if (!disabled) {
                    this->ilcMessageFactory->nopReportLVDT(&this->buffer, address);
                }
            }
        }
        this->endSubnet();
    }
    this->buffer.setLength(this->buffer.getIndex());
}

void RaisedBusList::update() {
    this->outerLoopData->broadcastCounter = RoundRobin::BroadcastCounter(this->outerLoopData->broadcastCounter);

    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        if (this->subnetData->getFACount(subnetIndex) > 0) {
            int32_t saaPrimary[16];
            int32_t daaPrimary[32];
            int32_t daaSecondary[32];
            memset(saaPrimary, 0, sizeof(saaPrimary));
            memset(daaPrimary, 0, sizeof(daaPrimary));
            memset(daaSecondary, 0, sizeof(daaSecondary));
            for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
                uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
                int32_t primaryDataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
                int32_t secondaryDataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).SecondaryDataIndex;

                if (address <= 16) {
                    saaPrimary[address - 1] = this->appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                } else {
                    daaPrimary[address - 17] = this->appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                    daaSecondary[address - 17] = this->appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex];
                }
            }
            this->buffer.setIndex(this->setForceCommandIndex[subnetIndex]);
            this->ilcMessageFactory->broadcastForceDemand(&this->buffer, this->outerLoopData->broadcastCounter, this->forceActuatorState->slewFlag, saaPrimary, daaPrimary, daaSecondary);

            int32_t statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
            this->expectedFAResponses[dataIndex] = 1;
            this->roundRobinFAReportServerStatusIndex[subnetIndex] = RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
            statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            while (this->subnetData->getFAIndex(subnetIndex, statusIndex).Disabled) {
                this->roundRobinFAReportServerStatusIndex[subnetIndex] = RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
                statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            }
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, statusIndex).Address;
            dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;

            this->buffer.setIndex(this->faStatusCommandIndex[subnetIndex]);
            this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
            this->expectedFAResponses[dataIndex] = 2;
        }
        if (this->subnetData->getHPCount(subnetIndex) > 0) {
            int8_t steps[78];
            for (int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
                uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
                int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
                // Steps are swapped because negative steps extend and positive steps retract
                // This doesn't match what most people would expect so we are swapping it
                steps[address - 1] = -this->appliedHardpointSteps->commandedSteps[dataIndex];
            }
            this->buffer.setIndex(this->moveStepCommandIndex[subnetIndex]);
            this->ilcMessageFactory->broadcastStepMotor(&this->buffer, this->outerLoopData->broadcastCounter, steps);
        }
        if (this->subnetData->getHMCount(subnetIndex) > 0) {
            this->buffer.setIndex(this->hmLVDTCommandIndex[subnetIndex]);
            for (int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
                uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
                int32_t dataIndex = this->subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
                bool disabled = this->subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
                if (!disabled) {
                    if (this->lvdtSampleClock == 0) {
                        this->ilcMessageFactory->reportLVDT(&this->buffer, address);
                        this->expectedHMResponses[dataIndex] = 4;
                    } else {
                        this->ilcMessageFactory->nopReportLVDT(&this->buffer, address);
                        this->expectedHMResponses[dataIndex] = 3;
                    }
                }
            }
            this->lvdtSampleClock--;
            if (this->lvdtSampleClock < 0) {
                this->lvdtSampleClock = 4;
            }
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
