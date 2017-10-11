/*
 * RS232.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <RS232.h>
#include <IFPGA.h>
#include <IPublisher.h>
#include <iostream>
#include <U8ArrayUtilities.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

RS232::RS232(IFPGA* fpga, IPublisher* publisher)
	: displacement(publisher, fpga) {
	this->fpga = fpga;
	this->publisher = publisher;
	this->inclinometer = Inclinometer(fpga, publisher);
	this->rxBufferIndex = 0;
}

void RS232::writeDisplacementRequest() {
	this->displacement.writeDataRequest();
}

void RS232::writeInclinometerRequest() {
	this->inclinometer.writeDataRequest();
}

void RS232::read(int32_t port) {
	this->fpga->writeRS232RxCommandFIFO(port, 0);
	this->fpga->readRS232RxFIFO(this->rxBuffer, 3, 10);
	uint8_t gotPort = this->rxBuffer[0];
	uint16_t length = U8ArrayUtilities::u16(this->rxBuffer, 1);
	for(int i = 0; i < 3; i++) {
		cout << (int32_t)this->rxBuffer[i] << " ";
	}
	cout << endl;
	if (length != 0) {
		this->fpga->readRS232RxFIFO(this->rxBuffer, length, 10);
		for(int i = 0; i < length; i++) {
			cout << (int32_t)this->rxBuffer[i] << " ";
		}
		cout << endl;
		this->rxBufferIndex = 0;
		switch(gotPort) {
		case 1: this->displacement.readDataResponse(this->rxBuffer, &this->rxBufferIndex); break;
		case 2: this->inclinometer.readDataResponse(this->rxBuffer, &this->rxBufferIndex); break;
		default: cout << "Unknown Port " << (int32_t)this->rxBuffer[this->rxBufferIndex - 1] << endl; break;
		}
	}
	else {
		switch(port) {
		case 1:
		case 2:
		default: cout << "RS232::read() - Nothing Available" << endl; break;
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
