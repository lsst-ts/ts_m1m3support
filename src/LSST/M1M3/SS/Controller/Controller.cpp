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

Controller::Controller() {
    spdlog::debug("Controller: Controller()");
    pthread_mutex_init(&_mutex, NULL);
}

Controller::~Controller() {
    this->clear();
    pthread_mutex_destroy(&_mutex);
}

Controller& Controller::get() {
    static Controller controller;
    return controller;
}

void Controller::lock() {
    spdlog::trace("Controller: lock()");
    pthread_mutex_lock(&_mutex);
}

void Controller::unlock() {
    spdlog::trace("Controller: unlock()");
    pthread_mutex_unlock(&_mutex);
}

void Controller::clear() {
    spdlog::trace("Controller: clear()");
    this->lock();
    while (!_queue.empty()) {
        Command* command = this->dequeue();
        delete command;
    }
    this->unlock();
}

void Controller::enqueue(Command* command) {
    spdlog::trace("Controller: enqueue()");
    _queue.push(command);
}

Command* Controller::dequeue() {
    spdlog::trace("Controller: dequeue()");
    if (!_queue.empty()) {
        Command* command = _queue.front();
        _queue.pop();
        return command;
    }
    return 0;
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
