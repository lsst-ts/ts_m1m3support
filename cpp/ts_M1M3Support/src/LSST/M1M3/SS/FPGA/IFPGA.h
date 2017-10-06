/*
 * IFPGA.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef IFPGA_H_
#define IFPGA_H_

#include <NiFpga.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IFPGA {
public:
	virtual ~IFPGA();

	virtual int32_t initialize();
	virtual int32_t open();
	virtual int32_t close();
	virtual int32_t finalize();

	virtual bool isErrorCode(int32_t status);

	virtual int32_t waitForOuterLoopClock(int32_t timeout);
	virtual int32_t ackOuterLoopClock();

	virtual int32_t writeHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout);
	virtual int32_t readHealthAndStatusFIFOAvailable();
	virtual int32_t readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout);

	virtual int32_t writeRS232TxFIFO(uint8_t* data, int32_t length, int32_t timeout);
	virtual int32_t readRS232RxFIFOAvailable();
	virtual int32_t readRS232RxFIFO(uint8_t* data, int32_t length, int32_t timeout);

	virtual void triggerModbus();
	virtual int32_t writeModbusTxFIFO(uint16_t* data, int32_t length, int32_t timeout);
	virtual int32_t writeModbusRxCommandFIFO(uint8_t data, int32_t timeout);
	virtual int32_t readModbusRxFIFOAvailable();
	virtual int32_t readModbusRxFIFO(uint16_t* data, int32_t length, int32_t timeout);

	virtual int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout);
	virtual int32_t ackModbusIRQ(int32_t subnet);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IFPGA_H_ */
