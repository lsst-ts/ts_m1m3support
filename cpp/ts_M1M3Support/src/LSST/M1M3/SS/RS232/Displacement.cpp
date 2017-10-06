/*
 * Displacement.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#include <Displacement.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Displacement::Displacement() {
	this->createTxBuffer();
}

Displacement::~Displacement() {
	// TODO Auto-generated destructor stub
}

void Displacement::createTxBuffer() {
	this->txBuffer[0] = (uint8_t)'M';
	this->txBuffer[1] = (uint8_t)'0';
	this->txBuffer[2] = (uint8_t)'\r';
	this->txBuffer[3] = (uint8_t)'\n';
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
