/*
 * Controller.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <Controller.h>
#include <Context.h>
#include <CommandFactory.h>
#include <Command.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Controller::Controller(CommandFactory* commandFactory) {
	Log.Debug("Controller: Controller()");
	this->commandFactory = commandFactory;
	pthread_mutex_init(&this->mutex, NULL);
}

Controller::~Controller() {
	this->clear();
	pthread_mutex_destroy(&this->mutex);
}

void Controller::lock() {
	Log.Trace("Controller: lock()");
	pthread_mutex_lock(&this->mutex);
}

void Controller::unlock() {
	Log.Trace("Controller: unlock()");
	pthread_mutex_unlock(&this->mutex);
}

void Controller::clear() {
	Log.Trace("Controller: clear()");
	this->lock();
	Command* command;
	while(!this->queue.empty()) {
		command = this->dequeue();
		this->commandFactory->destroy(command);
	}
	this->unlock();
}

void Controller::enqueue(Command* command) {
	Log.Trace("Controller: enqueue()");
	this->queue.push(command);
}

Command* Controller::dequeue() {
	Log.Trace("Controller: dequeue()");
	if (!this->queue.empty()) {
		Command* command = this->queue.front();
		this->queue.pop();
		return command;
	}
	return 0;
}

void Controller::execute(Command* command) {
	Log.Trace("Controller: execute()");
	command->ackInProgress();
	command->execute();
	command->ackComplete();
	this->commandFactory->destroy(command);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
