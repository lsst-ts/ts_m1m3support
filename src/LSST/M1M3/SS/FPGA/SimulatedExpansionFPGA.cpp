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

#include <SimulatedExpansionFPGA.h>
#include <cstdlib>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SimulatedExpansionFPGA::SimulatedExpansionFPGA() {
    spdlog::debug("SimulatedExpansionFPGA: SimulatedExpansionFPGA()");

    for (int i = 0; i < RND_CNT; ++i) {
        _rnd[i] = float((rand() % 2000) - 1000) / 1000.0;
    }
    _rndIndex = 0;
}

float SimulatedExpansionFPGA::_getRnd() {
    ++_rndIndex;
    if (_rndIndex > RND_CNT) {
        _rndIndex = 0;
    }
    return _rnd[_rndIndex];
}

int32_t SimulatedExpansionFPGA::initialize() {
    spdlog::debug("SimulatedExpansionFPGA: initialize()");
    return 0;
}

int32_t SimulatedExpansionFPGA::open() {
    spdlog::debug("SimulatedExpansionFPGA: open()");
    return 0;
}

int32_t SimulatedExpansionFPGA::close() {
    spdlog::debug("SimulatedExpansionFPGA: close()");
    return 0;
}

int32_t SimulatedExpansionFPGA::finalize() {
    spdlog::debug("SimulatedExpansionFPGA: finalize()");
    return 0;
}

bool SimulatedExpansionFPGA::isErrorCode(int32_t status) { return status != 0; }

int32_t SimulatedExpansionFPGA::sample() { return 0; }

int32_t SimulatedExpansionFPGA::readSlot1(float* data) {
    data[0] = 1.0 + _getRnd() * 0.5;
    data[1] = 1.0 + _getRnd() * 0.5;
    data[2] = 1.0 + _getRnd() * 0.5;
    data[3] = 1.0 + _getRnd() * 0.5;
    data[4] = 1.0 + _getRnd() * 0.5;
    data[5] = 1.0 + _getRnd() * 0.5;
    return 0;
}

int32_t SimulatedExpansionFPGA::readSlot2(uint32_t* data) { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
