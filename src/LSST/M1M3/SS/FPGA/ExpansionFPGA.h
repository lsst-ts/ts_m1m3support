#ifndef EXPANSIONFPGA_H_
#define EXPANSIONFPGA_H_

#include <IExpansionFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ExpansionFPGA : public IExpansionFPGA {
public:
    ExpansionFPGA();

    int32_t initialize() override;
    int32_t open() override;
    int32_t close() override;
    int32_t finalize() override;

    bool isErrorCode(int32_t status) override;

    int32_t sample() override;

    int32_t readSlot1(float* data) override;
    int32_t readSlot2(uint32_t* data) override;

private:
    uint32_t session;
    size_t remaining;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXPANSIONFPGA_H_ */
