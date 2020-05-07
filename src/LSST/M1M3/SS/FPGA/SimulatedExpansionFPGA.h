#ifndef LSST_M1M3_SS_FPGA_SIMULATEDEXPANSIONFPGA_H_
#define LSST_M1M3_SS_FPGA_SIMULATEDEXPANSIONFPGA_H_

#include <NiFpga.h>
#include <IExpansionFPGA.h>

#define RND_CNT 50

namespace LSST {
namespace M1M3 {
namespace SS {

class SimulatedExpansionFPGA : public IExpansionFPGA {
public:
    SimulatedExpansionFPGA();

    int32_t initialize() override;
    int32_t open() override;
    int32_t close() override;
    int32_t finalize() override;

    bool isErrorCode(int32_t status) override;

    int32_t sample() override;

    int32_t readSlot1(float* data) override;
    int32_t readSlot2(uint32_t* data) override;

private:
    float rnd[RND_CNT];
    int rndIndex;
    float getRnd();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_SIMULATEDEXPANSIONFPGA_H_ */
