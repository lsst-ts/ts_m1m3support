/*
 * Controller.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <Controller.h>
#include <IContext.h>
#include <IPublisher.h>
#include <ICommandFactory.h>
#include <ICommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Controller::Controller(ICommandFactory* commandFactory) {
	this->commandFactory = commandFactory;
	pthread_mutex_init(&this->mutex, NULL);
}

Controller::~Controller() {
	this->clear();
	pthread_mutex_destroy(&this->mutex);
}

void Controller::lock() {
	pthread_mutex_lock(&this->mutex);
}

void Controller::unlock() {
	pthread_mutex_unlock(&this->mutex);
}

void Controller::clear() {
	this->lock();
	ICommand* command;
	while(!this->queue.empty()) {
		command = this->dequeue();
		this->commandFactory->destroy(command);
	}
	this->unlock();
}

void Controller::enqueue(ICommand* command) {
	this->queue.push(command);
}

ICommand* Controller::dequeue() {
	if (!this->queue.empty()) {
		ICommand* command = this->queue.front();
		this->queue.pop();
		return command;
	}
	return 0;
}

void Controller::execute(ICommand* command) {
	command->ackInProgress();
	command->execute();
	command->ackComplete();
	this->commandFactory->destroy(command);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
