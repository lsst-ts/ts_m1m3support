/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <ReportServerStatusBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

ReportServerStatusBusList::ReportServerStatusBusList(ILCSubnetData* subnetData,
                                                     ILCMessageFactory* ilcMessageFactory)
        : BusList(subnetData, ilcMessageFactory) {
    SPDLOG_DEBUG("ReportServerStatusBusList: ReportServerStatusBusList()");
}

void ReportServerStatusBusList::buildBuffer() {
    BusList::buildBuffer();
    SPDLOG_DEBUG("ReportServerStatusBusList: buildBuffer()");

    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->startSubnet(subnetIndex);
        for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
            bool disabled = this->subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
                this->expectedFAResponses[dataIndex] = 1;
            }
        }
        for (int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
            uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
            int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
            bool disabled = this->subnetData->getHPIndex(subnetIndex, hpIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
                this->expectedHPResponses[dataIndex] = 1;
            }
        }
        for (int hmIndex = 0; hmIndex < this->subnetData->getHMCount(subnetIndex); hmIndex++) {
            uint8_t address = this->subnetData->getHMIndex(subnetIndex, hmIndex).Address;
            int32_t dataIndex = this->subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
            bool disabled = this->subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->reportServerStatus(&this->buffer, address);
                this->expectedHMResponses[dataIndex] = 1;
            }
        }
        this->endSubnet();
    }
    this->buffer.setLength(this->buffer.getIndex());
}
