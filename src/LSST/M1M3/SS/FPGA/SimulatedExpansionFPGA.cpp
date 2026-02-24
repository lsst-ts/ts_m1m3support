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

#include <SimulatedExpansionFPGA.h>
#include <SimulatedFPGA.h>
#include <cstdlib>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

SimulatedExpansionFPGA::SimulatedExpansionFPGA() {
    SPDLOG_DEBUG("SimulatedExpansionFPGA: SimulatedExpansionFPGA()");
}

void SimulatedExpansionFPGA::initialize() { SPDLOG_DEBUG("SimulatedExpansionFPGA: initialize()"); }

void SimulatedExpansionFPGA::open() { SPDLOG_DEBUG("SimulatedExpansionFPGA: open()"); }

void SimulatedExpansionFPGA::close() { SPDLOG_DEBUG("SimulatedExpansionFPGA: close()"); }

void SimulatedExpansionFPGA::finalize() { SPDLOG_DEBUG("SimulatedExpansionFPGA: finalize()"); }

void SimulatedExpansionFPGA::sample() {}

void SimulatedExpansionFPGA::readSlot1(float* data) {
    data[0] = 1.0 + getRndPM1() * 0.5;
    data[1] = 1.0 + getRndPM1() * 0.5;
    data[2] = 1.0 + getRndPM1() * 0.5;
    data[3] = 1.0 + getRndPM1() * 0.5;
    data[4] = 1.0 + getRndPM1() * 0.5;
    data[5] = 1.0 + getRndPM1() * 0.5;
}

void SimulatedExpansionFPGA::readSlot2(uint32_t* data) {}
