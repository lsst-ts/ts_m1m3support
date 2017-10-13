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

	/*!
	 * Writes data to the command FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Writes data to the command FIFO.
	 * @param[in] data The data to write to the FIFO.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs);

	/*!
	 * Writes data to the request FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Writes data to the request FIFO.
	 * @param[in] data The data to write to the FIFO.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t writeRequestFIFO(uint16_t data, int32_t timeoutInMs);

	/*!
	 * Reads data from the U8 response FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Reads data from the U16 response FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);

	/*!
	 * Reads data from the SGL response FIFO.
	 * @param[in] data The buffer to write to the FIFO.
	 * @param[in] length The number of elements in the buffer to write.
	 * @param[in] timeoutInMs The timeout in ms. -1 is infinite.
	 * @return Status code.
	 */
	virtual int32_t readSGLFIFO(float* data, int32_t length, int32_t timeoutInMs);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IFPGA_H_ */
