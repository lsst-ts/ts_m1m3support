/*
 * ControllerThread.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <ControllerThread.h>
#include <Controller.h>
#include <unistd.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ControllerThread::ControllerThread(Controller* controller) {
    this->controller = controller;
    this->keepRunning = true;
}

void ControllerThread::run() {
    Log.Info("ControllerThread: Start");
    while (this->keepRunning) {
        this->controller->lock();
        Command* command = this->controller->dequeue();
        this->controller->unlock();
        if (command) {
            this->controller->execute(command);
        } else {
            usleep(100);
        }
    }
    Log.Info("ControllerThread: Completed");
}

void ControllerThread::stop() {
    this->keepRunning = false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
