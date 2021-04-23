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

#include <SimulatedFPGA.h>
#include <SAL_MTM1M3C.h>
#include <SAL_MTMountC.h>
#include <M1M3SSPublisher.h>
#include <FPGAAddresses.h>
#include <cstring>
#include <Timestamp.h>
#include <CRC.h>
#include <SettingReader.h>

#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include "SAL_MTMount.h"
#include "ccpp_sal_MTMount.h"

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Return data writen to modbus. The data are right shifted by 1 to allow for
 * signaling data end. See FPGA code for details.
 */
uint8_t _readModbus(uint16_t data) { return (data >> 1) & 0xFF; }

double getRndPM1() { return static_cast<double>(rand()) / (RAND_MAX / 2.0) - 1.0; }

SimulatedFPGA::SimulatedFPGA() {
    SPDLOG_INFO("SimulatedFPGA: SimulatedFPGA()");
    _lastRequest = -1;
    memset(&supportFPGAData, 0, sizeof(SupportFPGAData));
    supportFPGAData.DigitalInputStates = 0x0001 | 0x0002 | 0x0008 | 0x0010 | 0x0040 | 0x0080;
    _mgrMTMount = SAL_MTMount();
    _mgrMTMount.salTelemetrySub(const_cast<char*>("MTMount_elevation"));

    _monitorMountElevationThread = std::thread(&SimulatedFPGA::_monitorElevation, this);

    MTM1M3_hardpointActuatorDataC* hardpointActuatorData = M1M3SSPublisher::get().getHardpointActuatorData();
    hardpointActuatorData->encoder[0] = 15137;
    hardpointActuatorData->encoder[1] = 20079;
    hardpointActuatorData->encoder[2] = 26384;
    hardpointActuatorData->encoder[3] = 27424;
    hardpointActuatorData->encoder[4] = 17560;
    hardpointActuatorData->encoder[5] = 23546;

    _sendResponse = true;
}

SimulatedFPGA::~SimulatedFPGA() {
    _exitThread = true;

    _monitorMountElevationThread.join();

    _mgrMTMount.salShutdown();
}

