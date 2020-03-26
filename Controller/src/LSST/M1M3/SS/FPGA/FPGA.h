#ifndef FPGA_H_
#define FPGA_H_

#include <NiFpga.h>
#include <IFPGA.h>
#include <SupportFPGAData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The class used to communicate with the FPGA.
 */
class FPGA : public IFPGA {
private:
    uint32_t session;
    size_t remaining;
    uint16_t u16Buffer[1];
    NiFpga_IrqContext outerLoopIRQContext;
    NiFpga_IrqContext modbusIRQContext;
    NiFpga_IrqContext ppsIRQContext;
    SupportFPGAData supportFPGAData;

public:
    FPGA();
    virtual ~FPGA();

    SupportFPGAData* getSupportFPGAData() override { return &this->supportFPGAData; }

    int32_t initialize() override;
    int32_t open() override;
    int32_t close() override;
    int32_t finalize() override;

    bool isErrorCode(int32_t status) override;

    int32_t waitForOuterLoopClock(int32_t timeout) override;
    int32_t ackOuterLoopClock() override;

    int32_t waitForPPS(int32_t timeout) override;
    int32_t ackPPS() override;

    int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout) override;
    int32_t ackModbusIRQ(int32_t subnet) override;

    void pullTelemetry() override;

    int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs) override;
    int32_t writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    int32_t writeRequestFIFO(uint16_t data, int32_t timeoutInMs) override;
    int32_t writeTimestampFIFO(uint64_t timestamp) override;
    int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) override;
    int32_t readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FPGA_H_ */
