/*
 * BusList.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#include <BusList.h>
#include <FPGAAddresses.h>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

BusList::BusList() {
	this->subnetData =0;
	this->ilcMessageFactory =  0;
	this->subnetStartIndex = 0;
}

BusList::BusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory) {
	this->subnetData = subnetData;
	this->ilcMessageFactory = ilcMessageFactory;
	memset(this->expectedHPResponses, 0, sizeof(this->expectedHPResponses));
	memset(this->expectedFAResponses, 0, sizeof(this->expectedFAResponses));
	this->subnetStartIndex = 0;
}

void BusList::startSubnet(uint8_t subnet) {
	switch(subnet) {
	case 0: subnet = FPGAAddresses::ModbusSubnetATx; break;
	case 1: subnet = FPGAAddresses::ModbusSubnetBTx; break;
	case 2: subnet = FPGAAddresses::ModbusSubnetCTx; break;
	case 3: subnet = FPGAAddresses::ModbusSubnetDTx; break;
	case 4: subnet = FPGAAddresses::ModbusSubnetETx; break;
	default: subnet = 255; break;
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
