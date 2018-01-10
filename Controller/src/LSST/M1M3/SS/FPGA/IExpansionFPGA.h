/*
 * IExpansionFPGA.h
 *
 *  Created on: Jan 9, 2018
 *      Author: ccontaxis
 */

#ifndef IEXPANSIONFPGA_H_
#define IEXPANSIONFPGA_H_

#include <NiFpga.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IExpansionFPGA {
public:
	virtual ~IExpansionFPGA();

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
	 * Asks the expansion FPGA to sample data.
	 */
	virtual int32_t sample();

	/*!
	 * Reads the power supply current sample data.
	 * @param[in] data An array of 6 floats to store the data in.
	 */
	virtual int32_t readSlot1(float* data);
	/*!
	 * Reads the power supply status sample data.
	 * @param[in] data A list of bitflags
	 */
	virtual int32_t readSlot2(uint32_t* data);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IEXPANSIONFPGA_H_ */
