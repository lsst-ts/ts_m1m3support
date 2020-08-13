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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <Command.h>

#include <queue>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * The main controller for the application.
 */
class Controller {
public:
    Controller();
    ~Controller();

    static Controller& get();

    void lock();
    void unlock();

    void clear();

    void enqueue(Command* command);
    Command* dequeue();

    /**
     * Executes command. Command is deleted after it is executed.
     *
     * @param command command to execute
     */
    void execute(Command* command);

private:
    Controller& operator=(const Controller&) = delete;
    Controller(const Controller&) = delete;

    pthread_mutex_t _mutex;
    std::queue<Command*> _queue;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTROLLER_H_ */
