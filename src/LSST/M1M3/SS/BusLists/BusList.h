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

#include <ModbusBuffer.h>
#include <ILCDataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ILCMessageFactory;
class ILCSubnetData;

/**
 * Abstract class. Holds list of ILCs on the bus. Allows CSC to query ILCs of
 * their status, command what is needed etc. This is achieved by storing
 * (caching) ModbusBuffer with required bytes, which is updated in update call
 * with new force demands etc.
 *
 * Buses 1-4 holds only Force Actuator (FA) ILCs. Bus 5 is reserved for
 * Hardpoint Actuator (HP).
 *
 * The major challenge in commanding ILCs on the buses is to keep messages
 * short and this system as fast as possible. Not all ILCs can be queried in a
 * single loop for all data.
 *
 * To speed up query build-up, FPGA transmitt buffer is prepared in buildBuffer
 * methed. It is then updated at specified parts in update method.
 *
 * Only required data are quieried in every loop. Other queries (e.g.
 * ServerState,..) are distributed, and only ILC subset is updated on every
 * loop. The subset moves in round-robin fashion.
 */
class BusList {
public:
    BusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);

    /**
     * (Re)-build message send to FPGA. Should be called when external
     * conditions changes - e.g. when an ILC is disabled.
     *
     * @note called from ILC::buildBusLists()
     */
    virtual void buildBuffer();

    int32_t getLength() { return this->buffer.getLength(); }
    uint16_t* getBuffer() { return this->buffer.getBuffer(); }

    int32_t* getExpectedHPResponses() { return this->expectedHPResponses; }
    int32_t* getExpectedFAResponses() { return this->expectedFAResponses; }
    int32_t* getExpectedHMResponses() { return this->expectedHMResponses; }

    /**
     * Called when update to buffer is required. Shall write new values to ILC
     * commands etc.
     */
    virtual void update() {}

protected:
    /**
     * Status of messages on a subnet.
     */
    ILCSubnetData* subnetData;
    ILCMessageFactory* ilcMessageFactory;

    /**
     * Buffer holding data send to FPGA Command FIFO.
     */
    ModbusBuffer buffer;

    // number of expected responses
    int32_t expectedHPResponses[HP_COUNT];
    int32_t expectedFAResponses[FA_COUNT];
    int32_t expectedHMResponses[HP_COUNT];
    int32_t subnetStartIndex;

    /**
     * Writes command to start a subnet message on the bus. Length is filled in endSubnet() method.
     */
    void startSubnet(uint8_t subnet);

    /**
     * Ends subnet. Writes IRQ trigger and sets buffer length.
     */
    void endSubnet();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* BUSLIST_H_ */
