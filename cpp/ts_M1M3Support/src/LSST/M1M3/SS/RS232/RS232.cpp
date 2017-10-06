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

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

RS232::RS232(IFPGA* fpga, IPublisher* publisher) {
	this->fpga = fpga;
	this->publisher = publisher;
	this->inclinometer = Inclinometer(fpga, publisher);
	this->createDisplacementTxBuffer();
	this->rxBufferIndex = 0;
}

void RS232::writeInclinometerRequest() {
	cout << "Writing Request" << endl;
	this->inclinometer.queryData();
}

void RS232::read() {
	cout << "Reading" << endl;
	int32_t available = this->fpga->readRS232RxFIFOAvailable();
	if (available > 3) {
		this->fpga->readRS232RxFIFO(this->rxBuffer, available, 10);
		for(int i = 0; i < available; i++) {
			cout << (int32_t)this->rxBuffer[i] << " ";
		}
		cout << endl;
		this->rxBufferIndex = 0;
		while(this->rxBufferIndex < available) {
			switch(this->rxBuffer[this->rxBufferIndex++]) {
			case 1: this->inclinometer.parseData(this->rxBuffer, &this->rxBufferIndex); break;
			default: cout << "Unknown Port " << (int32_t)this->rxBuffer[this->rxBufferIndex - 1] << endl;
			}
		}
	}
	else {
		cout << "RS232::read() - Nothing Available" << endl;
	}
}

void RS232::createDisplacementTxBuffer() {
	this->displacementTxBuffer[0] = 2;
	this->displacementTxBuffer[1] = 0;
	this->displacementTxBuffer[2] = 4;
	this->displacementTxBuffer[3] = (uint8_t)'M';
	this->displacementTxBuffer[4] = (uint8_t)'0';
	this->displacementTxBuffer[5] = (uint8_t)'\r';
	this->displacementTxBuffer[6] = (uint8_t)'\n';
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
