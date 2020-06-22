/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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

#ifndef FPGA_H_
#define FPGA_H_

#include <NiFpga.h>
#include <IFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * The class used to communicate with the FPGA. Encapsulates various NiFpga
 * calls, primary dealing with FPGAs FIFOs.
 */
class FPGA : public IFPGA {
public:
    FPGA();
    virtual ~FPGA();

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
    void pullHealthAndStatus() override;

    int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t data, int32_t timeoutInMs) override;
    int32_t writeTimestampFIFO(uint64_t timestamp) override;
    int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) override;
    void readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;

    void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) override;
    int32_t readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeoutInMs = 10) override;

private:
    uint32_t session;
    size_t remaining;
    uint16_t u16Buffer[1];
    NiFpga_IrqContext outerLoopIRQContext;
    NiFpga_IrqContext modbusIRQContext;
    NiFpga_IrqContext ppsIRQContext;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FPGA_H_ */
