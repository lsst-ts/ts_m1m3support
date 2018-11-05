/*
 * IFPGA.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#include <IFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IFPGA::IFPGA() { }
IFPGA::~IFPGA() { }

SupportFPGAData* IFPGA::getSupportFPGAData() { return 0; }

int32_t IFPGA::initialize() { return 0; }
int32_t IFPGA::open() { return 0; }
int32_t IFPGA::close() { return 0; }
int32_t IFPGA::finalize() { return 0; }

bool IFPGA::isErrorCode(int32_t status) { return 0; }

int32_t IFPGA::waitForOuterLoopClock(int32_t timeout) { return 0; }
int32_t IFPGA::ackOuterLoopClock() { return 0; }

int32_t IFPGA::waitForPPS(int32_t timeout) { return 0; }
int32_t IFPGA::ackPPS() { return 0; }

int32_t IFPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) { return 0; }
int32_t IFPGA::ackModbusIRQ(int32_t subnet) { return 0; }

void IFPGA::pullTelemetry() { }

int32_t IFPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) { return 0; }
int32_t IFPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) { return 0; }
int32_t IFPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) { return 0; }
int32_t IFPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) { return 0; }
int32_t IFPGA::writeTimestampFIFO(uint64_t timestamp) { return 0; }
int32_t IFPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) { return 0; }
int32_t IFPGA::readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
