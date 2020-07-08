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

    virtual void initialize() = 0;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void finalize() = 0;

    virtual void waitForOuterLoopClock(int32_t timeout) = 0;
    virtual void ackOuterLoopClock() = 0;

    virtual void waitForPPS(int32_t timeout) = 0;
    virtual void ackPPS() = 0;

    virtual void waitForModbusIRQ(int32_t subnet, int32_t timeout) = 0;
    virtual void ackModbusIRQ(int32_t subnet) = 0;

    virtual void pullTelemetry() = 0;
    virtual void pullHealthAndStatus() = 0;

    virtual void writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) = 0;
    virtual void writeCommandFIFO(uint16_t data, int32_t timeoutInMs) = 0;
    virtual void writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) = 0;
    virtual void writeRequestFIFO(uint16_t data, int32_t timeoutInMs) = 0;
    virtual void writeTimestampFIFO(uint64_t timestamp) = 0;
    virtual void readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) = 0;
    virtual void readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) = 0;

    virtual void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) = 0;
    virtual void readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeoutInMs = 10) = 0;

protected:
    SupportFPGAData supportFPGAData;
    HealthAndStatusFPGAData healthAndStatusFPGAData;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IFPGA_H_ */
