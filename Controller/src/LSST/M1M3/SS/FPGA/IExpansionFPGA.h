/*
 * IExpansionFPGA.h
 *
 *  Created on: Nov 1, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_
#define LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_

#include <NiFpga.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IExpansionFPGA {
   public:
    IExpansionFPGA();
    virtual ~IExpansionFPGA();

    virtual int32_t initialize();
    virtual int32_t open();
    virtual int32_t close();
    virtual int32_t finalize();

    virtual bool isErrorCode(int32_t status);

    virtual int32_t sample();

    virtual int32_t readSlot1(float* data);
    virtual int32_t readSlot2(uint32_t* data);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_ */
