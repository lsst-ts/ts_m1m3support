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

#ifndef BUSLIST_H_
#define BUSLIST_H_

#include <IBusList.h>
#include <ModbusBuffer.h>
#include <ILCDataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ILCMessageFactory;
class ILCSubnetData;

class BusList : public IBusList {
protected:
    ILCSubnetData* subnetData;
    ILCMessageFactory* ilcMessageFactory;

    ModbusBuffer buffer;
    int32_t expectedHPResponses[HP_COUNT];
    int32_t expectedFAResponses[FA_COUNT];
    int32_t expectedHMResponses[HP_COUNT];
    int32_t subnetStartIndex;

public:
    BusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);

    int32_t getLength() { return this->buffer.getLength(); }
    uint16_t* getBuffer() { return this->buffer.getBuffer(); }

    int32_t* getExpectedHPResponses() { return this->expectedHPResponses; }
    int32_t* getExpectedFAResponses() { return this->expectedFAResponses; }
    int32_t* getExpectedHMResponses() { return this->expectedHMResponses; }

protected:
    void startSubnet(uint8_t subnet);
    void endSubnet();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* BUSLIST_H_ */
