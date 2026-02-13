/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSST_M1M3_SS_FPGA_HealthAndStatusFPGAData_H_
#define LSST_M1M3_SS_FPGA_HealthAndStatusFPGAData_H_

#include <stdint.h>

namespace LSST {
namespace M1M3 {
namespace SS {

// number of Modbus ports
const int PORTNUM = 5;
// Number of counters/flags per port
const int PORT_COUNTNUM = 6;

/**
 * A single modbus port data. Reads data from FIFO buffer, and provides access.
 */
class ModbusPort {
public:
    ModbusPort();

    /**
     * Fill values from FIFO buffer.
     */
    void fillFromFIFO(uint64_t* buffer);

    // flag's constants
    const uint64_t TxInternalFIFOOverflow = 0x01;

    uint64_t errorFlag;
    uint64_t txBytes;
    uint64_t txFrames;
    uint64_t rxBytes;
    uint64_t rxFrames;
    uint64_t instructionCount;
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
