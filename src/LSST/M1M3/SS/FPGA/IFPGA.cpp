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

#include <IFPGA.h>
#include <FPGAAddresses.h>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif

#include <string.h>

using namespace LSST::M1M3::SS;

IFPGA& IFPGA::get() {
#ifdef SIMULATOR
    static SimulatedFPGA simulatedfpga;
    return simulatedfpga;
#else
    static LSST::M1M3::SS::FPGA fpga;
    return fpga;
#endif
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
