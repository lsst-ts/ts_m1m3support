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

#include <iomanip>
#include <iostream>
#include <thread>

#include <spdlog/spdlog.h>

#include <cRIO/NiError.h>

#include <FPGA.h>
#include <FPGAAddresses.h>
#include <Model.h>
#include <NiFpga_M1M3SupportFPGA.h>
#include <U8ArrayUtilities.h>
#include <unistd.h>

using namespace LSST::M1M3::SS;

FPGA::FPGA() {
    SPDLOG_DEBUG("FPGA: FPGA()");
    _session = 0;
    _remaining = 0;
    _outerLoopIRQContext = 0;
    _modbusIRQContext = 0;
    _ppsIRQContext = 0;

    _modbus_irqs = getIrq(1) | getIrq(2) | getIrq(3) | getIrq(4) | getIrq(5);
}

FPGA::~FPGA() {}

void FPGA::initialize() {
    SPDLOG_DEBUG("FPGA: initialize()");
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_Initialize());
}

void FPGA::open() {
    SPDLOG_DEBUG("FPGA: open()");
    NiOpen("/var/lib/M1M3support", NiFpga_M1M3SupportFPGA, "RIO0", NiFpga_OpenAttribute_NoRun, &(_session));

    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Download", NiFpga_Download(_session));
    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Reset", NiFpga_Reset(_session));
    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Run", NiFpga_Run(_session, 0));

    std::this_thread::sleep_for(std::chrono::seconds(1));
    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_ReserveIrqContext",
                       NiFpga_ReserveIrqContext(_session, &_outerLoopIRQContext));
    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_ReserveIrqContext",
                       NiFpga_ReserveIrqContext(_session, &_modbusIRQContext));
    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_ReserveIrqContext",
                       NiFpga_ReserveIrqContext(_session, &_ppsIRQContext));
}

void FPGA::close() {
    SPDLOG_DEBUG("FPGA: close()");
    for (auto c : _contexes) {
        NiFpga_UnreserveIrqContext(_session, c.second);
    }
    NiFpga_UnreserveIrqContext(_session, _outerLoopIRQContext);
    NiFpga_UnreserveIrqContext(_session, _modbusIRQContext);
    NiFpga_UnreserveIrqContext(_session, _ppsIRQContext);
    cRIO::NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Close", NiFpga_Close(_session, 0));
}

void FPGA::finalize() {
    SPDLOG_DEBUG("FPGA: finalize()");
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_Finalize());
}

void FPGA::waitForOuterLoopClock(uint32_t timeout) {
    uint32_t assertedIRQs = 0;
    uint8_t timedOut = false;
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_WaitOnIrqs(_session, _outerLoopIRQContext, NiFpga_Irq_0,
                                                              timeout, &assertedIRQs, &timedOut));
}

void FPGA::ackOuterLoopClock() {
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, NiFpga_Irq_0));
}

void FPGA::waitForPPS(uint32_t timeout) {
    uint32_t assertedIRQs = 0;
    NiFpga_Bool timedOut = NiFpga_False;
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_WaitOnIrqs(_session, _ppsIRQContext, NiFpga_Irq_10,
                                                              timeout, &assertedIRQs, &timedOut));
}

void FPGA::ackPPS() {
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, NiFpga_Irq_10));
}

void FPGA::waitForModbusIRQs(uint32_t warning_timeout, uint32_t error_timeout) {
    uint32_t irqs = _modbus_irqs;
    uint32_t warning_irqs = 0;

    auto start = std::chrono::steady_clock::now();
    auto now = start;
    auto warning_time = now + std::chrono::milliseconds(warning_timeout);
    auto end_error = now + std::chrono::milliseconds(error_timeout);
    uint32_t remaining = error_timeout;

    while (irqs != 0) {
        uint32_t asserted_irqs = 0;
        NiFpga_Bool timed_out = NiFpga_False;

        remaining = std::chrono::floor<std::chrono::milliseconds>(
                            std::chrono::duration_cast<std::chrono::milliseconds>(end_error - now))
                            .count();

        SPDLOG_TRACE("Waiting for modbus IRQs: {:06b}, timeout {} ms", irqs, remaining);

        cRIO::NiThrowError(
                "Waiting for Modbus IRQs",
                NiFpga_WaitOnIrqs(_session, _modbusIRQContext, irqs, remaining, &asserted_irqs, &timed_out));

        if (timed_out) {
            Model::instance().getSafetyController()->modbusIRQTimeout(remaining, irqs);
            return;
        }

        now = std::chrono::steady_clock::now();

        if (now >= end_error) {
            Model::instance().getSafetyController()->modbusIRQTimeout(error_timeout, irqs);
            return;
        }

        irqs &= ~asserted_irqs;

        if (warning_irqs == 0 && now >= warning_time) {
            warning_irqs = irqs;
        }
    }

    if (warning_irqs != 0) {
        SPDLOG_WARN(
                "Modbus IRQs triggered after {:.03f} ms - IRQs after warning time: "
                "{:06b}",
                std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1000.0f,
                warning_irqs);
    }
}

