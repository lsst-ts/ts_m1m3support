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

#include <PPSThread.h>
#include <FPGA.h>
#include <M1M3SSPublisher.h>
#include <spdlog/spdlog.h>
#include <Timestamp.h>
#include <FPGAAddresses.h>
#include <NiError.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PPSThread::PPSThread() { _keepRunning = true; }

void PPSThread::run() {
    spdlog::info("PPSThread: Start");
    while (_keepRunning) {
        try {
            IFPGA::get().waitForPPS(2500);
        } catch (NiError& er) {
            spdlog::warn("PPSThread: Failed to receive pps");
            continue;
        }
        IFPGA::get().ackPPS();
        uint64_t timestamp = Timestamp::toFPGA(M1M3SSPublisher::get().getTimestamp());
        if (_keepRunning) {
            IFPGA::get().writeTimestampFIFO(timestamp);
        }
    }
    spdlog::info("PPSThread: Completed");
}

void PPSThread::stop() { _keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
