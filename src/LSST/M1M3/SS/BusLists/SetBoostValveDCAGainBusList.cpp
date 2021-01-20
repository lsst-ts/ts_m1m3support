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

#include <SetBoostValveDCAGainBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <M1M3SSPublisher.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SetBoostValveDCAGainBusList::SetBoostValveDCAGainBusList(ILCSubnetData* subnetData,
                                                         ILCMessageFactory* ilcMessageFactory)
        : BusList(subnetData, ilcMessageFactory) {
    SPDLOG_DEBUG("SetBoostValveDCAGainBusList: SetBoostValveDCAGainBusList()");
    MTM1M3_logevent_forceActuatorInfoC* forceInfo = M1M3SSPublisher::get().getEventForceActuatorInfo();
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->startSubnet(subnetIndex);
        for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
            bool disabled = this->subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->setBoostValveDCAGains(
                        &this->buffer, address, forceInfo->mezzaninePrimaryCylinderGain[dataIndex],
                        forceInfo->mezzanineSecondaryCylinderGain[dataIndex]);
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
