/*
 * FPGA.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <FPGA.h>
#include <NiFpga_M1M3Support.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

FPGA::FPGA() {
	this->session = 0;
	this->remaining = 0;
	this->junkU8[0] = 0;
	this->junkU16[0] = 0;
	this->junkU64[0] = 0;
	this->outerLoopIRQContext = 0;
	this->modbusIRQContext = 0;
}

FPGA::~FPGA() {

}

int32_t FPGA::initialize() {
	return NiFpga_Initialize();
}

int32_t FPGA::open() {
	int32_t status = NiFpga_Open("/usr/ts_M1M3Support/"NiFpga_M1M3Support_Bitfile, NiFpga_M1M3Support_Signature, "RIO0", 0, &(this->session));
	status = NiFpga_Abort(this->session);
	status = NiFpga_Download(this->session);
	status = NiFpga_Reset(this->session);
	status = NiFpga_Run(this->session, 0);
	usleep(1000000);
	NiFpga_ReserveIrqContext(this->session, &this->outerLoopIRQContext);
	NiFpga_ReserveIrqContext(this->session, &this->modbusIRQContext);
	return status;
}

int32_t FPGA::close() {
	NiFpga_UnreserveIrqContext(this->session, this->outerLoopIRQContext);
	NiFpga_UnreserveIrqContext(this->session, this->modbusIRQContext);
	return NiFpga_Close(this->session, 0);
}

int32_t FPGA::finalize() {
	return NiFpga_Finalize();
}

bool FPGA::isErrorCode(int32_t status) {
	return NiFpga_IsError(status);
}

int32_t FPGA::waitForOuterLoopClock(int32_t timeout) {
	uint32_t assertedIRQs = 0;
	uint8_t timedOut = false;
	int32_t result = NiFpga_WaitOnIrqs(this->session, this->outerLoopIRQContext, NiFpga_Irq_0, timeout, &assertedIRQs, &timedOut);
	return result;
}

int32_t FPGA::ackOuterLoopClock() {
	return NiFpga_AcknowledgeIrqs(this->session, NiFpga_Irq_0);
}

int32_t FPGA::writeHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout) {
	return NiFpga_WriteFifoU64(this->session, NiFpga_M1M3Support_HostToTargetFifoU64_HealthAndStatusControlFIFO, data, length, timeout, &(this->remaining));
}

int32_t FPGA::readHealthAndStatusFIFOAvailable() {
	this->readHealthAndStatusFIFO(this->junkU64, 0, 0);
	return (int32_t)this->remaining;
}

int32_t FPGA::readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout) {
	return NiFpga_ReadFifoU64(this->session, NiFpga_M1M3Support_TargetToHostFifoU64_HealthAndStatusDataFIFO, data, length, timeout, &(this->remaining));
}

int32_t FPGA::writeRS232TxFIFO(uint8_t* data, int32_t length, int32_t timeout) {
	return NiFpga_WriteFifoU8(this->session, NiFpga_M1M3Support_HostToTargetFifoU8_RS232TxFIFO, data, length, timeout, &(this->remaining));
}

int32_t FPGA::readRS232RxFIFOAvailable() {
	this->readRS232RxFIFO(this->junkU8, 0, 0);
	return (int32_t)this->remaining;
}

int32_t FPGA::readRS232RxFIFO(uint8_t* data, int32_t length, int32_t timeout) {
	return NiFpga_ReadFifoU8(this->session, NiFpga_M1M3Support_TargetToHostFifoU8_RS232RxFIFO, data, length, timeout, &(this->remaining));
}

void FPGA::triggerModbus() {
	NiFpga_WriteBool(this->session, NiFpga_M1M3Support_ControlBool_ModbusTrigger, NiFpga_True);
	usleep(1000);
	NiFpga_WriteBool(this->session, NiFpga_M1M3Support_ControlBool_ModbusTrigger, NiFpga_False);
}

int32_t FPGA::writeModbusTxFIFO(uint16_t* data, int32_t length, int32_t timeout) {
	return NiFpga_WriteFifoU16(this->session, NiFpga_M1M3Support_HostToTargetFifoU16_ModbusTxDataFIFO, data, length, timeout, &(this->remaining));
}

int32_t FPGA::writeModbusRxCommandFIFO(uint8_t data, int32_t timeout) {
	this->junkU8[0] = data;
	return NiFpga_WriteFifoU8(this->session, NiFpga_M1M3Support_HostToTargetFifoU8_ModbusRxCommandFIFO, this->junkU8, 1, timeout, &(this->remaining));
}

int32_t FPGA::readModbusRxFIFOAvailable() {
	this->readModbusRxFIFO(this->junkU16, 0, 0);
	return (int32_t)this->remaining;
}

int32_t FPGA::readModbusRxFIFO(uint16_t* data, int32_t length, int32_t timeout) {
	return NiFpga_ReadFifoU16(this->session, NiFpga_M1M3Support_TargetToHostFifoU16_ModbusRxDataFIFO, data, length, timeout, &(this->remaining));
}

int32_t FPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) {
	uint32_t irqs = 0;
	switch(subnet) {
	case 1: irqs = NiFpga_Irq_1; break;
	case 2: irqs = NiFpga_Irq_2; break;
	case 3: irqs = NiFpga_Irq_3; break;
	case 4: irqs = NiFpga_Irq_4; break;
	case 5: irqs = NiFpga_Irq_5; break;
	default: break;
	}
	if (irqs != 0) {
		uint32_t assertedIRQs = 0;
		uint8_t timedOut = false;
		int32_t result = NiFpga_WaitOnIrqs(this->session, this->modbusIRQContext, irqs, timeout, &assertedIRQs, &timedOut);
		return result;
	}
	return 0;
}

int32_t FPGA::ackModbusIRQ(int32_t subnet) {
	uint32_t irqs = 0;
	switch(subnet) {
	case 1: irqs = NiFpga_Irq_1; break;
	case 2: irqs = NiFpga_Irq_2; break;
	case 3: irqs = NiFpga_Irq_3; break;
	case 4: irqs = NiFpga_Irq_4; break;
	case 5: irqs = NiFpga_Irq_5; break;
	default: break;
	}
	if (irqs != 0) {
		return NiFpga_AcknowledgeIrqs(this->session, irqs);
	}
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
