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

#include <Controller.h>
#include <Context.h>
#include <CommandFactory.h>
#include <Command.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Controller::Controller() { spdlog::debug("Controller: Controller()"); }

Controller::~Controller() { clear(); }

Controller& Controller::get() {
    static Controller controller;
    return controller;
}

void Controller::clear() {
    spdlog::trace("Controller: clear()");
    {
        std::lock_guard<std::mutex> lg(_mutex);
        while (!_queue.empty()) {
            Command* command = dequeue();
            delete command;
        }
    }
}

void Controller::enqueue(Command* command) {
    spdlog::trace("Controller: enqueue()");
    {
        std::lock_guard<std::mutex> lg(_mutex);
        _queue.push(command);
        _cv.notify_one();
    }
}

Command* Controller::dequeue() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait_for(lock, 1s);
    if (!_queue.empty()) {
        spdlog::trace("Controller: dequeue()");
        Command* command = _queue.front();
        _queue.pop();
        return command;
    }
    return NULL;
}

void Controller::execute(Command* command) {
    spdlog::trace("Controller: execute()");
    try {
        command->ackInProgress();
        command->execute();
        command->ackComplete();
    } catch (std::exception& e) {
        spdlog::error("Cannot execute command: {}", e.what());
        command->ackFailed(e.what());
    }

    delete command;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
