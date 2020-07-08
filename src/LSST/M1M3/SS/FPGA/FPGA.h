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

    void initialize() override;
    void open() override;
    void close() override;
    void finalize() override;

    void waitForOuterLoopClock(int32_t timeout) override;
    void ackOuterLoopClock() override;

    void waitForPPS(int32_t timeout) override;
    void ackPPS() override;

    void waitForModbusIRQ(int32_t subnet, int32_t timeout) override;
    void ackModbusIRQ(int32_t subnet) override;

    void pullTelemetry() override;
    void pullHealthAndStatus() override;

    void writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    void writeCommandFIFO(uint16_t data, int32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t data, int32_t timeoutInMs) override;
    void writeTimestampFIFO(uint64_t timestamp) override;
    void readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) override;
    void readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;

    void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) override;
    void readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeoutInMs = 10) override;

private:
    uint32_t _session;
    size_t _remaining;
    NiFpga_IrqContext _outerLoopIRQContext;
    NiFpga_IrqContext _modbusIRQContext;
    NiFpga_IrqContext _ppsIRQContext;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FPGA_H_ */
