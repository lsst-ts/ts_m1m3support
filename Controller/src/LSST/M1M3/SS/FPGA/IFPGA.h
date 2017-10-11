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

/*!
 * An interface for accessing FPGA functionality.
 */
class IFPGA {
public:
	virtual ~IFPGA();

	/*!
	 * Initializes the FPGA library.
	 * @return Status code.
	 */
	virtual int32_t initialize();
	/*!
	 * Opens the connection to the FPGA.
	 * @return Status code.
	 */
	virtual int32_t open();
	/*!
	 * Closes the connection to the FPGA.
	 * @return Status code.
	 */
	virtual int32_t close();
	/*!
	 * Finalizes the FPGA library.
	 * @return Status code.
	 */
	virtual int32_t finalize();

	/*!
	 * Returns true if the provided status is an error code.
	 * @return True if the status is an error code.
	 */
	virtual bool isErrorCode(int32_t status);

	/*!
	 * Waits for the outer loop clock IRQ to trigger or for the
	 * timeout to occur.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t waitForOuterLoopClock(int32_t timeout);
	/*!
	 * Acknowledges the outer loop clock IRQ allowing the clock
	 * to continue.
	 * @return Status code.
	 */
	virtual int32_t ackOuterLoopClock();

	/*!
	 * Writes data to the general purpose command FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeGPCommandFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Writes data to the general purpose request FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeGPRequestFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Reads data from the general purpose response FIFO.
	 * @param[in] data The buffer to put the data read from the FIFO into.
	 * @param[in] length The number of elements to read.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readGPResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Writes data to the health and status FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The length of the buffer.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout);
	/*!
	 * Reads the number of elements that are available in the health and status Rx FIFO.
	 * @return The number of elements available.
	 */
	virtual int32_t readHealthAndStatusFIFOAvailable();
	/*!
	 * Reads data from the health and status Rx FIFO.
	 * @param[in] data The buffer to put the data read from the FIFO into.
	 * @param[in] length The number of elements to read.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeout);

	/*!
	 * Writes data to the RS232 Tx FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The length of the buffer.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeRS232TxFIFO(uint8_t* data, int32_t length, int32_t timeout);
	/*!
	 * Writes data to the RS232 Rx command FIFO.
	 * @param[in] data The port to request data from.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeRS232RxCommandFIFO(uint8_t data, int32_t timeout);
	/*!
	 * Reads the number of elements that are available in the RS232 Rx FIFO.
	 * @return The number of elements available.
	 */
	virtual int32_t readRS232RxFIFOAvailable();
	/*!
	 * Reads data from the RS232 Rx FIFO.
	 * @param[in] data The buffer to put the data read from the FIFO into.
	 * @param[in] length The number of elements to read.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readRS232RxFIFO(uint8_t* data, int32_t length, int32_t timeout);

	/*!
	 * Generate the software trigger for modbus communications.
	 */
	virtual void triggerModbus();
	/*!
	 * Writes data to the modbus Tx FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The length of the buffer.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeModbusTxFIFO(uint16_t* data, int32_t length, int32_t timeout);
	/*!
	 * Writes data to the modbus Rx command FIFO.
	 * @param[in] data The port to request data from.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeModbusRxCommandFIFO(uint8_t data, int32_t timeout);
	/*!
	 * Reads the number of elements that are available in the modbus Rx FIFO.
	 * @return The number of elements available.
	 */
	virtual int32_t readModbusRxFIFOAvailable();
	/*!
	 * Reads data from the modbus Rx FIFO.
	 * @param[in] data The buffer to put the data read from the FIFO into.
	 * @param[in] length The number of elements to read.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readModbusRxFIFO(uint16_t* data, int32_t length, int32_t timeout);

	/*!
	 * Waits for the modbus IRQ to trigger or for the
	 * timeout to occur.
	 * @param[in] subnet The subnet to wait for.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout);
	/*!
	 * Acknowledges the modbus IRQ.
	 * @param[in] subnet The subnet to acknowledge.
	 * @return Status code.
	 */
	virtual int32_t ackModbusIRQ(int32_t subnet);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IFPGA_H_ */
