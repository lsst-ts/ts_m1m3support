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

#include <FPGA.h>
#include <FPGAAddresses.h>
#include <M1M3SSPublisher.h>
#include <PPSThread.h>
#include <Timestamp.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

PPSThread::PPSThread() { _keepRunning = true; }

void PPSThread::run() {
    SPDLOG_INFO("PPSThread: Start");
    while (_keepRunning) {
        try {
            IFPGA::get().waitForPPS(2500);
        } catch (std::runtime_error &er) {
            SPDLOG_WARN("PPSThread: Failed to receive pps: {}", er.what());
            continue;
        }
        IFPGA::get().ackPPS();
        uint64_t timestamp = Timestamp::toFPGA(M1M3SSPublisher::instance().getTimestamp());
        if (_keepRunning) {
            IFPGA::get().writeTimestampFIFO(timestamp);
        }
    }
    SPDLOG_INFO("PPSThread: Completed");
}

void PPSThread::stop() { _keepRunning = false; }
