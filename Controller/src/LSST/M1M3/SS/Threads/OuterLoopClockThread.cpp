/*
 * OuterLoopClockThread.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <OuterLoopClockThread.h>
#include <CommandFactory.h>
#include <Controller.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <Command.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OuterLoopClockThread::OuterLoopClockThread(CommandFactory* commandFactory, Controller* controller, IFPGA* fpga, M1M3SSPublisher* publisher) {
	this->commandFactory = commandFactory;
	this->controller = controller;
	this->fpga = fpga;
	this->publisher = publisher;
	this->keepRunning = true;
	pthread_mutex_init(&this->updateMutex, NULL);
}

OuterLoopClockThread::~OuterLoopClockThread() {
	pthread_mutex_destroy(&this->updateMutex);
}

void OuterLoopClockThread::run() {
	Log.Info("OuterLoopClockThread: Start");
	while(this->keepRunning) {
		if (this->fpga->waitForOuterLoopClock(1000) == 0) {
			this->controller->lock();
			if (this->keepRunning) {
				this->controller->enqueue(this->commandFactory->create(Commands::UpdateCommand, &this->updateMutex));
			}
			this->controller->unlock();
			pthread_mutex_lock(&this->updateMutex);
			pthread_mutex_unlock(&this->updateMutex);
			this->fpga->ackOuterLoopClock();
		}
		else {
			Log.Warn("OuterLoopClockThread: Failed to receive outer loop clock");
		}
	}
	Log.Info("OuterLoopClockThread: Completed");
}

void OuterLoopClockThread::stop() {
	this->keepRunning = false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
