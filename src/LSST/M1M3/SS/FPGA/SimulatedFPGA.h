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

#ifndef LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_
#define LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_

#include <chrono>
#include <mutex>
#include <queue>
#include <thread>

#include <SAL_MTM1M3C.h>
#include <SAL_MTMount.h>
#include <SAL_MTMountC.h>

#include <IFPGA.h>
#include <ILCSubnetData.h>
#include <HardpointActuatorSettings.h>

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

    void writeMPUFIFO(cRIO::MPU& mpu) override {}
    std::vector<uint8_t> readMPUFIFO(cRIO::MPU& mpu) override { throw std::runtime_error("readMPU called"); }

    void waitOnIrqs(uint32_t irqs, uint32_t timeout, bool& timedout, uint32_t* triggered = NULL) override {
        timedout = false;
    }
    void ackIrqs(uint32_t irqs) override {}
    uint32_t getIrq(uint8_t bus) override { return 0; }

    void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) override;
    void readHealthAndStatusFIFO(uint64_t* data, size_t length, uint32_t timeoutInMs = 10) override;

private:
    std::thread _monitorMountElevationThread;
    std::mutex _elevationReadWriteLock;
    SAL_MTMount _mgrMTMount;

    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    HardpointActuatorSettings* _hardpointActuatorSettings;

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
    void _writeModbus16(std::queue<uint16_t>* reponse, int16_t data);
    void _writeModbus32(std::queue<uint16_t>* reponse, int32_t data);
    void _writeModbusFloat(std::queue<uint16_t>* response, float data);
    void _writeModbusCRC(std::queue<uint16_t>* response);
    void _writeHP_ILCStatus(std::queue<uint16_t>* response, int index);

    void _monitorElevation(void);

    float _mountElevation = 90.;
    std::chrono::steady_clock::time_point _mountElevationValidTo;
    bool _simulatingToHorizon;
    bool _mountSimulatedMovementFirstPass = true;

    bool _exitThread = false;

    bool _sendResponse;

    // simulates properly clock signal
    std::chrono::time_point<std::chrono::steady_clock> _nextClock;

    // simulates pressure raising after air valve is opened
    std::chrono::time_point<std::chrono::steady_clock> _lastAirOpen;

    float _getAirPressure();
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
