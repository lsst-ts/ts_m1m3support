#ifndef LSST_M1M3_SS_FPGA_IFPGA_H_
#define LSST_M1M3_SS_FPGA_IFPGA_H_

#include <SupportFPGAData.h>
#include <HealthAndStatusFPGAData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abstract interface for FPGA. Both real FPGA and simulated class implements
 * this. Singleton.
 *
 * Methods throws NiError on error.
 */
class IFPGA {
public:
    IFPGA(){};
    virtual ~IFPGA(){};

    static IFPGA& get();

    SupportFPGAData* getSupportFPGAData() { return &supportFPGAData; }
    HealthAndStatusFPGAData* getHealthAndStatusFPGAData() { return &healthAndStatusFPGAData; }

    virtual int32_t initialize() = 0;
    virtual int32_t open() = 0;
    virtual int32_t close() = 0;
    virtual int32_t finalize() = 0;

    virtual bool isErrorCode(int32_t status) = 0;

    virtual int32_t waitForOuterLoopClock(int32_t timeout) = 0;
    virtual int32_t ackOuterLoopClock() = 0;

    virtual int32_t waitForPPS(int32_t timeout) = 0;
    virtual int32_t ackPPS() = 0;

    virtual int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout) = 0;
    virtual int32_t ackModbusIRQ(int32_t subnet) = 0;

    virtual void pullTelemetry() = 0;
    virtual void pullHealthAndStatus() = 0;

    virtual int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) = 0;
    virtual int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs) = 0;
    virtual void writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) = 0;
    virtual void writeRequestFIFO(uint16_t data, int32_t timeoutInMs) = 0;
    virtual int32_t writeTimestampFIFO(uint64_t timestamp) = 0;
    virtual int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) = 0;
    virtual int32_t readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) = 0;

    virtual void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) = 0;
    virtual int32_t readHealthAndStatusFIFO(uint64_t * data, int32_t length, int32_t timeoutInMs = 10) = 0;

protected:
    SupportFPGAData supportFPGAData;
    HealthAndStatusFPGAData healthAndStatusFPGAData;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IFPGA_H_ */
