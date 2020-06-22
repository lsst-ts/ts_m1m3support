/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <FreezeSensorBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <RoundRobin.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FreezeSensorBusList::FreezeSensorBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory,
                                         MTM1M3_outerLoopDataC* outerLoopData)
        : BusList(subnetData, ilcMessageFactory) {
    spdlog::debug("FreezeSensorBusList: FreezeSensorBusList()");
    this->outerLoopData = outerLoopData;
    this->lvdtSampleClock = 0;
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->freezeSensorCommandIndex[subnetIndex] = -1;
        this->faStatusCommandIndex[subnetIndex] = -1;
        this->roundRobinFAReportServerStatusIndex[subnetIndex] = 0;
    }
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->startSubnet(subnetIndex);
        if (this->subnetData->getFACount(subnetIndex) > 0) {
            this->freezeSensorCommandIndex[subnetIndex] = this->buffer.getIndex();
            this->ilcMessageFactory->broadcastPneumaticFreezeSensorValues(
                    &this->buffer, this->outerLoopData->broadcastCounter);
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
                this->roundRobinFAReportServerStatusIndex[subnetIndex] =
                        RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
                statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            }
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, statusIndex).Address;
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
            this->faStatusCommandIndex[subnetIndex] = this->buffer.getIndex();
            this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
            this->expectedFAResponses[dataIndex] = 2;
        }
        if (this->subnetData->getHPCount(subnetIndex) > 0) {
            this->freezeSensorCommandIndex[subnetIndex] = this->buffer.getIndex();
            this->ilcMessageFactory->broadcastElectromechanicalFreezeSensorValues(
                    &this->buffer, this->outerLoopData->broadcastCounter);
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
        if (this->subnetData->getHMCount(subnetIndex) > 0) {
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

void FreezeSensorBusList::update() {
    this->outerLoopData->broadcastCounter =
            RoundRobin::BroadcastCounter(this->outerLoopData->broadcastCounter);
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->buffer.setIndex(this->freezeSensorCommandIndex[subnetIndex]);
        if (this->subnetData->getFACount(subnetIndex) > 0) {
            this->ilcMessageFactory->broadcastPneumaticFreezeSensorValues(
                    &this->buffer, this->outerLoopData->broadcastCounter);
        } else if (this->subnetData->getHPCount(subnetIndex) > 0) {
            this->ilcMessageFactory->broadcastElectromechanicalFreezeSensorValues(
                    &this->buffer, this->outerLoopData->broadcastCounter);
        }
        if (this->subnetData->getFACount(subnetIndex) > 0) {
            int32_t statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
            this->expectedFAResponses[dataIndex] = 1;
            this->roundRobinFAReportServerStatusIndex[subnetIndex] =
                    RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
            statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            while (this->subnetData->getFAIndex(subnetIndex, statusIndex).Disabled) {
                this->roundRobinFAReportServerStatusIndex[subnetIndex] =
                        RoundRobin::Inc(statusIndex, this->subnetData->getFACount(subnetIndex));
                statusIndex = this->roundRobinFAReportServerStatusIndex[subnetIndex];
            }
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, statusIndex).Address;
            dataIndex = this->subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;

            this->buffer.setIndex(this->faStatusCommandIndex[subnetIndex]);
            this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
            this->expectedFAResponses[dataIndex] = 2;
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
