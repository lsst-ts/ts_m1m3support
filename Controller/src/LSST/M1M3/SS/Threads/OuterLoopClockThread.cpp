/*
 * OuterLoopClockThread.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <OuterLoopClockThread.h>
#include <ICommandFactory.h>
#include <IController.h>
#include <IFPGA.h>
#include <ICommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OuterLoopClockThread::OuterLoopClockThread(ICommandFactory* commandFactory, IController* controller, IFPGA* fpga) {
	this->commandFactory = commandFactory;
	this->controller = controller;
	this->fpga = fpga;
	this->keepRunning = true;
	pthread_mutex_init(&this->updateMutex, NULL);
}

OuterLoopClockThread::~OuterLoopClockThread() {
	pthread_mutex_destroy(&this->updateMutex);
}

void OuterLoopClockThread::run() {
	while(this->keepRunning) {
		this->fpga->waitForOuterLoopClock(-1);
		this->controller->lock();
		if (this->keepRunning) {
			this->controller->enqueue(this->commandFactory->create(Commands::UpdateCommand, &this->updateMutex));
		}
		this->controller->unlock();
		pthread_mutex_lock(&this->updateMutex);
		pthread_mutex_unlock(&this->updateMutex);
		this->fpga->ackOuterLoopClock();
	}
}

void OuterLoopClockThread::stop() {
	this->keepRunning = false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
