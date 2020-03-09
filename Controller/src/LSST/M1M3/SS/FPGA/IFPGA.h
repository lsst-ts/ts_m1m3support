/*
 * IFPGA.h
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FPGA_IFPGA_H_
#define LSST_M1M3_SS_FPGA_IFPGA_H_

#include <NiFpga.h>
#include <SupportFPGAData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IFPGA {
public:
	IFPGA();
	virtual ~IFPGA();

	virtual SupportFPGAData* getSupportFPGAData();

	virtual int32_t initialize();
	virtual int32_t open();
	virtual int32_t close();
	virtual int32_t finalize();

	virtual bool isErrorCode(int32_t status);

	virtual int32_t waitForOuterLoopClock(int32_t timeout);
	virtual int32_t ackOuterLoopClock();

	virtual int32_t waitForPPS(int32_t timeout);
	virtual int32_t ackPPS();

	virtual int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout);
	virtual int32_t ackModbusIRQ(int32_t subnet);

	virtual void pullTelemetry();

	virtual int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);
	virtual int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs);
	virtual int32_t writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);
	virtual int32_t writeRequestFIFO(uint16_t data, int32_t timeoutInMs);
	virtual int32_t writeTimestampFIFO(uint64_t timestamp);
	virtual int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);
	virtual int32_t readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IFPGA_H_ */