void SimulatedFPGA::_monitorElevation(void) {
    MTMount_elevationC mountElevationInstance;

    SPDLOG_DEBUG("Start monitoring mount elevation...");

    while (!_exitThread) {
        ReturnCode_t status = _mgrMTMount.getSample_elevation(&mountElevationInstance);

        if (status == 0) {
            SPDLOG_DEBUG("Got valid elevation sample...");

            {
                std::lock_guard<std::mutex> lock_g(_elevationReadWriteLock);
                _mountElevation = mountElevationInstance.actualPosition;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void SimulatedFPGA::initialize() { SPDLOG_DEBUG("SimulatedFPGA: initialize()"); }

void SimulatedFPGA::open() { SPDLOG_DEBUG("SimulatedFPGA: open()"); }

void SimulatedFPGA::close() { SPDLOG_DEBUG("SimulatedFPGA: close()"); }

void SimulatedFPGA::finalize() { SPDLOG_DEBUG("SimulatedFPGA: finalize()"); }

void SimulatedFPGA::waitForOuterLoopClock(uint32_t timeout) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void SimulatedFPGA::ackOuterLoopClock() {}

void SimulatedFPGA::waitForPPS(uint32_t timeout) { std::this_thread::sleep_for(std::chrono::seconds(1)); }

void SimulatedFPGA::ackPPS() {}

void SimulatedFPGA::waitForModbusIRQ(int32_t subnet, uint32_t timeout) {}

void SimulatedFPGA::ackModbusIRQ(int32_t subnet) {}

void SimulatedFPGA::pullTelemetry() {
    SPDLOG_TRACE("SimulatedFPGA: pullTelemetry()");
    uint64_t timestamp = Timestamp::toRaw(M1M3SSPublisher::get().getTimestamp());
    this->supportFPGAData.Reserved = 0;
    this->supportFPGAData.InclinometerTxBytes = 0;
    this->supportFPGAData.InclinometerRxBytes = 0;
    this->supportFPGAData.InclinometerTxFrames = 0;
    this->supportFPGAData.InclinometerRxFrames = 0;
    this->supportFPGAData.InclinometerErrorTimestamp = 0;
    this->supportFPGAData.InclinometerErrorCode = 0;
    this->supportFPGAData.InclinometerSampleTimestamp = timestamp;

    // Inclinometer raw value is measured as (negative) zenith angle (0 = zenith, -90 = horizon).
    // Converts elevation to zenith angle and adds random 1/200th deg (=18 arcsec) noise.

    {
        std::lock_guard<std::mutex> lock_g(_elevationReadWriteLock);
        this->supportFPGAData.InclinometerAngleRaw =
                (int32_t)((_mountElevation - 90.0) * 1000.0) + (getRndPM1() * 5.0);
    }

    this->supportFPGAData.DisplacementTxBytes = 0;
    this->supportFPGAData.DisplacementRxBytes = 0;
    this->supportFPGAData.DisplacementTxFrames = 0;
    this->supportFPGAData.DisplacementRxFrames = 0;
    this->supportFPGAData.DisplacementErrorTimestamp = 0;
    this->supportFPGAData.DisplacementErrorCode = 0;
    this->supportFPGAData.DisplacementSampleTimestamp = timestamp;
    this->supportFPGAData.DisplacementRaw1 = (int32_t)(getRndPM1() + 10) * 1000.0;
    this->supportFPGAData.DisplacementRaw2 = (int32_t)(getRndPM1() + 20) * 1000.0;
    this->supportFPGAData.DisplacementRaw3 = (int32_t)(getRndPM1() + 30) * 1000.0;
    this->supportFPGAData.DisplacementRaw4 = (int32_t)(getRndPM1() + 40) * 1000.0;
    this->supportFPGAData.DisplacementRaw5 = (int32_t)(getRndPM1() + 50) * 1000.0;
    this->supportFPGAData.DisplacementRaw6 = (int32_t)(getRndPM1() + 60) * 1000.0;
    this->supportFPGAData.DisplacementRaw7 = (int32_t)(getRndPM1() + 70) * 1000.0;
    this->supportFPGAData.DisplacementRaw8 = (int32_t)(getRndPM1() + 80) * 1000.0;
    this->supportFPGAData.AccelerometerSampleCount++;
    this->supportFPGAData.AccelerometerSampleTimestamp = timestamp;
    this->supportFPGAData.AccelerometerRaw1 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw2 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw3 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw4 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw5 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw6 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw7 = getRndPM1() * 0.01;
    this->supportFPGAData.AccelerometerRaw8 = getRndPM1() * 0.01;
    this->supportFPGAData.GyroTxBytes = 0;
    this->supportFPGAData.GyroRxBytes = 0;
    this->supportFPGAData.GyroTxFrames = 0;
    this->supportFPGAData.GyroRxFrames = 0;
    this->supportFPGAData.GyroErrorTimestamp = 0;
    this->supportFPGAData.GyroErrorCode = 0;
    this->supportFPGAData.GyroSampleTimestamp = timestamp;
    this->supportFPGAData.GyroRawX = getRndPM1() * 0.01;
    this->supportFPGAData.GyroRawY = getRndPM1() * 0.01;
    this->supportFPGAData.GyroRawZ = getRndPM1() * 0.01;
    this->supportFPGAData.GyroStatus = 0x7F;
    this->supportFPGAData.GyroSequenceNumber++;
    if (this->supportFPGAData.GyroSequenceNumber > 127) {
        this->supportFPGAData.GyroSequenceNumber = 0;
    }
    this->supportFPGAData.GyroTemperature = 24 + int(getRndPM1() * 2);
    this->supportFPGAData.GyroBITTimestamp = timestamp;
    this->supportFPGAData.GyroBIT0 = 0x7F;
    this->supportFPGAData.GyroBIT1 = 0x7F;
    this->supportFPGAData.GyroBIT2 = 0x7F;
    this->supportFPGAData.GyroBIT3 = 0x7F;
    this->supportFPGAData.GyroBIT4 = 0x7F;
    this->supportFPGAData.GyroBIT5 = 0x7F;
    this->supportFPGAData.GyroBIT6 = 0x7F;
    this->supportFPGAData.GyroBIT7 = 0x7F;
    this->supportFPGAData.DigitalInputSampleCount++;
    this->supportFPGAData.DigitalInputTimestamp = timestamp;
    //	this->supportFPGAData.DigitalInputStates = 0;
    this->supportFPGAData.DigitalOutputSampleCount++;
    this->supportFPGAData.DigitalOutputTimestamp = timestamp;
    //	this->supportFPGAData.DigitalOutputStates = 0;
    this->supportFPGAData.PowerSupplySampleCount++;
    this->supportFPGAData.PowerSupplyTimestamp = timestamp;
    //	this->supportFPGAData.PowerSupplyStates = 0;
}

void SimulatedFPGA::pullHealthAndStatus() {}

uint8_t _broadCastCounter() {
    return static_cast<uint8_t>(M1M3SSPublisher::get().getOuterLoopData()->broadcastCounter) << 4;
}

void SimulatedFPGA::writeCommandFIFO(uint16_t* data, size_t length, uint32_t timeoutInMs) {
    for (size_t i = 0; i < length;) {
        uint16_t signal = data[i++];
        uint16_t dataLength = 0;
        uint16_t subnet = 0;
        std::queue<uint16_t>* response = 0;
        switch (signal) {
            case FPGAAddresses::AccelerometerAx:
            case FPGAAddresses::AccelerometerAz:
            case FPGAAddresses::AccelerometerBy:
            case FPGAAddresses::AccelerometerBz:
            case FPGAAddresses::AccelerometerCx:
            case FPGAAddresses::AccelerometerCz:
            case FPGAAddresses::AccelerometerDy:
            case FPGAAddresses::AccelerometerDz:
                break;
            case FPGAAddresses::ModbusSubnetATx:
                subnet = 1;
                response = &_subnetAResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetBTx:
                subnet = 2;
                response = &_subnetBResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetCTx:
                subnet = 3;
                response = &_subnetCResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetDTx:
                subnet = 4;
                response = &_subnetDResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetARx:
            case FPGAAddresses::ModbusSubnetBRx:
            case FPGAAddresses::ModbusSubnetCRx:
            case FPGAAddresses::ModbusSubnetDRx:
                break;
            case FPGAAddresses::ModbusSubnetETx:
                subnet = 5;
                response = &_subnetEResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::GyroTx: {
                int size = data[i++];  // Read Tx Size
                i += size;             // Read All Tx Data
                break;
            }
            case FPGAAddresses::ModbusSubnetERx:
            case FPGAAddresses::GyroRx:
                break;
            case FPGAAddresses::ILCPowerInterlockStatus:
            case FPGAAddresses::FanCoilerHeaterInterlockStatus:
            case FPGAAddresses::AirSupplyInterlockStatus:
            case FPGAAddresses::CabinetDoorInterlockStatus:
            case FPGAAddresses::TMAMotionStopInterlockStatus:
            case FPGAAddresses::GISHeartbeatInterlockStatus:
            case FPGAAddresses::AirSupplyValveOpen:
            case FPGAAddresses::AirSupplyValveClosed:
            case FPGAAddresses::MirrorCellLightsOn:
            case FPGAAddresses::HeartbeatToSafetyController: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates =
                        (this->supportFPGAData.DigitalOutputStates & 0xFE) | (state ? 0x01 : 0x00);
                break;
            }
            case FPGAAddresses::AirSupplyValveControl: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates =
                        (this->supportFPGAData.DigitalOutputStates & 0xEF) | (state ? 0x00 : 0x10);
                this->supportFPGAData.DigitalInputStates =
                        (this->supportFPGAData.DigitalInputStates & 0xFCFF) | (state ? 0x0200 : 0x0100);
                break;
            }
            case FPGAAddresses::MirrorCellLightControl: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates =
                        (this->supportFPGAData.DigitalOutputStates & 0xDF) | (state ? 0x20 : 0x00);
                this->supportFPGAData.DigitalInputStates =
                        (this->supportFPGAData.DigitalInputStates & 0xFBFF) | (state ? 0x0400 : 0x0000);
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkAOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x01) | (state ? 0x01 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkBOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x02) | (state ? 0x02 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkCOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x04) | (state ? 0x04 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkDOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x08) | (state ? 0x08 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkAOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x10) | (state ? 0x10 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkBOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x20) | (state ? 0x20 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkCOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x40) | (state ? 0x40 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkDOn: {
                int state = data[i++];
                this->supportFPGAData.PowerSupplyStates =
                        (this->supportFPGAData.PowerSupplyStates & ~0x80) | (state ? 0x80 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::Displacement:
            case FPGAAddresses::Inclinometer:
            case FPGAAddresses::ModbusSoftwareTrigger:
            case FPGAAddresses::Telemetry:
            case FPGAAddresses::Timestamp:
                break;
            default:
                break;
        }
        if (response != 0) {
            bool stepMotorBroadcast = false;
            ++i;  // Read Software Trigger
            // Response length is prepended at request
            uint64_t rawTimestamp = Timestamp::toRaw(M1M3SSPublisher::get().getTimestamp());
            response->push((uint16_t)(rawTimestamp));
            response->push((uint16_t)(rawTimestamp >> 16));
            response->push((uint16_t)(rawTimestamp >> 32));
            response->push((uint16_t)(rawTimestamp >> 48));  // Write Global Timestamp
            size_t endIndex = i - 1 + dataLength - 1;
            // The first -1 is for the software trigger
            // The second -1 is for the trigger
            while (i < endIndex) {
                _sendResponse = true;
                uint8_t address = _readModbus(data[i++]);   // Read Address
                uint8_t function = _readModbus(data[i++]);  // Read Function Code
                if (address == 248 && function == 66) {
                    stepMotorBroadcast = true;
                }
                if (address == 248 || address == 249) {
                    // This is a broadcast, no response, just read until 0xA000
                    while ((data[i] & 0xF000) != 0x2000) {
                        ++i;
                    }
                    i--;  // Shift to CRC for the remainder of the time
                          // However broadcasts are followed by a timestamp
                          // so only shift 1
                } else if (address == 0 && function == 0) {
                    // This is a special nop, read until 0xF000 != 0
                    while (data[i] == 0) {
                        ++i;
                    }
                    i -= 4;  // Shift to CRC for the rest of the parsing to work
                } else if (subnet >= 1 && subnet <= 4) {
                    // if bus is turned off, don't send any response
                    if ((supportFPGAData.PowerSupplyStates & 0x10) == 0x00) {
                        _sendResponse = false;
                    }

                    ForceActuatorApplicationSettings* forceActuatorApplicationSettings =
                            SettingReader::get().getForceActuatorApplicationSettings();
                    int zIndex = -1;
                    for (int j = 0; j < FA_COUNT; ++j) {
                        if (forceActuatorApplicationSettings->Table[j].Subnet == subnet &&
                            forceActuatorApplicationSettings->Table[j].Address == address) {
                            zIndex = j;
                            break;
                        }
                    }
                    int pIndex = zIndex;
                    int sIndex = forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[zIndex];
                    switch (function) {
                        case 17:                               // Report Server Id
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 12);        // Write Message Length
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, signal);
                            _writeModbus(response, address);  // Write ILC Unique Id
                            _writeModbus(response, 0);        // TODO: Write ILC Application Type
                            _writeModbus(response, 0);        // TODO: Write Network Node Type
                            _writeModbus(response, 0);        // TODO: Write ILC Selected Options
                            _writeModbus(response, 0);        // TODO: Write Network Node Options
                            _writeModbus(response, 99);       // TODO: Write Major Revision
                            _writeModbus(response, 99);       // TODO: Write Minor Revision
                            // TODO: Write Firmware Name
                            _writeModbusCRC(response);
                            break;
                        case 18:  // Report Server Status
                            _writeModbus(response, address);
                            _writeModbus(response, function);
                            _writeModbus(response, 0);  // TODO: Write IlC State
                            _writeModbus(response, 0);
                            _writeModbus(response, 0);  // TODO: Write ILC Status
                            _writeModbus(response, 0);
                            _writeModbus(response, 0);  // TODO: Write ILC Faults
                            _writeModbusCRC(response);
                            break;
                        case 65: {                                     // Change ILC Mode
                            ++i;                                       // Read Reserved Byte
                            uint8_t newMode = _readModbus(data[i++]);  // Read New Mode
                            _writeModbus(response, address);           // Write Address
                            _writeModbus(response, function);          // Write Function
                            _writeModbus(response, 0);                 // Write Reserved Byte
                            _writeModbus(response, newMode);           // Write ILC State
                            _writeModbusCRC(response);
                            break;
                        }
                        case 73:                               // Set Boost Valve DCA Gains
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            i += 4;                            // Read Primary Cylinder DCA Gain
                            i += 4;                            // Read Secondary Cylinder DCA Gain
                            _writeModbusCRC(response);
                            break;
                        case 74: {                             // Read Boost Valve DCA Gains
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            uint8_t buffer[4];
                            float gain = 1.0;
                            memcpy(buffer, &gain, 4);
                            _writeModbus(response, buffer[3]);
                            _writeModbus(response, buffer[2]);
                            _writeModbus(response, buffer[1]);
                            _writeModbus(response, buffer[0]);  // Write Primary Cylinder DCA Gain
                            _writeModbus(response, buffer[3]);
                            _writeModbus(response, buffer[2]);
                            _writeModbus(response, buffer[1]);
                            _writeModbus(response, buffer[0]);  // Write Secondary Cylinder DCA Gain
                            _writeModbusCRC(response);
                            break;
                        }
                        case 75: {  // Force Demand
                            ++i;    // Read Slew Flag
                            uint8_t word1 = _readModbus(data[i++]);
                            uint8_t word2 = _readModbus(data[i++]);
                            uint8_t word3 = _readModbus(data[i++]);
                            _writeModbus(response, address);              // Write Address
                            _writeModbus(response, function);             // Write Function
                            _writeModbus(response, _broadCastCounter());  // Write ILC Status
                            uint8_t buffer[4];
                            float force = (((float)((static_cast<uint32_t>(word1) << 16) |
                                                    (static_cast<uint32_t>(word2) << 8) | word3)) /
                                           1000.0) +
                                          (getRndPM1() * 0.5);
                            memcpy(buffer, &force, 4);
                            _writeModbus(response, buffer[3]);
                            _writeModbus(response, buffer[2]);
                            _writeModbus(response, buffer[1]);
                            _writeModbus(response, buffer[0]);  // Write Primary Cylinder Force
                            if (address > 16) {
                                word1 = _readModbus(data[i++]);
                                word2 = _readModbus(data[i++]);
                                word3 = _readModbus(data[i++]);
                                force = (((float)((static_cast<uint32_t>(word1) << 16) |
                                                  (static_cast<uint32_t>(word2) << 8) | word3)) /
                                         1000.0) +
                                        (getRndPM1() * 0.5);
                                memcpy(buffer, &force, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response, buffer[0]);  // Write Secondary Cylinder Force
                            }
                            _writeModbusCRC(response);
                            break;
                        }
                        case 76: {                                        // Force And Status
                            _writeModbus(response, address);              // Write Address
                            _writeModbus(response, function);             // Write Function
                            _writeModbus(response, _broadCastCounter());  // Write ILC Status
                            uint8_t buffer[4];
                            float force = (((float)M1M3SSPublisher::get()
                                                    .getEventAppliedCylinderForces()
                                                    ->primaryCylinderForces[pIndex]) /
                                           1000.0) +
                                          (getRndPM1() * 0.5);  // Update to Primary Cylinder Force
                            // uncomment to simulate follow up error
                            // if (subnet == 1 && address == 17 && force > 500) force = 200;
                            memcpy(buffer, &force, 4);
                            _writeModbus(response, buffer[3]);
                            _writeModbus(response, buffer[2]);
                            _writeModbus(response, buffer[1]);
                            _writeModbus(response, buffer[0]);  // Write Primary Cylinder Force
                            if (address > 16) {
                                force = (((float)M1M3SSPublisher::get()
                                                  .getEventAppliedCylinderForces()
                                                  ->secondaryCylinderForces[sIndex]) /
                                         1000.0) +
                                        (getRndPM1() * 0.5);  // Update to Secondary Cylinder Force
                                memcpy(buffer, &force, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response,
                                             buffer[0]);  // Write Secondary Cylinder Force
                            }
                            _writeModbusCRC(response);
                            break;
                        }
                        case 80: {  // ADC Scan Rate
                            uint8_t scanRate = _readModbus(data[i++]);
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, scanRate);  // Write ADC Scan Rate
                            _writeModbusCRC(response);
                            break;
                        }
                        case 81:                               // ADC Channel Offset and Sensitivity
                            ++i;                               // Read ADC Channel
                            i += 4;                            // Read Offset
                            i += 4;                            // Read Sensitivity
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbusCRC(response);
                            break;
                        case 107:                              // Reset
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbusCRC(response);
                            break;
                        case 110: {                            // Read Calibration
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            for (int j = 0; j < 24; ++j) {
                                uint8_t buffer[4];
                                float value = 0.0;
                                memcpy(buffer, &value, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response, buffer[0]);  // Write Calibration Data
                            }
                            _writeModbusCRC(response);
                            break;
                        }
                        case 119: {                            // Read DCA Pressure Values
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            for (int j = 0; j < 4; ++j) {
                                uint8_t buffer[4];
                                float value = 120.0;
                                memcpy(buffer, &value, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response, buffer[0]);  // Write DCA Pressure
                            }
                            _writeModbusCRC(response);
                            break;
                        }
                        case 120:                              // Read DCA Id
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x01);
                            _writeModbus(response, signal);
                            _writeModbus(response, address);  // Write DCA Unique Id
                            _writeModbus(response, 0);        // TODO: Write DCA Firmware Type
                            _writeModbus(response, 99);       // TODO: Write Major Revision
                            _writeModbus(response, 99);       // TODO: Write Minor Revision
                            _writeModbusCRC(response);
                            break;
                        case 121:                              // Read DCA Status
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);  // TODO: Write DCA Status
                            _writeModbusCRC(response);
                            break;
                        default:
                            break;
                    }
                } else if (subnet == 5 && address >= 1 && address <= 6) {
                    // if bus is turned off, don't send any response
                    if ((supportFPGAData.PowerSupplyStates & 0x10) == 0x00) {
                        _sendResponse = false;
                    }

                    auto fillHPStatus = [address, function, &response, this](int steps) {
                        _writeModbus(response, address);              // Write Address
                        _writeModbus(response, function);             // Write Function
                        _writeModbus(response, _broadCastCounter());  // Write ILC Status
                        // Number of steps issued / 4 + current encoder
                        // The encoder is also inverted after being received to match axis direction
                        // So we have to also invert the encoder here to counteract that
                        if (steps < 4 && steps > 0) {
                            steps = 4;
                        } else if (steps > -4 && steps < 0) {
                            steps = -4;
                        }
                        int32_t encoder =
                                -(M1M3SSPublisher::get().getHardpointActuatorData()->encoder[address - 1]) +
                                SettingReader::get().getHardpointActuatorSettings()->getEncoderOffset(
                                        address - 1) -
                                steps / 4;
                        _writeModbus(response, (encoder >> 24) & 0xFF);
                        _writeModbus(response, (encoder >> 16) & 0xFF);
                        _writeModbus(response, (encoder >> 8) & 0xFF);
                        _writeModbus(response, encoder & 0xFF);  // Write Encoder
                        uint8_t buffer[4];
                        float force = getRndPM1() * 8.0;
                        memcpy(buffer, &force, 4);
                        _writeModbus(response, buffer[3]);
                        _writeModbus(response, buffer[2]);
                        _writeModbus(response, buffer[1]);
                        _writeModbus(response, buffer[0]);  // Write Measured Force
                        _writeModbusCRC(response);
                    };

                    switch (function) {
                        case 17:                               // Report Server Id
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 12);        // Write Message Length
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, signal);
                            _writeModbus(response, address);  // Write ILC Unique Id
                            _writeModbus(response, 0);        // TODO: Write ILC Application Type
                            _writeModbus(response, 0);        // TODO: Write Network Node Type
                            _writeModbus(response, 0);        // TODO: Write ILC Selected Options
                            _writeModbus(response, 0);        // TODO: Write Network Node Options
                            _writeModbus(response, 99);       // TODO: Write Major Revision
                            _writeModbus(response, 99);       // TODO: Write Minor Revision
                            // TODO: Write Firmware Name
                            _writeModbusCRC(response);
                            break;
                        case 18:  // Report Server Status
                            _writeModbus(response, address);
                            _writeModbus(response, function);
                            _writeModbus(response, 0);  // TODO: Write IlC State
                            _writeModbus(response, 0);
                            _writeModbus(response, 0);  // TODO: Write ILC Status
                            _writeModbus(response, 0);
                            _writeModbus(response, 0);  // TODO: Write ILC Faults
                            _writeModbusCRC(response);
                            break;
                        case 65: {                                     // Change ILC Mode
                            ++i;                                       // Read Reserved Byte
                            uint8_t newMode = _readModbus(data[i++]);  // Read New Mode
                            _writeModbus(response, address);           // Write Address
                            _writeModbus(response, function);          // Write Function
                            _writeModbus(response, 0);                 // Write Reserved Byte
                            _writeModbus(response, newMode);           // Write ILC State
                            _writeModbusCRC(response);
                            break;
                        }
                        case 66: {  // Step Motor
                            fillHPStatus(data[i++]);
                            break;
                        }
                        case 67: {  // Force And Status
                            int steps = 0;
                            if (stepMotorBroadcast) {
                                steps = M1M3SSPublisher::get()
                                                .getHardpointActuatorData()
                                                ->stepsCommanded[address - 1];
                            }
                            fillHPStatus(steps);
                            break;
                        }
                        case 80: {  // ADC Scan Rate
                            uint8_t scanRate = _readModbus(data[i++]);
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, scanRate);  // Write ADC Scan Rate
                            _writeModbusCRC(response);
                            break;
                        }
                        case 81:                               // ADC Channel Offset and Sensitivity
                            ++i;                               // Read ADC Channel
                            i += 4;                            // Read Offset
                            i += 4;                            // Read Sensitivity
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbusCRC(response);
                            break;
                        case 107:                              // Reset
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbusCRC(response);
                            break;
                        case 110: {                            // Read Calibration
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            for (int j = 0; j < 24; ++j) {
                                uint8_t buffer[4];
                                float value = 0.0;
                                memcpy(buffer, &value, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response, buffer[0]);  // Write Calibration Data
                            }
                            _writeModbusCRC(response);
                            break;
                        }
                        default:
                            break;
                    }
                } else if (subnet == 5 && address >= 84 && address <= 89) {
                    switch (function) {
                        case 17:                               // Report Server Id
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 12);        // Write Message Length
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, signal);
                            _writeModbus(response, address);  // Write ILC Unique Id
                            _writeModbus(response, 0);        // TODO: Write ILC Application Type
                            _writeModbus(response, 0);        // TODO: Write Network Node Type
                            _writeModbus(response, 0);        // TODO: Write ILC Selected Options
                            _writeModbus(response, 0);        // TODO: Write Network Node Options
                            _writeModbus(response, 99);       // TODO: Write Major Revision
                            _writeModbus(response, 99);       // TODO: Write Minor Revision
                            // TODO: Write Firmware Name
                            _writeModbusCRC(response);
                            break;
                        case 18:  // Report Server Status
                            _writeModbus(response, address);
                            _writeModbus(response, function);
                            _writeModbus(response, 0);  // TODO: Write IlC State
                            _writeModbus(response, 0);
                            _writeModbus(response, 0);  // TODO: Write ILC Status
                            _writeModbus(response, 0);
                            _writeModbus(response, 0);  // TODO: Write ILC Faults
                            _writeModbusCRC(response);
                            break;
                        case 65: {                                     // Change ILC Mode
                            ++i;                                       // Read Reserved Byte
                            uint8_t newMode = _readModbus(data[i++]);  // Read New Mode
                            _writeModbus(response, address);           // Write Address
                            _writeModbus(response, function);          // Write Function
                            _writeModbus(response, 0);                 // Write Reserved Byte
                            _writeModbus(response, newMode);           // Write ILC State
                            _writeModbusCRC(response);
                            break;
                        }
                        case 107:                              // Reset
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbusCRC(response);
                            break;
                        case 119: {                            // Read DCA Pressure Values
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            for (int j = 0; j < 4; ++j) {
                                uint8_t buffer[4];
                                float value = 120.0;
                                memcpy(buffer, &value, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response, buffer[0]);  // Write DCA Pressure
                            }
                            _writeModbusCRC(response);
                            break;
                        }
                        case 120:                              // Read DCA Id
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x01);
                            _writeModbus(response, signal);
                            _writeModbus(response, address);  // Write DCA Unique Id
                            _writeModbus(response, 0);        // TODO: Write DCA Firmware Type
                            _writeModbus(response, 99);       // TODO: Write Major Revision
                            _writeModbus(response, 99);       // TODO: Write Minor Revision
                            _writeModbusCRC(response);
                            break;
                        case 121:                              // Read DCA Status
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            _writeModbus(response, 0x00);
                            _writeModbus(response, 0x00);  // TODO: Write DCA Status
                            _writeModbusCRC(response);
                            break;
                        case 122:                              // Report LVDT
                            _writeModbus(response, address);   // Write Address
                            _writeModbus(response, function);  // Write Function
                            for (int j = 0; j < 2; ++j) {
                                uint8_t buffer[4];
                                float value = 0.0;
                                memcpy(buffer, &value, 4);
                                _writeModbus(response, buffer[3]);
                                _writeModbus(response, buffer[2]);
                                _writeModbus(response, buffer[1]);
                                _writeModbus(response, buffer[0]);  // Write LVDT
                            }
                            _writeModbusCRC(response);
                            break;
                        default:
                            break;
                    }
                }
                i += 2;  // Read CRC
                ++i;     // Read End of Frame
                ++i;     // Read Wait for Rx or Delay
            }
            ++i;  // Read Write Trigger IRQ
        }
    }
}

