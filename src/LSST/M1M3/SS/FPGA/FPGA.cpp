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
#include <NiStatus.h>
#include <NiError.h>
#include <unistd.h>
#include <U8ArrayUtilities.h>
#include <FPGAAddresses.h>

#include <iostream>
#include <iomanip>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FPGA::FPGA() {
    spdlog::debug("FPGA: FPGA()");
    _session = 0;
    _remaining = 0;
    this->u16Buffer[0] = 0;
    _outerLoopIRQContext = 0;
    _modbusIRQContext = 0;
    _ppsIRQContext = 0;
}

FPGA::~FPGA() {}

int32_t FPGA::initialize() {
    spdlog::debug("FPGA: initialize()");
    return NiReportError(__PRETTY_FUNCTION__, NiFpga_Initialize());
}

int32_t FPGA::open() {
    spdlog::debug("FPGA: open()");
    int32_t status = NiFpga_Open("/home/admin/Bitfiles/" NiFpga_M1M3SupportFPGA_Bitfile,
                                 NiFpga_M1M3SupportFPGA_Signature, "RIO0", 0, &(_session));
    if (status) {
        return NiReportError(__PRETTY_FUNCTION__, status);
    }
    status = NiFpga_Abort(_session);
    status = NiFpga_Download(_session);
    status = NiFpga_Reset(_session);
    status = NiFpga_Run(_session, 0);
    usleep(1000000);
    NiFpga_ReserveIrqContext(_session, &_outerLoopIRQContext);
    NiFpga_ReserveIrqContext(_session, &_modbusIRQContext);
    NiFpga_ReserveIrqContext(_session, &_ppsIRQContext);
    return NiReportError(__PRETTY_FUNCTION__, status);
}

int32_t FPGA::close() {
    spdlog::debug("FPGA: close()");
    NiFpga_UnreserveIrqContext(_session, _outerLoopIRQContext);
    NiFpga_UnreserveIrqContext(_session, _modbusIRQContext);
    NiFpga_UnreserveIrqContext(_session, _ppsIRQContext);
    return NiReportError(__PRETTY_FUNCTION__, NiFpga_Close(_session, 0));
}

int32_t FPGA::finalize() {
    spdlog::debug("FPGA: finalize()");
    return NiReportError(__PRETTY_FUNCTION__, NiFpga_Finalize());
}

bool FPGA::isErrorCode(int32_t status) {
    bool isError = NiFpga_IsError(status);
    if (isError) {
        spdlog::error("FPGA: Error code {}", status);
    }
    return isError;
}

int32_t FPGA::waitForOuterLoopClock(int32_t timeout) {
    uint32_t assertedIRQs = 0;
    uint8_t timedOut = false;
    int32_t result = NiReportError(__PRETTY_FUNCTION__,
                                   NiFpga_WaitOnIrqs(_session, _outerLoopIRQContext, NiFpga_Irq_0,
                                                     timeout, &assertedIRQs, &timedOut));
    return result;
}

int32_t FPGA::ackOuterLoopClock() {
    return NiReportError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, NiFpga_Irq_0));
}

int32_t FPGA::waitForPPS(int32_t timeout) {
    uint32_t assertedIRQs = 0;
    uint8_t timedOut = false;
    return NiReportError(__PRETTY_FUNCTION__,
                         NiFpga_WaitOnIrqs(_session, _ppsIRQContext, NiFpga_Irq_10, timeout,
                                           &assertedIRQs, &timedOut));
}

int32_t FPGA::ackPPS() { return NiFpga_AcknowledgeIrqs(_session, NiFpga_Irq_10); }

int32_t FPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) {
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
        return NiReportError(__PRETTY_FUNCTION__, NiFpga_WaitOnIrqs(_session, _modbusIRQContext,
                                                                    irqs, timeout, &assertedIRQs, &timedOut));
    }
    return 0;
}

int32_t FPGA::ackModbusIRQ(int32_t subnet) {
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
        return NiReportError(__PRETTY_FUNCTION__, NiFpga_AcknowledgeIrqs(_session, irqs));
    }
    return 0;
}