void FPGA::ackModbusIRQs() {
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, _modbus_irqs));
}

void FPGA::pullTelemetry() {
    SPDLOG_TRACE("FPGA: pullTelemetry()");
    cRIO::FPGA::writeRequestFIFO(FPGAAddresses::Telemetry, 0);
    uint16_t length;
    readU16ResponseFIFO(&length, 1, 20);
    uint8_t buffer[1024];
    readU8ResponseFIFO(buffer, length, 20);
    supportFPGAData.Reserved = U8ArrayUtilities::U64(buffer, 0);
    supportFPGAData.InclinometerTxBytes = U8ArrayUtilities::U64(buffer, 8);
    supportFPGAData.InclinometerRxBytes = U8ArrayUtilities::U64(buffer, 16);
    supportFPGAData.InclinometerTxFrames = U8ArrayUtilities::U64(buffer, 24);
    supportFPGAData.InclinometerRxFrames = U8ArrayUtilities::U64(buffer, 32);
    supportFPGAData.InclinometerErrorTimestamp = U8ArrayUtilities::U64(buffer, 40);
    supportFPGAData.InclinometerErrorCode = U8ArrayUtilities::U8(buffer, 48);
    supportFPGAData.InclinometerSampleTimestamp = U8ArrayUtilities::U64(buffer, 49);
    supportFPGAData.InclinometerAngleRaw = U8ArrayUtilities::I32(buffer, 57);
    supportFPGAData.DisplacementTxBytes = U8ArrayUtilities::U64(buffer, 61);
    supportFPGAData.DisplacementRxBytes = U8ArrayUtilities::U64(buffer, 69);
    supportFPGAData.DisplacementTxFrames = U8ArrayUtilities::U64(buffer, 77);
    supportFPGAData.DisplacementRxFrames = U8ArrayUtilities::U64(buffer, 85);
    supportFPGAData.DisplacementErrorTimestamp = U8ArrayUtilities::U64(buffer, 93);
    supportFPGAData.DisplacementErrorCode = U8ArrayUtilities::U8(buffer, 101);
    supportFPGAData.DisplacementSampleTimestamp = U8ArrayUtilities::U64(buffer, 102);
    supportFPGAData.DisplacementRaw1 = U8ArrayUtilities::I32(buffer, 110);
    supportFPGAData.DisplacementRaw2 = U8ArrayUtilities::I32(buffer, 114);
    supportFPGAData.DisplacementRaw3 = U8ArrayUtilities::I32(buffer, 118);
    supportFPGAData.DisplacementRaw4 = U8ArrayUtilities::I32(buffer, 122);
    supportFPGAData.DisplacementRaw5 = U8ArrayUtilities::I32(buffer, 126);
    supportFPGAData.DisplacementRaw6 = U8ArrayUtilities::I32(buffer, 130);
    supportFPGAData.DisplacementRaw7 = U8ArrayUtilities::I32(buffer, 134);
    supportFPGAData.DisplacementRaw8 = U8ArrayUtilities::I32(buffer, 138);
    supportFPGAData.AccelerometerSampleCount = U8ArrayUtilities::U64(buffer, 142);
    supportFPGAData.AccelerometerSampleTimestamp = U8ArrayUtilities::U64(buffer, 150);
    for (int i = 0; i < 8; i++) {
        supportFPGAData.AccelerometerRaw[i] = U8ArrayUtilities::SGL(buffer, 158 + i * 4);
    }
    supportFPGAData.GyroTxBytes = U8ArrayUtilities::U64(buffer, 190);
    supportFPGAData.GyroRxBytes = U8ArrayUtilities::U64(buffer, 198);
    supportFPGAData.GyroTxFrames = U8ArrayUtilities::U64(buffer, 206);
    supportFPGAData.GyroRxFrames = U8ArrayUtilities::U64(buffer, 214);
    supportFPGAData.GyroErrorTimestamp = U8ArrayUtilities::U64(buffer, 222);
    supportFPGAData.GyroErrorCode = U8ArrayUtilities::U8(buffer, 230);
    supportFPGAData.GyroSampleTimestamp = U8ArrayUtilities::U64(buffer, 231);
    supportFPGAData.GyroRawX = U8ArrayUtilities::SGL(buffer, 239);
    supportFPGAData.GyroRawY = U8ArrayUtilities::SGL(buffer, 243);
    supportFPGAData.GyroRawZ = U8ArrayUtilities::SGL(buffer, 247);
    supportFPGAData.GyroStatus = U8ArrayUtilities::U8(buffer, 251);
    supportFPGAData.GyroSequenceNumber = U8ArrayUtilities::U8(buffer, 252);
    supportFPGAData.GyroTemperature = U8ArrayUtilities::I16(buffer, 253);
    supportFPGAData.GyroBITTimestamp = U8ArrayUtilities::U64(buffer, 255);
    supportFPGAData.GyroBIT0 = U8ArrayUtilities::U8(buffer, 263);
    supportFPGAData.GyroBIT1 = U8ArrayUtilities::U8(buffer, 264);
    supportFPGAData.GyroBIT2 = U8ArrayUtilities::U8(buffer, 265);
    supportFPGAData.GyroBIT3 = U8ArrayUtilities::U8(buffer, 266);
    supportFPGAData.GyroBIT4 = U8ArrayUtilities::U8(buffer, 267);
    supportFPGAData.GyroBIT5 = U8ArrayUtilities::U8(buffer, 268);
    supportFPGAData.GyroBIT6 = U8ArrayUtilities::U8(buffer, 269);
    supportFPGAData.GyroBIT7 = U8ArrayUtilities::U8(buffer, 270);
    supportFPGAData.DigitalInputSampleCount = U8ArrayUtilities::U64(buffer, 271);
    supportFPGAData.DigitalInputTimestamp = U8ArrayUtilities::U64(buffer, 279);
    supportFPGAData.DigitalInputStates = U8ArrayUtilities::U16(buffer, 287);
    supportFPGAData.DigitalOutputSampleCount = U8ArrayUtilities::U64(buffer, 289);
    supportFPGAData.DigitalOutputTimestamp = U8ArrayUtilities::U64(buffer, 297);
    supportFPGAData.DigitalOutputStates = U8ArrayUtilities::U8(buffer, 305);
    supportFPGAData.PowerSupplySampleCount = U8ArrayUtilities::U64(buffer, 306);
    supportFPGAData.PowerSupplyTimestamp = U8ArrayUtilities::U64(buffer, 314);
    supportFPGAData.PowerSupplyStates = U8ArrayUtilities::U8(buffer, 322);
}

