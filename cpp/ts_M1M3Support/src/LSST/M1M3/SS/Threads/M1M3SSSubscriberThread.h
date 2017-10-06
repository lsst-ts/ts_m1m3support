/*
 * M1M3SSSubscriberThread.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSSUBSCRIBERTHREAD_H_
#define M1M3SSSUBSCRIBERTHREAD_H_

#include <IThread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISubscriber;
class IController;
class IPublisher;
class ICommand;
class ICommandFactory;

class M1M3SSSubscriberThread: public IThread {
private:
	ISubscriber* subscriber;
	IController* controller;
	IPublisher* publisher;
	ICommandFactory* commandFactory;
	bool keepRunning;

	void enqueueCommandIfAvailable(ICommand* command);

public:
	M1M3SSSubscriberThread(ISubscriber* subscriber, IController* controller, IPublisher* publisher, ICommandFactory* commandFactory);
	virtual ~M1M3SSSubscriberThread();

	virtual void run();
	virtual void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSSUBSCRIBERTHREAD_H_ */
