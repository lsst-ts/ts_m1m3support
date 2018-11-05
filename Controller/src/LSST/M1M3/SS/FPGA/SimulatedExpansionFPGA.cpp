/*
 * SimulatedExpansionFPGA.cpp
 *
 *  Created on: Nov 1, 2018
 *      Author: ccontaxis
 */

#include <SimulatedExpansionFPGA.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SimulatedExpansionFPGA::SimulatedExpansionFPGA() {
	Log.Debug("SimulatedExpansionFPGA: SimulatedExpansionFPGA()");
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
	data[0] = 1.1;
	data[1] = 1.2;
	data[2] = 1.3;
	data[3] = 1.4;
	data[4] = 1.5;
	data[5] = 1.6;
	return 0;
}

int32_t SimulatedExpansionFPGA::readSlot2(uint32_t* data) {
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
