/*
 * IFPGA.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <IFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IFPGA::~IFPGA() { }

int32_t IFPGA::initialize() { return 0; }

int32_t IFPGA::open() { return 0; }

int32_t IFPGA::close() { return 0; }

int32_t IFPGA::finalize() { return 0; }

bool IFPGA::isErrorCode(int32_t status) { return false; }

int32_t IFPGA::waitForOuterLoopClock(int32_t timeout) { return 0; }

int32_t IFPGA::ackOuterLoopClock() { return 0; }

int32_t IFPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) { return 0; }

int32_t IFPGA::ackModbusIRQ(int32_t subnet) { return 0; }

int32_t IFPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::readSGLResponseFIFO(float* data, int32_t length, int32_t timeoutInMs) { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
