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

#include <spdlog/spdlog.h>

#include <Command.h>
#include <ControllerThread.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <M1M3SSPublisher.h>
#include <OuterLoopClockThread.h>
#include <Timestamp.h>
#include <UpdateCommand.h>

using namespace LSST::M1M3::SS;

OuterLoopClockThread::OuterLoopClockThread() { _keepRunning = true; }

void OuterLoopClockThread::run() {
  SPDLOG_INFO("OuterLoopClockThread: Start");
  while (_keepRunning) {
    try {
      IFPGA::get().waitForOuterLoopClock(1000);
    } catch (std::runtime_error &er) {
      SPDLOG_WARN(
          "OuterLoopClockThread: Failed to receive outer loop clock: {}",
          er.what());
    }

    if (_keepRunning) {
      ControllerThread::get().enqueue(new UpdateCommand(&_updateMutex));
    }
    _updateMutex.lock();
    _updateMutex.unlock();
    IFPGA::get().ackOuterLoopClock();
  }
  SPDLOG_INFO("OuterLoopClockThread: Completed");
}

void OuterLoopClockThread::stop() { _keepRunning = false; }