void SimulatedFPGA::_writeModbus(std::queue<uint16_t>* response, uint16_t data) {
    if (_sendResponse == false) {
        return;
    }
    _crcVector.push(data);
    response->push((data << 1) | 0x9000);
}

void SimulatedFPGA::_writeModbusCRC(std::queue<uint16_t>* response) {
    uint16_t buffer[256];
    int i = 0;
    while (!_crcVector.empty()) {
        buffer[i] = _crcVector.front();
        i++;
        _crcVector.pop();
    }
    uint16_t crc = CRC::modbus(buffer, 0, i);
    response->push((((crc >> 0) & 0xFF) << 1) | 0x9000);
    response->push((((crc >> 8) & 0xFF) << 1) | 0x9000);  // Write CRC

    uint64_t rawTimestamp = Timestamp::toRaw(M1M3SSPublisher::get().getTimestamp());

    for (int i = 0; i < 8; ++i) {
        response->push(0xB000 | (rawTimestamp & 0xFF));  // Write Timestamp
        rawTimestamp >>= 8;
    }
    response->push(0xA000);  // Write End of Frame
}

void SimulatedFPGA::writeRequestFIFO(uint16_t* data, size_t length, uint32_t timeoutInMs) {
    int signal = data[0];
    std::queue<uint16_t>* modbusResponse = 0;
    switch (signal) {
        case FPGAAddresses::AccelerometerAx:
        case FPGAAddresses::AccelerometerAz:
        case FPGAAddresses::AccelerometerBy:
        case FPGAAddresses::AccelerometerBz:
        case FPGAAddresses::AccelerometerCx:
        case FPGAAddresses::AccelerometerCz:
        case FPGAAddresses::AccelerometerDy:
        case FPGAAddresses::AccelerometerDz:
        case FPGAAddresses::ModbusSubnetATx:
        case FPGAAddresses::ModbusSubnetBTx:
        case FPGAAddresses::ModbusSubnetCTx:
        case FPGAAddresses::ModbusSubnetDTx:
        case FPGAAddresses::ModbusSubnetARx:
            modbusResponse = &_subnetAResponse;
            break;
        case FPGAAddresses::ModbusSubnetBRx:
            modbusResponse = &_subnetBResponse;
            break;
        case FPGAAddresses::ModbusSubnetCRx:
            modbusResponse = &_subnetCResponse;
            break;
        case FPGAAddresses::ModbusSubnetDRx:
            modbusResponse = &_subnetDResponse;
            break;
        case FPGAAddresses::ModbusSubnetETx:
        case FPGAAddresses::GyroTx:
        case FPGAAddresses::ModbusSubnetERx:
            modbusResponse = &_subnetEResponse;
            break;
        case FPGAAddresses::GyroRx:
        case FPGAAddresses::ILCPowerInterlockStatus:
        case FPGAAddresses::FanCoilerHeaterInterlockStatus:
        case FPGAAddresses::AirSupplyInterlockStatus:
        case FPGAAddresses::CabinetDoorInterlockStatus:
        case FPGAAddresses::TMAMotionStopInterlockStatus:
        case FPGAAddresses::GISHeartbeatInterlockStatus:
        case FPGAAddresses::AirSupplyValveOpen:
        case FPGAAddresses::AirSupplyValveClosed:
        case FPGAAddresses::MirrorCellLightsOn:
        case FPGAAddresses::HeartbeatToSafetyController:
        case FPGAAddresses::AirSupplyValveControl:
        case FPGAAddresses::MirrorCellLightControl:
        case FPGAAddresses::DCAuxPowerNetworkAOn:
        case FPGAAddresses::DCAuxPowerNetworkBOn:
        case FPGAAddresses::DCAuxPowerNetworkCOn:
        case FPGAAddresses::DCAuxPowerNetworkDOn:
        case FPGAAddresses::DCPowerNetworkAOn:
        case FPGAAddresses::DCPowerNetworkBOn:
        case FPGAAddresses::DCPowerNetworkCOn:
        case FPGAAddresses::DCPowerNetworkDOn:
        case FPGAAddresses::Displacement:
        case FPGAAddresses::Inclinometer:
        case FPGAAddresses::ModbusSoftwareTrigger:
        case FPGAAddresses::Telemetry:
        case FPGAAddresses::Timestamp:
            break;
        default:
            break;
    }
    if (modbusResponse != 0) {
        _u16Response.push((uint16_t)modbusResponse->size());
        while (modbusResponse->size() > 0) {
            _u16Response.push(modbusResponse->front());
            modbusResponse->pop();
        }
    }
}

void SimulatedFPGA::writeTimestampFIFO(uint64_t timestamp) {}

void SimulatedFPGA::readU8ResponseFIFO(uint8_t* data, size_t length, uint32_t timeoutInMs) {}

void SimulatedFPGA::readU16ResponseFIFO(uint16_t* data, size_t length, uint32_t timeoutInMs) {
    for (size_t i = 0; i < length; ++i) {
        data[i] = _u16Response.front();
        _u16Response.pop();
    }
}

void SimulatedFPGA::writeHealthAndStatusFIFO(uint16_t request, uint16_t param) {}

void SimulatedFPGA::readHealthAndStatusFIFO(uint64_t* data, size_t length, uint32_t timeoutInMs) {
    for (size_t i = 0; i < length; i++) {
        data[i] = i;
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