void FPGA::pullTelemetry() {
    spdlog::trace("FPGA: pullTelemetry()");
    this->writeRequestFIFO(FPGAAddresses::Telemetry, 0);
    uint16_t length[1];
    this->readU16ResponseFIFO(length, 1, 20);
    uint8_t buffer[1024];
    this->readU8ResponseFIFO(buffer, (int32_t)length[0], 20);
    this->supportFPGAData.Reserved = U8ArrayUtilities::U64(buffer, 0);
    this->supportFPGAData.InclinometerTxBytes = U8ArrayUtilities::U64(buffer, 8);
    this->supportFPGAData.InclinometerRxBytes = U8ArrayUtilities::U64(buffer, 16);
    this->supportFPGAData.InclinometerTxFrames = U8ArrayUtilities::U64(buffer, 24);
    this->supportFPGAData.InclinometerRxFrames = U8ArrayUtilities::U64(buffer, 32);
    this->supportFPGAData.InclinometerErrorTimestamp = U8ArrayUtilities::U64(buffer, 40);
    this->supportFPGAData.InclinometerErrorCode = U8ArrayUtilities::U8(buffer, 48);
    this->supportFPGAData.InclinometerSampleTimestamp = U8ArrayUtilities::U64(buffer, 49);
    this->supportFPGAData.InclinometerAngleRaw = U8ArrayUtilities::I32(buffer, 57);
    this->supportFPGAData.DisplacementTxBytes = U8ArrayUtilities::U64(buffer, 61);
    this->supportFPGAData.DisplacementRxBytes = U8ArrayUtilities::U64(buffer, 69);
    this->supportFPGAData.DisplacementTxFrames = U8ArrayUtilities::U64(buffer, 77);
    this->supportFPGAData.DisplacementRxFrames = U8ArrayUtilities::U64(buffer, 85);
    this->supportFPGAData.DisplacementErrorTimestamp = U8ArrayUtilities::U64(buffer, 93);
    this->supportFPGAData.DisplacementErrorCode = U8ArrayUtilities::U8(buffer, 101);
    this->supportFPGAData.DisplacementSampleTimestamp = U8ArrayUtilities::U64(buffer, 102);
    this->supportFPGAData.DisplacementRaw1 = U8ArrayUtilities::I32(buffer, 110);
    this->supportFPGAData.DisplacementRaw2 = U8ArrayUtilities::I32(buffer, 114);
    this->supportFPGAData.DisplacementRaw3 = U8ArrayUtilities::I32(buffer, 118);
    this->supportFPGAData.DisplacementRaw4 = U8ArrayUtilities::I32(buffer, 122);
    this->supportFPGAData.DisplacementRaw5 = U8ArrayUtilities::I32(buffer, 126);
    this->supportFPGAData.DisplacementRaw6 = U8ArrayUtilities::I32(buffer, 130);
    this->supportFPGAData.DisplacementRaw7 = U8ArrayUtilities::I32(buffer, 134);
    this->supportFPGAData.DisplacementRaw8 = U8ArrayUtilities::I32(buffer, 138);
    this->supportFPGAData.AccelerometerSampleCount = U8ArrayUtilities::U64(buffer, 142);
    this->supportFPGAData.AccelerometerSampleTimestamp = U8ArrayUtilities::U64(buffer, 150);
    this->supportFPGAData.AccelerometerRaw1 = U8ArrayUtilities::SGL(buffer, 158);
    this->supportFPGAData.AccelerometerRaw2 = U8ArrayUtilities::SGL(buffer, 162);
    this->supportFPGAData.AccelerometerRaw3 = U8ArrayUtilities::SGL(buffer, 166);
    this->supportFPGAData.AccelerometerRaw4 = U8ArrayUtilities::SGL(buffer, 170);
    this->supportFPGAData.AccelerometerRaw5 = U8ArrayUtilities::SGL(buffer, 174);
    this->supportFPGAData.AccelerometerRaw6 = U8ArrayUtilities::SGL(buffer, 178);
    this->supportFPGAData.AccelerometerRaw7 = U8ArrayUtilities::SGL(buffer, 182);
    this->supportFPGAData.AccelerometerRaw8 = U8ArrayUtilities::SGL(buffer, 186);
    this->supportFPGAData.GyroTxBytes = U8ArrayUtilities::U64(buffer, 190);
    this->supportFPGAData.GyroRxBytes = U8ArrayUtilities::U64(buffer, 198);
    this->supportFPGAData.GyroTxFrames = U8ArrayUtilities::U64(buffer, 206);
    this->supportFPGAData.GyroRxFrames = U8ArrayUtilities::U64(buffer, 214);
    this->supportFPGAData.GyroErrorTimestamp = U8ArrayUtilities::U64(buffer, 222);
    this->supportFPGAData.GyroErrorCode = U8ArrayUtilities::U8(buffer, 230);
    this->supportFPGAData.GyroSampleTimestamp = U8ArrayUtilities::U64(buffer, 231);
    this->supportFPGAData.GyroRawX = U8ArrayUtilities::SGL(buffer, 239);
    this->supportFPGAData.GyroRawY = U8ArrayUtilities::SGL(buffer, 243);
    this->supportFPGAData.GyroRawZ = U8ArrayUtilities::SGL(buffer, 247);
    this->supportFPGAData.GyroStatus = U8ArrayUtilities::U8(buffer, 251);
    this->supportFPGAData.GyroSequenceNumber = U8ArrayUtilities::U8(buffer, 252);
    this->supportFPGAData.GyroTemperature = U8ArrayUtilities::I16(buffer, 253);
    this->supportFPGAData.GyroBITTimestamp = U8ArrayUtilities::U64(buffer, 255);
    this->supportFPGAData.GyroBIT0 = U8ArrayUtilities::U8(buffer, 263);
    this->supportFPGAData.GyroBIT1 = U8ArrayUtilities::U8(buffer, 264);
    this->supportFPGAData.GyroBIT2 = U8ArrayUtilities::U8(buffer, 265);
    this->supportFPGAData.GyroBIT3 = U8ArrayUtilities::U8(buffer, 266);
    this->supportFPGAData.GyroBIT4 = U8ArrayUtilities::U8(buffer, 267);
    this->supportFPGAData.GyroBIT5 = U8ArrayUtilities::U8(buffer, 268);
    this->supportFPGAData.GyroBIT6 = U8ArrayUtilities::U8(buffer, 269);
    this->supportFPGAData.GyroBIT7 = U8ArrayUtilities::U8(buffer, 270);
    this->supportFPGAData.DigitalInputSampleCount = U8ArrayUtilities::U64(buffer, 271);
    this->supportFPGAData.DigitalInputTimestamp = U8ArrayUtilities::U64(buffer, 279);
    this->supportFPGAData.DigitalInputStates = U8ArrayUtilities::U16(buffer, 287);
    this->supportFPGAData.DigitalOutputSampleCount = U8ArrayUtilities::U64(buffer, 289);
    this->supportFPGAData.DigitalOutputTimestamp = U8ArrayUtilities::U64(buffer, 297);
    this->supportFPGAData.DigitalOutputStates = U8ArrayUtilities::U8(buffer, 305);
    this->supportFPGAData.PowerSupplySampleCount = U8ArrayUtilities::U64(buffer, 306);
    this->supportFPGAData.PowerSupplyTimestamp = U8ArrayUtilities::U64(buffer, 314);
    this->supportFPGAData.PowerSupplyStates = U8ArrayUtilities::U8(buffer, 322);
}

