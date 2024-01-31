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

#include <ILCMessageFactory.h>
#include <ILCSubnetData.h>
#include <SAL_MTM1M3C.h>
#include <SetADCChanneOffsetAndSensitivityBusList.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

SetADCChanneOffsetAndSensitivityBusList::SetADCChanneOffsetAndSensitivityBusList(
        ILCSubnetData *subnetData, ILCMessageFactory *ilcMessageFactory)
        : BusList(subnetData, ilcMessageFactory) {
    SPDLOG_DEBUG(
            "SetADCChanneOffsetAndSensitivityBusList: "
            "SetADCChanneOffsetAndSensitivityBusList()");
}

void SetADCChanneOffsetAndSensitivityBusList::buildBuffer() {
    BusList::buildBuffer();
    SPDLOG_DEBUG("SetADCChanneOffsetAndSensitivityBusList: buildBuffer()");

    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; ++subnetIndex) {
        startSubnet(subnetIndex);
        for (int faIndex = 0; faIndex < subnetData->getFACount(subnetIndex); ++faIndex) {
            uint8_t address = subnetData->getFAIndex(subnetIndex, faIndex).Address;
            int32_t dataIndex = subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
            bool disabled = subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
            if (!disabled) {
                ilcMessageFactory->setADCChannelOffsetAndSensitivity(&buffer, address, 1, 0, 0);
                ilcMessageFactory->setADCChannelOffsetAndSensitivity(&buffer, address, 2, 0, 0);
                expectedFAResponses[dataIndex] = 2;
            }
        }
        for (int hpIndex = 0; hpIndex < subnetData->getHPCount(subnetIndex); ++hpIndex) {
            uint8_t address = subnetData->getHPIndex(subnetIndex, hpIndex).Address;
            int32_t dataIndex = subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
            bool disabled = subnetData->getHPIndex(subnetIndex, hpIndex).Disabled;
            if (!disabled) {
                ilcMessageFactory->setADCChannelOffsetAndSensitivity(&buffer, address, 1, 0, 0);
                expectedHPResponses[dataIndex] = 1;
            }
        }
        endSubnet();
    }
    buffer.setLength(buffer.getIndex());
}
