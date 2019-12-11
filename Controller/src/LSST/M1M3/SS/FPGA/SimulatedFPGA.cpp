/*
 * SimulatedFPGA.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#include <SimulatedFPGA.h>
#include <SAL_MTM1M3C.h>
#include <SAL_MTMountC.h>
#include <M1M3SSPublisher.h>
#include <FPGAAddresses.h>
#include <cstring>
#include <Timestamp.h>
#include <CRC.h>
#include <ForceActuatorApplicationSettings.h>
#include <unistd.h>
#include <cstdlib>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SimulatedFPGA::SimulatedFPGA(M1M3SSPublisher* publisher, MTMount_ElevationC* tmaElevation, MTMount_AzimuthC* tmaAzimuth, ForceActuatorApplicationSettings* forceActuatorApplicationSettings) {
    Log.Info("SimulatedFPGA: SimulatedFPGA()");
    this->publisher = publisher;
    this->tmaElevation = tmaElevation;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->tmaAzimuth = tmaAzimuth;
    this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
    this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
    this->appliedHardpointSteps = this->publisher->getEventAppliedHardpointSteps();
    this->outerLoopData = this->publisher->getOuterLoopData();
    this->lastRequest = -1;
    memset(&this->supportFPGAData, 0, sizeof(SupportFPGAData));
    this->supportFPGAData.DigitalInputStates = 0x0001 | 0x0002 | 0x0008 | 0x0010 | 0x0040 | 0x0080;
    this->hardpointActuatorData->encoder[0] = 15137;
    this->hardpointActuatorData->encoder[1] = 20079;
    this->hardpointActuatorData->encoder[2] = 26384;
    this->hardpointActuatorData->encoder[3] = 27424;
    this->hardpointActuatorData->encoder[4] = 17560;
    this->hardpointActuatorData->encoder[5] = 23546;
    for (int i = 0; i < RND_CNT; ++i) {
        this->rnd[i] = float((rand() % 2000) - 1000) / 1000.0;
    }
    this->rndIndex = 0;
}

float SimulatedFPGA::getRnd() {
    ++this->rndIndex;
    if (this->rndIndex > RND_CNT) {
        this->rndIndex = 0;
    }
    return this->rnd[rndIndex];
}

int32_t SimulatedFPGA::initialize() {
    Log.Debug("SimulatedFPGA: initialize()");
    return 0;
}

int32_t SimulatedFPGA::open() {
    Log.Debug("SimulatedFPGA: open()");
    return 0;
}

int32_t SimulatedFPGA::close() {
    Log.Debug("SimulatedFPGA: close()");
    return 0;
}

int32_t SimulatedFPGA::finalize() {
    Log.Debug("SimulatedFPGA: finalize()");
    return 0;
}

bool SimulatedFPGA::isErrorCode(int32_t status) {
    return status != 0;
}

int32_t SimulatedFPGA::waitForOuterLoopClock(int32_t timeout) {
    usleep(20000);
    return 0;
}

int32_t SimulatedFPGA::ackOuterLoopClock() {
    return 0;
}

int32_t SimulatedFPGA::waitForPPS(int32_t timeout) {
    usleep(1000000);
    return 0;
}

int32_t SimulatedFPGA::ackPPS() {
    return 0;
}

int32_t SimulatedFPGA::waitForModbusIRQ(int32_t subnet, int32_t timeout) {
    return 0;
}

int32_t SimulatedFPGA::ackModbusIRQ(int32_t subnet) {
    return 0;
}

void SimulatedFPGA::pullTelemetry() {
    Log.Trace("SimulatedFPGA: pullTelemetry()");
    uint64_t timestamp = Timestamp::toRaw(this->publisher->getTimestamp());
    this->supportFPGAData.Reserved = 0;
    this->supportFPGAData.InclinometerTxBytes = 0;
    this->supportFPGAData.InclinometerRxBytes = 0;
    this->supportFPGAData.InclinometerTxFrames = 0;
    this->supportFPGAData.InclinometerRxFrames = 0;
    this->supportFPGAData.InclinometerErrorTimestamp = 0;
    this->supportFPGAData.InclinometerErrorCode = 0;
    this->supportFPGAData.InclinometerSampleTimestamp = timestamp;
    this->supportFPGAData.InclinometerAngleRaw = (int32_t)(this->tmaElevation->Elevation_Angle_Actual * 1000.0) + (this->getRnd() * 5.0);
    this->supportFPGAData.DisplacementTxBytes = 0;
    this->supportFPGAData.DisplacementRxBytes = 0;
    this->supportFPGAData.DisplacementTxFrames = 0;
    this->supportFPGAData.DisplacementRxFrames = 0;
    this->supportFPGAData.DisplacementErrorTimestamp = 0;
    this->supportFPGAData.DisplacementErrorCode = 0;
    this->supportFPGAData.DisplacementSampleTimestamp = timestamp;
    this->supportFPGAData.DisplacementRaw1 = (int32_t)(this->getRnd() + 10) * 1000.0;
    this->supportFPGAData.DisplacementRaw2 = (int32_t)(this->getRnd() + 20) * 1000.0;
    this->supportFPGAData.DisplacementRaw3 = (int32_t)(this->getRnd() + 30) * 1000.0;
    this->supportFPGAData.DisplacementRaw4 = (int32_t)(this->getRnd() + 40) * 1000.0;
    this->supportFPGAData.DisplacementRaw5 = (int32_t)(this->getRnd() + 50) * 1000.0;
    this->supportFPGAData.DisplacementRaw6 = (int32_t)(this->getRnd() + 60) * 1000.0;
    this->supportFPGAData.DisplacementRaw7 = (int32_t)(this->getRnd() + 70) * 1000.0;
    this->supportFPGAData.DisplacementRaw8 = (int32_t)(this->getRnd() + 80) * 1000.0;
    this->supportFPGAData.AccelerometerSampleCount++;
    this->supportFPGAData.AccelerometerSampleTimestamp = timestamp;
    this->supportFPGAData.AccelerometerRaw1 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw2 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw3 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw4 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw5 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw6 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw7 = this->getRnd() * 0.01;
    this->supportFPGAData.AccelerometerRaw8 = this->getRnd() * 0.01;
    this->supportFPGAData.GyroTxBytes = 0;
    this->supportFPGAData.GyroRxBytes = 0;
    this->supportFPGAData.GyroTxFrames = 0;
    this->supportFPGAData.GyroRxFrames = 0;
    this->supportFPGAData.GyroErrorTimestamp = 0;
    this->supportFPGAData.GyroErrorCode = 0;
    this->supportFPGAData.GyroSampleTimestamp = timestamp;
    this->supportFPGAData.GyroRawX = this->getRnd() * 0.01;
    this->supportFPGAData.GyroRawY = this->getRnd() * 0.01;
    this->supportFPGAData.GyroRawZ = this->getRnd() * 0.01;
    this->supportFPGAData.GyroStatus = 0x7F;
    this->supportFPGAData.GyroSequenceNumber++;
    if (this->supportFPGAData.GyroSequenceNumber > 127) {
        this->supportFPGAData.GyroSequenceNumber = 0;
    }
    this->supportFPGAData.GyroTemperature = 24 + int(this->getRnd() * 2);
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

int32_t SimulatedFPGA::writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    for (int i = 0; i < length;) {
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
                response = &this->subnetAResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetBTx:
                subnet = 2;
                response = &this->subnetBResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetCTx:
                subnet = 3;
                response = &this->subnetCResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetDTx:
                subnet = 4;
                response = &this->subnetDResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::ModbusSubnetARx:
            case FPGAAddresses::ModbusSubnetBRx:
            case FPGAAddresses::ModbusSubnetCRx:
            case FPGAAddresses::ModbusSubnetDRx:
                break;
            case FPGAAddresses::ModbusSubnetETx:
                subnet = 5;
                response = &this->subnetEResponse;
                dataLength = data[i++];
                break;
            case FPGAAddresses::GyroTx: {
                int size = data[i++]; // Read Tx Size
                i += size;            // Read All Tx Data
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
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.DigitalOutputStates & 0xFE) | (state ? 0x01 : 0x00);
                break;
            }
            case FPGAAddresses::AirSupplyValveControl: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.DigitalOutputStates & 0xEF) | (state ? 0x00 : 0x10);
                this->supportFPGAData.DigitalInputStates = (this->supportFPGAData.DigitalInputStates & 0xFCFF) | (state ? 0x0200 : 0x0100);
                break;
            }
            case FPGAAddresses::MirrorCellLightControl: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.DigitalOutputStates & 0xDF) | (state ? 0x20 : 0x00);
                this->supportFPGAData.DigitalInputStates = (this->supportFPGAData.DigitalInputStates & 0xFBFF) | (state ? 0x0400 : 0x0000);
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkAOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xFE) | (state ? 0x01 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkBOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xFD) | (state ? 0x02 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkCOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xFB) | (state ? 0x04 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCAuxPowerNetworkDOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xF7) | (state ? 0x08 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkAOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xEF) | (state ? 0x10 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkBOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xDF) | (state ? 0x20 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkCOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0xBF) | (state ? 0x40 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::DCPowerNetworkDOn: {
                int state = data[i++];
                this->supportFPGAData.DigitalOutputStates = (this->supportFPGAData.PowerSupplyStates & 0x7F) | (state ? 0x80 : 0x00);
                // TODO: Set Power Supply Currents
                break;
            }
            case FPGAAddresses::Displacement:
            case FPGAAddresses::Inclinometer:
            case FPGAAddresses::ModbusSoftwareTrigger:
            case FPGAAddresses::Telemetry:
            case FPGAAddresses::HealthAndStatus:
            case FPGAAddresses::Timestamp:
                break;
            default:
                break;
        }
        if (response != 0) {
            bool stepMotorBroadcast = false;
            ++i; // Read Software Trigger
            // Response length is prepended at request
            uint64_t rawTimestamp = Timestamp::toRaw(this->publisher->getTimestamp());
            response->push((uint16_t)(rawTimestamp >> 48));
            response->push((uint16_t)(rawTimestamp >> 32));
            response->push((uint16_t)(rawTimestamp >> 16));
            response->push((uint16_t)rawTimestamp); // Write Global Timestamp
            int endIndex = i - 1 + dataLength - 1;
            // The first -1 is for the software trigger
            // The second -1 is for the trigger
            while (i < endIndex) {
                uint16_t address = this->readModbus(data[i++]);  // Read Address
                uint16_t function = this->readModbus(data[i++]); // Read Function Code
                if (address == 248 && function == 66) {
                    stepMotorBroadcast = true;
                }
                if (address == 248 || address == 249) {
                    // This is a broadcast, no response, just read until 0xA000
                    while ((data[i] & 0xF000) != 0x2000) {
                        ++i;
                    }
                    i--; // Shift to CRC for the remainder of the time
                         // However broadcasts are followed by a timestamp
                         // so only shift 1
                } else if (address == 0 && function == 0) {
                    // This is a special nop, read until 0xF000 != 0
                    while (data[i] == 0) {
                        ++i;
                    }
                    i -= 4; // Shift to CRC for the rest of the parsing to work
                } else if (subnet >= 1 && subnet <= 4) {
                    int zIndex = -1;
                    for (int j = 0; j < FA_COUNT; ++j) {
                        if (this->forceActuatorApplicationSettings->Table[j].Subnet == subnet && this->forceActuatorApplicationSettings->Table[j].Address == address) {
                            zIndex = j;
                            break;
                        }
                    }
                    int pIndex = zIndex;
                    int sIndex = this->forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[zIndex];
                    switch (function) {
                        case 17:                                   // Report Server Id
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 12);       // Write Message Length
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, signal);
                            this->writeModbus(response, address); // Write ILC Unique Id
                            this->writeModbus(response, 0);       // TODO: Write ILC Application Type
                            this->writeModbus(response, 0);       // TODO: Write Network Node Type
                            this->writeModbus(response, 0);       // TODO: Write ILC Selected Options
                            this->writeModbus(response, 0);       // TODO: Write Network Node Options
                            this->writeModbus(response, 99);      // TODO: Write Major Revision
                            this->writeModbus(response, 99);      // TODO: Write Minor Revision
                            // TODO: Write Firmware Name
                            this->writeModbusCRC(response);
                            break;
                        case 18: // Report Server Status
                            this->writeModbus(response, address);
                            this->writeModbus(response, function);
                            this->writeModbus(response, 0); // TODO: Write IlC State
                            this->writeModbus(response, 0);
                            this->writeModbus(response, 0); // TODO: Write ILC Status
                            this->writeModbus(response, 0);
                            this->writeModbus(response, 0); // TODO: Write ILC Faults
                            this->writeModbusCRC(response);
                            break;
                        case 65: {                                          // Change ILC Mode
                            ++i;                                            // Read Reserved Byte
                            uint16_t newMode = this->readModbus(data[i++]); // Read New Mode
                            this->writeModbus(response, address);           // Write Address
                            this->writeModbus(response, function);          // Write Function
                            this->writeModbus(response, 0);                 // Write Reserved Byte
                            this->writeModbus(response, newMode);           // Write ILC State
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 73:                                   // Set Boost Valve DCA Gains
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            i += 4;                                // Read Primary Cylinder DCA Gain
                            i += 4;                                // Read Secondary Cylinder DCA Gain
                            this->writeModbusCRC(response);
                            break;
                        case 74: {                                 // Read Boost Valve DCA Gains
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            uint8_t buffer[4];
                            float gain = 1.0;
                            memcpy(buffer, &gain, 4);
                            this->writeModbus(response, buffer[3]);
                            this->writeModbus(response, buffer[2]);
                            this->writeModbus(response, buffer[1]);
                            this->writeModbus(response, buffer[0]); // Write Primary Cylinder DCA Gain
                            this->writeModbus(response, buffer[3]);
                            this->writeModbus(response, buffer[2]);
                            this->writeModbus(response, buffer[1]);
                            this->writeModbus(response, buffer[0]); // Write Secondary Cylinder DCA Gain
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 75: { // Force Demand
                            ++i;   // Read Slew Flag
                            uint16_t word1 = this->readModbus(data[i++]);
                            uint16_t word2 = this->readModbus(data[i++]);
                            uint16_t word3 = this->readModbus(data[i++]);
                            this->writeModbus(response, address);                                        // Write Address
                            this->writeModbus(response, function);                                       // Write Function
                            this->writeModbus(response, (uint8_t)this->outerLoopData->broadcastCounter); // Write ILC Status
                            uint8_t buffer[4];
                            float force = (((float)((word1 << 16) | (word2 << 8) | word3)) / 1000.0) + (this->getRnd() * 0.5);
                            memcpy(buffer, &force, 4);
                            this->writeModbus(response, buffer[3]);
                            this->writeModbus(response, buffer[2]);
                            this->writeModbus(response, buffer[1]);
                            this->writeModbus(response, buffer[0]); // Write Primary Cylinder Force
                            if (address > 16) {
                                word1 = this->readModbus(data[i++]);
                                word2 = this->readModbus(data[i++]);
                                word3 = this->readModbus(data[i++]);
                                force = (((float)((word1 << 16) | (word2 << 8) | word3)) / 1000.0) + (this->getRnd() * 0.5);
                                memcpy(buffer, &force, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write Secondary Cylinder Force
                            }
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 76: {                                                                       // Force And Status
                            this->writeModbus(response, address);                                        // Write Address
                            this->writeModbus(response, function);                                       // Write Function
                            this->writeModbus(response, (uint8_t)this->outerLoopData->broadcastCounter); // Write ILC Status
                            uint8_t buffer[4];
                            float force = (((float)this->appliedCylinderForces->primaryCylinderForces[pIndex]) / 1000.0) + (this->getRnd() * 0.5); // Update to Primary Cylinder Force
                            memcpy(buffer, &force, 4);
                            this->writeModbus(response, buffer[3]);
                            this->writeModbus(response, buffer[2]);
                            this->writeModbus(response, buffer[1]);
                            this->writeModbus(response, buffer[0]); // Write Primary Cylinder Force
                            if (address > 16) {
                                force = (((float)this->appliedCylinderForces->secondaryCylinderForces[sIndex]) / 1000.0) + (this->getRnd() * 0.5); // Update to Secondary Cylinder Force
                                memcpy(buffer, &force, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write Secondary Cylinder Force
                            }
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 80: { // ADC Scan Rate
                            uint16_t scanRate = this->readModbus(data[i++]);
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, scanRate); // Write ADC Scan Rate
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 81:                                   // ADC Channel Offset and Sensitivity
                            ++i;                                   // Read ADC Channel
                            i += 4;                                // Read Offset
                            i += 4;                                // Read Sensitivity
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbusCRC(response);
                            break;
                        case 107:                                  // Reset
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbusCRC(response);
                            break;
                        case 110: {                                // Read Calibration
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            for (int j = 0; j < 24; ++j) {
                                uint8_t buffer[4];
                                float value = 0.0;
                                memcpy(buffer, &value, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write Calibration Data
                            }
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 119: {                                // Read DCA Pressure Values
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            for (int j = 0; j < 4; ++j) {
                                uint8_t buffer[4];
                                float value = 120.0;
                                memcpy(buffer, &value, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write DCA Pressure
                            }
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 120:                                  // Read DCA Id
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x01);
                            this->writeModbus(response, signal);
                            this->writeModbus(response, address); // Write DCA Unique Id
                            this->writeModbus(response, 0);       // TODO: Write DCA Firmware Type
                            this->writeModbus(response, 99);      // TODO: Write Major Revision
                            this->writeModbus(response, 99);      // TODO: Write Minor Revision
                            this->writeModbusCRC(response);
                            break;
                        case 121:                                  // Read DCA Status
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00); // TODO: Write DCA Status
                            this->writeModbusCRC(response);
                            break;
                        default:
                            break;
                    }
                } else if (subnet == 5 && address >= 1 && address <= 6) {
                    switch (function) {
                        case 17:                                   // Report Server Id
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 12);       // Write Message Length
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, signal);
                            this->writeModbus(response, address); // Write ILC Unique Id
                            this->writeModbus(response, 0);       // TODO: Write ILC Application Type
                            this->writeModbus(response, 0);       // TODO: Write Network Node Type
                            this->writeModbus(response, 0);       // TODO: Write ILC Selected Options
                            this->writeModbus(response, 0);       // TODO: Write Network Node Options
                            this->writeModbus(response, 99);      // TODO: Write Major Revision
                            this->writeModbus(response, 99);      // TODO: Write Minor Revision
                            // TODO: Write Firmware Name
                            this->writeModbusCRC(response);
                            break;
                        case 18: // Report Server Status
                            this->writeModbus(response, address);
                            this->writeModbus(response, function);
                            this->writeModbus(response, 0); // TODO: Write IlC State
                            this->writeModbus(response, 0);
                            this->writeModbus(response, 0); // TODO: Write ILC Status
                            this->writeModbus(response, 0);
                            this->writeModbus(response, 0); // TODO: Write ILC Faults
                            this->writeModbusCRC(response);
                            break;
                        case 65: {                                          // Change ILC Mode
                            ++i;                                            // Read Reserved Byte
                            uint16_t newMode = this->readModbus(data[i++]); // Read New Mode
                            this->writeModbus(response, address);           // Write Address
                            this->writeModbus(response, function);          // Write Function
                            this->writeModbus(response, 0);                 // Write Reserved Byte
                            this->writeModbus(response, newMode);           // Write ILC State
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 66: {                                                                       // Step Motor
                            int steps = data[i++];                                                       // Read Steps
                            this->writeModbus(response, address);                                        // Write Address
                            this->writeModbus(response, function);                                       // Write Function
                            this->writeModbus(response, (uint8_t)this->outerLoopData->broadcastCounter); // Write ILC Status
                            // Number of steps issued / 4 + current encoder
                            // The encoder is also inverted after being received to match axis direction
                            // So we have to also invert the encoder here to counteract that
                            if (steps < 4 && steps > 0) {
                                steps = 4;
                            } else if (steps > -4 && steps < 0) {
                                steps = -4;
                            }
                            int32_t encoder = -this->hardpointActuatorData->encoder[address - 1] - (steps / 4);
                            this->writeModbus(response, (encoder >> 24) & 0xFF);
                            this->writeModbus(response, (encoder >> 16) & 0xFF);
                            this->writeModbus(response, (encoder >> 8) & 0xFF);
                            this->writeModbus(response, encoder & 0xFF); // Write Encoder
                            uint8_t buffer[4];
                            float force = this->getRnd() * 8.0;
                            memcpy(buffer, &force, 4);
                            this->writeModbus(response, buffer[3]);
                            this->writeModbus(response, buffer[2]);
                            this->writeModbus(response, buffer[1]);
                            this->writeModbus(response, buffer[0]); // Write Measured Force
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 67: { // Force And Status
                            int steps = this->appliedHardpointSteps->commandedSteps[address - 1];
                            this->writeModbus(response, address);                                        // Write Address
                            this->writeModbus(response, function);                                       // Write Function
                            this->writeModbus(response, (uint8_t)this->outerLoopData->broadcastCounter); // Write ILC Status
                            // Number of steps issued / 4 + current encoder
                            // The encoder is also inverted after being received to match axis direction
                            // So we have to also invert the encoder here to counteract that
                            if (steps < 4 && steps > 0) {
                                steps = 4;
                            } else if (steps > -4 && steps < 0) {
                                steps = -4;
                            }
                            int32_t encoder = -this->hardpointActuatorData->encoder[address - 1];
                            if (stepMotorBroadcast) {
                                encoder = encoder - (steps / 4);
                            }
                            this->writeModbus(response, (encoder >> 24) & 0xFF);
                            this->writeModbus(response, (encoder >> 16) & 0xFF);
                            this->writeModbus(response, (encoder >> 8) & 0xFF);
                            this->writeModbus(response, encoder & 0xFF); // Write Encoder
                            uint8_t buffer[4];
                            float force = this->getRnd() * 8.0;
                            memcpy(buffer, &force, 4);
                            this->writeModbus(response, buffer[3]);
                            this->writeModbus(response, buffer[2]);
                            this->writeModbus(response, buffer[1]);
                            this->writeModbus(response, buffer[0]); // Write Measured Force
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 80: { // ADC Scan Rate
                            uint16_t scanRate = this->readModbus(data[i++]);
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, scanRate); // Write ADC Scan Rate
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 81:                                   // ADC Channel Offset and Sensitivity
                            ++i;                                   // Read ADC Channel
                            i += 4;                                // Read Offset
                            i += 4;                                // Read Sensitivity
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbusCRC(response);
                            break;
                        case 107:                                  // Reset
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbusCRC(response);
                            break;
                        case 110: {                                // Read Calibration
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            for (int j = 0; j < 24; ++j) {
                                uint8_t buffer[4];
                                float value = 0.0;
                                memcpy(buffer, &value, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write Calibration Data
                            }
                            this->writeModbusCRC(response);
                            break;
                        }
                        default:
                            break;
                    }
                } else if (subnet == 5 && address >= 84 && address <= 89) {
                    switch (function) {
                        case 17:                                   // Report Server Id
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 12);       // Write Message Length
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, signal);
                            this->writeModbus(response, address); // Write ILC Unique Id
                            this->writeModbus(response, 0);       // TODO: Write ILC Application Type
                            this->writeModbus(response, 0);       // TODO: Write Network Node Type
                            this->writeModbus(response, 0);       // TODO: Write ILC Selected Options
                            this->writeModbus(response, 0);       // TODO: Write Network Node Options
                            this->writeModbus(response, 99);      // TODO: Write Major Revision
                            this->writeModbus(response, 99);      // TODO: Write Minor Revision
                            // TODO: Write Firmware Name
                            this->writeModbusCRC(response);
                            break;
                        case 18: // Report Server Status
                            this->writeModbus(response, address);
                            this->writeModbus(response, function);
                            this->writeModbus(response, 0); // TODO: Write IlC State
                            this->writeModbus(response, 0);
                            this->writeModbus(response, 0); // TODO: Write ILC Status
                            this->writeModbus(response, 0);
                            this->writeModbus(response, 0); // TODO: Write ILC Faults
                            this->writeModbusCRC(response);
                            break;
                        case 65: {                                          // Change ILC Mode
                            ++i;                                            // Read Reserved Byte
                            uint16_t newMode = this->readModbus(data[i++]); // Read New Mode
                            this->writeModbus(response, address);           // Write Address
                            this->writeModbus(response, function);          // Write Function
                            this->writeModbus(response, 0);                 // Write Reserved Byte
                            this->writeModbus(response, newMode);           // Write ILC State
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 107:                                  // Reset
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbusCRC(response);
                            break;
                        case 119: {                                // Read DCA Pressure Values
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            for (int j = 0; j < 4; ++j) {
                                uint8_t buffer[4];
                                float value = 120.0;
                                memcpy(buffer, &value, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write DCA Pressure
                            }
                            this->writeModbusCRC(response);
                            break;
                        }
                        case 120:                                  // Read DCA Id
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x01);
                            this->writeModbus(response, signal);
                            this->writeModbus(response, address); // Write DCA Unique Id
                            this->writeModbus(response, 0);       // TODO: Write DCA Firmware Type
                            this->writeModbus(response, 99);      // TODO: Write Major Revision
                            this->writeModbus(response, 99);      // TODO: Write Minor Revision
                            this->writeModbusCRC(response);
                            break;
                        case 121:                                  // Read DCA Status
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            this->writeModbus(response, 0x00);
                            this->writeModbus(response, 0x00); // TODO: Write DCA Status
                            this->writeModbusCRC(response);
                            break;
                        case 122:                                  // Report LVDT
                            this->writeModbus(response, address);  // Write Address
                            this->writeModbus(response, function); // Write Function
                            for (int j = 0; j < 2; ++j) {
                                uint8_t buffer[4];
                                float value = 0.0;
                                memcpy(buffer, &value, 4);
                                this->writeModbus(response, buffer[3]);
                                this->writeModbus(response, buffer[2]);
                                this->writeModbus(response, buffer[1]);
                                this->writeModbus(response, buffer[0]); // Write LVDT
                            }
                            this->writeModbusCRC(response);
                            break;
                        default:
                            break;
                    }
                }
                i += 2; // Read CRC
                ++i;    // Read End of Frame
                ++i;    // Read Wait for Rx or Delay
            }
            ++i; // Read Write Trigger IRQ
        }
    }
    return 0;
}

void SimulatedFPGA::writeModbus(std::queue<uint16_t>* response, uint16_t data) {
    this->crcVector.push(data);
    response->push((data << 1) | 0x9000);
}

void SimulatedFPGA::writeModbusCRC(std::queue<uint16_t>* response) {
    uint16_t buffer[256];
    int i = 0;
    while (!this->crcVector.empty()) {
        buffer[i] = this->crcVector.front();
        i++;
        this->crcVector.pop();
    }
    uint16_t crc = CRC::modbus(buffer, 0, i);
    response->push((((crc >> 0) & 0xFF) << 1) | 0x9000);
    response->push((((crc >> 8) & 0xFF) << 1) | 0x9000); // Write CRC
    for (int i = 0; i < 8; ++i) {
        response->push(0xB000); // Write Timestamp
    }
    response->push(0xA000); // Write End of Frame
}

uint16_t SimulatedFPGA::readModbus(uint16_t data) {
    return (data >> 1) & 0xFF;
}

int32_t SimulatedFPGA::writeCommandFIFO(uint16_t data, int32_t timeoutInMs) {
    return 0;
}

int32_t SimulatedFPGA::writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
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
            modbusResponse = &this->subnetAResponse;
            break;
        case FPGAAddresses::ModbusSubnetBRx:
            modbusResponse = &this->subnetBResponse;
            break;
        case FPGAAddresses::ModbusSubnetCRx:
            modbusResponse = &this->subnetCResponse;
            break;
        case FPGAAddresses::ModbusSubnetDRx:
            modbusResponse = &this->subnetDResponse;
            break;
        case FPGAAddresses::ModbusSubnetETx:
        case FPGAAddresses::GyroTx:
        case FPGAAddresses::ModbusSubnetERx:
            modbusResponse = &this->subnetEResponse;
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
        case FPGAAddresses::HealthAndStatus:
        case FPGAAddresses::Timestamp:
            break;
        default:
            break;
    }
    if (modbusResponse != 0) {
        this->u16Response.push((uint16_t)modbusResponse->size());
        while (modbusResponse->size() > 0) {
            this->u16Response.push(modbusResponse->front());
            modbusResponse->pop();
        }
    }
    return 0;
}

int32_t SimulatedFPGA::writeRequestFIFO(uint16_t data, int32_t timeoutInMs) {
    uint16_t newData[1];
    newData[0] = data;
    return this->writeRequestFIFO(newData, 1, timeoutInMs);
}

int32_t SimulatedFPGA::writeTimestampFIFO(uint64_t timestamp) {
    return 0;
}

int32_t SimulatedFPGA::readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) {
    return 0;
}

int32_t SimulatedFPGA::readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) {
    for (int i = 0; i < length; ++i) {
        data[i] = this->u16Response.front();
        this->u16Response.pop();
    }
    return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