void FPGA::pullHealthAndStatus() {
    writeHealthAndStatusFIFO(2, 0);
    uint64_t buffer[64];
    readHealthAndStatusFIFO(buffer, 64, 500);
    healthAndStatusFPGAData.refresh(buffer);
}

int32_t FPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    return NiReportError(
            __PRETTY_FUNCTION__,
            NiFpga_WriteFifoU16(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_CommandFIFO, data,
                                length, timeoutInMs, &_remaining));
}

int32_t FPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) {
    this->u16Buffer[0] = data;
    return this->writeCommandFIFO(this->u16Buffer, 1, timeoutInMs);
}

void FPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_WriteFifoU16(_session, NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_RequestFIFO,
                                     data, length, timeoutInMs, &_remaining));
}

void FPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) {
    this->writeRequestFIFO(&data, 1, timeoutInMs);
}

int32_t FPGA::writeTimestampFIFO(uint64_t timestamp) {
    uint64_t buffer[1] = {timestamp};
    return NiReportError(__PRETTY_FUNCTION__,
                         NiFpga_WriteFifoU64(_session,
                                             NiFpga_M1M3SupportFPGA_HostToTargetFifoU64_TimestampControlFIFO,
                                             buffer, 1, 0, &_remaining));
}

int32_t FPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) {
    return NiReportError(
            __PRETTY_FUNCTION__,
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

int32_t FPGA::readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeoutInMs) {
    return NiReportError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadFifoU64(_session,
                               NiFpga_M1M3SupportFPGA_TargetToHostFifoU64_HealthAndStatusDataFIFO, data,
                               length, timeoutInMs, &_remaining));
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
