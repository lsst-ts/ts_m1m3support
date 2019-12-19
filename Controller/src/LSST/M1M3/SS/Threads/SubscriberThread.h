/*
 * SubscriberThread.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
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
 * The subscriber thread is responsible for accepting commands.
 */
class SubscriberThread : public IThread {
private:
    M1M3SSSubscriber* subscriber;
    Controller* controller;
    M1M3SSPublisher* publisher;
    CommandFactory* commandFactory;
    bool keepRunning;

public:
    SubscriberThread(M1M3SSSubscriber* subscriber, Controller* controller, M1M3SSPublisher* publisher, CommandFactory* commandFactory);

    void run();
    void stop();

private:
    void enqueueCommandIfAvailable(Command* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SUBSCRIBERTHREAD_H_ */
