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

#include <FPGAAddresses.h>
#include <IFPGA.h>
#include <NiFpga.h>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif

#include <string.h>

using namespace LSST::M1M3::SS;

IFPGA &IFPGA::get() {
#ifdef SIMULATOR
    static SimulatedFPGA simulatedfpga;
    return simulatedfpga;
#else
    static LSST::M1M3::SS::FPGA fpga;
    return fpga;
#endif
}

uint16_t IFPGA::getTxCommand(uint8_t bus) { return FPGAAddresses::ModbusSubnetsTx[bus - 1]; }

uint16_t IFPGA::getRxCommand(uint8_t bus) { return FPGAAddresses::ModbusSubnetsRx[bus - 1]; }

uint32_t IFPGA::getIrq(uint8_t bus) {
    switch (bus) {
        case 1:
            return NiFpga_Irq_1;
        case 2:
            return NiFpga_Irq_2;
        case 3:
            return NiFpga_Irq_3;
        case 4:
            return NiFpga_Irq_4;
        case 5:
            return NiFpga_Irq_5;
        default:
            return 0;
    }
}

void IFPGA::setPower(bool aux, bool network) {
    bool a[4], n[4];
    memset(a, aux, 4);
    memset(n, network, 4);
    setPower(a, n);
}

void IFPGA::setPower(const bool aux[4], const bool network[4]) {
    uint16_t buffer[16] = {
            FPGAAddresses::DCPowerNetworkAOn,    network[0], FPGAAddresses::DCPowerNetworkBOn,    network[1],
            FPGAAddresses::DCPowerNetworkCOn,    network[2], FPGAAddresses::DCPowerNetworkDOn,    network[3],
            FPGAAddresses::DCAuxPowerNetworkAOn, aux[0],     FPGAAddresses::DCAuxPowerNetworkBOn, aux[1],
            FPGAAddresses::DCAuxPowerNetworkCOn, aux[2],     FPGAAddresses::DCAuxPowerNetworkDOn, aux[3]};
    IFPGA::get().writeCommandFIFO(buffer, 16, 0);
}
