/*
 * BusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
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
    int32_t expectedHMResponses[HM_COUNT];
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
