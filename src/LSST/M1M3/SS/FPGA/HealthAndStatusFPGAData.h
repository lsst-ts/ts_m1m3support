#ifndef LSST_M1M3_SS_FPGA_HealthAndStatusFPGAData_H_
#define LSST_M1M3_SS_FPGA_HealthAndStatusFPGAData_H_

#include <stdint.h>

namespace LSST {
namespace M1M3 {
namespace SS {

const int PORTNUM = 5;

/**
 * A single modbus port data. Reads data from FIFO buffer, and provides access.
 */
class ModbusPort {
public:
    ModbusPort();

    /**
     * Fill values from FIFO buffer.
     */
    void fillFromFIFO(uint64_t *buffer);

    // flag's constants
    const uint64_t TxInternalFIFOOverflow = 0x01;

    uint64_t errorFlag;
    uint64_t txBytes;
    uint64_t txFrames;
    uint64_t rxBytes;
    uint64_t rxFrames;
};

/**
 * Health and status reader. Reads data from FIFO, updates local variables.
 */
class HealthAndStatusFPGAData {
public:
    HealthAndStatusFPGAData() {}

    /**
     * Reads data from FIFO, populates local variables.
     *
     * @throw NiError on FPGA/NI error
     */
    void refresh(uint64_t buffer[PORTNUM * PORT_COUNTNUM]);

    ModbusPort ports[PORTNUM];
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // !LSST_M1M3_SS_FPGA_HealthAndStatusFPGAData_H_
