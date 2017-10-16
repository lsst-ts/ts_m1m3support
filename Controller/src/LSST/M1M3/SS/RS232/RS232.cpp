/*
 * RS232.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <RS232.h>
#include <IFPGA.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RS232::RS232(IFPGA* fpga, IPublisher* publisher)
	: displacement(publisher, fpga),
	  inclinometer(publisher, fpga) {
}

void RS232::writeDisplacementRequest() {
	this->displacement.writeDataRequest();
}

void RS232::readDisplacementResponse() {
	this->displacement.readDataResponse();
}

void RS232::writeInclinometerRequest() {
	this->inclinometer.writeDataRequest();
}

void RS232::readInclinometerResponse() {
	this->inclinometer.readDataResponse();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
