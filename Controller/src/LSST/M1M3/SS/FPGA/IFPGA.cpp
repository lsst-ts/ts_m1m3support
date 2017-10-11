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

int32_t IFPGA::writeGPCommandFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::writeGPRequestFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::readGPResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) { return 0; }

int32_t IFPGA::writeHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout) { return 0; }

int32_t IFPGA::readHealthAndStatusFIFOAvailable() { return 0; }

int32_t IFPGA::readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout) { return 0; }

int32_t IFPGA::writeRS232TxFIFO(uint8_t* data, int32_t length, int32_t timeout) { return 0; }

int32_t IFPGA::writeRS232RxCommandFIFO(uint8_t data, int32_t timeout) { return 0; }

int32_t IFPGA::readRS232RxFIFOAvailable() { return 0; }

int32_t IFPGA::readRS232RxFIFO(uint8_t* data, int32_t length, int32_t timeout) { return 0; }

void IFPGA::triggerModbus() { }

int32_t IFPGA::writeModbusTxFIFO(uint16_t* data, int32_t length, int32_t timeout) { return 0; }

int32_t IFPGA::writeModbusRxCommandFIFO(uint8_t data, int32_t timeout) { return 0; }

int32_t IFPGA::readModbusRxFIFOAvailable() { return 0; }

int32_t IFPGA::readModbusRxFIFO(uint16_t* data, int32_t length, int32_t timeout) { return 0; }

int32_t IFPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) { return 0; }

int32_t IFPGA::ackModbusIRQ(int32_t subnet) { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
