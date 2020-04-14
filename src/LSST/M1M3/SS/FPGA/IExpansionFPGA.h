#ifndef LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_
#define LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_

#include <ExpansionFPGAApplicationSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abstract singleton class for expansion FPGA.
 */
class IExpansionFPGA {
public:
    IExpansionFPGA() { this->expansionFPGAApplicationSettings = NULL; };
    virtual ~IExpansionFPGA(){};

    static IExpansionFPGA& get();

    void setExpansionFPGAApplicationSettings(
            ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings) {
        this->expansionFPGAApplicationSettings = expansionFPGAApplicationSettings;
    }

    virtual int32_t initialize() = 0;
    virtual int32_t open() = 0;
    virtual int32_t close() = 0;
    virtual int32_t finalize() = 0;

    virtual bool isErrorCode(int32_t status) = 0;

    virtual int32_t sample() = 0;

    virtual int32_t readSlot1(float* data) = 0;
    virtual int32_t readSlot2(uint32_t* data) = 0;

protected:
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_ */
