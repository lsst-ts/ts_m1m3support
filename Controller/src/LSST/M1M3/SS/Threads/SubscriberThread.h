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

class ISubscriber;
class IController;
class IPublisher;
class ICommand;
class ICommandFactory;

class SubscriberThread: public IThread {
private:
	ISubscriber* subscriber;
	IController* controller;
	IPublisher* publisher;
	ICommandFactory* commandFactory;
	bool keepRunning;

public:
	SubscriberThread(ISubscriber* subscriber, IController* controller, IPublisher* publisher, ICommandFactory* commandFactory);

	void run();
	void stop();

private:
	void enqueueCommandIfAvailable(ICommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SUBSCRIBERTHREAD_H_ */
