/*
 * ReadBoostValveDCAGainBusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <ReadBoostValveDCAGainBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ReadBoostValveDCAGainBusList::ReadBoostValveDCAGainBusList(ILCSubnetData* subnetData,
                                                           ILCMessageFactory* ilcMessageFactory)
        : BusList(subnetData, ilcMessageFactory) {
    spdlog::debug("ReadBoostValveDCAGainBusList: ReadBoostValveDCAGainBusList()");
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->startSubnet(subnetIndex);
        for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
            bool disabled = this->subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->readBoostValveDCAGains(&this->buffer, address);
                this->expectedFAResponses[dataIndex] = 1;
            }
        }
        this->endSubnet();
    }
    this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
