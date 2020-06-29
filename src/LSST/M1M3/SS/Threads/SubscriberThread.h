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

#ifndef SUBSCRIBERTHREAD_H_
#define SUBSCRIBERTHREAD_H_

#include <IThread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSSubscriber;
class Controller;
class M1M3SSPublisher;
class Command;
class CommandFactory;

/*!
 * The _subscriber thread is responsible for accepting commands.
 */
class SubscriberThread : public IThread {
public:
    SubscriberThread(M1M3SSSubscriber* subscriber, Controller* controller, M1M3SSPublisher* publisher,
                     CommandFactory* commandFactory);

    void run();
    void stop();

private:
    void _enqueueCommandIfAvailable(Command* command);

    M1M3SSSubscriber* _subscriber;
    Controller* _controller;
    M1M3SSPublisher* _publisher;
    CommandFactory* _commandFactory;
    bool _keepRunning;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SUBSCRIBERTHREAD_H_ */
