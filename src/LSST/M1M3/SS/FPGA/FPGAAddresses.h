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

#ifndef FPGAADDRESSES_H_
#define FPGAADDRESSES_H_

namespace LSST {
namespace M1M3 {
namespace SS {
namespace FPGAAddresses {

/**
 * An enumeration match FPGA signals to port numbers.
 */
enum Type {
    NA = 0,
    AccelerometerAx = 1,   // Slot1/AI0
    AccelerometerAz = 2,   // Slot1/AI1
    AccelerometerBy = 3,   // Slot1/AI2
    AccelerometerBz = 4,   // Slot1/AI3
    AccelerometerCx = 5,   // Slot2/AI0
    AccelerometerCz = 6,   // Slot2/AI1
    AccelerometerDy = 7,   // Slot2/AI2
    AccelerometerDz = 8,   // Slot2/AI3
    ModbusSubnetATx = 9,   // Slot3/DIO0
    ModbusSubnetBTx = 10,  // Slot3/DIO1
    ModbusSubnetCTx = 11,  // Slot3/DIO2
    ModbusSubnetDTx = 12,  // Slot3/DIO3
    ModbusSubnetARx = 13,  // Slot3/DIO4
    ModbusSubnetBRx = 14,  // Slot3/DIO5
    ModbusSubnetCRx = 15,  // Slot3/DIO6
    ModbusSubnetDRx = 16,  // Slot3/DIO7
    ModbusSubnetETx = 17,  // Slot4/DIO0
    GyroTx = 18,           // Slot4/DIO1
    Slot4DIO2 = 19,
    Slot4DIO3 = 20,
    ModbusSubnetERx = 21,  // Slot4/DIO4
    GyroRx = 22,           // Slot4/DIO5
    Slot4DIO6 = 23,
    Slot4DIO7 = 24,
    ILCPowerInterlockStatus = 25,         // Slot5/DI0
    FanCoilerHeaterInterlockStatus = 26,  // Slot5/DI1
    Slot5DI2 = 27,
    AirSupplyInterlockStatus = 28,    // Slot5/DI3
    CabinetDoorInterlockStatus = 29,  // Slot5/DI4
    Slot5DI5 = 30,
    TMAMotionStopInterlockStatus = 31,  // Slot5/DI6
    GISHeartbeatInterlockStatus = 32,   // Slot5/DI7
    AirSupplyValveOpen = 33,            // Slot5/DI8
    AirSupplyValveClosed = 34,          // Slot5/DI9
    MirrorCellLightsOn = 35,            // Slot5/DI10
    Slot5DI11 = 36,
    Slot5DI12 = 37,
    Slot5DI13 = 38,
    Slot5DI14 = 39,
    Slot5DI15 = 40,
    Slot5DI16 = 41,
    Slot5DI17 = 42,
    Slot5DI18 = 43,
    Slot5DI19 = 44,
    Slot5DI20 = 45,
    Slot5DI21 = 46,
    Slot5DI22 = 47,
    Slot5DI23 = 48,
    Slot5DI24 = 49,
    Slot5DI25 = 50,
    Slot5DI26 = 51,
    Slot5DI27 = 52,
    Slot5DI28 = 53,
    Slot5DI29 = 54,
    Slot5DI30 = 55,
    Slot5DI31 = 56,
    HeartbeatToSafetyController = 57,              // Slot6/CH0
    CriticalFailureToSafetyController = 58,        // Slot6/CH1
    MirrorRaisingLoweringToSafetyController = 59,  // Slot6/CH2
    MirrorParkedToSafetyController = 60,           // SLot6/CH3
    AirSupplyValveControl = 61,                    // Slot6/CH4
    MirrorCellLightControl = 62,                   // Slot6/CH5
    Slot6CH6 = 63,
    Slot6CH7 = 64,
    DCAuxPowerNetworkAOn = 65,  // Slot7/CH0
    DCAuxPowerNetworkBOn = 66,  // Slot7/CH1
    DCAuxPowerNetworkCOn = 67,  // Slot7/CH2
    DCAuxPowerNetworkDOn = 68,  // Slot7/CH3
    DCPowerNetworkAOn = 69,     // Slot7/CH4
    DCPowerNetworkBOn = 70,     // Slot7/CH5
    DCPowerNetworkCOn = 71,     // Slot7/CH6
    DCPowerNetworkDOn = 72,     // Slot7/CH7
    Displacement = 73,
    Inclinometer = 74,
    Slot8Port3 = 75,
    Slot8Port4 = 76,
    ModbusSoftwareTrigger = 252,
    Telemetry = 253,
    Timestamp = 254
};

/**
 * Digital Input bytes.
 */
enum DigitalInputs {
    AirValveOpened = 0x0100,
    AirValveClosed = 0x0200,
    CellLightsOn = 0x0400,
};

/**
 * Digital Output bytes.
 */
enum DigitalOutputs {
    HeartbeatOutputState = 0x01,
    AirCommandOutputOn = 0x10,
    CellLightsOutputOn = 0x20,
};

enum PowerSupply {
    AuxA = 0x01,
    AuxB = 0x02,
    AuxC = 0x04,
    AuxD = 0x08,
    A = 0x10,
    B = 0x20,
    C = 0x40,
    D = 0x80
};

/**
 * Array of commands to transmit message.
 */
const static uint8_t ModbusSubnetsTx[] = {FPGAAddresses::ModbusSubnetATx, FPGAAddresses::ModbusSubnetBTx,
                                          FPGAAddresses::ModbusSubnetCTx, FPGAAddresses::ModbusSubnetDTx,
                                          FPGAAddresses::ModbusSubnetETx};

/**
 * Array of requests to receive message.
 */
const static uint8_t ModbusSubnetsRx[] = {FPGAAddresses::ModbusSubnetARx, FPGAAddresses::ModbusSubnetBRx,
                                          FPGAAddresses::ModbusSubnetCRx, FPGAAddresses::ModbusSubnetDRx,
                                          FPGAAddresses::ModbusSubnetERx};

} /* namespace FPGAAddresses */
} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FPGAADDRESSES_H_ */
