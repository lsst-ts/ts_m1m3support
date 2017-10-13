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
	this->u16Buffer[0] = 0;
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

int32_t FPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
	return NiFpga_WriteFifoU16(this->session, NiFpga_M1M3Support_HostToTargetFifoU16_CommandFIFO, data, length, timeoutInMs, &this->remaining);
}

int32_t FPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) {
	this->u16Buffer[0] = data;
	return this->writeCommandFIFO(this->u16Buffer, 1, timeoutInMs);
}

int32_t FPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
	return NiFpga_WriteFifoU16(this->session, NiFpga_M1M3Support_HostToTargetFifoU16_RequestFIFO, data, length, timeoutInMs, &this->remaining);
}

int32_t FPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) {
	this->u16Buffer[0] = data;
	return this->writeRequestFIFO(this->u16Buffer, 1, timeoutInMs);
}

int32_t FPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) {
	return NiFpga_ReadFifoU8(this->session, NiFpga_M1M3Support_TargetToHostFifoU8_U8ResponseFIFO, data, length, timeoutInMs, &this->remaining);
}

int32_t FPGA::readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
	return NiFpga_ReadFifoU16(this->session, NiFpga_M1M3Support_TargetToHostFifoU16_U16ResponseFIFO, data, length, timeoutInMs, &this->remaining);
}

int32_t FPGA::readSGLFIFO(float* data, int32_t length, int32_t timeoutInMs) {
	return NiFpga_ReadFifoSgl(this->session, NiFpga_M1M3Support_TargetToHostFifoSgl_SGLResponseFIFO, data, length, timeoutInMs, &this->remaining);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
