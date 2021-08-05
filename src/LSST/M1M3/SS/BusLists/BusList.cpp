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

#include <BusList.h>
#include <FPGAAddresses.h>
#include <cstring>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

BusList::BusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory) {
    this->subnetData = subnetData;
    this->ilcMessageFactory = ilcMessageFactory;
}

void BusList::buildBuffer() {
    SPDLOG_DEBUG("BusList: buildBuffer()");
    memset(expectedHPResponses, 0, sizeof(expectedHPResponses));
    memset(expectedFAResponses, 0, sizeof(expectedFAResponses));
    memset(expectedHMResponses, 0, sizeof(expectedHMResponses));
    subnetStartIndex = 0;
    buffer.reset();
}

void BusList::startSubnet(uint8_t subnet) {
    switch (subnet) {
        case 0:
            subnet = FPGAAddresses::ModbusSubnetATx;
            break;
        case 1:
            subnet = FPGAAddresses::ModbusSubnetBTx;
            break;
        case 2:
            subnet = FPGAAddresses::ModbusSubnetCTx;
            break;
        case 3:
            subnet = FPGAAddresses::ModbusSubnetDTx;
            break;
        case 4:
            subnet = FPGAAddresses::ModbusSubnetETx;
            break;
        default:
            subnet = 255;
            break;
    }
    this->buffer.writeSubnet(subnet);
    this->subnetStartIndex = this->buffer.getIndex();
    this->buffer.writeLength(0);
    this->buffer.writeSoftwareTrigger();
}

void BusList::endSubnet() {
    this->buffer.writeTriggerIRQ();
    this->buffer.set(this->subnetStartIndex, this->buffer.getIndex() - this->subnetStartIndex - 1);
}
