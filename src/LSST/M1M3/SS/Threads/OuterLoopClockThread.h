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

#ifndef OUTERLOOPCLOCKTHREAD_H_
#define OUTERLOOPCLOCKTHREAD_H_

#include <IThread.h>
#include <DataTypes.h>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class CommandFactory;
class Controller;
class FPGA;
class M1M3SSPublisher;

/*!
 * The outer loop clock thread is responsible for controlling
 * the outer loop execution timing.
 */
class OuterLoopClockThread : public IThread {
public:
    OuterLoopClockThread(CommandFactory* commandFactory, Controller* controller, M1M3SSPublisher* publisher);
    ~OuterLoopClockThread();

    void run();
    void stop();

private:
    CommandFactory* _commandFactory;
    Controller* _controller;
    M1M3SSPublisher* _publisher;
    uint16_t _timestampUpdateBuffer[5];
    bool _keepRunning;
    pthread_mutex_t _updateMutex;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* OUTERLOOPCLOCKTHREAD_H_ */