void FPGA::pullHealthAndStatus() {
    writeHealthAndStatusFIFO(2, 0);
    uint64_t buffer[64];
    readHealthAndStatusFIFO(buffer, 64, 500);
    healthAndStatusFPGAData.refresh(buffer);
}

void FPGA::writeCommandFIFO(uint16_t *data, size_t length, uint32_t timeoutInMs) {
    cRIO::NiThrowError(__PRETTY_FUNCTION__,
                       NiFpga_WriteFifoU16(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_CommandFIFO,
                                           data, length, timeoutInMs, &_remaining));
}

void FPGA::writeRequestFIFO(uint16_t *data, size_t length, uint32_t timeoutInMs) {
    cRIO::NiThrowError(__PRETTY_FUNCTION__,
                       NiFpga_WriteFifoU16(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_RequestFIFO,
                                           data, length, timeoutInMs, &_remaining));
}

void FPGA::writeTimestampFIFO(uint64_t timestamp) {
    cRIO::NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_WriteFifoU64(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU64_TimestampControlFIFO,
                                &timestamp, 1, 0, &_remaining));
}

void FPGA::readU8ResponseFIFO(uint8_t *data, size_t length, uint32_t timeoutInMs) {
    cRIO::NiThrowError(__PRETTY_FUNCTION__,
                       NiFpga_ReadFifoU8(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoU8_U8ResponseFIFO,
                                         data, length, timeoutInMs, &_remaining));
}

void FPGA::readU16ResponseFIFO(uint16_t *data, size_t length, uint32_t timeoutInMs) {
    cRIO::NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadFifoU16(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoU16_U16ResponseFIFO, data,
                               length, timeoutInMs, &_remaining));
}

void FPGA::waitOnIrqs(uint32_t irqs, uint32_t timeout, bool &timedout, uint32_t *triggered) {
    static std::hash<std::thread::id> hasher;
    size_t k = hasher(std::this_thread::get_id());
    NiFpga_IrqContext contex;
    try {
        contex = _contexes.at(k);
    } catch (std::out_of_range &e) {
        NiFpga_ReserveIrqContext(_session, &contex);
        _contexes[k] = contex;
    }

    NiFpga_Bool ni_timedout = false;
    cRIO::NiThrowError(__PRETTY_FUNCTION__,
                       NiFpga_WaitOnIrqs(_session, contex, irqs, timeout, triggered, &ni_timedout));
    timedout = ni_timedout;
}

void FPGA::ackIrqs(uint32_t irqs) {
    cRIO::NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, irqs));
}

void FPGA::writeHealthAndStatusFIFO(uint16_t request, uint16_t param) {
    uint16_t data[2] = {request, param};
    cRIO::NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_WriteFifoU16(_session,
                                NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_HealthAndStatusControlFIFO, data,
                                2, 0, &_remaining));
}

void FPGA::readHealthAndStatusFIFO(uint64_t *data, size_t length, uint32_t timeoutInMs) {
    cRIO::NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadFifoU64(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoU64_HealthAndStatusDataFIFO,
                               data, length, timeoutInMs, &_remaining));
}

void FPGA::readRawAccelerometerFIFO(uint64_t *raw, size_t samples) {
    size_t _rawRemaining = 0;

    cRIO::NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadFifoU64(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoFxp_RawAccelerometer_Resource,
                               raw, 8 * samples, NiFpga_InfiniteTimeout, &_rawRemaining));
}
