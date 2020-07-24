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

#include <ControllerThread.h>
#include <Controller.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

namespace LSST {
namespace M1M3 {
namespace SS {

ControllerThread::ControllerThread(Controller* controller) {
    _controller = controller;
    _keepRunning = true;
}

void ControllerThread::run() {
    spdlog::info("ControllerThread: Start");
    while (_keepRunning) {
        _controller->lock();
        Command* command = _controller->dequeue();
        _controller->unlock();
        if (command) {
            _controller->execute(command);
        } else {
            std::this_thread::sleep_for(100us);
        }
    }
    spdlog::info("ControllerThread: Completed");
}

void ControllerThread::stop() { _keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
