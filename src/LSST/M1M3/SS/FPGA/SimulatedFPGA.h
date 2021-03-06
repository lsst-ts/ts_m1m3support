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

#ifndef LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_
#define LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_

#include <IFPGA.h>
#include <thread>
#include <mutex>
#include <queue>
#include <ILCSubnetData.h>
#include <HardpointActuatorSettings.h>

#include <SAL_MTMountC.h>
#include "SAL_MTMount.h"

struct MTMount_AltC;
struct MTMount_AzC;
struct MTM1M3_logevent_appliedCylinderForcesC;
struct MTM1M3_logevent_appliedHardpointStepsC;
struct MTM1M3_hardpointActuatorDataC;
struct MTM1M3_outerLoopDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * FPGA simulator. Simulates MODBUS communication with devices.
 */
class SimulatedFPGA : public IFPGA {
public:
    SimulatedFPGA();

    ~SimulatedFPGA();

    void initialize() override;
    void open() override;
    void close() override;
    void finalize() override;

    uint16_t getTxCommand(uint8_t bus) override {}
    uint16_t getRxCommand(uint8_t bus) override {}

    void waitForOuterLoopClock(uint32_t timeout) override;
    void ackOuterLoopClock() override;

    void waitForPPS(uint32_t timeout) override;
    void ackPPS() override;

    void waitForModbusIRQ(int32_t subnet, uint32_t timeout) override;
    void ackModbusIRQ(int32_t subnet) override;

    void pullTelemetry() override;
    void pullHealthAndStatus() override;

    void writeCommandFIFO(uint16_t* data, size_t length, uint32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t* data, size_t length, uint32_t timeoutInMs) override;
    void writeTimestampFIFO(uint64_t timestamp) override;
    void readU8ResponseFIFO(uint8_t* data, size_t length, uint32_t timeoutInMs) override;
    void readU16ResponseFIFO(uint16_t* data, size_t length, uint32_t timeoutInMs) override;

    void waitOnIrqs(uint32_t irqs, uint32_t timeout, uint32_t* triggered = NULL) override {}
    void ackIrqs(uint32_t irqs) override {}

    void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) override;
    void readHealthAndStatusFIFO(uint64_t* data, size_t length, uint32_t timeoutInMs = 10) override;

private:
    std::thread _monitorMountElevationThread;
    std::mutex _elevationReadWriteLock;
    SAL_MTMount _mgrMTMount;

    int _lastRequest;
    std::queue<uint16_t> _u8Response;
    std::queue<uint16_t> _u16Response;
    std::queue<uint16_t> _subnetAResponse;
    std::queue<uint16_t> _subnetBResponse;
    std::queue<uint16_t> _subnetCResponse;
    std::queue<uint16_t> _subnetDResponse;
    std::queue<uint16_t> _subnetEResponse;

    std::queue<uint16_t> _crcVector;
    void _writeModbus(std::queue<uint16_t>* response, uint16_t data);
    void _writeModbusCRC(std::queue<uint16_t>* response);

    void _monitorElevation(void);

    float _mountElevation = 90.;

    bool _exitThread = false;

    bool _sendResponse;
};

/**
 * Returns random double normalized to -1..1.
 *
 * @return double within -1..1, derived with rand() function with sound
 * statistics.
 *
 * @see rand()
 */
double getRndPM1();

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_ */
