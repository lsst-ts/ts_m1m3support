/*
 * M1M3SSSubscriberThread.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSSubscriberThread.h>
#include <ISubscriber.h>
#include <IController.h>
#include <IPublisher.h>
#include <ICommandFactory.h>
#include <ICommand.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSSubscriberThread::M1M3SSSubscriberThread(ISubscriber* subscriber, IController* controller, IPublisher* publisher, ICommandFactory* commandFactory) {
	this->subscriber = subscriber;
	this->controller = controller;
	this->publisher = publisher;
	this->commandFactory = commandFactory;
	this->keepRunning = true;
}

M1M3SSSubscriberThread::~M1M3SSSubscriberThread() {
	// TODO Auto-generated destructor stub
}

void M1M3SSSubscriberThread::run() {
	while(this->keepRunning) {
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandStart());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandEnable());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandDisable());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandStandby());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandShutdown());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnAirOn());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnAirOff());
		usleep(100);
	}
}

void M1M3SSSubscriberThread::stop() {
	this->keepRunning = false;
}

void M1M3SSSubscriberThread::enqueueCommandIfAvailable(ICommand* command) {
	if (command) {
		if(command->validate()) {
			this->controller->lock();
			this->controller->enqueue(command);
			this->controller->unlock();
		}
		else {
			command->ackFailed("Validation");
			this->commandFactory->destroy(command);
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
