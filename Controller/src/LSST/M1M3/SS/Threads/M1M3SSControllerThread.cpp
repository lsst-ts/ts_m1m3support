/*
 * M1M3SSControllerThread.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSControllerThread.h>
#include <IController.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSControllerThread::M1M3SSControllerThread(IController* controller) {
	this->controller = controller;
	this->keepRunning = true;
}

M1M3SSControllerThread::~M1M3SSControllerThread() {
	// TODO Auto-generated destructor stub
}

void M1M3SSControllerThread::run() {
	while(this->keepRunning) {
		this->controller->lock();
		ICommand* command = this->controller->dequeue();
		this->controller->unlock();
		if (command) {
			this->controller->execute(command);
		}
		else {
			usleep(100);
		}
	}
}

void M1M3SSControllerThread::stop() {
	this->keepRunning = false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
