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

#include <SAL_MTMountC.h>
#include "SAL_MTMount.h"

#define RND_CNT 50

struct MTMount_AltC;
struct MTMount_AzC;
struct MTM1M3_logevent_appliedCylinderForcesC;
struct MTM1M3_logevent_appliedHardpointStepsC;
struct MTM1M3_hardpointActuatorDataC;
struct MTM1M3_outerLoopDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class ForceActuatorApplicationSettings;

/**
 * FPGA simulator. Simulates MODBUS communication with devices.
 */
class SimulatedFPGA : public IFPGA {
public:
    SimulatedFPGA();

    ~SimulatedFPGA();

    void setPublisher(M1M3SSPublisher* publisher);
    void setForceActuatorApplicationSettings(
            ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

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
    M1M3SSPublisher* _publisher;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    MTMount_AzimuthC _tmaAzimuth;
    MTMount_ElevationC _tmaElevation;
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
    uint16_t _readModbus(uint16_t data);

    void _monitorElevation(void);

    bool _exitThread = false;
    float _rnd[RND_CNT];
    int _rndIndex;
    float _getRnd();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_ */
