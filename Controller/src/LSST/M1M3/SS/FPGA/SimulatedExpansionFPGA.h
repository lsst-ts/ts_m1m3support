/*
 * SimulatedExpansionFPGA.h
 *
 *  Created on: Nov 1, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FPGA_SIMULATEDEXPANSIONFPGA_H_
#define LSST_M1M3_SS_FPGA_SIMULATEDEXPANSIONFPGA_H_

#include <NiFpga.h>
#include <IExpansionFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class SimulatedExpansionFPGA: public IExpansionFPGA {
public:
	SimulatedExpansionFPGA();

	int32_t initialize();
	int32_t open();
	int32_t close();
	int32_t finalize();

	bool isErrorCode(int32_t status);

	int32_t sample();

	int32_t readSlot1(float* data);
	int32_t readSlot2(uint32_t* data);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_SIMULATEDEXPANSIONFPGA_H_ */
