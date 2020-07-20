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

#include <FPGA.h>
#include <NiFpga_M1M3SupportFPGA.h>
#include <NiError.h>
#include <unistd.h>
#include <U8ArrayUtilities.h>
#include <FPGAAddresses.h>

#include <iostream>
#include <iomanip>
#include <thread>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FPGA::FPGA() {
    spdlog::debug("FPGA: FPGA()");
    _session = 0;
    _remaining = 0;
    _outerLoopIRQContext = 0;
    _modbusIRQContext = 0;
    _ppsIRQContext = 0;
}

FPGA::~FPGA() {}

void FPGA::initialize() {
    spdlog::debug("FPGA: initialize()");
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_Initialize());
}

void FPGA::open() {
    spdlog::debug("FPGA: open()");
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Open",
                 NiFpga_Open("/home/admin/Bitfiles/" NiFpga_M1M3SupportFPGA_Bitfile,
                             NiFpga_M1M3SupportFPGA_Signature, "RIO0", 0, &(_session)));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Abort", NiFpga_Abort(_session));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Download", NiFpga_Download(_session));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Reset", NiFpga_Reset(_session));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Run", NiFpga_Run(_session, 0));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_ReserveIrqContext",
                 NiFpga_ReserveIrqContext(_session, &_outerLoopIRQContext));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_ReserveIrqContext",
                 NiFpga_ReserveIrqContext(_session, &_modbusIRQContext));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_ReserveIrqContext",
                 NiFpga_ReserveIrqContext(_session, &_ppsIRQContext));
}

void FPGA::close() {
    spdlog::debug("FPGA: close()");
    NiFpga_UnreserveIrqContext(_session, _outerLoopIRQContext);
    NiFpga_UnreserveIrqContext(_session, _modbusIRQContext);
    NiFpga_UnreserveIrqContext(_session, _ppsIRQContext);
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Close", NiFpga_Close(_session, 0));
}

void FPGA::finalize() {
    spdlog::debug("FPGA: finalize()");
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_Finalize());
}

void FPGA::waitForOuterLoopClock(int32_t timeout) {
    uint32_t assertedIRQs = 0;
    uint8_t timedOut = false;
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_WaitOnIrqs(_session, _outerLoopIRQContext, NiFpga_Irq_0, timeout,
                                                        &assertedIRQs, &timedOut));
}

void FPGA::ackOuterLoopClock() {
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, NiFpga_Irq_0));
}

void FPGA::waitForPPS(int32_t timeout) {
    uint32_t assertedIRQs = 0;
    uint8_t timedOut = false;
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_WaitOnIrqs(_session, _ppsIRQContext, NiFpga_Irq_10, timeout,
                                                        &assertedIRQs, &timedOut));
}

void FPGA::ackPPS() { NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, NiFpga_Irq_10)); }

void FPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) {
    uint32_t irqs = 0;
    switch (subnet) {
        case 1:
            irqs = NiFpga_Irq_1;
            break;
        case 2:
            irqs = NiFpga_Irq_2;
            break;
        case 3:
            irqs = NiFpga_Irq_3;
            break;
        case 4:
            irqs = NiFpga_Irq_4;
            break;
        case 5:
            irqs = NiFpga_Irq_5;
            break;
        default:
            break;
    }
    if (irqs != 0) {
        uint32_t assertedIRQs = 0;
        uint8_t timedOut = false;
        NiThrowError(__PRETTY_FUNCTION__,
                     NiFpga_WaitOnIrqs(_session, _modbusIRQContext, irqs, timeout, &assertedIRQs, &timedOut));
    }
}

void FPGA::ackModbusIRQ(int32_t subnet) {
    uint32_t irqs = 0;
    switch (subnet) {
        case 1:
            irqs = NiFpga_Irq_1;
            break;
        case 2:
            irqs = NiFpga_Irq_2;
            break;
        case 3:
            irqs = NiFpga_Irq_3;
            break;
        case 4:
            irqs = NiFpga_Irq_4;
            break;
        case 5:
            irqs = NiFpga_Irq_5;
            break;
        default:
            break;
    }
    if (irqs != 0) {
        NiThrowError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, irqs));
    }
}

void FPGA::pullTelemetry() {
    spdlog::trace("FPGA: pullTelemetry()");
    writeRequestFIFO(FPGAAddresses::Telemetry, 0);
    uint16_t length[1];
    readU16ResponseFIFO(length, 1, 20);
    uint8_t buffer[1024];
    readU8ResponseFIFO(buffer, (int32_t)length[0], 20);
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
    supportFPGAData.AccelerometerRaw1 = U8ArrayUtilities::SGL(buffer, 158);
    supportFPGAData.AccelerometerRaw2 = U8ArrayUtilities::SGL(buffer, 162);
    supportFPGAData.AccelerometerRaw3 = U8ArrayUtilities::SGL(buffer, 166);
    supportFPGAData.AccelerometerRaw4 = U8ArrayUtilities::SGL(buffer, 170);
    supportFPGAData.AccelerometerRaw5 = U8ArrayUtilities::SGL(buffer, 174);
    supportFPGAData.AccelerometerRaw6 = U8ArrayUtilities::SGL(buffer, 178);
    supportFPGAData.AccelerometerRaw7 = U8ArrayUtilities::SGL(buffer, 182);
    supportFPGAData.AccelerometerRaw8 = U8ArrayUtilities::SGL(buffer, 186);
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

void FPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_WriteFifoU16(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_CommandFIFO, data,
                                     length, timeoutInMs, &_remaining));
}

void FPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) { writeCommandFIFO(&data, 1, timeoutInMs); }

void FPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_WriteFifoU16(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_RequestFIFO, data,
                                     length, timeoutInMs, &_remaining));
}

void FPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) { writeRequestFIFO(&data, 1, timeoutInMs); }

void FPGA::writeTimestampFIFO(uint64_t timestamp) {
    NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_WriteFifoU64(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU64_TimestampControlFIFO,
                                &timestamp, 1, 0, &_remaining));
}

void FPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) {
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_ReadFifoU8(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoU8_U8ResponseFIFO, data,
                                   length, timeoutInMs, &_remaining));
}

void FPGA::readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_ReadFifoU16(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoU16_U16ResponseFIFO,
                                    data, length, timeoutInMs, &_remaining));
}

void FPGA::writeHealthAndStatusFIFO(uint16_t request, uint16_t param) {
    uint16_t data[2] = {request, param};
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_WriteFifoU16(_session,
                                     NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_HealthAndStatusControlFIFO,
                                     data, 2, 0, &_remaining));
}

void FPGA::readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeoutInMs) {
    NiThrowError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadFifoU64(_session, NiFpga_M1M3SupportFPGA_TargetToHostFifoU64_HealthAndStatusDataFIFO,
                               data, length, timeoutInMs, &_remaining));
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
