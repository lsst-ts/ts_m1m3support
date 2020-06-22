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
    this->commandFactory = commandFactory;
    this->controller = controller;
    this->publisher = publisher;
    this->keepRunning = true;
    pthread_mutex_init(&this->updateMutex, NULL);
}

OuterLoopClockThread::~OuterLoopClockThread() { pthread_mutex_destroy(&this->updateMutex); }

void OuterLoopClockThread::run() {
    spdlog::info("OuterLoopClockThread: Start");
    while (this->keepRunning) {
        if (IFPGA::get().waitForOuterLoopClock(1000) == 0) {
            this->controller->lock();
            if (this->keepRunning) {
                this->controller->enqueue(
                        this->commandFactory->create(Commands::UpdateCommand, &this->updateMutex));
            }
            this->controller->unlock();
            pthread_mutex_lock(&this->updateMutex);
            pthread_mutex_unlock(&this->updateMutex);
            IFPGA::get().ackOuterLoopClock();
        } else {
            spdlog::warn("OuterLoopClockThread: Failed to receive outer loop clock");
        }
    }
    spdlog::info("OuterLoopClockThread: Completed");
}

void OuterLoopClockThread::stop() { this->keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
