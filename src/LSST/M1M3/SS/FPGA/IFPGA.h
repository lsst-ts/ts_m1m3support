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

#ifndef LSST_M1M3_SS_FPGA_IFPGA_H_
#define LSST_M1M3_SS_FPGA_IFPGA_H_

#include <cRIO/FPGA.h>

#include <HealthAndStatusFPGAData.h>
#include <SupportFPGAData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abstract interface for FPGA. Both real FPGA and simulated class implements
 * this. Singleton.
 *
 * \emoji :exclamation: Please see LSST::M1M3::SS::FPGA class for communication
 * details.
 *
 * @see FPGA
 * @see https://github.com/lsst-ts/ts_m1m3SupportFPGA
 */
class IFPGA : public cRIO::FPGA {
public:
    IFPGA() : cRIO::FPGA(cRIO::SS) {}
    virtual ~IFPGA() {}

    static IFPGA &get();

    uint16_t getTxCommand(uint8_t bus) override;
    uint16_t getRxCommand(uint8_t bus) override;
    uint32_t getIrq(uint8_t bus) override;

    /**
     * Call after pullTelemetry to retrieve telemetry data.
     *
     * @return telemetry data
     */
    SupportFPGAData *getSupportFPGAData() { return &supportFPGAData; }

    /**
     * Call after pullHealthAndStatus to retrieve HealthAndStatus data.
     *
     * @return HealthAndStatus data
     */
    HealthAndStatusFPGAData *getHealthAndStatusFPGAData() { return &healthAndStatusFPGAData; }

    /**
     * Wait for outer loop clock interrupt for synchronization between C++ and
     * FPGA code. The interrupt (0) is raised every 20 ms inside FPGA code
     * (OuterLoop/OuterLoopClock.vi).
     *
     * @param timeout after that time (in millisecond) expires, and error will be
     * thrown
     *
     * @throw NiError on NI error
     *
     * @see writeTimestampFIFO
     */
    virtual void waitForOuterLoopClock(uint32_t timeout) = 0;

    /**
     * Acknowledge (clear interrupt 0) outer loop clock.
     *
     * @throw NiError on NI error
     */
    virtual void ackOuterLoopClock() = 0;

    /**
     * Waits for Peer-to-Peer Synchronization interrupt (10). The interrupt is
     * raised in Timestamp/Timestamp.vi and signals FPGA is ready to read peer
     * time offset from TimestampControlFIFO.
     *
     * @param timeout call timeout in milliseconds. TimeStamp VI introduces 1
     * milliseconds delay between successive waits for interrupt, so this value
     * shall be larger than 1000.
     *
     * @throw NiError on NI error
     */
    virtual void waitForPPS(uint32_t timeout) = 0;

    /**
     * Acknowledge (clear) Peer-to-Peer Synchronization interrupt.
     *
     * @throw NiError on NI error
     */
    virtual void ackPPS() = 0;

    /**
     * Wait for ModBus interrupts. The interrupt is generated when ModBus
     * command 0x7 is processed.
     *
     * @param warning_timeout warning timeout for IRQ call in milliseconds
     * @param error_timeout after that time (in millisecond) expires, and error
     * will be thrown
     *
     * @throw NiError on NI error
     * @throw std::runtime_error on timeout
     */
    virtual void waitForModbusIRQs(uint32_t warning_timeout, uint32_t error_timeout) = 0;

    /**
     * Acknowledge ModBus interrupt reception. Interrupt can be generated
     * (raised on the FPGA side) again after being acknowledged.
     *
     * @throw NiError on NI error
     */
    virtual void ackModbusIRQs() = 0;

    /**
     * Retrieve telemetry data.
     *
     * @throw NiError on NI error
     */
    virtual void pullTelemetry() = 0;

    /**
     * Retrieve Health&Status data.
     *
     * @throw NiError on NI error
     */
    virtual void pullHealthAndStatus() = 0;

    /**
     * Write current timestamp value into timestampFIFO. Shall be called after
     * PPS interrupt is received. See [NI forum for
     * details](https://forums.ni.com/t5/LabVIEW-Time-Sensitive/Time-Synchronization-FPGA-I-Os/gpm-p/3538972?profile.language=en).
     *
     * @param timestamp current system timestamp in nanoseconds
     *
     * @throw NiError on NI error
     */
    virtual void writeTimestampFIFO(uint64_t timestamp) = 0;

    /**
     *
     * @throw NiError on NI error
     */
    virtual void readU8ResponseFIFO(uint8_t *data, size_t length, uint32_t timeoutInMs) = 0;

    /**
     * Requests HealthAndStatus data. Response can be read calling
     * readHealthAndStatusFIFO method.
     *
     * Request types are:
     *
     * * **1** return single U64 value at address param
     * * **2** return full HealthAndStatus memory. Size is specified when
     *         creating HealthAndStatus memory.
     * * **3** clear health and status memory
     *
     * @param request request type, see above
     * @param param relevant only for request 1
     *
     * @throw NiError on NI error
     */
    virtual void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) = 0;

    /**
     * Copy HealthAndStatus data into supplied data buffer.
     *
     * @param data memory data will be copied
     * @param length buffer length. Must match FPGA defined memory size (64)
     * @param timeoutInMs timeout in microseconds
     *
     * @throw NiError on NI error
     */
    virtual void readHealthAndStatusFIFO(uint64_t *data, size_t length, uint32_t timeoutInMs = 10) = 0;

    /**
     * Clear raw accelerometer FIFO.
     */
    virtual void clearRawAccelerometerFIFO() = 0;

    /**
     * Reads raw accelerometer FIFO. Waits for new data if no data are available.
     *
     * @param raw 8 raw accelerometer values. Data are stored as 24 bits fixed
     * points (5 bits integer part) in uint64_t. So actually on 3 bytes out of
     * 64 are used - that copies NI interface for accessing fixed points
     * @param samples number of samples
     */
    virtual void readRawAccelerometerFIFO(uint64_t *raw, size_t samples) = 0;

    /**
     * Sets all auxiliary and network buses power (A-D).
     *
     * @param aux auxiliary buses power (true = on)
     * @param network network buses power (true = on)
     */
    void setPower(bool aux, bool network);

    /**
     * Sets aux and network power.
     *
     * @param aux[4] new auxiliar power, buses A-D
     * @param network[4] new network power, buses A-D
     */
    void setPower(const bool aux[4], const bool network[4]);

protected:
    SupportFPGAData supportFPGAData;
    HealthAndStatusFPGAData healthAndStatusFPGAData;

private:
    IFPGA &operator=(const IFPGA &) = delete;
    IFPGA(const IFPGA &) = delete;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IFPGA_H_ */
