/*
 * FPGA.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef FPGA_H_
#define FPGA_H_

#include <IFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The class used to communicate with the FPGA.
 */
class FPGA : public IFPGA {
private:
	uint32_t session;
	size_t remaining;
	uint8_t junkU8[1];
	uint16_t junkU16[1];
	uint64_t junkU64[1];
	NiFpga_IrqContext outerLoopIRQContext;
	NiFpga_IrqContext modbusIRQContext;

public:
	FPGA();
	virtual ~FPGA();

	int32_t initialize();
	int32_t open();
	int32_t close();
	int32_t finalize();

	bool isErrorCode(int32_t status);

	int32_t waitForOuterLoopClock(int32_t timeout);
	int32_t ackOuterLoopClock();

	int32_t writeGPCommandFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);
	int32_t writeGPRequestFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);
	int32_t readGPResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);

	int32_t writeHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout);
	int32_t readHealthAndStatusFIFOAvailable();
	int32_t readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout);

	int32_t writeRS232TxFIFO(uint8_t* data, int32_t length, int32_t timeout);
	int32_t writeRS232RxCommandFIFO(uint8_t data, int32_t timeout);
	int32_t readRS232RxFIFOAvailable();
	int32_t readRS232RxFIFO(uint8_t* data, int32_t length, int32_t timeout);

	void triggerModbus();
	int32_t writeModbusTxFIFO(uint16_t* data, int32_t length, int32_t timeout);
	int32_t writeModbusRxCommandFIFO(uint8_t data, int32_t timeout);
	int32_t readModbusRxFIFOAvailable();
	int32_t readModbusRxFIFO(uint16_t* data, int32_t length, int32_t timeout);

	int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout);
	int32_t ackModbusIRQ(int32_t subnet);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FPGA_H_ */
