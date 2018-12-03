/*
 * SimulatedExpansionFPGA.cpp
 *
 *  Created on: Nov 1, 2018
 *      Author: ccontaxis
 */

#include <SimulatedExpansionFPGA.h>
#include <cstdlib>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SimulatedExpansionFPGA::SimulatedExpansionFPGA() {
	Log.Debug("SimulatedExpansionFPGA: SimulatedExpansionFPGA()");

	for(int i = 0; i < RND_CNT; ++i) {
		this->rnd[i] = float((rand() % 2000) - 1000) / 1000.0;
	}
	this->rndIndex = 0;
}

float SimulatedExpansionFPGA::getRnd() {
	++this->rndIndex;
	if (this->rndIndex > RND_CNT) {
		this->rndIndex = 0;
	}
	return this->rnd[rndIndex];
}

int32_t SimulatedExpansionFPGA::initialize() {
	Log.Debug("SimulatedExpansionFPGA: initialize()");
	return 0;
}

int32_t SimulatedExpansionFPGA::open() {
	Log.Debug("SimulatedExpansionFPGA: open()");
	return 0;
}

int32_t SimulatedExpansionFPGA::close() {
	Log.Debug("SimulatedExpansionFPGA: close()");
	return 0;
}

int32_t SimulatedExpansionFPGA::finalize() {
	Log.Debug("SimulatedExpansionFPGA: finalize()");
	return 0;
}

bool SimulatedExpansionFPGA::isErrorCode(int32_t status) {
	return status != 0;
}

int32_t SimulatedExpansionFPGA::sample() {
	return 0;
}

int32_t SimulatedExpansionFPGA::readSlot1(float* data) {
	data[0] = 1.0 + this->getRnd() * 0.5;
	data[1] = 1.0 + this->getRnd() * 0.5;
	data[2] = 1.0 + this->getRnd() * 0.5;
	data[3] = 1.0 + this->getRnd() * 0.5;
	data[4] = 1.0 + this->getRnd() * 0.5;
	data[5] = 1.0 + this->getRnd() * 0.5;
	return 0;
}

int32_t SimulatedExpansionFPGA::readSlot2(uint32_t* data) {
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
