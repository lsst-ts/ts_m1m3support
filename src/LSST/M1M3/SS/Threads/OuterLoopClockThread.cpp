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

#include <OuterLoopClockThread.h>
#include <CommandFactory.h>
#include <Controller.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <Command.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OuterLoopClockThread::OuterLoopClockThread(CommandFactory* commandFactory, Controller* controller,
                                           M1M3SSPublisher* publisher) {
    _commandFactory = commandFactory;
    _controller = controller;
    _publisher = publisher;
    _keepRunning = true;
    pthread_mutex_init(&_updateMutex, NULL);
}

OuterLoopClockThread::~OuterLoopClockThread() { pthread_mutex_destroy(&_updateMutex); }

void OuterLoopClockThread::run() {
    spdlog::info("OuterLoopClockThread: Start");
    while (_keepRunning) {
        if (IFPGA::get().waitForOuterLoopClock(1000) == 0) {
            _controller->lock();
            if (_keepRunning) {
                _controller->enqueue(_commandFactory->create(Commands::UpdateCommand, &_updateMutex));
            }
            _controller->unlock();
            pthread_mutex_lock(&_updateMutex);
            pthread_mutex_unlock(&_updateMutex);
            IFPGA::get().ackOuterLoopClock();
        } else {
            spdlog::warn("OuterLoopClockThread: Failed to receive outer loop clock");
        }
    }
    spdlog::info("OuterLoopClockThread: Completed");
}

void OuterLoopClockThread::stop() { _keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
